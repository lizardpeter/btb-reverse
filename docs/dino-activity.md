# Dinosaur activity

The Dinosaur activity is one of the cleanest fully recoverable minigames in the executable. It is a drag-and-drop skeleton assembly game driven by nine small plaintext level files plus per-piece bitmaps.

## Level matrix

The activity has three dinosaur choices and three difficulty levels:

| Dinosaur | Easy | Medium | Hard |
|---|---:|---:|---:|
| Raptor | 7 pieces | 10 pieces | 14 pieces |
| T-Rex | 7 pieces | 10 pieces | 14 pieces |
| Triceratops | 7 pieces | 10 pieces | 13 pieces |

The selected level is encoded as:

`level_index = species_index * 3 + difficulty_index`

and stored in global `0x004FC438`.

## dino.txt format

All nine files have the same logical shape:

```text
N
<target_x_0> <target_y_0>
...
<target_x_N-1> <target_y_N-1>

<start_x_0> <start_y_0>
...
<start_x_N-1> <start_y_N-1>

80 300
100 120
-1 -1

<piece_id_0> <piece_id_1> ... <piece_id_N-1>
```

The exact tail length is not completely fixed. Eight levels contain two extra coordinate pairs; **Raptor Easy contains three**.

### Meaning

- `N` — piece count.
- first `N` coordinate pairs — assembled-skeleton **target positions**, indexed by piece ID.
- second `N` coordinate pairs — on-screen **starting/tray positions**, indexed by display slot.
- coordinate pair `2N` — a special render anchor copied into globals `0x004FC400/0x004FC404` (and float mirrors). It is `(80, 300)` in eight levels but **`(456, 248)` in Raptor Easy**. The normal drag/drop path does not use this anchor.
- any later coordinate pairs before the sentinel — **legacy/unused tail data in this executable**. Exhaustive Dino-module references show no reader after the first extra pair. Thus `(100,120)` is unused in all nine shipped levels; in Raptor Easy, the trailing `(80,300)` is also unused because `(456,248)` is the first extra pair and therefore the actual special-render anchor.
- `-1 -1` — coordinate-list terminator.
- final `N` integers — permutation assigning a real `piece<ID>.bmp` to each display/start slot.

The loader deliberately stores all coordinate pairs in one flat array, reads until the sentinel, then reads exactly `N` permutation integers.

## Why the permutation matters

For display slot `i`:

```text
piece_id = permutation[i]
target   = target_positions[piece_id]
start    = start_positions[i]
surface  = piece<piece_id>.bmp
```

That is exactly what the initialization assembly does. The permutation randomizes where the actual bones appear around the screen while keeping each bone's correct skeleton target tied to its true piece ID.

Examples:

- Raptor Easy: `6 3 0 1 4 2 5`
- Raptor Medium: `6 9 3 0 7 1 4 2 5 8`
- Raptor Hard: `6 11 9 3 0 7 12 1 4 13 2 5 8 10`

T-Rex uses the same permutations for corresponding Easy/Medium/Hard levels. Triceratops Easy differs (`3 6 0 1 4 2 5`), and Triceratops Hard has only 13 pieces.

## Loader

### `0x0040A440 LoadDinoLevelData`

High-confidence behavior:

1. formats the selected level directory's `dino.txt` path
2. opens it with `OpenGameDataFileWithCDFallback`
3. reads `N` into global `0x0043EE8C`
4. reads coordinate pairs into the flat array beginning at `0x005101C8` until `-1 -1`
5. reads exactly `N` permutation entries into the table beginning at `0x00510358`
6. writes a sentinel into the in-memory coordinate table as well

## Initialization

### `0x0040A670 InitializeDinoActivity`

The initializer loads the selected background plus `N` piece surfaces. Each piece is registered with the shared DirectDraw surface-reload registry and receives the magenta transparency color key.

It then builds an array of runtime piece records beginning around `0x004FC448`.

## Runtime piece record

The stride is **0x30 bytes**.

| Offset | Working field | Evidence |
|---:|---|---|
| `+0x00` | `target_x` | copied from target array indexed by `piece_id` |
| `+0x04` | `target_y` | copied from target array indexed by `piece_id` |
| `+0x08` | `current_x` | initialized from display-slot/start coordinate |
| `+0x0C` | `current_y` | initialized from display-slot/start coordinate |
| `+0x10` | `state` | tested/changed by drag, snap, placed, and return paths |
| `+0x14` | `src_rect.left` | initialized to 0 |
| `+0x18` | `src_rect.top` | initialized to 0 |
| `+0x1C` | `src_rect.right` | surface width from `GetSurfaceDesc` |
| `+0x20` | `src_rect.bottom` | surface height from `GetSurfaceDesc` |
| `+0x24` | `render_mode` | initialized to 1; set to 0 while the selected piece is cursor-owned; value 2 selects a special anchored render path |
| `+0x28` | `piece_id` | copied from permutation table |
| `+0x2C` | `surface` | loaded DirectDraw piece surface |

