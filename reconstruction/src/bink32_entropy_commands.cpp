#include "btb/bink32_entropy_commands.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <utility>

namespace btb::bink32_recovered {
std::optional<ExpandedBlockRun> expand_bink_value_commands(
    LsbBitReader& compressed, const DynamicHuffmanTree& tree,
    std::span<std::uint8_t> destination,std::uint8_t previous) {
    // The retail DLL trusts memory pointers supplied by its own allocator.
    // Native file input is untrusted; do not allocate a pathological plane.
    constexpr std::size_t kMaxBlockValues=1U<<20;
    if(destination.size()>kMaxBlockValues)return std::nullopt;
    auto cursor=compressed;
    std::vector<std::uint8_t> scratch(destination.size());
    constexpr std::array<std::uint8_t,4> run_lengths{4,8,12,32};
    ExpandedBlockRun result{};
    std::size_t pos=0;
    const auto begin=cursor.bit_position();
    while(pos<scratch.size()) {
        // Original 0x30010242 and 0x30010372: 1 = fill remaining
        // destination with immediate four-bit constant.
        const auto immediate=cursor.read(1);
        if(!immediate)return std::nullopt;
        if(*immediate) {
            const auto value=cursor.read(4);
            if(!value)return std::nullopt;
            std::fill(scratch.begin()+static_cast<std::ptrdiff_t>(pos),
                      scratch.end(),static_cast<std::uint8_t>(*value));
            result.filled_remainder=true;
            pos=scratch.size();
            break;
        }
        // Original 0x300102A6..0x300102DC: table-based Huffman read.
        const auto symbol=read_dynamic_huffman_symbol(cursor,tree);
        if(!symbol)return std::nullopt;
        ++result.huffman_symbols;
        const std::uint8_t code=symbol->value;
        if(code<12) {
            scratch[pos++]=code;
            previous=code;
        } else if(code<=15) {
            // Original 0x30010325 indexed 0x30047BDC+code:
            // the four run lengths are 4,8,12,32.
            const auto repeat=run_lengths[code-12];
            if(repeat>scratch.size()-pos)return std::nullopt;
            std::fill_n(scratch.begin()+static_cast<std::ptrdiff_t>(pos),
                        repeat,previous);
            pos+=repeat;
            ++result.repeat_commands;
        } else return std::nullopt;
    }
    result.bits_consumed=cursor.bit_position()-begin;
    std::copy(scratch.begin(),scratch.end(),destination.begin());
    compressed=cursor;
    return result;
}
// x86 30010632..30010651:
// cl = (high << 4)|low; if (cl & 0x80) cl = -(cl & 0x7f);
// else cl &= 0x7f; cl += 0x80, store one unsigned byte.
std::uint8_t combine_bink_signed_nibbles(
    std::uint8_t high,std::uint8_t low) noexcept {
    const auto packed=std::uint8_t(((high&15U)<<4)|(low&15U));
    const auto magnitude=static_cast<unsigned>(packed&0x7fU);
    return static_cast<std::uint8_t>(
        packed&0x80U ? 128U-magnitude : 128U+magnitude);
}
std::optional<std::uint8_t> read_bink_signed_sample(
    LsbBitReader& bits,
    const DynamicHuffmanTree& high_tree,
    const DynamicHuffmanTree& low_tree) noexcept {
    auto probe=bits;
    const auto high=read_dynamic_huffman_symbol(probe,high_tree);
    if(!high)return std::nullopt;
    const auto low=read_dynamic_huffman_symbol(probe,low_tree);
    if(!low)return std::nullopt;
    const auto sample=combine_bink_signed_nibbles(high->value,low->value);
    bits=probe;
    return sample;
}
std::optional<DecodedBinkBlockModes> decode_bink_block_mode_values(
    LsbBitReader& compressed,const DynamicHuffmanTree& tree,
    std::size_t count,std::uint8_t previous_mode) {
    if(!count || count>(1U<<20) || previous_mode>9)
        return std::nullopt;
    auto probe=compressed;
    DecodedBinkBlockModes decoded{};
    decoded.values.resize(count);
    const auto r=expand_bink_value_commands(
        probe,tree,decoded.values,previous_mode);
    if(!r)return std::nullopt;
    // The original 4-bit command alphabet can emit literal 10/11
    // although those are not valid standard Bink1 pixel block types.
    if(std::any_of(decoded.values.begin(),decoded.values.end(),
        [](std::uint8_t v){return v>9;}))
        return std::nullopt;
    decoded.statistics=*r;
    compressed=probe;
    return decoded;
}

std::optional<BinkBlockTypeBundle> read_bink_block_type_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& type_tree,
    std::size_t plane_width,std::size_t maximum_values) {
    if(!plane_width || plane_width>16384 || !maximum_values ||
       maximum_values>(1U<<20))return std::nullopt;
    // Original 0x3000FC00 derives the count width from 512 + plane
    // width in 8x8 blocks; the first original bundle uses this context.
    const auto context=init_entropy_plane(
        0,0x200U,static_cast<std::uint32_t>(plane_width),4,1,false);
    if(!context.table_bit_width || context.table_bit_width>24)
        return std::nullopt;
    auto probe=bits;
    const auto start=probe.bit_position();
    const auto count=probe.read(context.table_bit_width);
    if(!count || *count>maximum_values)return std::nullopt;
    BinkBlockTypeBundle bundle{};
    bundle.encoded_count=*count;
    if(*count==0) {
        bundle.end_of_bundle=true;
    } else {
        const auto expanded=decode_bink_block_mode_values(
            probe,type_tree,*count,0);
        if(!expanded)return std::nullopt;
        bundle.modes=std::move(expanded->values);
        bundle.commands=expanded->statistics;
    }
    bundle.bits_consumed=probe.bit_position()-start;
    bits=probe;
    return bundle;
}
std::optional<BinkFirstPictureBlockTypes>
decode_first_bink_picture_block_types(
    std::span<const std::uint8_t> video_packet,
    std::size_t plane_width,std::size_t plane_height) {
    if(!plane_width || !plane_height || plane_width>16384 ||
       plane_height>16384 || plane_width>(64U*1024U*1024U)/plane_height)
        return std::nullopt;
    const auto columns=(plane_width+7)/8;
    const auto rows=(plane_height+7)/8;
    if(columns>(1U<<20)/rows)return std::nullopt;
    const auto prologue=parse_alpha_picture_entropy_prologue(video_packet);
    if(!prologue)return std::nullopt;
    auto picture=video_packet.subspan(
        prologue->picture.initial_byte_offset);
    LsbBitReader reader(picture);
    if(!reader.skip(prologue->picture.trees.consumed_bits))
        return std::nullopt;
    const auto modes=read_bink_block_type_bundle(
        reader,prologue->picture.trees.trees[0],
        plane_width,columns*rows);
    if(!modes)return std::nullopt;
    BinkFirstPictureBlockTypes output{};
    output.prologue=std::move(*prologue);
    output.first_bundle=std::move(*modes);
    output.next_picture_bit_offset=
        output.prologue.picture.initial_byte_offset*8+
        reader.bit_position();
    return output;
}

} // namespace btb::bink32_recovered
