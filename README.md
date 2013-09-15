oak
===

Cross-platform lightweight game engine. Oak is written in C++, and currently supports Android,
Windows, and Browsers (through Emscripten).

License
-------

Oak is licensed under the MIT license, making it usable in both free and commercial projects.

Build
-----

Oak's build system is based on [Waf](http://code.google.com/p/waf/), so you need a
decently recent version of Python 2.x installed.

### Windows - Visual C++

From a VS command prompt:
```sh
cd build
python waf configure
python waf build_release_windows_x86_msvc
```

### Windows - MinGW

Having the MinGW bin directory in your PATH may help ;)

```sh
cd build
python waf configure
python waf build_release_windows_x86_gcc
```

### Linux (work in progress, not yet officially supported)

You need to install libX11-dev and libgl1-mesa-dev, then build as usual:
```sh
sudo apt-get install libX11-dev libgl1-mesa-dev
cd build
python waf configure
python waf build_release_linux_x86_gcc
```

### Android

You need the Android NDK installed and must specifiy its root to the configure step

```sh
cd build
python waf configure --ndk <NDK-ROOT>
python waf build_release_android9_armv7_ndk
```

### Browser

You need LLVM + Clang + Emscripten to be installed and available in your PATH.

```sh
cd build
python waf configure
python waf build_release_browser_js_emcc
```

### Everything, all at once

Configure can be run only once if you need to build multiple variants, so this would be valid:

```sh
cd build
python waf configure --ndk <NDK-ROOT>
python waf build_release_windows_x86_msvc
python waf build_release_windows_x86_msvc
python waf build_release_windows_x86_gcc
python waf build_release_browser_js_emcc
python waf build_release_android9_armv7_ndk
```
