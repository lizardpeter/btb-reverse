# Full-game C++26 integration gate

This is the **whole-game** milestone tracker. It replaces the temptation to
treat every newly buildable subgame helper as a completed game. The developer
requested that full compilation/preview packaging be deferred until the
source-level game itself is connected.

The original primary binary is the verified 311,296-byte PE32 executable
documented in [initial-analysis.md](initial-analysis.md); the tiny disc-root
launcher is not the game.

## Ten activities in the native 68-state router

The original game's top-level state machine
(`reconstruction/include/btb/game_flow.hpp`) has all ten of these
initialize/run pairs:

| Game-owned module | Retail outer states | Source-level recovery | Unified host integration |
|---|---|---|---|
| Herding / Pets Corner | `0x0E / 0x0F` | data/runtime/presentation models | Not hosted |
| Dinosaur | `0x14 / 0x15` | data, runtime, presentation and animation | Partial playable source host; retail visual/audio not complete |
| Spud Skate | `0x1A / 0x1B` | data/runtime/presentation models | Not hosted |
| Maze | `0x20 / 0x21` | data, movement, Spud NPC, presentation and transition models | Not hosted |
| Fireworks | `0x24 / 0x25` | layout, editor and sequence models | Not hosted |
| Squirrel | `0x28 / 0x29` | data, runtime and presentation models | Not hosted |
| Bob's Band | `0x2E / 0x2F` | editor, 5x24 grid, sequence I/O, 5 machine animations, 3 conductors and 24s sequencer | **Source-integrated** with the new shared 68-state root; actual bitmap/audio platform callbacks still not bound |
| Park Designer | `0x32 / 0x33` | data/runtime models | Not hosted |
| Golf | `0x36 / 0x37` | data/runtime + **new full-game round lifecycle, exact scoring and fixed-five-attempt source driver** | **Source-integrated** in `GameRoot`; source-backed partial presentation; exact input/animation and sound devices remain unbound |
| Spud Maze | `0x3A / 0x3B` | data/runtime models | Not hosted |

These are source-coverage descriptions, **not** percentages or claims of
frame-identical completeness. A module may have a recovered source-level
algorithm and still lack its production renderer, I/O, or frame dispatch.

## Production runtime integration order

1. **Game-root runtime:** retail WinMain / input timestamp and 68-state
   dispatch, correct profile selection and persistent main-loop globals.
2. **Shared platform backends:** DirectDraw surface/blit semantics, magenta
   key, lifetime/reload, DirectInput mapping and cursor, DirectSound sound
   groups/priority, Bink movie playback, printing and activity WAV music.
3. **Front-end:** all 12 generic UI screen layouts, hot-area hit masks,
   action codes, deferred sound arbitration, walkthrough movies and leave /
   replay overlays.
4. **Each activity:** implement init/run/presentation/unload on the same
   shared runtime with no game-specific shortcuts or hard-coded mock assets.
5. **Persistence:** 5 player profiles, unlock/progress flags, per-activity
   saved state, Band composition grids and shared completion codes.
6. **Fidelity verification:** per-state trace and frame-by-frame reference
   comparisons against the original executable, including no-op/dormant
   branches, transitions, and edge interactions.

The existing `btb_game` Windows preview demonstrates that native integration
is feasible, but it is only a bootstrap and should not be mistaken for a
fully integrated game. The reconstructed subsystem source should remain
independent of that prototype and move into the final shared runtime.

## Definition of source complete

Only mark the whole game source complete when:

- starting a new profile, loading an old profile and leaving the game follow
  the recovered 68-state flow;
- every shipped activity can be selected, completed, abandoned, replayed,
  resumed (where supported), and unloaded correctly;
- assets, sound and animations come from the original files and common
  resource code, not placeholders;
- all progress/unlock/save writes and file format boundaries are closed;
- every recovery claim is traceable to executable instructions or original
  file records; unresolved behavior is explicitly documented.

**Build policy for the current reconstruction stage:** continue source
recovery, commit in small units, and defer the next full compile/playable
packaging effort until the entire implementation is linked at source level.
Do not interpret an automated legacy CI check as proof of whole-game fidelity.

## October 8 source-integration progress

This source-only pass added a new platform-neutral whole-game coordination
layer and did **not** run a complete compile:

- `full_game_runtime.hpp/.cpp` contains the **original ten init/run pairs**
  connected to the existing 68-state dispatcher. It preserves modal/movie
  interception, screen-mode 13/14 state restoration, shared input-pulse
  clearing, activity init/run/unload ownership and centrally applied progress
  writes. The original per-state handler addresses remain in
  `game_flow.hpp`; the new coordinator does not claim to reproduce unbound
  UI, replay, or minigame behaviors.
- `full_game_bobs_band.hpp/.cpp` is the first real activity driver:
  reads original `Data/SubGameOpen/machinedata.txt`, restores its selected
  conductor/player composition, translates native frame commands into
  source-neutral draw/audio effects, writes slots 52..54 and shared code 6,
  saves the 480-byte composition and 20-byte `last.txt`, then releases the
  activity and routes to Play Again 0x3C.
