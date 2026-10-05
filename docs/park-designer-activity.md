# Park Designer / DYP activity

Park Designer is the `Data/SubGameDYP` module entered through:

- `0x0040B7E0 InitializeParkDesignerActivity`
- `0x00410D10 UpdateParkDesignerActivity`

It is a data-driven object-placement editor with summer/winter asset banks, three editor modes, deletion/printing controls, per-object collision polygons, and a player-specific binary save.

## Source assets

The activity contains large summer and winter banks for:

- grass/background pieces
- flowers
- benches
- trees
- bushes
- bases / pillars / roofs
- pond shapes
- fountains
- reeds / rocks / lilies
- decoration objects
- bandstand objects
- pond objects

UI assets include:

- Summer / Winter toggles
- Delete / Delete All
- Print
- View
- Decorate / Band / Pond mode controls
- three mode trays
- up/down controls
- delete cursor

The `SNOW/` directory supplies the winter versions of the world/object art.

## Bound-area loader

### `0x0040AEA0 LoadParkDesignerData`

The routine first opens:

`data\\subgamedyp\\boundareas.txt`

using the shared CD-fallback file loader.

Retail has a fixed in-memory polygon corpus:

- **3 modes**
- **4 categories per mode**
- **5 variants per category**
- **60 polygons total**
- **30 coordinate slots per polygon**

Each polygon is a stream of integer X/Y pairs terminated by `-1 -1`.

The backing count table begins at `0x00508B04`. Counts include the sentinel because retail stores the sentinel pair before testing X for `-1`. Collision code therefore passes `count - 1` to `PointInPolygon`.

The coordinate backing store begins at `0x005041B8`.

### Exact indexing formula

The collision lookup around `0x0040D3B7` proves the dimensions:

```text
mode = record_index < 100 ? 0
     : record_index < 300 ? 1
                          : 2

polygon_index =
    mode * 20
  + bound_category * 5
  + bound_variant
```

The source file has 12 visual paragraph groups of five lines each, exactly matching:

`3 modes × 4 categories × 5 variants`.

This is reproduced in `parse_bound_areas`.

## Persistent object table

The designer owns exactly **400 object records**, beginning at `0x004FCAB0`.

Each record is **0x4C bytes**. The reconstructed byte-exact structure is `RetailObjectRecord32`.

High-confidence fields:

| Offset | Meaning |
|---:|---|
| `+0x00` | left/world X |
| `+0x04` | top/world Y |
| `+0x08` | right bound |
| `+0x0C` | bottom bound |
| `+0x10` | 32-bit DirectDraw surface pointer |
| `+0x1C` | object code/type |
| `+0x20` | object subcode |
| `+0x24` | enabled/active flag |
| `+0x28` | sprite width |
| `+0x2C` | sprite height |
| `+0x30` | sort/depth layer |
| `+0x34` | record index |
| `+0x38` | bound-area category 0..3 |
| `+0x3C` | bound-area variant 0..4 |

Fields whose exact semantic role is not yet closed remain named `unknown_*` in source.

### Record-index families

The polygon lookup partitions the 400 records into three families:

- records 0..99 -> bound mode 0
- records 100..299 -> bound mode 1
- records 300..399 -> bound mode 2

The editor also contains explicit compaction/copy logic for the 0..99 and 300..399 ranges, confirming that these ranges are intentional storage classes rather than incidental object counts.

## Geometry refresh

### `0x0040DBE0 RefreshParkDesignerObjectGeometry`

Given an object record index, this helper:

1. resolves the record's object code to a DirectDraw surface bank;
2. queries the surface description;
3. derives sprite width/height;
4. updates `right = left + width`;
5. updates `bottom = top + height`;
6. sets the active flag;
7. stores width/height and the record's own index;
8. records an object-code-dependent runtime value.

This is used immediately after placement and when reconstructing loaded objects.

## Save format

The player-specific save filenames are:

- `dypdata1.txt`
- `dypdata2.txt`
- `dypdata3.txt`
- `dypdata4.txt`
- `dypdata5.txt`

Despite the extension, they are **binary**.

### `0x0040AC90 SaveParkDesignerData`

The routine opens the selected file in binary write mode and writes:

