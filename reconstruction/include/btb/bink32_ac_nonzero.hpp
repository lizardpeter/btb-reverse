#pragma once
// 2002 retail Bink32 0x30011C4B / 0x30011F7F and 0x30011C61.
// Original four-entry indirect branch at VA 0x3001241C:
// case 1: expand one command into four coded subcommands;
// case 3: emit a direct signed 16-bit AC coefficient.
// This is a verified *subset* of the original 0x30011B10 coefficient
// parser, not an entire macroblock/frame decoder.
#include "btb/bink32_packet_decode.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct AcDirectCoefficient {
    std::uint8_t slot{};
    std::int16_t value{};
    std::uint8_t low_magnitude_bits{};
    std::uint32_t consumed_bits{};
};
// Original 0x30011F7F through 0x30012003, indirect opcode 3.
// A prior decoder pass has selected the high magnitude-class bit and
// width; this routine consumes that width's low magnitude bits and
// a sign bit. On success it zeroes the command byte, writes the signed
// 16-bit coefficient at slot = (command >> 2), and commits bit state.
[[nodiscard]] std::optional<AcDirectCoefficient> decode_bink_ac_direct3(
    LsbBitReader& source, std::uint8_t& command,
    unsigned low_magnitude_bits, std::uint16_t high_magnitude_bit,
    std::span<std::int16_t> coefficients) noexcept;

// Original 0x30011C61..30011C8E, indirect opcode 1.
// Returns the control byte replacement and three entries appended to
// the dispatch table. This does not consume compressed stream bits.
[[nodiscard]] std::optional<std::array<std::uint8_t,4>>
expand_bink_ac_control1(std::uint8_t command) noexcept;
} // namespace btb::bink32_recovered
