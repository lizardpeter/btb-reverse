#include "btb/bink32_ac_nonzero.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
std::optional<AcDirectCoefficient> decode_bink_ac_direct3(
    LsbBitReader& source, std::uint8_t& command,
    unsigned low_magnitude_bits, std::uint16_t high_magnitude_bit,
    std::span<std::int16_t> coefficients) noexcept {
    // Indirect jump table entry 3 only; source upper six bits are the
    // index of the destination signed-16 coefficient.
    if ((command & 3U) != 3U) return std::nullopt;
    const auto slot=static_cast<unsigned>(command >> 2);
    if (slot>=coefficients.size()) return std::nullopt;
    // In the original outer loop, the class bit was initialized as
    // 1 << (mode - 1), with mode in 2..15; EAX shrinks in later passes.
    // Reject impossible/unrepresentable sizes before touching any state.
    if (low_magnitude_bits>14 || high_magnitude_bit==0 ||
        high_magnitude_bit>16384 ||
        (high_magnitude_bit & (high_magnitude_bit-1))!=0 ||
        low_magnitude_bits>=16 ||
        (1U << low_magnitude_bits)>high_magnitude_bit)
        return std::nullopt;

    auto cursor=source;
    const auto low=cursor.read(low_magnitude_bits);
    if (!low) return std::nullopt;
    const auto negative=cursor.read(1);
    if (!negative) return std::nullopt;
    const auto magnitude=static_cast<std::uint32_t>(*low | high_magnitude_bit);
    if (magnitude>32767) return std::nullopt;
    const auto signed_value=static_cast<std::int16_t>(
        *negative ? -static_cast<std::int32_t>(magnitude)
                  : static_cast<std::int32_t>(magnitude));
    const AcDirectCoefficient result{static_cast<std::uint8_t>(slot),
       signed_value,static_cast<std::uint8_t>(low_magnitude_bits),
       static_cast<std::uint32_t>(low_magnitude_bits+1)};
    coefficients[slot]=signed_value;
    command=0; // 30012003 marks the packed command consumed
    source=cursor;
    return result;
}
std::optional<std::array<std::uint8_t,4>>
expand_bink_ac_control1(std::uint8_t command) noexcept {
    if ((command & 3U)!=1U) return std::nullopt;
    // 30011C61: shr bl,2; shl cl,2; then offsets 2,12h,22h,32h.
    const auto base=static_cast<unsigned>(command & 0xfcU);
    return std::array<std::uint8_t,4>{
        static_cast<std::uint8_t>(base+0x02U),
        static_cast<std::uint8_t>(base+0x12U),
        static_cast<std::uint8_t>(base+0x22U),
        static_cast<std::uint8_t>(base+0x32U)
    };
}
} // namespace btb::bink32_recovered
