#pragma once
// Original 2002 binkw32.dll 0x30011B10 coefficient magnitude-level
// scheduler, bounded to modes 0..6 where x86 EAX bit widths are <=31.
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
// Supports only fully representable mode-bitcounts 0 through 6.
// Modes 7..15 need separate original x86 shift-count rollover
// analysis and are explicitly rejected without consuming input.
// On success output DC coefficient 0 is preserved, AC 1..63 reset
// to original zero initialization before processing controls.
[[nodiscard]] std::optional<AcOuterLevelResult>
decode_bink_ac_outer_levels(
    LsbBitReader& compressed,
    std::span<std::int16_t> output_coefficients) noexcept;
} // namespace btb::bink32_recovered
