#!/usr/bin/python

import os
import sys
import argparse
import tempfile
import shutil
import subprocess

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

# find app template
scriptDir = os.path.abspath(os.path.dirname(os.path.realpath(__file__)))
templateDir = os.path.join(scriptDir, "template")

# create a temp directory for the new app
tempDir = os.path.abspath(tempfile.mkdtemp())
appDir = os.path.join(tempDir, "app")

# copy all the base template
shutil.copytree(templateDir, appDir)

# copy game content (there are issues with files at the root of
# the assets/ folder, so we always create a "data" subdirectory)
shutil.copytree(gameDir, os.path.join(appDir, "assets", "data"))

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
apkFilename = os.path.join(appDir, "bin", "OakApp-debug.apk")
shutil.copy2(apkFilename, outputDir)

# run on device
if args.run:
    print("Running game on device")
    subprocess.call(["adb", "shell", "am", "start", "com.tabsorspaces.oakapp/android.app.NativeActivity"], shell = True)

# delete everything temporary
os.chdir(outputDir)
shutil.rmtree(tempDir)
