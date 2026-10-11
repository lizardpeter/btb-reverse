# High-throughput source reconstruction: 35 original functions

Primary executable: `Bob the Builder - Bob Builds a Park.exe`,
Win32 PE, 311,296 bytes,
SHA-256 `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

This batch covers the original function bodies at these addresses.
The authoritative pseudocode is in
`analysis/ghidra_main_exe/functions/<address>.c`. The C++26 files
implement **host-neutral source-level behavior**; they are not replacements
for ABI-accurate x86 COM or fully reconstructed original modules.

## Graphics, DirectSound and low-level lifetime (18 functions)

- `0x00401F80`: InitializeDisplayForGameWindow
- `0x00402600`: PresentGameFrame
- `0x00402FF0`: ReleaseDisplayResources
- `0x00402FE0`: DisplayManagerDestructor
- `0x004020A0`: DestroyDisplayManager
- `0x00404E10`: BitmapPrinterDestructor
- `0x00404530`: CWaveFile_Destructor
- `0x00409390`: ShutdownDirectSound8
- `0x00403B70`: SetSurfaceTransparencyColorKey
- `0x00403480`: PresentDisplay
- `0x00403BE0`: CSoundManager_Initialize
- `0x00404070`: CSound_ScalarDeletingDestructor
- `0x00404090`: CSound_Destructor
- `0x004042F0`: CSound_GetFreeBuffer
- `0x00406780`: FUN_00406780
- `0x00406440`: MsvcStringSetEnd
- `0x00406110`: FUN_00406110
- `0x00406350`: MsvcStringTidy

## Movie, UI, file and gameplay control (15 functions)

- `0x00409210`: ApplyGlobalBinkVolume
- `0x00408E10`: OpenBinkMovieWithFallback
- `0x00408EB0`: OpenGlobalBinkMovie
- `0x00408D90`: InitializeBinkPlaybackSystem
- `0x00409070`: DecodeAndBlitBinkFrameNoAdvance
- `0x00428010`: PlayActivityMusicByIndex
- `0x004280D0`: CloseWalkthroughMovie
- `0x0042A240`: UpdateSceneSetMovie
- `0x0042A1A0`: UpdateStartupVideoSequence
- `0x00428470`: UpdateGenericHelpHoverVoice
- `0x004169A0`: AnyRectCornerInsideRect
- `0x00406E10`: OpenGameDataFileWithCDFallback
- `0x0041CBE0`: FindMazeShortestPath
- `0x00423220`: BuildShortestSpudMazeGraphPath
- `0x004093C0`: ApplyGlobalGameVolume

## Source game audio planner integration (2 functions)

- `0x00410F70`: PlayInputInterruptibleParkDesignerVoice
- `0x0042CFD0`: PreparePlayAgainTransition

The actual `AudioEffectPlanner` now has Park Designer voice and PlayAgain
transition entry points using the recovered per-slot SoundManager. The
transition emits real device commands; its DirectDraw blit and latch remain
explicit host actions rather than synthetic audio or fake playback.

**Tests:** `btb_retail_system_closure_tests`,
`btb_retail_media_game_closure_tests` and the updated
`btb_full_game_audio_source_tests`, plus the existing project suites.

**Limitations:** native COM and Bink providers must execute the callbacks.
MSVC string reference-counting preserves the old decrement/free policy but
uses memory-safe host spans; invalid pointers and buffer lengths are guarded.
Some original decompilation has unknown registers or hardware failures,
so constants and error-code paths are not asserted as full retail parity.
Function status remains `source_written` until differential tests using
the retail executable are established. Completeness is not inferred from
the number of registry entries.
