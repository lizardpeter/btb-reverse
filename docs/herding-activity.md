# Herding / Pets Corner activity

The Herding activity lives in the `Data/SubGame1` asset cluster and is entered through:

- `0x00419600 InitializeHerdingActivity`
- `0x004182B0 UpdateHerdingActivity`

The current pass is converting its data and runtime structures into source-level form.

## Source assets

Verified activity resources include:

- `herd.txt`
- `Pickles_1_8bit.bmp`
- `sheeptoolbar.bmp`
- `rabbittoolbar.bmp`
- `ducktoolbar.bmp`
- `gateright.bmp`
- `gateleft.bmp`
- `trailer1.bmp` / `trailer2.bmp`
- `traviscab.bmp`
- `uisurround.bmp`
- `sheepbag.bmp`
- `rabbitbag.bmp`
- `duckbag.bmp`
- `scrufty_sprite_8bit.bmp`
- `sheep_shadow.bmp`
- `bunny.bmp`
- `DUCK_01.bmp`
- `bk)1_revised_01.bmp`

## herd.txt loader

### `0x00415CB0 LoadHerdingCoordinateData`

The retail loader opens `Data\\SubGame1\\herd.txt` and repeatedly parses integer X/Y pairs into the shared flat coordinate array at `0x005101C8`.

Unlike several other loaders, it does **not** consume `-1 -1` as an end-of-file condition. Those pairs are stored in the array. It stops only when `fscanf` reaches EOF, then appends one extra in-memory `-1 -1` pair.

That behavior matters because the Herding initializer later uses the embedded sentinels to split the source data into logical groups.

## Retail file segmentation

The shipped file contains:

1. **6 fixed setup positions**
2. a sentinel-delimited group of **12** points
3. a group of **5** points
4. a group of **6** points
5. a final group of **1** point

The first six source pairs are:

```text
(719,266)
(527,114)
(376,170)
(478,471)
(93,324)
(100,120)
```

Their exact object names are being assigned from the initializer/runtime consumers rather than guessed from coordinates.

## First coordinate group transform

The initializer begins processing the grouped data at flat index 6. For every point in group 0 it writes:

```text
retail_x = source_x - 64
retail_y = source_y - 100
```

into the runtime table beginning at `0x004439F8`, and stores the resulting point count at `0x00443A98`.

For the shipped data, group 0 contains 12 points.

## Remaining groups

After each embedded `-1 -1`, the initializer advances to the next group.

- group 1: copied into a runtime coordinate table beginning around `0x00510628`; shipped count 5
- group 2: copied into another runtime coordinate table around `0x005104E8`; shipped count 6
- group 3: one final coordinate `(92,324)`, copied into dedicated globals near `0x0050AF70/0x0050AF74`

The runtime consumers are now being traced to determine whether these are animal routes, enclosure boundaries, spawn paths, or interaction regions. The reconstruction intentionally preserves them as ordered coordinate groups until that evidence is complete.

## Runtime entity table

`InitializeHerdingActivity` clears and constructs a large array of **0x64-byte activity entity records** beginning around `0x0050B3AC`.

The initializer creates at least 17 records and fills fields that include:

- integer position
- floating position mirrors
- state/type IDs
- sprite surface pointer
- sprite dimensions / animation dimensions
- additional state/animation fields

The exact record layout is being recovered from `UpdateHerdingActivity` before committing a final C++ structure.

## Source reconstruction

Current buildable source:

- `reconstruction/include/btb/herding_data.hpp`
- `reconstruction/src/herding_data.cpp`
- `reconstruction/tests/herding_data_test.cpp`

It preserves the exact retail sentinel segmentation and the verified group-0 coordinate transform.


## Retail gameplay loop

The original activity instructions and the executable now line up end-to-end.

Farmer Pickles:

1. moves through the large scrolling Pets Corner map;
2. approaches Travis' trailer and selects one of three food bags;
3. approaches an animal matching that food;
4. the animal joins Pickles' follower list;
5. Pickles leads it toward its species' correct home;
6. the animal leaves the follower formation and enters an autonomous home-entry sequence;
7. after the second-stage home path completes, its behavior becomes 99 (delivered).

Correct homes are:

- sheep -> **pen**
- rabbits -> **rabbit hutches**
- ducks -> **pond**

Scruffty is the explicit disruptor: if he collides with an animal that is following Pickles, retail removes that animal from the follower list and sends it wandering again.

## Exact 0x64-byte entity record

