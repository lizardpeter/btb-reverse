# Final 224 original-function architecture mappings

**All 224 are original functions in the statically linked runtime region**.
These map source evidence and dependency relationships only.
No new C++ gameplay implementations are claimed by this batch.

## Scope and provenance

Original executable SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05

- Original Ghidra pseudocode text is referenced for every address.
- Original direct caller and callee edges include imported names.
- Original DAT global references and their cross-module sharing are indexed.
- Candidate class/module owners and the original call prototype are labeled tentative.
- Original pseudocode SHA-256 hashes are recorded to detect corpus drift.
- The original 191 source mappings remain preserved; newly reconstructed C++ models are tracked separately.

## Runtime categories (name-derived, not guaranteed compiler provenance)

| Candidate kind | Functions |
|---|---:|
| crt_or_compiler_helper_candidate | 19 |
| statically_linked_runtime_unknown | 205 |

## Reproducible artifacts

- analysis/reconstruction/architecture_224_selection.csv — frozen final 224 addresses.
- analysis/reconstruction/architecture_224.csv — detailed 224-function report.
- analysis/reconstruction/architecture_224_summary.json — 224-function audit.
- analysis/reconstruction/architecture_615.csv — all 615 original functions reconciled.
- analysis/reconstruction/architecture_615_summary.json — disjoint cohort / fidelity totals.
- analysis/reconstruction/architecture_615_globals.json — full original global-reference census.
- reconstruction/include/btb/original_architecture_615.hpp — constexpr metadata only.

## Reconstruction boundary

615/615 architecture-mapped does not mean 615/615 rewritten in C++.
Current source-written or compiled functions: 413; remaining: 202.
Frozen architecture cohorts are independent of reconstruction status.
Recover runtime layouts, identify import thunks,
and integrate original gameplay functions before claiming fidelity.
