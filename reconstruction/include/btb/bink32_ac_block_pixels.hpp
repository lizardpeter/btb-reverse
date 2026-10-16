#pragma once
// Source-derived compressed AC-mode -> 64 signed coefficients ->
// original 4-bit quantizer -> 16x16 reconstructed pixel block.
// Requires one DC coefficient supplied by a caller: the upstream
// retail video macroblock/DC parser remains under reconstruction.
// No Bink DLL, FFmpeg executable or subprocess is invoked here.
#include "btb/bink32_ac_outer_levels.hpp"
#include "btb/bink32_original_quant.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
struct DecodedAcQuantBlock {
    std::array<std::int16_t,64> coefficients{};
    BinkFullIdctPixels pixels{};
    AcOuterLevelResult ac{};
    std::uint8_t quant_selector{};
    std::size_t consumed_bits{};
};
// Reads the original four-bit AC mode, all coefficient passes, then
// the original four-bit quant selector and fully quantized inverse DCT.
// On malformed input, does not change the incoming compressed cursor.
// Every AC mode 0..15 is supported; reference/motion paths are separate.
[[nodiscard]] std::optional<DecodedAcQuantBlock>
decode_bink_ac_quantized_block(
    LsbBitReader& compressed,std::int16_t already_decoded_dc) noexcept;
} // namespace btb::bink32_recovered
