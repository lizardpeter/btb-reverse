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
| Herding / Pets Corner | `0x0E / 0x0F` | data/runtime/AI helper/presentation models | **Partial source driver**: real herd.txt, native render layers, sound 581, completion/progress 50; requires unrecovered full entity-AI provider |
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
  Other frontend screens still require their original rendering and modal behavior.
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

The missing work is not just linking these files. The other eight games need
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

The original Dino, Spud, Adventure, Music, Activity Select Back/Help,
and replay Yes/No/Edit/View/difficulty **negative-action state transitions
are now reconstructed and applied by the common root**. Instruction,
walkthrough, Help presentation and source-variant side effects still require
their respective runtime/renderer adapters. Rendering and real audio remain
unbound. See [generic UI source integration](full-game-generic-ui.md)
and [x86 menu branch evidence](full-game-menu-actions.md).

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

## Binary-verified chooser/replay action closure

A fresh direct x86 audit recovered the exact `action+6` branch tables
for the Dino, Spud, Adventure and Music choosers, plus the replay Yes/No,
replay difficulty and Fireworks Edit/View paths. The former root dropped
negative actions on the floor; it now consumes the returned actions and
updates the original global state/variant fields.

The Music chooser's value 0/1/2 is now supplied to
`BobsBandDriver` to open Bob/Wendy/Farmer's respective original sequence
file. Both Band and Golf source drivers now set the native saved init state
(0x2E or 0x36) and replay class 1 when entering Play Again. The replay
difficulty screen updates Golf's difficulty before the next initialization.
The Activity Select Back action now correctly enters five-sign
Player Profile state 0x01. Fireworks Edit/View routing is represented, but
Fireworks animation/resource reinitialization still needs a full driver.

See [exact x86 menu routing](full-game-menu-actions.md) and
[per-branch CSV evidence](../ghidra/gameflow_menu_action_routes.csv).
No full compile or source-test execution has occurred.

## Retail common replay preparation and input-interruptible voice

Direct disassembly of the original `0x42CFD0` routine has closed the
shared Play Again prelude used by the original activity exit flows.
It releases the prior activity-music group, draws the replay underlay,
sets native latch `0x51C300 = 1`, stops all currently managed sounds,
plays **`PA_BOB_01.wav` (ID 573)** with priority 50 / class 1, and marks
that managed sound input-interruptible.

`full_game_replay_transition.hpp` captures this behavior; on Golf/Band
exits, `GameRoot` now sends the stop/play instructions and records the
replay-active latch. `full_game_audio.cpp` honors the original
input-interruptible slot state for later managed sound reaping.

This is source-level audio/lifecycle coverage, **not** actual DirectSound
or DirectDraw output. Exact reference: [native replay preparation and
menu branch evidence](full-game-menu-actions.md).

## Ten original instruction/walkthrough pairs — binary-audited

The ten original pregame setup/update pairs (**20 of the 68 outer
states**) have now been source-routed into `GameRoot` with the existing
table-driven `GenericUiScreenDriver` and a separate retail pregame
action policy. Exact `-6..-1` action branches are recovered and indexed
in [60-row original jump table evidence](../ghidra/pregame_action_routes.csv).
Back returns to the appropriate chooser, while the **Spud Skate -2**
branch specifically enters init 0x1A, unlike the other Easy/-2 paths.

The four pregame setups for Herding, Fireworks, Squirrel and Park Designer
now preserve native **global Bink movie mode 14**. They keep their setup
state while the host observes unfinished playback and continue into the
update state on the same frame Bink completes. The common input-pulse
clearing now also suppresses that frame's host click, preventing phantom
instruction selections.

Source binding now connects:
- the 23 original `uiBitmapName.txt` BMP backgrounds, including subgame-
  selected Dino, Spud and Adventure source indices;
- the ten original `binkwalk.txt` Bink walkthroughs plus ten spoken-help
  WAVs and three additional NULL-movie help entries;
- the four `startupmovie.txt` global intro films at indices
  **0, 4, 5, 7**, distinct from the looping walkthroughs.

