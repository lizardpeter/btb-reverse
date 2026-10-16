#include "btb/bink32_dc_stream.hpp"
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

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
bool expand_original_bink_zero_delta_group(
    LsbBitReader& source,std::int16_t previous_dc,
    std::span<std::int16_t> output) noexcept {
    // Original decoder 30010D94 chooses min(remaining_count, 8).
    // 30010DA5..30010DD6 consumes a four-bit nibble.
    // If it equals zero, 30010E8D..30010EBE performs REP STOSW
    // with the PREVIOUS cumulative sample, no sign/delta bits.
    if(output.empty() || output.size()>8)return false;
    auto bits=source;
    const auto encoded_width=bits.read(4);
    if(!encoded_width || *encoded_width!=0)return false;
    for(auto& v:output)v=previous_dc;
    source=bits;
    return true;
}
std::optional<std::int16_t> decode_original_bink_dc_delta_group(
    LsbBitReader& source,std::int16_t previous_dc,
    std::span<std::int16_t> output) noexcept {
    // Original 0x30010D94 handles at most 8 samples per group.
    // Original 0x30010DA5..0x30010DD6 consumes a 4-bit
    // unsigned width, then 0x30010DE2..0x30010E86 reads one
    // magnitude plus conditional sign per sample.
    if(output.empty() || output.size()>8)return std::nullopt;
    auto bits=source;
    const auto width=bits.read(4);
    if(!width)return std::nullopt;
    std::array<std::int16_t,8> scratch{};
    std::uint16_t previous=std::bit_cast<std::uint16_t>(previous_dc);
    for(std::size_t i=0;i<output.size();++i) {
        std::uint32_t magnitude=0;
        if(*width) {
            const auto encoded=bits.read(*width);
            if(!encoded)return std::nullopt;
            magnitude=*encoded;
        }
        if(magnitude) {
            const auto sign=bits.read(1);
            if(!sign)return std::nullopt;
            const auto delta=static_cast<std::uint16_t>(
                *sign ? -static_cast<std::int32_t>(magnitude)
                      : static_cast<std::int32_t>(magnitude));
            previous=static_cast<std::uint16_t>(previous+delta);
        }
        scratch[i]=std::bit_cast<std::int16_t>(previous);
    }
    std::copy_n(scratch.begin(),output.size(),output.begin());
    source=bits;
    return std::bit_cast<std::int16_t>(previous);
}
std::optional<OriginalBinkDcPlane> decode_original_bink_dc_delta_plane(
    LsbBitReader& source,std::int16_t previous_dc,
    std::size_t sample_count) {
    // Source 30010D94: group_count = min(remaining_count,8),
    // decode 4-bit width then group_count signed cumulative deltas,
    // iterate at 30010EC2 until all original output words written.
    constexpr std::size_t kMaximumDcWords=1U<<20;
    if(sample_count>kMaximumDcWords)return std::nullopt;
    auto bits=source;
    const auto start=bits.bit_position();
    OriginalBinkDcPlane result{};
    result.signed_words_le.resize(sample_count*2);
    std::array<std::int16_t,8> group{};
    std::size_t emitted=0;
    while(emitted<sample_count) {
        const auto n=std::min(std::size_t{8},sample_count-emitted);
        const auto last=decode_original_bink_dc_delta_group(
            bits,previous_dc,std::span<std::int16_t>(group).first(n));
        if(!last)return std::nullopt;
        previous_dc=*last;
        for(std::size_t i=0;i<n;++i) {
            const auto word=std::bit_cast<std::uint16_t>(group[i]);
            const auto j=(emitted+i)*2;
            result.signed_words_le[j]=static_cast<std::uint8_t>(word);
            result.signed_words_le[j+1]=static_cast<std::uint8_t>(word>>8);
        }
        emitted+=n;
    }
    result.final_sample=previous_dc;
    result.consumed_bits=bits.bit_position()-start;
    source=bits;
    return result;
}
std::optional<OriginalBinkDcPlane> decode_original_bink_dc_context_packet(
    LsbBitReader& source,unsigned count_width,unsigned value_width,
    bool signed_values,std::size_t maximum_output_words) {
    // 30010C07 reads an output sample count using context +0x28.
    // 30010C74 handles count==0 immediately.
    // 30010C91..30010D6C reads the first sample with context +0x08
    // and optional sign (context +0x0C), then 30010D74 stores
    // its first WORD. 30010D94 runs delta groups for count-1.
    constexpr std::size_t kMaximumWords=1U<<20;
    if(!count_width || count_width>24 || !value_width ||
       value_width>16 || !maximum_output_words ||
       maximum_output_words>kMaximumWords)
        return std::nullopt;
    auto bits=source;
    const auto begin=bits.bit_position();
    const auto encoded_count=bits.read(count_width);
    if(!encoded_count || *encoded_count>maximum_output_words)
        return std::nullopt;
    const auto count=static_cast<std::size_t>(*encoded_count);
    OriginalBinkDcPlane result{};
    if(count==0) {
        result.consumed_bits=bits.bit_position()-begin;
        source=bits;
        return result;
    }
    const auto first_magnitude_bits=value_width-(signed_values?1U:0U);
    const auto first_magnitude=bits.read(first_magnitude_bits);
    if(!first_magnitude)return std::nullopt;
    std::int32_t initial=static_cast<std::int32_t>(*first_magnitude);
    if(signed_values && initial!=0) {
        const auto sign=bits.read(1);
        if(!sign)return std::nullopt;
        if(*sign)initial=-initial;
    }
    const auto first_word=static_cast<std::uint16_t>(initial);
    const auto first=std::bit_cast<std::int16_t>(first_word);
    result.signed_words_le.reserve(count*2);
    result.signed_words_le.push_back(static_cast<std::uint8_t>(first_word));
    result.signed_words_le.push_back(static_cast<std::uint8_t>(first_word>>8));
    if(count>1) {
        const auto tail=decode_original_bink_dc_delta_plane(
            bits,first,count-1);
        if(!tail)return std::nullopt;
        result.signed_words_le.insert(result.signed_words_le.end(),
            tail->signed_words_le.begin(),tail->signed_words_le.end());
        result.final_sample=tail->final_sample;
    } else {
        result.final_sample=first;
    }
    result.consumed_bits=bits.bit_position()-begin;
    source=bits;
    return result;
}
std::optional<OriginalBinkDcPlane> decode_original_bink_dc_context_packet(
    LsbBitReader& source,const EntropyPlaneSetup& context,
    std::size_t maximum_output_words) {
    // The existing original 3000FC00 reconstruction already
    // calculates precisely these three fields:
    // [context+28] count width, [context+08] first-value bit
    // width, [context+0C] signed-value flag/prefix mask.
    return decode_original_bink_dc_context_packet(
        source,context.table_bit_width,context.symbol_bits,
        context.prefix_mask!=0,maximum_output_words);
}
std::optional<BinkDcContextBlocks> decode_original_bink_dc_context_blocks(
    LsbBitReader& dc_bits,LsbBitReader& ac_bits,
    const EntropyPlaneSetup& context,std::size_t maximum_blocks) {
    if(maximum_blocks==0 || maximum_blocks>(1U<<20))
        return std::nullopt;
    auto dc_probe=dc_bits;
    auto ac_probe=ac_bits;
    auto plane=decode_original_bink_dc_context_packet(
        dc_probe,context,maximum_blocks);
    if(!plane)return std::nullopt;
    BinkDcContextBlocks result{};
    result.dc_plane=std::move(*plane);
    BinkDcWordStream words{result.dc_plane.signed_words_le,0};
    result.blocks.reserve(result.dc_plane.signed_words_le.size()/2);
    const auto start=ac_probe.bit_position();
    while(words.remaining_words()!=0) {
        auto block=decode_original_bink_block_from_dc_plane(
            words,ac_probe);
        if(!block)return std::nullopt;
        result.blocks.push_back(std::move(*block));
    }
    result.ac_bits_consumed=ac_probe.bit_position()-start;
    dc_bits=dc_probe;
    ac_bits=ac_probe;
    return result;
}

