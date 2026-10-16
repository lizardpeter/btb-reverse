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
- recovered 0x30-byte piece runtime/state model
- drag/drop and accepted/rejected placement flow
- Bob/Ellis 131-frame sprite-sheet animation controller
- exact six character-animation frame ranges and interaction reaction routing

Next, this module will be expanded with persistent progress/completion behavior and a thin DirectDraw-facing rendering adapter. Shared DirectDraw, printing, Bink, input, and sound mechanisms remain documented separately so activity logic does not duplicate platform code.

## Original native Windows game session (development mode)

btb_game now has two distinct Windows host paths:

- Default: the native original-source host. The older Dinosaur GDI preview requires `--legacy-dino-preview`.
- **Native original-source host:** the reconstructed GameRoot and NativeGameSession drive a real DirectDraw7 640x480 window and an 80-slot DirectSound8 device. Run the Windows x86 build with the installed game's original files:

    .\btb_game.exe --native-original "C:\path\to\Bob Builds a Park"

The directory must contain original loaddata/NumUiHotArea.txt, uiHotArea.txt, uiHotAreaReplace.txt, uiBitmapName.txt, startupmovie.txt and binkwalk.txt, the original managed-sound binklist.txt and the Data/ bitmap/WAV resources. No game files or replacement images are embedded. The development host initially uses original profile slot 1 until the native five-sign profile selector is reconstructed.

The host installs source-backed Activity Select, seven chooser/replay screens and ten original pregame screens using their distinct bitmap table offsets. It installs native Dinosaur, Golf, Fireworks, Bob's Band and Park Designer drivers, then supplies real mouse and managed-sound observations to the recovered outer-state machine. Dinosaur runs the recovered 00409E00 controller against all nine original levels; completedmovie.txt and sceneset.bik are connected to their original outer states. Generic Yes/No modals render the activity underlay and original button art before returning the choice to the activity.

**Unfinished native dependencies are explicit failure conditions, not retail parity:**

- The Windows x86 native host now decodes original global and walkthrough Bink movies with the installed `Exe/binkw32.dll` and submits decoded frames through DirectDraw7. Bink playback, audio and inset placement still require retail differential validation; the x64 host cannot load the original 32-bit DLL.
- Park Designer requires its unreconstructed 0x0040E440 editor/render service. Herding requires the original continuous motion/collision provider.
- Other minigame initializers and the original profile-entry/sign workflow remain incomplete.
- No original executable differential or ABI-equivalence verification is claimed. Source-written is not equivalent to compiled/linked/retail-verified.

All production src/*.cpp files are now checked for build-target attachment. CI compiles Linux, Windows x64 and Windows x86, runs executable gameplay integration tests and verifies native DirectDraw/DirectSound/Bink-provider linkage.
