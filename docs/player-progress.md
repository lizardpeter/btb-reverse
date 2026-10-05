# Player progress and Firework Finale unlock

The game keeps five player progress records. Each record contains exactly 100
signed 32-bit integers and is stored as ASCII integers in player1.txt through
player5.txt.

The in-memory base for player 0 is 0x0051B4D0 and each player record is
400 bytes (100 x 4).

## Visible progress block

Slots 50 through 64 are the only progress values cleared by retail profile
deletion and the only range consumed by UpdateProgressScreen.

| Slot | Player-0 address | Meaning |
|---:|---:|---|
| 50 | 0x0051B598 | Pets Corner |
| 51 | 0x0051B59C | Squirrel Run |
| 52 | 0x0051B5A0 | Bob's Band - Bob |
| 53 | 0x0051B5A4 | Bob's Band - Wendy |
| 54 | 0x0051B5A8 | Bob's Band - Farmer Pickles |
| 55 | 0x0051B5AC | Park Designer |
| 56 | 0x0051B5B0 | Raptor skeleton |
| 57 | 0x0051B5B4 | Triceratops skeleton |
| 58 | 0x0051B5B8 | T-Rex skeleton |
| 59 | 0x0051B5BC | Spud Maze |
| 60 | 0x0051B5C0 | Spud Skate |
| 61 | 0x0051B5C4 | Maze |
| 62 | 0x0051B5C8 | Golf |
| 63 | 0x0051B5CC | Firework Finale started |
| 64 | 0x0051B5D0 | reserved/unused |

Every slot 50 through 63 has an identified game-code writer.

No game-code writer exists for slot 64. It is only touched by generic player
load/save/delete code and by the Progress-screen sum loop, so normal retail
profiles leave it at zero.

## Thirteen prerequisites

The actual pre-finale requirements are exactly slots 50 through 62: 13 values.

This matches the original activity design:

- Pets Corner
- Squirrel Run
- Bob's Band with Bob
- Bob's Band with Wendy
- Bob's Band with Farmer Pickles
- Park Designer
- all three dinosaur species
- Spud Maze
- Spud Skate
- Maze
- Golf

The Dino completion write is species-based. The selected level index is
species-major:

species * 3 + difficulty

and the progress write divides by 3, producing species index 0, 1, or 2.

## Exact retail unlock arithmetic

The Activity Select updater walks slots 50 through 64 and sums positive values.
On every such pass it first reasserts the finale-locked/intercept flag. When
the sum reaches 13, it:

- sets the finale-unlocked latch to 1;
- clears the finale-locked/intercept flag.

The unlocked value is a **one-way runtime latch**: the below-threshold branch
does not clear it. If an already-unlocked in-memory state later observes a
below-threshold record, the locked flag can be reasserted by the sum pass while
the unlocked latch remains 1; the next Activity Select setup sees that latch
and does not reassert the lock.

Under normal retail-created saves the threshold is equivalent to completing all
13 prerequisites, because slots 63 and 64 are zero before the finale starts.

For edited/corrupt saves, the exact executable behavior matters: positive
values in slots 63 or 64 can contribute to the sum.

## Activity Select interception

Activity action 0x22 is the Firework Finale route.

While the finale-locked flag is set, selecting action 0x22 does not enter the
Fireworks pregame state and does not replace the outer game-flow state. Instead
retail stops managed sounds, loads:

- data/ui/Progress-screen.bmp
- data/ui/star.bmp

and opens the Mr Bentley progress view.

Selecting any **other** activity while the locked flag is set routes normally
and transiently clears that locked flag. Activity Select setup reasserts it on
return unless the unlocked latch has already been set.

Once the unlock latch is set, Activity Select no longer intercepts action 0x22
and the same tile routes directly to outer state **0x22**, the normal
Fireworks/Finale pregame setup.

## Finale progress flag

`InitializeFireworksActivity` writes slot 63 to `1` immediately at function
entry, before gameplay begins. The exact write is at `0x00411CE0`, indexed by
the current player.

Therefore slot 63 means **Firework Finale started/entered**, not completed.

This is now closed by exhaustive executable reference analysis:

- player-0 slot 63 address `0x0051B5CC` has exactly one game-code reference,
  the indexed initialization write above;
- player-0 slot 64 address `0x0051B5D0` has **no game-code reference at all**;
- reaching the certificate, pressing Print, accepting the shared leave-activity
  confirmation, saving/unloading Fireworks, and the dormant state-17 handler
  do not write any player-progress value.

There is therefore **no separate Firework Finale completed marker** in retail.
Slot 64 is not a hidden completion flag.

## Star layout

UpdateProgressScreen walks all 15 storage slots, but the visual layout was
designed around the 13 prerequisites.

The exact coordinate-index table is:

12, 11, 8, 9, 10, 0, 3, 4, 5, 1, 2, 6, 7, -1, 1

for slots 50..64 respectively.

Slot 63 resolves to (357,-1), effectively hiding its star above the screen.
Slot 64 reuses the same visible coordinate as index 1, but the slot remains
zero in normal retail saves.

## Grouped Mr Bentley feedback

Hovering prerequisite stars uses grouped progress narration:

- Bob's Band: slots 52..54
- dinosaurs: slots 56..58
- Spud activities: slots 59..60
- Adventure Playground: slots 61..62

Retail uses sound IDs 955/956 as positive-completion variants and 957..959
for one, two, or three items remaining.

The typed C++26 reconstruction is in player_progress.hpp/.cpp and tests.
