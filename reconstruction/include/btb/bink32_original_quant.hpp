#pragma once
// Exact 16 fixed 8x8 quantization matrices extracted from user's original
// 2002 Bink32 DLL at 0x30049E40 (0x100-byte stride per 4-bit selector).
// Recovered via 0x30011729..0x30011733 and block caller 0x3000F0ED.
// Quant tables are compiled in; no external DLL, process or assets required.
#include "btb/bink32_full_idct.hpp"
#include "btb/bink32_packet_decode.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
namespace btb::bink32_recovered {
inline constexpr std::size_t kOriginalBinkQuantTables=16;
using BinkOriginalQuantMatrix=std::array<std::int32_t,64>;
[[nodiscard]] std::optional<std::span<const std::int32_t,64>>
original_quant_matrix(std::uint8_t selector) noexcept;
// A complete 8x8 coefficient input transformed with exact original quant
// weights. Coefficient generation from actual compressed frame is separate.
[[nodiscard]] std::optional<BinkFullIdctPixels>
bink_idct_original_quant(
    std::span<const std::int16_t> coefficients,
    std::uint8_t selector) noexcept;
// Original 0x3000F0ED reads the 4-bit quant matrix selector AFTER the
// coefficient expansion at 0x3000F0A5. Given the already-decoded 64
// coefficients, this executes the original selector->matrix->iDCT path.
// Transactional: malformed coefficient input or short stream consumes no bits.
[[nodiscard]] std::optional<BinkFullIdctPixels>
decode_original_quantized_block(
    LsbBitReader& compressed,
    std::span<const std::int16_t> coefficients) noexcept;
} // namespace btb::bink32_recovered
