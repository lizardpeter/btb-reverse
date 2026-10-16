#include "btb/bink32_ac_group4.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink AC group4 test line %d: %s\n",__LINE__,#x);std::abort();}}while(0)

struct Writer {
    std::array<std::uint8_t,8> data{};unsigned count{};
    void write(unsigned value,unsigned width){
        for(unsigned i=0;i<width;++i){
            if((value>>i)&1U)data[count/8]|=std::uint8_t(1U<<(count%8));
            ++count;
        }
    }
    LsbBitReader reader()const{
        return LsbBitReader(std::span<const std::uint8_t>(data).first((count+7)/8));
    }
};
int main() {
    {
        Writer encoded;
        encoded.write(0,1);encoded.write(3,3);encoded.write(0,1); // 20 -> 11
        encoded.write(1,1);                                       // defer 21
        encoded.write(0,1);encoded.write(5,3);encoded.write(1,1); // 22 -> -13
        encoded.write(1,1);                                       // defer 23
        auto bits=encoded.reader();
        std::array<std::int16_t,64> coeff{};coeff.fill(4);
        std::uint8_t ctrl=std::uint8_t((20U<<2)|2U);
        const auto group=decode_bink_ac_group4_2(bits,ctrl,3,8,coeff);
        CHECK(group&&group->written_count==2&&group->deferred_count==2);
        CHECK(group->deferred_commands[0]==((23<<2)|3U));
        CHECK(group->deferred_commands[1]==((21<<2)|3U));
        CHECK(group->consumed_bits==encoded.count&&bits.bit_position()==encoded.count);
        CHECK(ctrl==0&&coeff[20]==11&&coeff[21]==4&&coeff[22]==-13&&coeff[23]==4);
        CHECK(coeff[19]==4&&coeff[24]==4);
    }
    {
        Writer pending;for(unsigned j=0;j<4;++j)pending.write(1,1);
        auto bits=pending.reader();
        std::array<std::int16_t,64> coeff{};coeff.fill(11);
        std::uint8_t ctrl=2;
        const auto group=decode_bink_ac_group4_2(bits,ctrl,0,1,coeff);
        CHECK(group&&group->deferred_count==4&&group->written_count==0);
        CHECK(group->deferred_commands[0]==15&&group->deferred_commands[3]==3);
        CHECK(bits.bit_position()==4&&ctrl==0);
        for(auto x:coeff)CHECK(x==11);
    }
    {
        std::array<std::uint8_t,1> truncated{0};
        LsbBitReader bits(truncated);
        std::array<std::int16_t,64> coeff{};coeff.fill(27);
        std::uint8_t ctrl=2;
        CHECK(!decode_bink_ac_group4_2(bits,ctrl,8,256,coeff));
        CHECK(bits.bit_position()==0&&ctrl==2);
        for(auto x:coeff)CHECK(x==27);
        ctrl=(61U<<2)|2U;
        CHECK(!decode_bink_ac_group4_2(bits,ctrl,0,1,coeff));
        CHECK(bits.bit_position()==0&&ctrl==((61U<<2)|2U));
        ctrl=3;
        CHECK(!decode_bink_ac_group4_2(bits,ctrl,0,1,coeff));
        CHECK(bits.bit_position()==0&&ctrl==3);
    }
    std::puts("Bink original AC group-of-four coefficient/control decoding: PASS");
}
