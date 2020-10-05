## How to setup
```sh
git clone https://github.com/Mempler/IceSDK


# --ide=(`vscode`, `vs2017`, `vs2019`) --target?=(`android`, `emscripten`)
python3 ./tools/setup.py --ide=vscode
```

## Android
```
.\tools\win\gn.exe gen out
cd out

python3 ..\tools\build4android.py 30 %LOCALAPPDATA%\Android\Sdk\ndk\21.3.6528147 %LOCALAPPDATA%\Android\Sdk\ndk-bundle BareBones

# Open in Android Studio && Run
```

## WebAssembly
args.gn
```gn
is_clang = true
use_lld = true

clang_base_path = "$HOME/emsdk/upstream"

target_os = "emscripten"
target_cpu = "wasm32"
```

## Supported Platforms


| ![Windows](./.assets/windows_64x64.png) | ![Linux](./.assets/linux_64x64.png) | ![osX](./.assets/osx_64x64.png) | ![android](./.assets/android_64x64.png) | ![ios](./.assets/ios_64x64.png) | ![web](./.assets/wasm_64x64.png) |
| --------------------------------------- | ----------------------------------- | ------------------------------- | --------------------------------------- | ------------------------------- | -------------------------------- |
| :heavy_check_mark:                      | :heavy_check_mark:                  | :x:                             | :heavy_check_mark:                      | :x:                             | :heavy_check_mark:               |

## Compile Status

| Platform   | Windows                                                                                             | Linux                                                                                               | MacOS |
| ---------- | --------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- | ----- |
| PC         | ![GitHub Workflow Status](https://github.com/Mempler/IceSDK/workflows/PC%20Windows/badge.svg)       | ![GitHub Workflow Status](https://github.com/Mempler/IceSDK/workflows/PC%20Linux/badge.svg)         | :x:   |
| Android    | ![GitHub Workflow Status](https://github.com/Mempler/IceSDK/workflows/Win32%20Android/badge.svg)    | ![GitHub Workflow Status](https://github.com/Mempler/IceSDK/workflows/Linux%20Android/badge.svg)    | :x:   |
| iOS        | :x:                                                                                                 | :x:                                                                                                 | :x:   |
| Emscripten | ![GitHub Workflow Status](https://github.com/Mempler/IceSDK/workflows/Win32%20Emscripten/badge.svg) | ![GitHub Workflow Status](https://github.com/Mempler/IceSDK/workflows/Linux%20Emscripten/badge.svg) | :x:   |
