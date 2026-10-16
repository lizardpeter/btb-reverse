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
} // namespace btb::bink32_recovered
