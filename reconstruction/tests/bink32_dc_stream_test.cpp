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
    std::puts("Bink original predecoded DC stream -> AC/quant IDCT 16x16 pixels: PASS");
}
