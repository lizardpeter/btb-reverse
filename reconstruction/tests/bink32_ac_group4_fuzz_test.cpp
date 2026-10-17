#include "btb/bink32_ac_group4.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
using namespace btb::bink32_recovered;
static std::uint32_t state=0x72d9bf39U;
static std::uint32_t next_random(){
    state^=state<<13;state^=state>>17;state^=state<<5;return state;
}
#define REQUIRE(x) do {if(!(x)) {std::fprintf(stderr,"AC input invariant at %d\n",__LINE__);std::abort();}}while(0)
int main(){
    // Deterministic stress-test for malformed original opcode-0 and
    // opcode-2 compressed streams. This is not retail frame parity.
    for(int test=0;test<50000;++test) {
        std::array<std::uint8_t,16> bytes{};
        for(auto& b:bytes)b=std::uint8_t(next_random());
        const unsigned size=next_random()%17;
        LsbBitReader stream(std::span<const std::uint8_t>(bytes).first(size));
        std::array<std::int16_t,64> coeff{};
        for(auto& v:coeff)v=static_cast<std::int16_t>(
            static_cast<int>(next_random()%65535U)-32767);
        const auto previous_coefficients=coeff;
        std::uint8_t command=static_cast<std::uint8_t>(next_random());
        const auto previous_command=command;
        const unsigned width=next_random()%18U;
        const auto high=static_cast<std::uint16_t>(next_random());
        const bool mode0=(next_random()%2U)!=0;
        const auto outcome=mode0?
            decode_bink_ac_group4_0(stream,command,width,high,coeff):
            decode_bink_ac_group4_2(stream,command,width,high,coeff);
        if(!outcome) {
            REQUIRE(stream.bit_position()==0);
            REQUIRE(command==previous_command);
            REQUIRE(coeff==previous_coefficients);
        } else {
            REQUIRE(outcome->written_count+outcome->deferred_count==4);
            REQUIRE(outcome->consumed_bits==stream.bit_position());
            REQUIRE(stream.bit_position()<=size*8);
            REQUIRE(mode0?(previous_command&3U)==0U:
                           (previous_command&3U)==2U);
            REQUIRE(mode0?command==static_cast<std::uint8_t>(
                (previous_command&0xfcU)+0x11U):command==0);
        }
    }
    std::puts("Bink original AC dispatch: 50,000 input invariants PASS");
}
