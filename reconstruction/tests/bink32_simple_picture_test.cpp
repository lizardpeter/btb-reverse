#include "btb/bink32_simple_picture.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
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
int main(int argc,char**argv) {
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
    // At the second row boundary both first-row motion bundles are
    // exhausted; original read_motion_values reads a zero count for
    // each even though row 1 has no motion blocks.
    encoded.put(0,10); // second-row motion X end marker
    encoded.put(0,10); // second-row motion Y end marker
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
    for(std::size_t source=0;source<decoded->source_refills.size();++source)
        CHECK(decoded->source_refills[source]==
              (source==4 || source==5 ? 2U : 1U));
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
    {
        // The original retail 640x480 static scene uses nine type
        // bundle refills: 8 x 560 skip blocks then 320. Compare the
        // new generic nine-source row scheduler against the independent
        // skip-only scanner over all sixty compressed block rows.
        Bits static_scene;trees(static_scene);
        for(unsigned row=0;row<60;++row) {
            if(row%7==0) {
                static_scene.put(row==56?320:560,10);
                static_scene.put(1,1);static_scene.put(0,4);
            }
            if(row==0)
                for(unsigned n:{10U,13U,11U,10U,10U,10U,10U,13U})
                    static_scene.put(0,n);
        }
        const auto scene=packet(static_scene);
        std::vector<std::uint8_t> prior(640*480);
        for(std::size_t i=0;i<prior.size();++i)
            prior[i]=static_cast<std::uint8_t>(i*11U);
        const auto simple=decode_bink_simple_picture_luma(
            scene,640,480,prior);
        const auto skip_only=decode_bink_skip_only_picture_luma(
            scene,640,480,prior);
        CHECK(simple && skip_only);
        CHECK(simple->image.pixels==skip_only->luma);
        CHECK(simple->mode_counts[0]==4800);
        CHECK(simple->source_refills[0]==9);
        for(unsigned source=1;source<9;++source)
            CHECK(simple->source_refills[source]==1);
        CHECK(simple->next_plane_byte_offset==
              skip_only->next_plane_byte_offset);
    }
    {
        // Original Scaled marker occurs on both row boundaries, while
        // a single 16x16 Fill subtype/color payload is decoded at
        // the even-row upper-left anchor. Other three 8x8 cells must
        // be covered by that one source block.
        Bits scaled;trees(scaled);
        scaled.put(2,10);scaled.put(1,1);scaled.put(1,4);
        scaled.put(1,10);scaled.put(1,1);scaled.put(6,4);
        scaled.put(1,10);scaled.put(1,1);
        scaled.put(4,4);scaled.put(2,4);
        scaled.put(0,10); // pattern
        scaled.put(0,10);scaled.put(0,10); // X / Y
        scaled.put(0,10);scaled.put(0,10); // intra/inter DC
        scaled.put(0,10); // run
        scaled.put(0,10); // second-row subtype end
        scaled.put(0,10); // second-row color end
        const auto full=decode_bink_simple_picture_luma(
            packet(scaled),16,16);
        CHECK(full && full->mode_counts[1]==2);
        CHECK(std::all_of(full->image.pixels.begin(),
                          full->image.pixels.end(),
                          [](std::uint8_t pixel){return pixel==0x42;}));
        CHECK(full->next_plane_byte_offset==52+
            ((scaled.bits+31)/32)*4);

        // A lower-row marker that does not match the upper-left
        // anchor must never leave half a 16x16 tile undecoded.
        Bits wrong;trees(wrong);
        wrong.put(2,10);wrong.put(0,1);
        wrong.put(1,4);wrong.put(0,4); // scaled then Skip
        wrong.put(1,10);wrong.put(1,1);wrong.put(6,4);
        wrong.put(1,10);wrong.put(1,1);
        wrong.put(4,4);wrong.put(2,4);
        wrong.put(0,10);
        wrong.put(0,10);wrong.put(0,10);
        wrong.put(0,10);wrong.put(0,10);
        wrong.put(0,10);
        wrong.put(0,10); // subtype exhausted at row 1
        wrong.put(0,10); // color exhausted at row 1
        CHECK(!decode_bink_simple_picture_luma(packet(wrong),16,16));
    }
    {
        // Two-column 16x16 Scaled Pattern: one pair of colors and eight
        // Huffman-coded mask bytes, each doubled in both pixel axes.
        Bits source;trees(source);
        source.put(2,10);source.put(1,1);source.put(1,4);
        source.put(1,10);source.put(1,1);source.put(8,4);
        source.put(2,10);source.put(0,1);
        source.put(0,4);source.put(3,4); // color 3
        source.put(12,4);source.put(8,4); // color 200
        source.put(8,10);
        for(unsigned i=0;i<8;++i) {
            source.put(0xA,4);source.put(0xA,4);
        }
        source.put(0,10);source.put(0,10); // motion
        source.put(0,10);source.put(0,10); // DC
        source.put(0,10); // run
        source.put(0,10); // subtype exhausted at row 1
        source.put(0,10); // colors exhausted at row 1
        source.put(0,10); // patterns exhausted at row 1
        const auto image=decode_bink_simple_picture_luma(
            packet(source),16,16);
        CHECK(image && image->mode_counts[1]==2);
        for(std::size_t y=0;y<16;++y) {
            CHECK(image->image.pixels[y*16+0]==3);
            CHECK(image->image.pixels[y*16+1]==3);
            CHECK(image->image.pixels[y*16+2]==200);
            CHECK(image->image.pixels[y*16+3]==200);
        }
    }
    {
        // 16x16 scaled RAW: 64 original color bytes, upscaled 2x
        // without interpolation or invented intermediate values.
        Bits source;trees(source);
        source.put(2,10);source.put(1,1);source.put(1,4);
        source.put(1,10);source.put(1,1);source.put(9,4);
        source.put(64,10);source.put(0,1);
        for(unsigned i=0;i<64;++i) {
            const auto byte=(i*3U)&255U;
            source.put(byte>>4,4);
            source.put(byte&15U,4);
        }
        source.put(0,10); // pattern
        source.put(0,10);source.put(0,10);
        source.put(0,10);source.put(0,10);
        source.put(0,10);
        source.put(0,10); // subtype exhausted at row 1
        source.put(0,10); // colors exhausted at row 1
        const auto image=decode_bink_simple_picture_luma(
            packet(source),16,16);
        CHECK(image && image->mode_counts[1]==2);
        for(std::size_t y=0;y<16;++y)
            for(std::size_t x=0;x<16;++x)
                CHECK(image->image.pixels[y*16+x]==
                    static_cast<std::uint8_t>(
                        3*((y/2)*8+(x/2))));
    }
    // Optional original BIK smoke: count full compressed luma planes the
    // strict source-native subset supports, without embedding retail
    // assets or claiming original pixel parity from placeholder history.
    for(int arg=1;arg<argc;++arg) {
        std::ifstream in(argv[arg],std::ios::binary);
        CHECK(in.good());
        std::vector<std::uint8_t> movie{
            std::istreambuf_iterator<char>(in),
            std::istreambuf_iterator<char>()};
        const auto index=parse_container_index(movie);
        CHECK(index);
        std::vector<std::uint8_t> previous(
            std::size_t(index->header.width)*index->header.height,0x80);
        std::size_t supported=0,first_without_reference=0;
        std::array<std::size_t,10> totals{};
        for(std::uint32_t frame=1;frame<=index->header.frame_count;++frame) {
            const auto parts=split_encoded_frame(*index,movie,frame);
            CHECK(parts);
            if((index->video_flags&0x00100000U)==0)continue;
            const auto simple=decode_bink_simple_picture_luma(
                parts->video,index->header.width,index->header.height,
                previous);
            if(simple) {
                ++supported;
                previous=simple->image.pixels;
                for(std::size_t i=0;i<totals.size();++i)
                    totals[i]+=simple->mode_counts[i];
            } else if(frame==1) {
                ++first_without_reference;
            }
        }
        std::printf(
            "Retail Bink strict simple-luma subset: %zu/%u frames; "
            "skip=%zu motion=%zu fill=%zu pattern=%zu raw=%zu; "
            "initial-frame-needs-full-decoder=%zu\\n",
            supported,index->header.frame_count,
            totals[0],totals[2],totals[6],totals[8],totals[9],
            first_without_reference);
    }
    std::puts("Native Bink compressed simple-picture row scheduling: PASS");
}
