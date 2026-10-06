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

## Main Bink loop / pass markers

The final pair is:

```text
44 660
```

The first value is the synchronized-stream seek target used when a Bink reaches
its actual movie end. All four quality streams are returned to exact frame
**44** through `0x00424C80 SeekAndDecodeBinkToExactFrame`.

The second value, **660**, is not simply a one-shot "end of game" frame. Global
`0x00514EA8` counts how many times the primary stream has reached frame 660:

- first encounter: counter becomes 1 and gameplay continues;
- second encounter: counter becomes 2, playback phase changes to 1, and retail
  starts `end.bik`.

Thus the synchronized stunt sequence runs through a first pass, continues to
the actual Bink end, seeks back to frame 44, and then runs a second pass until
frame 660.

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

## Exact score and two-pass runtime

The runtime globals are now closed:

| Global | Meaning |
|---|---|
| `0x00514EA4` | current quality: 0 Bad, 1 Normal, 2 Okay, 3 Good |
| `0x00514EA8` | number of times frame 660 has been reached |
| `0x00514EAC` | playback phase: 0 synchronized run, 1 end movie, 2 complete |
| `0x00514EB0` | running score |
| `0x0044656C` | active stunt index, or -1 |

On a successful stunt press, retail derives exactly the same numeric quality
used by the four Bink streams:

```text
0 = bad.bik
1 = normal.bik
2 = ok.bik
3 = good.bik
```

At a stunt's configured sound-trigger frame, the game selects a non-`-1`
sound from `sound_ids[stunt][quality]` and then performs:

```text
score += current_quality
```

So the exact weights really are:

- Bad = **0**
- Normal = **1**
- Okay = **2**
- Good = **3**

### Why the retail maximum is 45, not 24

The eighth stunt has sound trigger frame **665**, but the pass marker is
**660**. Retail therefore has a special first-pass frame-660 branch that uses
the final stunt's sound row and adds its current quality to the score before
playback later wraps to frame 44.

The scoring opportunities are consequently:

- pass 1: stunts 0..6 at their normal trigger frames + stunt 7 at the special
  frame-660 branch = **8 scores**;
- pass 2: stunts 0..6 at their normal trigger frames; the second frame-660
  encounter enters `end.bik` before stunt 7 can score = **7 scores**.

Total: **15 scoring opportunities**.

With Good worth 3, the exact maximum is therefore:

```text
15 * 3 = 45
```

This replaces the earlier provisional 24-point maximum.

### Exact per-frame ordering

Within the synchronized-run branch, retail processes the important gameplay
events in this order:

1. if this is a configured stunt sound-trigger frame, play the quality-row
   sound and add the current quality to score;
2. if this is the active stunt's return frame, reset quality to Bad and clear
   the active stunt;
3. if stunt input is active and no stunt is active, classify the current
   timing window and latch quality 1/2/3;
4. if frame == 660, increment the pass-marker count:
   - hit 1: special-score final stunt;
   - hit 2: enter `end.bik`.

The ordering matters because sound/score happens before return-to-Bad, and
frame-660 input is classified before the special final-stunt score.

### Result presentation quirk

When playback phase becomes 1, the result voice branch runs once and marks
player progress slot **60 / SpudSkate** complete.

The score thresholds are exactly **10** and **20**, but the low branch has a
retail fallthrough:

- score <10: play one of **773..774 (SS2_SPU_16/17)** and then also fall
  through to play one of **775..777 (SS2_SPU_18..20)**;
- score 10..19: play one of **775..777**;
- score >=20: play one of **778..779 (SS2_SPU_21/22)**.

After the result Bink reaches its final frame and managed audio has finished,
playback phase becomes **2**.

When `UpdateSpudSkateActivity` sees phase 2 it:

1. runs the shared Play Again transition;
2. unloads Spud Skate resources;
3. enters main game-flow state `0x3C` (Play Again Yes/No).

The exact gameplay runtime is now represented in
`reconstruction/include/btb/spud_skate_runtime.hpp` and
`reconstruction/src/spud_skate_runtime.cpp`.


## Exact synchronized rendering

The initializer opens exactly five Bink streams:

1. `bad.bik`
2. `normal.bik`
3. `ok.bik`
4. `good.bik`
5. `end.bik`

Each uses a **600x380** DirectDraw surface. During synchronized gameplay the
current quality value 0..3 directly selects which of the four quality surfaces
is presented at **(20,20)**, while all four Binks remain synchronized.

When the quality Binks reach their actual movie end, retail resets all four and
uses `0x00424C80 SeekAndDecodeBinkToExactFrame` to bring each one back to the
timing-file loop start **frame 44**.

The remaining UI surfaces are:

- `minimised.bmp` — activity background;
- `skatespudscore.bmp` — 10 score digits, each **24x50**;
- `1.bmp`, `2.bmp`, `3.bmp` — Normal/Okay/Good quality overlays.

Score rendering is exact:

