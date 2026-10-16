#pragma once
// 2002 Bink32 internal 0x300101A0: recover the value-command expansion
// portions of the native block data reader. Not a complete video decoder.
// Original repeated-value opcode table is at VA 0x30047BDC.
#include "btb/bink32_dynamic_huffman.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct ExpandedBlockRun {
    std::size_t bits_consumed{};
    std::uint32_t huffman_symbols{}, repeat_commands{};
    bool filled_remainder{};
};
// Huffman value command path 0x30010262..0x30010364.
// A bit 0 requests the next Huffman symbol; values 0..11 are literals,
// 12..15 repeat the previous output value 4/8/12/32 times.
// A bit 1 followed by a 4-bit value fills the remaining output.
// This is a strict, transactional reconstruction of that one branch
// of the original routine; other block coefficients remain incomplete.
[[nodiscard]] std::optional<ExpandedBlockRun> expand_bink_value_commands(
    LsbBitReader& compressed,
    const DynamicHuffmanTree& tree,
    std::span<std::uint8_t> destination,
    std::uint8_t previous_value=0);
} // namespace btb::bink32_recovered
