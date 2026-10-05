# Clean-room reconstruction

This directory contains source-level reconstructions derived from observed game behavior, static analysis, and the external data formats.

It does **not** contain original game code or proprietary assets.

The goal is to move progressively from:

`binary evidence -> named structures/algorithms -> buildable source-level equivalent`

The first buildable module is the Dinosaur level-data layer.

## Build

```bash
cmake -S reconstruction -B build/reconstruction
cmake --build build/reconstruction
ctest --test-dir build/reconstruction --output-on-failure
```

The code currently covers:

- exact species/difficulty level indexing
- `dino.txt` parsing
- variable-length post-piece coordinate tail
- permutation validation
- the recovered Dino cursor proximity and rectangle hit tests

The DirectDraw-backed activity runtime will be added separately so the portable data/gameplay model stays independent of the original Windows rendering layer.
