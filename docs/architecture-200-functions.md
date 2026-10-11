# Architecture map: 200 additional original BTB functions

This is **architecture mapping**, NOT 200 recompiled C++ functions.
All 181 previously unmapped game/platform functions and 19 referenced CRT
functions were selected. Their original Ghidra text and direct-call
relationships are indexed, but class names are hypotheses.

Original EXE SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

## Files

- analysis/reconstruction/architecture_200_selection.csv: frozen 200 addresses
- analysis/reconstruction/architecture_200.csv: prototype, module, tentative C++ owner, original global/asset references, callers, callees and uncertainty
- analysis/reconstruction/architecture_global_xrefs.json: original DAT-address sharing across functions, untyped
- analysis/reconstruction/architecture_200_summary.json: audited totals
- reconstruction/include/btb/original_architecture_map.hpp: C++26 METADATA ONLY

## Candidate module distribution

| Candidate original module | Count |
|---|---:|
| shared_platform | 64 |
| static_runtime | 19 |
| park_designer | 17 |
| spud_maze | 16 |
| fireworks | 14 |
| maze | 12 |
| squirrel | 11 |
| golf | 10 |
| shared_activity | 10 |
| bobs_band | 7 |
| profiles_and_ui | 6 |
| herding | 5 |
| spud_skate | 5 |
| dinosaur | 3 |
| game_flow | 1 |

## Highest-call-impact original functions still requiring C++ reconstruction

- 0x00402cf0 PlayManagedSoundById — 47 direct callers, candidate SoundSubsystem
- 0x0042fb62 crt_fclose — 36 direct callers, candidate MsvcRuntimeModule
- 0x004304d0 __ftol — 27 direct callers, candidate MsvcRuntimeModule
- 0x004038d0 LoadBitmapToDirectDrawSurface — 26 direct callers, candidate BitmapSurfaceSubsystem
- 0x00430d2a FUN_00430d2a — 15 direct callers, candidate MsvcRuntimeModule
- 0x00415e50 BlitColorKeyedSurfaceClipped — 13 direct callers, candidate GolfActivityModule
- 0x00433720 _strlen — 13 direct callers, candidate MsvcRuntimeModule
- 0x00430495 __CxxThrowException@8 — 13 direct callers, candidate MsvcRuntimeModule
- 0x00406150 MsvcStringAssignSubstring — 9 direct callers, candidate MsvcStringSubsystem
- 0x00406460 MsvcStringGrow — 9 direct callers, candidate MsvcStringSubsystem
- 0x00415d70 AngleBetweenIntegerPointsDegrees — 9 direct callers, candidate GolfActivityModule
- 0x004365c0 FUN_004365c0 — 9 direct callers, candidate MsvcRuntimeModule
- 0x00433630 FUN_00433630 — 9 direct callers, candidate MsvcRuntimeModule
- 0x004303ba FUN_004303ba — 8 direct callers, candidate MsvcRuntimeModule
- 0x00403d20 CSoundManager_Create — 7 direct callers, candidate CSoundManager
- 0x00430937 FUN_00430937 — 7 direct callers, candidate MsvcRuntimeModule
- 0x00436540 _memset — 7 direct callers, candidate MsvcRuntimeModule
- 0x00430144 crt_sprintf — 7 direct callers, candidate MsvcRuntimeModule
- 0x0043a228 FUN_0043a228 — 7 direct callers, candidate MsvcRuntimeModule
- 0x00430500 FUN_00430500 — 6 direct callers, candidate MsvcRuntimeModule
- 0x0043277a FUN_0043277a — 6 direct callers, candidate MsvcRuntimeModule
- 0x00430f00 FUN_00430f00 — 6 direct callers, candidate MsvcRuntimeModule
- 0x004063a0 MsvcStringAssignBuffer — 5 direct callers, candidate MsvcStringSubsystem
- 0x004372d7 FUN_004372d7 — 5 direct callers, candidate MsvcRuntimeModule
- 0x00432662 FUN_00432662 — 5 direct callers, candidate MsvcRuntimeModule

## Fidelity limitations

- Ghidra C-style prototypes are not the original C++ declarations.
- Module ownership based on address and names is tentative.
- This does not invent receiver/class layouts.
- Original DAT addresses are lexical references, not proven data types or read/write fields.
- Indirect calls and callback targets are not recovered from the direct-call graph.
- All previous source-written and verified stages are preserved unchanged.
- No additional original functions were converted to executable C++ by this mapping.

Next: resolve struct layouts, classify global writes, bind actual class
receivers and convert dependencies into integrated source files.
