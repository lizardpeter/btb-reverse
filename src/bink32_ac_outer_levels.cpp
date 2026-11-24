#include "btb/bink32_ac_outer_levels.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::bink32_recovered {
std::optional<AcOuterLevelResult> decode_bink_ac_outer_levels(
    LsbBitReader& compressed,
    std::span<std::int16_t> output_coefficients) noexcept {
    if(output_coefficients.size()!=64)return std::nullopt;
    auto bits=compressed;
    const auto begin=bits.bit_position();
    // Original 30011B4C..30011B80 extracts a four-bit AC mode.
    const auto encoded_mode=bits.read(4);
    if(!encoded_mode)return std::nullopt;
    const auto mode=static_cast<std::uint8_t>(*encoded_mode);
    std::array<std::int16_t,64> output{};
    // Source clears 63 AC values at 30011B3D..30011B48,
    // while caller supplies the already reconstructed DC.
    output[0]=output_coefficients[0];
    auto pending=initial_bink_ac_pending();
    AcOuterLevelResult summary{};
    summary.mode=mode;
    if(mode>=2) {
        // Original 2002 x86 instructions:
        // 30011B9A mov eax, 1 ; 30011BB8 shl eax, cl (cl=mode-1)
        // 30011BD5 mov [esp+28], eax: class bit = 1 << (mode-1).
        // 30011BE8 mov al, bl: EAX is then RESET TO MODE.
        // 30011BF4 dec eax before each magnitude-class pass:
        // widths therefore mode-1, mode-2, ..., 1 (NOT 2^(mode-1)-1).
        // 30012020 sar word [esp+28],1; 30012025 dec ecx
        // counts exactly mode-1 passes. All 4-bit modes (0..15) have
        // read widths <=14, so no x86 shift-count rollover occurs.
        std::uint16_t class_bit=static_cast<std::uint16_t>(
            1U<<(mode-1));
        unsigned low_count=mode;
        for(unsigned i=0;i<static_cast<unsigned>(mode-1);++i) {
            --low_count;
            const auto pass=run_bink_ac_command_pass(
                bits,pending,output,low_count,class_bit);
            if(!pass)return std::nullopt;
            summary.written_coefficients+=pass->written_coefficients;
            summary.activations+=pass->activation_reads;
            ++summary.magnitude_passes;
            class_bit=static_cast<std::uint16_t>(class_bit>>1);
        }
    }
    if(mode>0) {
        // 30012030..300122E7 is the final signed +/-1 pass.
        const auto last=run_bink_ac_unit_pass(bits,pending,output);
        if(!last)return std::nullopt;
        summary.written_coefficients+=last->written_coefficients;
        summary.activations+=last->activations;
    }
    summary.pending_controls=static_cast<std::uint32_t>(
        pending.pending().size());
    summary.consumed_bits=static_cast<std::uint32_t>(
        bits.bit_position()-begin);
    std::copy(output.begin(),output.end(),output_coefficients.begin());
    compressed=bits;
    return summary;
}
} // namespace btb::bink32_recovered
