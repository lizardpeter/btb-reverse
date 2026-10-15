# Three gameplay loops recovered from the PE32 original

Original executable SHA-256: `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

| Address | Source-level reconstruction |
|---|---|
| `00413450` | Fireworks show playback: original 400-centisecond columns, crowd phases 0–3/7, 40 active Bink events, row gates, random crowd voices, original movie restart order and completion to state 16. Linked to previously verified `fireworks::RetailShowRuntime` and `Sequence` rather than copied scheduling tables. |
| `00423630` | Spud Maze Pilchard: original states 0–7, pause/initialization, pursuit-route traversal, graph neighbor selection excluding reverse edge, collision interrupt, wander and return route, frame counter and sound selection. Typed graph/path and movement providers required. |
| `004273F0` | Squirrel placement: original states 0–8, strict pointer hit rectangles, return/movement sequence, placement and retrieval, seven positive feedback voices, relative-count response voices, game completion. Typed movement and audio providers required. |

All three are out-of-line C++26 compiled source models linked into the existing
`btb_recompiled_core`, and covered by the `original_gameplay3` executable
regression. No source stub is counted as a finished gameplay path.

These are recovered gameplay-state coordination routines, **not** complete
native device integrations or certified original-executable parity. The
Fireworks IO still requires real Bink/SoundManager/DirectDraw providers.
The existing `full_game` activity drivers have not been switched to the
new adapters. Each source reconstructs observable control flow with
host-safe bounded indexes where Ghidra's original register/stack recovery
is ambiguous; unknown game-global semantics and exact original binary
behavior remain separate validation tasks.

These three source bodies are now **compiles** after CI validation (577/615,
38 remaining). No original executable differential comparisons were added.

## Validation

The corrected source branch passed all four jobs in
[CI run 38765856072](https://github.com/lizardpeter/btb-reverse/actions/runs/38765856072):
Linux C++26 build and **142/142 CTest tests**, Python checks, Windows
x64 native game/provider compilation and tests, and Windows x86 game/provider
compilation and regression tests.

This is source-level coverage: original x86 ABI and platform behavior
comparisons added in this batch: **0**. The original retail executable
does not execute in these tests. Full retail-equivalence certification
remains **0/615**. Native driver admission remains to be integrated.
