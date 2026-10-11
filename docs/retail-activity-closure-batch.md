# Second batch: 10 more original game functions

Original executable checksum:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
These functions join the 35-function batch in
`docs/batch-35-original-functions.md`.

- `0x004048F0`: CWaveFile_ResetFile
- `0x00404270`: CSound_RestoreBuffer
- `0x00404360`: CSound_Play
- `0x0041FB90`: HitTestBobsBandRegions
- `0x0041AEB0`: DebounceMazeDirectionalInput
- `0x00427320`: UpdateSquirrelConveyorSlideCycle
- `0x00408460`: FUN_00408460
- `0x00424C80`: SeekAndDecodeBinkToExactFrame
- `0x0040D7D0`: DrawParkDesignerSegmentedPondSurface
- `0x00402B00`: SoundManagerConstructor

The new source file `reconstruction/include/btb/retail_activity_closure.hpp`
covers WinMM wave reset, DirectSound lost-buffer recovery and voice
playback, Bobs Band hit regions, maze input debouncing, squirrel conveyor,
native keyboard translation, exact Bink frame seek/iteration, and
Park Designer pond surface segments.

`SoundManagerConstructor` is already represented by the real
`AudioEffectPlanner` constructor and `SoundCatalog` loader. This mapping
is based on the existing tests for catalog import, 80-slot initialization,
and persisted per-sound filename records.

The new `btb_retail_activity_closure_tests` suite exercises these
state transitions and the exact order of callback invocations.
The imported game binary cannot be executed inside this suite, and
the original Win32/COM/Bink object pointers are not recreated.
Ghidra produced unrecovered native register variables in parts of
`CSound_RestoreBuffer` and `SeekAndDecodeBinkToExactFrame`;
required provider inputs are explicit C++ arguments rather than
invented constants.

Review state remains `source_written` pending native differential
testing against the original game executable.