The source rectangle and loose-piece visibility semantics are now directly supported by the draw path.

### Piece-state values

The field at piece-record offset `+0x10` is now recoverable as a small state enum:

| Value | Working name | Observed behavior |
|---:|---|---|
| 0 | Loose | piece is available at its start/tray position |
| 1 | Dragging | selected piece; tray copy hidden and piece surface installed as cursor |
| 2 | AcceptedDrop | correct drop accepted; cursor cleared; interaction mode 2 finalizes the piece on the next update |
| 3 | RejectedDrop | wrong placement feedback; piece remains carried and can be tried again |
| 4 | Placed | renderer draws the piece permanently at its target position |

The activity-global interaction mode at `0x004FC3F0` is separate:

- 0 = idle/select
- 1 = carrying/trying to drop a piece
- 2 = finalize an accepted drop

## Runtime state machine

### `0x00409E00 UpdateDinoActivity`

The activity uses a global interaction mode at `0x004FC3F0`.

### Mode 0 — idle / select a piece

When the mouse is activated, the routine scans piece records that are not already placed. It builds each piece's current rectangle from `current_x/current_y/width/height` and calls the Dino rectangle hit-test at `0x00409CA0`.

When a piece is selected:

- the selected piece index is stored at `0x0043EE90`
- its runtime state changes from idle
- the global mode changes to dragging
- `SetCursorSurface` is called with the piece surface so the dragged bone becomes the cursor graphic
- drag-position globals are initialized
- the routine records cursor/piece offsets used during movement

### Mode 1 — carrying / release decision

On release, the code compares the current pointer position against the target coordinate of the selected piece using `0x00409C60`, a square-distance/tolerance test.

Only the **selected piece's own target** is accepted.

If the release is close enough:

- mode advances to the snap/placement transition
- the selected record enters its success state
- active feedback audio is reset
- one of several randomized success voice/effect IDs is played

If the release is not valid:

- the piece enters state 3 (`RejectedDrop`)
- randomized negative/return feedback is played
- **interaction mode stays at 1**, so the player is still carrying the selected bone and may immediately try another drop
- the record remains unplaced

### Mode 2 — finalize accepted drop

Only a correct drop changes the interaction mode to 2. On the next pass the selected piece transitions from state 2 to state 4 (`Placed`), the completed-piece counter at `0x004FC440` increments, the cursor/piece-selection globals are reset, and interaction mode returns to idle.

There is a defensive state-reset branch in the mode-2 code, but rejected drops do not normally enter mode 2.

## Completion

When the completed-piece counter reaches `N`, the completion gate is exact.

- if managed feedback is still playing and completion phase is not 1, retail
  keeps drawing the activity and waits;
- phase **0** begins the certificate/completion presentation;
- phase **1** keeps the certificate interactive even while its completion voice
  is still playing;
- a retained phase **10** branch performs the Play Again transition directly,
  although no normal Dino writer to phase 10 has been identified.

The phase-0 branch:

1. derives **species = encoded_level_index / 3**;
2. stops all managed sounds;
3. plays **completion sound 185 + species** at priority 50, arbitration class 1;
4. writes the selected species progress slot to 1 if it was still zero;
5. writes shared completion code 1;
6. loads/registers the species completion artwork:
   - Raptor -> `data\\subgamedino\\vel.bmp`
   - Triceratops -> `data\\subgamedino\\tri.bmp`
   - Tyrannosaurus -> `data\\subgamedino\\trex.bmp`
7. advances completion phase to 1;
8. runs the certificate print-button controller.

The persistent progress slots are exactly:

- Raptor -> slot 56
- Triceratops -> slot 57
- Tyrannosaurus -> slot 58

Difficulty does not choose a separate progress slot.

The retained phase-10 branch calls the shared Play Again transition, stores
front-end state `0x10`, enters outer state **0x3C**, sets next UI context
`0x14`, unloads Dino resources, and preserves the species progress write.

The central game-flow dispatcher also owns the live user-exit handoff from the
certificate to the same Play Again state.

## Rendering

### `0x00409990 DrawDinoActivity`

This routine draws:

1. the level background
2. shared animated/character UI elements
3. every piece according to its current runtime state and position

