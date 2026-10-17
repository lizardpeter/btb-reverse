#include "btb/bink32_ac_block_pixels.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
using namespace btb::bink32_recovered;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"Bink AC->pixels test line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct Writer {
    std::array<std::uint8_t,64> bytes{};
    std::size_t bits{};
    void put(unsigned value,unsigned n) {
        for(unsigned i=0;i<n;++i){
            CHECK(bits<bytes.size()*8);
            if((value>>i)&1U)
                bytes[bits/8]|=static_cast<std::uint8_t>(1U<<(bits%8));
            ++bits;
        }
    }
    LsbBitReader reader() const {
        return LsbBitReader(std::span<const std::uint8_t>(bytes)
                            .first((bits+7)/8));
    }
};
int main() {
    // First verified all-mode integration: original AC mode nibble,
    // original pending-command seed (all six inactive in every pass),
    // four-bit quant selector, and original 16x16 quantized IDCT.
    for(unsigned mode=0;mode<=15;++mode) {
        Writer w;w.put(mode,4);
        for(unsigned i=0;i<mode*6;++i)w.put(0,1);
        w.put(mode,4);
        auto compressed=w.reader();
        const auto block=decode_bink_ac_quantized_block(compressed,128);
        CHECK(block&&block->ac.mode==mode);
        CHECK(block->ac.magnitude_passes==(mode?mode-1:0));
        CHECK(block->quant_selector==mode&&block->coefficients[0]==128);
        for(unsigned i=1;i<64;++i)CHECK(block->coefficients[i]==0);
        const auto expected=bink_idct_original_quant(block->coefficients,
            static_cast<std::uint8_t>(mode));
        CHECK(expected&&block->pixels==*expected);
        CHECK(block->consumed_bits==w.bits);
        CHECK(compressed.bit_position()==w.bits);
    }
    {
        // Mode 0 has no coefficient passes. Matrix0 and caller's
        // existing DC128 output a uniform original 16x16 sample16.
        const std::array<std::uint8_t,1> bits{0x00};
        LsbBitReader compressed(bits);
        const auto block=decode_bink_ac_quantized_block(compressed,128);
        CHECK(block&&block->consumed_bits==8);
        for(const auto sample:block->pixels)CHECK(sample==16);
    }
    {
        // Mode1: six activation decisions in the final +/-1 pass.
        // Command index 3 activates, reads negative sign, then select
        // original quant table 1. Nonzero AC reaches pixel output.
        Writer w;w.put(1,4);w.put(0,5);w.put(1,1);w.put(1,1);w.put(1,4);
        auto compressed=w.reader();
        const auto block=decode_bink_ac_quantized_block(compressed,128);
        CHECK(block&&block->coefficients[3]==-1);
        CHECK(block->ac.written_coefficients==1);
        CHECK(block->consumed_bits==w.bits&&w.bits==15);
        const auto expected=bink_idct_original_quant(block->coefficients,1);
        CHECK(expected&&block->pixels==*expected);
    }
    {
        // Mode4 first pass: 3 low bits, class-bit8. One explicit
        // value13 at coeff[3]; all subsequent controls inactive.
        Writer w;w.put(4,4);
        w.put(0,5);w.put(1,1);w.put(5,3);w.put(0,1);
        w.put(0,5);w.put(0,5);w.put(0,5);
        w.put(15,4);
        auto compressed=w.reader();
        const auto block=decode_bink_ac_quantized_block(compressed,64);
        CHECK(block&&block->ac.mode==4&&block->ac.magnitude_passes==3);
        CHECK(block->ac.written_coefficients==1&&block->coefficients[3]==13);
        CHECK(block->quant_selector==15&&block->consumed_bits==33);
        CHECK(compressed.bit_position()==33);
        const auto expected=bink_idct_original_quant(block->coefficients,15);
        CHECK(expected&&block->pixels==*expected);
    }
    {
        // Mode 15 exercises 14 descending magnitude read widths.
        // First pass: low0x1234, high0x4000, negative. Then 13
        // magnitude passes and the signed-unit pass remain inactive.
        Writer w;w.put(15,4);
        w.put(0,5);w.put(1,1);w.put(0x1234,14);w.put(1,1);
        for(unsigned k=1;k<14;++k)w.put(0,5);
        w.put(0,5);w.put(14,4);
        auto compressed=w.reader();
        const auto block=decode_bink_ac_quantized_block(compressed,-29);
        CHECK(block&&block->ac.mode==15&&block->ac.magnitude_passes==14);
        CHECK(block->ac.written_coefficients==1);
        CHECK(block->coefficients[0]==-29);
        CHECK(block->coefficients[3]==-static_cast<int>(0x4000|0x1234));
        CHECK(block->quant_selector==14);
        CHECK(block->consumed_bits==99&&compressed.bit_position()==99);
        const auto expected=bink_idct_original_quant(block->coefficients,14);
        CHECK(expected&&block->pixels==*expected);
    }
    {
        // Truncated AC and truncated quantizer must not consume the
        // caller's input cursor.
        const std::array<std::uint8_t,1> short_input{0x0F};
        LsbBitReader truncated_ac(short_input);
        CHECK(!decode_bink_ac_quantized_block(truncated_ac,123));
        CHECK(truncated_ac.bit_position()==0);
        const std::array<std::uint8_t,1> bits{0};
        LsbBitReader truncated_quant(bits);
        CHECK(truncated_quant.skip(1));
        CHECK(!decode_bink_ac_quantized_block(truncated_quant,123));
        CHECK(truncated_quant.bit_position()==1);
    }
    std::puts("Bink AC mode 0-15 -> quant matrices -> 16x16 native pixels: PASS");
}
