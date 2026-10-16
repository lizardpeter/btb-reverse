#include "btb/bink32_simple_picture.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do {if(!(e)){std::fprintf(stderr,"Bink simple picture test %d: %s\n",__LINE__,#e);std::abort();}}while(0)
struct Bits {
    std::vector<std::uint8_t> bytes{};
    std::size_t bits{};
    void put(unsigned value,unsigned width) {
        for(unsigned i=0;i<width;++i) {
            if(bytes.size()<=bits/8)bytes.push_back(0);
            if((value>>i)&1U)bytes[bits/8]|=
                static_cast<std::uint8_t>(1U<<(bits%8));
            ++bits;
        }
    }
};
static std::vector<std::uint8_t> packet(const Bits& bits) {
    std::vector<std::uint8_t> data(52,0);
    data[0]=48;
    data.insert(data.end(),bits.bytes.begin(),bits.bytes.end());
    // Original next-plane alignment is in 32-bit words.
    while(data.size()%4)data.push_back(0);
    return data;
}
static void trees(Bits& writer) {
    for(unsigned i=0;i<23;++i)writer.put(0,4);
}
int main() {
    // Six real compressed 8x8 block types, spanning TWO rows,
    // decoded with all nine source bundle refills and actual
    // reference pixels. The standard mode set is:
    // row 0 = Skip, Fill, Motion; row 1 = Pattern, Raw, Skip.
    Bits encoded;trees(encoded);
    encoded.put(6,10);encoded.put(0,1);
    for(auto mode:{0U,6U,2U,8U,9U,0U})encoded.put(mode,4);
    encoded.put(0,10); // scaled subtype bundle terminated

    std::array<std::uint8_t,67> colors{};
    colors[0]=77;
    colors[1]=5;
    colors[2]=200;
    for(unsigned i=0;i<64;++i)
        colors[i+3]=static_cast<std::uint8_t>(i*3);
    encoded.put(67,10);encoded.put(0,1);
    for(auto value:colors) {
        encoded.put(value>>4,4); // adaptive high-nibble context
        encoded.put(value&15U,4);
    }
    std::array<std::uint8_t,8> pattern{};
    for(unsigned i=0;i<8;++i)pattern[i]=i&1U?0x55:0xAA;
    encoded.put(8,10);
    for(auto value:pattern) {
        encoded.put(value&15U,4);encoded.put(value>>4,4);
    }
    encoded.put(1,10);encoded.put(1,1);encoded.put(8,4);
    const auto x_sign_bit=encoded.bits;encoded.put(1,1); // -8 x motion
    encoded.put(1,10);encoded.put(1,1);encoded.put(0,4); // zero y
    encoded.put(0,10); // unsigned intra DC: no values
    encoded.put(0,10); // signed inter DC: no values
    encoded.put(0,10); // run bundle: no values
    const auto compressed_bits=encoded.bits;
    auto video=packet(encoded);
    std::vector<std::uint8_t> reference(24*16);
    for(std::size_t i=0;i<reference.size();++i)
        reference[i]=static_cast<std::uint8_t>((i/24)*29+(i%24));
    const auto decoded=decode_bink_simple_picture_luma(
        video,24,16,reference);
    CHECK(decoded);
    CHECK(decoded->image.width==24 && decoded->image.height==16);
    CHECK(decoded->image.pixels.size()==reference.size());
    CHECK(decoded->mode_counts[0]==2 && decoded->mode_counts[2]==1);
    CHECK(decoded->mode_counts[6]==1 && decoded->mode_counts[8]==1);
    CHECK(decoded->mode_counts[9]==1);
    for(auto refills:decoded->source_refills)CHECK(refills==1);
    CHECK(decoded->next_plane_byte_offset==
          52+((compressed_bits+31)/32)*4);
    const auto& px=decoded->image.pixels;
    CHECK(px[0]==reference[0]); // row 0 Skip
    CHECK(px[8]==77 && px[15]==77); // row 0 Fill
    CHECK(px[16]==reference[8]); // row 0 Motion X=-8
    CHECK(px[7*24+23]==reference[7*24+15]);
    CHECK(px[8*24+0]==5 && px[8*24+1]==200); // Pattern
    CHECK(px[9*24+0]==200 && px[9*24+1]==5);
    CHECK(px[8*24+8]==0 && px[8*24+9]==3); // Raw 8x8
    CHECK(px[15*24+15]==static_cast<std::uint8_t>(63*3));
    CHECK(px[8*24+16]==reference[8*24+16]); // row 1 Skip
    CHECK(!decode_bink_simple_picture_luma(video,24,16)); // no prev
    CHECK(!decode_bink_simple_picture_luma(video,24,16,
        std::span<const std::uint8_t>(reference).first(12)));
    CHECK(!decode_bink_simple_picture_luma(
        std::span<const std::uint8_t>(video).first(140),
        24,16,reference));
    auto bad_motion=video;
    const auto x_sign_absolute=52*8+x_sign_bit;
    bad_motion[x_sign_absolute/8] &=
        static_cast<std::uint8_t>(~(1U<<(x_sign_absolute%8)));
    CHECK(!decode_bink_simple_picture_luma(
        bad_motion,24,16,reference)); // +8 leaves right edge

    // Unsupported DCT blocks must fail rather than fabricate pixels.
    auto dct=video;
    const auto fourth_mode_bit=52*8+92+10+1+3*4;
    for(unsigned i=0;i<4;++i) {
        const auto bit=fourth_mode_bit+i;
        dct[bit/8]&=static_cast<std::uint8_t>(~(1U<<(bit%8)));
        if((5U>>i)&1U)dct[bit/8]|=static_cast<std::uint8_t>(1U<<(bit%8));
    }
    CHECK(!decode_bink_simple_picture_luma(dct,24,16,reference));

    // Complete single-row image with no reference: genuine compressed
    // uniform Fill mode, one uniform adaptive color.
    Bits key;trees(key);
    key.put(2,10);key.put(1,1);key.put(6,4);
    key.put(0,10); // subtype
    key.put(2,10);key.put(1,1);key.put(7,4);key.put(9,4);
    key.put(0,10); // pattern
    key.put(0,10);key.put(0,10); // motion
    key.put(0,10);key.put(0,10); // DC
    key.put(0,10); // run
    const auto independent=decode_bink_simple_picture_luma(
        packet(key),16,8);
    CHECK(independent && independent->mode_counts[6]==2);
    CHECK(std::all_of(independent->image.pixels.begin(),
                      independent->image.pixels.end(),
                      [](auto p){return p==0x79;}));
    std::puts("Native Bink compressed simple-picture row scheduling: PASS");
}
