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

## Mouse/keyboard input arbitration

`0x0041AF60 ComputeMazePlayerInputDirection` has now been promoted to typed C++26 rather than being represented only as a named binary helper.

The persistent input-mode global at `0x00512124` has an intentional **one-call delay in both directions**:

- if the call begins in mouse mode and any keyboard direction bit is active, retail writes keyboard mode immediately but still executes the **mouse path for the current call**
- if the call begins in keyboard mode and the cursor differs from the remembered coordinates at `0x0050AFC8/0x0050AFCC`, retail writes mouse mode immediately but still executes the **keyboard path for the current call**

The next call sees the new mode.

Keyboard direction bits bypass the three-sample debounce entirely. Their exact precedence is:

| Bit | Axis result | Precedence |
|---:|---|---|
| `0x02` | X = +1 | wins over `0x01` |
| `0x01` | X = -1 | only if `0x02` is clear |
| `0x08` | Y = -1 | wins over `0x04` |
| `0x04` | Y = +1 | only if `0x08` is clear |

The mouse path first truncates the player's float coordinates to integers. A cursor distance **<= 6.0 pixels** returns immediately, before debounce and before retail rewrites the shared node-capture radius at `0x00443D64` to **4**. Outside that dead zone, retail derives an integer direction angle, stores sine/cosine to 32-bit float temporaries, and emits an axis only when the absolute component is **strictly greater than 0.25**. Only that mouse-derived pair is fed through `DebounceMazeDirectionalInput`.

The reconstruction is in:

- `reconstruction/include/btb/maze_input.hpp`
- `reconstruction/src/maze_input.cpp`
- `reconstruction/tests/maze_input_test.cpp`

## Shared integer-angle and node-seeking movement

The shared helper `0x00415D70 AngleBetweenIntegerPointsDegrees` is intentionally **not** replaced with `atan2`.

Retail computes:

1. `abs(dx)` and `abs(dy)`
2. `ratio = abs(dy) / abs(dx)`
3. if `abs(dx) == 0`, substitutes **9999.0** directly for the ratio
4. `atan(ratio) * 57.2949981689453125`
5. manual 90/180/270-degree quadrant correction
6. truncation toward zero through `0x004304D0`

That creates several real quantization artifacts:

- exact right = 90°
- exact down = 180°
- exact left = 270°
- exact up = **359°**, not 0°
- down-right 45° diagonal = **134°**, not 135°
- up-left 45° diagonal = **314°**, not 315°

`0x0041CC20 MoveMazeActorTowardNode` reuses that helper. It truncates the actor's float position before choosing the angle, converts the integer speed to floating point, then performs:

`y -= cos(angle) * speed`

`x += sin(angle) * speed`

and stores Y before X. The routine also calls the integer-point distance helper and immediately discards its result; there is **no distance stop inside this helper**.

A consequence of the 359° vertical-up angle is that a nominally straight upward node-seeking movement has a tiny leftward drift. That behavior is preserved in `maze_motion.hpp/.cpp` and its regression rather than normalized away.

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

## Animated screen transition compositor

`0x0041C710 DrawMazeScreenTransition` is now represented as typed source geometry/state.

The routine first draws the current full background and timer, then animates only the **interior maze viewport**:

- left = **20**
- top = **20**
- right = **620**
- bottom = **400**
- visible transition area = **600x380**

Legal screen changes are adjacent `West <-> Middle <-> East`. The transition direction is therefore equivalent to whether the destination screen index is greater or less than the previous one.

On initialization retail changes transition phase **1 -> 2**, sets the signed direction to **+1 / -1**, and seeds the acceleration numerator at **10**. Each rendered frame uses:

`pixel_step = speed_numerator / 5`

with integer truncation. The numerator increments by one up to **200**, is clamped against the remaining horizontal travel, and is finally forced back to a minimum of **10**. This gives the slide its accelerating motion while still closing the final gap.