- `full_game_front_end.hpp/.cpp` reads all **three** original UI files:
  `NumUiHotArea.txt`, `uiHotArea.txt`, `uiHotAreaReplace.txt`.
  The main **Activity Select** state pair 0x04/0x05 now has a real source
  adapter with all ten hit regions, strict edges, hover/click managed-sound
  effects, deferred click resolution, and the 0x22 locked-Finale interception.
  Other eleven front-end screens remain to be routed through shared UI.
  The host must supply genuine random values at screen initialization.
- `full_game_profiles.cpp` reads/writes `playerinfo.txt` and five
  `player*.txt` records with the existing retail profile/parser implementations,
  protects active profile ownership, and preserves exact profile-deletion
  behavior (clear only progress slots 50..64; retain stale hidden data).
  The common activity exit path now attempts profile saves when an output
  directory has been configured.
- Source regression fixtures were added for all of the above, but **not
  compiled or executed**. Passing earlier preview CI is not evidence that
  these newly added modules work in an integrated executable.

### Still outstanding before full recompilation

The missing work is not just linking these files. The other nine games need
equivalent source adapters, all twelve UI contexts need common rendering and
action handling, and the original resources must be provided to the shared
surface/audio/Bink backends. Frame-level retail verification and profile
round trips across every activity remain required. No new completion
percentage is asserted from source-file counts alone.

## Shared effect and source-asset bridge (source-only)

The whole-game activity pipeline now has an additional common layer:
`full_game_effect_pipeline.hpp/.cpp` converts recovered
`ActivityFrameOutput` draw/audio commands into preserved-order resource
instructions without inventing standalone minigame renderers. The new
`full_game_audio` source connects the already recovered 80-slot managed
sound policy to actual WAV catalog IDs, loading the native 20-byte filename
records from original `Data/sound/binklist.txt`. The asset resolver provides
case-insensitive original Windows path behavior across Linux/Windows and
installed-disc fallback.

The installed `Data/sound` catalog contains 1000 IDs (0..999); the separate
`loaddata` copy contains 996 (0..995). This discrepancy is recorded instead
of silently treating the files as identical.

See [shared graphics/audio effect bridge](full-game-shared-effects.md) for
interfaces, source-only regression fixtures, and outstanding DirectDraw /
DirectSound implementation steps. This milestone is **not an end-to-end
rendering or audio playback result**; no new full compilation was requested.

## Golf driver integration (source-only)

`golf_activity.hpp/.cpp` and `full_game_golf.hpp/.cpp` implement the
recovered native Golf state progression and game-root activity adapter:
original `golfdata.txt` parse; a hard-coded five attempts at all difficulty
settings (retail ignores the parsed 5/4/3 table); 0..88 aim and 0..1000 power;
even-angle launch; ball flight/friction; strict <15 target snap; final-score
managed voices; and completion progress **slot 62** followed by Play Again
**state 0x3C**. Unverified DirectInput gesture mapping and eight-by-eight
Bob-swing animation remain external instead of being invented as shortcuts.

See [Golf source lifecycle and remaining presentation gap](full-game-golf.md).
New source tests were written but **not compiled or executed**.

## Seven recovered native chooser/replay UI pairs

`full_game_generic_ui.hpp/.cpp` now provides the same file-driven
hover/click/deferred-voice logic for every original 12-screen table index,
including strict polygon hit tests for nonrectangular original hotspots.
`GameRoot` registers **seven confirmed 68-state pairs**: Dino chooser
(0x10/0x11), Music chooser (0x2A/0x2B), Spud chooser (0x16/0x17),
Adventure chooser (0x1C/0x1D), Play Again Yes/No (0x3C/0x3D),
Play Again difficulty (0x3E/0x3F), and Fireworks replay edit/view
(0x42/0x43). These sit alongside the specialized Activity Select pair
already wired on 0x04/0x05.

Source-only means negative actions (-1/-6, -2..-5, -20/-21,
-30..-32) remain observable and still require their original enclosing
state-specific interpretations. Rendering and real audio remain unbound.
See [generic UI source integration](full-game-generic-ui.md).

## Original UI bitmap paths and Golf controls

The uploaded `loaddata/uiBitmapName.txt` was recovered as a distinct
**23-name + END.bmp** source table. `full_game_ui_bitmaps.hpp/.cpp`
now parses it, and `GenericUiScreenDriver` can render its opaque original
backdrop by the explicit outer-state source index, without confusing those
23 file records with the 12 hot-area screen layouts. Menu overlays/animated
source frames remain unverified.

The installed BBC electronic booklet `ebooklet/golf.htm` independently
confirms Golf's left/right cursor/arrow aiming and two-step mouse/Space
shooting interaction. `GolfDriver` now consumes the generic click pulse
to enter power mode and lock the power on the following click. Fine aim
scaling, swing animation completion, voice completion and exact HUD remain
outside the driver until original-executable evidence closes them.

No new full compilation, binary packaging, or gameplay session was run.
