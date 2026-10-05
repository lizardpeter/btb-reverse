# Spud Maze activity

Spud Maze is entered through:

- `0x004207E0 InitializeSpudMazeActivity`
- `0x00423F40 UpdateSpudMazeActivity`

Its main graph/tuning file is `loaddata/spudmaze_nodes.txt`.

## Retail loader

### `0x00420560 LoadSpudMazeData`

The loader reads five screen graphs separated by tokens beginning with `NEXT` and terminated by `END`.

Each node record is:

`<label> <index> <x> <y> <direction_bits> <node_type> <link0> <link1> <link2> <link3>`

Retail stores each node as eight int32 values after the source index chooses the slot.

### Y correction

Immediately after reading a node, retail performs `node.y -= 11`.

### Screen capacity

The flattened node table reserves 30 records per screen, so the retail maximum is 30 nodes for each of the five screens.

### NEXT marker quirk

The shipped text is inconsistent: one separator is written as `NEXT_2nd Screen`, while later separators are single tokens such as `NEXT_3rdScreen`.

The reconstruction accepts both spellings and produces the same effective five-screen graph.

## Data after END

The exact read order is:

1. 20 triples = 5 screens x 4 `(x,y,node)` reference records
2. one float player speed
3. 3 regular Spud speeds
4. 3 package-carrying Spud speeds
5. 3 difficulty timer values
6. 3 Spud spawn times
7. one Spud animation-delay integer
8. five additional screen-specific coordinate lists

### Player-speed quirk

The file contains `2.0`, but immediately after reading it the executable overwrites the runtime value with IEEE-754 `4.0f`.

So the actual retail runtime speed is 4.0 even though the source file says 2.0.

### Difficulty values

| Difficulty | Regular speed | Package speed | Timer | Spawn time |
|---|---:|---:|---:|---:|
| Easy | 1 | 1 | 500 | -1 |
| Medium | 1 | 1 | 400 | 1000 |
| Hard | 2 | 2 | 300 | 200 |

A spawn time of `-1` disables that spawn path on Easy.

### Animation delay

The shipped value is `6`.

## Final coordinate lists

Retail reads these with fixed compile-time/static counts rather than sentinels:

| Screen | Point count |
|---:|---:|
| 0 | 8 |
| 1 | 5 |
| 2 | 4 |
| 3 | 6 |
| 4 | 9 |

They are the damaged skateboard-track **repair spots**. Retail reserves 11 status slots per screen, uses the fixed counts above, draws selected damaged sections from the numbered `1-1.bmp` ... `5-9.bmp` assets, and clears a spot when Bob begins repairing it.

## Source reconstruction

Current source:

- `reconstruction/include/btb/spud_maze_data.hpp`
- `reconstruction/src/spud_maze_data.cpp`
- `reconstruction/tests/spud_maze_data_test.cpp`

It preserves the five-screen graph, node links/types, 11-pixel Y correction, reference triples, all difficulty tuning, the ignored file speed, retail 4.0 runtime speed, and fixed trailing list sizes.

## Difficulty damage selection

The activity does **not** mark all 32 repair spots as broken.

During initialization retail:

1. computes a phase seed as `rand()%4 + 1`;
2. walks all 32 repair spots in screen order;
3. marks a section damaged when:

```text
counter % difficulty_divisor == 0
```

4. increments the counter after every spot.

The divisors are:

| Difficulty | Divisor | Resulting damaged sections |
|---|---:|---:|
| Easy | 8 | exactly 4 |
| Medium | 5 | 6 or 7 |
| Hard | 3 | 10 or 11 |

Every selected damaged spot gets status 1 in the retail status table at `0x00514110`; every selected spot also increments global repairs-remaining at `0x005144C8`.

The initial value is copied to `0x00513F38`, giving retail both the original repair count and the mutable remaining count.

The exact selector is reproduced by `select_repair_damage`.

## Screen-transition node types

`UpdateSpudMazeBobMovement` tests the special node-type bits directly.

| Bit | Meaning |
|---:|---|
| `0x80` | transition one screen left, upper lane |
| `0x400` | transition one screen left, lower lane |
| `0x100` | transition one screen right, upper lane |
| `0x800` | transition one screen right, lower lane |

