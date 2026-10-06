# Bob's Band / Grand Opening activity

Bob's Band is the Grand Opening music-sequencer activity in the executable range around:

- `0x0041D7C0 LoadGrandOpeningMachineData`
- `0x0041DB20 InitializeGrandOpeningActivity`
- `0x00420030 UpdateGrandOpeningActivity`

The activity lets the player assemble a short composition from animated/sounded machine clips, then play it under one of three conductors.

## Conductors

Retail conductor IDs are:

| ID | Conductor | Backing track |
|---:|---|---|
| 0 | Bob | `bobmt.wav` |
| 1 | Wendy | `Wendymt.wav` |
| 2 | Farmer Pickles | `fpmt.wav` |

Each player profile has a separate saved composition for each conductor.

## machinedata.txt

### `0x0041D7C0 LoadGrandOpeningMachineData`

Retail opens:

`data\\subgameOpen\\machinedata.txt`

and parses exactly **95 integers**:

1. 10 machine positions = 20 integers
2. 10 machine sizes = 20
3. 3 conductor positions = 6
4. 3 conductor sizes = 6
5. 3 conductor animation frame counts = 3
6. 10 four-integer static-animation rectangles = 40

Total:

```text
20 + 20 + 6 + 6 + 3 + 40 = 95
```

The source file itself contains original developer comments confirming those blocks.

### Retail conductor-position quirk

The executable parses the three conductor positions and three conductor sizes into stack locals, but does **not** copy those six pairs into persistent activity globals before returning.

The reconstruction preserves them as `parsed_but_unused_*` so the source file can be represented losslessly without falsely claiming that retail uses those values.

## Machine IDs

The initializer loads the 10 machine animation surfaces in this exact order:

| ID | Machine |
|---:|---|
| 0 | Roley 1 second |
| 1 | Roley 2 second |
| 2 | Muck 1 second |
| 3 | Muck 2 second |
| 4 | Lofty 1 second |
| 5 | Lofty 2 second |
| 6 | Dizzy 1 second |
| 7 | Dizzy 2 second |
| 8 | Scoop 1 second |
| 9 | Scoop 2 second |

The duration table at `0x00444BE8` is exactly:

```text
1 2 1 2 1 2 1 2 1 2
```

so even-numbered machine IDs occupy one timeline step and odd-numbered IDs occupy two.

## Composition grid

The persistent composition begins at `0x005123B8`.

Its shape is exactly:

```text
5 rows x 24 time steps = 120 int32 cells
```

A fully written save is therefore:

```text
120 * 4 = 480 bytes
```

Cell values are:

| Value | Meaning |
|---:|---|
| -1 | empty |
| 0..9 | event start, machine ID |
| 10 | continuation of a two-step event |

### Row meaning

The five rows are **musical pitch rows**.

Retail loads the 50 WAV groups as variation 1 through variation 5, with 10 machine types in each variation.

Playback computes the loaded sound slot as:

```text
slot = 40 - row*10 + machine_id
```

Therefore:

| Grid row | WAV suffix | Exact pitch |
|---:|---:|---|
| 0 | 5 | C# |
| 1 | 4 | B |
| 2 | 3 | A |
| 3 | 2 | G# |
| 4 | 1 | F# |

### Exact pitches

The WAV files themselves contain Broadcast Wave `bext` descriptions. The shipped Roley one-second clips identify suffixes as:

```text
roley1_1.wav -> F#
roley1_2.wav -> G#
roley1_3.wav -> A
roley1_4.wav -> B
roley1_5.wav -> C#
```

Retail maps suffix 5 to row 0 and suffix 1 to row 4, so the sequencer wall runs **C# / B / A / G# / F# from top to bottom**.

This also matches the original Bob's Band instructions: bricks placed higher in the wall play at a higher pitch, while lower bricks play at a lower pitch.

A grid cell therefore stores only the machine/duration type; the row supplies the exact musical pitch.

## Timeline

The sequencer has **24 one-second steps**, giving a total composition duration of **24 seconds**.

The playback routine derives elapsed time from the performance counter, converts the counter difference to centiseconds, then divides by 100 to obtain the current one-second timeline index.

This matches the machine design directly:

- 1-second machine -> one grid cell
- 2-second machine -> start cell + one continuation cell

