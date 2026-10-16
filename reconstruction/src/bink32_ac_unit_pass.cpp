#include "btb/bink32_ac_unit_pass.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
std::optional<AcUnitPassResult> run_bink_ac_unit_pass(
    LsbBitReader& compressed,AcPendingCommands& controls,
    std::span<std::int16_t> coefficients) noexcept {
    if(coefficients.size()!=64 || controls.head>256 ||
       controls.head>controls.end ||
       controls.end>AcPendingCommands::capacity)
        return std::nullopt;
    auto cursor=compressed;
    auto local=controls;
    std::array<std::int16_t,64> values{};
    std::copy(coefficients.begin(),coefficients.end(),values.begin());
    AcUnitPassResult stats{};
    const auto first_bit=cursor.bit_position();
    auto current=local.head;
    std::size_t operations=0;
    while(current<local.end) {
        if(++operations>4096)return std::nullopt;
        auto& code=local.data[current];
        if(!code) {++current;continue;}
        const auto activation=cursor.read(1);
        if(!activation)return std::nullopt;
        ++stats.activations;
        if(!*activation){++current;continue;}
        const auto op=code&3U;
        const auto first=static_cast<unsigned>(code>>2);
        if(op==1) {
            const auto expanded=expand_bink_ac_control1(code);
            if(!expanded || local.end>local.data.size()-3)
                return std::nullopt;
            code=(*expanded)[0];
            for(unsigned i=1;i<4;++i)
                local.data[local.end++]=(*expanded)[i];
            stats.expanded_commands+=3;
            // 300120D6 -> 300122DF: do not advance current.
            continue;
        }
        if(op==3) {
            // 3001229D..300122D3:
            // The second bit supplies the sign; x86 constructs +1/-1.
            if(first>=values.size())return std::nullopt;
            const auto sign=cursor.read(1);
            if(!sign)return std::nullopt;
            values[first]=*sign?-1:1;
            code=0;
            ++stats.written_coefficients;
            ++current;
            continue;
        }
        // Opcode 0 rewrites current byte before the shared four-slot
        // body; opcode 2 zeroes current byte and advances its pointer.
        if(first>60)return std::nullopt;
        const bool keep_current=op==0;
        if(keep_current)
            code=static_cast<std::uint8_t>((code&0xfcU)+0x11U);
        else
            code=0;
        if(!keep_current)++current;
        // 300120E6..3001229B: a zero decision emits +/-1 using a
        // second sign bit. A one decision prepends an opcode 3 for
        // a later pass. Thus all writes and deferred bytes are bounded.
        for(unsigned i=0;i<4;++i) {
            const auto defer=cursor.read(1);
            if(!defer)return std::nullopt;
            const auto slot=first+i;
            if(*defer) {
                if(!local.head)return std::nullopt;
                local.data[--local.head]=static_cast<std::uint8_t>(
                    (slot<<2)|3U);
                ++stats.deferred_commands;
            } else {
                const auto sign=cursor.read(1);
                if(!sign)return std::nullopt;
                values[slot]=*sign?-1:1;
                ++stats.written_coefficients;
            }
        }
    }
    stats.consumed_bits=static_cast<std::uint32_t>(
        cursor.bit_position()-first_bit);
    for(std::size_t i=local.head;i<local.end;++i)
        stats.remaining_commands+=local.data[i]!=0;
    std::copy(values.begin(),values.end(),coefficients.begin());
    controls=local;
    compressed=cursor;
    return stats;
}
} // namespace btb::bink32_recovered
