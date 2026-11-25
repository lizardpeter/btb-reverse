# Squirrel instruction-level differential verification

The native production conveyor generator now shares the tested source routine, including the original raw feedback words, optional missing destination, 7-way permutation, and exact process CRT draw order.

Run the optional read-only original-image diagnostic:

```sh
python tools/verify_retail_squirrel_disassembly.py "/path/to/Bob the Builder - Bob Builds a Park.exe" --repo .
```

The diagnostic checks the exact retail image SHA-256, reads five bounded x86 routines with GNU objdump, and translates their restricted integer instruction subset into a temporary C++ comparison program. It also reads the original initialized permutation table. The reference does not call the reconstructed selector, actor or RNG. Unsupported instructions and unmapped memory fail the diagnostic. It is not a general-purpose emulator or a full PE/Win32 execution test. No original executable, assets or generated disassembly are stored in Git.

Local results against the extracted corresponding source logic:

| Routine | Cases | Mismatches |
| --- | ---: | ---: |
| 004259D0 motion selection | 69,984 | 0 |
| 00427270 actor movement | 308,074 | 0 |
| 00426F30 conveyor generation (uses original 0042FFC4 CRT rand) | 82,944 | 0 |
| 00427320 conveyor slide | 73,800 | 0 |

The conveyor comparison checks both output connector words, piece IDs, correctness flags, chosen permutation, optional landing connector and the final raw CRT seed. The integration uses the same routine rather than duplicating its algorithm. Full device rendering and retail gameplay equivalence remain separate validation gates.

Slide verification compares both original movement modes, animation countdown and frame, transition phase, positions near and beyond both endpoints, unsigned window-counter rollover, and generator callback count. The live driver currently selects the normal-speed branch; the alternate speed mode remains a separate host-integration concern.

The live Squirrel driver also no longer blocks the actor's selection just because the squirrel's independent run assembly is animating. The original 004273F0 tests conveyor phase and run section, and permits the first choice during the initial approach. A native regression starts that selection immediately, then confirms one placement and the conveyor transition. The typed adapter retains its existing bound of three pieces per level rather than allowing raw out-of-bounds piece slots.
