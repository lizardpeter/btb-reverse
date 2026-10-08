# First integrated C++26 executable (experimental)

The reconstruction now has a real application target, `btb_game`, instead
of only isolated library/test targets. This is the **first playable host**, not
a recompilation of the whole retail executable.

## Building

Requirements: CMake 3.25+ and a C++26-capable compiler.

On Windows with Visual Studio 2022:

```powershell
cmake -S reconstruction -B build/reconstruction-win -G "Visual Studio 17 2022" -A x64
cmake --build build/reconstruction-win --config Release --target btb_game
.\build\reconstruction-win\Release\btb_game.exe
```

On Linux, the same game-session code is built with GCC 14:

```sh
CXX=g++-14 cmake -S reconstruction -B build/reconstruction
cmake --build build/reconstruction --target btb_game btb_game_session_tests
ctest --test-dir build/reconstruction -R 'game_session|game_executable_smoke' --output-on-failure
./build/reconstruction/btb_game --smoke
./build/reconstruction/btb_game --dino-level "/path/to/original/dino.txt"
```

The Linux application is **headless** at this stage. The graphical host is
Windows-only. CI also builds a Windows x64 preview and, on a successful run,
uploads `btb-game-win64-preview` as a downloadable workflow artifact. The
artifact does not contain retail copyrighted assets.

## Running the Windows preview

1. Copy/use your legally obtained original game's installed `Data` files.
   No retail resources are included in this repository.
2. Launch `btb_game.exe`. The interface is a fixed 640x480 logical canvas,
   rescaled with the client window.
3. Use **1/2/3** for Raptor/Triceratops/T-Rex and **Q/W/E** for
   Easy/Medium/Hard.
4. Press **O** or click **Open original Dino level** and choose that
   species/difficulty's **dino.txt** file.
5. Click to pick up loose bones; move the cursor to a target and release the
   mouse button. An invalid release keeps the piece carried. **Esc** returns
   to the activity selection screen; Esc again exits the game.

The host loads `piece<ID>.bmp` images adjacent to the selected `dino.txt`,
reads their **actual dimensions** into the reconstructed Dino hit-testing
runtime and draws them with the original magenta transparency key. The original
retail Dino data parser validates the piece permutation and starting/target
positions. If the BMPs cannot be found, the host shows **obvious placeholders**
so that wiring and input can still be exercised; placeholder hitbox dimensions
are not retail-exact.

The graphical wrapper consumes the real `compose_dino_frame` draw-command
ordering and runs the original Dino puzzle-mode controller for selection,
rejected/accepted drops, finalization, scoring, and completion. The high-level
outer-state dispatch comes from the recovered 68-state `game_flow` table. The
application keeps `DinoRun` as its outer state when a puzzle is complete,
matching the original engine's completion-phase ownership.

## Boundaries / work still outstanding

- **Retail UI images, all hover areas, click audio, background scene, Bob/Ellis
  animation textures, and certificate presentation** are not yet wired to the
  Windows shell. The placeholder host UI is intentionally not advertised as
  an accurate recreation.
- Runtime audio is not playing yet; the session exposes the recovered retail
  sound IDs for integration with the sound manager. The completion event does
  not currently persist the retail profile/progress-slot write.
- This is not the original DirectDraw/DirectInput/DirectSound frame loop.
  GDI is a temporary platform bootstrap. The reconstructed application-frame
  policies remain available in `application_runtime.hpp`.
- Other activities have compiled reconstruction modules and tests but are
  **not available** in the new executable until their complete runtime and
  presentation dependencies are hosted.
- The source installer/CD layouts can have additional asset directory nesting.
  Only BMPs placed adjacent to the chosen `dino.txt` are currently searched
  for each piece.

## Milestone definition

This stage is considered successful when:

- `btb_game` and `btb_game_session_tests` compile;
- Linux smoke/session tests pass;
- Windows CI builds the executable and uploads the artifact;
- a Windows manual run opens an original Dino level and permits a full puzzle
  from start to completion without the original executable.

**No milestone is claimed as visually faithful or complete merely because
the source code compiles.**
