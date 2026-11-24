# Faithful source and enhanced boundary

The top-level source layout follows the BO2, BO3 and Ghosts projects.
This migration relocates 216 portable/recovered production translation units
to `src/` and 25 native host/platform units to `runtime/`. Headers and tests
are relocated as unchanged Git blobs. It does not change original gameplay,
media decoding, rendering, or input logic. Current function stages remain
unchanged: compile coverage is not behavior certification.

The canonical root CMake project includes `cmake/BTBTargets.cmake`.
The old `reconstruction/CMakeLists.txt` includes the same file, so old
build commands retain their output paths and do not compile duplicated
source. Native diagnostic C++ entrypoints live in `tools/native/`.
The existing uncompiled-production-source configuration guard covers both
`src/` and `runtime/`.

The default `btb_game` links faithful code only. With
`BTB_BUILD_ENHANCED=ON`, `btb_game_enhanced` is a separate baseline
executable and linked-core smoke target. No optional improvement has been
implemented yet. Future improvements are explicit enhanced-only modules
and overrides; changes restoring original behavior remain faithful.

Live function records move to `records/`, while original Ghidra evidence,
validation observations and architecture reports remain in `analysis/`.
Older evidence may still cite pre-migration `reconstruction/src/`,
`reconstruction/include/`, or `reconstruction/tests/` paths. For current
file locations, see the moved source and ledger.

Original installed data is still required for gameplay. The migration is
not a claim that every activity is graphically verified on Windows.
