#pragma once
// Source-level reconstruction of original Bink32 internal 0x3000FCB0
// (tree permutation) and 0x300100E0 (stable subtree interleave).
// Video coefficients, motion compensation and YUV output are still pending.
#include "btb/bink32_packet_decode.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct DynamicHuffmanTree {
    std::uint8_t selector{};
    std::array<std::uint8_t,16> symbol_permutation{};
    std::size_t consumed_bits{};
};
// Original 0x300100E0: read one bit to choose from each subtree, preserving
// relative order. Once a subtree is exhausted, copy remaining symbols.
[[nodiscard]] bool interleave_huffman_subtrees(
    LsbBitReader& bits, std::span<const std::uint8_t> left,
    std::span<const std::uint8_t> right, std::span<std::uint8_t> destination) noexcept;
// Full 16-symbol permutation construction for each of the two branches and
// all four interleave submodes. Short/invalid inputs leave cursor unchanged.
[[nodiscard]] std::optional<DynamicHuffmanTree> read_dynamic_huffman_tree(
    LsbBitReader& bits) noexcept;
// Apply recovered permutation after the actual original fixed Huffman table
// lookup. Not yet a complete Bink video/entropy-plane decoder.
[[nodiscard]] std::optional<FixedHuffmanSymbol> read_dynamic_huffman_symbol(
    LsbBitReader& bits, const DynamicHuffmanTree& tree) noexcept;
// Source order of original 3000E582..3000E682: two leading entropy
// trees, sixteen repeated channel/plane maps, and five trailing maps.
struct BlockEntropyTrees {
    std::array<DynamicHuffmanTree,23> trees{};
    std::size_t consumed_bits{};
};
// Transactional conversion of all 23 original tree setup calls into a
// source-native C++ object. Does not yet decode coefficients or pixels.
[[nodiscard]] std::optional<BlockEntropyTrees> read_block_entropy_trees(
    LsbBitReader& bits) noexcept;

} // namespace btb::bink32_recovered
