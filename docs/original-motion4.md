# Four source-level original game movement coordinators

Source executable SHA-256:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

| PE32 address | Implemented portion |
|---|---|
| `0041B140` UpdateMazePlayerMovement | Previous-position preservation, source input, recovered 4-pixel current-node and link capture, direction mask and animation, keyboard 4-way movement, portal flags and screen transition. Uses the existing `maze::scan_player_navigation_nodes` implementation. |
| `00421360` UpdateSpudMazeBobMovement | Same recovered node/capture input kernel for five-screen Spud Maze, movement flags, original-style portal handling, repair-point collision, sound and completion callback. No synthetic device implementation is inserted. |
| `0041CCB0` UpdateMazeSpudNPC | Eight-state phase coordinator using existing original `maze_spud_controller.hpp` helpers for offscreen wait, selecting one of four packages, arrival, carry routing, reset path, and return home. Uses the already-recovered x87-derived `maze::move_actor_toward_node` kernel for NPC motion. |
| `00416A30` UpdateHerdingAnimal | Source-backed Herding entity record and original home-entry/home-waypoint progression for sheep, rabbit and duck; original sound/animation counter, motion and collision delegation, and delivered-animal count. |

**Important remaining limitations.** These are source-level entry-point
models, not x86 disassembly-equivalent replacements. The original movement
functions contain multiple x87 branch paths and mouse-to-heading behaviors
that require verified original register/state tracing. The Spud carrying
collision uses a required native host distance callback, and the Herding
roam/follow/temporary-target behavior remains in its existing separate
recovered modules pending final connection to this dispatcher. Original
portal destination tables need differential validation. The source models
use mandatory typed native IO: there are no fabricated DirectInput,
physics or audio devices.

Tests cover keyboard movement and freeze; portal capture; repair sound
and completion; Spud NPC spawn/package pickup/return/offscreen reset; and
Herding home-entry, final delivery, animation and collision handling.

All four entries passed Linux C++26, Python and native Windows x86/x64 CI
and are promoted to `compiles`, while the incomplete source-specific
behavior and original x86 parity remain explicitly unresolved. No additional original PE32 differential comparisons are added
and no full retail behavior parity is certified (0/615).

## CI validation

[Actions run 38787706201](https://github.com/lizardpeter/btb-reverse/actions/runs/38787706201)
passed every gate: Linux C++26 native core and **148/148 CTests**,
Python tools, Windows x86 native game/Win32 provider builds and tests,
and Windows x64 native game/provider builds and regression.

The compiled-source count is **595/615 (96.7%)**; **20** remain.
Original executable differential checks added in this patch: **0**.
Fully retail-verified functions: **0/615**.
