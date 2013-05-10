python waf distclean
python waf configure --ndk C:\android-ndk-r8e
rem python waf configure
python waf build_debug_windows_x86_msvc
python waf build_debug_windows_x86_gcc
python waf build_debug_browser_js_emcc
python waf build_debug_android9_armv7_ndk
