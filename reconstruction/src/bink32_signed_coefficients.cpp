#include "btb/bink32_signed_coefficients.hpp"
#include <algorithm>
#include <cstdint>
#include <vector>

namespace btb::bink32_recovered {
std::optional<SignedCoefficientRun> decode_bink_signed_coefficients(
    LsbBitReader& compressed,
    const DynamicHuffmanTree& magnitude_tree,
    std::span<std::int8_t> destination) {
    // Retail writes directly into its own allocated output. Decode into
    // scratch to preserve caller state for malformed/untrusted files.
    constexpr std::size_t kMaximumCoefficients=1U<<20;
    if(destination.size()>kMaximumCoefficients)return std::nullopt;
    auto bits=compressed;
    std::vector<std::int8_t> scratch(destination.size());
    const auto start=bits.bit_position();
    SignedCoefficientRun result{};
    for(std::size_t pos=0;pos<scratch.size();) {
        // Original 0x30010A12/0x30010A32: one-bit branch selector.
        const auto fill=bits.read(1);
        if(!fill)return std::nullopt;
        std::uint32_t magnitude=0;
        if(*fill) {
            // Original 30010B34..30010B95: one unsigned nibble followed
            // by a sign bit *only if the nibble is nonzero*.
            const auto immediate=bits.read(4);
            if(!immediate)return std::nullopt;
            magnitude=*immediate;
        } else {
            // Original 30010A66..30010ADB: one tree-lookup magnitude,
            // with a conditional sign bit at 30010AE3..30010B11.
            const auto decoded=read_dynamic_huffman_symbol(bits,magnitude_tree);
            if(!decoded)return std::nullopt;
            magnitude=decoded->value;
            ++result.explicit_coefficients;
        }
        if(magnitude>15)return std::nullopt;
        bool negative=false;
        if(magnitude!=0){
            const auto sign=bits.read(1);
            if(!sign)return std::nullopt;
            negative=*sign!=0;
        }
        const auto sample=static_cast<std::int8_t>(
            negative ? -static_cast<std::int32_t>(magnitude)
                     : static_cast<std::int32_t>(magnitude));
        if(*fill) {
            // 30010BA0..30010BC2 writes the same signed byte to the
            // entire remaining coefficient region.
            std::fill(scratch.begin()+static_cast<std::ptrdiff_t>(pos),
                      scratch.end(),sample);
            result.filled_remainder=true;
            pos=scratch.size();
        } else {
            scratch[pos++]=sample;
        }
    }
    result.bits_consumed=bits.bit_position()-start;
    std::copy(scratch.begin(),scratch.end(),destination.begin());
    compressed=bits;
    return result;
}
} // namespace btb::bink32_recovered