The runtime array begins at `0x0050B3A8`, with a stride of **0x64 bytes**.

| Offset | Field |
|---:|---|
| `+0x00` | entity ID/index |
| `+0x04` | animation frame |
| `+0x08` | direction/orientation |
| `+0x0C` | integer X |
| `+0x10` | integer Y |
| `+0x14` | previous X |
| `+0x18` | previous Y |
| `+0x1C` | floating X |
| `+0x20` | floating Y |
| `+0x24` | unknown |
| `+0x28` | unknown |
| `+0x2C` | animation/timing counter |
| `+0x30` | entity type |
| `+0x34..+0x40` | source rectangle |
| `+0x44` | 32-bit DirectDraw surface pointer |
| `+0x48` | movement speed |
| `+0x4C` | movement scalar |
| `+0x50` | behavior state |
| `+0x54` | unknown |
| `+0x58` | temporary-target flag/timer |
| `+0x5C` | temporary target X |
| `+0x60` | temporary target Y |

The source reconstruction intentionally leaves unresolved fields as `unknown_*` instead of inventing semantics.

## Entity types

Verified type IDs:

| Type | Entity |
|---:|---|
| 0 | Farmer Pickles |
| 1 | sheep |
| 2 | rabbit |
| 3 | duck |
| 7 | Scruffty |
| 12 | trailer 1 |
| 13 | Travis cab |
| 14 | trailer 2 |
| 15 | left gate |
| 16 | right gate |
| 17 | inactive/free entity slot |

Types 4-6 are referenced by retail collision/state logic but are deliberately not named yet.

The entity map is also in `ghidra/herding_entity_types.csv`.

## Difficulty population

For each of the three animal species, the initializer creates:

```text
animal_count = difficulty_index + 3
```

Therefore:

- Easy: 3 sheep + 3 rabbits + 3 ducks
- Medium: 4 + 4 + 4
- Hard: 5 + 5 + 5

Scruffty is only created/enabled when `difficulty_index > 0`, so he is absent on Easy and present on Medium/Hard.

## Food selection

Global `0x00510718` is the selected food index. It starts at `-1`.

The food indices and bag assets are:

| Value | Food | Attracted entity |
|---:|---|---|
| 0 | duck food / `duckbag.bmp` | duck |
| 1 | rabbit food / `rabbitbag.bmp` | rabbit |
| 2 | sheep food / `sheepbag.bmp` | sheep |

### Picking food at Travis' trailer

The player update checks three food hotspots. When Pickles gets close enough to a different bag, retail chooses between a Pickles voice line and a Travis voice line:

| Food | Pickles line | Travis line |
|---|---:|---:|
| duck | 582 `PC_PIC_02.wav` | 609 `PC_TR_02.wav` |
| rabbit | 583 `PC_PIC_03.wav` | 610 `PC_TR_03.wav` |
| sheep | 584 `PC_PIC_04.wav` | 611 `PC_TR_04.wav` |

Changing food releases every animal currently following Pickles. For every released follower retail sets:

```text
temporary_target_timer = 200
target_x = 286 + rand()%400
target_y = 450 + rand()%400
```

and removes its index from the follower list before activating the newly selected food.

## Following Pickles

The follower list is the 20-entry int32 array at `0x0050AF14`; `-1` marks an empty slot.

In free behavior state 0, an animal checks whether Pickles' currently selected food matches its required food:

```text
sheep  -> selected food 2
rabbit -> selected food 1
duck   -> selected food 0
```

If it matches and the animal enters attraction range, the entity index is inserted into the first free follower slot.

The join-follow feedback is selected from:

| Species/food | Retail sound IDs |
|---|---|
| duck | 585 / 586 (`PC_PIC_05`, `PC_PIC_07`) |
| rabbit | 587 / 588 (`PC_PIC_06`, `PC_PIC_08`) |
| sheep | 589 / 590 (`PC_PIC_09`, `PC_PIC_11`) |

While following, animals are given ordered target positions behind/around Farmer Pickles. Global `0x00510770` is reset each frame and used as the follower ordinal, so multiple animals form a spaced moving group rather than occupying the same point.

## Scruffty distraction

Scruffty is entity type 7.

The outer Herding update sends Scruffty around the five points copied from `herd.txt` group 1:

```text
(450,360)
(600,179)
(886,139)
(1028,304)
(755,360)
```

This identifies group 1 as the **Scruffty patrol path** with high confidence.

