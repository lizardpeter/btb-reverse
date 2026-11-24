#include "btb/bink32_ac_group4.hpp"
#include <algorithm>
#include <bit>
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
    // 30011CDA..30011D1D reads EAX bits independently of the
    // magnitude-class WORD at [esp+28]. EAX can exceed the class-bit
    // position; the previous check rejected valid source streams.
    if (first>60 || low_magnitude_bits>31 || high_magnitude_bit==0 ||
        (high_magnitude_bit & (high_magnitude_bit-1))!=0)
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
        const auto magnitude=static_cast<std::uint16_t>(
            *low|high_magnitude_bit);
        // 30011D43 MOVSX EDI,DI; 30011D48 NEG EDI; 30011D4E
        // MOV WORD stores low 16 bits, including signed wrap cases.
        const auto raw=static_cast<std::int32_t>(
            std::bit_cast<std::int16_t>(magnitude));
        scratch[index]=std::bit_cast<std::int16_t>(
            static_cast<std::uint16_t>(*negative?-raw:raw));
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
std::optional<AcGroup4> decode_bink_ac_group4_0(
    LsbBitReader& source, std::uint8_t& command,
    unsigned low_magnitude_bits, std::uint16_t high_magnitude_bit,
    std::span<std::int16_t> coefficients) noexcept {
    if((command & 3U)!=0U)return std::nullopt;
    const auto original=command;
    auto synthetic=static_cast<std::uint8_t>((command&0xfcU)|2U);
    const auto decoded=decode_bink_ac_group4_2(
        source,synthetic,low_magnitude_bits,high_magnitude_bit,coefficients);
    if(!decoded)return std::nullopt;
    // 30011C52..30011C5F rewrites the original control instead of
    // zeroing it as at 30011C93; following four decisions share
    // the exact 30011C9E..30011F7A body.
    command=static_cast<std::uint8_t>((original&0xfcU)+0x11U);
    return decoded;
}

} // namespace btb::bink32_recovered
