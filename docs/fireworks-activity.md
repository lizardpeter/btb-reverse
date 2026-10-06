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

### Exact placement actor channels

The editor has two independent actor-state channels, stored at
`0x0050AB20` and `0x0050AB24`.

They split the 3x6 grid by authored row:

| Channel | Authored rows | Voice family |
|---|---|---|
| 0 / Bob | rows 0 and 1 | `ZFE_BOB_*` |
| 1 / Wendy | row 2 | `ZFE_WEN_*` |

The directly written channel states are:

| State | Meaning |
|---:|---|
| 0 | idle |
| 1 | palette/pick-up latched |
| 10 | placement queued |
| 11 | placement commit |
| 12 | dormant legacy movement handler; present in DrawFireworksEditor but no retail writer exists |

On placement release, row 2 selects Wendy's channel; rows 0/1 select Bob's.
Retail stores the pending row/column, writes channel state **10**, and plays a
random placement voice before calling `PlaceFireworkGridItem(..., commit=0)`
for validation.

The sound pools are exact:

- Bob rows 0/1 placement release: **867..870 = ZFE_BOB_13..16**
- Wendy row 2 placement release: **904..907 = ZFE_WEN_13..16**
- Bob palette selection: **862..866 = ZFE_BOB_08..12**
- Wendy palette selection: **899..903 = ZFE_WEN_08..12**

`DrawFireworksEditor` advances **10 -> 11** on one frame. On the next frame,
state 11 calls `PlaceFireworkGridItem(..., commit=1)` using the current
selected type, then clears that channel to 0. It counts all 18 authored cells;
if the grid is now full, it plays **860 = ZFE_BOB_06.wav** for the Bob channel
or **897 = ZFE_WEN_06.wav** for the Wendy channel.

The state-12 branch is now decoded source-level despite being dormant. It
increments the actor animation tick, advances the source row every six draws,
and wraps rows **25 -> 13**. It calls the shared retail angle helper toward a
static per-actor target, rounds the integer angle into one of eight 45-degree
sprite columns using `(angle + 22) / 45`, then moves exactly:

```text
x += trunc(sin(angle * 0.0174535308) *  5)
y += trunc(cos(angle * 0.0174535308) * -5)
```

The static targets are **Bob (260,210)** and **Wendy (260,20)**. After moving,
retail computes Euclidean distance to the target; if it is strictly **<10.0**,
the channel returns to state 0 and source column 4. The angle helper itself
uses the original 9999.0 vertical sentinel and x87 truncate-toward-zero
conversion, which the C++ reconstruction preserves.

Exhaustive writes to the two channel-state globals still show **no write of
value 12**, so this is genuine dormant/legacy code rather than a normal
placement transition.

### Exact bottom-control behavior

`0x00413B60 UpdateFireworksEditorControls` is now reconstructed at the
decision level.

Before hit-testing, if **either** actor channel is positive, retail tests the
three bottom controls using the mouse point shifted by **(+40,+20)**.

A shared cooldown at `0x0050AB84` is handled before any control work: positive
values are decremented and the function returns immediately.

The control behavior is:

| Control | Hover | Click |
|---|---|---|
| Play / 27 | **881 = ZFE_BOB_28** | **880 = ZFE_BOB_27**, enter state 14 |
| Delete Selected / 26 | **882 = ZFE_BOB_29** | **890 = ZFE_D_WEN_01**, enter state 13; clicking again leaves state 13 with no click voice |
| Delete All / 25 | **883 = ZFE_BOB_30** | **885 = ZFE_BOB_33**, open generic Yes/No context 3 |

Play is ignored unless **both** actor channels are idle.

Delete Selected has one unusual conflict branch: when the shared interaction
gate is active and either actor channel is specifically state **1**, retail
clears both channels, returns the Fireworks state to editor state 0, restores
the normal cursor, installs a **10-frame cooldown**, and returns.

The previous-hover control latch at `0x00442A30` is **not reset** when the
mouse leaves all three controls. Consequently, leaving a control into empty
space and returning to that same control does not replay its hover voice until
some other control has replaced the remembered action.

