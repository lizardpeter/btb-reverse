#include "btb/bink32_packet_decode.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
using namespace btb::bink32_recovered;
#define CHECK(x) do { if(!(x)) { \
    std::fprintf(stderr,"Bink Huffman test line %d: %s\\n",__LINE__,#x); \
    std::abort(); } } while(0)
int main() {
    // Exhaust every prefix of each of the 16 original fixed Huffman LUTs.
    const std::array<unsigned,16> widths{
        4,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7
    };
    std::uint32_t tested=0;
    for(unsigned id=0;id<16;++id) {
        for(unsigned p=0;p<(1U<<widths[id]);++p) {
            const std::array<std::uint8_t,2> bytes{
                static_cast<std::uint8_t>(p),0};
            LsbBitReader r(bytes);
            const auto symbol=read_fixed_huffman_symbol(r,id);
            CHECK(symbol&&symbol->value<16);
            CHECK(symbol->consumed_bits>0);
            CHECK(symbol->consumed_bits<=widths[id]);
            CHECK(r.bit_position()==symbol->consumed_bits);
            if(id==0)CHECK(symbol->consumed_bits==4&&symbol->value==p);
            ++tested;
        }
    }
    CHECK(tested==976);
    const std::array<std::uint8_t,4> zeros{0,0,0,0};
    LsbBitReader invalid(zeros);
    CHECK(!read_fixed_huffman_symbol(invalid,16));
    CHECK(invalid.bit_position()==0);
    LsbBitReader short_data({});
    CHECK(!read_fixed_huffman_symbol(short_data,0));
    LsbBitReader fixed_1(zeros);
    const auto leaf=read_fixed_huffman_symbol(fixed_1,1);
    CHECK(leaf&&leaf->consumed_bits==1&&leaf->value==0);
    std::puts("Original 2002 Bink fixed Huffman lookup tables: PASS");
}