`full_game_pregame_resources.cpp` joins original screen and walkthrough
sources; the original pregame UI factory retains the bitmap table and
can change its background if the selected subgame changes. Golf's driver
now accepts the first-run difficulty written by pregame state 0x35,
not just replay difficulty 0x3F.

**This is source-level routing/resource planning, not completed pregame
presentation.** Real Bink decode/timing, independent walkthrough looping
and speech, UI overlays and Help, automatic no-difficulty Start completion,
native DirectDraw/DirectSound, and remaining activity drivers are still
outstanding. No new compile or test execution was run.

See [full pregame audit and integration boundaries](full-game-pregame.md).

## Native Windows frame execution boundary — source-only

The previously separate `GameRoot` state coordinator,
`GameEffectPipeline` original asset/sound planning, and original
`binkwalk.txt`/`startupmovie.txt` catalogs are now joined in
`NativeGameSession`. On each frame it requests actual managed buffer
and Bink completion observations, advances one original outer state,
opens/closes real source-backed walkthroughs and global intro movies,
plans sound and BMP operations, and submits the entire ordered frame to
an explicit `NativeGameDevice`.

The new Win32 platform source is in:

- `win32_original_directdraw.hpp/.cpp`: real DirectDraw7 640×480
  fullscreen flip chain or windowed primary/offscreen Blt, original BMP
  pixels, magenta source key, proper sprite crops, and surface-loss reload.
- `win32_original_directsound.hpp/.cpp`: real DirectSound8 80-slot
  managed buffer observation/play/stop, original RIFF WAV sample upload,
  backing audio and individual activity sounds.
- `win32_original_game_device.hpp/.cpp`: binds both physical devices
  to `NativeGameSession`; requires an actual original Bink provider.
- `win32_original_bink_exports.hpp/.cpp`: strict source loader for
  exact original `binkw32.dll` exports. This is **not Bink decoding**.
- `full_game_blt_clip.hpp`: 640×480 native sprite source/destination
  clipping for partially offscreen original game animations.

The original Bink DLL, its complete calling ABI and independent decoded
frame-copy service are **not verified/hosted**. Additionally, the
seven activities with no newly unified driver plus Herding's still-missing full entity AI and the exact screen-specific
overlay sprite composition remain significant blockers. The existing
Dinosaur-only preview is unchanged.

Regression *sources* include
`full_game_native_session_source_test.cpp` (movie/audio/frame order,
missing-adapter/missing-asset failures, global Bink pixel preservation)
and `full_game_blt_clip_source_test.cpp` (all viewport boundaries).
**Neither was compiled or executed.** There has been no end-to-end
Windows native binary launch or visual/audio parity assertion.

See [native Windows platform source status](full-game-native-platform.md).

## Pets Corner / Herding source adapter — 0x0E / 0x0F

The recovered `herding_presentation.hpp` data now feeds a native
`HerdingScene` compositor. It emits original source-backed BMP commands
for the background, animated/depth-sorted entities, three conditional
world food bags, UI surround and selected food toolbar. The original
600×380 world viewport at (20,20) is preserved via a new optional
per-Draw destination clip; `Win32OriginalDirectDraw` clips both source
and destination to that subviewport without damaging the UI. The
strictly nontransitive retail equal-depth qsort behavior is modeled as
stable equal-depth order in portable C++ pending differential validation.

`HerdingDriver` now participates in `GameRoot`'s source-level
0x0E/0x0F activity lifecycle. It parses original `Data/SubGame1/herd.txt`,
accepts the exact pregame difficulty/normalized 1.5-unit directional
input, requests initial Pets Corner music and managed voice **581**,
then uses the recovered zero-remaining-animal, speech-wait, last
voice **599/600**, progress **slot 50** and Play Again **0x3C**
completion sequence. It preserves original saved state **0x0E**.

**The full animal AI is still missing.** The driver requires a
`HerdingSimulationProvider` with actual source-derived entity,
follower, food selection, home-route and Scruffty collision behavior.
It fails explicitly if that provider is absent or supplies an invalid
entity frame; it does not invent animal motion. Its replay-class
write still needs original executable evidence.

