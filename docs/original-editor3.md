# Original Fireworks and Squirrel editor functions

Three larger gameplay bodies are reconstructed as out-of-line C++ and linked into the game core:

| Address | Function | Original body bytes |
| --- | --- | --- |
| 004126F0 | HitTestFireworksEditorRegions | 1,117 |
| 004259D0 | SelectSquirrelPlacementMotionProfile | 1,379 |
| 00426F30 | GenerateSquirrelConveyorChoices | 819 |

The Fireworks function preserves ordered region scanning, Bob/Wendy actor gates, the different grid/palette pointer offsets, delete-mode returns, picking up a placed firework, grid removal, cursor requests, palette selections, and every sound/random branch. It preserves the unusual mode-1 pointer behavior: add ten before scanning, subtract twenty only on the non-returning path. Sound requests reload the actor globals afterwards. Cursor/sound operations require explicit services; no default successful service exists.

The Squirrel selector reconstructs the complete 147-word stack table, section-to-placed-slot decrement sequence, transposed piece indices, all selection branches, four motion globals and timing resets. The conveyor generator uses the already compiled original MSVC rand body and its shared seed. It preserves correct/decoy pool creation, rejection loops, the seven outer permutations, random pool interleaving and unused-slot values. The original decoy pair's first field is the combined inner piece ID, rather than a normalized connector category. That detail is retained and independently checked.

`begin_original_run_assembly` connects the recovered selector to the existing loaded-motion animation consumer. This does not claim that the complete Squirrel activity initializer/update/render path is installed in the native host. The Fireworks function is compiled and linked to executable validation and an explicit service interface; wiring it into the provisional native Fireworks driver still requires the original pointer/cursor and release-flow integration. The existing driver's simplified click loop is not silently replaced with incomplete glue.

`tools/build_retail_editor_vectors.py` executes the original SHA-verified PE32 routines in Unicorn. The Squirrel routines execute their real original rand calls. The Fireworks run controls the existing cursor and managed-sound dependencies and records their requests. One thousand Fireworks cases compare the return, all relevant state fields, grid, final RNG seed, and ordered cursor/sound traces; two thousand Squirrel cases compare both routines' complete input/output state and final RNG seed. They cover all retail run sections, levels, difficulties, connector stages, the legacy unspecified-variant branch, and randomized piece/connector plans. These 3,000 checks establish bounded control/data behavior, not real rendering/audio, complete ABI or universal retail certification.

The native API explicitly bounds Squirrel levels to the original three, connector values to the original range, and motion lookups to the reconstructed table. Invalid source data raises host exceptions instead of performing arbitrary memory accesses. The exact instruction listings and original output vectors are committed; the executable remains outside Git.

The full Clang 19 C++26 build and 127 CTest tests pass, as do the 16 Python tool tests. The editor regression also passes native ASan/UBSan checks (leak detection disabled for restricted `/proc`). Windows x86 and x64 CI include the editor target. The preceding Windows build failure was fixed with explicit standard-library includes in commit 761264e, and that commit's entire CI run passed.

Compiled-source coverage is 444/615; 171 functions remain. Native gameplay integration and full retail-equivalent recompilation are still incomplete. No function is promoted to `behavior_verified`.
