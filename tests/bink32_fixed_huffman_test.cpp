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
    // Retail designstartup.bik frame 24 U plane, final bundle:
    // selector 8 needs six bits for table indexing, but its last
    // genuine symbol uses only two bits and three packet bits remain.
    // Missing LOOKAHEAD can be zero-extended, not the consumed code.
    const std::array<std::uint8_t,1> packed_tail{0x40};
    LsbBitReader tail(packed_tail);
    CHECK(tail.skip(6));
    const auto short_leaf=read_fixed_huffman_symbol(tail,8);
    CHECK(short_leaf && short_leaf->value==1 &&
          short_leaf->consumed_bits==2 && tail.bit_position()==8);
    // Only one real bit remains, but this particular selector-eight
    // prefix requires two; do not fabricate any consumed bits.
    const std::array<std::uint8_t,1> one_bit{0x80};
    LsbBitReader truncated(one_bit);
    CHECK(truncated.skip(7));
    CHECK(!read_fixed_huffman_symbol(truncated,8));
    CHECK(truncated.bit_position()==7);
    // Selector 0 always consumes four bits, so a two-bit tail
    // must still reject even though its lookup can be padded.
    LsbBitReader fixed_width(packed_tail);
    CHECK(fixed_width.skip(6));
    CHECK(!read_fixed_huffman_symbol(fixed_width,0));
    CHECK(fixed_width.bit_position()==6);
    std::puts("Original 2002 Bink fixed Huffman lookup tables: PASS");
}