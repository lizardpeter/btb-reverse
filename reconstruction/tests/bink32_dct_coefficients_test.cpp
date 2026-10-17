#include "btb/bink32_dct_coefficients.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink DCT coefficient test %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct TestBits {
    std::vector<std::uint8_t> bytes{};
    std::size_t count{};
    void put(unsigned value,unsigned width) {
        for(unsigned i=0;i<width;++i) {
            if(bytes.size()<=count/8)bytes.push_back(0);
            if(value&(1U<<i))bytes[count/8]|=
                static_cast<std::uint8_t>(1U<<(count&7));
            ++count;
        }
    }
};
int main() {
    {
        // The Bink intra/inter DC value is supplied from its own
        // bundle. An AC plane header of zero jumps to 4-bit quant.
        TestBits encoded;
        encoded.put(0,4);encoded.put(13,4);
        LsbBitReader reader(encoded.bytes);
        const auto block=decode_bink_dct_coefficients(reader,-17);
        CHECK(block && block->values[0]==-17);
        CHECK(block->quant_selector==13 && block->magnitude_planes==0);
        CHECK(block->changed_count==0 && block->consumed_bits==8);
        CHECK(reader.bit_position()==8);
        CHECK(std::all_of(block->values.begin()+1,block->values.end(),
                          [](auto v){return v==0;}));
        LsbBitReader override_bits(encoded.bytes);
        const auto overridden=decode_bink_dct_coefficients(
            override_bits,53,static_cast<std::uint8_t>(7));
        CHECK(overridden && overridden->values[0]==53);
        CHECK(overridden->quant_selector==7 && override_bits.bit_position()==4);
    }
    for(unsigned negative=0;negative<2;++negative) {
        // Root groups at scan 4, 24, 44 are all disabled.
        // The explicit scan-index-1 leaf emits signed coefficient
        // +1 or -1; scan 2 and scan 3 remain disabled.
        TestBits encoded;encoded.put(1,4);
        encoded.put(0,1);encoded.put(0,1);encoded.put(0,1);
        encoded.put(1,1);encoded.put(negative,1);
        encoded.put(0,1);encoded.put(0,1);
        encoded.put(9,4);
        LsbBitReader reader(encoded.bytes);
        const auto b=decode_bink_dct_coefficients(reader,123);
        CHECK(b && b->values[0]==123);
        CHECK(b->values[1]==(negative?-1:1));
        CHECK(b->changed_count==1);
        CHECK(b->changed_scan_indices[0]==1);
        CHECK(b->magnitude_planes==1 && b->quant_selector==9);
        CHECK(b->consumed_bits==15 && reader.bit_position()==15);
    }
    {
        // Two magnitude planes. At the higher plane, scan-index 1
        // gets a signed magnitude 3. Its node then retires; all
        // remaining nodes are disabled at the lower bit plane.
        TestBits encoded;encoded.put(2,4);
        for(unsigned i=0;i<3;++i)encoded.put(0,1);
        encoded.put(1,1);encoded.put(1,1);encoded.put(1,1);
        encoded.put(0,1);encoded.put(0,1);
        for(unsigned i=0;i<5;++i)encoded.put(0,1);
        encoded.put(4,4);
        LsbBitReader reader(encoded.bytes);
        const auto b=decode_bink_dct_coefficients(reader,7);
        CHECK(b && b->values[1]==-3);
        CHECK(b->changed_count==1 && b->quant_selector==4);
        CHECK(b->consumed_bits==21 && reader.bit_position()==21);
    }
    {
        // Recover a complete group of four consecutive scan
        // indices through the original quadtree mode-zero path.
        // Original scan indices 4,5,6,7 map to raster 2,3,10,11.
        TestBits encoded;encoded.put(1,4);
        encoded.put(1,1); // first root is active
        for(unsigned i=0;i<4;++i) {
            encoded.put(0,1); // emit now instead of defer
            encoded.put(i&1U,1); // alternating signs
        }
        for(unsigned i=0;i<6;++i)encoded.put(0,1);
        encoded.put(2,4);
        LsbBitReader reader(encoded.bytes);
        const auto b=decode_bink_dct_coefficients(reader,99);
        CHECK(b && b->changed_count==4);
        CHECK(b->values[2]==1 && b->values[3]==-1);
        CHECK(b->values[10]==1 && b->values[11]==-1);
        CHECK(b->changed_scan_indices[0]==4);
        CHECK(b->changed_scan_indices[3]==7);
        CHECK(b->consumed_bits==23 && reader.bit_position()==23);
    }
    {
        // Rollback is required even after a valid first nibble.
        std::array<std::uint8_t,1> clipped{1};
        LsbBitReader reader(clipped);
        CHECK(!decode_bink_dct_coefficients(reader,1));
        CHECK(reader.bit_position()==0);
        LsbBitReader invalid(clipped);
        CHECK(!decode_bink_dct_coefficients(
            invalid,1,static_cast<std::uint8_t>(16)));
        CHECK(invalid.bit_position()==0);
    }
    std::puts("Bink1 original signed DCT coefficient tree: PASS");
}
