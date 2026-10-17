#include "btb/bink32_ac_block_pixels.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
std::optional<DecodedAcQuantBlock> decode_bink_ac_quantized_block(
    LsbBitReader& compressed,std::int16_t already_decoded_dc) noexcept {
    auto bits=compressed;
    const auto start=bits.bit_position();
    DecodedAcQuantBlock result{};
    result.coefficients[0]=already_decoded_dc;
    const auto ac=decode_bink_ac_outer_levels(bits,result.coefficients);
    if(!ac)return std::nullopt;
    result.ac=*ac;
    // Original block site 3000F0ED reads the quantization selector
    // only AFTER 3000F0A5 returns the entire AC coefficient array.
    const auto selector=bits.read(4);
    if(!selector)return std::nullopt;
    result.quant_selector=static_cast<std::uint8_t>(*selector);
    const auto pixels=bink_idct_original_quant(
        result.coefficients,result.quant_selector);
    if(!pixels)return std::nullopt;
    result.pixels=*pixels;
    result.consumed_bits=bits.bit_position()-start;
    compressed=bits;
    return result;
}
} // namespace btb::bink32_recovered
