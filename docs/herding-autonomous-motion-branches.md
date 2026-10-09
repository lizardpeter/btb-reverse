# Pets Corner autonomous motion branches recovered from retail PE32

**Original executable:** `BTB-BBP/Exe/Bob the Builder - Bob Builds a Park.exe`,
311,296 bytes; SHA-256:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
The original PE32 text and data were independently inspected, including
`0x4172B0..0x417432`, `0x417B1D..0x417C85`,
`0x417FFE..0x418101`, and the CRT RNG at `0x42FFBA..0x42FFE1`.

These are **narrow original-game code recoveries**, not a complete
autonomous `OriginalHerdingMotionSource`. The entire activity still
needs the initializer, full state-0 dispatch, Scruffty patrol,
nav/polygon retries, frame order and Win32 differential validation.

## Followed animal -> tracked point: source 0x4172B0..0x417432

The branch first checks that the animal is present in the real
20-entry follower table at `0x50AF78`. It resolves a tracked source
coordinate from the original `0x50B2F4/0x50B2F8` table and compares
integer point distance using `0x415D30` against **30.0**, strictly.

If the point is reached:
- Remove the corresponding animal ID from the follower table
  (write -1).
- **Consume exactly one call to original rand (0x42FFC4), discarding
  the output**. This matters to later game RNG behavior.
- Mark source `movement_active = 1` and return 1; do not translate
  its position or update the animation on this branch.

Otherwise:
- Compute the original integer heading using `0x415D70`.
- Translate X/Y by `sin(heading)*current_speed` and
  `-cos(heading)*current_speed`, truncate float positions and set
  the original facing sector.
- Set movement_active=1.
- Subtract **5** from animation countdown at record offset +0x2C.
  When expired, reset to **100**, increment animation frame and
  wrap **3 -> 0**.

`herding_source_follower_approach.hpp` implements this specific
branch with an explicit tracked point. It returns a source RNG-call
count and follower-removal effect so the caller can preserve both
without guessing later scheduling. It does not select follower targets.

## Random recovery targets: 0x417FFE..0x418101

The binary stores these four pairs beginning at `0x443AC8`:

| Index | X | Y |
|---|---:|---:|
| 0 | 100 | 766 |
| 1 | 400 | 400 |
| 2 | 891 | 616 |
| 3 | 1128 | 659 |

**Critically, it calls rand()%4 twice, independently**. The first
index selects the X coordinate, the second selects Y. Thus there are
**16 possible destinations**, not four fixed random points.

The source then calls heading `0x415D70` with a **signed sprite
anchor**, not the ordinary visual center. The x86 instructions
`0x417FCA..0x417FFC` compute
`actorX = currentIntegerX + trunc((sourceLeft-sourceRight)/2)`
and the equivalent Y expression. For normal positive-width cells,
this is **left/up from the entity's top-left**, not right/down.
The actor then moves exactly **3.0 units**, stores float/int position
and sprite facing, resets speed to zero, and tests transformed
`herd.txt` navigation polygon 0 with the original `0x428520`.
If outside, the assembly jumps back to `0x417FB2` to copy current
integer X/Y to previous X/Y, recompute the anchor, consume another
two RNG rolls and translate the **current accumulated float position**
again. This all happens in the *same frame*.

`herding_source_recovery_target.hpp` recovers the point table and
independent roll sequencing. `herding_source_roaming_recovery.hpp`
implements a single attempt. **`herding_source_roaming_loop.hpp`
now reconstructs the complete source-controlled retry sequence.**
The loop keeps the candidate's evolving float X/Y, copies native
integer X/Y to prior-position fields on each attempt, derives the
signed half-cell steering anchor anew, consumes two sequential
process-global random results, and exits only on a true polygon-inside
result. A resumable work budget prevents an unbounded host call when
the source polygon/data is invalid, but budget exhaustion is a
**Pending** result, never falsely reported as a retail exit or a new
frame. The caller must resume while exclusively holding the original
RNG before allowing any other game subsystem to run.

The production overload takes `OriginalRetailRandom&` directly,
so consecutive X/Y draws are consumed from a single shared
gamewide random state instead of a local generator.
`HerdingEventSimulation` can also independently verify an upstream
source frame's pre/post RNG state, attempt count and final entity
coordinates by replaying this loop with the original `herd.txt`
polygon. The source bridge now preserves previous-position fields
written by the motion routine instead of fabricating them.

## CRT source randomness: 0x42FFBA and 0x42FFC4

The original executable links an inline Microsoft CRT-style generator:

```cpp
state = state * 0x343FDu + 0x269EC3u; // uint32 wrap
return (state >> 16) & 0x7FFF;
```

`srand` writes directly to global `0x447558`. Its initial PE
data value is 1, but game startup does NOT leave it there:
`0x4027E7` performs an initial seed from a time-related function,
and then `0x4027F4` calls Win32 **GetTickCount** (IAT
`0x43B164`) and `0x4027FB` calls `srand` again.
The second seed **overwrites** the first. Native gameplay must
use the observed startup tick count, and then preserve **one
serial RNG call order across all minigames**, or it will diverge.

`retail_crt_random.hpp` provides that exact LCG model.
Source regression examples use `srand(1)` only as a predictable
test seed, not a claim about actual gameplay startup.

## Roaming speed/animation tail: 0x417C0C..0x417C85

A specific free-roaming branch compares current speed against
source `0x43B3E0 = 0.4f`. If strictly **greater than 0.4**,
it subtracts 0.4; otherwise it sets speed to original literal
**0.1f**. The source also subtracts **20** from the animation
countdown, reloads 100 on expiration, and advances/wraps the
species-specific frame range:

- Sheep: frames 6..9
- Rabbit: frames 3..6
- Duck: frames 0..5

`herding_source_roam_braking.hpp` models this branch. This is
different from the earlier recovered +0.01 speed acceleration
path and from the per-render animation update, so they must NOT be
blindly called once every frame.

## Animal chatter: 0x417B1D..0x417BB0

The idle/reaction branch first makes `rand()%8`.

- If zero, it plays a randomly chosen sound in the species group,
  **without checking existing sounds**.
- Otherwise it checks all three sound IDs in that group through
  the source manager at `0x402C60`; only when none is active
  does it consume another `rand()%3` and request a sound.
- IDs are `0x331 + species_group*3 + rand()%3`
  (817–825), with priority **10** and sound class **0**.

`herding_source_animal_chatter.hpp` models the decision and exact
number/order of original PRNG draws. Actual managed-sound playing
statuses must come from DirectSound, never a synthetic elapsed timer.

## Source regression fixtures and remaining work

New **uncompiled, unexecuted** source fixtures:

- `herding_source_follower_approach_source_test.cpp`
- `herding_source_roaming_recovery_source_test.cpp`
- `herding_source_roaming_loop_source_test.cpp` (three-attempt same-frame
  recovery versus resumable chunks, exact RNG usage and original
  previous-position updates)
- `herding_source_roam_braking_source_test.cpp`
- `herding_source_animal_chatter_source_test.cpp`
- `retail_crt_random_source_test.cpp`

Machine-address source audit: `ghidra/herding_motion_new_branch_evidence.csv`.
Some portable trigonometric paths remain mathematically equivalent
but **not proven bit-identical** to retail x87 FSIN/FCOS/FPATAN.

To finish Pets Corner autonomously we still must reconstruct exact
initial entity and target allocation, multi-branch frame dispatch,
normal free roam and pursuit, Scruffty movement, the within-frame
polygon retry loop, click/input event generation, and the full
Windows game's physical presentation path.

No full build or native execution has been performed under the
standing instruction to defer compilation.
