# Main game-flow state machine

## Dispatcher root

`0x0042A2C0 RunMainGameFlow` is the central high-level game controller.

It indexes a **68-entry jump table** with global state:

`0x0044DE14`

Valid values are `0x00..0x43`. The machine-readable state -> handler map is in:

`ghidra/gameflow_states.csv`

## Key architectural result

The activity lifecycle is highly regular. Most playable activities have a dedicated **initialize state** immediately followed by a **runtime/update state**. Around those pairs are reusable front-end states for walkthrough/help movies, loading/transition screens, completion screens, sound teardown, and returning to activity selection.

This means the executable is not one monolithic game loop. The central dispatcher acts as an activity router and each minigame owns its own resource/init/update logic.

## Identified activity pairs

| States | Activity | Init entry | Runtime entry | Confidence |
|---|---|---:|---:|---|
| `0x0E / 0x0F` | Herding / `Data\\SubGame1` | `0x00419600` | `0x004182B0` | high / medium |
| `0x14 / 0x15` | Dinosaur | `0x0040A670` | `0x00409E00` | high |
| `0x1A / 0x1B` | Spud Skate | `0x00424360` | `0x00424D10` | high |
| `0x20 / 0x21` | Maze | `0x0041A730` | `0x0041D4D0` | high |
| `0x24 / 0x25` | Fireworks | `0x00411C80` | `0x00413F10` | high |
| `0x28 / 0x29` | Squirrel | `0x00425160` | `0x004279F0` | high |
| `0x2E / 0x2F` | Bob's Band / music sequencer | `0x0041DB20` | `0x00420030` | high |
| `0x32 / 0x33` | Park Designer / DYP | `0x0040B7E0` | `0x00410D10` | high |
| `0x36 / 0x37` | Golf | `0x004145F0` | `0x00415830` | high |
| `0x3A / 0x3B` | Spud Maze | `0x004207E0` | `0x00423F40` | high |

### Why these names are strong

The state blocks call these entry points directly, and the called code contains matching source-data references:

- Golf initialization loads `Data\\SubGameGolf\\flag.bmp`, ball sprites, marker, power bar, shadow, numerals, and `golfbg.bmp`.
- Maze initialization loads `Data\\SubGameMaze` assets and `lofty engine.wav`.
- Spud Maze uses `loaddata\\spudmaze_nodes.txt` plus `Data\\SubGameSpudMaze` repair/toolbar/screen assets.
- Spud Skate is tied to `spuddata1.txt`, `spuddata2.txt`, `spuddata3.txt`, `soundinfo.txt`, and its result Bink/score surfaces.
- Squirrel initialization parses `Data\\SubGameSquirrel\\sqdata.txt` and loads the squirrel/conveyor/Lofty assets.
- Fireworks reads `fireworks.txt` and `fireworkboxes.txt` and loads the firework sprite families.
- Park Designer is rooted in `Data\\SubGameDYP` and `boundareas.txt`.
- Dino uses the Raptor/Triceratops/T-Rex difficulty tables and per-level `dino.txt`.
- Herding is the `Data\\SubGame1` cluster containing Pickles, sheep, rabbit, duck, gates, trailer, Travis cab, and `herd.txt`.
- Bob's Band owns the `Data\\SubGameOpen` five-machine sequencer, three conductor tracks, 5x24 grid, and multi-pitch WAV banks.

## Spud Maze -> Spud Skate boundary

The code boundary is especially clear:

- `0x00423F40` is the Spud Maze runtime function.
- `0x004240F5` is the end of the Spud Maze runtime region.
- `0x00424100` is already Spud Skate resource teardown/helper code.
- `0x00424230` begins parsing the three Spud Skate data tables.
- `0x00424360` initializes Spud Skate.
- `0x00424D10` is the Spud Skate runtime state.
- `0x00424E20` begins the Squirrel `sqdata.txt` parser.
- `0x00425160` initializes the Squirrel activity.

This gives us a useful sequence of clean module boundaries in the executable.

## Other global flow state

Do not confuse the primary activity state at `0x0044DE14` with `0x0044DDB0`.