## Placement

### `0x0041F3A0 CanPlaceGrandOpeningEvent`

Given a row, step, and machine ID, retail:

1. reads the machine span from the 1/2 duration table;
2. rejects spans that would pass step 23;
3. requires every target cell to be `-1`.

### `0x0041F4B0 PlaceGrandOpeningEvent`

On success:

```text
grid[row][step] = machine_id
```

and for a 2-second machine:

```text
grid[row][step+1] = 10
```

The source reconstruction implements this as `can_place_event` / `place_event`.

## Deletion

### `0x0041F430 RemoveGrandOpeningEventAtCell`

Deletion is owner-aware.

If the clicked cell contains continuation value 10, retail scans left until it reaches the owning machine start value 0..9.

It then:

1. clears the event start to -1;
2. reads that machine's 1/2-step duration;
3. clears all continuation cells belonging to the event.

The source reconstruction implements this as `remove_event_at`.

## Exact editor hit table

`0x0041FB90 HitTestBobsBandRegions` scans exactly **132** strict-interior
regions stored as 20-byte records.

The table is built by the initializer in this order:

1. **120 composition-wall cells** — 5 rows x 24 seconds, action **11**;
2. **10 irregular machine-palette regions** — actions **0..9**;
3. **secondary Stop region** — action **22**;
4. **legacy Play-region record** — action **23**.

The 120 wall rectangles are generated from one exact formula:

```text
left   = 44 + second * 23
top    = 310 + pitch_row * 19
right  = left + 23
bottom = top + 19
action = 11
```

This is also the exact draw origin for event-start bitmaps in
`DrawGrandOpeningCompositionAndMachines`.

The ten machine-palette rectangles are:

| Type | Rect |
|---:|---|
| 0 | (232,284)-(274,306) |
| 1 | (191,262)-(231,288) |
| 2 | (286,249)-(325,273) |
| 3 | (248,233)-(285,260) |
| 4 | (353,221)-(387,245) |
| 5 | (316,205)-(351,232) |
| 6 | (411,210)-(446,231) |
| 7 | (372,195)-(408,219) |
| 8 | (481,194)-(512,222) |
| 9 | (447,193)-(479,220) |

The special records are:

- action **22**: **(251,415)-(301,463)** — secondary Stop region, used by
  state-9 playback and by the delete-cursor normalization path;
- action **23**: **(340,415)-(395,463)** — present in the retail table but has
  no state-0/state-1 editor dispatch behavior in this build.

The hit test also applies a retail cursor-hotspot adjustment before checking
rectangles:

- state **1 / MachineSelected**: **+10,+10**;
- all other states: **+5,+5**.

All rectangle comparisons are strict: `x > left && x < right`,
`y > top && y < bottom`.

The complete 132-row table is retained in
`ghidra/bobs_band_editor_regions.csv`.

## Exact machine/conductor rendering

### Composition wall

Event-start cells 0..9 are color-key blitted at:

```text
x = 44 + 23 * second
y = 310 + 19 * pitch_row
```

Continuation value 10 and empty value -1 are not drawn as event bitmaps.

### Machine animation sheets

Each of the five machines has a short and long sprite sheet. State **0** draws
frame 0 of the short sheet. State **1** animates the short sheet; state **2**
animates the long sheet.

| Machine | Short dest | Short frame | Long dest | Long frame |
|---|---:|---:|---:|---:|
| Roley | (0,130) | 189x168 | (3,130) | 246x177 |
| Muck | (86,93) | 200x162 | (86,93) | 222x177 |
| Lofty | (225,2) | 165x238 | (225,2) | 177x242 |
| Dizzy | (342,105) | 84x102 | (342,105) | 85x103 |
| Scoop | (393,75) | 132x143 | (393,75) | 138x167 |

Short animations contain **15 frames**. Long animations contain **30**.

The machine animation tick increments by the shared retail animation delta.
Once the accumulated tick becomes greater than **3**, the source frame
advances and the tick resets. On reaching the end frame, retail rewinds the
animation to frame 0, **draws animated-sheet frame 0 once more on that update**,
then clears the machine animation state to static for the following frame.

### Conductor geometry and idle animation

The conductor sheet geometry is:

