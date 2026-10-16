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
} // namespace btb::bink32_recovered
