# Full-game completion status

The completion criterion is the reconstructed C++26 Windows executable playing
all original activities through their normal outcomes, preserving profile and
activity saves, and returning through the original menu/replay flow. A registered
driver, translated function count, link smoke test, or packaged EXE does not
establish that criterion.

## Confirmed blockers

- Park Designer: `win32_recompiled_host.cpp` constructs `ParkDesignerDriver`
  without a `ParkDesignerNativeService`. The first activity frame fails. Native
  scene resources and the full 0040E440 editor interaction must be implemented
  and connected; existing pure scene/control helpers are not a playable editor.
- Pets Corner: the native motion source covers substantial follower, home-route,
  food and Scruffty behavior, but the remaining animal/entity collision and
  group-two recovery ordering still need full source integration.
- Original boot presentation is bypassed by the current native host.
- Current production video playback still depends on the original x86 Bink DLL;
  the separately recovered codec is not yet the production host backend.
- No complete Windows playthrough of every activity has established original
  game behavior or save/reload parity.

## Pets Corner movement correction, 2026-10-21

The original PE32 (SHA256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`)
confirms the per-update movement-flag clear at 00418ED9..00418EF0. The native
source now clears these flags every update, allowing an animal that previously
followed or fled to resume default roaming on a later frame.

The non-follower branch 004175F0 and 00417B1D..00417C85 now runs when Pickles
is strictly less than 80 units away without attracting the animal. It uses the
original negative-half-width anchor, outward integer heading, distance times
float literal 0043B3E4 (bits 3D088889), speed braking and species animation.
Original chatter consumes the shared MSVC random stream and receives measured
per-ID managed sound status for 817..825 from the native device observation.
The transactional verifier checks the resulting motion and chatter evidence.

The native regression exercises near/far transitions, correct movement-flag
reset, managed sound output and rejection/rollback of corrupted movement.
Previously unregistered Herding regression sources are now CTest targets with
assertions enabled in Release, and are included in both Windows CI builds.

These changes do not complete the full game. Floating trigonometry remains a
source-equation reconstruction rather than proven bit-exact x87 execution.
Sound status is observed at the frame boundary; same-frame sound arbitration
against multiple animal requests still needs differential verification.

### Branch-specific original execution check

`tools/herding_near_escape_differential.py --exe <retail-exe> --native
<btb_herding_near_escape_audit>` executes original 00416A30 with Unicorn,
using the SHA256-verified executable, controlled group-two geometry and measured
sound-status fixtures. The matching C++ probe is an optional CMake target.
All **864 cases passed** across three species, eight relative positions,
three speeds, two animation/countdown states, three RNG seeds and two sound
status masks. Integer position, facing, animation, movement flag, final RNG
state and sound ID/priority/class are checked; float coordinates and speed use
an absolute tolerance of 0.0001. This validates the near-animal branch within
those controlled dependencies, not the pre-dispatch polygon recovery or the
full original activity.

The 17 focused native/source/session tests also passed in a local GCC 13
C++23 compatibility build; project C++26 requirements are unchanged. The
C++26 Linux CI suite passed for commit a95d973. Windows CI/playthrough status
must be checked separately.
