#! /usr/bin/env python
# encoding: utf-8

import os,sys
from waflib import Configure,Options,Utils
from waflib.Tools import ccroot
from waflib.Configure import conf
@conf
def find_emxx(conf):
	cxx=conf.find_program(['em++'], var = "CXX")
	cxx=conf.cmd_to_list(cxx)
	conf.env.CXX_NAME='emcc'
	conf.env.CXX=cxx
@conf
def emxx_common_flags(conf):
	v=conf.env
	v['CXX_SRC_F']=[]
	v['CXX_TGT_F']=['-c','-o']
	if not v['LINK_CXX']:v['LINK_CXX']=v['CXX']
	v['CXXLNK_SRC_F']=[]
	v['CXXLNK_TGT_F']=['-o']
	v['CPPPATH_ST']='-I%s'
	v['DEFINES_ST']='-D%s'
	v['LIB_ST']='-l%s'
	v['LIBPATH_ST']='-L%s'
	v['STLIB_ST']='-l%s'
	v['STLIBPATH_ST']='-L%s'
	v['RPATH_ST']='-Wl,-rpath,%s'
	v['SONAME_ST']='-Wl,-h,%s'
	v['SHLIB_MARKER']='-Wl,-Bdynamic'
	v['STLIB_MARKER']='-Wl,-Bstatic'
	v['cxxprogram_PATTERN']='%s'
	v['CXXFLAGS_cxxshlib']=['-fPIC']
	v['LINKFLAGS_cxxshlib']=['-shared']
	v['cxxshlib_PATTERN']='lib%s.so'
	v['LINKFLAGS_cxxstlib']=['-Wl,-Bstatic']
	v['cxxstlib_PATTERN']='lib%s.a'
	v['LINKFLAGS_MACBUNDLE']=['-bundle','-undefined','dynamic_lookup']
	v['CXXFLAGS_MACBUNDLE']=['-fPIC']
	v['macbundle_PATTERN']='%s.bundle'
@conf
def emxx_modifier_browser(conf):
	v=conf.env
	v['cxxprogram_PATTERN']='%s.html'
	v['cxxshlib_PATTERN']='%s.dll'
	v['implib_PATTERN']='lib%s.dll.a'
	v['IMPLIB_ST']='-Wl,--out-implib,%s'
	v['CXXFLAGS_cxxshlib']=[]
	v.append_value('LINKFLAGS',['-Wl,--enable-auto-import'])
@conf
def emxx_modifier_platform(conf):
	emxx_modifier_func=getattr(conf,'emxx_modifier_'+conf.env.TARGET_OS,None)
	if emxx_modifier_func:
		emxx_modifier_func()
def configure(conf):
	conf.find_emxx()
	conf.emxx_common_flags()
	conf.emxx_modifier_platform()
	conf.cxx_load_tools()
	conf.cxx_add_flags()
	conf.link_add_flags()