1. all **400 × 0x4C-byte object records**
2. exactly **28 additional int32 state values**

### Load half of `0x0040AEA0`

After loading `boundareas.txt`, the same routine opens the selected `dypdataN.txt` with `rb` and reads the exact same sequence.

Therefore a complete retail save is:

```text
400 * 76 + 28 * 4 = 30,512 bytes
```

The source reconstruction provides lossless `read_save` / `write_save` routines and intentionally preserves every unresolved field.

## Collision / placement validation

The activity uses the shared `PointInPolygon` routine at `0x00428520`.

The object-collision path uses:

- object record index -> one of the 3 bound modes
- record `+0x38` -> category
- record `+0x3C` -> variant
- the corresponding polygon from `boundareas.txt`

The mouse/object-local point is tested against the polygon after subtracting the object's top-left world position.

## Source reconstruction

Current buildable source:

- `reconstruction/include/btb/park_designer_data.hpp`
- `reconstruction/src/park_designer_data.cpp`
- `reconstruction/tests/park_designer_data_test.cpp`

It currently covers:

- exact 3×4×5 bound-area layout
- sentinel/count behavior
- exact 0x4C object-record layout
- 400-record storage
- exact 28-value save tail
- binary save read/write
- record-index -> collision polygon mapping

Next work is assigning the three record families and four polygon categories their final editor semantics, then reconstructing place/delete/mode/summer-winter behavior.


## Seasons

Global `0x00509344` is the persisted season/theme selection.

| Value | Season |
|---:|---|
| 0 | Summer |
| 1 | Winter |

### `0x0040D1D0 ApplyParkDesignerSeason`

This function selects one of the paired normal/SNOW DirectDraw surface banks and copies the selected surfaces into the active Park Designer tables.

It covers existing placed-object rendering as well as the surface banks used by later placements. The normal branch uses the ordinary SubGameDYP assets; the nonzero branch uses the corresponding `SNOW/` assets.

The loaded save value at `0x00509344` is applied during activity initialization, so the player's chosen season persists in `dypdataN.txt`.

## Editor modes

Global `0x00507B5C` is the editor mode.

The UI dispatch checks control indices 7 through 10 and stores:

```text
mode = control_index - 7
```

The asset load order and mode-specific placement/audio paths resolve those values:

| Control index | Mode value | Mode |
|---:|---:|---|
| 7 | 0 | Pond |
| 8 | 1 | Bandstand |
| 9 | 2 | Decorate |
| 10 | 3 | View |

Index 11 is the context-sensitive Delete control.

These values are represented by `EditorMode` / `EditorControl` in the reconstruction.

### Delete control

When mode is **View (3)**, the Delete control loads:

`data\\ui\\Deleteallobjects.bmp`

and enters the shared Yes/No confirmation overlay.

A confirmed result in `UpdateParkDesignerActivity`:

1. sets the `object_code` field of all 400 records to `-1`;
2. resets the Pond placement counter to 0;
3. resets the Bandstand placement counter to 200;
4. resets the Decorate placement counter to 300;
5. clears current selection/editor state.

In Pond/Bandstand/Decorate modes, the same control instead activates the normal delete tool/cursor for removing an individual placed object.

## View toolbar

`UpdateParkDesignerEditorInteraction` has a four-entry View toolbar. The recovered actions are:

- item 0 -> Summer
- item 1 -> Winter
- item 2 -> Print
- item 3 -> View-side auxiliary control/voice path

Items 0 and 1 write the new season to `0x00509344`, play DYP View-family feedback, then call `ApplyParkDesignerSeason`.

Item 2 calls the shared printing function at `0x00409730`. The original retail support documentation confirms that using an in-game print control first writes `Printme.bmp`, even if the subsequent printer dialog is cancelled.

The item-3 hover path is confirmed; its exact click semantics are being kept separate until the downstream branch is fully closed.


## Draw ordering

The field at object-record offset `+0x30` is now identified as the **sort/depth layer**.

`RefreshParkDesignerObjectGeometry` fills it from the constant table at `0x0043FAC4`, indexed by object code.

Observed table families include:

- object code 0 -> layer 0
- early codes 1..7 -> layer 199
- the next broad group beginning at code 8 -> layer 99
- later object families predominantly -> layer 0

