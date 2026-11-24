# All ten original pregame screens: executable-derived C++26 integration

This is a source-only recovery of the **ten missing original pregame
instruction pairs**, not a preview renderer. These are **20 of the original
68 top-level states** that were previously listed as requiring adapters.

New files:

- `include/btb/full_game_pregame.hpp`
- `include/btb/full_game_walkthrough_catalog.hpp`
- `src/full_game_walkthrough_catalog.cpp`
- `include/btb/full_game_pregame_resources.hpp`
- `src/full_game_pregame_resources.cpp`
- `ghidra/pregame_action_routes.csv` — **60 actual jump-table entries**
- Source-only tests in `tests/full_game_pregame*_source_test.cpp`
  and `full_game_walkthrough_catalog_source_test.cpp`.

The production coordinator `GameRoot` now has
`install_pregame_front_end_pair` for the same source-backed
`GenericUiScreenDriver` used by the chooser/replay screens. This
reuses the original polygon/UI/sound policy without creating unrelated
mock instruction screens.

## Exact source state assignments

All rows were checked against the installed 311,296-byte retail PE32
executable, using `objdump -d -Mintel` and direct little-endian
jump-table reads. Addresses are genuine virtual addresses in that build.

| Activity | Setup / update | Generic UI index | Movie mode 14 on setup? | Exit/start via -5 | Back via -1 |
|---|---|---:|---|---|---|
| Herding / Pets Corner | 0x0C / 0x0D | 2 | Yes | 0x0E | 0x04 |
| Dinosaur | 0x12 / 0x13 | 2 | No | 0x14 | 0x10 |
| Spud Skate | 0x18 / 0x19 | 8 | No | 0x1A | 0x16 |
| Maze | 0x1E / 0x1F | 2 | No | 0x20 | 0x1C |
| Fireworks | 0x22 / 0x23 | 7 | Yes | 0x24 | 0x04 |
| Squirrel | 0x26 / 0x27 | 2 | Yes | 0x28 | 0x04 |
| Bob's Band | 0x2C / 0x2D | 8 | No | 0x2E | 0x2A |
| Park Designer | 0x30 / 0x31 | 8 | Yes | 0x32 | 0x04 |
| Golf | 0x34 / 0x35 | 2 | No | 0x36 | 0x1C |
| Spud Maze | 0x38 / 0x39 | 2 | No | 0x3A | 0x16 |

The six-entry input jump tables use **`action+6`** for values
`-6..-1`. The recovered policy is:

- **-1**: return to the original parent chooser / Activity Select.
- **-6**: retain the instruction update state; contextual Help remains
  the dedicated generic UI system's responsibility.
- **-5**: enter that activity's exact init state.
- **-4/-3/-2**: write difficulty 2/1/0 to global `0x51C284` and
  remain in the instruction update state on difficulty-selection screens.
  On all four no-difficulty screens, -2 directly starts the activity.
  Fireworks 0x42BFF3, Bob's Band 0x42C668 and Park Designer 0x42C8E3
  store zero and fall through into their -5 handlers (verified directly
  against the original PE32 instructions).
- **Spud Skate -2** jumps **straight to 0x1A**, resets difficulty
  to zero and sets start latch `0x51C32C = 1`. This matches the
  real `0x42B6AD` branch, not a guessed no-difficulty-screen convention.
- Herding's three difficulty branches also write persistent
  `0x51C340`, which its setup reloads into current difficulty.
  This is not a universal feature of every activity.

### A critical movie-mode transition

Four setup blocks—Herding, Fireworks, Squirrel and Park Designer—load
their instruction surfaces, call `OpenGlobalBinkMovie(0x408EB0)`,
**decrement the outer state back to setup**, and store
`0x51C27C = 14`.

The existing `game_flow::dispatch_step` knows how to resume native
movie mode 14: it waits for completion; then increments the state,
resets screen mode to 7 for Fireworks update 0x23 or 2 otherwise,
**clears the input pulse**, and dispatches that update on the *same*
frame. `GameRoot` now actually enters this mode and waits for an
observed completion event, instead of silently skipping the movie.

