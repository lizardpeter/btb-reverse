# Golf activity

Golf is the next activity being converted into source-level form.

## Entry points

- `0x004145F0 InitializeGolfActivity`
- `0x00415830 UpdateGolfActivity`
- `0x004159F0 LoadGolfData` — direct reference to `data\\subgamegolf\\golfdata.txt`

## golfdata.txt

The retail data file is unusually helpful because it contains comments describing most of its own schema.

Ordered fields:

1. Bob X/Y position
2. Bob sprite-sheet filename
3. number of Bob rotations
4. Bob sprite frame X/Y size
5. Bob frame count
6. ball X/Y offset from Bob
7. three animated course-object records:
   - graphic position
   - graphic filename
   - hot-area position
   - frame size
   - frame count
   - repeat count
8. three spectator records:
   - filename
   - position
   - frame size
   - frame count
9. three attempt counts, one per difficulty
10. three power-bar speeds, one per difficulty
11. ball frame size
12. ball frame count
13. two additional X/Y pairs not described by the source comments

For the shipped data:

- attempts = `5, 4, 3`
- power-bar speeds = `2, 4, 6`
- ball frame size = `10 x 10`
- ball frame count = `1`
- trailing pairs = `(105,247)` and `(304,155)`

The three animated course objects are Flag, Windmill, and Clown. The three spectators are Wendy, Spud, and Wendy.

## Retail post-parse corrections

### Bob frame size

The file provides:

`159 184`

but immediately after parsing the retail loader overwrites the corresponding globals with:

`133 174`

The reconstruction therefore retains both the file metadata and the effective retail value.

### Ball offset and initial ball position

The file provides Bob at:

`(98, 195)`

and ball offset:

`(91, 140)`

The executable subtracts 2 from offset X and 5 from offset Y:

`(89, 135)`

It then derives the initial ball position:

```text
x = 98 + 89  = 187
y = 195 + 135 = 330
```

Those derived values are stored at globals `0x0050AD80 / 0x0050AD7C` and converted to floating-point ball coordinates during `InitializeGolfActivity`.

## Difficulty tuning

The power-bar speed table is indexed directly by the game's current difficulty global. In the active swing state, the selected speed is multiplied by the current bar direction and accumulated into a 0..1000 power value.

Thus the shipped difficulties intentionally make the power meter progressively faster:

- Easy: 2
- Medium: 4
- Hard: 6

The attempt table similarly decreases:

- Easy: 5
- Medium: 4
- Hard: 3

## Physics/math helpers

Immediately after the data loader are two clearly mathematical helpers:

- `0x00415D30` computes Euclidean distance between two integer points using sqrt(dx² + dy²).
- `0x00415D70` computes a quadrant-corrected angle between two integer points using absolute deltas plus `fpatan`, returning an integer angle.

At `0x00414A30`, another helper converts an integer angle plus magnitude into X/Y floating velocity components using cosine/sine. This is part of the golf-ball launch path.

## Clean-room reconstruction

The typed parser lives in:

- `reconstruction/include/btb/golf_data.hpp`
- `reconstruction/src/golf_data.cpp`

It intentionally preserves the two undocumented trailing points rather than guessing their meaning.

The next Golf pass is the runtime state machine: aim/rotation, oscillating power meter, launch velocity, obstacle/hole collision, attempts/scoring, and completion.
