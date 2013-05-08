#! /usr/bin/env python
# encoding: utf-8

import os

from waflib.Configure import conf
def configure(conf):
	exeDir = os.path.join(conf.options.ndk, "toolchains", "arm-linux-androideabi-4.6", "prebuilt", "windows-x86_64", "bin")
	conf.find_program(['arm-linux-androideabi-ar'], var = "AR", path_list=[exeDir])
	conf.env.ARFLAGS='r'
