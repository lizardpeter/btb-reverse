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

## Source reconstruction

The typed parser lives in:

- `reconstruction/include/btb/golf_data.hpp`
- `reconstruction/src/golf_data.cpp`

It intentionally preserves the two undocumented trailing points rather than guessing their meaning.

The next Golf pass is the runtime state machine: aim/rotation, oscillating power meter, launch velocity, obstacle/hole collision, attempts/scoring, and completion.


## Runtime state machine

The core gameplay routine is `0x00415080 UpdateGolfGameplayState`, keyed by global state `0x0050AD9C`.

The high-confidence phases are:

| State | Meaning |
|---:|---|
| 0 | aim selection |
| 1 | oscillating power meter |
| 2 | calculate shot parameters / begin swing |
| 99 | Bob swing animation transition |
| 3 | moving ball / friction / obstacle-target checks |
| 4 | resolve and snap to a course-object target when close enough |
| 5 | evaluate shot outcome, play feedback, choose next transition |
| 6 | reset ball for another attempt and decrement remaining attempts |

### Aim

The aim control is clamped to **0..88**. The shot setup quantizes it to an even integer degree:

`angle = (aim / 2) * 2`

### Power meter

State 1 indexes `power_bar_speed_by_difficulty` and adds:

`power += speed[difficulty] * direction`

The value is clamped to **0..1000**. On hitting either end, the direction changes sign.

### Launch

State 2 derives the moving-ball speed from power:

`speed = power / 3 + 500`

The current aim is converted to the even-degree launch angle.

### Ball movement

During the moving-ball state:

`magnitude = speed / 80`

```text
x += cos(angle * pi/180) * magnitude
y -= sin(angle * pi/180) * magnitude
```

Friction uses the double constant **0.012** embedded at `0x0043B3B0`:

`deceleration = max(10, floor(speed * 0.012))`

Then:

`speed -= deceleration`

If speed drops below 20, retail sets it to zero.

### Course-object targets

For each of the three Flag/Windmill/Clown records, the target center is:

`graphic_position + hot_area_position`

For the shipped file that gives:

- Flag: `(330,178) + (99,160) = (429,338)`
- Windmill: `(230,15) + (129,164) = (359,179)`
- Clown: `(429,31) + (103,129) = (532,160)`

The executable uses a **15.0-pixel** target distance threshold. When a stopped ball is close enough, the resolution state snaps it to that target center and records the selected target index.

## Parsed-but-unused attempt table

The file explicitly contains difficulty attempt counts:

`5 4 3`

and `LoadGolfData` parses them into a three-element global table.

However, a full code-reference sweep shows no runtime read of that table in this executable. `InitializeGolfActivity` instead assigns the remaining-attempt counter:

`0x0050ABB0 = 5`

unconditionally.

The reset state decrements that fixed counter after a shot. Therefore the shipped comments/data appear to preserve an intended difficulty-dependent attempt feature that is inactive in this retail executable.

By contrast, the power-speed table `2 4 6` is actively read by state 1 using the current difficulty index.

## Clean-room runtime coverage

The Golf reconstruction now tests:

- retail data-file parsing
- Bob frame-size override
- ball-offset correction and derived start position
- aim clamp/quantization
- difficulty power-meter speed
- power-meter bounce at 0/1000
- launch speed equation
- angle/magnitude vector conversion
- per-tick ball movement
- 1.2% / minimum-10 friction
- stop threshold
- target-center calculation
- 15-pixel target detection
- retail fixed initial attempt count

The next Golf work is detailed collision/outcome/scoring behavior around states 4-6 and identifying the two undocumented trailing data points.


## Golf runtime state machine

The inner Golf state global is `0x0050AD9C`. `0x00415080 UpdateGolfRoundState` implements these states:

| State | Meaning |
|---:|---|
| 0 | Aim |
| 1 | Power meter |
| 2 | Launch setup |
| 99 | Swing animation delay |
| 3 | Ball flight |
| 4 | Resolve landing against course targets |
| 5 | Score/result feedback |
| 6 | Reset for next attempt |

The machine-readable map is in `ghidra/golf_states.csv`.

### State 0 — Aim

The retail aim value is clamped to **0..88**. Mouse/keyboard input adjusts the value until the shot is accepted, then the round advances to the power-meter state.

### State 1 — Power meter

The power value is bounded to **0..1000**. Each update adds:

```text
difficulty_speed * direction
```

where difficulty speed is the parsed `2 / 4 / 6` table and direction is `+1` or `-1`.

At either endpoint the value is clamped and direction reverses.

### State 2 — Launch setup

The aim is converted to an even integer angle:

```text
angle = trunc(aim / 2.0) * 2
```

The initial shot magnitude is:

```text
speed = power / 3 + 500
```

So retail shots begin in the range **500..833**.

### State 99 — Swing delay

The game intentionally uses state value **99** as an intermediate animation delay rather than placing it contiguously in the 0..6 state range. It advances a small nested frame/tick counter before entering state 3.

### State 3 — Ball flight

`0x00414A30 ComputeGolfVelocityComponents` computes:

```text
vx = cos(angle * pi / 180) * (speed / 80)
vy = sin(angle * pi / 180) * (speed / 80)
```

The ball applies:

```text
x += vx
y -= vy
```

each update.

Retail deceleration is:

```text
deceleration = max(10, int(speed * 0.012))
speed -= deceleration
```

When the resulting speed falls below **20**, it is forced to zero and the state advances.

### State 4 — Landing resolution

The stopped ball is compared against the three parsed course-object target centers. One verified proximity constant is **15.0 pixels**. A matched object index is stored for the score/result state; otherwise the target index remains `-1`.

### State 5 — Score and feedback

This state interprets the landing result and accumulated score/attempt state, then selects one of several voice-feedback groups from the global sound catalog. The exact semantic labels for every voice branch are still being assigned from the sound table.

### State 6 — Reset next attempt

The retail code:

1. restores ball X/Y from the initial integer ball position
2. clears the power value
3. decrements attempts remaining
4. resets power direction to positive
5. returns the inner state to 0

The outer `UpdateGolfActivity` handles the eventual completion / Play Again transition once attempts reach zero and active result audio has completed.

## Reconstructed runtime

`reconstruction/golf_runtime.*` now preserves:

- exact state enum values, including state 99
- 0..88 aim clamp
- even-degree aim quantization
- 0..1000 oscillating power meter
- per-difficulty meter speeds
- `power / 3 + 500` shot magnitude
- degree-to-radian velocity conversion
- retail speed decay constants
- 15-pixel target hit radius

The remaining Golf pass is mostly the detailed score/voice branch table and the exact special-target scoring semantics.