For a left transition retail performs:

```text
screen -= 1
node -= 2
transition_direction = 1
```

For a right transition:

```text
screen += 1
node += 2
transition_direction = 2
```

The upper/lower distinction comes from the source graph's paired edge nodes around Y=300 and Y=386.

These semantics are represented by `ScreenTransition` and `apply_screen_transition` in the source reconstruction.

## Repair interaction

The repair loop in `DrawAndUpdateSpudMazeScene` iterates the current screen's repair points and tests Bob's current position against damaged sections.

Mouse click or Space near a damaged section enters one of two paths.

### Bob has the hammer

Retail:

- sets Bob animation/state to repair state 9;
- marks the repair animation active;
- sets a five-step repair sub-counter;
- clears that repair spot's damaged status;
- plays a non-repeating random line from **702..708**:
  - `SS1_BOB_09.wav` through `SS1_BOB_15.wav`.

`UpdateSpudMazeBobAnimation` runs the repair animation. When its final repair sub-step completes, retail decrements `0x005144C8`, the repairs-remaining counter.

### Bob does not have the hammer

The repair does not begin. Retail plays one of:

- 709 = `SS1_BOB_16.wav`
- 710 = `SS1_BOB_17.wav`

## Pilchard and the hammer

### `0x00423470 HandlePilchardCollisionAndHammerDrop`

Retail continuously checks Bob/Pilchard distance.

A valid collision requires:

- collision cooldown expired;
- Bob currently has the hammer;
- Bob is not in the active repair animation;
- the remaining positional guards pass.

On collision:

```text
hammer_screen = current_screen
hammer_x = bob_x + (bob_direction == 1 ? 10 : -10)
hammer_y = bob_y + 41
has_hammer = 0
collision_cooldown = 500
```

It also activates the dropped-hammer animation/render state.

Bob reaction sound is selected from:

- 696..701 = `SS1_BOB_03.wav` through `SS1_BOB_08.wav`

and, if neither Pilchard reaction is already playing, retail adds one of:

- 720 = `SS1_PIL_03.wav`
- 721 = `SS1_PIL_04.wav`.

### Hammer recovery

While Bob does not have the hammer and remains on the screen where it was dropped, the scene-update path draws/animates `hammer.bmp`.

Walking Bob back into the pickup range sets `has_hammer = 1` again. One verified horizontal pickup guard is an absolute X delta of **less than 20 pixels** after retail's hammer sprite offset adjustment.

## Activity success and timeout

`UpdateSpudMazeActivity` exits the active game when either:

```text
repairs_remaining <= 0
```

or:

```text
timer < 0
```

These outcomes are deliberately different.

### Success

If repairs remaining reaches zero:

- the player/activity completion flag is updated;
- common progress/save processing runs;
- the activity unloads;
- game flow advances to shared Play Again state `0x3C`.

### Timeout

If the timer becomes negative while repairs remain:

- the activity still exits to the shared completion/play-again flow;
- **the success/progress flag is not set**.

At timer value exactly **15**, retail stops managed sound and plays:

- 994 = `lowtime.wav`

The source reconstruction exposes this distinction as `ActivityOutcome::Success` versus `ActivityOutcome::Timeout`.

## Startup sequence

Before normal repair gameplay, a short character sequence runs through `UpdateSpudMazeMrBentleyIntro`.

Normal startup then:

- starts shared activity music index **7** = `Skateboardfix.wav`;
- plays sound 694 = `SS1_BOB_01.wav`;
- marks the startup voice persistent through the managed-sound system.

## Runtime source

The runtime reconstruction now includes:

- `spud_maze_runtime.hpp`
- `spud_maze_runtime.cpp`
- `spud_maze_runtime_test.cpp`

It currently preserves:

- exact edge-transition bits and screen/node remapping
- retail periodic damage-selection algorithm
- exact difficulty repair counts
- success-vs-timeout semantics
- hammer-drop geometry/cooldown
- repair/no-hammer/Pilchard sound groups
- startup music/voice constants
