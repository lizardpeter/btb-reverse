#pragma once
// Source-derived translation of original Bink 2002 DLL 0x30011350..0x30011715.
// An 8x8 signed-coefficient block and its 8x8 quantization matrix are
// transformed into a 16x16 byte block, doubled in *both* axes as the
// original function's duplicate DWORD stores show. No DLL is called.
// This standalone transform does not parse macroblock modes or Bink frames.
#include <array>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
using BinkFullIdctPixels = std::array<std::uint8_t, 16 * 16>;
[[nodiscard]] std::optional<BinkFullIdctPixels> bink_idct_16x16(
    std::span<const std::int16_t> coefficients,
    std::span<const std::int32_t> quant_matrix) noexcept;
} // namespace btb::bink32_recovered
