#include "btb/bink32_entropy_commands.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

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
} // namespace btb::bink32_recovered
