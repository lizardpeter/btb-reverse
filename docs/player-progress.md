# Player progress and Firework Finale unlock

The game has five player profiles. Each profile owns a fixed progress record of
**100 signed 32-bit integers (400 bytes in memory)**.

The five records begin at `0x0051B4D0` with a stride of `0x190` bytes.

## Save files

Retail uses:

- `player1.txt`
- `player2.txt`
- `player3.txt`
- `player4.txt`
- `player5.txt`

Despite the fixed int32 in-memory representation, these files are **text**.
The save routine writes exactly 100 values using `%d `; the load routine
zeroes the 100-value record and reads the same format back.

The source-level equivalent is:

- `reconstruction/include/btb/player_progress.hpp`
- `reconstruction/src/player_progress.cpp`
- `reconstruction/tests/player_progress_test.cpp`

## Activity progress block

The Progress screen treats indices 50 through 64 as the visible activity
progress block.

| Index | Meaning |
|---:|---|
| 50 | Pets Corner |
| 51 | Squirrel Run |
| 52 | Bob's Band — Bob conductor |
| 53 | Bob's Band — Wendy conductor |
| 54 | Bob's Band — Farmer Pickles conductor |
| 55 | Park Designer |
| 56 | Dinosaur Discovery — Raptor |
| 57 | Dinosaur Discovery — Triceratops |
| 58 | Dinosaur Discovery — T-Rex |
| 59 | Spud Maze / skateboard repair |
| 60 | Spud Skate |
| 61 | Maze |
| 62 | Golf |
| 63 | Firework Finale entered |
| 64 | unresolved retail progress slot |

The machine-readable version is `ghidra/player_progress_slots.csv`.

## Thirteen pre-finale requirements

The intended pre-finale requirement block is exactly indices **50..62**:
13 values.

This agrees with the original manual:

- every other activity must have been completed;
- Bob's Band must have been played with **all three conductors**;
- Dinosaur Discovery must have completed **all three dinosaur skeletons**.

The Dino completion write is especially useful evidence. Retail computes the
selected Dino level as:

```text
level_index = species * 3 + difficulty
```

and divides that value by 3 when selecting the persistent completion slot.
Therefore the three Dino progress flags are species flags, not difficulty flags.

## Exact retail unlock arithmetic

The Progress-screen update loops over **all 15 values 50..64**.

For each positive value it adds the full integer value to a running total.
It then performs:

```text
FinaleLocked = 1

if progress_sum >= 13:
    FinaleUnlocked = 1
    FinaleLocked = 0
```

The relevant globals are:

- `0x0051C304` — Firework Finale locked/intercept-click flag
- `0x0051C308` — Firework Finale unlocked latch

Under normal gameplay, slots 63 and 64 are zero before the finale, so reaching
13 means all thirteen prerequisite flags 50..62 are complete.

The reconstruction deliberately also preserves the executable's exact arithmetic:
an edited/corrupt save can satisfy the threshold using positive values in 63/64.

## Activity Select behavior

The Activity Select table assigns action **34 / 0x22** to the
`open` activity tile. State `0x22` is the already recovered Fireworks
pregame state.

When `FinaleLocked != 0` and the player clicks action 0x22, the generic UI
handler intercepts the action rather than changing the game-flow state. It
loads:

- `data\\ui\\Progress-screen.bmp`
- `data\\ui\\star.bmp`

and raises the Progress-screen overlay state.

When the finale is unlocked, the lock flag is zero, the interception does not
occur, and action 0x22 proceeds normally into the Firework Finale.

During Activity Select setup, if the unlocked latch is still zero, retail
explicitly reasserts `FinaleLocked = 1`.

## Finale-side slots

Index 63 is not a prerequisite. `InitializeFireworksActivity` writes it to 1
as the finale starts.

Index 64 remains unresolved. It is included in the retail progress-screen sum,
but no direct writer has yet been identified. The reconstruction therefore
keeps it as `Unknown64` rather than inventing a certificate/completion meaning.
