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
// Fast path for an entire 8x8 inverse DCT block with ONLY coefficient
// (0,0) set. Source: 300113CE..300113FA DC column replication and
// 30011590..30011705 horizontal pass with original (+127)>>8 rounding
// and byte narrowing. Rejects general AC blocks until full IDCT is ready.
[[nodiscard]] std::optional<std::array<std::uint8_t,64>>
idct_dc_only_block8(
    std::span<const std::int16_t> coefficients,
    std::int32_t quant_dc) noexcept;
} // namespace btb::bink32_recovered
