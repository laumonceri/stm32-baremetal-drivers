#!/usr/bin/env python3
"""Convert a photo into a 128x128 RGB565 C header for ST7735_DrawImage.

Center-crops the image to a square (so nothing gets stretched), resizes it
to 128x128, and quantizes each pixel to RGB565.

Usage:
    python3 img_to_rgb565.py <input_image> [-o output.h] [-n array_name]

Example:
    python3 img_to_rgb565.py ~/Downloads/tulips.jpg -o tulips_image.h -n tulips_image
"""

import argparse
import os
from PIL import Image

SIZE = 128


def rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def convert(src_path, dst_path, array_name):
    im = Image.open(src_path).convert("RGB")
    w, h = im.size
    side = min(w, h)
    left = (w - side) // 2
    top = (h - side) // 2
    im = im.crop((left, top, left + side, top + side))
    im = im.resize((SIZE, SIZE), Image.LANCZOS)

    pixels = list(im.getdata())
    guard = os.path.basename(dst_path).upper().replace(".", "_").replace("-", "_")

    lines = []
    lines.append(f"#ifndef {guard}")
    lines.append(f"#define {guard}")
    lines.append("")
    lines.append("#include <stdint.h>")
    lines.append("")
    lines.append(f"/* {SIZE}x{SIZE} RGB565, row-major. Generated from {src_path}. */")
    lines.append(f"static const uint16_t {array_name}[{SIZE} * {SIZE}] = {{")
    for row in range(SIZE):
        row_pixels = pixels[row * SIZE:(row + 1) * SIZE]
        vals = ", ".join(f"0x{rgb565(r, g, b):04X}U" for r, g, b in row_pixels)
        lines.append("  " + vals + ",")
    lines.append("};")
    lines.append("")
    lines.append(f"#endif /* {guard} */")

    with open(dst_path, "w") as f:
        f.write("\n".join(lines) + "\n")

    print(f"wrote {dst_path} ({array_name}[{SIZE * SIZE}])")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", help="Source image file (jpg/png/etc)")
    parser.add_argument("-o", "--output", help="Output header path (default: <input basename>_image.h)")
    parser.add_argument("-n", "--name", help="C array name (default: <input basename>_image)")
    args = parser.parse_args()

    base = os.path.splitext(os.path.basename(args.input))[0]
    dst_path = args.output or f"{base}_image.h"
    array_name = args.name or f"{base}_image"

    convert(args.input, dst_path, array_name)


if __name__ == "__main__":
    main()
