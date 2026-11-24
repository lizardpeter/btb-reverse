# Retail Pets Corner gameplay probe (2026-11-24)

This optional diagnostic uses the actual installed `herd.txt` and drives the
native activity only with original normalized keyboard input. It never
changes an entity position, behavior state, save file or progression flag
directly. Its external route planner reads scene observations to steer Pickles.
The route planner is not production game AI.

Build:
```
cmake --build build --target btb_retail_herding_playthrough
```
Run the executable with `<installed-root>/Data/SubGame1 0 1`, then with
difficulty 1 and 2, seed 1. The probe requires the original game data;
no retail data is distributed or written. It checks the exact Pets Corner
progress slot, completion code, replay class, saved activity initializer
and outer-state transition.

Local pre-commit probe on 145222e using the shipped data and seed 1:
- Easy: 9/9 delivered, 5,708 ticks, completion/exit emitted.
- Medium: 12/12 delivered, 46,120 ticks, completion/exit emitted.
- Hard: 15/15 delivered, 43,902 ticks, completion/exit emitted.
- Separately: 24 randomized runs of 4,000 input ticks (96,000 total) on
  all three difficulties, no fatal simulation errors.

These are native source-logic results, not graphical Windows certification:
the diagnostic intentionally observes idle managed sound and doesn't host
DirectDraw/DirectSound or validate actual voice timing.
