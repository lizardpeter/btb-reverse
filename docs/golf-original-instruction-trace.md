# Golf original instruction trace

The original 00415080 shot lifecycle is tested from accepted power (state 2) through swing delay, flight, landing, feedback, and reset (state 0). The reference uses the matching PE SHA-256, GNU objdump instruction translation, a bounded x87 stack model, original x87 vector arithmetic, modeled integer point distance and truncation, the translated original CRT random function, and captured managed sound requests. It does not execute the Windows executable or physical graphics/audio devices.

The local source-equivalent comparison produced 178,178 complete trajectories (89 aim values × 1,001 power values × two display modes), 31,252,395 matching frame comparisons, FNV-1a fingerprint 5bf54240cf8eca71. Every frame compares state, bitwise float coordinates, velocity, launch angle, height, persistent cadence counter, score, attempts, matched target, power, RNG state, and voice ID. Original sound priority/class are checked at the call boundary.

The production Round regression uses this fingerprint, not a duplicate physics model. The optional reference tool additionally compares every field to the translated original directly:

    python tools/verify_retail_golf_disassembly.py /path/to/original.exe --std c++20

Run with an x86-64 GNU-style compiler and an 80-bit long double. Synthetic meter speeds initialize every power; some are unreachable using shipped difficulty settings. Swing completion is provided at the original 72nd update. This is isolated gameplay verification, not whole retail parity.
