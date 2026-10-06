# Lofty's Squirrel Run

Squirrel Run occupies the executable range beginning at `0x00424E20` and is entered through:

- `0x00425160 InitializeSquirrelActivity`
- `0x004279F0 UpdateSquirrelActivity`

The activity is a conveyor-selection puzzle. The retail instructions match the executable: the player chooses an object on the conveyor, Lofty fetches it and carries it to the next run position, and successful placement advances the squirrel/run progression.

## Assets

The activity loads:

- `level1.bmp`
- `level2.bmp`
- `level3.bmp`
- `squriel_1_86.bmp`
- `nutpile.bmp`
- `LOFTYmoves8bit.bmp`
- `loftyarmswing_8bit.bmp`
- `body_01.bmp`
- `hook.bmp`
- `eyes.bmp`
- `mouth.bmp`
- `conveyor.bmp`
- `allconveyor.bmp`
- `all.bmp`
- `top.bmp`, `left.bmp`, `right.bmp`, and `bottom.bmp`

## sqdata.txt

### `0x00424E20 LoadSquirrelData`

Retail opens:

`Data\\SubGameSquirrel\\sqdata.txt`

and consumes exactly **71 integers**:

1. one 7-integer header;
2. eight 7-integer table rows;
3. one animation/phase delay integer;
4. two integers read into a temporary and discarded;
5. one four-integer tuning tuple stored in globals;
6. one final alignment/offset integer.

The shipped loaded values are:

```text
header:
0 3 10 27 8 -4 -4

8 x 7 table:
0 0 1  -1  0 0 0
0 0 0   0  0 0 0
0 0 1 -17  0 0 0
0 0 1 -33  0 0 0
0 0 -1 17  0 0 0
0 0 19 13  0 0 0
0 0 0 -32  0 0 0
0 0 0  32  0 0 0

delay: 7
discarded: 105 79
loaded-but-unused scalars: 1 2 2 3
initial horizontal alignment offset: 70
```

The header/table semantics are now closed from the only live consumers in
`0x00425F40 UpdateAndDrawSquirrelRunAssembly`.

The seven header values are the shared **horizontal movement deltas** for seven
animation/motion substeps:

```text
substep: 0  1  2  3  4   5   6
dx:      0  3 10 27  8  -4  -4
```

Every full seven-substep movement therefore advances **+40 pixels in X**.

The 8x7 table contains **vertical movement profiles** for those same substeps:

| Profile | Seven Y deltas | Net Y | Live? |
|---:|---|---:|---|
| 0 | `0 0 1 -1 0 0 0` | 0 | yes |
| 1 | `0 0 0 0 0 0 0` | 0 | yes |
| 2 | `0 0 1 -17 0 0 0` | -16 | yes |
| 3 | `0 0 1 -33 0 0 0` | -32 | yes |
| 4 | `0 0 -1 17 0 0 0` | +16 | yes |
| 5 | `0 0 19 13 0 0 0` | +32 | yes |
| 6 | `0 0 0 -32 0 0 0` | -32 | **loaded but not selected** |
| 7 | `0 0 0 32 0 0 0` | +32 | **loaded but not selected** |

Retail splits the seven steps into two movement phases: **0..2** and **3..6**.

`0x004259D0 SelectSquirrelPlacementMotionProfile` first selects compact motion
keys 0..13. The renderer maps those keys to vertical profiles with the exact
stack table:

```text
key:      0 1 2 3 4 5 6 7 8 9 10 11 12 13
profile:  0 0 3 3 5 5 4 4 2 2  0  0  1  1
```

Consequently profiles 6 and 7 are genuine authored/loaded legacy rows but have
no live selector key in this executable build.

The C++ model now names these as `horizontal_motion_deltas`,
`vertical_motion_deltas`, `VerticalMotionProfile`, and
`vertical_profile_for_motion_key`. The machine-readable evidence is in
`ghidra/squirrel_motion_profiles.csv`.

### Remaining loaded scalars

The two values **105 79** are scanned into a stack temporary and never stored.

The next four values **1 2 2 3** are stored into globals
`0x51505C`, `0x515060`, `0x515064`, and `0x514F04`.
An exhaustive executable reference sweep finds only the loader writes: there is
**no later read of any of these four globals** in this build. They are therefore
loaded-but-unused legacy scalars, not live runtime tuning.

The final value **70** is live. It is stored at `0x515084` and read exactly
once by `InitializeSquirrelActivity`, where retail computes the initial run X
position as:

```text
initial_run_x = fixed_x_base - 70
```

The reconstruction names it `initial_horizontal_alignment_offset`.

### Unread legacy tail

After the 71 consumed integers the shipped file still contains:

```text
110 0
234 146
384 177
362 203
172 0
```

The retail function closes the file before these five pairs. They are therefore **not runtime Squirrel data in this executable build**.

The reconstruction exposes a separate tooling helper to inventory them without allowing them to affect the retail parse result.

## Conveyor generation

### `0x00426F30 GenerateSquirrelConveyorChoices`

This function constructs the currently offered conveyor pieces from:

- difficulty;
- current level/run section;
- current run progress/type state;
- randomized choice generation.

It fills the activity's active-item flags, piece values, and conveyor item positions.

The routine also chooses a randomized starting offset using `rand()%7`.

## Difficulty and conveyor mix

`GenerateSquirrelConveyorChoices` sets an explicit correctness flag for every offered conveyor item.

The player-selection branch reads that flag directly:

- flag 1 -> state 1 -> state 3 -> correct-placement branch;
- flag 0 -> state 2 -> state 4 -> decoy-placement branch.

The number of choices is difficulty-dependent:

| Difficulty | Correct choices | Decoys | Total visible choices |
|---|---:|---:|---:|
| Easy | 3 | 0 | 3 |
| Medium | 3 | 1 | 4 |
| Hard | 2 | 2 | 4 |

The initializer independently computes the visible-choice count as `3 + (difficulty != Easy)`, matching the generator.

### 36-piece encoding

Every generated piece value is in the range 0..35 and is constructed as:

```text
piece_id = outer_variant * 9 + inner_a * 3 + inner_b
```

Therefore the retail piece space is exactly:

```text
4 outer variants x 3 inner-A values x 3 inner-B values = 36
```

The reconstruction exposes `encode_piece_id` and `decode_piece_id` but deliberately keeps the two inner dimensions neutrally named until their authored meaning is closed from rendering and run geometry.

### Seven outer-variant permutations

Retail chooses `rand()%7` and indexes a static 7x4 permutation table:

```text
0 1 2 3
0 3 2 1
1 3 2 0
1 0 2 3
3 2 0 1
2 1 3 0
3 1 2 0
```

This gives generated choices distinct/randomized outer variants without changing the separate correct/decoy flag.

## Connector-chain construction

The 12 random values created by `InitializeSquirrelActivity` are a **3-level connector plan**:

```text
3 levels x 4 connector values
```

Every source value is initially generated with `rand()%3`.

Retail then forces continuity between levels:

```text
level[1][0] = level[0][3]
level[2][0] = level[1][3]
```

So each level begins with the connector state where the preceding level ended.

Each level contains exactly **3 required placements**:

```text
piece 0: connector[0] -> connector[1]
piece 1: connector[1] -> connector[2]
piece 2: connector[2] -> connector[3]
```

For those normal piece indices, `GenerateSquirrelConveyorChoices` builds every correct choice with exactly that required from/to pair.

Decoys are generated by repeatedly drawing `rand()%3` until:

```text
decoy.from != required.from
decoy.to   != required.to
```

Thus a retail decoy deliberately mismatches **both** connector dimensions.

The source reconstruction now represents this with:

- `RunPlan`
- `chain_run_plan`
- `required_connectors`
- `is_correct_connector_pair`
- `is_retail_decoy_pair`.

### Piece ID meaning

The 36-piece encoding can therefore be named more specifically:

```text
piece_id =
    outer_variant * 9
  + from_connector * 3
  + to_connector
```

The four outer variants remain a separate visual/object dimension; correctness is determined by the connector pair and the explicit generated correctness flag.

## Retail puzzle geometry

The four selectable conveyor item origins are:

```text
(100,300)
(189,300)
(370,300)
(459,300)
```

Each is tested with strict interior bounds:

- width 89
- height 133.

The three run placement targets used by states 3/4 are:

```text
(116,327)
(276,327)
(436,327)
```

Lofty's home/return target used by states 5/6 is:

```text
(93,185)
```

These values are now constants in `squirrel_runtime.hpp`.

## Level count and run-end gate

Difficulty also controls how many run levels must be completed:

- Easy: 1 level = 3 correct placements
- Medium: 2 levels = 6 correct placements
- Hard: 3 levels = 9 correct placements

Retail's level index is compared directly with the difficulty index.

When the run-section animation/progression value becomes greater than 6:

- if `level_index < difficulty`, retail starts the next level transition;
- if `level_index >= difficulty`, it marks the activity complete.

At the moment the final level is recognized, retail stops managed sound and plays one of **664..667**:

- 664 = `SR_WEN_22.wav`
- 665 = `SR_WEN_23.wav`
- 666 = `SR_WEN_24.wav`
- 667 = `SR_WEN_25.wav`

The outer completion stage then separately performs its final 665..667 Wendy line before exiting.