New staged (not compiled) regression sources:
`full_game_herding_presentation_source_test.cpp` and
`full_game_herding_source_test.cpp`. Existing
`full_game_blt_clip_source_test.cpp` now also covers nested world
viewport cropping.

See [source Herding reconstruction and remaining AI gaps](herding-activity.md).

### Pets Corner source behavior decisions beyond the renderer

`herding_behavior_bridge.hpp/.cpp` now composes the existing native
food hotspot, follower list, Scruffty distraction, animal home-route,
gate timing, and terminal delivery helpers into one event-level source
state. Every state change is triggered by a retail-confirmed event
reported by a future original movement/collision provider, rather than
invented distance checks or steering.

The behavior bridge validates the population on each retail difficulty,
preserves original voice choices and random draw arguments, and
decrements undelivered animals only at confirmed terminal waypoints.
Its new source fixture models all nine Easy-level home deliveries and
both species-dependent gate events.

`HerdingSimulationProvider` now emits ordered game-audio operations
alongside the actual entity records, allowing food/follower/Scruffty
voice requests to reach `GameEffectPipeline`. Continuous navigation,
autonomous animal steering/collision and the actual event detector are
not complete.

No compilation, device execution or original-binary differential
validation occurred during this pass.

### Direct retail executable geometry audit — source-first

The installed `BTB-BBP/Exe/Bob the Builder - Bob Builds a Park.exe`
was retrieved and verified at 311,296 bytes with matching SHA-256.
Three original x86 paths are now reverse translated rather than
approximated: `0x428520` integer polygon containment (0=inside),
`0x418C2D` Pickles navigation axis-recovery order and its now-resolved
mouse-mode/directional-count gate, and `0x4169A0` one-way inclusive
four-corner rectangle collision. The Scruffty collision callsite at
`0x417A2E` also confirms **animal rect first, dog rect second**,
computed from the genuine sprite dimensions in 0x64-byte records.

`HerdingEventSimulation` implements the previously abstract
`HerdingSimulationProvider` using source-confirmed motion records and
contact events. It compares Pickles updates against the recovered
polygon/axis-slide algorithm (using the source input count and
`0x443A9C` mouse mode) and **derives Scruffty collision rectangles
directly from the original entity records**, then applies
already-recovered food, follower, home-route and sound behavior
transactionally.

The remaining prerequisite is a complete
`OriginalHerdingMotionSource` that produces the actual retail
per-entity steering, free-roam movement and contact probes.
Therefore **Herding is still only partially source-integrated**:
its behavior transitions are real, but native full gameplay is
not complete. No code was compiled, tested or run during this pass.

Evidence and code: [Herding retail geometry](herding-retail-geometry.md)
and `full_game_herding_event_simulation.cpp`.

### Retail heading, per-frame animal steering and waypoint arrival closure

Another direct audit of the original `UpdateHerdingAnimal` binary
recovered the shared integer heading routine `0x415D70` and two
steering loops `0x416C45/0x416DCE`. Source now preserves the game's
approximate radian conversion, unusual 9999 vertical fallback
producing **359° Up**, signed quadrant conversions, 45° sprite cell
quantization, X sin/Y minus cos movement, and **0.01** speed increase
while below **0.8**, with bit-checked original PE float constants.

`HerdingEventSimulation` can verify both the **original target
coordinate → integer heading** and the **heading → animation/
movement/speed** result from the underlying animal movement source.
The floating implementation still needs native x87 parity verification.

The original x87 distance helper at `0x415D30` and the strict
`distance < 10` branch also now gate animal home-route transitions.
`HerdingRecoveredBehavior` no longer accepts bare "arrived"
events from remote coordinates: the actual animal must be within ten
units of the correct entrance waypoint and then the correct second-stage
destination. Regression sources now move each of nine Easy-mode
animals through both home points before decrementing the undelivered
counter.

