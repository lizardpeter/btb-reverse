#pragma once
// 2002 original Bink32 decoder 0x3000EDC8..3000EDE6 and
// 0x3000F081..3000F0A5: acquire one signed 16-bit DC coefficient
// from a previously decoded delta/word plane, then submit its value
// as coeff[0] to the 0x30011B10 AC coefficient unpacker.
//
// The DC planes themselves originate in the 0x30010BF0 delta decoder
// called at 0x3000E8E5/3000E8F5. This module deliberately consumes
// predecoded words; it does not pretend they reside in the AC bits.
#include "btb/bink32_ac_block_pixels.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
struct BinkDcWordStream {
    std::span<const std::uint8_t> bytes{};
    std::size_t position{}; // byte offset, incremented by two
    [[nodiscard]] std::size_t remaining_words() const noexcept {
        return position<=bytes.size() ? (bytes.size()-position)/2 : 0;
    }
};
// Copy a native original 16-bit little-endian signed DC WORD and
// advance the source cursor exactly two bytes on success.
[[nodiscard]] std::optional<std::int16_t>
read_original_bink_dc_word(BinkDcWordStream& dc) noexcept;
struct BinkDcAcBlock {
    std::int16_t dc{};
    DecodedAcQuantBlock decoded{};
    std::size_t dc_bytes_consumed{};
};
// The same source ordering as 0x3000F081...3000F107:
// predecoded DC WORD -> 64 coeffs -> AC bitstream -> 4-bit quantizer
// -> original two-pass iDCT -> 16x16 native sample pixels.
// Entire two-input operation commits only on success.
[[nodiscard]] std::optional<BinkDcAcBlock>
decode_original_bink_block_from_dc_plane(
    BinkDcWordStream& dc_plane,LsbBitReader& ac_stream) noexcept;
// A genuine branch of original 0x30010BF0, at 0x30010DA5..0x30010EBE.
// For up to eight output samples, read the four-bit delta-width nibble.
// When that nibble is zero, the original 0x30010E8D..0x30010EBE
// repeats its previous signed 16-bit sample across the whole run.
// Nonzero-width delta decoding is still outstanding; reject it here.
[[nodiscard]] bool expand_original_bink_zero_delta_group(
    LsbBitReader& source,std::int16_t previous_dc,
    std::span<std::int16_t> output) noexcept;
// Original 0x30010D94..0x30010ECE full short-group delta path:
// one width nibble (0..15) followed by up to 8 signed magnitudes.
// A nonzero magnitude consumes a sign bit. Each signed delta is
// cumulatively added to the preceding sample and written as WORD.
// Width zero simply repeats the preceding value. This helper starts
// at the group-width nibble; higher-level 0x30010BF0 initial-plane
// headers, output allocation, and previous-sample initialization
// are NOT part of this routine and still need original-source wiring.
// On failure output and compressed cursor are unchanged.
[[nodiscard]] std::optional<std::int16_t>
decode_original_bink_dc_delta_group(
    LsbBitReader& source,std::int16_t previous_dc,
    std::span<std::int16_t> output) noexcept;
struct OriginalBinkDcPlane {
    std::vector<std::uint8_t> signed_words_le{};
    std::int16_t final_sample{};
    std::size_t consumed_bits{};
};
// Original 30010D94/30010ECE loops over contiguous groups of at most
// eight values. With the initial predictor and total SAMPLE COUNT
// supplied by a caller that has parsed 30010BF0's plane header,
// decode all groups into the exact signed-word little-endian stream
// consumed at 3000EDC8/3000F087. No source cursor commits unless the
// entire plane is complete; max output is capped to bound allocations.
[[nodiscard]] std::optional<OriginalBinkDcPlane>
decode_original_bink_dc_delta_plane(
    LsbBitReader& source,std::int16_t previous_dc,
    std::size_t sample_count);
// Original 0x30010BF0: decode a complete DC context record from
// its bitstream when the caller supplies the two *original context*
// bit widths (both computed by 0x3000FC00):
// - count_width: first variable-count field (context +0x28).
// - value_width: first sample's unsigned width (context +0x08).
// - signed_values: if context +0x0C != 0, the first value uses
//   (value_width-1) magnitude bits and an optional sign bit;
//   otherwise it uses value_width bits, unsigned.
// A count of zero encodes no samples. For count>0 the first value
// is emitted as signed WORD, followed by the recovered 8-sample
// delta groups for count-1 entries. The previous sample need no
// longer be invented or externally supplied.
// Original 0x3000FC00 context initialization and the actual frame
// block traversal must still be connected to this helper.
[[nodiscard]] std::optional<OriginalBinkDcPlane>
decode_original_bink_dc_context_packet(
    LsbBitReader& source,unsigned count_width,unsigned value_width,
    bool signed_values,std::size_t maximum_output_words=1U<<20);
// Use the already-recovered original 0x3000FC00 context setup
// (EntropyPlaneSetup::table_bit_width, symbol_bits, prefix_mask)
// directly instead of supplying arbitrary count/magnitude widths.
[[nodiscard]] std::optional<OriginalBinkDcPlane>
decode_original_bink_dc_context_packet(
    LsbBitReader& source,const EntropyPlaneSetup& context,
    std::size_t maximum_output_words=1U<<20);
// Batch handoff: one compressed original DC context record produces its
// signed WORD stream, then every WORD is submitted to the existing
// original AC/quant/iDCT decoder. This is an INTRA block-subset bridge,
// not the 0x3000E400 block scheduler or a complete retail frame.
// Both independent compressed cursors commit only if every block succeeds.
struct BinkDcContextBlocks {
    OriginalBinkDcPlane dc_plane{};
    std::vector<BinkDcAcBlock> blocks{};
    std::size_t ac_bits_consumed{};
};
[[nodiscard]] std::optional<BinkDcContextBlocks>
decode_original_bink_dc_context_blocks(
    LsbBitReader& dc_bits,LsbBitReader& ac_bits,
    const EntropyPlaneSetup& context,
    std::size_t maximum_blocks=4096);

// Assemble already-decoded 16x16 blocks into one bounded single-channel
// raster surface. Tile order is deliberately an EXPLICIT ROW-MAJOR
// harness convention, NOT a claim about the original Bink 0x3000E400
// block-type scheduler, color-plane placement or motion prediction.
// Width/height may end in partial 16x16 tiles; copy only visible pixels.
struct BinkDecodedRaster16 {
    std::size_t width{};
    std::size_t height{};
    std::vector<std::uint8_t> pixels{}; // tightly packed, stride=width
};
[[nodiscard]] std::optional<BinkDecodedRaster16>
rasterize_bink_blocks16(
    std::span<const BinkDcAcBlock> blocks,
    std::size_t width,std::size_t height);
// Decode a DC-context record and the corresponding separate AC stream,
// then rasterize every recovered 16x16 block into the caller-specified
// raster dimensions. Reject block-count/size disagreement. ALL stages
// are transactional across both bitreaders, including plane assembly.
[[nodiscard]] std::optional<BinkDecodedRaster16>
decode_original_bink_dc_context_raster16(
    LsbBitReader& dc_bits,LsbBitReader& ac_bits,
    const EntropyPlaneSetup& context,
    std::size_t width,std::size_t height);

} // namespace btb::bink32_recovered