The other six setups increment to update directly and use native screen
mode 2 or 8. All ten also request `OpenWalkthroughMovie(0x4281D0)`,
which is an independent Bink subsystem and still needs a platform host.

The earlier GameRoot code cleared the global input-pulse latch but still
forwarded `ActivityFrameInput::click_pulse = true` to the reentered
activity/menu. This could accidentally activate Start/Easy when the user
dismissed a movie. The coordinator now clears this host-side click too.

## Original backdrop and walkthrough resources

`OpenWalkthroughMovie` (004281D0) reads only the BIK path at
`0048E6F0 + index * 0x104`. The second column of `binkwalk.txt` is retained
as source metadata, not a required playback asset. The actual narration
comes from the independent managed-sound IDs in `RunMainGameFlow`
(0042A2C0), already dispatched by `GenericUiFrontEnd`. For example,
Maze uses ID 488 and Golf uses ID 489. The retail media tree contains
those numbered catalog assets, not the obsolete walkthrough-help WAVs.
A missing actual BIK or required managed sound still fails normally.

The installed `loaddata/uiBitmapName.txt` is 23 bitmap names plus
`END.bmp`. Disassembly confirms that the instruction setup
passes a **256-byte-per-name** pointer from the loaded table at base
`0x490770`. For example, Herding's literal `0x490B70` means
index 4 (Pets Corner background); Park Designer's `0x491970`
means index 18.

`loaddata/binkwalk.txt` is exactly **13 whitespace-separated records**:
ten movie/help-WAV pairs followed by three rows with a literal
`NULL` movie and additional help WAV. The parser preserves both
classes and rejects malformed replacements without erasing a previously
valid catalog.

| Activity | Background slot | Walkthrough index |
|---|---:|---:|
| Herding | 4 — `petscorner.bmp` | 0 |
| Dinosaur | 5–7 — `vel/tri/trex.bmp` | 1 |
| Spud Maze / Skate | 10–11 — `repairskate/rideskate.bmp` | 2–3 |
| Maze / Golf | 13–14 — `golfcollect/playgolf.bmp` | 4–5 |
| Fireworks | 15 — `go.bmp` | 6 |
| Squirrel | 16 — `sqrinstbg.bmp` | 7 |
| Bob's Band | 17 — `mp.bmp` | 8 |
| Park Designer | 18 — `instdyp.bmp` | 9 |

Dinosaur, Spud and Adventure use source-global `0x51C2E4`
selection when resolving their BMPs; the Spud and Adventure walkthrough
indices add offsets 2 and 4 respectively. This is why all ten
cannot simply index a bitmap or movie array using their generic UI screen
number.

The `OriginalPregameResources` adapter joins the already parsed
real backdrop and real `binkwalk.txt` entries to produce an explicit
presentation contract: bitmap Draw, Bink filename, help WAV filename,
and global movie-mode14 requirement. It does not synthesize art or sound.

### What is still outstanding

- A complete native DirectDraw surface compositor for instructional
  foreground/hover/depressed artwork and proper clipping/draw ordering.
- Real global Bink and looping walkthrough Bink startup, frame updates,
  timing, and CloseWalkthroughMovie teardown.
- Full instruction-screen Help and narrated audio; no-difficulty Start
  is directly routed by -2, not by a synthetic deferred -5.
- Binding real asset configuration and presentation to every normal
  game path, not just source-level driver instances.
- Cross-activity visual, audio, save and input validation against the
  actual 2002 executable.

**No new full build, CTest run or executable validation has taken place.**
All new test files are still source-only pending the complete game build.

## Global startup-film catalog — exact four mode-14 sources

The installed `loaddata/startupmovie.txt` was also read. It contains
eight filenames, ordered Herd, Dino, Skate, Adventure, Fireworks,
Squirrel, Music and Designer. The retail setup's live pointers are
`0x493970 + 0x100 * index`, directly confirming the four pregame
`OpenGlobalBinkMovie` calls:

