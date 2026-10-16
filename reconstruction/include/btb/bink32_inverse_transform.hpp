#pragma once
// Partial 2002 Bink32 inverse-transform reconstruction.
// Original 0x300113CE..0x300113FA: fast path for 8x1 DC-only coefficient
// columns. All non-DC coefficients must be zero; general iDCT is pending.
#include <array>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
using BinkDctColumn=std::array<std::int16_t,8>;
using BinkDctIntermediateColumn=std::array<std::int32_t,8>;
[[nodiscard]] std::optional<BinkDctIntermediateColumn>
idct_dc_only_column8(
    std::span<const std::int16_t> column,
    std::int32_t quant_dc) noexcept;
} // namespace btb::bink32_recovered
