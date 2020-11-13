#!/bin/python

import os
import sys
import platform
import argparse

parser = argparse.ArgumentParser(
    description='Cross platform way to easily compile assets.')

parser.add_argument('--target', type=str)
parser.add_argument('--out', type=str)
args = parser.parse_args()


def find_platform():
    is_linux = platform.system() == "Linux"
    is_win = platform.system() == "Windows"

    host_platform = ""

    if is_linux:
        host_platform = "linux"
    elif is_win:
        host_platform = "win"

    return host_platform


def Main():
    current = os.path.dirname(__file__)
    old_path = os.getcwd()

    platform = find_platform()
    build_dir = args.out
    target = args.target

    executable = os.path.realpath("%s/%s/AssetCompiler" % (current, platform))

    print("%s %s %s/Assets" % (executable, "./Assets", build_dir))

    os.chdir(target)
    os.system("%s %s %s/Assets" % (executable, "./Assets", build_dir))
    os.chdir(old_path)


if __name__ == "__main__":
    Main()
