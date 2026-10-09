# Original Herding steering audit — C++26 source recovery

## Retail source and verified offsets

Input: `BTB-BBP/Exe/Bob the Builder - Bob Builds a Park.exe`
(311,296 bytes; SHA-256:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`).
This is the real retail PE32 and not guessed movement math from the
sprites, instruction booklet or generic game-engine formulas.

The `UpdateHerdingAnimal` code invokes an integer heading helper at
`0x415D70`, quantizes the returned heading into one of eight sprite
directions, and updates entity float X/Y. The same kernel occurs at
`0x416C45..0x416CA5` (first-stage approach) and
`0x416DCE..0x416E2E` (second-stage home approach).
A related frame at `0x416FF8..0x41704B` moves followers using
the distinct **3.0** movement magnitude.

The following literal original IEEE-754 constants were verified
in the executable's `.rdata` section:

| PE address | Raw 32-bit bits | Floating value | Meaning |
|---|---|---|---|
| 0x43B384 | `0x3C8EFAB5` | 0.017453530803322792 | Radians per integer degree |
| 0x43B438 | `0x3F4CCCCD` | 0.800000011920929 | Roaming speed threshold |
| 0x43B434 | `0x3C23D70A` | 0.009999999776482582 | Speed increment |
| 0x43B430 | `0x40400000` | 3.0 | Follower movement magnitude |
| 0x43B378 | `0x41200000` | 10.0 | Home waypoint arrival radius |

The source `herding_source_steering.hpp` carries all four movement
constants and has C++26 `static_assert(bit_cast<uint32_t>())` checks
against their raw PE bits.

### Decoded executable steering kernel

Once source integer `heading` has been calculated:

1. Compute `radians = float(heading) * float_at_0x43B384`.
2. Update `entity.x_float += sin(radians) * entity.speed`, storing
   the resulting value to float32.
3. Update `entity.y_float -= cos(radians) * entity.speed`, also storing
   to float32.
4. Call `0x4304D0` to truncate float coordinates toward zero into
   integer X/Y (not floor on negative numbers).
5. Mark movement active (`+0x48 = 1`).
6. Compute sprite facing index by integer
   **`(heading + 22) / 45`** (signed truncating division), and
   subtract 8 once if the quotient is 8 or more.
7. In the two roaming/home approach branches, if speed is
   **less than 0.8**, add source literal **0.01** to the original
   float speed after applying that frame's translation. Do not
   prematurely clamp, replace with `max`, or apply an external
   frame-time multiplier.

The actual source operations are a sequence of x87
`FILD/FMUL/FSIN/FCOS/FMUL/FADD/FSUBR/FSTP`. The portable
implementation uses `std::sin/std::cos`, matching the derived
equations, float32 storage, integer truncation and sprite sector
quantization, **but not guaranteed bit-exact x87 trig**.

### Integrated evidence checks

`HerdingEventSimulation` now accepts an optional
`HerdingSteeringEvidence` for specific original motion paths. It
contains the entity index, pre-step X/Y, native integer heading,
speed and whether the original roaming acceleration branch is active.

For each such probe the adapter independently checks the resulting
record's float32 X/Y, truncated integer X/Y, facing index and
post-step speed against `original_herding_steering_step`.
A probe with an invalid entity index, nonfinite coordinates,
wrong facing or wrong motion aborts the frame before food, follower,
home, speech, score or save state can be mutated.

Because of the explicitly different x87 precision, **0.001 world-unit
tolerance** is provisionally allowed for float positions/speed.
This is an evidence check for source reconstruction, not a claim of
verified retail-perfect frames or a replacement for a native
differential test.

Regression *source*, not an executed test:
- `reconstruction/tests/herding_source_steering_source_test.cpp`
  covers cardinal directions, facing sectors, signed truncation,
  original 3.0 movement and speed ramp.
- `reconstruction/tests/full_game_herding_event_simulation_source_test.cpp`
  now checks acceptance of an expected steering probe and rejection
  of a contradictory position without committing any behavior state.

## Remaining Pets Corner code

The next critical functions are the complete source heading
`0x415D70` quadrant/quantization logic, animal random-target and
behavior-state decisions around `0x416F49..0x418101`,
the input/recovery path in `UpdateHerdingActivity`, and
actual initializer entity position/population sequencing.

At present `OriginalHerdingMotionSource` is still unimplemented.
This pass **closes a verifiable steering kernel**, not full animal AI
or a native running Pets Corner. Full build, CTest, runtime capture
and original-game differential validation were deferred.
