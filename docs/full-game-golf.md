# Golf source-level activity driver

The C++26 Golf gameplay helpers already reconstructed (native
`0x00415080`) now have a **shared-game lifecycle**:

- `reconstruction/include/btb/golf_activity.hpp`
- `reconstruction/src/golf_activity.cpp`
- `reconstruction/include/btb/full_game_golf.hpp`
- `reconstruction/src/full_game_golf.cpp`

This is separate from the former Dinosaur-only preview. The source
driver implements the original `0x36 GolfInit / 0x37 GolfRun` pair via
`ActivityDriver` and passes score/audio/progress effects to `GameRoot`.

## Original file confirmation

I inspected the retail-installed `Data/SubGameGolf/golfdata.txt`:

- Bob starts at `(98,195)`; the source file lists size `159×184`,
  but native overwrites it with `133×174`.
- Ball offset `(91,140)` is corrected by native `(-2,-5)`, so the
  starting ball is **(187,330)**.
- Flag target center **(429,338)**, Windmill **(359,179)**, Clown
  **(532,160)**, each from the parsed bitmap position + target offset.
- The file contains attempt counts **5/4/3** and power meter speeds
  **2/4/6** for three difficulties, but **only the speeds are live**.
  Initializing Golf always assigns **five attempts** at every difficulty
  in this retail build. The adapter deliberately does not reduce attempts
  to 4/3 for medium/hard.
- The original file names all three targets and spectator sprite paths;
  these are forwarded to the shared draw stream, with the original
  `golfbg.bmp` background and Bob sprite path.

## Implemented inner round lifecycle

`golf::Round` consumes explicit input/action events and retains retail
inner states **0,1,2,99,3,4,5,6**. State 99 is a swing-animation delay,
not a conventional physics state.

1. **Aim 0:** clamp aim 0..88; an input acceptance event starts power.
2. **Power 1:** meter oscillates 0..1000 with source-loaded speed 2/4/6;
   acceptance freezes the current displayed value.
3. **Launch 2:** even-degree quantization, launch speed
   `power/3 + 500`, switch to state 99.
4. **Swing 99:** require an external original-animation completion event.
   The animation is known to use nested eight-by-eight tick/frame counters,
   but that counter's final wiring to Bob's source sheets is not yet
   validated. The gameplay layer does not insert an invented delay.
5. **Flight 3:** use recovered cos/sin velocity, integer speed/80,
   deceleration max(10, truncated 1.2%), stop below 20.
6. **Landing 4:** check all three targets in original order, strict
   distance <15 using truncated integer ball coordinates; snap if matched.
7. **Feedback 5:** perform precise point/voice rules once per attempt,
   then require an external managed-voice completion event.
8. **Reset 6:** restore ball position, clear power, reset direction, lower
   attempts by one. When the fifth attempt completes, set **Golf progress
   slot 62 to 1**, unload, and enter the Play Again state **0x3C**.

The final-attempt special speech uses IDs **133,134** if score >4 or
**135** otherwise, replacing normal hit/miss voices. Ordinary successful
hits yield 102..113, and miss cases follow the native aiming/distance
classifier preserved in `golf_runtime.cpp`.

## Present source-backed artwork versus still unverified presentation

`GolfDriver::compose_frame` currently emits **eight source-backed layers**:
the original `golfbg.bmp`, three target objects, three spectators, and
Bob's parsed `golfsprite_8bit.bmp`.

This is **not the finished Golf compositor**. It does not yet have
proven source rectangles/animation frames for all objects, the actual ball
sprite, aim indicator, arrow, power bar, shot result overlay, or scoreboard.
The driver does not invent missing bitmaps. These eight commands are kept in
a staged adapter until the exact retail draw ordering is recovered.

The input bridge is similarly explicit: `queue_control` receives
`aim_delta`, `accept_aim`, `accept_power`,
`swing_animation_finished`, and `feedback_audio_finished` from a future
original-DirectInput / animation / sound-device adapter. The generic
`ActivityFrameInput` does not contain a recovered retail keyboard-to-aim
mapping, so this driver does **not** silently interpret arbitrary clicks as
the native swing controls.

## Deferred source regressions

- `reconstruction/tests/golf_activity_source_test.cpp` exercises five
  consecutive original-data shots, fixed attempts across difficulty, physics,
  score/audio completion gating and progress termination.
- `reconstruction/tests/full_game_golf_source_test.cpp` routes the same
  source model through `GameRoot` state `0x36` to `0x37`, checks
  data-backed presentation effects, and verifies the slot-62/Play Again
  handoff after the fifth completed round.

**No compilation or execution of these newly added source tests occurred**.
A complete native game still needs the other activity drivers, sprite/DirectX
providers, exact UI input maps and remaining front-end state handlers.
