# Source reconstruction

This directory contains **direct source-level reconstructions of the original game** derived from the executable, recovered data structures, external data files, API behavior, and decompiler/static-analysis evidence.

The objective is fidelity to the original implementation:

`binary evidence -> named original structures/functions -> compilable reconstructed source`

Where a function, structure, enum, constant, table, or state-machine behavior can be recovered, the reconstruction should preserve it rather than intentionally redesigning it.

The first buildable module is the Dinosaur level-data and gameplay layer.

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
- recovered Dino cursor proximity and rectangle hit tests

Next, this module will be expanded with the recovered original piece record, piece-state enum, drag/drop update loop, snap/return transitions, completion tracking, sound-event mapping, and DirectDraw-facing rendering behavior.
