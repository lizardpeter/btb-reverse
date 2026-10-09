# Herding / Pets Corner activity

The Herding activity lives in the `Data/SubGame1` asset cluster and is entered through:

- `0x00419600 InitializeHerdingActivity`
- `0x004182B0 UpdateHerdingActivity`

The activity is now substantially source-level. The current pass closes the exact retail renderer/presentation layer on top of the already reconstructed AI, follower, food, home-route, completion, and entity-record behavior.

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

One fixed pair is exact: pair **#3 = (478,471)** is read directly by
`InitializeHerdingActivity` into the first entity's floating X/Y fields, so
it is **Farmer Pickles' retail start position**. Exhaustive Herding-module
references show **no retail consumer for the other five fixed setup pairs** in
this executable build; the reconstruction preserves them as unused source data
rather than assigning speculative meanings.

## First coordinate group transform

The initializer begins processing the grouped data at flat index 6. For every point in group 0 it writes:

```text
retail_x = source_x - 64
retail_y = source_y - 100
```

into the runtime table beginning at `0x004439F8`, and stores the resulting point count at `0x00443A98`.

For the shipped data, group 0 contains 12 points.

## Remaining coordinate groups

After each embedded `-1 -1`, the initializer advances to the next group.

- **group 1** -> `0x00510628`, 5 points: exact **Scruffty patrol path**
- **group 2** -> `0x005104E8`, 6 points: exact **animal exclusion polygon**
- **group 3** -> `0x0050AF70/0x0050AF74`, one point `(92,324)`: exact
  **animal free-roam navigation recovery/re-entry target**

Group 2 is the lower-world polygon:

```text
(30,500)
(500,500)
(500,500)
(1200,500)
(1200,850)
(30,850)
```

For a non-following herd animal, `UpdateHerdingAnimal` tests its current
integer position against this polygon. If the animal is inside, retail restores
its previous position and repeatedly steers it toward **(650,486)**, retesting
the polygon after each movement step until it is outside. This is therefore an
exclusion/escape region rather than a route.

Group 3 is used by the outer Herding free-roam update. After an animal's normal
movement step, retail tests the new position against group 0. If it crossed
outside that navigation polygon, the position is restored and the animal is
steered toward **(92,324)**. This makes group 3 the recovery/re-entry target.

These semantics are machine-readable in
`ghidra/herding_coordinate_groups.csv`.

## Runtime entity table

`InitializeHerdingActivity` clears and constructs a large array of **0x64-byte activity entity records** beginning around `0x0050B3AC`.

The initializer creates at least 17 records and fills fields that include:

- integer position
- floating position mirrors
- state/type IDs
- sprite surface pointer
- sprite dimensions / animation dimensions
- additional state/animation fields

The exact 0x64-byte record layout is now committed in `herding_runtime.hpp` with offset assertions for every live field.

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
| `+0x1C` | direction in degrees as float; initialized from integer direction × 45.0 |
| `+0x20` | floating X |
| `+0x24` | floating Y |
| `+0x28` | resolved Pickles movement-direction mask; bits 1/2/4/8 = Up/Right/Down/Left |
| `+0x2C` | animation-frame countdown; decremented during animation and reset to retail delays such as 50/100/150 |
| `+0x30` | entity type |
| `+0x34..+0x40` | source rectangle |
| `+0x44` | 32-bit DirectDraw surface pointer |
| `+0x48` | movement-active flag; retail writes 0/1 |
| `+0x4C` | floating movement speed |
| `+0x50` | behavior state |
| `+0x54` | unused in this executable; no retail references |
| `+0x58` | temporary-target countdown/timer |
| `+0x5C` | temporary target X |
| `+0x60` | temporary target Y |

The remaining record semantics are now substantially closed. In particular,
`+0x28` is not raw DirectInput state: `UpdateHerdingActivity` resolves input
into this direction mask and later maps that mask to the eight facing indices:

```text
0 Up        1 UpRight   2 Right     3 DownRight
4 Down      5 DownLeft  6 Left      7 UpLeft
```

