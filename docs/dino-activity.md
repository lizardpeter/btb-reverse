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

`level_index = dinosaur_index + 3 * difficulty_index`

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
- coordinate pair `2N` — a runtime animation anchor copied into globals `0x004FC400/0x004FC404` (and float mirrors). It is `(80, 300)` in eight levels but **`(456, 248)` in Raptor Easy**.
- any later coordinate pairs before the sentinel — additional level-specific positions. The common trailing values `(80, 300)` and `(100, 120)` occur after the Raptor Easy special anchor; their exact consumers are still being traced.
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
| `+0x24` | `draw_loose_piece` | 1 in the tray / after final placement; 0 while the piece is represented by the cursor |
| `+0x28` | `piece_id` | copied from permutation table |
| `+0x2C` | `surface` | loaded DirectDraw piece surface |

The source rectangle and loose-piece visibility semantics are now directly supported by the draw path.

### Piece-state values

The field at piece-record offset `+0x10` is now recoverable as a small state enum:

| Value | Working name | Observed behavior |
|---:|---|---|
| 0 | Loose | piece is available at its start/tray position |
| 1 | Dragging | selected piece; tray copy hidden and piece surface installed as cursor |
| 2 | AcceptedDrop | cursor is cleared and the renderer uses the shared snap-transition anchor |
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

### Mode 1 — dragging / release decision

On release, the code compares the current pointer position against the target coordinate of the selected piece using `0x00409C60`, a square-distance/tolerance test.

Only the **selected piece's own target** is accepted.

If the release is close enough:

- mode advances to the snap/placement transition
- the selected record enters its success state
- active feedback audio is reset
- one of several randomized success voice/effect IDs is played

If the release is not valid:

- the piece enters a different return/failure state
- randomized negative/return feedback is played
- the record remains unplaced

### Mode 2 — finalize snap / return

The record's animation/state value determines the result.

For a successful snap:

- piece state changes to the final placed state
- the placed/enabled field is updated
- the completed-piece counter at `0x004FC440` increments
- the global interaction mode returns to idle

The render/update logic handles the intermediate movement state between the current position and target position.

## Completion

When the completed-piece counter reaches `N`, the activity waits for managed feedback sounds to finish before transitioning into its completion/replay flow.

The main activity flow eventually enters state `0x3C`, the already recovered **Play Again Yes/No** screen.

The code also updates the game's persistent completion/progress table for the current player, dinosaur, and difficulty.

## Rendering

### `0x00409990 DrawDinoActivity`

This routine draws:

1. the level background
2. shared animated/character UI elements
3. every piece according to its current runtime state and position

Placed/snapping/dragging states choose different position sources. The piece surface and source dimensions come directly from the runtime record.

The common sprite compositor invoked by this routine is shared with other activities and is being named separately from Dino-specific logic.

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

## Remaining Dino work

The activity is now structurally reconstructed. The remaining pass is narrower:

- assign exact enum names to the per-piece state values
- identify the runtime animation anchor and trailing extra positions' final semantic names
- name the shared sprite/animation helpers called from `DrawDinoActivity`
- map the numeric Dino feedback sound IDs back to filenames from `Data/sound/binklist.txt`
- write a clean source-level equivalent of the Dino loader and update loop


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

### Difficulty-specific groups

The initializer stores the difficulty index at `0x004FC43C`.

- activity-start group: `185 + difficulty`
  - 185 `DD_MRE_13.wav`
  - 186 `DD_MRE_14.wav`
  - 187 `DD_MRE_15.wav`
- completion group: `188 + difficulty`
  - 188 `DD_MRE_18.wav`
  - 189 `DD_MRE_17.wav`
  - 190 `DD_MRE_16.wav`

## Species index ordering

The executable's fixed path table establishes the actual index order:

- 0 = Raptor
- 1 = Triceratops
- 2 = T-Rex

Difficulty is 0 = Easy, 1 = Medium, 2 = Hard, so `species + 3*difficulty` spans the nine path-table records in their exact binary order.
