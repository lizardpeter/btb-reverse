#include "btb/bink32_original_quant.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <utility>

using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Original quant table test line %d: %s\n",__LINE__,#x);std::abort();}}while(0)

static std::uint64_t fnv64(std::span<const std::uint8_t> pixels) {
    std::uint64_t h=0xcbf29ce484222325ULL;
    for(const auto b:pixels)h=(h^b)*0x100000001b3ULL;
    return h;
}
int main() {
    CHECK(!original_quant_matrix(16));
    CHECK(!original_quant_matrix(255));
    CHECK(original_quant_matrix(0)&&original_quant_matrix(15));
    const auto q0=*original_quant_matrix(0);
    const auto q1=*original_quant_matrix(1);
    const auto q15=*original_quant_matrix(15);
    CHECK(q0[0]==65536&&q0[1]==96582&&q0[63]==10289);
    CHECK(q1[0]==87381&&q1[1]==128776&&q1[63]==13719);
    CHECK(q15[0]==2883584&&q15[1]==4249624&&q15[63]==452718);
    std::array<std::int16_t,64> coeff{};
    coeff[0]=128;
    for(const auto [selector,expected]:std::array<std::pair<unsigned,unsigned>,3>{{
        {0,16},{1,21},{15,192}
    }}) {
        const auto pixels=bink_idct_original_quant(
            coeff,static_cast<std::uint8_t>(selector));
        CHECK(pixels);
        for(const auto p:*pixels)CHECK(p==expected);
    }
    // Independent fixture produced locally using the 4096 ORIGINAL raw
    // quantization bytes and the existing integer IDCT source, not
    // mathematically regenerated approximation tables.
    for(std::size_t i=0;i<coeff.size();++i)
        coeff[i]=static_cast<std::int16_t>(int((i*79)%511)-255);
    const std::array<std::pair<unsigned,std::uint64_t>,6> fixtures{{
        {0,0x21e608d6d488fe25ULL},
        {1,0x85e302a67a9a0365ULL},
        {2,0x0ebad66f2cfd7c15ULL},
        {7,0x0874636011842009ULL},
        {8,0x2444763eb84a9c0dULL},
        {15,0x086b4c82941ad615ULL}
    }};
    for(const auto& [selector,expected_hash]:fixtures) {
        const auto pixels=bink_idct_original_quant(
            coeff,static_cast<std::uint8_t>(selector));
        CHECK(pixels&&fnv64(*pixels)==expected_hash);
        for(std::size_t y=0;y<16;y+=2)
            for(std::size_t x=0;x<16;x+=2) {
                const auto p=(*pixels)[y*16+x];
                CHECK(p==(*pixels)[y*16+x+1]);
                CHECK(p==(*pixels)[(y+1)*16+x]);
                CHECK(p==(*pixels)[(y+1)*16+x+1]);
            }
    }
    CHECK(!bink_idct_original_quant(
        std::span<const std::int16_t>(coeff).first(63),0));
    CHECK(!bink_idct_original_quant(coeff,16));
    // Original video block caller consumes a four-bit quant index.
    // Exercise two selectors packed into one compressed byte (0xB3).
    const std::array<std::uint8_t,1> compressed{0xb3};
    LsbBitReader quant_bits(compressed);
    const auto first=decode_original_quantized_block(quant_bits,coeff);
    const auto expected3=bink_idct_original_quant(coeff,3);
    CHECK(first&&expected3&&*first==*expected3);
    CHECK(quant_bits.bit_position()==4);
    const auto second=decode_original_quantized_block(quant_bits,coeff);
    const auto expected11=bink_idct_original_quant(coeff,11);
    CHECK(second&&expected11&&*second==*expected11);
    CHECK(quant_bits.bit_position()==8);
    CHECK(!decode_original_quantized_block(quant_bits,coeff));
    CHECK(quant_bits.bit_position()==8);
    LsbBitReader short_bits(compressed);
    CHECK(short_bits.skip(5));
    CHECK(!decode_original_quantized_block(short_bits,coeff));
    CHECK(short_bits.bit_position()==5);
    LsbBitReader invalid_input(compressed);
    CHECK(!decode_original_quantized_block(
        invalid_input,std::span<const std::int16_t>(coeff).first(63)));
    CHECK(invalid_input.bit_position()==0);
    // Original coefficient unpacker 0x30011B10 has a real zero-AC
    // special case; preserve the upstream-decoded DC and consume only
    // [AC-count nibble = 0][quantization nibble].
    std::array<std::int16_t,64> only_dc{};
    only_dc[0]=128;
    for(const auto [selector,expected_pixel]:std::array<std::pair<unsigned,unsigned>,3>{{
        {0,16},{1,21},{15,192}
    }}) {
        const std::array<std::uint8_t,1> packed{
            static_cast<std::uint8_t>(selector<<4)};
        LsbBitReader block_bits(packed);
        const auto block=decode_bink_zero_ac_block(block_bits,128);
        const auto expected=bink_idct_original_quant(
            only_dc,static_cast<std::uint8_t>(selector));
        CHECK(block&&expected&&*block==*expected);
        CHECK(block_bits.bit_position()==8);
        for(const auto p:*block)CHECK(p==expected_pixel);
    }
    {
        const std::array<std::uint8_t,1> nonzero_mode{0x42};
        LsbBitReader rejected(nonzero_mode);
        CHECK(!decode_bink_zero_ac_block(rejected,128));
        CHECK(rejected.bit_position()==0);
        LsbBitReader short_input(nonzero_mode);
        CHECK(short_input.skip(5));
        CHECK(!decode_bink_zero_ac_block(short_input,128));
        CHECK(short_input.bit_position()==5);
    }
    std::puts("Bink original 16 quantization tables + AC iDCT: PASS");
}
