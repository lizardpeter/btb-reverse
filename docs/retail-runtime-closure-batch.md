# Bob Builds a Park — native runtime closure, batch 3

Verified source identity: 311296-byte PE32 original executable
SHA-256 `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
Ghidra pseudocode for each function remains in `analysis/ghidra_main_exe/functions`.

## Added original-function source models

- `0x00411010` — PlaceFireworkGridItem
- `0x00412B80` — DeleteFireworkAtSelectedRegion
- `0x0041F430` — RemoveGrandOpeningEventAtCell
- `0x00403540` — ClearBackBuffer
- `0x00403500` — BltFastToBackBuffer
- `0x00402620` — RestoreAllDirectDrawSurfaces
- `0x00403590` — UpdateDisplayDestinationRect
- `0x00414A30` — VectorFromDegreesAndMagnitude
- `0x0041AE90` — FloatAbs
- `0x00401000` — PlayHelpVoice
- `0x00402F60` — ReapFinishedSounds

The native fireworks placement logic includes 6-column row crossing and
continuation type marker +14. Grand Opening event deletion scans backward from
continuation markers greater than 9, then clears the recovered source length.
Selected fireworks deletion requires original action 12 and a primary type ID.

The DirectDraw functions now explicitly model null render surfaces as
`E_POINTER`, original 100-byte DDBLTFX color fill, BltFast flags/HRESULT,
the success-to-zero result of RestoreAllSurfaces, and the ClientToScreen
sequence when updating the windowed destination rectangle.

The **actual** `AudioEffectPlanner` now provides source-backed
`plan_help_voice(sound_id, stop_existing, observed)` implementing the
original StopAllManagedSounds-before-PlayManagedSoundById ordering with
priority 50 and class 2. Existing runtime `reap` implements
ReapFinishedSounds with real observations, including the no-premature-stop
case. New tests exercise both paths, not just standalone wrappers.

Source models use hardware callbacks rather than generated fake audio,
video, or surfaces. Host validation guards and `long double` trigonometry
are intentionally NOT claimed to be exact retail x87 / x86 ABI parity.
The 11 additions are tracked as `source_written`, not `compiles` or
`behavior_verified`, until independent native trace comparison is available.
