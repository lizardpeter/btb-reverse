# Fireworks activity

The Fireworks activity is the second minigame being taken from address-level notes into source-level reconstruction.

## Main entry points

- `0x00411C80 InitializeFireworksActivity`
- `0x00413F10 UpdateFireworksActivity`
- `0x004115C0 LoadFireworkMovieBank` (high-confidence working name)
- `0x00411B10 SaveFireworksLayoutAndUnloadResources`

The activity mixes DirectDraw UI composition with a preloaded Bink bank for the firework effects.

## Rectangle data

The initializer reads:

- 18 records from `Data\\SubGameFirework\\graph\\fireworks.txt`
- 12 records from `Data\\SubGameFirework\\graph\\fireworkboxes.txt`

Both files use eight integers per record, representing four 2D corners. The retail executable keeps only corner 0 and corner 2, giving the top-left and bottom-right rectangle.

For `fireworks.txt`, the executable subtracts 20 from all four retained coordinates during the temporary parse. When copying those records into the runtime hit table, it adds 30 back to the two X fields only. The exact final transform is therefore `x + 10, y - 20`.

For `fireworkboxes.txt`, the retained rectangle is copied without that placement shift.

This also explains why malformed values in redundant second/fourth vertices do not change behavior: those values are read but discarded.

## Runtime region table

The initializer builds one common interactive-region table:

1. 18 placement regions, each assigned action/type 12
2. 12 palette regions, assigned type IDs 0 through 11
3. three hard-coded control regions assigned action IDs 27, 25, and 26

That adds 33 activity-specific interactive regions.

The update path beginning around `0x004126F0` iterates these records, hit-tests the current mouse position, and branches on the action/type field.

## Firework type map

The 12 palette bitmap load order is identical to the first 12 entries of the Bink effect bank.

| Type | Firework | Left/single clip | Right clip |
|---:|---|---|---|
| 0 | red airbomb | airbombredleft.bik | airbombredright.bik |
| 1 | small green | smallgreenleft.bik | smallgreenright.bik |
| 2 | medium red | mediumleftred.bik | mediumrightred.bik |
| 3 | large blue | bigblueleft.bik | bigblueright.bik |
| 4 | large red | bigredleft.bik | bigredright.bik |
| 5 | medium green | Mediumgreenleft.bik | Mediumgreenright.bik |
| 6 | small blue | smallblueleft.bik | smallblueright.bik |
| 7 | blue airbomb | airbombblueleft.bik | airbombblueright.bik |
| 8 | red candle | Candlered.bik | single/symmetric |
| 9 | red spinner | Wheelred.bik | single/symmetric |
| 10 | green spinner | Wheelgreen.bik | single/symmetric |
| 11 | blue candle | Candleblue.bik | single/symmetric |

For types 0 through 7, bank index `type + 12` is the matching right-hand movie.

Machine-readable maps:

- `ghidra/firework_types.csv`
- `ghidra/firework_movies.csv`

## 23-entry Bink bank

`0x004115C0` iterates exactly 23 movie paths.

- 0-7: left-facing variants of directional firework types
- 8-11: candle/spinner clips
- 12-19: right-facing variants
- 20: `topmiddle.bik`
- 21: `fireworkcrowdloop.bik`
- 22: `fireworkcrowdend.bik`

The routine opens each source file through the CD-fallback path, reads it into retained memory, initializes a per-movie playback/surface record, and then opens the Bink handle from that retained data. This appears designed to avoid repeated CD seeks while playing a user-authored show.

## Authored 3x6 show grid

The user's show is stored as exactly **18 signed 32-bit integers**, arranged as 3 rows x 6 columns at global `0x0050A678`.

- `-1` = empty
- `0..11` = a Firework type
- the code contains generic continuation-marker support for multi-cell items (`type + 14`)
- the shipped per-type span table at `0x00442734` contains only `1` values, so every retail firework occupies exactly one cell

### Persistence

There are five player-specific filenames in a 50-byte-stride table at `0x00442638`:

- `firedata1.txt`
- `firedata2.txt`
- `firedata3.txt`
- `firedata4.txt`
- `firedata5.txt`

Despite the `.txt` extension, these files are **binary**. The initializer reads them as 18 four-byte integers and `0x00411B10 SaveFireworksLayoutAndUnloadResources` opens the selected file with mode `wb` and writes all 18 values as 4-byte records.

## Exact editor controls

The combined editor hit table contains 33 regions:

- 18 placement cells -> action 12
- 12 palette cells -> actions 0..11
- Play: `(292,416)-(346,472)` -> action 27
- Delete All: `(102,416)-(156,472)` -> action 25
- Delete Selected: `(483,416)-(537,472)` -> action 26

The three button meanings are proven from the loaded surfaces and control code:

