# Four original gameplay / UI coordinators

Ghidra PE32 original SHA-256:
`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

This batch adds the actual C++26 source and linked executable tests for:

| Address | Source-level reconstruction |
|---|---|
| `004227F0` | Spud Maze scene. Source's 5 backgrounds, active repair sprite animation and onscreen markers, original Pilchard and Bob sprite clip transitions, raising hammer, sound dispatch, border, Windows FILETIME timer and 20px hammer markers. |
| `00424690` | Spud Skate Bink movie presentation. Scores as tens and ones at guest UI positions, original Bink per-channel decode/advance/wait and seek at end, voice tables and event gates, control selection, completion Bink and certificate transition. |
| `004267B0` | Squirrel draw scene: source background and foreground layers, overlay sequence, separate squirrel frame passes, voice ID completion, scrolled sprites and clipped actors, delegated original run-assembly update, borders. |
| `00401700` | Contextual Help: original menu/dino/maze/herding hotspot branches, location and progress gating, suppress repeated speech, context voices and click-to-leave behavior. |

All four use *required* typed IO providers. The original Bink/DirectDraw
decoder, mouse/event sources, voice engine, Windows FILETIME and legacy
asset-table resolution are external platform dependencies, not substituted
by no-ops. The adapter contract lets the original game driver connect
actual services without embedding guessed game assets.

### Limitations

The original PE32 pseudocode still contains unresolved register and
stack artifacts in Bink and sprite paths. Some table indices, pointer-
animation frames, and priority branches are reconstructed as host-safe
source models rather than instruction-for-instruction ports. Runtime Bink
frame number, mouse hover arbitration, sprite clipping and game-global
struct layouts need original executable differential comparisons. The
entry-point calls are not yet admitted into the native live game drivers.

All four source bodies passed Linux C++26, Python and Windows x86/x64
integrated game/provider CI and are now marked `compiles`. Full
retail behavior certification remains **0/615** and original-executable
comparison count added in this patch is **0**.

## CI admission — October 15, 2026

All four jobs in [Actions run 38783605153](https://github.com/lizardpeter/btb-reverse/actions/runs/38783605153)
passed: Linux C++26 core and **147/147 CTest tests**, Python tooling,
Windows x64 game/provider compilation and regressions, and Windows x86
native game/COM/Bink-provider builds plus smoke/regression checks.

The compiled-source ledger is now **591/615 (96.1%)**, with **24**
remaining. This does not imply original-executable equivalence:
new direct original-binary comparisons **0** and complete retail behavior
certifications **0/615**. The backend hooks have not yet been admitted
to the running gameplay host.