Inside `UpdateHerdingAnimal`, the retail code iterates type-7 entities and builds collision rectangles. On collision, if the current animal is present in `0x0050AF14`:

1. it sets the animal temporary-target timer to **200**;
2. assigns `target_x = 286 + rand()%400`;
3. keeps target Y at the animal's current vertical center;
4. removes the animal index from the follower list;
5. plays a randomized Pets Corner reaction line.

This directly implements the documented rule that Scruffty can distract an animal following Pickles and make it wander off.

## Home transition

Once a follower is close to its species staging/home target, the animal leaves normal follower steering and enters the nonzero behavior-state machine.

### First-stage home entrances

Behavior states 10-15 steer toward hard-coded species entrance coordinates:

| Species | Home | Retail steering target |
|---|---|---|
| sheep | pen | `(639,135)` |
| rabbit | rabbit hutches | `(815,91)` |
| duck | pond | `(1040,264)` |

The executable actually stores X as 679/855/1080 and subtracts 40 before steering.

### Behavior-state classes

| State | Meaning |
|---:|---|
| 0 | free roam / food attraction / follow / collision AI |
| 1 | begin home-route sequence |
| 2-9 | retail no-op |
| 10-15 | approach home entrance |
| 16-19 | retail no-op |
| 20-25 | enter/traverse home |
| 26-98 | retail no-op |
| 99 | delivered/home terminal animation |

State 1 allocates one of the 10-15 route states using a per-species counter. When the final animal of a species enters this phase, retail plays:

- sheep: 596 `PC_PIC_16.wav`
- rabbit: 597 `PC_PIC_17.wav`
- duck: 598 `PC_PIC_18.wav`

When the first-stage target is reached, the code adds 10 to the behavior state, entering 20-25.

States 20-25 use a second species/state waypoint table. Reaching the final target writes behavior state **99** and decrements the global undelivered-animal counter at `0x00510764`.

State 99 fixes the animal to its terminal/home-facing animation sequence.

The behavior map is machine-readable in `ghidra/herding_behavior_states.csv`.

## Rendering and world scrolling

### `0x00416310 CompareHerdingEntitiesByDepth`

The renderer builds an array of entity pointers and `qsort`s them. Gates (types 15/16) are explicitly special-cased; ordinary entities are ordered using their sprite-bottom Y coordinate.

### `0x00416370 DrawHerdingActivity`

This routine:

- scrolls the large `bk)1_revised_01.bmp` world around Farmer Pickles;
- computes camera X/Y globals;
- depth-sorts the active 0x64-byte entities;
- draws Pickles, animals, Scruffty, trailers/gates, and UI;
- uses the shared clipped DirectDraw blitter at `0x00415E50`.

## Navigation polygons

`herd.txt` group 0 is transformed by `(-64,-100)` and used by the world/player movement path.

Group 2 is copied without that transform and is passed to the shared `PointInPolygon` routine by animal AI. It is therefore a movement/collision constraint polygon, although its more specific in-game semantic name is not assigned yet.

Group 3 remains a single dedicated point `(92,324)`; its precise role is still being traced.


## Completion and outer flow

Global `0x00510764` is the count of animals that have not yet completed their home-entry route.

It is initialized by the species-creation loops to:

```text
3 * (difficulty + 3)
```

which gives:

- Easy: 9
- Medium: 12
- Hard: 15

Every animal that reaches the end of its states-20..25 home path changes to state 99 and decrements this count.

### Completion stage

Global `0x00510774` is the two-stage completion gate.

When the undelivered count reaches zero:

1. stage 0 waits until `AnyManagedSoundPlaying()` reports no active managed speech/effect;
2. retail randomly plays sound 599 or 600:
   - 599 = `PC_PIC_19.wav`
   - 600 = `PC_PIC_20.wav`
3. completion stage becomes 1;
4. stage 1 again waits for managed audio to finish;
5. retail records activity/progress completion;
6. outer game-flow state becomes **0x3C**, the shared Play Again Yes/No flow;
7. `0x00419390 UnloadHerdingActivityResources` releases the Pets Corner resources.

The completion gate is reproduced in source as `herding_completion_step`.

## Activity startup audio

A one-time path near the end of `UpdateHerdingActivity` starts shared activity music index **6**, which is the previously recovered `data\\music\\petscorner.wav` entry.

It also starts managed sound ID **581**, `PC_PIC_01.wav`, as the initial Pets Corner voice line and marks that sound persistent for the startup phase.
