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

The exact gameplay role of those five point lists is still being traced from the runtime.

## Source reconstruction

Current source:

- `reconstruction/include/btb/spud_maze_data.hpp`
- `reconstruction/src/spud_maze_data.cpp`
- `reconstruction/tests/spud_maze_data_test.cpp`

It preserves the five-screen graph, node links/types, 11-pixel Y correction, reference triples, all difficulty tuning, the ignored file speed, retail 4.0 runtime speed, and fixed trailing list sizes.