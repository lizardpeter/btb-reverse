# Shared original graphics/audio effect bridge — source-only milestone

This work connects the platform-neutral `GameRoot` effect stream to authentic
retail file lookup and the recovered managed-sound policy. It does **not**
replace DirectDraw or DirectSound with a made-up implementation. Compilation
and Windows preview packaging remain intentionally deferred.

## Source files

- `reconstruction/include/btb/full_game_audio.hpp`
- `reconstruction/src/full_game_audio.cpp`
- `reconstruction/include/btb/original_asset_resolver.hpp`
- `reconstruction/src/original_asset_resolver.cpp`
- `reconstruction/include/btb/full_game_resources.hpp`
- `reconstruction/src/full_game_resources.cpp`
- `reconstruction/include/btb/full_game_effect_pipeline.hpp`
- `reconstruction/src/full_game_effect_pipeline.cpp`

Deferred source regression files are in `reconstruction/tests/` with
`full_game_audio_source_test.cpp`,
`original_asset_resolver_source_test.cpp`,
`full_game_resources_source_test.cpp`, and
`full_game_effect_pipeline_source_test.cpp`.

## Actual WAV catalog evidence

The original `Data/sound/binklist.txt` was read directly from the uploaded
retail install data. It has **exactly 1,000 filename + numeric ID records**,
spanning IDs 0 through 999, without repeated IDs. Its filenames are at most
19 bytes long, matching the recovered `0x14` (20-byte) fixed filename
record within `RetailSoundManager32`.

The separate `loaddata/binklist.txt` copy is **not identical**: it has 996
records (IDs 0..995) and omits the four extra end-of-catalog test/demo
entries. Both variants are valid parser inputs; the actual source supplied
by the running retail process must determine which catalog is active.
The reconstruction does not invent absent IDs or extend either catalog.

`SoundCatalog::read` parses CRLF/whitespace, requires one filename and one
bounded numeric ID, rejects malformed/duplicate/oversize entries, and
preserves an already loaded catalog on error. On successful installation,
`AudioEffectPlanner` fills the recovered manager's 20-byte filename table
and resets only its runtime sound-slot state. The manager catalog length is
1100 slots (original retail object capacity), not a claim that the installed
game had 1100 audio files.

## Managed effects and timing

The original 80-slot managed sound policy, not a new priority system, is used:

- `PlayManagedSoundById` handles sound enabled checks, already-playing
  returns, exclusive class-1 blockers, preemptible class-2 requests, priority
  eviction, buffer reuse, and the original unusual double `CSound::Play`
  on first group acquisition.
- `StopAllManagedSounds` marks allocated groups stopped and emits
  `StopRewindManagedSlot` commands. Slots remain cached for replay.
- The `reap_finished_slots` pass runs **before** the next
  `GameRoot` frame's requested audio operations. Observed device status
  drives completion, avoiding the false assumption that a sound is still
  playing just because it was scheduled on a previous frame.
- `StartBackingTrack`, `StopBackingTrack`, and direct machine-pitch sample
  WAVs are mapped to distinct commands, not forced into the managed 80-slot
  voice table.
- Exact commands emitted from `GameRoot::advance` retain source order. The
  eventual platform adapter must execute them in that order and report
  successes, failures and actual buffer-playing status. Current
  `next_managed_load_succeeds` is a planning input for that device result,
  **not evidence that a DirectSound buffer was created**.

The current planner emits `SoundEffect` operation descriptors; **actual
hardware sound playback remains outstanding**. In particular, direct
sample group lifetimes, per-slot input-interruptible latches and failure
recovery must be reconciled with live DirectSound behavior before signoff.

## Original asset resolution

The production game uses paths like `Data\\SubGameOpen\\music_01.bmp`,
`data\\sound\\AS_BOB_01.wav`, and
`loaddata\\uiHotAreaReplace.txt`, with inconsistent letter casing.
Windows native lookup ignores case; Linux does not.

`OriginalAssetResolver` performs component-wise ASCII case-folded matching
inside the configured installed game root. A missing installed file may
fall back to the configured CD/disc root, consistent with the original
source data open strategy. No alternate assets are manufactured.

It also rejects absolute paths, drive-qualified paths and `..` traversal;
refuses symlink traversal outside the roots; and reports ambiguous
case-folded directory matches instead of choosing arbitrarily.

The `resolve_game_frame_assets` adapter validates every original
`GameRoot` draw asset and every filename-bearing sound operation, while
forwarding the exact `Draw::color_keyed` flag and source crop rectangles.
This is **resource preparation, not a bitmap renderer**.

## Remaining integration

The final full-game platform host must still:

1. Consume `ResolvedDraw` in order using the authentic color-keyed
   DirectDraw/bitmap compositor, registered surfaces, clipping and
   lost-surface reload.
2. Consume `ResolvedSoundEffect` using actual DirectSound WAV buffers and
   report per-slot playing states, load/rewind errors, and completion.
3. Supply the game-root high-resolution elapsed time, keyboard/mouse
   input pulses and shared frame-delta values.
4. Bind the remaining nine native activity adapters and the other eleven
   front-end UI screens, including their original asset lifetimes.
5. Compare against original executable captures for visual, audio,
   interaction and persistence fidelity.

No assertion of a complete game is made on the basis of this source bridge;
the original full-game completion gate remains in
[full-game-integration.md](full-game-integration.md).