## Placement state machine

Global `0x005150E4` is a nine-state placement/interactor state.

| State | Reconstructed role |
|---:|---|
| 0 | idle / select conveyor item |
| 1 | move toward selected item, correct-piece path |
| 2 | move toward selected item, decoy-piece path |
| 3 | commit correct placement |
| 4 | commit decoy placement |
| 5 | return placement actor home |
| 6 | alternate return |
| 7 | move to return/recycle point |
| 8 | carry/return piece to conveyor |

The primary/alternate names describe the observed branch structure; they do not assume an undocumented original developer enum name.

### State 0: player selection

Retail scans conveyor item rectangles while input is active.

On click:

- the selected conveyor index is stored;
- the item's active/valid flag chooses state 1 or state 2.

This is the executable implementation of the manual's “click on an object from the conveyor belt” interaction.

### Fetch and placement

States 1 and 2 move Lofty's placement actor toward the selected conveyor item.

Arrival advances to states 3 or 4 respectively.

State 3 performs the main accepted/progression branch:

- copies the selected piece value into the current run storage;
- updates available conveyor state;
- advances run/section progression;
- either starts a squirrel/run transition or ultimately sets the activity-complete flag.

State 4 performs the alternate placement/feedback branch and advances through the return/recycle states.

### Motion helper

`0x00427270 StepSquirrelPlacementActorTowardTarget` moves X and Y independently by **2 pixels per update**.

For each axis:

- if absolute remaining distance is greater than 3, move by +/-2;
- otherwise snap to the target coordinate.

It reports arrival only after both axes are at their target.

This exact rule is reproduced by `step_toward_target`.

## Placement feedback audio

Observed correct-placement feedback pools include:

Lofty:

- 633 = `SR_LOF_02.wav`
- 634 = `SR_LOF_03.wav`
- 635 = `SR_LOF_04.wav`
- 636 = `SR_LOF_05.wav`

Wendy:

- 656 = `SR_WEN_14.wav`
- 657 = `SR_WEN_15.wav`
- 658 = `SR_WEN_16.wav`

Observed decoy-placement feedback pools include:

Lofty:

- 637 = `SR_LOF_06.wav`
- 638 = `SR_LOF_07.wav`

Wendy:

- 645 = `SR_WEN_03.wav`
- 646 = `SR_WEN_04.wav`
- 648 = `SR_WEN_06.wav`
- 649 = `SR_WEN_07.wav`
- 650 = `SR_WEN_08.wav`
- 651 = `SR_WEN_09.wav`
- 652 = `SR_WEN_10.wav`

The exact conditional choice among these pools remains in the original branch structure; the source reconstruction keeps the verified groups without inventing dialogue semantics.

## Intro / startup audio

The outer activity sequence uses:

- 643 = `SR_WEN_01.wav`
- 632 = `SR_LOF_01.wav`
- 644 = `SR_WEN_02.wav`

as its staged startup/help dialogue.

## Completion

Global `0x005150E0` is the complete flag.

When complete, `UpdateSquirrelActivity` continues drawing the scene and runs a separate completion stage at `0x00515058`.

### Stage 0

Retail:

1. stops managed sounds;
2. chooses `rand()%3`;
3. plays one of:
   - 665 = `SR_WEN_23.wav`
   - 666 = `SR_WEN_24.wav`
   - 667 = `SR_WEN_25.wav`;
4. advances to stage 1.

### Stage 1

The activity waits until no managed sound remains active.

It then advances to stage 2.

### Stage 2+

Retail:

- marks Squirrel Run complete in player/progress data;
- executes the common progress/save update;
- calls `UnloadSquirrelActivityResources`;
- changes outer game flow to **0x3C**, the shared Play Again flow.

## Source reconstruction

Buildable source:

- `reconstruction/include/btb/squirrel_data.hpp`
- `reconstruction/src/squirrel_data.cpp`
- `reconstruction/include/btb/squirrel_runtime.hpp`
- `reconstruction/src/squirrel_runtime.cpp`

Tests:

- `reconstruction/tests/squirrel_data_test.cpp`
- `reconstruction/tests/squirrel_runtime_test.cpp`

Current source coverage includes:

- exact retail sqdata read boundary;
- exact seven-step horizontal motion deltas and eight vertical motion profiles;
- exact 14-key -> live vertical-profile mapping;
- proof that loaded profiles 6/7 and the four stored legacy scalars are unused;
- exact initial horizontal alignment use of the final loaded value 70;
- preservation of discarded and unread legacy values;
- exact 0-8 placement-state IDs;
- exact +/-2 / <=3-snap movement rule;
- verified startup and placement sound groups;
- exact three-stage completion gate and final Wendy sound range.
