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
