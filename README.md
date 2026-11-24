# btb-reverse

Reverse-engineering workspace for **Bob the Builder: Bob Builds a Park** (Windows, 2002).

## Original function reconstruction coverage

**615/615 (100%)** of the original PE32 functions are now recorded at
the **compiles** stage in the reviewed C++26 source inventory; **0 remain**
unreviewed or architecture-only. Every compile-stage function is associated
with a C++ source file, validation test, and evidence record. This count is
maintained in [the generated function summary](records/summary.json),
[the reviewed source ledger](records/reviewed_functions.csv),
and [the full 615-function architecture inventory](docs/architecture-615-functions.md).

**100% function source coverage is not 100% runtime or binary equivalence.**
Windows activity/device integration, real Bink playback, original ABI parity,
and complete start-to-finish retail playthrough certification are tracked
separately.

The full-function inventory still records **0/615 functions as fully
retail-behavior-verified**. This is a distinct certification threshold, not
20 missing C++ function models. Runtime integration, differential testing,
and the complete playable original-game flow remain separate milestones.
See [the compiled movement and NPC batch](docs/original-motion4.md),
[activity coordinators](docs/original-activity4.md),
and [drawing coordinators](docs/original-draw4.md).

## Source material

The original game files are intentionally **not committed** to this repository. Point the tooling at a local copy of the installed game or CD contents.

Verified primary game executable:

- path: `Exe/Bob the Builder - Bob Builds a Park.exe`
- size: `311296` bytes
- SHA-256: `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`
- format: PE32 / i386 / Windows GUI
- linker: Microsoft 6.0-era
- timestamp: 2002-08-06 13:29:47 UTC
- image base: `0x00400000`
- entry point: `0x00430FEA`

The 45,056-byte root-level `Bob the Builder - Bob Builds a Park.exe` is a launcher/AutoMenu executable, not the main game.

## Repository layout

- `src/` — faithful original game and native Bink decoder reconstruction, including reconstructed gameplay coordinators.
- `include/btb/` — shared source interfaces and generated original-function registries.
- `runtime/` — native executable entrypoints and platform/device adapters (Win32, DirectDraw, DirectSound, movie surfaces, printing).
- `enhanced/` — opt-in improvements, separate from faithful behavior; currently a baseline-only build target.
- `records/` — canonical reviewed function ledger and generated status, work queue and dependency graph.
- `tests/` — portable and native source regression suites.
- `tools/` — evidence generators, scanners and native diagnostics (`tools/native/`).
- `analysis/` and `ghidra/` — original-executable evidence, architecture reports and reverse-engineering tooling.
- `docs/` — technical notes and integration documentation.
- `cmake/` — one shared build graph; `reconstruction/` is a compatibility CMake entrypoint only.

## Faithful and enhanced builds

The default build is faithful and never compiles enhancements into `btb_game`.
Use an independent build directory when enabling the enhanced variant.

```bash
cmake -S . -B build/faithful
cmake --build build/faithful --target btb_game
cmake -S . -B build/enhanced -DBTB_BUILD_ENHANCED=ON
cmake --build build/enhanced --target btb_game_enhanced
```

For Visual Studio, add `-A x64` to configure and `--config Release` to build.
Both launchers use the original installed data; original EXE or Bink DLL
are not required by the native x64 path. Enhanced currently shares the
faithful implementation and adds no new gameplay features. Future improvements
must be explicit opt-in changes under `enhanced/`, with the faithful build
and its regression suite preserved. Fixes that restore original behavior
belong in `src/` or the platform adapter in `runtime/`, not in enhanced.

See [source layout policy](docs/source-layout.md) and [AGENTS.md](AGENTS.md).

## First scan

Run:

```bash
python tools/inventory.py "/path/to/BTB-BBP" --out analysis/generated/inventory.json
python tools/exe_strings.py "/path/to/BTB-BBP/Exe/Bob the Builder - Bob Builds a Park.exe" --out analysis/generated/exe-strings.json
```

On Windows PowerShell:

```powershell
python tools/inventory.py "C:\path\to\BTB-BBP" --out analysis/generated/inventory.json
python tools/exe_strings.py "C:\path\to\BTB-BBP\Exe\Bob the Builder - Bob Builds a Park.exe" --out analysis/generated/exe-strings.json
```

## Current reversing direction

The main executable is small and unprotected enough that the plan is to recover it subsystem-by-subsystem:

1. startup / WinMain / main state machine
2. file/data-table loader
3. UI and bitmap/surface manager
4. DirectDraw renderer
5. DirectInput input layer
6. DirectSound/WAV layer
7. Bink movie layer
8. individual subgames and park designer
9. reconstruct original source-level structures, names, constants, and control flow
10. build a faithful source reconstruction of the original game

See `docs/initial-analysis.md` for the first verified anchors.

## First integrated game executable

The root CMake project builds `btb_game` and the
native Windows C++26 host, with a shared game-flow dispatcher and drivers
registered for **all ten activities**. That does not imply all ten have
completed a real on-device playthrough: the five-stream Spud Skate backend, native Fireworks bank and global/movie
walkthrough providers now have source-reconstructed paths, but still require
whole-installation media and live-device certification.
The compiled-source milestone is complete; runtime certification is not.

Build/run instructions and current scope: [Integrated C++26 executable](docs/integrated-cpp26-build.md).

Successful GitHub Actions Windows runs publish the compiled, asset-free
`btb-game-win64-preview` executable artifact. Use the original installed game
data with `btb_game.exe --native-original <root>`; the x64 movie path is native
and fail-closed, not a fallback to the retail DLL.

Run the [whole-installation Bink audit](docs/bink32-main-codec-integration.md)
to identify media that fails native decoding before running the full game.

## Ongoing full-game source recovery

The next source reconstruction milestone includes the formerly missing
[Bob's Band music sequencer](docs/bobs-band-activity.md):
five machine channels, the five-by-24 composition wall, ten note types,
three conductors, editor transitions, playback, and all fifty pitch-bank
sample mappings. This module is now **source-integrated with the common 68-state game
coordinator** (including original conductor selection and binary sequence
saving). The native host also has source-backed profile creation, five-slot
profile persistence, original \`videoseq.txt\` startup sequencing, and the
\`sceneset.bik\` transition wired through the global movie provider. These are
compiled/tested integration milestones, not a claim of retail-equivalent
gameplay. Golf has a source-integrated round driver, and the original seven
chooser/replay pairs route their binary-verified negative menu actions in the
shared coordinator.

See [full-game integration](docs/full-game-integration.md) and
[retail menu action evidence](docs/full-game-menu-actions.md).

## Full-game instruction and movie source recovery

The complete set of **ten original instruction/pregame setup-update
pairs** is now modeled in the source-level 68-state `GameRoot`
coordinator. They have their exact original six-way action tables,
parent Back targets, first-run difficulty writes, special Spud Skate
start branch, original BMP background slots, ten Bink walkthroughs,
and the four separately opened global intro movies.

The remaining native-host work is runtime validation of original assets,
all ten activity flows, movie presentation/timing, audio, and screen-specific
behavior. The registered adapters are not proof of complete playthroughs.

See [full pregame reverse-engineering evidence](docs/full-game-pregame.md)
and [full-game integration gate](docs/full-game-integration.md).
