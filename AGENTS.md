# Repository instructions

- Preserve original behavior in `src/` and `include/btb/`. Keep original-machine evidence and function-address provenance. Never use enhancement work to inflate faithful coverage.
- Put Windows device bindings, startup and executable entrypoints in `runtime/`. Correct native compatibility defects without changing recovered gameplay semantics.
- Put optional user-facing changes in `enhanced/` and link them exclusively into `btb_game_enhanced`. Do not replace or conditionally compile the faithful core with enhanced flags.
- Maintain the reviewed function ledger at `records/reviewed_functions.csv`; regenerate derived records and metadata using the tools in `tools/`. Historical evidence in `analysis/` and `ghidra/` is immutable except for documented corrections.
- Configure the canonical build with `cmake -S . -B build/faithful`. Legacy `cmake -S reconstruction` includes the same graph. Use `-DBTB_BUILD_ENHANCED=ON` for the separate enhanced executable.
- Run source regressions and native Windows linkage checks; never claim live interactive parity from compilation or headless tests alone.
- Do not commit original retail game data, original binaries, or extracted retail assets.