- **27 Play** -> internal state 14
- **26 Delete Selected** -> toggles internal state 13 and changes the cursor; clicking a placement slot in state 13 removes that item
- **25 Delete All** -> opens `data\\ui\\DeleteFireworks.bmp` through the shared Yes/No confirmation modal; a Yes result clears all 18 slots to `-1`

## Placement path

`0x004126F0 HitTestFireworksEditorRegions` resolves the region under the pointer.

`0x00412B50 DecodeSelectedFireworkGridCell` converts a placement-region index into:

`row = index / 6`, `column = index % 6`.

`0x00412BD0 BeginFireworksEditorAction` handles palette press, existing-slot pickup/replacement, Delete All, and Play.

`0x00412D50 CompleteFireworksEditorAction` stores the selected type or validates a placement release. The actual grid commit occurs after the short placement animation, using `PlaceFireworkGridItem(..., commit=1)`.

## Fireworks internal state machine

The activity has its own state global at `0x0050A5BC`, distinct from the 68-state outer game flow.

Observed states:

| State | Meaning |
|---:|---|
| 0 | normal editor |
| 1 | editor drag/release interaction variant |
| 2 | initialize selected-firework preview event |
| 3 | play selected-firework preview Bink |
| 4 | finish preview and return to editor |
| 8 | prepare authored show: stop sounds, unload editor, load movie bank, initialize timing/events |
| 9 | active authored show playback via `UpdateFireworksShowPlayback` |
| 13 | delete-selected mode |
| 14 | post-show wait / begin completion movie |
| 15 | play `Data\\movies\\fireworkcomplete.bik` |
| 16 | certificate/results screen |
| 17 | finalize completion, save grid, return to outer Play Again flow |

States 5-7 and 10-12 currently dispatch to no-op targets in the retail jump table.

## Show playback

`0x00413450 UpdateFireworksShowPlayback` advances through the six authored columns. For each column it can activate up to three events, one for each row.

Each active event stores its type, playback state, and row. Row determines display position and Bink-bank addressing:

| Row | Position | Retail movie index rule |
|---:|---|---|
| 0 | `(0,0)` | `type` |
| 1 | `(440,0)` | `type + 12` |
| 2 | `(220,200)` | `type` |

The middle-row arithmetic is unconditional. Therefore retail types 8, 9, and 10 in row 1 map to bank indices 20, 21, and 22 (the special top-middle/crowd clips), while type 11 computes index 23, one past the verified 23-entry bank. The source reconstruction intentionally records this quirk rather than normalizing it.

The show also drives the crowd-loop/crowd-end clips and timed crowd/voice sound effects. When the show reaches its terminal phase, it switches the internal state to 16.

## Completion flow

State 14 waits for the relevant managed sound activity to settle, clears the display, and opens `Data\\movies\\fireworkcomplete.bik`, then enters state 15.

State 15 runs the shared Bink player. When the movie completes it returns through show setup and starts the completion sound path.

State 16 draws the certificate/results screen through `0x004138A0 DrawFireworksCertificateScreen`, including the common print function.

State 17:

1. updates persistent progress
2. changes outer game-flow state to `0x3C` (Play Again Yes/No)
3. records Fireworks completion
4. calls `SaveFireworksLayoutAndUnloadResources`

At this point the Fireworks editor data model, persistence, control actions, movie bank, authored show sequencing, and completion route are all structurally recovered.


## Authored 3x6 timeline

The editor's authored sequence is the 18-dword table beginning at global `0x0050A678`.

It is interpreted as:

```text
row 0: slots  0.. 5
row 1: slots  6..11
row 2: slots 12..17
```

So each row has six **timeline columns**. Empty cells contain `-1`; occupied cells contain a palette type `0..11`.

### Grid helpers

The core helpers are now high-confidence:

- `0x00410FD0 CanPlaceFireworkGridItem(row, column, type)`
- `0x00411010 PlaceFireworkGridItem(row, column, type, commit)`
- `0x00411080 RemoveFireworkGridItem(row, column)`

The executable contains a generic per-type span table at `0x00442734`. The helper can reserve consecutive cells and uses `type + 14` as continuation markers when span > 1.

However, the verified retail table entries for all 12 shipped Fireworks palette types are **1**. Therefore every actual firework occupies exactly one cell in this build. The multi-cell implementation is generic/dead capability rather than retail level data.

### Validate then commit

Placement is intentionally two-phase.

On release over a candidate authored cell, `0x00412D50 CompleteFireworksEditorAction` calls:

```text
PlaceFireworkGridItem(row, column, selected_type, commit=false)
```

If valid, it records the pending row/column and starts a short placement animation.

When that animation reaches its commit state, the update path around `0x00413347` calls the same helper with:

```text
commit=true
```

Only then is the selected type written into `0x0050A678`.

That distinction is reproduced by the source-level `Sequence::validate_placement` / `Sequence::commit_placement` API.

### Replace behavior

