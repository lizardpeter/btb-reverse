# Reconstructed original-function batch: 71 identities

Original executable SHA-256
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.
All Ghidra pseudocode originals remain checked into
`analysis/ghidra_main_exe/functions` and in `uregraph`.

## 15 original game/system routines

- `0x00402640` FullscreenSystemKeyHook
- `0x00403830` ReloadRegisteredBitmapSurfaces
- `0x00407920` LoadGlobalDataTables
- `0x00409CD0` UpdateDinoPrintButton
- `0x0040D870` DrawParkDesignerPondSurfaceSegment
- `0x0040E3E0` FinishParkDesignerObjectDrag
- `0x0041F4B0` PlaceGrandOpeningEvent
- `0x00422760` MaybePlayRandomSpudVoice
- `0x00427270` StepSquirrelPlacementActorTowardTarget
- `0x00428120` UpdateWalkthroughMovie
- `0x00428E30` DrawGenericUIScreen
- `0x00407770` LoadVideoSequence
- `0x00415CB0` LoadHerdingCoordinateData
- `0x00407390` LoadOptionsConfig
- `0x00407430` LoadPlayAgainOverlayData

These now have C++26 functions for system-key handling, registered bitmap
lifecycle, ordered global loaders, Dino print hover/click, pond geometry,
Park Designer drag finishing, Grand Opening placement, Spud voice gating,
squirrel movement, walkthrough, UI drawing, video list loading,
herding coordinate loading, and two UI config record readers.

## 17 CRT/import routines

- `0x0042FFBA` RetailSrand
- `0x0042FFC4` RetailRand
- `0x0042FBDC` OperatorDeleteProxy
- `0x0042FBCE` RetailOperatorNew
- `0x00430D93` RetailMalloc
- `0x00430DA5` RetailNhMalloc
- `0x00430A1F` RetailFopenInternal
- `0x00430A3F` RetailFopen
- `0x0042FBB8` RetailFscanf
- `0x0042FF99` NLGNotify1
- `0x0042FFA2` NLGNotifyFromStack
- `0x00430441` RetailExceptionDestructor
- `0x00430464` RetailTypeInfoDestructor
- `0x00430CFE` RetailSwapSourceBytes
- `0x00430F2F` RetailItoa
- `0x00406600` RetailScalarDeletingException
- `0x0042FB5C` PrintDlgAThunk

Includes exact MSVC `rand` LCG `0x343FD/0x269EC3`, `srand`,
operator-new/free forwarding, `__nh_malloc` retries, `fopen` and
`fscanf` forwarding, NLG global state, exception/type-info cleanup,
source-byte swapping, signed decimal `itoa`, and imported WinAPI thunking.

## 39 MSVC EH unwind landing pads

- `0x0043A360` Unwind_0043A360
- `0x0043A380` Unwind_0043A380
- `0x0043A38E` Unwind_0043A38E
- `0x0043A3B0` Unwind_0043A3B0
- `0x0043A3B8` Unwind_0043A3B8
- `0x0043A3D0` Unwind_0043A3D0
- `0x0043A3D8` Unwind_0043A3D8
- `0x0043A3E0` Unwind_0043A3E0
- `0x0043A3E8` Unwind_0043A3E8
- `0x0043A400` Unwind_0043A400
- `0x0043A408` Unwind_0043A408
- `0x0043A410` Unwind_0043A410
- `0x0043A418` Unwind_0043A418
- `0x0043A430` Unwind_0043A430
- `0x0043A438` Unwind_0043A438
- `0x0043A450` Unwind_0043A450
- `0x0043A45B` Unwind_0043A45B
- `0x0043A466` Unwind_0043A466
- `0x0043A471` Unwind_0043A471
- `0x0043A47C` Unwind_0043A47C
- `0x0043A487` Unwind_0043A487
- `0x0043A492` Unwind_0043A492
- `0x0043A49D` Unwind_0043A49D
- `0x0043A4C0` Unwind_0043A4C0
- `0x0043A4C8` Unwind_0043A4C8
- `0x0043A4D0` Unwind_0043A4D0
- `0x0043A4D8` Unwind_0043A4D8
- `0x0043A4F0` Unwind_0043A4F0
- `0x0043A4FB` Unwind_0043A4FB
- `0x0043A510` Unwind_0043A510
- `0x0043A540` Unwind_0043A540
- `0x0043A560` Unwind_0043A560
- `0x0043A578` Unwind_0043A578
- `0x0043A58C` Unwind_0043A58C
- `0x0043A5A0` Unwind_0043A5A0
- `0x0043A5B4` Unwind_0043A5B4
- `0x0043A5C8` Unwind_0043A5C8
- `0x0043A5DC` Unwind_0043A5DC
- `0x0043A5F0` Unwind_0043A5F0

These are **compiler-generated exception cleanup stubs**, not original
gameplay logic. Each address gets an individual C++26 entrypoint that
dispatches its recovered cleanup function against an explicit reconstructed
EBP frame and offset. Pointer-indirect and in-place destructors are
distinguished. Host callbacks must provide the real C++ string, exception,
allocator, and printer destructors. Compiled tests check all 39 distinct
target addresses, stack offsets, and destructor kinds.

## Verification limits

The source models are not x86 binary-equivalent, do not duplicate Win32
calling conventions, and cannot prove the real retail game is fully
recompiled. The Ghidra bodies do not conclusively recover all globals,
registers, binary resources, or hardware outcomes. Source-level tests
verify observable policies with fake providers, and any host bounds guards
are intentional divergences. All 71 source mappings remain at the
`source_written` stage, not `behavior_verified`.

Do not confuse **71 functions mapped to source** with 71 new gameplay
functions: only **15** are game/system routines; 39 are EH cleanup
landing pads and 17 are CRT/import functions.
