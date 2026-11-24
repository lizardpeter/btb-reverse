#pragma once
// 2002 Bink32 internal 0x300101A0: recover the value-command expansion
// portions of the native block data reader. Not a complete video decoder.
// Original repeated-value opcode table is at VA 0x30047BDC.
#include "btb/bink32_dynamic_huffman.hpp"
#include "btb/bink32_dc_stream.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
struct ExpandedBlockRun {
    std::size_t bits_consumed{};
    std::uint32_t huffman_symbols{}, repeat_commands{};
    bool filled_remainder{};
};
// Limited per-token EXPERIMENTAL command harness for studying the
// recovered Huffman/runs. NOT the original retail Bink bundle framing:
// original read_block_types consumes one immediate/Huffman branch bit
// for the entire count-prefixed bundle, not before each Huffman symbol.
// Source-correct retail decoding uses decode_bink_block_mode_values.
// This helper remains available only for isolated synthetic tests.
[[nodiscard]] std::optional<ExpandedBlockRun> expand_bink_value_commands(
    LsbBitReader& compressed,
    const DynamicHuffmanTree& tree,
    std::span<std::uint8_t> destination,
    std::uint8_t previous_value=0);
// Original 0x30010632..0x30010651 combines two four-bit symbols to a
// signed-magnitude 7-bit number and recenters it around 128.
// Independent primitive of the alternate block-value decoder (0x30010400).
[[nodiscard]] std::uint8_t combine_bink_signed_nibbles(
    std::uint8_t high, std::uint8_t low) noexcept;
[[nodiscard]] std::optional<std::uint8_t> read_bink_signed_sample(
    LsbBitReader& bits,
    const DynamicHuffmanTree& high_tree,
    const DynamicHuffmanTree& low_tree) noexcept;
// Expand bounded original entropy commands into actual 0..9 block mode
// identifiers. Invalid/reserved identifiers fail without cursor writes.
// This decodes the block TYPE bundle, not ancillary motion/color bundles.
struct DecodedBinkBlockModes {
    std::vector<std::uint8_t> values{};
    ExpandedBlockRun statistics{};
};
[[nodiscard]] std::optional<DecodedBinkBlockModes> decode_bink_block_mode_values(
    LsbBitReader& compressed,const DynamicHuffmanTree& tree,
    std::size_t count,std::uint8_t previous_mode=0);


// Original 0x300101A0 block-type bundle framing: a context-derived count,
// with count zero signalling the end of this bundle, followed by
// the existing compressed literal/repeat/fill value-command body.
// The per-plane frame scheduler can request another bundle later.
struct BinkBlockTypeBundle {
    std::vector<std::uint8_t> modes{};
    std::uint32_t encoded_count{};
    std::size_t bits_consumed{};
    bool end_of_bundle{};
    ExpandedBlockRun commands{};
};
[[nodiscard]] std::optional<BinkBlockTypeBundle>
read_bink_block_type_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& type_tree,
    std::size_t plane_width,std::size_t maximum_values=1U<<18);

// From the actual SJPi video packet through its alpha/picture boundary,
// source-order Huffman trees, count prefix, then first block-type bundle.
// This is the first resolved compressed frame command data; remaining
// bundle types and later row refills are still pending.
struct BinkFirstPictureBlockTypes {
    BinkAlphaPictureEntropy prologue{};
    BinkBlockTypeBundle first_bundle{};
    std::size_t next_picture_bit_offset{};
};
[[nodiscard]] std::optional<BinkFirstPictureBlockTypes>
decode_first_bink_picture_block_types(
    std::span<const std::uint8_t> video_packet,
    std::size_t plane_width,std::size_t plane_height);