For travel toward the east, the previous screen is the left blit and its source-left edge advances rightward; the destination screen is revealed as an increasing strip from the right. The first active frame is exactly:

- previous source: `[23,20,620,400]` at destination `(20,20)`
- destination source: `[20,20,23,400]` at destination `(617,20)`
- actor offsets: **597 / -3**
- next speed numerator: **11**

For travel toward the west, the ownership swaps: the destination screen is the narrow left strip while the previous screen is shifted right. The first active frame is:

- destination source: `[617,20,620,400]` at destination `(20,20)`
- previous source: `[20,20,618,400]` at destination `(23,20)`
- actor offsets: **-597 / 3**
- next speed numerator: **11**

Completion clears the transition phase and signed direction after the moving crop reaches/passes the 20/620 boundary. The C++26 reconstruction intentionally preserves the executable's final-frame integer overshoot behavior rather than clamping the source rectangle to aesthetically cleaner bounds.

The implementation/regression is:

- `reconstruction/include/btb/maze_transition.hpp`
- `reconstruction/src/maze_transition.cpp`
- `reconstruction/tests/maze_transition_test.cpp`

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
- `reconstruction/include/btb/maze_input.hpp`
- `reconstruction/src/maze_input.cpp`
- `reconstruction/include/btb/maze_motion.hpp`
- `reconstruction/src/maze_motion.cpp`
- `reconstruction/include/btb/maze_player_navigation.hpp`
- `reconstruction/src/maze_player_navigation.cpp`
- `reconstruction/include/btb/maze_transition.hpp`
- `reconstruction/src/maze_transition.cpp`
- `reconstruction/include/btb/maze_presentation.hpp`
- `reconstruction/include/btb/maze_controller.hpp`
- `reconstruction/tests/maze_data_test.cpp`
- `reconstruction/tests/maze_runtime_test.cpp`
- `reconstruction/tests/maze_input_test.cpp`
- `reconstruction/tests/maze_motion_test.cpp`
- `reconstruction/tests/maze_player_navigation_test.cpp`
- `reconstruction/tests/maze_transition_test.cpp`
- `reconstruction/tests/maze_presentation_test.cpp`
- `reconstruction/tests/maze_controller_test.cpp`

It currently reproduces the complete graph/data parse, portal semantics, retail-bounded shortest-path search, one-call-late mouse/keyboard arbitration, exact shared integer-angle and node-seeking motion, player 4px/30px node-candidate scanning, three-sample all-or-nothing mouse debounce, the complete two-screen slide compositor, exact surface bindings, actor sheet geometry/origins, timer/package HUD composition, and the typed outer completion/startup/leave controller.

The main remaining Maze source closures are the rest of the player interpolation/animation state machine and Spud's eight-state package/path controller.


## Maze Spud NPC eight-state controller (partial closure)

The original PE32 executable's `0x0041CCB0 UpdateMazeSpudNPC` dispatches through
**eight actual retail jump-table targets** at `0x0041D4AC`. The addresses and
state names are now recorded in `ghidra/maze_spud_npc_states.csv`.

The source-level reconstruction has added a deliberately bounded, auditable
`maze_spud_controller.hpp` rather than pretending the entire NPC is already
ported:

- **Off-screen gate:** when Spud is on another screen and the NPC phase is
  2..7, decrement `0x5120E8`. The zero countdown frame still returns without
  updating previous X/Y. On the following negative tick, retail restores the
  selected package record to status **1** and returns to phase **1**.
- **Spawn state 0:** a difficulty interval of `-1` skips the spawn work and
  position snapshot. Otherwise, the delay is decremented **before** the test:
  `counter <= 0` enters phase 1.
- **Package state 1:** only records with a valid nonnegative node and package
  status **1** qualify. Retail counts qualifying entries among **four slots**,
  chooses a random **rank modulo that count**, then resolves that rank in slot
  order. It never uses `rand()%4` against unchecked slots.
