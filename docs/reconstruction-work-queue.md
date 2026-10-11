# Whole-program C++26 reconstruction inventory

The original Bob Builds a Park main PE32 executable has SHA-256
\`c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05\`.
Ghidra 11.4.2 produced pseudocode for **615 discovered functions** with **1,846 direct call relations** (1,550 internal and 296 calls to 163 external imported API symbols).

This repository retains the **complete generated pseudocode** at
\`analysis/ghidra_main_exe/functions/<address>.c\`. It is **not** original C++,
**not** behaviorally verified, and **not** automatically compilable. More
executable functions may remain unidentified by Ghidra.

## Working with all 615 identities

The Python 3.12+ generator, \`tools/build_reconstruction_index.py\`, reads
the checked-in Ghidra files and produces:

| Output | Purpose |
|---|---|
| \`analysis/reconstruction/function_status.csv\` | Every original address, current review stage, module, exact pseudocode link, dependency counts, test and evidence paths |
| \`analysis/reconstruction/work_queue.csv\` | Ranked tasks, dependency-first with stable address ties; priority is a heuristic, not completion evidence |
| \`analysis/reconstruction/dependency_groups.json\` | Tarjan strongly connected components and external dependency groups; functions in cycles should be reconstructed as one group |
| \`analysis/reconstruction/summary.json\` | Machine-verifiable coverage counts, subsystem sizes and unresolved status |
| \`reconstruction/include/btb/original_function_registry.hpp\` | Compilable C++26, constexpr table of all 615 original addresses and names, binary-search lookup; **does not implement any original function** |

The subsystem boundaries come from \`docs/module-map.md\` and are **approximate**.
They provide scheduling hints, not proof that every address belongs to the
suggested activity or that unrelated code is independent.

## Reconstruction state: evidence only, never inferred

Do **not** edit the generated outputs. Edit the sparse manual overlay
\`analysis/reconstruction/reviewed_functions.csv\`, one row per **proven**
reconstruction:

\`\`\`csv
address,stage,cpp_source,validation_test,evidence,notes
\`\`\`

The allowed stages are:

- \`unreviewed\`: pseudocode imported; a function has **not** been audited,
  even if an existing C++ module looks semantically similar.
- \`under_analysis\`: active analysis, no source-level closure asserted.
- \`source_written\`: evidence-backed C++ exists and source path is recorded.
- \`compiles\`: reconstructed source linked to a test that has been compiled
  and executed; this is not retail behavioral parity.
- \`behavior_verified\`: original executable evidence and specific validation
  support the behavior claim.

The generator refuses to claim a written source without a real C++ file and
evidence. The latter two stages also require an actual test file. Files and
tests alone do not promote status. Stronger claims should include reference
trace and test run URLs in the evidence/notes fields.

All 615 function rows start **unreviewed** until examined explicitly. The
existing C++ modules in \`reconstruction/src\` remain intact; the queue does not
erase or discount that substantial work. They need to be mapped to original
function addresses and independently audited before making per-function
coverage claims.

## Regenerate, test, and compile

\`\`\`sh
python3 tools/build_reconstruction_index.py
python3 tools/build_reconstruction_index.py --check
python3 -m unittest discover -s tools/tests -p 'test_*.py'
cmake -S reconstruction -B build/reconstruction
cmake --build build/reconstruction --target btb_original_function_registry_tests
ctest --test-dir build/reconstruction -R '^original_function_registry$' --output-on-failure
\`\`\`

The standalone registry target compiles quickly without building the entire
preview game. Full game/module compilation and tests continue under the
original \`repo-checks\` workflow. The registry CI generates, verifies, builds,
runs tests, and commits generated outputs so they cannot silently drift.

## Order-of-work policy

Start with shared globals/structures and dependency groups used by multiple
activities; validate calling conventions and state mutation before
translating every function. Implement strongly connected groups together,
preserve exact externally observed behavior, and continuously test small
source units. Compile checks prove only that **those units** compile. A
615-entry registry is *not* a 615-function recompilation.
