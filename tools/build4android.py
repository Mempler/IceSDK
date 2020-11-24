#!/bin/python3
# Compile script to compile all binaries at once!

import os
import shutil
import subprocess
import argparse

from pathlib import Path

CPUs = ["armv7a", "aarch64", "i686", "x86_64"]
CPUMap = {
    CPUs[0]: "armeabi-v7a",
    CPUs[1]: "arm64-v8a",
    CPUs[2]: "x86",
    CPUs[3]: "x86_64",
}

parser = argparse.ArgumentParser(
    description='Compile all android CPU targets at once.')

parser.add_argument('android_ndk_target', type=int)
parser.add_argument('android_ndk_path', type=str)
parser.add_argument('android_ndk_bundle_path', type=str)
parser.add_argument('project', type=str)
args = parser.parse_args()


def get_args_gn(cpu):
    return f"""
is_clang = true
use_lld = true

android_ndk_target = "{args.android_ndk_target}"
android_ndk_path = "{args.android_ndk_path}"

clang_base_path = "$android_ndk_path/toolchains/llvm/prebuilt/windows-x86_64"

target_os = "android"
target_cpu = "{cpu}"
"""


def main():
    script_path = os.path.dirname(os.path.realpath(__file__))
    # TODO: add linux support
    gn_path = os.path.join(script_path, "win/gn.exe")

    for cpu in CPUs:
        arch_name = CPUMap[cpu]
        ndk_libcpp_path = os.path.join(
            args.android_ndk_bundle_path, f"sources/cxx-stl/llvm-libc++/libs/{arch_name}")

        # MKDIR app/src/main/jniLibs
        os.makedirs(f"Android/app/src/main/jniLibs/{arch_name}", exist_ok=True)
        os.makedirs(f"build/{arch_name}",  exist_ok=True)

        with open(f"build/{arch_name}/args.gn", "w") as f:
            f.write(get_args_gn(cpu))

        process = subprocess.Popen([gn_path, "gen", f"build/{arch_name}"],
                                   shell=True)
        process.wait()

        process = subprocess.Popen(
            ["ninja", "-C", f"build/{arch_name}", f"{args.project}"],
            shell=True)
        process.wait()

        shutil.copyfile(f"build/{arch_name}/lib{args.project}.so",
                        f"../IceSDK/Android/IceSDK/app/src/main/jniLibs/{arch_name}/libmain.so")

        shutil.copyfile(f"{ndk_libcpp_path}/libc++_shared.so",
                        f"../IceSDK/Android/IceSDK/app/src/main/jniLibs/{arch_name}/libc++_shared.so")

        shutil.copyfile(f"../third-party/sdl2/lib/android/{cpu}/libhidapi.so",
                        f"../IceSDK/Android/IceSDK/app/src/main/jniLibs/{arch_name}/libhidapi.so")

        shutil.copyfile(f"../third-party/sdl2/lib/android/{cpu}/libSDL2.so",
                        f"../IceSDK/Android/IceSDK/app/src/main/jniLibs/{arch_name}/libSDL2.so")


if __name__ == "__main__":
    main()