Clicking an already occupied placement region in the ordinary editor first removes its old type. The current palette selection can then validate/commit into the now-empty cell. This gives the editor its replace behavior.

## Editor controls

The three hard-coded bottom controls are now fully identified.

| Action | Rectangle | Control |
|---:|---|---|
| 25 | ~X 102..156, Y 416..472 | **Delete All** |
| 26 | ~X 483..537, Y 416..472 | **Delete tool** |
| 27 | ~X 292..346, Y 416..472 | **Play** |

### Delete All

Action 25 loads:

`data\\ui\\DeleteFireworks.bmp`

and invokes the shared `UpdateYesNoConfirmationOverlay` using confirmation context 3. On Yes, the Fireworks update loop observes result flag `0x0051C2DC` and fills all 18 authored slots with `-1`.

### Delete tool

Action 26 uses:

- `fireworkdeletered.bmp`
- `fireworkdeletedep.bmp`

The editor enters internal mode **13** and swaps to the delete cursor. In mode 13, clicking an occupied placement region calls `RemoveFireworkGridItem`.

### Play

Action 27 uses:

- `fireworkplayred.bmp`
- `fireworkplaydep.bmp`

It moves the activity into its play/show sequence.

Machine-readable action metadata is in `ghidra/firework_editor_actions.csv`.

## Show playback ordering

The authored show is played **column by column**, not by row.

`0x00413450 UpdateFireworksShowPlayback` advances a timeline column from 0 through 5. For each current column it checks all three rows, creating up to three simultaneous event records.

For an occupied cell, an event records:

- firework type
- timeline row
- playback state/frame fields

### Row-to-screen/movie behavior

The show player uses the row directly:

| Row | Screen origin | Movie-bank arithmetic |
|---:|---|---|
| 0 | `(0, 0)` | `movie = type` |
| 1 | `(440, 0)` | `movie = type + 12` |
| 2 | `(220, 200)` | `movie = type` |

Thus rows 0 and 1 behave like left/right banks for directional types 0..7, while row 2 uses the base clip at a central/lower position.

The source-level sequence model exposes this exact arithmetic through `retail_movie_index_for_row`.

### Retail edge case

The code adds 12 for row 1 **unconditionally**. Therefore:

- row-1 type 8 -> bank index 20 (`topmiddle.bik`)
- row-1 type 9 -> bank index 21 (`fireworkcrowdloop.bik`)
- row-1 type 10 -> bank index 22 (`fireworkcrowdend.bik`)
- row-1 type 11 -> bank index 23, one beyond the verified 23-entry bank

The placement validator does not itself reject those combinations.

This may be an original-game constraint enforced indirectly by UI expectations, or an original retail bug. The reconstruction records the behavior exactly rather than silently normalizing it.

Machine-readable row rules are in `ghidra/firework_playback_rows.csv`.

## source-level reconstruction status

The Fireworks reconstruction now includes:

- source rectangle parser
- runtime placement X offset
- all 12 palette types
- verified 23-entry Bink bank
- exact 3x6 authored timeline
- empty/occupied state
- validate/commit placement split
- replacement
- remove/delete-all
- occupied/full counts
- column-based playback event generation
- exact retail row coordinates and Bink-index arithmetic

What remains for a fully source-level Fireworks recreation is mainly the **show scheduler/timing and Bink event lifetime**, plus the detailed editor animation/hover state.


## Exact retail show cadence

The scheduler inside `0x00413450 UpdateFireworksShowPlayback` is now resolved directly from the timing arithmetic.

It computes:

```text
column = elapsed_ms / 400
remainder = elapsed_ms % 400
```

and only launches authored-grid cells while:

```text
column < 6
```

This matches the six authored timeline columns exactly.

Within each 400 ms column, rows are intentionally staggered:

| Row | Launch offset within column |
|---:|---:|
| 0 | 0 ms |
| 1 | 100 ms |
| 2 | 200 ms |

So the nominal scheduled launch time for a cell is:

```text
launch_ms = column * 400 + row_offset
```

The final authored column therefore launches at approximately:

- row 0: 2000 ms
- row 1: 2100 ms
- row 2: 2200 ms

The scheduler uses guard flags so each row threshold is emitted once per 400 ms window.

### Active event record

Each launched firework is copied into one of the active runtime records beginning at approximately `0x005093F0`. The observed record stride is **16 bytes**:

| Offset | Meaning |
|---:|---|
| `+0x00` | firework type, `-1` = free event slot |
| `+0x04` | playback/event state |
| `+0x08` | frame/substate field |
| `+0x0C` | authored row index |

When the event's Bink clip completes, the event type is reset to `-1`, freeing the slot.

This exact cadence is now represented in `fireworks_sequence.hpp` through `kTimelineColumnPeriodMs`, `retail_row_launch_offset_ms`, and `retail_scheduled_launch_ms`.