Rendering is now source-level end-to-end.

The exact order is:

1. opaque level background;
2. advance both Dino character animation channels;
3. Bob;
4. Ellis;
5. first piece pass: every state-4 (`Placed`) piece at permanent
   `target_x/target_y`;
6. second piece pass:
   - `render_mode == 2`: special-anchor/offset rendering;
   - otherwise states 0-3 at `current_x/current_y` when
     `render_mode != 0`.

The character destination positions are species-specific:

| Species | Bob | Ellis |
|---|---|---|
| Raptor | (268,20) | (366,20) |
| Triceratops | (312,20) | (414,21) |
| Tyrannosaurus | (308,20) | (394,20) |

Bob uses a **113x97** horizontal sprite sheet; Ellis uses **93x105**.
Source left/right are `frame*width` and `(frame+1)*width`.

Selecting a piece sets `render_mode = 0`, because `SetCursorSurface` makes the cursor own the bone image while it is carried. A correct drop clears the cursor and sets state 2; on the next update the piece becomes state 4 and `render_mode` is restored to 1.

There is also a `render_mode == 2` branch which renders from the special anchor using fixed offset tables:

- X: `0, 10, 20, 10, 0, -10, -20, -10`
- Y: `-40, -10, 0, 10, 40, 10, 0, -10`

However, normal Dino initialization/update code only writes render modes 0 and 1. Its table-index global `0x004FC424` is initialized/reset to 4, and no normal runtime update of that index was found. This is therefore a dormant/legacy special rendering path, not the accepted-drop animation.

The shared compositor is `0x00415E50 BlitColorKeyedSurfaceClipped`.

## Dino-specific geometry helpers

### `0x00409C60 IsCursorNearPoint`

Tests whether the current cursor position is within an integer tolerance of a supplied X/Y point:

```text
abs(cursor_x - x) <= tolerance
&&
abs(cursor_y - y) <= tolerance
```

This is the drop-target test.

### `0x00409CA0 IsCursorInsideRect`

Tests the current cursor position against an inclusive rectangle. It is used when selecting a loose piece.

## Resource teardown

### `0x0040A530 UnloadDinoActivityResources`

Unregisters/releases the Dino background, shared Dino surfaces, every loaded piece surface, associated animation resources, and then stops shared activity music.

## Current reconstruction status

The buildable C++ reconstruction under `reconstruction/` now covers:

- exact species-major level indexing (`species*3 + difficulty`)
- `dino.txt` parsing and permutation validation
- the recovered 0x30-byte piece model
- loose-piece selection/hit testing
- cursor-owned dragging
- correct and incorrect drop behavior
- accepted-drop finalization
- completed-piece tracking
- the retail snap-tolerance rule

Those tests are passing in GitHub Actions.

Dino is now effectively source-level for its game-owned logic and presentation:
level parsing, piece runtime, shared Bob/Ellis animations, exact frame composition,
species progress writes, startup/completion sound mapping, completion/certificate
flow, print-button behavior, and resource/outer-state integration are all
represented in the C++26 reconstruction.

The variable coordinate tail is no longer an unresolved item: only its first
pair is consumed by Dino, and all later pairs are legacy/unused data in this
build.

## Shared print button

`0x00409CD0 UpdateDinoPrintButton` is now reconstructed exactly.

It always draws the current species completion artwork first. The print hit box
uses strict interior bounds:

```text
x: 298 < x < 347
y: 421 < y < 465
```

Hover/pressed overlays are drawn at **(292,417)**:

- `Data\\SubGameFirework\\certprint.bmp` for normal hover;
- `Data\\SubGameFirework\\certprintdep.bmp` for pressed visual state.

The first hover plays managed sound **143** at priority 50, arbitration class 2,
with a one-shot hover latch that resets when the pointer leaves the rectangle.

A click stops managed sounds, draws `data\\ui\\printbar.bmp`, then calls
the common `0x00409730 PrintCurrentGameFrame` path. That same print function
is used by Park Designer and Fireworks; see `docs/printing-system.md`.


### Coordinate-tail validation

All nine original files were checked against the parser model:

| Level | N | Coordinate pairs before sentinel | Extra pairs after 2N |
|---|---:|---:|---|
| Raptor Easy | 7 | 17 | `(456,248), (80,300), (100,120)` |
| Raptor Medium | 10 | 22 | `(80,300), (100,120)` |
| Raptor Hard | 14 | 30 | `(80,300), (100,120)` |
| T-Rex Easy | 7 | 16 | `(80,300), (100,120)` |
| T-Rex Medium | 10 | 22 | `(80,300), (100,120)` |
| T-Rex Hard | 14 | 30 | `(80,300), (100,120)` |
| Triceratops Easy | 7 | 16 | `(80,300), (100,120)` |
| Triceratops Medium | 10 | 22 | `(80,300), (100,120)` |
| Triceratops Hard | 13 | 28 | `(80,300), (100,120)` |