| Mode-14 pregame | Table slot | Original retail filename |
|---|---:|---|
| Herding | 0 | `Data\\movies\\herdstartup.bik` |
| Fireworks | 4 | `Data\\movies\\fireworkstartup.bik` |
| Squirrel | 5 | `Data\\movies\\squirelstartup.bik` |
| Park Designer | 7 | `Data\\movies\\designstartup.bik` |

The other startup movie names remain in the table but are not asserted to
be directly opened by the six non-mode14 pregame setup handlers. Their
complete lifecycle may be driven from other source states.

`full_game_startup_movies.hpp/.cpp` loads the exact eight records.
The full-game pregame setup now exposes an
`original_global_intro_movie_index` alongside its independently
loaded walkthrough movie index. These are different movies: do not
replace the intro with the help/walkthrough animation.

A new source-only test `full_game_startup_movies_source_test.cpp`
covers all four indices, filenames, and malformed-file rejection.

## Source-level Adventure → Golf lifecycle regression

`full_game_adventure_to_golf_source_test.cpp` exercises a complete
**single GameRoot** flow using the original action codes and real data
formats:

1. Adventure chooser setup `0x1C` → update `0x1D`;
2. select the source's Golf tile **-3** → set `0x51C2E4 = 1` and
   route to Golf pregame setup `0x34`;
3. resolve original Golf instruction backdrop slot **14**,
   `Data\\ui\\instruction\\playgolf.bmp`, and Adventure walkthrough
   index **5**;
4. choose Easy **-2** on pregame update `0x35`, writing difficulty
   zero to `0x51C284`;
5. press the separately encoded Start **-5** and enter Golf init `0x36`;
6. initialize the source-backed Golf round with **Easy difficulty 0**
   regardless of the initially configured fallback difficulty, retain
   the game's original **five attempts**, then enter runtime `0x37`.

The original background registry is now retained by source-backed
pregame drivers. When a menu selects a different Dinosaur, Spud or
Adventure subgame, `GameRoot` supplies the new retail
`0x51C2E4` value **before the next instruction setup**, allowing
the same generic UI driver to recalculate its original BMP slot.
Previously, creating the driver for one choice could leave it displaying
that choice's background after another choice was selected.

This is an integration **test source fixture only**. It does not
prove that the full Windows executable was compiled, played, or that
the DirectDraw/Bink/DirectSound outputs match the retail game.


## Retail instruction-media cross-check (2026-11-24)

The supplied BTB-BBP original directory contains every one of the ten BIK
names in the first column of binkwalk.txt. None of the legacy walkthrough
help WAV names occur in the supplied sound directory. The actual
Data/sound/binklist.txt mappings below all resolve to existing original WAVs
(checked against the complete directory listing of 1,006 entries).

| Activity | Managed sound ID | Actual WAV |
|---|---:|---|
| Herding | 501 | IS_PC_WEN_01.wav |
| Dinosaur | 496 | IS_DD_WEN_01.wav |
| Spud Skate | 506 | IS_SS2_WEN_01.wav |
| Maze | 488 | IS_CG1_WEN_01.wav |
| Fireworks opening | 498 | IS_GO_BOB_01.wav |
| Fireworks follow-up | 499 | IS_GO_WEN_O1.wav |
| Squirrel | 503 | IS_SR_WEN_01.wav |
| Bob's Band | 500 | IS_MP_WEN_01.wav |
| Park Designer | 497 | IS_DYP_WEN_01.wav |
| Golf | 489 | IS_CG2_WEN_01.wav |
| Spud Maze | 505 | IS_SS1_WEN_01.wav |

The regression uses the production generic UI, session, resolver and sound
planner with isolated fixture files and a recording device for every
pregame route, including the four intro-gated routes and the two-stage
Fireworks narration. It does not claim interactive DirectDraw or DirectSound
verification.
