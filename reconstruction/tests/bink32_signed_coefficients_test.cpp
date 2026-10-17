#include "btb/bink32_signed_coefficients.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"signed coefficients at line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct Writer {
    std::vector<std::uint8_t> bytes{};
    unsigned count{};
    void put(unsigned bits,unsigned width){
        for(unsigned i=0;i<width;++i) {
            if(count/8>=bytes.size())bytes.push_back(0);
            if((bits>>i)&1U)bytes[count/8]|=std::uint8_t(1U<<(count%8));
            ++count;
        }
    }
};
static DynamicHuffmanTree identity() {
    DynamicHuffmanTree out{};out.selector=0;
    for(unsigned i=0;i<16;++i)out.symbol_permutation[i]=std::uint8_t(i);
    return out;
}
static void explicit_value(Writer& w,unsigned magnitude,bool negative){
    w.put(0,1);w.put(magnitude,4);if(magnitude)w.put(negative,1);
}
static void fill(Writer& w,unsigned magnitude,bool negative){
    w.put(1,1);w.put(magnitude,4);if(magnitude)w.put(negative,1);
}
int main() {
    const auto tree=identity();
    Writer encoded;
    explicit_value(encoded,6,true);
    explicit_value(encoded,0,false);
    explicit_value(encoded,15,false);
    fill(encoded,7,true);
    std::array<std::int8_t,7> samples{};
    LsbBitReader bits(encoded.bytes);
    const auto result=decode_bink_signed_coefficients(bits,tree,samples);
    constexpr std::array<std::int8_t,7> expected{-6,0,15,-7,-7,-7,-7};
    CHECK(result&&result->explicit_coefficients==3&&result->filled_remainder);
    CHECK(result->bits_consumed==encoded.count&&bits.bit_position()==encoded.count);
    CHECK(samples==expected);
    Writer all;
    for(unsigned v=0;v<16;++v)explicit_value(all,v,v%2!=0);
    std::array<std::int8_t,16> all_samples{};
    LsbBitReader many(all.bytes);
    const auto complete=decode_bink_signed_coefficients(many,tree,all_samples);
    CHECK(complete&&!complete->filled_remainder&&complete->explicit_coefficients==16);
    for(unsigned i=0;i<16;++i)
        CHECK(all_samples[i]==(i%2?-int(i):int(i)));
    CHECK(many.bit_position()==all.count);
    {
        // The fill opcode's zero nibble must consume no sign bit.
        Writer zero;fill(zero,0,true);
        std::array<std::int8_t,18> sample{};
        LsbBitReader r(zero.bytes);
        const auto v=decode_bink_signed_coefficients(r,tree,sample);
        CHECK(v&&v->bits_consumed==5&&v->filled_remainder);
        for(auto x:sample)CHECK(x==0);
    }
    {
        // Deliberately truncate the branch's magnitude sign-bit.
        // One-byte bit reader ends at 8 bits: after a 4-bit prefix,
        // a 4-bit zero nibble can test no-sign mode only.
        Writer bad;explicit_value(bad,6,true);
        std::array<std::int8_t,1> sample{99};
        LsbBitReader empty{std::span<const std::uint8_t>{}};
        CHECK(!decode_bink_signed_coefficients(empty,tree,sample));
        CHECK(empty.bit_position()==0&&sample[0]==99);
        std::array<std::uint8_t,1> one{0xff};
        LsbBitReader advance(one);
        CHECK(advance.skip(5));
        CHECK(!decode_bink_signed_coefficients(advance,tree,sample));
        CHECK(advance.bit_position()==5&&sample[0]==99);
        // Output and compressed cursor stay unchanged on oversized repeat.
        std::vector<std::int8_t> too_many(2,99);
        CHECK(!decode_bink_signed_coefficients(empty,tree,too_many));
        CHECK(too_many[0]==99&&too_many[1]==99);
    }
    {
        Writer value;fill(value,9,true);
        std::vector<std::int8_t> large(8192);
        LsbBitReader r(value.bytes);
        const auto v=decode_bink_signed_coefficients(r,tree,large);
        CHECK(v&&v->bits_consumed==6);
        for(auto x:large)CHECK(x==-9);
    }
    std::puts("Bink 0x30010980 signed coefficient reconstruction: PASS");
}
