#pragma once
// Original 2002 binkw32.dll 0x30011B10 coefficient magnitude-level
// scheduler for all modes 0..15 using original x86 read widths 0..14.
// It reads the initial four-bit AC mode from the input and runs the
// source-derived mutable command-list passes followed by +/-1 pass.
// Does not parse DC, block modes, quant selector, chroma or full frames.
#include "btb/bink32_ac_unit_pass.hpp"
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct AcOuterLevelResult {
    std::uint8_t mode{},magnitude_passes{};
    std::uint32_t consumed_bits{},written_coefficients{};
    std::uint32_t pending_controls{},activations{};
};
// Implements every four-bit AC mode 0..15. For mode >=2, the original
// starts its low-bit read count at mode-1 and decrements once per pass,
// independently from the high class-bit 1 << (mode-1).
// The two values were previously conflated, incorrectly excluding 7..15.
// On success output DC coefficient 0 is preserved, AC 1..63 reset
// to original zero initialization before processing controls.
[[nodiscard]] std::optional<AcOuterLevelResult>
decode_bink_ac_outer_levels(
    LsbBitReader& compressed,
    std::span<std::int16_t> output_coefficients) noexcept;
} // namespace btb::bink32_recovered
