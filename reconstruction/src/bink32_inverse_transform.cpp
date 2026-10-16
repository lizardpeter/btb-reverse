#include "btb/bink32_inverse_transform.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>

namespace btb::bink32_recovered {
std::optional<BinkDctIntermediateColumn>
idct_dc_only_column8(
    std::span<const std::int16_t> column,
    std::int32_t quant_dc) noexcept {
    if(column.size()!=8)return std::nullopt;
    // Original OR test of seven AC coefficients:
    // 30011393..300113CC loops x=0..7 and checks rows 1..7.
    if(std::any_of(column.begin()+1,column.end(),[](std::int16_t x){
        return x!=0;
    }))return std::nullopt;

    // 300113CE movsx eax,word[ebx]
    // 300113D1 imul eax,dword[edx] (low 32 bits, wrapping)
    // 300113D4 sar eax,11 (signed arithmetic right shift)
    // Eight identical DWORD stores at 300113D7..300113F4.
    const auto product=static_cast<std::int64_t>(column[0])*quant_dc;
    const auto low=static_cast<std::uint32_t>(product);
    const auto signed_low=std::bit_cast<std::int32_t>(low);
    const auto scaled=static_cast<std::int32_t>(signed_low>>11);
    BinkDctIntermediateColumn out{};
    out.fill(scaled);
    return out;
}
std::optional<std::array<std::uint8_t,64>>
idct_dc_only_block8(
    std::span<const std::int16_t> coefficients,
    std::int32_t quant_dc) noexcept {
    if(coefficients.size()!=64 ||
       std::any_of(coefficients.begin()+1,coefficients.end(),
          [](std::int16_t coefficient){return coefficient!=0;}))
        return std::nullopt;
    // The first (vertical) pass at 300113CE replicates DC*Q >> 11
    // in all eight rows. Every other column is zero. The horizontal
    // butterfly at 30011590..30011705 therefore outputs the same
    // value for every pixel, with original (+0x7f) >> 8 rounding.
    const BinkDctColumn dc_column{
        coefficients[0],0,0,0,0,0,0,0
    };
    const auto columns=idct_dc_only_column8(dc_column,quant_dc);
    if(!columns)return std::nullopt;
    const std::int32_t value=(*columns)[0];
    const std::int32_t rounded=(value+127)>>8;
    // 30011617..300116CD uses the low byte, not saturating casts.
    const auto output=static_cast<std::uint8_t>(rounded);
    std::array<std::uint8_t,64> pixels{};
    pixels.fill(output);
    return pixels;
}

} // namespace btb::bink32_recovered
