# Maze and Spud Maze original initialization source models

PE32 executable SHA-256: `c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05`.

Two newly authored original-address C++26 entrypoint functions:

- `0041A730` — initializes the three-screen Maze. Installs the
  activity-unload callback, consumes parsed graph data via the existing
  `maze::Data` importer contract, recreates the original 20-field actor
  records and random distinct spawn selection for easy/medium/hard groups,
  loads the three background surfaces and seven keyable sprite surfaces
  into the original guest-address slots, creates the engine sound,
  applies original `(volume-100)*0x2a`, and seeds the game-time base.
- `004207E0` — initializes the five-screen Spud Maze, 5 original
  background slots, 10 named sprites, 32 repair-spot sprite slots
  beginning at `0x005141ec`, and difficulty-derived seeded repair
  damage through the already-source-recovered
  `spud_maze::select_repair_damage`, plus the five-group actor
  start table and clock/init flags.

Actual asset and sound operations are **mandatory provider calls**,
not no-op implementations. The original screen asset filename ranges
and repair sprite names are supplied by real importer/provider metadata
rather than guessed string synthesis; game driver admission is still
required. The named sprite paths in the new source are hypotheses from
the Ghidra symbol names and need confirmation against the source files.
Likewise, the five-group Spud NPC *population pattern* is a source
model requiring exact differential checks; no claim of complete
original opcode parity is made.

The out-of-line bodies are linked into `btb_recompiled_core`, with
regressions for all three difficulty levels, surface count and color
keys, seeded spawn constraints, repair counts, clock setup,
invalid difficulty, missing surfaces and inconsistent repair-data
census. No original executable binary comparisons or native gameplay
provider equivalence were performed in this batch.

After passing the CI gate, both entries are marked `compiles`, with
original executable parity and host device admission still unverified.

## Validated admission, October 15, 2026

[GitHub Actions run 38771945489](https://github.com/lizardpeter/btb-reverse/actions/runs/38771945489)
passed all four jobs: Linux C++26 build and **144/144 CTests**, Python
tool checks, Windows x64 native game/provider build and tests, and Windows
x86 native game/COM-provider build and regression.

Both source bodies are now `compiles`: 581/615 total and 34 remaining.
Zero new original PE32 executable comparisons were performed, and full
retail behavioral certification remains 0/615.
