#!/bin/bash
# Setup script for IDEs

import subprocess
import platform
import argparse
import shutil
import os

# Supported IDEs
IDEs = [
    "vs2017",
    "vs2019",

    "vscode",
]

# Target platforms
Platforms = [
    "android",
    "emscripten"
]

parser = argparse.ArgumentParser(
    description='IDE config helper script.')

parser.add_argument('--ide', choices=IDEs, type=str, required=True)
parser.add_argument('--target', choices=Platforms, type=str, required=False)

args = parser.parse_args()


def SetupEmscripten(platform):
    ext = ""
    if platform == "win":
        ext = ".bat"

    process = ""

    # emsdk install latest
    subprocess.Popen([os.path.abspath(f'third-party/emsdk/emsdk{ext}'), "install", "latest"],
                     shell=True, stdout=subprocess.PIPE).wait()

    # emsdk activate latest
    subprocess.Popen([os.path.abspath(f'third-party/emsdk/emsdk{ext}'), "activate", "latest"],
                     shell=True, stdout=subprocess.PIPE).wait()


def GenerateARGS(platform, target):
    if not os.path.isdir("./out"):
        os.mkdir("./out")

    shutil.copyfile(f"./.ide/GN/{platform}/{target}/args.gn",
                    "./out/args.gn")

    subprocess.call([f"./tools/{platform}/gn",
                     "gen",
                     "out"])


def GenerateVSCode(platform, target):
    GenerateARGS(platform, target)
    if os.path.isdir("./.vscode"):
        shutil.rmtree("./.vscode")

    shutil.copytree(f"./.ide/vscode/{platform}/{target}", "./.vscode")


def GenerateVisualStudio(platform, target, version):
    GenerateARGS(platform, target)
    subprocess.call([f"./tools/{platform}/gn",
                     "gen",
                     "out", f"--ide=vs{version}"])


def main():
    is_linux = platform.system() == "Linux"
    is_win = platform.system() == "Windows"

    target_platform = args.target
    host_platform = ""

    if is_linux:
        host_platform = "linux"
    elif is_win:
        host_platform = "win"
    else:
        print("Unsupported platform!")
        return 1

    if target_platform == None:
        target_platform = "pc"

    if target_platform == "emscripten":
        SetupEmscripten(host_platform)

    if args.ide == "vscode":
        GenerateVSCode(host_platform, target_platform)
    elif args.ide.startswith("vs20"):
        GenerateVisualStudio(host_platform, target_platform,
                             args.ide.split("vs")[1])


if __name__ == "__main__":
    exit(main())
