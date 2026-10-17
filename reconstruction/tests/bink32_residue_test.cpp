#include "btb/bink32_residue.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink residue test %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct TestBits {
    std::vector<std::uint8_t> bytes{};
    unsigned bits{};
    void put(unsigned value,unsigned width) {
        for(unsigned i=0;i<width;++i) {
            if(bytes.size()<=bits/8)bytes.push_back(0);
            if(value&(1U<<i))bytes[bits/8]|=
                static_cast<std::uint8_t>(1U<<(bits&7));
            ++bits;
        }
    }
};
int main() {
    {
        // 7-bit quota zero, 3-bit level zero, four root nodes
        // inactive: the entire 8x8 signed residue stays zero.
        TestBits data;
        data.put(0,7);data.put(0,3);
        for(unsigned i=0;i<4;++i)data.put(0,1);
        LsbBitReader bits(data.bytes);
        const auto r=decode_bink_residue_packet(bits);
        CHECK(r && r->consumed_bits==14 && bits.bit_position()==14);
        CHECK(std::all_of(r->coefficients.begin(),
                          r->coefficients.end(),[](auto v){return v==0;}));
        CHECK(r->coefficient_events==0 && r->refinement_events==0);
    }
    for(unsigned sign=0;sign<2;++sign) {
        // One immediate coefficient at scan[4]=2. Quota reaches
        // -1 after that one event, so no trailing bits are consumed.
        TestBits encoded;
        encoded.put(0,7);encoded.put(0,3);
        encoded.put(1,1); // activate first four-coefficient group
        encoded.put(0,1); // first coefficient emitted now
        encoded.put(sign,1);
        LsbBitReader bits(encoded.bytes);
        const auto result=decode_bink_residue_packet(bits);
        CHECK(result && result->coefficients[2]==(sign?-1:1));
        CHECK(result->coefficient_events==1);
        CHECK(result->consumed_bits==13 && bits.bit_position()==13);
        for(unsigned i=0;i<64;++i)
            if(i!=2)CHECK(result->coefficients[i]==0);
    }
    {
        // Two coefficient events: materialize +2 at level one,
        // then refine that same positive coefficient to +3 at
        // level zero. This exercises the nonzero list and quota.
        TestBits data;
        data.put(1,7); // allow exactly two successful events
        data.put(1,3); // start bit plane 2
        data.put(1,1); // expand first root
        data.put(0,1);data.put(0,1); // immediate positive +2
        data.put(1,1);data.put(1,1);data.put(1,1);
        data.put(0,1);data.put(0,1);data.put(0,1);data.put(0,1); // skip current and other roots
        data.put(1,1); // refine positive +2 to +3
        LsbBitReader bits(data.bytes);
        const auto r=decode_bink_residue_packet(bits);
        CHECK(r && r->coefficients[2]==3);
        CHECK(r->coefficient_events==1 && r->refinement_events==1);
        CHECK(r->consumed_bits==21 && bits.bit_position()==21);
    }
    {
        // Caller-supplied budget is the same representation as
        // original 0x3000E400 after the initial seven bits.
        TestBits raw;
        raw.put(0,3);
        for(unsigned i=0;i<4;++i)raw.put(0,1);
        LsbBitReader reader(raw.bytes);
        const auto result=decode_bink_residue_coefficients(reader,0);
        CHECK(result && result->consumed_bits==7);
        CHECK(reader.bit_position()==7);
        LsbBitReader invalid(raw.bytes);
        CHECK(!decode_bink_residue_coefficients(invalid,128));
        CHECK(invalid.bit_position()==0);
    }
    {
        // Even a partial three-bit mask header is rejected without
        // committing a bitreader position.
        std::array<std::uint8_t,1> truncated{0};
        LsbBitReader bits(truncated);
        CHECK(!decode_bink_residue_packet(bits));
        CHECK(bits.bit_position()==0);
    }
    std::puts("Source-native Bink residue coefficients: PASS");
}
