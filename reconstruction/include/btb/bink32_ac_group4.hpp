#pragma once
// Source-derived 2002 Bink32 coefficient decoder, original indirect opcode 2
// at 0x30011C93..0x30011F7A (jump table VA 0x3001241C).
// This 4-slot operation is only one branch of 0x30011B10, not a full frame.
#include "btb/bink32_ac_nonzero.hpp"
#include <array>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
// A decision bit per successive slot either emits one signed 16-bit AC
// coefficient or postpones that slot as an opcode-3 control. Deferred
// entries are returned in the original stack's reverse/prepend order.
struct AcGroup4 {
    std::array<std::uint8_t,4> deferred_commands{};
    std::uint8_t deferred_count{}, written_count{};
    std::uint32_t consumed_bits{};
};
[[nodiscard]] std::optional<AcGroup4> decode_bink_ac_group4_2(
    LsbBitReader& source, std::uint8_t& command,
    unsigned low_magnitude_bits, std::uint16_t high_magnitude_bit,
    std::span<std::int16_t> coefficients) noexcept;
} // namespace btb::bink32_recovered
