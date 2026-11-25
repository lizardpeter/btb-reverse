# Squirrel run-assembly frame trace

Original PE SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.

The original 00425F40 active path sets DAT_00515110 from the current atlas frame even on a suppressed draw. At 004265C7 it tests the previous value and resets absolute Y to the latched connector destination (237, 205, 173) before applying the D-phase delta. This is now owned by the motion kernel rather than a driver-side post-update correction. With delay 1 several phase boundaries can occur within one call; the old driver-side state comparison missed the reset in that case.

A local bounded integer-instruction translation of 00425F40, 004259D0 and 0042FFC4 captured each original DirectDraw blit intent; the other call (00402AF0) is a single RET. It did not execute a Windows PE or a physical renderer. A 32-bit ELF wrapper was attempted but the current Linux environment does not execute i386 ELF. The translated reference and a local source-equivalent kernel matched all 5,225,472 frame records (40,824 scenarios x 128 updates). The new production-header regression independently folds the corresponding state and raw draw intent into FNV-1a 64 fingerprint 9f2c9d55e0505c56. Each record has 10 state words and 5 draw words: phase, section, x/y, delay, substep, mode, sprite, high latch, CRT seed, then blit count, x/y, left/right. The test uses the known source-motion fixture, all connector tuples and piece IDs, sections 0..6, and delays 1 and 7. It intentionally hashes the raw frame-86 idle draw intent; the live adapter still rejects this out-of-range BMP rectangle. Exact production CI verification pending at commit time.

This validates a specific bounded animation-state domain, not complete retail gameplay or device behavior.

The optional `tools/verify_retail_squirrel_disassembly.py` diagnostic now includes the full frame comparison, reading the exact initialized words from the supplied original executable. It reports the first differing state/draw word rather than only a fingerprint. Run it with a compiler supporting the repository headers and `--std c++2c`. The original file stays local.
