#include "btb/bink32_dc_stream.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>

using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink DC test %d: %s\n",__LINE__,#x);std::abort();}}while(0)
int main() {
    // Original Bink 3000F087 uses a raw little-endian 16-bit word,
    // signed only once reconstructed as coefficient[0].
    const std::array<std::uint8_t,8> dc_bytes{
        0x80,0x00, 0x00,0x80, 0xff,0xff, 0x00,0x01
    };
    BinkDcWordStream words{dc_bytes,0};
    CHECK(words.remaining_words()==4);
    CHECK(read_original_bink_dc_word(words)==128);
    CHECK(words.position==2&&words.remaining_words()==3);
    CHECK(read_original_bink_dc_word(words)==-32768);
    CHECK(words.position==4);
    CHECK(read_original_bink_dc_word(words)==-1);
    CHECK(read_original_bink_dc_word(words)==256);
    CHECK(words.position==8&&words.remaining_words()==0);
    CHECK(!read_original_bink_dc_word(words)&&words.position==8);
    words.position=9;
    CHECK(!read_original_bink_dc_word(words)&&words.position==9);
    BinkDcWordStream partial{std::span<const std::uint8_t>(dc_bytes).first(3),2};
    CHECK(!read_original_bink_dc_word(partial)&&partial.position==2);
    // AC mode0, quant0 needs 8 bits and decodes one 16x16 all-DC block.
    const std::array<std::uint8_t,2> flat_blocks{
        0x00,0x10 // block1 mode=0 quant=0; block2 mode=0 quant=1
    };
    BinkDcWordStream dc{dc_bytes,0};
    LsbBitReader compressed(flat_blocks);
    const auto first=decode_original_bink_block_from_dc_plane(dc,compressed);
    CHECK(first&&first->dc==128&&first->dc_bytes_consumed==2);
    CHECK(first->decoded.ac.mode==0&&first->decoded.quant_selector==0);
    CHECK(first->decoded.coefficients[0]==128);
    CHECK(dc.position==2&&compressed.bit_position()==8);
    for(auto sample:first->decoded.pixels)CHECK(sample==16);
    const auto second=decode_original_bink_block_from_dc_plane(dc,compressed);
    CHECK(second&&second->dc==-32768&&second->decoded.quant_selector==1);
    CHECK(dc.position==4&&compressed.bit_position()==16);
    CHECK(!decode_original_bink_block_from_dc_plane(dc,compressed));
    CHECK(dc.position==4&&compressed.bit_position()==16);
    // No AC bytes: even if DC available, both streams must roll back.
    BinkDcWordStream available{dc_bytes,0};
    LsbBitReader empty{std::span<const std::uint8_t>{}};
    CHECK(!decode_original_bink_block_from_dc_plane(available,empty));
    CHECK(available.position==0&&empty.bit_position()==0);
    // No complete DC word: even if AC bits available, both roll back.
    BinkDcWordStream truncated_dc{std::span<const std::uint8_t>(dc_bytes).first(1),0};
    LsbBitReader encoded(flat_blocks);
    CHECK(!decode_original_bink_block_from_dc_plane(truncated_dc,encoded));
    CHECK(truncated_dc.position==0&&encoded.bit_position()==0);
    // Original 30010DD6 width==0 branch: one four-bit nibble
    // repeats the previous DC word in groups of at most eight.
    for(std::size_t len=1;len<=8;++len) {
        const std::array<std::uint8_t,1> source{0x00};
        LsbBitReader encoded(source);
        std::array<std::int16_t,8> values{};
        values.fill(41);
        CHECK(expand_original_bink_zero_delta_group(
            encoded,-32768,std::span<std::int16_t>(values).first(len)));
        CHECK(encoded.bit_position()==4);
        for(std::size_t i=0;i<8;++i)
            CHECK(values[i]==(i<len?-32768:41));
        // There is no second complete four-bit width after consuming
        // a five-bit prefix: rollback both bitreader and destination.
        LsbBitReader too_short(source);
        CHECK(too_short.skip(5));
        auto unchanged=values;
        CHECK(!expand_original_bink_zero_delta_group(
            too_short,123,std::span<std::int16_t>(values).first(len)));
        CHECK(too_short.bit_position()==5&&unchanged==values);
    }
    {
        const std::array<std::uint8_t,1> nonzero_width{0x09};
        LsbBitReader source(nonzero_width);
        std::array<std::int16_t,8> unchanged{};
        unchanged.fill(-9);
        CHECK(!expand_original_bink_zero_delta_group(source,123,unchanged));
        CHECK(source.bit_position()==0);
        for(auto v:unchanged)CHECK(v==-9);
        CHECK(!expand_original_bink_zero_delta_group(
            source,123,std::span<std::int16_t>{}));
        std::array<std::int16_t,9> too_long{};
        CHECK(!expand_original_bink_zero_delta_group(source,123,too_long));
        CHECK(source.bit_position()==0);
    }
    std::puts("Bink original predecoded DC stream -> AC/quant IDCT 16x16 pixels: PASS");
}