`+0x2C` is the per-entity animation-frame countdown. Pickles' update subtracts
10 or a movement-derived amount from it and resets it when animation frames
advance; gate/home-route code also seeds it to values such as 50. Offset
`+0x54` remains intentionally named unused because there are no retail
references to that dword anywhere in this executable.

The previously unknown entity type IDs 4..6 are also closed as
dormant/no-constructor retail IDs.

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

Types **4, 5, and 6 are dormant legacy IDs**. Exhaustive writes to the entity
type field inside `InitializeHerdingActivity` construct only 0, 1, 2, 3, 7,
12, 13, 14, 15, 16, and the cleared/inactive value 17. No retail entity is
constructed with type 4, 5, or 6, even though generic branches can still
compare against those values.

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

State 1 is now exact. Each species has its own counter at
`0x0050AFD0 + species*4`, initialized to zero. Retail executes:

```text
assigned_state = 10 + species_counter
species_counter += 1
```

and writes `assigned_state` directly to the animal's behavior state. If the
incremented counter equals `difficulty_index + 3`, the animal is the final
member of that species and retail plays:

- sheep: 596 `PC_PIC_16.wav`
- rabbit: 597 `PC_PIC_17.wav`
- duck: 598 `PC_PIC_18.wav`

With shipped difficulty values 0/1/2, species counts are 3/4/5, so the normal
allocator reaches only states **10..12 / 10..13 / 10..14**. State 15 has a
valid handler but is not assigned by normal retail population counts.

When the first-stage target is reached, the code adds 10 to the behavior state,
entering the corresponding 20..24 state in normal retail play.

States 20-25 use a second species/state waypoint table. For normal retail
Easy/Medium/Hard allocation, only states **20..24** are reached. The exact
post-initializer steering points are:

| State | Sheep / pen | Rabbit / hutches | Duck / pond |
|---:|---|---|---|
| 20 | (503,142) | (738,106) | (933,255) |
| 21 | (560,128) | (840,77) | (1073,331) |
| 22 | (707,99) | (759,94) | (1011,337) |
| 23 | (618,118) | (783,98) | (964,278) |
| 24 | (656,110) | (807,107) | (1022,286) |

The initializer constructs these from 15 embedded raw points, subtracts the
per-species sprite anchor X values **40/48/50**, then applies extra rabbit
**(-35,+10)** and duck **(-27,+36)** adjustments. The runtime index arithmetic
simplifies to:

```text
waypoint_index = species_index * 5 + (behavior_state - 20)
```

Both the first-stage entrance and second-stage target use the exact arrival
threshold **distance < 10.0**.

On first-stage arrival, retail adds 10 to the behavior state. On second-stage
arrival, it writes behavior state **99** and decrements the global
undelivered-animal counter at `0x00510764`.

The final sheep entering state 1 also starts the **left gate (type 15)**
animation with timer 50; the final rabbit starts the **right gate (type 16)**
with timer 50. Ducks do not trigger a gate.

State 99 fixes the animal to its terminal/home-facing animation sequence.

The behavior map is machine-readable in `ghidra/herding_behavior_states.csv`.

## Rendering and world scrolling

### `0x00416310 CompareHerdingEntitiesByDepth`

The renderer builds an array of entity pointers and `qsort`s them. Gates (types 15/16) are explicitly special-cased; ordinary entities are ordered using their sprite-bottom Y coordinate.

### `0x00416370 DrawHerdingActivity`

The renderer is now source-level at the camera/sort/sprite-composition layer.

#### Exact camera

Retail draws a **600x380** source viewport from the large
`bk)1_revised_01.bmp` world at screen destination **(20,20)**.

Horizontal camera:

```text
PicklesX < 300   -> cameraX = 0
PicklesX > 972   -> cameraX = 672
otherwise        -> cameraX = PicklesX - 300
```

Vertical camera:

```text
PicklesY < 110   -> cameraY = 0
PicklesY > 612   -> cameraY = 502
otherwise        -> cameraY = PicklesY - 110
```

The shared clipped blitter at `0x00415E50` subtracts these camera globals
from entity world coordinates internally, so the renderer passes entity X/Y
directly rather than pre-transforming them.

#### Exact depth sort

