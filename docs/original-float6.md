# Connected original floating-point conversion runtime

Six newly reconstructed source bodies are linked into `btb_recompiled_core` and the executable's core validation path:

| Original address | Recovered implementation |
| --- | --- |
| 00437520 | Runtime control flags to x87 control-word bits |
| 00437713 | Mantissa truncation, sticky-bit rounding, final carry |
| 004377E1 | Three-word most-significant-first mantissa shift |
| 0043786E | LD12 conversion using the original float/double format tables |
| 00437B3B | IEEE double bits to ten-byte extended representation |
| 004388A1 | Numeric decimal digit accumulation and extended normalization |

The converter calls the existing compiled original copy, zero, zero-test, carry and bit-test routines. The original `004379DA` and `004379F0` wrappers now have direct linked overloads using the real converter and recovered table values; their earlier callback overloads remain available for existing source tests. These wrappers were already counted and do not increase the function count.

Instruction review identified a defect in the compiled `004376BD` helper: its return must be the final unsigned carry, rather than unconditional success. That is corrected. The carry decides whether the conversion increments the exponent after rounding. Rounding follows the original sticky-bit condition, rather than substituting host IEEE rounding. Mantissa words use a different order from the decimal accumulator's little-endian words; the two paths retain their respective original orders.

The tables at `00449F48` and `00449F60` are read directly from the SHA-verified retail image. Instructions, table bytes, and derived result vectors are checked into Git; the executable is not included.

`tools/build_retail_float_vectors.py` executes the original x86 instructions with Unicorn and regenerates 34,314 independent cases: 5,096 control mappings, 9,870 round cases, 13,545 shifts, 1,011 double expansions, 4,192 LD12 conversions, and 600 decimal accumulations. The C++ regression compares both output words and conversion status, including the directly linked original wrappers. Cases include normal/subnormal values, exponent limits, overflow, infinities, NaNs, both signs of zero, word-boundary shifts, sticky bits, and carry propagation.

Valid retail inputs are preserved, including the original collapse of negative double zero to positive extended zero. The native API rejects invalid rounding indices, undersized output spans, and a zero decimal significand that would loop indefinitely in the original normalization routine. These safe host input checks are explicit differences outside the checked valid-input domain. This is bounded original-instruction output validation, not complete ABI or full-game certification. No ledger row is promoted to `behavior_verified`.
