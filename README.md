<img src="./.assets/logo-primary.svg" width="512px"/>

# IceSDK

IceSDK is a cold little Game SDK to create games. You can use individuel parts or
just use it as a Game Engine itself.

## How to setup

Setting things up is quite easy, you simply go ahead and run following commands:

```shell
# Clone the repository
git clone https://github.com/IceSDK/IceSDK

# Create a directory && go into it
mkdir build && cd build

# Generate CMake build files
cmake .. -GNinja # we recommend ninja generator since it's much faster
```

### NixOS

NixOS is a bit werid though. \
just enter `nix-shell` and run the commands above

## Supported Platforms

Current supported platforms:
iOS/MacOS aren't currently supported since i lack those devices.

| ![Windows](./.assets/windows_64x64.png) | ![Linux](./.assets/linux_64x64.png) | ![osX](./.assets/osx_64x64.png) | ![android](./.assets/android_64x64.png) | ![ios](./.assets/ios_64x64.png) | ![web](./.assets/wasm_64x64.png) |
| --------------------------------------- | ----------------------------------- | ------------------------------- | --------------------------------------- | ------------------------------- | -------------------------------- |
| :heavy_check_mark:                      | :heavy_check_mark:                  | :x:                             | :x:                                     | :x:                             | :x:                              |

## Compile Status

| Platform   | Windows                                                                                      | Linux                                                                                      | MacOS |
| ---------- | -------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------ | ----- |
| PC         | ![GitHub Workflow Status](https://github.com/IceSDK/IceSDK/workflows/PC%20Windows/badge.svg)   | ![GitHub Workflow Status](https://github.com/IceSDK/IceSDK/workflows/PC%20Linux/badge.svg)   | :x:   |
| Android    | :x:                                                                                          | :x:                                                                                        | :x:   |
| iOS        | :x:                                                                                          | :x:                                                                                        | :x:   |
| Emscripten | :x:                                                                                          | :x:                                                                                        | :x:   |

## License

IceSDK's code is licensed under the [MIT licence](https://opensource.org/licenses/MIT). Please see [the licence file](./LICENSE) for more information. [tl;dr](https://tldrlegal.com/license/mit-license) you can do whatever you want as long as you include the original copyright and license notice in any copy of the software/source.