`0x00416310 CompareHerdingEntitiesByDepth` sorts the pointer list before
drawing.

- left/right gates (types 15/16) always sort before ordinary entities;
- ordinary entities compare by:
  `world_y + (source_bottom - source_top)`;
- if the first ordinary entity is deeper, comparator returns +1;
- otherwise it returns **-1**, including equal depth. Retail never returns 0
  for equal ordinary bottoms.

#### Sprite geometry

| Entity | Retail cell/source geometry |
|---|---|
| Farmer Pickles | 128x128; vertical band = `direction/2`; odd directions use a frame bank 39 columns to the right |
| Sheep | 98x103, `direction*10 + frame` |
| Rabbit | 73x69, `direction*10 + frame` |
| Duck | 53x50, `direction*6 + frame` |
| Scruffty | 86x87, `direction*7 + frame` |
| Trailer 1 | fixed 151x240 |
| Travis cab | horizontal 230x240 frames |
| Trailer 2 | fixed 43x76 and forced world position (380,364) |
| Left gate | horizontal 120x70 frames, forced position (717,162) |
| Right gate | horizontal 120x70 frames, forced position (859,101) |

#### In-render animation timers

Several entity animation counters are advanced directly inside the renderer.

For free-roam animals (behavior state 0), the renderer subtracts the
truncate-toward-zero result of:

```text
movement_speed * 10
```

from the animation countdown. When the countdown expires it resets to **100**
and advances the frame:

- sheep cycle **6..9**;
- rabbits cycle **3..6**;
- ducks cycle **0..5**.

Nonzero behavior states do not use this free-roam renderer animation update.

The Travis cab uses a separate draw-count timer:

- decrement every draw;
- when it expires, reset to **10**;
- advance one frame;
- wrap **12 -> 0**.

Both gates use their own timer:

- countdown **-1** disables animation;
- otherwise decrement;
- on exactly zero, advance one frame;
- frames below 3 reload countdown **50**;
- reaching frame **3** leaves countdown at zero, so the gate remains at its
  terminal frame.

These policies are represented by the typed render-animation helpers in
`herding_presentation.hpp`.

#### Post-entity composition

After the depth-sorted entity pass, retail draws three conditional keyed world
overlays at authored positions:

- duck-food overlay: **(272,332)**
- rabbit-food overlay: **(365,354)**
- sheep-food overlay: **(269,394)**

It then draws the UI surround and, when a food type is selected, the selected
food toolbar at **(284,417)**.

The typed model is in
`reconstruction/include/btb/herding_presentation.hpp` with regression tests in
`reconstruction/tests/herding_presentation_test.cpp`. The recovered constants
are also preserved in `ghidra/herding_render_geometry.csv`.

#### Exact surface globals

The initializer-to-renderer surface bindings are now fully named:

| Global | Retail asset |
|---|---|
| `0x00510724` | `bk)1_revised_01.bmp` background |
| `0x00510728` | `Pickles_1_8bit.bmp` |
| `0x0051072C` | `DUCK_01.bmp` |
| `0x00510730` | `bunny.bmp` |
| `0x00510734` | `sheep_shadow.bmp` |
| `0x00510738` | `scrufty_sprite_8bit.bmp` |
| `0x0051073C` | `duckbag.bmp` |
| `0x00510740` | `rabbitbag.bmp` |
| `0x00510744` | `sheepbag.bmp` |
| `0x00510748` | `uisurround.bmp` |
| `0x0051074C` | `traviscab.bmp` |
| `0x00510750` | `trailer1.bmp` |
| `0x00510754` | `trailer2.bmp` |
| `0x00510758` | `gateleft.bmp` |
| `0x0051075C` | `gateright.bmp` |
| `0x005105A0` | `ducktoolbar.bmp` |
| `0x005105A4` | `rabbittoolbar.bmp` |
| `0x005105A8` | `sheeptoolbar.bmp` |

The three bag surfaces are copied into the renderer's compact array
`0x0050AF64..0x0050AF6C` in duck/rabbit/sheep order. The toolbar array uses
the same food-ID ordering. The map is machine-readable in
`ghidra/herding_surface_map.csv`.

