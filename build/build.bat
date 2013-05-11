rem python waf distclean
rem python waf configure --ndk C:\android-ndk-r8e
rem python waf configure
rem python waf build_debug_windows_x86_msvc
rem python waf build_debug_windows_x86_gcc
rem python waf -v build_debug_browser_js_emcc
python waf build_debug_android9_armv7_ndk