`0x0044DDB0` is also consulted by many runtime modules, but during startup it is used by the credits/loading slideshow:

- `0x004069C0 LoadCreditsSlideshow`
- `0x00406AD0 UpdateCreditsSlideshow`

It is therefore a secondary/shared phase variable rather than the main 68-state activity selector.

## Exact pre-dispatch interception

The source-level reconstruction now includes the control flow *before* the
68-entry jump table. `RunMainGameFlow` does not immediately dispatch
`0x0044DE14`; it checks these conditions in strict precedence order:

1. shutdown/credits phase `0x0044DDB0 != 0`;
2. Options overlay `0x0051C2BC`;
3. Progress screen `0x0051C324`;
4. generic Yes/No confirmation `0x0051C2D0`;
5. whole-game Quit confirmation `0x0051C2C0`;
6. leave-current-activity confirmation `0x0051C2C8`;
7. Play Again overlay `0x0051C2CC`;
8. positive legacy intercept `0x0051C2D8`;
9. global-Bink screen modes 13/14 at `0x0051C27C`;
10. only then the 68-state table.

Options, generic Yes/No, whole-game Quit, leave-current-activity, and the
Play Again overlay pause the shared global Bink before updating their modal.
The Progress screen is the notable exception and is serviced without that
pause call.

The positive `0x0051C2D8` path is retained legacy behavior. Values 1..9 call
the one-instruction `NoOpLegacyHook(0)`; values >=10 call
`NoOpLegacyHook(1)`. Either case consumes the frame without entering the
state table.

### Global-Bink modes 13 and 14

Screen mode **13** updates the shared global Bink. While it is playing, the
dispatcher returns immediately. On completion retail:

- restores `0x0044DE14` from saved state `0x0051B418`;
- changes screen mode to **2**;
- clears shared input pulse `0x004FBE54`;
- immediately continues into the restored state-table handler on the same
  frame.

Screen mode **14** similarly waits for the global Bink, then:

- increments the current outer state;
- sets screen mode **7** if the new state is `0x23`, otherwise **2**;
- clears the same input pulse;
- immediately dispatches the incremented state on that frame.

Finally, the executable performs an unsigned `state <= 0x43` check. Values
outside the 68-entry range fall through the common no-op return instead of
indexing the jump table.

The exact pre-dispatch sequence is machine-readable in
`ghidra/gameflow_dispatch_precedence.csv`.

## Source reconstruction

The main flow is now represented directly in:

- `reconstruction/include/btb/game_flow.hpp`
- `reconstruction/tests/game_flow_test.cpp`

That source contains:

- the full 68-value state enum;
- the original handler address for every state;
- semantic state kind/module/role metadata;
- exact modal/intercept precedence;
- mode-13/mode-14 Bink restoration behavior;
- invalid-state/no-op behavior.

The generic 12-screen front-end catalog is separately represented in
`reconstruction/include/btb/front_end_ui.hpp`.


## Bob's Band -> Spud Maze boundary

The source-oriented boundary is now exact:

- `0x0041D7C0` parses Bob's Band `machinedata.txt`.
- `0x0041DB20` initializes Bob's Band.
- `0x00420030` runs Bob's Band.
- Bob's Band runtime ends at `0x00420324`.
- `0x00420360` is already Spud Maze resource teardown/helper code.
- `0x00420560` begins parsing `loaddata\\spudmaze_nodes.txt`.
- `0x004207E0` is the later Spud Maze initializer.

So `0x00420360`, rather than `0x00420560` or the later `0x004207E0` initializer, is the actual start of the Spud Maze module when helper/teardown code is included.

## Bob's Band C++26 reconstruction (integration pending)

The formerly missing music sequencer now has a source-level grid model,
editor controller, 50-sample pitch-bank map, original `machinedata.txt` parser,
machine/conductor animation, and a frame-oriented playback/audio/progress plan.
The music machine is **not wired to the unified game executable** yet, and
these sources have deliberately not been included in a new compile stage.

See [Bob's Band / music sequencer](bobs-band-activity.md) for disassembly
anchors, limits, and input/asset contract.
