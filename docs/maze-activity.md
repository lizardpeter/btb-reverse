# Maze activity

The Maze activity is driven by a three-screen navigation graph in `loaddata/maze_nodes.txt`.

## Entry points

- `0x0041A4E0 LoadMazeData`
- `0x0041A730 InitializeMazeActivity`
- `0x0041D4D0 UpdateMazeActivity`

The resource teardown immediately before the loader releases Maze surfaces and stops activity music; it is being named separately in the symbol map.

## Screen/node format

The file contains three graph sections:

1. West screen
2. Middle screen
3. East screen

They are separated by tokens beginning with `NEXT` and terminated by `END`.

Each normal line begins with an ignored human-readable label, then:

```text
node_id
x y
direction_bits
node_type
up_link right_link down_link left_link
```

The executable reads the node ID separately, then reads exactly eight integers into a **0x20-byte node record**.

### Retail Y correction

Immediately after parsing a node, the loader subtracts **11** from its stored Y coordinate:

`retail_y = source_y - 11`

The source reconstruction retains both source and retail coordinates.

## Direction bits and link order

The data resolves the direction mask unambiguously:

| Bit | Direction | Link field |
|---:|---|---|
| 1 | Up | link 0 |
| 2 | Right | link 1 |
| 4 | Down | link 2 |
| 8 | Left | link 3 |

Example: West node 2 at `(87,67)` has mask 6 and links `[-1,3,6,-1]`, meaning Right + Down.

The loader stores each screen in its own 30-node logical range. Encountering a `NEXT` marker advances the internal base by 30 nodes.

## Node types / screen portals

The runtime around the Maze navigation transition branch tests the parsed node-type bits directly and assigns the active screen index.

| Node-type bit | Meaning | Destination screen index |
|---:|---|---:|
| `0` | normal navigation node | unchanged |
| `128 / 0x80` | portal to **West** | 0 |
| `256 / 0x100` | portal to **East** | 2 |
| `512 / 0x200` | portal to **Middle** | 1 |

The branch priority is exactly 0x80, then 0x100, then 0x200.

This matches the source graph topology:

- Middle's far-left exit node has type 128 and moves to West.
- Middle's far-right exit node has type 256 and moves to East.
- West's far-right boundary node and East's far-left boundary node have type 512 and return to Middle.

When a portal triggers, the executable also seeds the player's new position from the destination entry/reference coordinates and updates the active-screen/node globals.

## Post-END reference-node table

After `END`, the loader reads **12 triples**, arranged as 3 screens × 4 records:

`x y node_id`

The source comments describe these as:

- node list for left screen
- node list for middle screen
- node list for right screen

The reconstruction calls them `reference_nodes` until their exact runtime purpose is closed.

## Difficulty/runtime tuning

The remaining values are explicitly documented by the source file:

### Player speed

`2.0`

### Spud speed, regular

- Easy: 1
- Medium: 1
- Hard: 2

### Spud speed, carrying package

- Easy: 5
- Medium: 6
- Hard: 7

### Timer

- Easy: 240
- Medium: 180
- Hard: 120

### Spud spawn interval, frames

- Easy: 1500
- Medium: 1000
- Hard: 200

### Spud animation delay

`6`

The dramatic hard-mode spawn interval confirms that difficulty is not merely cosmetic in this activity.

## Directional input debounce

`0x0041AEB0 DebounceMazeDirectionalInput` is now exact at source level.

Retail retains two three-sample `int32` histories at `0x00512130..0x00512147`, one for X and one for Y. On every call it:

1. shifts sample 1 to sample 0
2. shifts sample 2 to sample 1
3. writes the newest axis intent to sample 2
4. requires all three X samples to match
5. requires all three Y samples to match

The important quirk is that the axes are **not** emitted independently. If either three-sample history is unstable, retail writes zero to **both** output intents. A newly pressed or changed direction therefore needs three consecutive matching samples before movement is emitted.

The C++26 reconstruction is `DirectionalDebounce` in `maze_runtime.hpp/.cpp`, with regression coverage for press, diagonal change, and release behavior.

## Exact surface map

`0x0041A730 InitializeMazeActivity` loads the following surfaces:

