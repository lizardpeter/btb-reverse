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
tuning tuple: 1 2 2 3
alignment offset: 70
```

The header and 8x7 table are dynamically indexed by the animation/placement runtime. They are preserved with neutral names in reconstruction because their exact authored column labels are not present in the binary.

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

## Placement state machine

Global `0x005150E4` is a nine-state placement/interactor state.

| State | Reconstructed role |
|---:|---|
| 0 | idle / select conveyor item |
| 1 | move toward selected item, primary path |
| 2 | move toward selected item, alternate path |
| 3 | commit primary placement |
| 4 | commit alternate placement |
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

Observed primary-placement feedback pools include:

Lofty:

- 633 = `SR_LOF_02.wav`
- 634 = `SR_LOF_03.wav`
- 635 = `SR_LOF_04.wav`
- 636 = `SR_LOF_05.wav`

Wendy:

- 656 = `SR_WEN_14.wav`
- 657 = `SR_WEN_15.wav`
- 658 = `SR_WEN_16.wav`

Observed alternate-placement feedback pools include:

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
- preservation of discarded and unread legacy values;
- exact 0-8 placement-state IDs;
- exact +/-2 / <=3-snap movement rule;
- verified startup and placement sound groups;
- exact three-stage completion gate and final Wendy sound range.