// Authenticated narrow retail case: a whole alpha-bearing SJPi luma
// plane consisting exclusively of SKIP blocks. Reads *all* block-type
// refills (not just the first bundle), verifies every other first-row
// bundle has an empty count and honors 32-bit plane alignment.
// A previous luma image must be supplied; no made-up pixel values.
struct BinkSkipLumaFrame {
    std::vector<std::uint8_t> luma{};
    std::size_t block_types_decoded{};
    std::size_t bundles_decoded{};
    std::size_t next_plane_byte_offset{};
};
[[nodiscard]] std::optional<BinkSkipLumaFrame>
decode_bink_skip_only_picture_luma(
    std::span<const std::uint8_t> video_packet,
    std::size_t plane_width,std::size_t plane_height,
    std::span<const std::uint8_t> previous_luma);


// The original 0x3000E400 motion bundles (X and Y) each encode a
// context-sized count followed by ONE constant/entropy branch bit.
// Each nonzero magnitude has its own sign bit; zero has no sign bit.
struct BinkSignedMotionBundle {
    std::vector<std::int8_t> values{};
    std::uint32_t encoded_count{};
    std::size_t bits_consumed{};
    bool end_of_bundle{};
    bool constant_bundle{};
};
[[nodiscard]] std::optional<BinkSignedMotionBundle>
read_bink_signed_motion_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& tree,
    std::size_t plane_width,std::size_t maximum_values=1U<<18);

// The pattern bundle is a count followed by pairs of original Huffman
// nibbles; the low nibble is coded first. No immediate-value branch.
struct BinkPatternBundle {
    std::vector<std::uint8_t> values{};
    std::uint32_t encoded_count{};
    std::size_t bits_consumed{};
    bool end_of_bundle{};
};
[[nodiscard]] std::optional<BinkPatternBundle>
read_bink_pattern_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& tree,
    std::size_t plane_width,std::size_t maximum_values=1U<<18);


// Original BINK_SRC_RUN bundle: one count, then a single immediate/Huffman
// branch; direct values are 4-bit, otherwise one mapped nibble each.
struct BinkRunBundle {
    std::vector<std::uint8_t> values{};
    std::uint32_t encoded_count{};
    std::size_t bits_consumed{};
    bool end_of_bundle{};
    bool constant_bundle{};
};
[[nodiscard]] std::optional<BinkRunBundle>
read_bink_run_bundle(
    LsbBitReader& bits,const DynamicHuffmanTree& tree,
    std::size_t plane_width,std::size_t maximum_values=1U<<18);

// Original BINK_SRC_COLORS bundle: the previous high nibble selects
// one of sixteen adaptive high-nibble trees; another tree supplies
// each low nibble. High-nibble state must survive across bundle refills.
struct BinkColorBundle {
    std::vector<std::uint8_t> values{};
    std::uint32_t encoded_count{};
    std::size_t bits_consumed{};
    std::uint8_t next_high_nibble{};
    bool end_of_bundle{};
    bool constant_bundle{};
};
[[nodiscard]] std::optional<BinkColorBundle>
read_bink_color_bundle(
    LsbBitReader& bits,const BlockEntropyTrees& trees,
    std::size_t plane_width,std::uint8_t previous_high_nibble=0,
    bool early_signed_color_format=false,
    std::size_t maximum_values=1U<<18);


// Decode the FIRST bundle refill for all nine original picture-plane
// sources in order, without inventing any block traversal or pixel data.
// This gives the unresolved 0x3000E400 scheduler a bounded source-native
// value pool, and exposes the precise next compressed bit position.
struct BinkPictureFirstBundleSet {
    BinkAlphaPictureEntropy prologue{};
    BinkBlockTypeBundle types{}, scaled_subtypes{};
    BinkColorBundle colors{};
    BinkPatternBundle patterns{};
    BinkSignedMotionBundle motion_x{},motion_y{};
    OriginalBinkDcPlane intra_dc{},inter_dc{};
    BinkRunBundle runs{};
    std::size_t next_picture_bit_offset{};
};
[[nodiscard]] std::optional<BinkPictureFirstBundleSet>
decode_first_bink_picture_bundle_set(
    std::span<const std::uint8_t> video_packet,
    std::size_t plane_width,std::size_t plane_height);

} // namespace btb::bink32_recovered