| Conductor | Dest | Frame size |
|---|---:|---:|
| Bob | (499,133) | 85x78 |
| Wendy | (504,132) | 68x80 |
| Farmer Pickles | (493,136) | 78x85 |

Outside playback, all three conductors use the same idle-frame policy:

- frame tick advances until **>5**, then advances one sprite frame;
- frames **0..19** are the ordinary idle cycle;
- when frame 20 is first reached, retail calls `rand()%3`;
- result **0** enters the special idle sequence **20..49**;
- results **1 or 2** immediately reset the conductor to frame 0;
- reaching frame 50 also resets to frame 0.

So there is a **1/3 chance** after each normal idle cycle to play the longer
special idle sequence.

During Bob's Band playback, the conductor uses the separate verified ranges
50..89 / 50..84 / 50..104 documented below.

The exact renderer constants are also retained in
`ghidra/bobs_band_visual_geometry.csv`.

## Toolbar

### `0x0041F820 UpdateGrandOpeningToolbar`

The four bottom controls are:

| Toolbar index | Control |
|---:|---|
| 0 | Play |
| 1 | Stop |
| 2 | Clear All |
| 3 | Delete |

Their retail hover/pressed overlays are:

| Control | Hover | Pressed | Blit origin |
|---|---|---|---:|
| Play | `mpplayred.bmp` | `mpplaydep.bmp` | (324,416) |
| Stop | `mpstopred.bmp` | `mpstopdep.bmp` | (260,416) |
| Clear All | `mpclearallred.bmp` | `mpclearalldep.bmp` | (103,416) |
| Delete | `mpdeletered.bmp` | `mpdeletedep.bmp` | (481,416) |

All are color-keyed overlays. State 9 also performs the separate action-22
hit test and, whenever that region is active, draws the same Stop-hover
`mpstopred.bmp` surface at **(260,416)**. A click through this secondary path
stops/resets the backing track and returns to editor state 0. Action 23 is
present in the hit table but is ignored by this state-9 secondary path.

### Play

Play leaves edit mode and enters activity state 8, then state 9.

During state 8 retail:

- records the current conductor as complete for the current player;
- captures performance-counter start values;
- starts the conductor-specific backing track;
- initializes playback timing.

### Stop

Stop is accepted during playback state 9 and returns the editor to state 0.

### Clear All

Clear All enters the game's confirmation overlay.

When the confirmed-clear flag is observed by the outer update, retail fills all 120 grid cells with `-1`.

### Delete

Delete toggles the delete cursor/tool. Clicking an event removes the entire owning event, including a two-second continuation cell.

### Exact editor action behavior

The editor-side state machine is now source-level too.

Palette actions **0..9** are the ten machine/duration bricks. From state 0,
selecting a palette brick:

- cancels Delete mode if active;
- installs that brick cursor;
- sets activity state **1 / MachineSelected**;
- previews the brick's direct machine WAV;
- maps type/2 to Roley/Muck/Lofty/Dizzy/Scoop and, only if that machine's
  animation state is idle, writes **1 for short** or **2 for long**.

While already in state 1, clicking the same palette type toggles selection off
and returns to state 0. Clicking a different type changes the held brick and
previews its WAV but does **not** kick the machine animation again.

Wall action **11** has two distinct retail paths:

- state 0: clicking an occupied cell removes the complete owning event,
  including continuation cells. With Delete off, the removed brick becomes the
  newly held cursor/type and state changes to 1. With Delete on, the event is
  deleted in place and a one-frame delete-interaction latch is raised.
- state 1: an empty cell attempts to place the held brick and returns to state
  0 on success. Clicking an occupied event start removes it, tries to put the
  held brick in that cell, and if successful swaps the removed brick into the
  cursor. If the replacement cannot fit, retail restores the removed event and
  keeps the original held brick. A continuation value 10 is **not**
  owner-resolved on this state-1 path; placement is attempted directly into the
  occupied continuation cell and therefore fails.

There is also a small original-code oddity around machine type **9 /
Scoop2Second**: on the replacement-failure restore path retail scans left over
preceding values >=9 before choosing the restore column. The reconstruction
preserves that address-level behavior rather than normalizing it away.

### Exact toolbar geometry and voices

