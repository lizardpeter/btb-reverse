# Enhanced variant

Opt in with `-DBTB_BUILD_ENHANCED=ON` and build `btb_game_enhanced`.
The default `btb_game` target never receives enhanced sources or definitions.
This is a baseline-only executable at present: it uses the same faithful core
and platform implementation. No widescreen, renderer, timing or audio change
has been introduced by the directory migration.

Future enhancements must be separate implementations/overrides linked only to
the enhanced target. Shared interfaces may be extended in the faithful layer
only if existing behavior remains unchanged and regression-tested. Keep
proven original behavior and parity corrections in the faithful source.
