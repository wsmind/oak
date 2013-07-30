#!/usr/bin/python

import os
import sys
import argparse
import tempfile
import shutil
import subprocess

# utility function to replace special tokens in files from config values
def applyConfig(filename, conf):
    # read everything
    file = open(filename)
    contents = file.read()
    file.close()
    
    # replace special values
    for (key, value) in conf.items():
        contents = contents.replace("${OAK:" + key + "}", value)
    
    # write the file again
    file = open(filename, "w")
    file.write(contents)
    file.close()

# command line arguments
parser = argparse.ArgumentParser(description = "Create an Android package for an Oak game")
parser.add_argument("--game", required = True, help = "path to the game folder (must contain main.lua)")
parser.add_argument("--engine", required = True, help = "engine to bundle with the game (.so)")
parser.add_argument("--out", help = "output folder for the apk (defaults to the current working directory)", default = os.getcwd())
parser.add_argument("--install", action = "store_true", help = "install the apk on the default adb device")
parser.add_argument("--run", action = "store_true", help = "run the apk on the default adb device")

args = parser.parse_args()

gameDir = os.path.abspath(args.game)
engineLib = os.path.abspath(args.engine)
outputDir = os.path.abspath(args.out)

# game sanity check
if not os.path.exists(os.path.join(gameDir, "main.lua")):
    sys.exit("The given game folder is not valid: no main.lua found")

# load android-specific game config (if any)
configPath = os.path.join(gameDir, "_platform", "android", "config.json")
jsonConfig = {}
if os.path.exists(configPath):
    import json
    configFile = open(configPath)
    jsonString = configFile.read()
    configFile.close()
    jsonConfig = json.loads(jsonString)

config = {}
config["appName"] = jsonConfig.get("appName", "OakApp")
config["packageName"] = jsonConfig.get("packageName", "com.example.oakapp")
config["label"] = jsonConfig.get("label", "Oak App")

# find app template
scriptDir = os.path.abspath(os.path.dirname(os.path.realpath(__file__)))
templateDir = os.path.join(scriptDir, "template")

# create a temp directory for the new app
tempDir = os.path.abspath(tempfile.mkdtemp())
appDir = os.path.join(tempDir, "app")

# copy all the base template
shutil.copytree(templateDir, appDir)

# apply configuration to the template (replace special tokens with their actual values)
applyConfig(os.path.join(appDir, "build.xml"), config)
applyConfig(os.path.join(appDir, "AndroidManifest.xml"), config)

# copy app icons (if any)
for resolution in ["ldpi", "mdpi", "hdpi", "xhdpi"]:
	iconPath = os.path.join(gameDir, "_platform", "android", "icon-" + resolution + ".png")
	if os.path.exists(iconPath):
		shutil.copy2(iconPath, os.path.join(appDir, "res", "drawable-" + resolution, "ic_launcher.png"))

# copy game content (there are issues with files at the root of
# the assets/ folder, so we always create a "data" subdirectory)
# platform-specific configuration is excluded, it should only contain
# metadata and not actual game data
shutil.copytree(gameDir, os.path.join(appDir, "assets", "data"), ignore = shutil.ignore_patterns("_platform"))

# copy engine .so
libDir = os.path.join(appDir, "libs", "armeabi")
os.makedirs(libDir)
shutil.copy2(engineLib, libDir)

# build package
os.chdir(appDir)
subprocess.call(["ant", "debug"], shell = True)

# install on device
if args.install:
    print("Installing apk on device")
    subprocess.call(["ant", "debug", "install"], shell = True)

# extract generated apk
apkFilename = os.path.join(appDir, "bin", config["appName"] + "-debug.apk")
if not os.path.exists(outputDir):
    os.makedirs(outputDir)
shutil.copy2(apkFilename, outputDir)

# run on device
if args.run:
    print("Running game on device")
    subprocess.call(["adb", "shell", "am", "start", config["packageName"] + "/android.app.NativeActivity"], shell = True)

# delete everything temporary
os.chdir(outputDir)
shutil.rmtree(tempDir)