| Global | Surface | Retail file | Size | Color key |
|---:|---|---|---:|---:|
| `0x00512100` | West background | `Data\\SubGameMaze\\LEFT_MAZE.bmp` | 640x480 | none |
| `0x00512104` | Middle background | `Data\\SubGameMaze\\MID_MAZE.bmp` | 640x480 | none |
| `0x00512108` | East background | `Data\\SubGameMaze\\RIGHT_MAZE.bmp` | 640x480 | none |
| `0x0051210C` | Player | `Data\\SubGameMaze\\player.bmp` | 1024x512 | `0x00FF00FF` |
| `0x00512110` | Spud | `Data\\SubGameMaze\\spud.bmp` | 1792x512 | `0x00FF00FF` |
| `0x00512114` | Package box | `Data\\SubGameMaze\\box.bmp` | 26x29 | `0x00FF00FF` |
| `0x00512118` | Package ghost | `Data\\SubGameMaze\\boxghost.bmp` | 16x19 | `0x00FF00FF` |
| `0x0051211C` | Completed-package marker | `Data\\SubGameMaze\\smallbox.bmp` | 16x19 | `0x00FF00FF` |
| `0x00512120` | Numerals | `Data\\SubGameMaze\\numerals.bmp` | 200x40 | `0x00000000` |
| `0x005144D0` | Timer strip | `Data\\SubGameSpudMaze\\timer.bmp` | 103x25 | `0x00FF00FF` |

Two retail oddities are worth preserving:

- Maze has its own `Data\\SubGameMaze\\timer.bmp` on disk, but the executable does **not** load it here. It deliberately references the identically sized Spud Maze timer path.
- `numerals.bmp` is loaded and color-keyed, but there is no Maze runtime read of its `0x00512120` surface global after initialization. It is a loaded-but-unused legacy surface in this build.

The machine-readable map is `ghidra/maze_surface_map.csv`.

## Actor sprite geometry

`0x0041C110 DrawMazeActivity` uses 128x128 source cells for both the player and Spud.

For both actors, source column 4 is remapped to column 3 before the rectangle is formed; all other columns pass through unchanged. The destination origins are asymmetric:

- player: `(trunc(x - 65), trunc(y - 64))`
- Spud: `(trunc(x + horizontal_screen_offset - 64), trunc(y - 64))`

The float-to-integer helper at `0x004304D0` explicitly changes the x87 control word to truncate toward zero, so the reconstruction uses truncating conversions rather than rounding.

## Timer and package HUD

`0x0041BF80 DrawMazeTimer` updates the countdown from the high-resolution counter and draws two HUD components.

The timer strip is placed at **(422,430)**. Its source rectangle is:

`[0, 0, trunc((remaining / initial) * 102), 25]`

Although the bitmap is 103 pixels wide, retail scales the visible fill against exactly **102.0** pixels.

Package completion is shown with `smallbox.bmp` for completed entries and `boxghost.bmp` for the rest. Difficulty selects exact totals **4 / 8 / 12**. Markers are spaced 20 pixels apart, centered from:

`start_x = 244 - (total / 2) * 20`

at Y=430.

The routine also contains a dormant row-wrap branch: when the accumulated X offset exceeds 250, it resets X offset to zero and moves the marker row to Y=450. Normal totals never reach that branch, but the C++26 presentation helper preserves it.

## Outer activity controller

The retail `0x0041D4D0 UpdateMazeActivity` controller is now separated from the graph logic conceptually:

- remaining time equal to **15** triggers managed sound ID **83** at priority 90/class 1
- success is `packages_remaining <= 0`
- timeout is `remaining_time < 0` while no screen transition is active
- outcome phase 0 stops managed feedback and selects sound ID **84**, **85**, or **86** according to completion/time state
- phase 1 continues drawing the activity while managed feedback is active
- successful completion writes player progress before entering the shared Play Again path
- the Play Again outer state is **0x3C**
- normal leaving unloads Maze resources and routes either to state **0x1C** or the shared movie state **0x40** depending on the shared transition code

## Source reconstruction

The typed C++26 reconstruction now spans:

- `reconstruction/include/btb/maze_data.hpp`
- `reconstruction/src/maze_data.cpp`
- `reconstruction/include/btb/maze_runtime.hpp`
- `reconstruction/src/maze_runtime.cpp`
- `reconstruction/include/btb/maze_presentation.hpp`
- `reconstruction/include/btb/maze_controller.hpp`
- `reconstruction/tests/maze_data_test.cpp`
- `reconstruction/tests/maze_runtime_test.cpp`
- `reconstruction/tests/maze_presentation_test.cpp`
- `reconstruction/tests/maze_controller_test.cpp`

It currently reproduces the complete graph/data parse, portal semantics, retail-bounded shortest-path search, three-sample all-or-nothing directional debounce, exact surface bindings, actor sheet geometry/origins, timer/package HUD composition, and the typed outer completion/startup/leave controller.

The main remaining Maze source closures are the full player interpolation state machine, Spud's eight-state package/path controller, and promotion of the animated two-screen transition composition into typed C++26 state.
