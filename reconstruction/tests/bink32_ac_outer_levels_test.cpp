#include "btb/bink32_ac_outer_levels.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
using namespace btb::bink32_recovered;
#define CHECK(c) do{if(!(c)){std::fprintf(stderr,"Bink outer AC at line %d: %s\n",__LINE__,#c);std::abort();}}while(0)
struct Writer {
    std::array<std::uint8_t,32> bytes{};std::size_t count{};
    void put(unsigned value,unsigned bits){
        for(unsigned i=0;i<bits;++i) {
            if((value>>i)&1U)bytes[count/8]|=std::uint8_t(1U<<(count%8));
            ++count;
        }
    }
    LsbBitReader reader()const{
        return LsbBitReader(std::span<const std::uint8_t>(bytes).first((count+7)/8));
    }
};
int main() {
    {
        // Original nibble 0 bypasses both coefficient stages.
        Writer w;w.put(0,4);
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};coeff.fill(57);coeff[0]=42;
        const auto r=decode_bink_ac_outer_levels(bits,coeff);
        CHECK(r&&r->mode==0&&r->magnitude_passes==0);
        CHECK(r->consumed_bits==4&&bits.bit_position()==4);
        CHECK(coeff[0]==42);
        for(unsigned i=1;i<64;++i)CHECK(coeff[i]==0);
    }
    // Each level's original command seed contains six commands. Zero
    // activation bits leave every control untouched, so modes 1..6
    // have exactly (mode-1) magnitude passes and one unit pass.
    for(unsigned mode=1;mode<=6;++mode) {
        Writer w;w.put(mode,4);
        for(unsigned i=0;i<mode*6;++i)w.put(0,1);
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};coeff.fill(-80);
        coeff[0]=19;
        const auto r=decode_bink_ac_outer_levels(bits,coeff);
        CHECK(r&&r->mode==mode&&r->magnitude_passes==mode-1);
        CHECK(r->activations==mode*6);
        CHECK(r->consumed_bits==4+mode*6);
        CHECK(bits.bit_position()==4+mode*6);
        CHECK(r->pending_controls==6);
        CHECK(coeff[0]==19);
        for(unsigned i=1;i<64;++i)CHECK(coeff[i]==0);
    }
    {
        // Mode1 has no magnitude passes, but DOES execute the final
        // +/-1 pass. Activate only the sixth seeded direct opcode
        // 0x0F (coefficient index3) and read negative sign.
        Writer w;w.put(1,4);
        w.put(0,5);w.put(1,1);w.put(1,1);
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};
        const auto r=decode_bink_ac_outer_levels(bits,coeff);
        CHECK(r&&r->mode==1&&r->magnitude_passes==0);
        CHECK(r->written_coefficients==1);
        CHECK(coeff[3]==-1&&bits.bit_position()==11);
    }
    {
        // Mode4: first pass has low bit count=7 and class bit=8.
        // It writes coefficient index3 from a 7-bit low magnitude.
        // The next two magnitude passes and unit pass are inactive.
        Writer w;w.put(4,4);
        w.put(0,5);w.put(1,1);w.put(0x52,7);w.put(0,1);
        w.put(0,5);w.put(0,5);w.put(0,5);
        auto bits=w.reader();
        std::array<std::int16_t,64> coeff{};
        const auto r=decode_bink_ac_outer_levels(bits,coeff);
        CHECK(r&&r->mode==4&&r->magnitude_passes==3);
        CHECK(r->written_coefficients==1&&coeff[3]==90);
        CHECK(bits.bit_position()==33&&r->consumed_bits==33);
    }
    {
        // Modes7..15 have EAX shift-width rollover in x86 and
        // are deliberately not supported by this bounded routine.
        for(unsigned mode=7;mode<=15;++mode) {
            Writer w;w.put(mode,4);
            auto bits=w.reader();
            std::array<std::int16_t,64> coeff{};coeff.fill(17);
            CHECK(!decode_bink_ac_outer_levels(bits,coeff));
            CHECK(bits.bit_position()==0);
            for(auto c:coeff)CHECK(c==17);
        }
    }
    {
        // Truncation in a later activation leaves all output
        // coefficients and the original input cursor untouched.
        const std::array<std::uint8_t,1> truncated{1};
        LsbBitReader bits(truncated);
        std::array<std::int16_t,64> coeff{};coeff.fill(25);
        CHECK(!decode_bink_ac_outer_levels(bits,coeff));
        CHECK(bits.bit_position()==0);
        for(auto x:coeff)CHECK(x==25);
        std::array<std::int16_t,63> short_dest{};
        CHECK(!decode_bink_ac_outer_levels(bits,short_dest));
    }
    std::puts("Bink original 4-bit AC mode + magnitude levels 0..6: PASS");
}
