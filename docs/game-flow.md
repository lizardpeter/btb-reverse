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
- `0x00424100` performs extensive Spud Maze resource cleanup.
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

## Next state-machine work

The next useful pass is to name the shared transition states rather than only the activity pairs. Their repeated call patterns already indicate common responsibilities:

- help/walkthrough movie launch
- activity intro movie
- completion movie
- loading bitmap / transition surface setup
- activity-select return
- sound reset
- generic play-again / quit flow

Once those are named, nearly every branch in `RunMainGameFlow` should read semantically.


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