The typed C++26 model is in
`reconstruction/include/btb/fireworks_editor.hpp` and
`reconstruction/src/fireworks_editor.cpp`, with a dedicated editor test.

The state-0 dispatcher ordering is now represented exactly too. A normal hit
calls `BeginFireworksEditorAction(action)` and then immediately
`CompleteFireworksEditorAction(action)` for the **same action**.

That exposes two retail details that were easy to miss when those helpers were
modeled independently:

- palette actions 0..11: the begin half changes the cursor and only latches/
  voices Bob or Wendy if that channel is idle, but the complete half writes
  `selected_type = action` **unconditionally**. Therefore clicking another
  palette item while the actor channel remains state 1 can change the selected
  firework without replaying the palette voice;
- placement action 12: the begin half removes an existing start item from the
  clicked cell before the complete half validates replacement. This is the
  exact reason occupied cells are replaceable even though the release half by
  itself rejects a cell that still contains a start type 0..11.

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
| 14 | Play-button transition / prepare `Data\\movies\\fireworkcomplete.bik` |
| 15 | play the pre-show `fireworkcomplete.bik` transition |
| 16 | certificate/results screen |
| 17 | legacy save/unload + Play Again handler; present in jump table but no retail write of state 17 has been found |

States 5-7 and 10-12 currently dispatch to no-op targets in the retail jump table.

### Exact selected-firework preview states 2 -> 3 -> 4

The short editor preview path is now source-level too.

- **state 2 / PreviewSetup** redraws the editor, copies the current
  `selected_type` into preview record `0x005093F0`, zeros the two following
  preview playback dwords, and increments the internal state to 3;
- **state 3 / PreviewPlayback** redraws the editor and calls
  `DecodeAndBlitBinkFrame` on the base-bank movie indexed directly by the
  selected type at screen origin **(199,39)**;
- while that movie is still active, state 3 remains state 3;
- when the Bink helper reports completion, retail increments to state 4 and
  calls `RestartBinkMovie` on the same movie;
- **state 4 / PreviewFinish** redraws the editor once, then writes internal
  state 0.

This preview path always uses the direct palette index **0..11**. It does not
apply the authored-show row-1 `type+12` arithmetic.

The typed preview controller is in
`reconstruction/include/btb/fireworks_editor.hpp`.

### Exact actor visual initialization

The Bob/Wendy editor actor visual state is now represented explicitly as well.
`InitializeFireworksActivity` initializes:

| Actor | X | Y | source column | source row | frame tick |
|---|---:|---:|---:|---:|---:|
| Bob | 260 | 168 | 4 | 0 | 0 |
| Wendy | 260 | 0 | 4 | 0 | 0 |

The actor sheets use **128x128** cells. `DrawFireworksEditor` builds each
source rectangle as:

```text
left   = source_column * 128
top    = source_row * 128
right  = left + 128
bottom = top + 128
```

The ordinary idle-animation branch forces source column **4**, increments a
per-actor frame tick each draw, advances the source row when the tick exceeds
5, clears the tick, and wraps source row **9 -> 0**. Thus idle row animation
advances every six editor draws through rows 0..8.

These visual fields and the exact source-rectangle/idle-tick helpers are now in
`fireworks_editor.hpp`.

### Exact Bob/Wendy mouse-follow controller

`DrawFireworksEditor` chooses one actor to idle and one actor to track the
shared mouse X coordinate every frame.

The normal vertical split is **Y=160**:

- mouse Y >160: Wendy idles, Bob tracks;
- mouse Y <=160: Bob idles, Wendy tracks.

If either placement actor channel is in palette-latched state **1**, that split
changes to **Y=35**.

Mouse X is clamped to **61..575**. The tracking actor follows the cursor by
exactly **one pixel per update**, using its sprite center `x+64`.

Bob's center path is the four-point polyline:

```text
(0,177) -> (240,296) -> (400,296) -> (640,177)
```

Wendy's center path is:

```text
(0,128) -> (300,128) -> (500,128) -> (640,128)
```