## Navigation polygons

`herd.txt` group 0 is transformed by `(-64,-100)` and is the main
**navigation polygon**. The outer update uses it for Farmer Pickles movement
validation, and free-roaming animals also validate their next movement step
against it.

Group 2 is the separate **animal exclusion polygon** described above. Its exact
escape steering target is **(650,486)**.

Group 3 is the single **free-roam navigation recovery target (92,324)** used
after an animal's attempted movement leaves group 0.


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


## Renderer reconstruction status

The remaining Herding work is no longer basic camera/entity geometry. Camera
clamping, depth ordering, directional source-cell layouts, Pickles' unusual
39-column alternate facing bank, trailer/cab/gate geometry, gate world
positions, and the post-entity food/UI composition are all represented directly
in C++26.

Further fidelity work can now focus on the small per-entity animation-timer
updates and any remaining conditional overlay state, rather than an unknown
rendering architecture.

## Shared full-game Pets Corner source integration

The recovered Herding presentation and completion rules now have an
explicit C++26 adapter for the common 68-state `GameRoot`:

- `reconstruction/include/btb/full_game_herding_presentation.hpp`
- `reconstruction/src/full_game_herding_presentation.cpp`
- `reconstruction/include/btb/full_game_herding.hpp`
- `reconstruction/src/full_game_herding.cpp`
- `reconstruction/tests/full_game_herding_presentation_source_test.cpp`
- `reconstruction/tests/full_game_herding_source_test.cpp`

The compositor accepts the original 0x64-byte entity records after an
AI update. It computes the retail 600×380 world camera, draws the exact
source BMP crop at (20,20), sorts gates ahead of ordinary entities, then
draws their actual sprite sheets with direction/frame source rectangles.
It applies the recovered per-render animation updates for ordinary
free-roam sheep/rabbits/ducks, Travis cab and gate frames. It draws the
three conditional world food bags, screen-fixed `uisurround.bmp`, and
the selected food toolbar at (284,417) in that order.

The shared `Draw` operation now supports an optional **destination
clip rectangle**. World sprites use (20,20)–(620,400) and the original
DirectDraw backend clips both source and destination to that area,
while screen-fixed UI layers remain unclipped except by 640×480. This
prevents a walking sprite outside the world from painting over the UI.

**Retail qsort nuance:** The binary's equal-ordinary-depth comparator
returns -1 rather than 0, which is not a strict weak ordering.
The host-native compositor uses stable sorting for identical depths
instead of passing that invalid comparator into C++ `std::sort`.
Further output comparison is needed to establish whether the original
qsort implementation reorders equal-depth objects.

### Activity lifecycle

The `HerdingDriver` sources `Data/SubGame1/herd.txt` (strict
6 fixed points, four groups of 12/5/6/1). It accepts original
Easy/Medium/Hard from pregame state 0x0D and creates the stage-0
completion state.

On its first successful update it requests retail Pets Corner backing
music `data\\music\\petscorner.wav` and original managed sound **581**.
Subsequent frames use the supplied original simulation's entity records
for drawing. Once the simulator reports that every animal reached home,
the exact native `herding_completion_step` waits for active managed
audio, requests final line **599 or 600** once, waits again, writes
progress slot **50**, sets saved outer state **0x0E**, and exits into
shared Play Again state **0x3C**. The already reconstructed common
Play Again transition requests managed voice **573**.

**Full AI is not yet implemented in this driver.** It requires an
explicit `HerdingSimulationProvider` responsible for native
free-roam/food selection, follower steering, home-entry AI, Scruffty
collision and actual delivered-animal counts. The driver refuses
initialization without that provider. Invalid entity/output state
produces an explicit activity failure, not fake successful gameplay.

The native keyboard input mask travels through `ActivityFrameInput`;
the driver invokes recovered `pickles_keyboard_motion` and supplies
its exact 1.5-unit X/Y directions to the entity provider. Its
positions, collisions and AI remain the provider's responsibility.

The new source-only fixtures exercise viewport crops, depth layers,
Pickles' unusual odd-direction 39-column bank, animation counters,
initial managed voice/music, first-run Hard population expectations,
the managed speech-gated completion sequence, progress persistence,
and fatal AI provider failures.