namespace {
constexpr std::size_t kDecodedTileSide=16;
constexpr std::size_t kMaximumRasterPixels=64U*1024U*1024U;
// Reject invalid or potentially hostile dimensions before multiplying.
// The raster is tightly packed, and the last tile is clipped to bounds.
std::optional<std::size_t> expected_raster_blocks(
    std::size_t width,std::size_t height) noexcept {
    if(width==0 || height==0 || width>16384 || height>16384 ||
       width>kMaximumRasterPixels/height)
        return std::nullopt;
    const auto tiles_x=(width+15)/kDecodedTileSide;
    const auto tiles_y=(height+15)/kDecodedTileSide;
    if(tiles_x>(1U<<18)/tiles_y) return std::nullopt;
    return tiles_x*tiles_y;
}
} // namespace

std::optional<BinkDecodedRaster16> rasterize_bink_blocks16(
    std::span<const BinkDcAcBlock> blocks,
    std::size_t width,std::size_t height) {
    const auto count=expected_raster_blocks(width,height);
    if(!count || blocks.size()!=*count)return std::nullopt;
    BinkDecodedRaster16 raster{};
    raster.width=width;
    raster.height=height;
    raster.pixels.resize(width*height);
    const auto tiles_x=(width+15)/kDecodedTileSide;
    for(std::size_t index=0;index<blocks.size();++index) {
        const auto left=(index%tiles_x)*kDecodedTileSide;
        const auto top=(index/tiles_x)*kDecodedTileSide;
        const auto copy_width=std::min(kDecodedTileSide,width-left);
        const auto copy_height=std::min(kDecodedTileSide,height-top);
        const auto& tile=blocks[index].decoded.pixels;
        for(std::size_t row=0;row<copy_height;++row) {
            std::copy_n(tile.begin()+static_cast<std::ptrdiff_t>(
                row*kDecodedTileSide),copy_width,
                raster.pixels.begin()+static_cast<std::ptrdiff_t>(
                    (top+row)*width+left));
        }
    }
    return raster;
}

std::optional<BinkDecodedRaster16> decode_original_bink_dc_context_raster16(
    LsbBitReader& dc_bits,LsbBitReader& ac_bits,
    const EntropyPlaneSetup& context,
    std::size_t width,std::size_t height) {
    const auto count=expected_raster_blocks(width,height);
    if(!count)return std::nullopt;
    auto dc_probe=dc_bits;
    auto ac_probe=ac_bits;
    auto decoded=decode_original_bink_dc_context_blocks(
        dc_probe,ac_probe,context,*count);
    if(!decoded)return std::nullopt;
    auto raster=rasterize_bink_blocks16(decoded->blocks,width,height);
    if(!raster)return std::nullopt;
    dc_bits=dc_probe;
    ac_bits=ac_probe;
    return raster;
}

} // namespace btb::bink32_recovered
