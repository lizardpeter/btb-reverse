# Spud Skate activity

Spud Skate is the `subgamespudskate` activity entered through:

- `0x00424360 InitializeSpudSkateActivity`
- `0x00424D10 UpdateSpudSkateActivity`

The activity is built around four synchronized Bink clips:

- `bad.bik`
- `normal.bik`
- `OK.bik`
- `good.bik`

with frame timing supplied by one of three difficulty files and sound feedback supplied by `soundinfo.txt`.

## Difficulty timing files

Retail selects one of:

- `spuddata1.txt` — Easy
- `spuddata2.txt` — Medium
- `spuddata3.txt` — Hard

The files themselves include original developer comments describing the format.

### `0x00424230 LoadSpudSkateTimingData`

The loader reads:

1. stunt count
2. `N` records of four frame markers
3. `N` return/end frames
4. one final two-integer loop-frame pair

The shipped stunt count is **8**.

Each four-frame record is interpreted by the retail input code as:

| Field | Meaning |
|---|---|
| 0 | Normal timing threshold / stunt window start |
| 1 | OK timing threshold |
| 2 | Good timing threshold |
| 3 | stunt window end, exclusive |

A button/key press is accepted only when the current Bink frame is inside:

```text
normal_start <= frame < end_exclusive
```

Retail then compares backward from the fourth marker and derives quality:

- `[normal_start, okay_start)` -> Normal
- `[okay_start, good_start)` -> OK
- `[good_start, end_exclusive)` -> Good

No successful input leaves the quality at 0, Bad.

### Difficulty tightening

The later difficulty files narrow the timing windows while keeping the same overall stunt layout.

For stunt 0:

- Easy: `85 97 107 116`
- Medium: `88 99 108 115`
- Hard: `94 103 110 115`

so the Hard Normal/OK windows are substantially tighter.

## Return-to-Bad frames

After the eight timing-window records the file contains eight frame values:

```text
134 202 271 360 426 531 592 679
```

Global `0x0044656C` stores the currently active stunt index.

When the current frame reaches that stunt's return frame, retail:

- resets active quality to Bad (0);
- clears the active stunt index to `-1`.

This is the file comment's “returns to bad.bnk” behavior.

## Main Bink loop range

The final pair is:

```text
44 660
```

The first value is passed into the Bink synchronization/seek helper for the four activity movie streams. The second is compared against the current Bink frame as the end of the main synchronized run.

The reconstruction names them:

- `loop_start_frame = 44`
- `loop_end_frame = 660`

## soundinfo.txt

The sound file has:

`8 stunts × 4 qualities × 5 candidate IDs`

followed by eight sound-trigger frames.

The original comments define the four rows for each stunt as:

1. Bad stunt voice overs
2. Normal stunt effects
3. OK stunt effects
4. Good stunt effects

`-1` means “no sound in this candidate slot.”

The final trigger frames are:

```text
124 180 260 330 415 500 580 665
```

### Retail sound selection

During playback retail determines which stunt section the current frame belongs to.

When the frame equals that stunt's trigger frame, it:

1. indexes `sound_ids[stunt][current_quality]`;
2. picks `rand()%5`;
3. retries if that candidate is `-1`;
4. plays the resulting managed sound ID.

This means Bad/Normal/OK/Good feedback is fully data-driven from the same quality value chosen by the stunt timing window.

## Score / completion observations

The main display code shows a numeric score using the score bitmap. The activity's running score global is updated from stunt quality values as playback progresses.

The exact score weighting is being traced separately; it is not guessed in the source parser.

When the activity's playback state reaches 2, `UpdateSpudSkateActivity`:

1. marks the activity complete;
2. runs the common progress/save update;
3. releases Spud Skate resources;
4. enters main game-flow state `0x3C` (Play Again Yes/No).

## Source reconstruction

Current source:

- `reconstruction/include/btb/spud_skate_data.hpp`
- `reconstruction/src/spud_skate_data.cpp`
- `reconstruction/tests/spud_skate_data_test.cpp`

It implements:

- exact 8-window timing format
- difficulty timing thresholds
- Normal/OK/Good frame classification
- eight return-to-Bad frames
- the 44/660 loop range
- exact 8×4×5 sound matrix
- eight sound trigger frames
