#include "btb/bink32_ac_command_pass.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
AcPendingCommands initial_bink_ac_pending() noexcept {
    AcPendingCommands state{};
    constexpr std::array<std::uint8_t,6> initial{
        0x10,0x60,0xb0,0x07,0x0b,0x0f
    };
    std::copy(initial.begin(),initial.end(),state.data.begin()+256);
    state.end=256+initial.size();
    return state;
}
std::optional<AcPendingCommands> make_bink_ac_pending(
    std::span<const std::uint8_t> commands) noexcept {
    AcPendingCommands out{};
    if(commands.size()>AcPendingCommands::capacity-out.end)
        return std::nullopt;
    std::copy(commands.begin(),commands.end(),out.data.begin()+256);
    out.end+=commands.size();
    return out;
}
std::optional<AcCommandPassResult> run_bink_ac_command_pass(
    LsbBitReader& compressed, AcPendingCommands& controls,
    std::span<std::int16_t> coefficients,unsigned low_magnitude_bits,
    std::uint16_t high_magnitude_bit) noexcept {
    if(coefficients.size()!=64 ||
       controls.head>256 || controls.head>controls.end ||
       controls.end>AcPendingCommands::capacity ||
       low_magnitude_bits>31 || !high_magnitude_bit ||
       (high_magnitude_bit&(high_magnitude_bit-1))!=0)
        return std::nullopt;
    auto cursor=compressed;
    auto local=controls;
    std::array<std::int16_t,64> values{};
    std::copy(coefficients.begin(),coefficients.end(),values.begin());
    const auto first_bit=cursor.bit_position();
    AcCommandPassResult result{};
    std::size_t current=local.head;
    // The x86 has an array inside a bounded local stack frame. Explicit
    // capacity and work limit prevent malicious streams from expanding
    // the original command list unboundedly.
    std::size_t operations=0;
    while(current<local.end) {
        if(++operations>4096)return std::nullopt;
        auto& code=local.data[current];
        if(!code) {++current;continue;}
        const auto activated=cursor.read(1);
        if(!activated)return std::nullopt;
        ++result.activation_reads;
        if(!*activated) {++current;continue;}
        switch(code&3U) {
        case 0:
        case 2: {
            // 30011C52/30011C93 both use the 30011C9E group-of-four
            // body, but case 0 retains a rewritten source command,
            // whereas case 2 clears it and moves to the next byte.
            const bool type_zero=(code&3U)==0;
            const auto group=type_zero?
                decode_bink_ac_group4_0(
                    cursor,code,low_magnitude_bits,high_magnitude_bit,values):
                decode_bink_ac_group4_2(
                    cursor,code,low_magnitude_bits,high_magnitude_bit,values);
            if(!group || group->deferred_count>local.head)
                return std::nullopt;
            local.head-=group->deferred_count;
            std::copy_n(group->deferred_commands.begin(),
                        group->deferred_count,local.data.begin()+local.head);
            result.written_coefficients+=group->written_count;
            result.deferred_commands+=group->deferred_count;
            if(!type_zero)++current;
            break;
        }
        case 1: {
            // 30011C61: rewrite current control to base+2, append
            // base+12h, +22h, +32h beyond the original end pointer.
            const auto expanded=expand_bink_ac_control1(code);
            if(!expanded || local.end>local.data.size()-3)
                return std::nullopt;
            code=(*expanded)[0];
            for(unsigned i=1;i<4;++i)
                local.data[local.end++]=(*expanded)[i];
            result.expanded_commands+=3;
            // 30011C8E returns to 3001200A without advancing current.
            break;
        }
        case 3: {
            const auto one=decode_bink_ac_direct3(
                cursor,code,low_magnitude_bits,high_magnitude_bit,values);
            if(!one)return std::nullopt;
            ++result.written_coefficients;
            ++current;
            break;
        }
        }
    }
    result.bit_count=static_cast<std::uint32_t>(
        cursor.bit_position()-first_bit);
    for(std::size_t i=local.head;i<local.end;++i)
        result.remaining_commands+=local.data[i]!=0;
    std::copy(values.begin(),values.end(),coefficients.begin());
    controls=local;
    compressed=cursor;
    return result;
}
} // namespace btb::bink32_recovered
