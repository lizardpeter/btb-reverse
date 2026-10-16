#include "btb/bink32_dc_stream.hpp"
#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
std::optional<std::int16_t> read_original_bink_dc_word(
    BinkDcWordStream& dc) noexcept {
    // Source original: 3000F081 mov eax,[ebp-248];
    // 3000F087 mov cx,word [eax]; 3000F08A add eax,2;
    // 3000F09E mov word [ebp-2c8],cx.
    if(dc.position>dc.bytes.size() ||
       dc.bytes.size()-dc.position<2)return std::nullopt;
    const auto word=static_cast<std::uint16_t>(
        dc.bytes[dc.position] |
        (static_cast<std::uint16_t>(dc.bytes[dc.position+1])<<8));
    const auto coefficient=std::bit_cast<std::int16_t>(word);
    dc.position+=2;
    return coefficient;
}
std::optional<BinkDcAcBlock> decode_original_bink_block_from_dc_plane(
    BinkDcWordStream& dc_plane,LsbBitReader& ac_stream) noexcept {
    auto dc=dc_plane;
    auto ac=ac_stream;
    const auto initial=dc.position;
    const auto coefficient=read_original_bink_dc_word(dc);
    if(!coefficient)return std::nullopt;
    const auto block=decode_bink_ac_quantized_block(ac,*coefficient);
    if(!block)return std::nullopt;
    BinkDcAcBlock result{};
    result.dc=*coefficient;
    result.decoded=*block;
    result.dc_bytes_consumed=dc.position-initial;
    dc_plane=dc;
    ac_stream=ac;
    return result;
}
} // namespace btb::bink32_recovered
