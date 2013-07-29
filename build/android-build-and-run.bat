rem python waf distclean
rem python waf configure --ndk C:\android-ndk-r8e
python waf build_debug_android9_armv7_ndk
rem python waf build_release_android9_armv7_ndk
..\tools\packager-android\packager-android.py --game ../samples/hello --engine output/debug_android9_armv7_ndk/liboak.so --out output/apk --install --run
echo Press any key to kill the app
pause
adb shell am force-stop com.tabsorspaces.oakapp
