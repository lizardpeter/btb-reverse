#include "btb/bink32_ac_group4.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
std::optional<AcGroup4> decode_bink_ac_group4_2(
    LsbBitReader& source, std::uint8_t& command,
    unsigned low_magnitude_bits, std::uint16_t high_magnitude_bit,
    std::span<std::int16_t> coefficients) noexcept {
    if ((command & 3U)!=2U || coefficients.size()!=64)
        return std::nullopt;
    const auto first=static_cast<unsigned>(command>>2);
    if (first>60 || low_magnitude_bits>14 || high_magnitude_bit==0 ||
        high_magnitude_bit>16384 ||
        (high_magnitude_bit & (high_magnitude_bit-1))!=0 ||
        (1U<<low_magnitude_bits)>high_magnitude_bit)
        return std::nullopt;
    auto cursor=source;
    std::array<std::int16_t,64> scratch{};
    std::copy(coefficients.begin(),coefficients.end(),scratch.begin());
    AcGroup4 result{};
    const auto start=cursor.bit_position();
    for(unsigned i=0;i<4;++i) {
        const auto postpone=cursor.read(1);
        if(!postpone)return std::nullopt;
        const auto index=first+i;
        if(*postpone) {
            // Original 30011CC6, 30011D7D, 30011EEB: decrement
            // the command-list end pointer and prepend opcode 3.
            result.deferred_commands[result.deferred_count++]=
                static_cast<std::uint8_t>((index<<2)|3U);
            continue;
        }
        const auto low=cursor.read(low_magnitude_bits);
        if(!low)return std::nullopt;
        const auto negative=cursor.read(1);
        if(!negative)return std::nullopt;
        const auto magnitude=std::uint32_t(*low|high_magnitude_bit);
        if(magnitude>32767)return std::nullopt;
        scratch[index]=static_cast<std::int16_t>(
            *negative?-static_cast<std::int32_t>(magnitude)
                     :static_cast<std::int32_t>(magnitude));
        ++result.written_count;
    }
    // Original list grows backward from its end pointer: reverse the
    // deferred entries to preserve their original in-memory order.
    std::reverse(result.deferred_commands.begin(),
                 result.deferred_commands.begin()+result.deferred_count);
    result.consumed_bits=static_cast<std::uint32_t>(
        cursor.bit_position()-start);
    std::copy(scratch.begin(),scratch.end(),coefficients.begin());
    command=0; // Original 30011C96 zeroes the consumed control slot.
    source=cursor;
    return result;
}
} // namespace btb::bink32_recovered
