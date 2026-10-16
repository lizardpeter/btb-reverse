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
} // namespace btb::bink32_recovered