**No new CMake target, C++ compile, device execution or visual capture
was performed.** This is a partial activity integration, not yet
a finished Pets Corner or complete game rebuild.

## Recovered food/follower/home-route behavior event bridge

`herding_behavior_bridge.hpp/.cpp` adds `HerdingRecoveredBehavior`,
a source-level state machine that composes the individual retail helper
rules already present in `herding_runtime.hpp`. This closes their
previous separation without inventing proximity/collision physics.

It validates exact animal populations (3/4/5 of each sheep, rabbit and
duck for Easy/Medium/Hard), then handles:

- Source-proven food pickup hotspots and the original Pickles/Travis
  voices **582–584 / 609–611**.
- Strictly ordered release of 20-entry follower-list indices when
  changing food, applying the two original `rand()%400` wandering
  targets without consuming guessed random values.
- Food-species matching and externally confirmed attraction collisions,
  inserting an animal only once and returning the original
  **585–590** follower voice selection.
- Externally confirmed Scruffty collisions, follower removal, temporary
  timer **200**, and the original randomized X target.
- Per-species route counters, assigning behavior **10–14** (depending
  on difficulty) from state 1, starting left/right gate timers **50**
  for the final sheep/rabbit, and issuing voice **596/597/598**.
- Explicit home-waypoint arrival events, moving 10–14 to 20–24 and
  then state **99**, decrementing the remaining count exactly once.

These inputs are event gates because **the original continuous movement,
steering, and proximity/collision detection are still missing**, not
because the player can arbitrarily teleport animals between states.
This bridge is a building block for the real `HerdingSimulationProvider`.

The activity provider also now reports ordered `Audio` effects to
`HerdingDriver`, so the above food, follower, dog and route voice IDs can
be delivered to the common physical managed-audio planner rather than
being silently discarded. It does not synthesize sound effects.

`herding_behavior_bridge_source_test.cpp` includes original-food,
follow/duplicate rejection, atomic follower release, Scruffty
distraction, all three species route allocations, final gate triggers
and the full **9-delivery Easy completion**. The tests are staged,
**not compiled or run**.

## New: exact executable geometry and source-confirmed physics bridge

The full original 311,296-byte PE32 executable was retrieved from the
connected `BTB-BBP/Exe` folder and directly disassembled for the
shared movement/collision primitives.

The results now committed:

- `retail_point_in_polygon.hpp` translates original helper
  **0x00428520**: integer-only ray crossings, signed 32-bit
  `IMUL/IDIV` interpolation, boundary inequalities, and the unusual
  return **0 inside / 1 outside**.
- `herding_navigation_boundary.hpp` translates Pickles'
  **0x00418C2D..0x00418CE4** response to leaving the nav polygon:
  keep original position if inside; otherwise try restoring X,
  then restoring Y, then restoring both. The binary's separate
  axis-slide eligibility input is explicitly required.
- `herding_retail_rect_contact.hpp` translates
  **0x004169A0..0x00416A2D**: four corners of rectangle A tested
  against B with inclusive edges. This is asymmetric and is not
  ordinary rectangle intersection.
- `full_game_herding_event_simulation.hpp/.cpp` now implements a
  genuine `HerdingSimulationProvider` adapter combining source
  movement snapshots with reconstructed food/follower/home state.
  Input frames are checked and applied transactionally. Rejected
  collision evidence, invalid route sequences or contradictory
  binary-verified Pickles movement prevent partial state changes.
- Render-animation frame/countdown values survive from one frame to
  the next, so the shared compositor no longer discards timer updates
  when a new movement snapshot is supplied.

The executable-equivalent geometry is documented in
[the retail geometry audit](herding-retail-geometry.md), with
independent source test files for polygon edge cases, axis-slide
branches and non-symmetric collision cases.

**Important gap:** the original per-entity physics source generating
steering, motion and contact candidates is still unfinished. The
simulation adapter does not invent animal positions or falsely
confirm contacts. The same uncompiled/full-game-not-yet-working
boundary remains.