The exact code ranges are retained from the binary rather than normalized into a new rendering model.

### Comparators

`0x0040D780 CompareParkDesignerObjectsBySortLayer`

performs the simpler active-object sort on `+0x30`.

`0x0040D5B0 CompareParkDesignerObjectsForDraw`

first compares sort layer; ties are then resolved with bottom/Y positioning and several special cases for object code 300, Pond records, category/variant values, and the currently selected designer object.

`DrawParkDesignerActivity` builds an array of pointers to the 400 persistent records and invokes these retail comparators before blitting the objects.


## Individual deletion

The delete-tool branch is now separated from placement/view behavior.

### `0x0040D2E0 FindTopmostParkDesignerObjectAtCursor`

Retail scans the **depth-sorted pointer array from back to front**, so the visually topmost eligible object wins.

For every candidate it:

1. rejects inactive records;
2. applies the caller's filter mode;
3. checks the mouse against the object's coarse left/top/right/bottom box;
4. converts the cursor into object-local coordinates;
5. chooses the exact `boundareas.txt` polygon from record index/category/variant;
6. calls the shared `PointInPolygon`;
7. returns the persistent record index when the polygon test succeeds.

This is why selection matches irregular object silhouettes rather than only sprite rectangles.

### Compound Pond cleanup

`0x0040DC90 ClearPondPrimaryObjectsBySelector` operates only on records 0..99:

| Selector | Retail effect |
|---:|---|
| 0 | clear every primary Pond record |
| 1 | clear records whose object code is exactly 7 |
| 2 | clear records with object code below 100 except code 7 |

The helper also clears the corresponding persisted Pond-selection globals for selectors 0 and 1.

### Compound Bandstand cleanup

`0x0040DCE0 ClearBandstandObjectsByCategorySelector` operates on records 200..299:

| Selector | Retail effect |
|---:|---|
| 0 | clear bound categories 0, 1, and 2 |
| 1 | clear category 2 only |

The individual delete branch invokes these helpers for specific compound-object cases before clearing the selected primary record.

### Delete branch

When the delete cursor/tool is active, retail calls `FindTopmostParkDesignerObjectAtCursor(1)`.

The selected record's storage range and metadata decide whether the operation is:

- a simple single-record delete;
- a Pond compound cleanup;
- a Bandstand compound cleanup plus linked-record-list repair.

The persistent portions of those cleanup helpers are now implemented and tested in `park_designer_data.cpp`.

## Toolbar helper layer

### `0x0040D420 HitTestParkDesignerToolbar`

Walks the toolbar rectangle table until its `-1` sentinel and returns the control index under the current cursor.

### `0x0040D470 DrawParkDesignerToolbarAndHover`

Draws normal/hover/depressed control surfaces and plays the matching `DYP_G_*` hover lines for the Pond/Bandstand/Decorate/View/Delete control group.

### `0x00410F70 PlayPersistentParkDesignerVoice`

Centralizes the activity's spoken UI feedback. It plays through the 80-slot managed sound system with priority 50 and marks the resulting slot persistent so the voice is not auto-reaped during editor state transitions.


## Exit / completion sequence

Global `0x00509368` is the Park Designer closing-state value used when the player leaves the activity.

The outer `UpdateParkDesignerActivity` path is:

| Stage | Behavior |
|---:|---|
| 0 | choose and play one of sounds 246..248 |
| 1 | keep drawing the designer and wait until managed speech/effects are finished |
| 2 | save/unload designer state, update progress/completion, and return to the outer game flow |

The three closing lines are:

- 246 = `DYP_G_BOB_12.wav`
- 247 = `DYP_G_BOB_13.wav`
- 248 = `DYP_G_BOB_14.wav`

At stage 2 retail calls `UnloadParkDesignerActivityResources`, which saves the player-specific `dypdataN.txt` before freeing activity resources.

The code then scans all 400 persistent object records. If **any** object has `object_code != -1`, the current player's Park Designer progress/completion flag is set.

The source-level equivalent is now in:

- `park_designer_runtime.hpp`
- `park_designer_runtime.cpp`
- `park_designer_runtime_test.cpp`