This is why the parser intentionally treats everything after the two N-sized coordinate blocks and before `-1 -1` as a variable-length extra-position tail.

The executable-side consumption is now exact:

- pair index `2N` is copied by `InitializeDinoActivity` into the special-render anchor globals `0x004FC400/0x004FC404` and their mirrors;
- no later extra-pair address is referenced by Dino gameplay/render code;
- the dormant `render_mode == 2` renderer applies the fixed 8-entry offset table around that first extra pair, with retail table index initialized to **4**.

The source model therefore exposes `special_render_anchor()` separately from
`legacy_unused_tail()`, while still preserving every parsed pair losslessly.


## Dino feedback sound groups

The managed sound catalog resolves the hard-coded numeric groups used by the update loop.

### Correct placement: IDs 164-169

- 164 `DD_MRE_04.wav`
- 165 `DD_MRE_05.wav`
- 166 `DD_MRE_06.wav`
- 167 `DD_BOB_01.wav`
- 168 `DD_BOB_01.wav` (duplicate catalog entry)
- 169 `DD_BOB_03.wav`

### Incorrect placement: IDs 170-175

- 170 `DD_MRE_07.wav`
- 171 `DD_MRE_08.wav`
- 172 `DD_MRE_09.wav`
- 173 `DD_BOB_04.wav`
- 174 `DD_BOB_05.wav`
- 175 `DD_BOB_06.wav`

### Species-specific intro/completion groups

A direct initializer trace corrects an older label: global `0x004FC43C`
stores the **species index**, not difficulty.

The initializer computes:

```text
encoded_level = species * 3 + difficulty
```

and stores species separately at `0x004FC43C`.

The actual sound use is:

- **completion group: `185 + species`**
  - Raptor: 185 `DD_MRE_13.wav`
  - Triceratops: 186 `DD_MRE_14.wav`
  - Tyrannosaurus: 187 `DD_MRE_15.wav`
- **activity-start group: `188 + species`**
  - Raptor: 188 `DD_MRE_18.wav`
  - Triceratops: 189 `DD_MRE_17.wav`
  - Tyrannosaurus: 190 `DD_MRE_16.wav`

Startup uses priority **90**, arbitration class **1**, and then marks the
resolved managed slot input-interruptible. Completion uses priority **50**,
arbitration class **1**.

## Species index ordering

The executable's fixed path table establishes the actual index order:

- 0 = Raptor
- 1 = Triceratops
- 2 = T-Rex

Difficulty is 0 = Easy, 1 = Medium, 2 = Hard, so `species*3 + difficulty` spans the nine path-table records in their exact binary order. The completion write divides this level index by 3, yielding the species index and therefore the three persistent Dino progress slots.


## Dino sound groups

The numeric IDs used by the activity can now be mapped back to the WAV catalog in `Data/sound/binklist.txt`.

| Purpose | IDs | WAV entries |
|---|---:|---|
| Correct bone drop | 164-169 | `DD_MRE_04.wav`, `DD_MRE_05.wav`, `DD_MRE_06.wav`, `DD_BOB_01.wav` (two IDs), `DD_BOB_03.wav` |
| Incorrect bone drop | 170-175 | `DD_MRE_07.wav`, `DD_MRE_08.wav`, `DD_MRE_09.wav`, `DD_BOB_04.wav`, `DD_BOB_05.wav`, `DD_BOB_06.wav` |
| Species completion group | 185-187 | `DD_MRE_13.wav`, `DD_MRE_14.wav`, `DD_MRE_15.wav` |
| Species activity-start group | 188-190 | `DD_MRE_18.wav`, `DD_MRE_17.wav`, `DD_MRE_16.wav` |

The correct/incorrect drop groups are chosen by `rand() % 6`. The initializer's snap tolerance is also exact: encoded level index 2 uses tolerance 9; every other Dino level uses tolerance 20.


## C++26 presentation module

The newly closed presentation/completion layer is in:

- `reconstruction/include/btb/dino_presentation.hpp`
- `reconstruction/src/dino_presentation.cpp`
- `reconstruction/tests/dino_presentation_test.cpp`

It intentionally preserves the dormant `render_mode == 2` and phase-10 paths
as retail behavior instead of deleting them from the faithful reconstruction.
