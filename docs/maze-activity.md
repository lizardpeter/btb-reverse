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

## Source reconstruction

The typed graph parser is in:

- `reconstruction/include/btb/maze_data.hpp`
- `reconstruction/src/maze_data.cpp`
- `reconstruction/tests/maze_data_test.cpp`

It reproduces:

- three screen sections
- node IDs and positions
- retail Y offset
- direction mask
- four directional links
- node type
- 3x4 reference-node table
- player speed
- all difficulty tuning tables
- animation delay

Next Maze work is runtime traversal: player interpolation between linked nodes, screen portal transitions, Spud spawning/path selection, package behavior, timer/completion, and collision with the player.
