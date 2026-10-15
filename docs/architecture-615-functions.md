# Unified architecture inventory: all 615 original functions

Original executable SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

Each original function belongs to exactly one evidence cohort:

- 191 functions formed the initial source-written baseline.
- 200 functions were originally architecture-only in the first pass.
- 224 functions were originally architecture-only in the final pass.

Current status: 603 source-written or compiled functions (600 at compile stage) and 12 architecture-only functions.

The full inventory keeps these statuses separate; no source-written
function was promoted to behavior-verified by architecture analysis.

Important global evidence: the original Ghidra DAT-address tokens are
cross-linked without assuming they are class members, types, or writes.
Original callgraph relationships include only direct observed targets.

See analysis/reconstruction/architecture_615.csv and
analysis/reconstruction/architecture_615_globals.json for all records.
Next step: actual C++ class and field-layout recovery with cross-game
behavior integration and original-executable differential validation.