The four bottom buttons use strict interior rectangle tests:

| Control | Rectangle |
|---|---|
| Play | (324,416)-(378,472) |
| Stop | (260,416)-(314,472) |
| Clear All | (103,416)-(157,472) |
| Delete | (481,416)-(535,472) |

Their conductor-specific managed voice base is:

| Conductor | Base ID | WAV family |
|---|---:|---|
| Bob | 510 | MP_BOB_01..13 |
| Wendy | 523 | MP_WEN_01..13 |
| Farmer Pickles | 536 | MP_PIC_01..13 |

First hover over a control after another/no control plays
`base + control_index + 6` at priority 50, flag 2.

Click behavior is exact:

- **Play**: edit state only, Delete off. Stops all managed sounds, sets a
  play-pending latch, and plays `base+1` at priority 50, flag 1. Retail does
  **not** enter state 8 immediately; a later toolbar update waits until managed
  audio is idle, clears the latch, then enters state **8 / PreparePlayback**.
- **Stop**: state 9 only, Delete off. Stops the backing track, returns to state
  0, and plays `base+3+rand()%2`.
- **Clear All**: edit state only, Delete off. Opens
  `data\\ui\\Deletebricks.bmp` with shared Yes/No context **2**. A
  nonzero confirmation result at the top of the activity update fills all 120
  grid cells with -1.
- **Delete**: edit state only. Entering installs the delete cursor and plays
  `base+9+rand()%2`; clicking Delete again restores the normal cursor and
  clears Delete mode.

Because hover processing runs before click dispatch, the first clicked frame
over a control can legitimately start **both** its hover voice and its action
voice. The reconstruction exposes them separately.

Delete mode itself is limited to the wall area. After editor input, if no
deletion occurred that frame and mouse Y is outside the strict
**290..400** band, retail clears Delete mode and restores the normal cursor. A
successful deletion raises a one-frame latch that suppresses this auto-cancel
once; the latch is then cleared.

Machine-readable details are in `ghidra/bobs_band_editor_actions.csv`.

## Playback

### `0x0041FC90 PlayAndDrawGrandOpeningComposition`

The routine:

1. draws the normal Grand Opening scene/composition;
2. computes current elapsed one-second timeline position;
3. walks all five variation rows;
4. examines the current timeline cell in each row;
5. ignores -1 and continuation marker 10;
6. converts machine ID + row into the correct loaded WAV slot;
7. restarts/plays that machine WAV;
8. advances the corresponding machine animation state;
9. animates the selected conductor over the backing track.

The machine WAV lookup is:

```text
loaded_sound_slot =
    40 - row*10 + machine_id
```

The exact playback trigger is now source-level. Retail stores the previous
elapsed centisecond count and only walks the five pitch rows when the derived
one-second timeline index changes. State 8 seeds the previous elapsed value
with **999999**, deliberately forcing second 0 to trigger on the first playback
update.

For every event-start cell 0..9 encountered on that new second, retail:

1. computes `40 - row*10 + machine_type`;
2. stops/resets that WAV if necessary and starts it again;
3. maps the machine type to Roley/Muck/Lofty/Dizzy/Scoop using `type/2`;
4. if that machine's animation state is zero, writes **1 for short** or
   **2 for long** from the type parity.

Continuation value 10 never triggers a WAV.

### Exact conductor playback animation

The playback conductor has a separate four-update animation clock.

Global `0x00512174` increments each playback update. When it becomes greater
than 3, retail resets it to zero, increments the conductor frame at
`0x00513F14`, and clamps/wraps that frame to the active conductor's exact
playback range:

| Conductor | Playback frames |
|---|---|
| Bob | **50..89** |
| Wendy | **50..84** |
| Farmer Pickles | **50..104** |

All three ranges therefore begin at frame **50**, and one animation frame lasts
four playback updates.

### State 8 playback preparation

State 8 is now represented exactly. It:

- advances the internal activity state to **9 / Playing**;
- writes shared completion/movie code **6** to `0x00446F38`;
- writes the selected conductor's progress flag for the current player;
- captures the performance-counter start value;
- starts the selected conductor backing track;
- writes previous elapsed centiseconds **999999**.

### Stop, backing-track end, and outer flow

