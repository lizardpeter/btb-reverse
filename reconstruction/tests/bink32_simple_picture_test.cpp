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
    {
        // Compressed RUN mode: true scan order zero traverses
        // columns alternately downward and upward. Four 16-pixel
        // lengths each reuse one color, spanning one complete 8x8 tile.
        Bits encoded;trees(encoded);
        encoded.put(1,10);encoded.put(1,1);encoded.put(3,4);
        // 8-pixel width -> width>>4 == 0, so the original
        // subordinate-type count has NINE bits (base 0x200).
        encoded.put(0,9); // subordinate scaled type
        encoded.put(4,10);encoded.put(0,1);
        for(unsigned pixel:{10U,20U,30U,40U}) {
            encoded.put(pixel>>4,4);
            encoded.put(pixel&15U,4);
        }
        encoded.put(0,10); // pattern bytes
        encoded.put(0,10);encoded.put(0,10); // X/Y
        encoded.put(0,10);encoded.put(0,10); // intra/inter DC
        encoded.put(4,10);encoded.put(1,1);encoded.put(15,4);
        encoded.put(0,4); // exact 8x8 scan-zero selector
        for(unsigned i=0;i<4;++i)encoded.put(1,1);
        const auto source=packet(encoded);
        const auto image=decode_bink_simple_picture_luma(source,8,8);
        CHECK(image && image->mode_counts[3]==1);
        for(std::size_t row=0;row<8;++row)
            for(std::size_t col=0;col<8;++col)
                CHECK(image->image.pixels[row*8+col]==
                      static_cast<std::uint8_t>(10*(1+col/2)));
        CHECK(image->next_plane_byte_offset==
              52+((encoded.bits+31)/32)*4);
        // Scan selector 1 uses a different original lookup but
        // preserves the same four decoded run values and pixel count.
        auto second_scan=source;
        const auto scan_bit=52*8+encoded.bits-8;
        second_scan[scan_bit/8]|=
            static_cast<std::uint8_t>(1U<<(scan_bit%8));
        const auto alternate=decode_bink_simple_picture_luma(
            second_scan,8,8);
        CHECK(alternate && alternate->mode_counts[3]==1);
        CHECK(alternate->image.pixels!=image->image.pixels);
        CHECK(alternate->image.pixels[59]==10);
        // One missing run color cannot yield a partial frame.
        auto truncated=source;
        truncated.resize(source.size()-4);
        CHECK(!decode_bink_simple_picture_luma(truncated,8,8));
    }
    {
        // Scaled 16x16 RUN: decode exactly one underlying 8x8
        // pattern and replicate each source pixel 2x on both axes.
        Bits encoded;trees(encoded);
        encoded.put(2,10);encoded.put(1,1);encoded.put(1,4);
        encoded.put(1,10);encoded.put(1,1);encoded.put(3,4);
        encoded.put(4,10);encoded.put(0,1);
        for(unsigned pixel:{1U,2U,3U,4U}) {
            encoded.put(pixel>>4,4);
            encoded.put(pixel&15U,4);
        }
        encoded.put(0,10); // pattern
        encoded.put(0,10);encoded.put(0,10); // X/Y
        encoded.put(0,10);encoded.put(0,10); // DC
        encoded.put(4,10);encoded.put(1,1);encoded.put(15,4);
        encoded.put(0,4); // scan order 0
        for(unsigned i=0;i<4;++i)encoded.put(1,1);
        encoded.put(0,10); // row 1: exhausted subtype
        encoded.put(0,10); // row 1: exhausted colors
        encoded.put(0,10); // row 1: exhausted runs
        const auto image=decode_bink_simple_picture_luma(
            packet(encoded),16,16);
        CHECK(image && image->mode_counts[1]==2);
        for(std::size_t row=0;row<16;++row)
            for(std::size_t col=0;col<16;++col)
                CHECK(image->image.pixels[row*16+col]==
                      static_cast<std::uint8_t>(1+col/4));
    }
    {
        // Full 16-pattern reference: every pixel has a distinct color
        // (1..64), so all permutations must produce distinguishable
        // raster outputs. These FNV-1a hashes were calculated from the
        // independent Bink1 bink_patterns[16][64] table, not from the
        // renderer's own lookup function.
        constexpr std::array<std::uint64_t,16> expected{{
            0xdd29d09686f817f5ULL, 0x1c2f7c0f319a1865ULL, 0xe8febb31381505b5ULL, 0x8ccce5638b5d3ac5ULL,
            0xd5dd24b48fd486b5ULL, 0x69d210a3937137e5ULL, 0x1920c220a428516dULL, 0xb5418dfd07454b05ULL,
            0x24899b222fd9c149ULL, 0xd3ffe7952e20d4a5ULL, 0x60994c2686c25c95ULL, 0x209d3c7b605343e5ULL,
            0x682556125077a6f1ULL, 0x960b8a40d767a585ULL, 0xfd63f025155ab895ULL, 0x0324945e3c436545ULL
        }};
        Bits packed;trees(packed);
        packed.put(1,10);packed.put(1,1);packed.put(3,4);
        packed.put(0,9); // width=8 means 9-bit subtype length
        packed.put(64,10);packed.put(0,1); // 64 literal colors
        for(unsigned i=1;i<=64;++i) {
            packed.put(i>>4,4);packed.put(i&15U,4);
        }
        packed.put(0,10); // pattern byte pool
        packed.put(0,10);packed.put(0,10); // signed motion pools
        packed.put(0,10);packed.put(0,10); // intra/inter DC pools
        packed.put(63,10);packed.put(1,1);packed.put(0,4);
        const auto scan_offset=packed.bits;
        packed.put(0,4); // four scan selector bits
        for(unsigned i=0;i<63;++i)packed.put(1,1);
        const auto base=packet(packed);
        for(unsigned scan=0;scan<16;++scan) {
            auto candidate=base;
            for(unsigned b=0;b<4;++b) {
                const auto bit=52*8+scan_offset+b;
                const auto mask=static_cast<std::uint8_t>(1U<<(bit%8));
                if((scan>>b)&1U)candidate[bit/8]|=mask;
                else candidate[bit/8]&=static_cast<std::uint8_t>(~mask);
            }
            const auto decoded=decode_bink_simple_picture_luma(
                candidate,8,8);
            CHECK(decoded && decoded->mode_counts[3]==1);
            CHECK(decoded->image.pixels.size()==64);
            std::uint64_t digest=0xcbf29ce484222325ULL;
            for(const auto pixel:decoded->image.pixels) {
                digest^=pixel;
                digest*=0x100000001b3ULL;
            }
            CHECK(digest==expected[scan]);
        }
    }
    {
        // Source-native 8x8 compressed residue block. Motion X/Y are
        // both zero, and the sparse residue tree adds +1 at scan[4].
        // Every other pixel must preserve its previous-frame value.
        Bits encoded;trees(encoded);
        encoded.put(1,10);encoded.put(1,1);encoded.put(4,4);
        encoded.put(0,9); // 8-wide plane: 9-bit subtype count
        encoded.put(0,10); // colors
        encoded.put(0,10); // pattern
        for(unsigned component=0;component<2;++component) {
            encoded.put(1,10);encoded.put(1,1);encoded.put(0,4);
        }
        encoded.put(0,10);encoded.put(0,10); // intra/inter DC
        encoded.put(0,10); // run
        encoded.put(0,7); // one residue coefficient event
        encoded.put(0,3); // start at mask=1
        encoded.put(1,1); // activate group at scan[4..7]
        encoded.put(0,1); // materialize scan[4]
        encoded.put(0,1); // positive
        const auto source=packet(encoded);
        std::vector<std::uint8_t> previous(64,128);
        const auto decoded=decode_bink_simple_picture_luma(
            source,8,8,previous);
        CHECK(decoded && decoded->mode_counts[4]==1);
        CHECK(decoded->image.pixels.size()==64);
        for(std::size_t i=0;i<64;++i)
            CHECK(decoded->image.pixels[i]==(i==2?129:128));
        CHECK(decoded->next_plane_byte_offset==
              52+((encoded.bits+31)/32)*4);
        CHECK(!decode_bink_simple_picture_luma(source,8,8));
        auto clipped=source;
        clipped.resize(69);
        CHECK(!decode_bink_simple_picture_luma(
            clipped,8,8,previous));
    }
    {
        // True compressed Bink1 INTRA block with no AC coefficients.
        // Its independent 8x8 IDCT uses the 2002 original quant DC
        // multiplier 0x10000: DC=8 -> 8*32/256 = 1 pixel.
        Bits encoded;trees(encoded);
        encoded.put(1,10);encoded.put(1,1);encoded.put(5,4);
        encoded.put(0,9); // 8-wide scaled subtype pool
        encoded.put(0,10); // colors
        encoded.put(0,10); // patterns
        encoded.put(0,10);encoded.put(0,10); // motion
        encoded.put(1,10);encoded.put(8,11); // unsigned intra DC=8
        encoded.put(0,10); // inter DC
        encoded.put(0,10); // run
        encoded.put(0,4);encoded.put(0,4); // zero AC, selector 0
        const auto image=decode_bink_simple_picture_luma(
            packet(encoded),8,8);
        CHECK(image && image->mode_counts[5]==1);
        CHECK(std::all_of(image->image.pixels.begin(),
                          image->image.pixels.end(),
                          [](auto p){return p==1;}));
        // Same original DC=8 with quant index 15: the decoder's
        // validated 0x2c0000 DC quantizer produces 44, not 1.
        auto high_quant=encoded;
        for(unsigned i=0;i<4;++i) {
            const auto bit=encoded.bits-4+i;
            high_quant.bytes[bit/8] |=
                static_cast<std::uint8_t>(1U<<(bit%8));
        }
        const auto amplified=decode_bink_simple_picture_luma(
            packet(high_quant),8,8);
        CHECK(amplified && amplified->image.pixels.size()==64);
        CHECK(std::all_of(amplified->image.pixels.begin(),
                          amplified->image.pixels.end(),
                          [](auto p){return p==44;}));
        // Nonzero AC requires a complete 8x8 transform. Fail
        // closed instead of substituting the 2x scaled iDCT.
        Bits ac=encoded;
        ac.bits-=8;ac.bytes.resize((ac.bits+7)/8);
        // The prior quantized DCT bits must be replaced, not
        // appended after the original eight-bit zero-AC header.
        for(unsigned n=0;n<8;++n) {
            const auto bit=ac.bits+n;
            if(bit/8<ac.bytes.size())
                ac.bytes[bit/8]&=static_cast<std::uint8_t>(
                    ~(1U<<(bit%8)));
        }
        ac.put(1,4);
        for(unsigned n=0;n<3;++n)ac.put(0,1);
        ac.put(1,1);ac.put(0,1); // scan1 coefficient +1
        ac.put(0,1);ac.put(0,1);
        ac.put(0,4);
        CHECK(!decode_bink_simple_picture_luma(packet(ac),8,8));
    }
    {
        // INTER DCT: source X/Y motion prediction is copied first,
        // then a zero-AC signed DC residual is added modulo 256.
        Bits encoded;trees(encoded);
        encoded.put(1,10);encoded.put(1,1);encoded.put(7,4);
        encoded.put(0,9);
        encoded.put(0,10);encoded.put(0,10);
        for(unsigned axis=0;axis<2;++axis) {
            encoded.put(1,10);encoded.put(1,1);encoded.put(0,4);
        }
        encoded.put(0,10); // intra DC
        encoded.put(1,10);encoded.put(8,10);encoded.put(0,1);
        encoded.put(0,10); // run
        encoded.put(0,4);encoded.put(0,4); // zero AC, quant 0
        std::vector<std::uint8_t> prior(64,255);
        const auto frame=decode_bink_simple_picture_luma(
            packet(encoded),8,8,prior);
        CHECK(frame && frame->mode_counts[7]==1);
        CHECK(std::all_of(frame->image.pixels.begin(),
                          frame->image.pixels.end(),
                          [](auto p){return p==0;})); // 255+1 wraps
        CHECK(!decode_bink_simple_picture_luma(packet(encoded),8,8));
        CHECK(std::all_of(prior.begin(),prior.end(),
                          [](auto p){return p==255;}));
    }
    {
        // A scaled 16x16 INTRA DC-only DCT uses a single 8x8
        // intra coefficient payload, doubled through the established
        // source-exact 16x16 nearest-neighbor pixel compositor.
        Bits encoded;trees(encoded);
        encoded.put(2,10);encoded.put(1,1);encoded.put(1,4);
        encoded.put(1,10);encoded.put(1,1);encoded.put(5,4);
        encoded.put(0,10);encoded.put(0,10);
        encoded.put(0,10);encoded.put(0,10);
        encoded.put(1,10);encoded.put(8,11);
        encoded.put(0,10);encoded.put(0,10);
        encoded.put(0,4);encoded.put(0,4);
        encoded.put(0,10); // second row subtype closes
        encoded.put(0,10); // second row intra DC closes
        const auto scaled=decode_bink_simple_picture_luma(
            packet(encoded),16,16);
        CHECK(scaled && scaled->mode_counts[1]==2);
        CHECK(scaled->image.pixels.size()==256);
        CHECK(std::all_of(scaled->image.pixels.begin(),
                          scaled->image.pixels.end(),
                          [](auto p){return p==1;}));
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
