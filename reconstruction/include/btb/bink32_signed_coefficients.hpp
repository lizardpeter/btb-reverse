#pragma once
// Source-derived reconstruction of 2002 binkw32.dll internal 0x30010980.
// Huffman magnitudes plus sign bits produce signed byte coefficients;
// command bit 1 fills the remaining output using an immediate nibble.
// Original pixel transforms remain separate and incomplete.
#include "btb/bink32_dynamic_huffman.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct SignedCoefficientRun {
    std::size_t bits_consumed{};
    std::uint32_t explicit_coefficients{};
    bool filled_remainder{};
};
[[nodiscard]] std::optional<SignedCoefficientRun>
decode_bink_signed_coefficients(
    LsbBitReader& compressed,
    const DynamicHuffmanTree& magnitude_tree,
    std::span<std::int8_t> destination);
} // namespace btb::bink32_recovered
