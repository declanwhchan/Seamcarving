# Seam Carving in C

This repository contains a simple C implementation of vertical seam carving for content-aware image resizing.

The program:

- reads a binary RGB image from `HJoceanSmall.bin`
- computes an energy map for the image
- finds the minimum-energy vertical seam using dynamic programming
- removes one seam at a time
- writes the resized results to `img0.bin` through `img4.bin`

## What Is Seam Carving?

Seam carving is an image resizing technique that removes low-energy paths of pixels instead of cropping or uniformly scaling the entire image. This helps preserve visually important content while shrinking the image.

In this project, a **vertical seam** is removed repeatedly. Each seam contains exactly one pixel from every row, and adjacent seam pixels can only shift by at most one column.

## Files

- `seamcarving.c` - main program and seam carving logic
- `seamcarving.h` - function declarations for seam carving
- `c_img.c` - image helper functions for reading, writing, and editing pixels
- `c_img.h` - image struct and helper declarations
- `HJoceanSmall.bin` - sample input image
- `img0.bin` to `img4.bin` - sample outputs after successive seam removals

## How It Works

The pipeline in `main()` is:

1. Read the input image from `HJoceanSmall.bin`.
2. Compute an energy image with `calc_energy()`.
3. Build the cumulative minimum seam-cost table with `dynamic_seam()`.
4. Recover the minimum-energy path with `recover_path()`.
5. Remove that seam with `remove_seam()`.
6. Write the result to disk.
7. Repeat 5 times.

## Build

### Windows (MinGW / GCC)

```bash
gcc seamcarving.c -o seamcarving.exe
```

### macOS / Linux

```bash
gcc seamcarving.c -lm -o seamcarving
```

## Important Build Note

`seamcarving.c` directly includes `c_img.c` with:

```c
#include "c_img.c"
```

Because of that, compile `seamcarving.c` by itself.

Do **not** compile with both source files like this:

```bash
gcc seamcarving.c c_img.c -o seamcarving
```

That would likely cause duplicate symbol / multiple definition errors.

## Run

### Windows

```bash
.\seamcarving.exe
```

### macOS / Linux

```bash
./seamcarving
```

The program prints the seam-removal iteration number:

```text
i = 0
i = 1
i = 2
i = 3
i = 4
```

It also writes these output files:

- `img0.bin`
- `img1.bin`
- `img2.bin`
- `img3.bin`
- `img4.bin`

Each output image is one pixel narrower than the previous one.

## Binary Image Format

The helper functions in `c_img.c` use a custom binary image format:

- first 2 bytes: image height
- next 2 bytes: image width
- remaining bytes: raw RGB pixel data

Pixels are stored in row-major order, with 3 bytes per pixel:

- red
- green
- blue

The height and width are written as 2-byte big-endian integers.

## Current Limitations

- The input filename is hardcoded as `HJoceanSmall.bin`.
- The program always removes exactly 5 seams.
- There is no command-line interface yet.
- Output files are written in the same custom `.bin` format, not PNG or JPEG.

## Possible Improvements

- accept input/output filenames as command-line arguments
- allow the user to choose how many seams to remove
- support horizontal seam carving
- export to standard image formats using an image library
- separate `c_img.c` from `seamcarving.c` at compile time for a cleaner build setup

## Summary

This project is a compact example of seam carving in C using:

- image energy computation
- dynamic programming
- path recovery by backtracking
- repeated image reconstruction