This substantially reduces the amount of unverified animal AI, but
the continuous target-selection/free-roam/steering dispatch and
collision-event generator remain unimplemented. No full build
or original-game execution was performed.

See [original angle, steering and distance audit](herding-native-steering.md).

### Original follower and autonomous-roam kernels (latest executable audit)

A direct audit of the original 2002 PE32 has added several additional
source-level movement and audio branches:

- **Tracked follower approach** (`0x4172B0..0x417432`):
  strict 30-unit point arrival, original follower-slot removal,
  preserved discarded `rand()` call, otherwise moving on the
  native integer heading with 5-countdown / three-frame animation.
- **Out-of-bounds random target recovery** (`0x417FFE..0x418101`):
  authentic four point pairs at `0x443AC8`, but **two independent
  `rand()%4` selections** for X and Y (16 possible destinations),
  source three-unit movement and polygon acceptance. The full
  same-frame retry loop remains unreconstructed.
- **Free-roam speed/animation branch** (`0x417C0C..0x417C85`):
  speed reduction by 0.4 only above 0.4, otherwise reset to 0.1,
  countdown -20 and species-specific animation frame wraps.
- **Animal chatter** (`0x417B1D..0x417BB0`):
  source `rand()%8` override and three managed-sound status checks,
  with source sound IDs 817–825, priority 10 / class 0.
- **Process-global original CRT random** (`0x42FFBA/0x42FFC4`):
  the exact 32-bit Microsoft LCG, with a shared 15-bit result,
  and startup's final **GetTickCount -> srand** overwrite at
  `0x4027F4/0x4027FB`. Constant seed 1 is only a test fixture.

This is genuine additional source coverage of the continuous
AI movement and source RNG schedule, but **not a complete
`OriginalHerdingMotionSource`**. These isolated branches still
need exact per-frame ordering, initializer state and other navigation
behavior integrated with the shared simulation.

[Executable-address evidence](../ghidra/herding_motion_new_branch_evidence.csv)
and [full movement audit](herding-autonomous-motion-branches.md).
No new compile or runtime execution was performed.

### Original same-frame roaming retry loop — executable reconstruction

The former isolated `0x417FFE..0x4180F1` roaming attempt is now
joined into the original **in-frame loop** at
`0x417F7C..0x418101`, with the exact jump
`0x4180FB -> 0x417FB2` for an outside-polygon result.

The disassembly corrected an important earlier source error:
the native heading origin is
`x + trunc((spriteLeft-spriteRight)/2)`,
`y + trunc((spriteTop-spriteBottom)/2)`, **not** the ordinary
visual sprite center. Each retry copies the latest integer X/Y into
the 0x64-byte record's previous-position fields, recalculates this
signed anchor, consumes another TWO process-global CRT RNG calls,
and translates the **accumulating** float X/Y by the original
three-unit movement. There is no float-position reset between
retries and no runtime cap evident in the retail branch.

`herding_source_roaming_loop.hpp` supplies the resumable
same-frame state machine. Its host-side per-call work budget is
not treated as a retail completion, and gameplay must not advance
or allow another RNG consumer between pending chunks. A new source
regression covers three consecutive attempts and validates the
result against a one-call run and the exact six original RNG draws.

`HerdingEventSimulation` can now independently verify an upstream
motion frame's start/end RNG states, reported retry count, direction,
current X/Y and original previous-position fields by replaying that
original polygon loop. The behavior import was also fixed to retain
source-supplied previous positions rather than fabricating them.
Tracked-follower arrival now has an overload that directly consumes
its original discarded rand() from the shared CRT stream.

**Not yet complete or executed:** the fully autonomous
`OriginalHerdingMotionSource`, entity startup/target selection,
all other animal branches, x87-differential behavior, Win32
rendering/Bink, and native game compilation. New tests remain
source fixtures only; no code was compiled during this pass.

[Exact in-frame movement evidence](../ghidra/herding_motion_new_branch_evidence.csv)
and [Pets Corner autonomous motion audit](herding-autonomous-motion-branches.md).