- **Travel state 2:** advance one path index only when distance to the target
  node is **strictly below 3**. On the last waypoint, recheck the box status;
  available status 1 becomes taken status **2**, enters state **4**, plays
  managed voice **89**, and adds **7** to the animation counter. If the box is
  no longer available, enter state **6** without claiming it.
- **Handoff state 3:** clears the `0x512154` carried/collision latch and
  advances to phase 4.
- **Home state 7:** on arrival at its last waypoint, enter spawn state 0 with
  the fixed, **500-tick** cooldown, not the per-difficulty spawn interval.
- **Previous-position snapshot:** ordinary in-view state exits copy actor
  X/Y `0x510790/0x510794` to `0x510798/0x51079C`; the off-screen and
  disabled-spawn early exits skip that update.

These branches are covered by `maze_spud_controller_test.cpp` as a standalone
C++26 CTest regression. Numeric constants and state decisions come from
the identified 2002 retail binary, not guessed replacement behavior.

**State 4 route selection closed:** the original routine first rotates the
last direction by 180 degrees (`(previous + 2) % 4`) and picks a random
starting slot with `rand()%4`. It scans all four node links cyclically.
Pass 1 excludes that reverse direction, Spud's potential neighbor matching
the player's current node, any of the player's four linked neighbors, and
missing links (`-1`). Pass 2 allows the reverse direction while retaining
the player-neighborhood exclusions. Pass 3 allows any non-`-1` neighbor.
The implementation in `select_spud_return_neighbor` retains the retail
degenerate fallthrough: if every link is `-1`, the handler still selects
the original randomized direction and enters phase 5 with node `-1`.
`maze_spud_neighbor_test.cpp` checks all three scan passes, cyclic wrap,
and that failure case.

**State 5 carrying/collision closure:** its node-seeking speed is **1**
on Easy/Medium and **2** on Hard. The path-node distance is tracked as a
minimum in global `0x444238`. Upon node distance **<3**, the handler updates
the current node and branches to phase **4** if the collision latch was clear,
or phase **6** if it was already set. Only on that waypoint-arrival branch,
a `rand()%30 == 0` check may select voice **90..93**.

Every phase-5 frame independently measures player/Spud distance after truncating
both actors' coordinates. Distance **<60.0** with a clear collision latch
drops the package: it marks package status **3**, clears the carrying flag,
sets the collision latch, copies Spud X/Y into the box position by truncation,
and plays voice **94 or 95**. The order matters: if a waypoint is reached and
the collision happens on the **same frame**, the next phase was already
chosen using the old latch. The typed function preserves this and is covered
by `maze_spud_carry_test.cpp`, along with both strict threshold boundaries.

**State 6 return-route setup closure:** state 6 clears the collision
latch at `0x512154` and calls the existing shortest-path search from Spud's
current node to a **screen-dependent edge node**. On the middle screen
(index 1), `rand()%2 == 0` produces node **15** through a literal
`dec/neg/sbb/and 0xF` sequence; the other parity selects node **0**.
West/East always select **0**. The routine resets path index to zero,
enters phase **7**, clears the carrying flag, and sets the sprite timing
counter to `trunc(difficulty_ticks / 2)`.

Each path setup/waypoint advance derives direction by scanning the current
node's four link entries in stored order and accepting the **first** match
for the next path node. A missing match returns **4**, not `-1`. These
semantics are in `spud_return_route_build`,
`spud_random_edge_node` and `spud_direction_to_path_node`, with
`maze_spud_reset_test.cpp` covering parity, duplicate links, no match and
signed truncation.

**Still open:** source-level integration of these independent Spud helpers
into one stateful frame controller, preserving the original shared-global
writes, exact random draw order and original resource/audio manager calls;
then direct frame-by-frame comparison against retail. Eight state branches
are now mapped, but the Maze Spud NPC is not yet a complete stand-alone
reimplementation.
