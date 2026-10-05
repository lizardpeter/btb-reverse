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

## Toolbar

### `0x0041F820 UpdateGrandOpeningToolbar`

The four bottom controls are:

| Toolbar index | Control |
|---:|---|
| 0 | Play |
| 1 | Stop |
| 2 | Clear All |
| 3 | Delete |

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

Player progress uses a 25-dword stride.

When playback is prepared, retail writes:

```text
progress[player*25 + conductor] = 1
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

Tests:

- `reconstruction/tests/grand_opening_data_test.cpp`
- `reconstruction/tests/grand_opening_sequence_test.cpp`

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
- conductor progress indexing.
