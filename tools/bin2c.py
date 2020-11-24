import os
import sys

if len(sys.argv) < 2:
    print("Usage: python bin2c.py <InputFile>")
    exit(0)

INPUT_PATH = sys.argv[1]
INPUT_PATH_NO_EXT = os.path.splitext(os.path.basename(INPUT_PATH))[0]
INPUT_PATH_EXT = os.path.splitext(os.path.basename(INPUT_PATH))[1]
OUTPUT_PATH = INPUT_PATH+".h"

INPUT_FILE = open(INPUT_PATH, 'rb')
OUTPUT_FILE = open(OUTPUT_PATH, 'w')

OUTPUT_FILE.write("""\
#pragma once

#include <cstdint>

""")

BIN_LEN = os.path.getsize(INPUT_PATH)

OUTPUT_ARR = f"const size_t {INPUT_PATH_NO_EXT}_{INPUT_PATH_EXT.replace('.', '')}_len = {BIN_LEN};\n"
OUTPUT_ARR += f"const uint8_t {INPUT_PATH_NO_EXT}_{INPUT_PATH_EXT.replace('.', '')}[{BIN_LEN}] = {{\n    "

for i in range(BIN_LEN):
    OUTPUT_ARR += hex(INPUT_FILE.read(1)[0])
    OUTPUT_ARR += ','

    if i % 15 == 14:
        OUTPUT_ARR += '\n    '

OUTPUT_ARR.strip(',')
OUTPUT_ARR += f"\n}};\n"

OUTPUT_FILE.write(OUTPUT_ARR)