- tens, only when nonzero: **(450,415)**;
- ones, always: **(475,415)**;
- source digit N: `(N*24,0)-((N+1)*24,50)`.

For quality >0, the matching `1.bmp` / `2.bmp` / `3.bmp` surface is drawn
at **(267,415)**.

The runtime/global map is machine-readable in
`ghidra/spud_skate_runtime_globals.csv`.

## Source-level presentation contract

The presentation layer is now represented directly in:

- `reconstruction/include/btb/spud_skate_presentation.hpp`
- `reconstruction/tests/spud_skate_presentation_test.cpp`

The exact retail surface bindings are:

| Global | Asset | Role |
|---|---|---|
| `0x00514E8C` | `bad.bik` | Bad-quality 600x380 movie surface |
| `0x00514E90` | `normal.bik` | Normal-quality movie surface |
| `0x00514E94` | `ok.bik` | Okay-quality movie surface |
| `0x00514E98` | `good.bik` | Good-quality movie surface |
| `0x00514E9C` | `end.bik` | result/end movie surface |
| `0x00514EA0` | `minimised.bmp` | activity background |
| `0x005148B4` | `skatespudscore.bmp` | score-digit strip |
| `0x00514B60` | `1.bmp` | Normal overlay |
| `0x00514B64` | `2.bmp` | Okay overlay |
| `0x00514B68` | `3.bmp` | Good overlay |

During synchronized play:

```text
quality 0 -> bad.bik
quality 1 -> normal.bik + 1.bmp
quality 2 -> ok.bik     + 2.bmp
quality 3 -> good.bik   + 3.bmp
```

Only qualities 1..3 draw an overlay. The movie destination remains
**(20,20), 600x380**. The overlay destination is **(267,415)**.

The score plan is exact and independent of quality:

- ones digit always at **(475,415)**;
- tens digit only when nonzero at **(450,415)**;
- each source digit is a **24x50** cell from `skatespudscore.bmp`.

When playback phase becomes `EndMovie`, the selected synchronized quality
surface is replaced by `end.bik`; no quality overlay is present. The score
remains represented by the same digit plan while the result/end-movie phase is
active.

The four quality streams remain synchronized as a set. When they hit their
actual movie end, the runtime requests all four to be reset and exact-seeked to
the timing-file loop start, shipped value **44**. This is represented by
`quality_stream_set` and the existing
`PlaybackFrameStep::resync_quality_streams/resync_frame` contract.

The machine-readable global/asset map is
`ghidra/spud_skate_surface_map.csv`.

## Outer activity flow and retail cleanup quirk

`0x00424D10 UpdateSpudSkateActivity` now has a source-level outer controller.

The one-time startup path:

- starts activity music index **8 = Skateboardrace.wav**;
- plays managed sound **758 = SS2_SPU_01.wav** at priority 50, flag 1;
- clears the shared startup-audio latch.

Normal completion, after playback phase reaches 2:

1. raises the shared front-end blocking flag;
2. runs `PreparePlayAgainTransition`;
3. calls the correct `0x00424100 UnloadSpudSkateActivityResources`;
4. sets outer state **0x3C / Play Again Yes/No**;
5. writes the retail navigation metadata values **0x16** and **0x1A** and clears
   the shared transition flag.

The quit/leave path has a notable original-code asymmetry. Instead of calling
Spud Skate's own unloader, retail calls **0x0041A390
UnloadMazeActivityResources**. The reconstruction intentionally records this
as a retail cleanup bug rather than silently correcting it.

If the shared leave-current-activity flag caused the exit, retail clears that
flag and initially routes to **0x16 / Spud chooser**. If the shared completion/
movie code at `0x00446F38` is non-`-1`, it overrides that route with
**0x40 / shared movie transition**.

## Source reconstruction

Current source:

- `reconstruction/include/btb/spud_skate_data.hpp`
- `reconstruction/src/spud_skate_data.cpp`
- `reconstruction/include/btb/spud_skate_runtime.hpp`
- `reconstruction/src/spud_skate_runtime.cpp`
- `reconstruction/include/btb/spud_skate_presentation.hpp`
- `reconstruction/tests/spud_skate_data_test.cpp`
- `reconstruction/tests/spud_skate_runtime_test.cpp`
- `reconstruction/tests/spud_skate_presentation_test.cpp`

It now implements:

- exact 8-window timing format
- difficulty timing thresholds
- Normal/OK/Good frame classification
- eight return-to-Bad frames
- exact synchronized Bink quality selection
- frame-44 seek / two-hit frame-660 pass lifecycle
- exact 15-opportunity, 45-point scoring model
- exact 8×4×5 sound matrix and eight normal trigger frames
- special first-pass final-stunt score at frame 660
- result-tier voice behavior, including the low-score fallthrough quirk
- exact Bink/surface asset bindings
- quality-to-movie and quality-to-overlay presentation mapping
- exact movie, score-digit and quality-overlay geometry
- end-movie presentation selection
- four-stream frame-44 synchronization contract