During state 9, Stop immediately returns the activity to state 0 and stops the
backing track. If the backing track stops naturally, retail also returns to
state 0.

State **10 / ExitToPlayAgain**:

1. calls the shared `PreparePlayAgainTransition`;
2. sets outer state **0x3C / Play Again Yes/No**;
3. writes Play Again context **0x2E**;
4. saves/unloads the current Bob's Band composition;
5. clears the shared transition flag and returns from the activity.

Quit/leave always saves/unloads the current composition. An accepted leave
request normally returns to **0x2A / Music chooser** and clears the leave flag.
If the shared completion/movie code is active, retail overrides that route with
**0x40 / shared movie transition**.

The exact playback/outer-flow globals are machine-readable in
`ghidra/bobs_band_runtime_globals.csv`.

## Saved compositions

There are 15 retail composition filenames:

```text
musicbob1.txt ... musicbob5.txt
musicwendy1.txt ... musicwendy5.txt
musicfarmer1.txt ... musicfarmer5.txt
```

The filename index is:

```text
index = conductor*5 + player_profile
```

Each file is raw binary despite the `.txt` extension.

### Load

Initialization:

1. fills all 120 cells with -1;
2. chooses the conductor/player filename;
3. if it exists, reads 5 rows x 24 int32 values.

### Save

### `0x0041D920 SaveAndUnloadGrandOpeningActivity`

Cleanup writes the exact same 5x24 dword grid back to the selected file.

The reconstruction provides byte-exact `read_composition` and `write_composition` functions.

## last.txt legacy write

Cleanup also opens `last.txt` and writes the current conductor dword **five times**.

No corresponding read reference to `last.txt` has been found in this executable build.

The reconstruction therefore exposes this payload as legacy behavior but does not assign it a gameplay meaning.

## Progress / three-conductor requirement

The persistent progress array at `0x0051B5A0` uses a **100-dword / 400-byte stride per player profile**.

The exact compiler arithmetic is:

```text
player -> player*5 -> player*25 -> byte offset player*400
```

When playback is prepared, retail writes:

```text
progress[player*100 + conductor] = 1
```

for conductor values 0..2.

This is the executable-side representation of completing music with:

- Bob
- Wendy
- Farmer Pickles

for a player profile.

The reconstruction exposes:

- `conductor_progress_index`
- `all_conductors_complete`.

## Activity states

Verified state IDs include:

| State | Role |
|---:|---|
| 0 | edit |
| 1 | machine selected / placement cursor |
| 8 | prepare playback |
| 9 | playing |
| 10 | exit to Play Again |

States 2..7 exist in the retail switch but route through passive/common drawing behavior in this outer loop.

## Source reconstruction

Buildable source:

- `reconstruction/include/btb/grand_opening_data.hpp`
- `reconstruction/src/grand_opening_data.cpp`
- `reconstruction/include/btb/grand_opening_sequence.hpp`
- `reconstruction/src/grand_opening_sequence.cpp`
- `reconstruction/include/btb/grand_opening_runtime.hpp`
- `reconstruction/src/grand_opening_runtime.cpp`

Tests:

- `reconstruction/tests/grand_opening_data_test.cpp`
- `reconstruction/tests/grand_opening_sequence_test.cpp`
- `reconstruction/tests/grand_opening_runtime_test.cpp`

Current source-level coverage includes:

- exact machinedata loader
- exact machine ID/duration map
- exact 5x24 save grid
- exact conductor/player filename selection
- exact 50-WAV row mapping
- exact placement/continuation rules
- owner-aware deletion
- 24-second playback timeline
- Play/Stop/Clear/Delete toolbar IDs
- backing-track mapping
- conductor progress indexing
- exact once-per-second five-row WAV trigger runtime
- exact short/long machine-animation kick states
- conductor playback frame ranges and four-update clock
- exact state-8 playback preparation
- Stop/backing-track completion return behavior
- state-10 Play Again transition and save/unload
- quit/leave Music-chooser vs shared-movie routing
- exact 132-region editor hit table with state-dependent +5/+10 hotspot shift
- exact wall-cell render geometry and all ten palette rectangles
- exact short/long machine sprite-sheet geometry and 15/30-frame lifetimes
- exact conductor sheet geometry and randomized editor idle/special-idle clock.