Retail linearly interpolates the current path Y, then subtracts the 128-pixel
sprite height to obtain the actor's top-left Y.

Standing uses source column **4** and animation rows **0..8**. Walking uses the
path slope plus left/right motion to select one of the other seven directional
columns and advances rows **13..24**.

Global `0x0050AB7C` is a small hysteresis value, not a timer. It starts at 0,
so an actor stops when its center is within **30 pixels** of mouse X. Once it
starts moving, retail writes 25, narrowing the stop threshold to **5 pixels**.
When the actor stops, the value returns to 0.

### Exact editor frame composition

The DirectDraw composition order in `0x00412E50 DrawFireworksEditor` is now
reconstructed end-to-end:

1. opaque **`Bk_01e.bmp`** at (0,0);
2. authored grid row 0 firework bitmaps;
3. authored grid row 2 firework bitmaps;
4. update Bob/Wendy mouse-follow animation;
5. **Wendy** sprite sheet;
6. color-keyed **`middle.bmp`** at **(205,104)**;
7. **Bob** sprite sheet;
8. color-keyed **`bottom.bmp`** at **(37,284)**;
9. process actor channel states 10/11/12;
10. authored grid row 1 firework bitmaps.

The grid bitmap position is derived from the runtime placement rectangle:

```text
row 0: x = left,     y = top - 30
row 1: x = left,     y = top - 30
row 2: x = left + 4, y = top - 23
```

That ordering has a visible retail asymmetry. State-11 placement commits occur
**after rows 0 and 2 have already been drawn but before row 1 is drawn**. A new
center-row firework can therefore appear during the same frame as its commit,
while a newly committed top/bottom-row firework first appears on the next
frame.

The exact surface/global map is in
`ghidra/firework_editor_surfaces.csv`, and the typed frame model is
`run_retail_editor_frame` in the C++26 reconstruction.

## Show playback

`0x00413450 UpdateFireworksShowPlayback` advances through the six authored columns. For each column it can activate up to three events, one for each row.

Show setup clears the type field of exactly **40 active-event records** at `0x005093F0..0x0050966F`, each with a 16-byte stride. A launch takes the first record whose type is `-1`, writes the type, zeros the two middle dwords, and stores the authored row. Row determines display position and Bink-bank addressing:

| Row | Position | Retail movie index rule |
|---:|---|---|
| 0 | `(0,0)` | `type` |
| 1 | `(440,0)` | `type + 12` |
| 2 | `(220,200)` | `type` |

The middle-row arithmetic is unconditional. Therefore retail types 8, 9, and 10 in row 1 map to bank indices 20, 21, and 22 (the special top-middle/crowd clips), while type 11 computes index 23, one past the verified 23-entry bank. The source reconstruction intentionally records this quirk rather than normalizing it.

The show also drives the crowd-loop/crowd-end clips and timed crowd/voice sound effects. When the show reaches its terminal phase, it switches the internal state to 16.

## Play -> transition movie -> authored show -> certificate

The exact runtime order is now resolved. Action **27 / Play** is the only editor
control that enters state 14. The Play-button path starts managed sound **880**
and writes internal state **14**.

State 14 calls `AnyManagedSoundPlaying`. If it returns 0, retail simply redraws
the editor and remains in state 14. Once it returns 1, retail clears the
display, disables input, opens `Data\\movies\\fireworkcomplete.bik`, and
enters state **15**. Despite the filename, this movie is therefore on the
**pre-show transition path**, not after the authored fireworks.

State 15 runs the shared Bink player. When `fireworkcomplete.bik` finishes,
retail:

1. re-enables input;
2. writes internal state **8**;
3. stops all managed sounds;
4. plays sound **142** at priority **90**, playback flag **1**;
5. marks that managed-sound slot persistent.

State 8 then reloads the 23-entry Fireworks Bink bank, resets crowd phase to
**0**, clears the 40 active-event slots, resets the exact show clock, and enters
state **9**. State 9 is the authored fireworks/crowd show.

The crowd presentation is exact:

- movie bank index **20** = `topmiddle.bik`; it is drawn continuously and
  rewound whenever it completes;
- phases **0, 1, 2** draw index **21** = `fireworkcrowdloop.bik`; each
  completed loop increments the phase and rewinds the movie;
- while that loop branch is active and no managed sound is playing, retail has
  a 1-in-10 chance per update to play one random ID from **323..347**, the
  retail **FD_*** Finale-dialogue group, at priority **50**, playback flag **2**;
- phase **3 and above** uses index **22** = `fireworkcrowdend.bik`;
- on that branch, if **349 = FD_WEN_02.wav** is not already playing, retail
  stops all managed sounds and starts it at priority **50**, playback flag **1**;
- when `fireworkcrowdend.bik` reaches its final frame, the phase is written
  directly to terminal value **7**;
- state **16 / Certificate** is entered only when the timeline column is at
  least **2** (8 seconds on the corrected 4-second clock) **and** crowd phase
  is **7**.

State 16 draws the certificate/results screen through
`0x004138A0 DrawFireworksCertificateScreen`, including the common print
function.

State 17's handler is now decoded exactly. If it were entered, it would:

1. call `0x0042CFD0 PreparePlayAgainTransition`;
2. set outer game-flow state `0x3C` (Play Again Yes/No);
3. write global `0x0051B418 = 0x24` (this address is **outside** the player-progress records);
4. call `SaveFireworksLayoutAndUnloadResources`;
5. clear global `0x0051C2FC`.

It does **not** update player progress. Exhaustive direct references to the
retail Fireworks state global still show no write of value 17, so this handler
is dormant/legacy retail code rather than the normal certificate transition.

The real certificate exit is the shared leave-current-activity modal
(`0x00429FE0 UpdateLeaveActivityConfirmation`). Accepting it raises
`0x0051C2E8`. `UpdateFireworksActivity` checks that flag **before** its
internal-state dispatch, saves/unloads Fireworks, clears the flag, and routes
the outer game flow to state **0x04 / Activity Select**.

The profile audit is also closed: `InitializeFireworksActivity` contains the
sole Fireworks progress write, setting the selected player's slot **63** to
`1` at `0x00411CE0`. The executable contains no reference to player-0 slot
64 at `0x0051B5D0`, and certificate drawing/printing, teardown, shared leave,
and dormant state 17 perform no player-progress write. There is therefore
**no separate retail Firework Finale completed flag**.

At this point the Fireworks editor data model, persistence, control actions, movie bank, authored show sequencing, certificate path, and completion/exit machinery are structurally recovered.


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

The exact editor placement channels and bottom-control hover/click/debounce
logic are now source-level too, including the row split, voice pools, two-frame
10->11 placement commit, full-grid feedback, shifted control hit point, and
dormant state-12 motion handler. Together with the show scheduler/timing,
40-record Bink event lifetime, pre-show movie handoff, crowd phases, sound
orchestration, certificate, and shared exit path, the remaining Fireworks work
is mostly lower-level DirectDraw composition and sprite-motion fidelity rather
than unresolved game-flow semantics.


## Exact retail show cadence

The scheduler inside `0x00413450 UpdateFireworksShowPlayback` uses the shared
`0x0041FC60 FileTimeDeltaCentiseconds` helper. That helper subtracts the low
DWORDs of two FILETIME values and divides the 100 ns delta by **100000**.
Its result is therefore in **10 ms units**.

This matters because the retail scheduler constants are not milliseconds.
It computes:

```text
elapsed_10ms = FileTimeDeltaCentiseconds(show_start, now)
column       = elapsed_10ms / 400
remainder    = elapsed_10ms % 400
```

So one authored timeline column lasts **400 x 10 ms = 4 seconds**.

`UpdateFireworksShowPlayback` also calls
`0x00413420 FileTimeDeltaMilliseconds`, which divides by 10000, but that
return value is immediately discarded. Treating that unused millisecond result
as the scheduler clock caused the earlier factor-of-ten reconstruction error.

Within each four-second column the three rows are staggered exactly as follows:

| Row | Retail threshold | Launch offset |
|---:|---:|---:|
| 0 | boundary | 0 ms |
| 1 | remainder >= 100 | 1000 ms |
| 2 | remainder >= 200 | 2000 ms |

The nominal launch time is therefore:

```text
launch_ms = column * 4000 + row_offset_ms
```

and the final authored column launches at approximately:

- row 0: **20000 ms**
- row 1: **21000 ms**
- row 2: **22000 ms**

Show setup initializes the previous elapsed value to **-401**. Crossing a
400-tick boundary fires row 0 and arms two persistent threshold gates for rows
1 and 2. If a frame skips past a threshold, retail fires that row on the first
observed frame after the threshold. If a frame skips directly into a new column
past the +1 s or +2 s thresholds, multiple row launches can therefore happen
on the same frame.

### Exact 40-record active-event pool

State 8 clears the type field of records spanning
`0x005093F0..0x0050966F`:

```text
40 records * 16 bytes = 0x280 bytes
```

Each retail record is:

| Offset | Meaning |
|---:|---|
| `+0x00` | firework type; `-1` means free |
| `+0x04` | zeroed auxiliary dword on allocation |
| `+0x08` | zeroed auxiliary dword on allocation |
| `+0x0C` | authored row index |

For every due authored cell, retail scans from record 0 upward and takes the
**first free record**. If all 40 are occupied, the launch is silently skipped.

During playback, the row selects the exact Bink index and screen position. When
the shared Bink draw/update helper reports completion, retail rewinds that Bink
and writes only `-1` to the record's type field. The other three dwords are
left stale until a later allocation overwrites them.

The exact threshold scheduler and active-event allocation/lifetime are now
represented in `reconstruction/include/btb/fireworks_sequence.hpp` and
`reconstruction/src/fireworks_sequence.cpp`.

### Certificate composition

`0x004138A0 DrawFireworksCertificateScreen` is now mapped down to its
profile-dependent layout and print interaction.

It:

- draws the certificate background;
- measures the selected player's glyph widths and horizontally centers the
  profile name around **X=491** at **Y=135**;
- draws the selected profile badge at **(471,156)** from a horizontal
  **50x45-pixel** badge strip;
- tests the print control using strict bounds:
  **296 < X < 348** and **418 < Y < 466**;
- on click, draws the depressed print surface and calls
  `ExportAndPrintGameImage`;
- on hover, draws the appropriate normal/hover print surface and plays
  sound **143 = CT_BOB_02.wav** once for the hover latch.

When managed audio is otherwise idle, the certificate can also start one
random voice from IDs **144..151**:

- 144 `CT_BOB_03.wav`
- 145 `CT_BOB_04.wav`
- 146 `CT_DIZ_01.wav`
- 147 `CT_DIZ_02.wav`
- 148 `CT_LOF_01.wav`
- 149 `CT_LOF_02.wav`
- 150 `CT_MUC_01.wav`
- 151 `CT_MUC_02.wav`

A separate retail latch means that random certificate line is started only
once during the certificate state.

The random-line selection itself has an exact retail quirk. When the cursor is
outside the Print button and no managed sound is active, retail calls
`rand()%8` **twice**:

1. the first roll becomes a candidate ID **144..151**, is compared against the
   previous candidate, and is stored if different;
2. a second independent roll becomes the ID **144..151** that is actually
   played.

The stored non-repeat candidate can therefore differ from the played line, and
the line that actually plays can repeat despite the apparent non-repeat code.

The certificate Print button is also not a completion/exit button. On click it
stops managed sounds, draws the pressed Print surface, and calls the shared
`0x00409730 ExportAndPrintGameImage` path. `DrawFireworksCertificateScreen`
contains **no write** to the Fireworks internal state or outer game-flow state.
Therefore state **17** remains dormant; leaving the certificate is handled by
the shared activity-level back/exit flags checked at the top of
`UpdateFireworksActivity`.

The exact layout and interaction model are in
`reconstruction/include/btb/fireworks_certificate.hpp`.
