# Integrated C++26 Windows executable (experimental)

The default Windows executable now hosts the reconstructed original 68-state
game flow. All ten activity drivers are registered. The native host uses the
installed retail data, DirectDraw/DirectSound providers, reconstructed profile
persistence, input, and native movie integration. This is not a claim of
finished retail-equivalent playthroughs.

## Build

Use a C++26-capable toolchain. The Windows CI builds both architectures using
Visual Studio 2026; Linux CI uses GCC 14.

```powershell
cmake -S . -B build/reconstruction-win -G "Visual Studio 18 2026" -A x64
cmake --build build/reconstruction-win --config Release --target btb_game
.\build\reconstruction-win\Release\btb_game.exe --native-original "C:\Games\BTB-BBP"
```

Or download the compiled, asset-free `btb-game-win64-preview` artifact from
a successful [repo-checks](https://github.com/lizardpeter/btb-reverse/actions/workflows/ci.yml)
run. Extract the executable and point it at your installed original game
directory, not at the launcher executable. No original game binaries or assets
are shipped in the artifact.

The x86 comparison build is configured with `-A Win32` in a separate build
directory. See [native Bink provider selection](bink32-native-windows-optin.md):
x64 selects the recovered codec by default, while x86 retains the retail
movie provider as a comparison mode.

## Running the reconstructed game

```powershell
.\btb_game.exe --native-original "C:\Games\BTB-BBP"
.\btb_game.exe --native-original "C:\Games\BTB-BBP" --skip-bink
.\btb_game.exe --recompiled-smoke
```

The optional `--skip-bink` development mode skips specific global movies and
walkthrough transitions; it is not normal retail playback. Native movie
failures are explicit in normal mode. `--profile-slot 1..5` selects an original
profile slot. The earlier standalone Dino preview is available only through
`--legacy-dino-preview`; it is not the normal full-game host.

The original-data directory must contain the retail resource tree. Preserve
its structure and use a backup when evaluating the preview, particularly
for original saved profiles. No on-device full-game certification is implied
by successful automated tests.

## Tests

```sh
CXX=g++-14 cmake -S . -B build/reconstruction
cmake --build build/reconstruction --parallel
ctest --test-dir build/reconstruction --output-on-failure
./build/reconstruction/btb_game --recompiled-smoke
```

Linux runs the portable/headless game and source contracts; the graphical
host is Windows-only. For focused source regressions:

```sh
cmake --build build/reconstruction --target btb_herding_source_regressions btb_full_game_source_regressions --parallel
ctest --test-dir build/reconstruction --output-on-failure -R 'herding_|full_game_'
```

## Remaining certification work

- Complete each activity on actual original resources, through its native
  setup, instruction, gameplay, completion, and replay/return transition.
- Exercise all five profile slots and progress persistence across sessions.
- Audit every installed Bink stream, including audio-bearing globals,
  separate walkthrough narration, Spud Skate and Fireworks banks.
- Verify real DirectDraw/DirectSound output and input on Windows.
- Differentially compare behavior against the original PE32. The 615/615
  compile-stage function inventory is not a retail equivalence metric.

The [full-game integration gate](full-game-integration.md) and individual
activity evidence document the lower-level source contracts.
