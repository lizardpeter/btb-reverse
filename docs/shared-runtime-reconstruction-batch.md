# Original Bob Builds a Park: 24 shared-runtime functions

Recovered from the SHA-256-pinned 311,296-byte main executable, using
`analysis/ghidra_main_exe/functions/*.c` and existing exact 32-bit C++ layouts.
The code in `reconstruction/include/btb/retail_shared_runtime.hpp` models
retail observable state transitions or COM/Bink call ordering in C++26.

## Reconstructed routines

- `0x00407AF0` — SetCursorSurface
- `0x00407960` — BeginLoadingCursorAnimation
- `0x00407E10` — UpdateDirectInputAcquireState
- `0x004280A0` — StopActivityMusic
- `0x00402CC0` — StopSoundSlot
- `0x00402BC0` — ReleaseSoundSlot
- `0x00410FD0` — CanPlaceFireworkGridItem
- `0x00411080` — RemoveFireworkGridItem
- `0x0041F3A0` — CanPlaceGrandOpeningEvent
- `0x0040D420` — HitTestParkDesignerToolbar
- `0x0040DC90` — ClearPondPrimaryObjectsBySelector
- `0x0040DCE0` — ClearBandstandObjectsByCategorySelector
- `0x0040D780` — CompareParkDesignerObjectsBySortLayer
- `0x00416310` — CompareHerdingEntitiesByDepth
- `0x00425980` — BuildSquirrelFrameRect
- `0x00409260` — PauseGlobalBinkMovie
- `0x004092A0` — ResumeGlobalBinkMovie
- `0x00408F50` — CloseGlobalBinkMovie
- `0x00402FC0` — DisplayManagerConstructor
- `0x00404510` — CWaveFile_Constructor
- `0x00404DD0` — BitmapPrinterConstructor
- `0x00403BB0` — CSoundManager_Constructor
- `0x00403BC0` — CSoundManager_Destructor
- `0x004043F0` — CSound_SetVolume

## What this does and does not certify

- Each original Ghidra address maps to a real, tested C++ source model.
- The new `btb_retail_shared_runtime_tests` executable checks core behavior,
  state mutation, order of audio/Bink callbacks, and boundary conditions.
- Hardware calls are supplied as callbacks; this suite does not generate
  audio, render graphics or pretend to run the complete original WinMain.
- C++ integration and compiler checks are NOT original-executable differential
  behavior verification. Source fidelity remains staged `source_written`.
- Defensive bounds checks against damaged metadata are host-safety
  divergences, not claims about what shipped x86 code would do.
- The original `PointInPolygon`, fullscreen rendering, and game coordinator
  remain separately implemented and tested. No original game behavior is
  replaced by fallback graphics or fake assets in this batch.

The important recovered quirks include row-crossing fireworks placement,
different row-boundary behavior in Grand Opening, non-strict sorting
comparators, correct 80-slot sound cleanup, and the exact Bink
pause/volume/input-gate sequencing.
