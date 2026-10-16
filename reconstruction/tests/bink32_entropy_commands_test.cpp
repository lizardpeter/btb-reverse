#include "btb/bink32_entropy_commands.hpp"
#include "btb/bink32_block_modes.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <span>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink RLE command test line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct Writer {
    std::vector<std::uint8_t> bytes{};
    std::size_t bits{};
    void write(std::uint32_t value,unsigned width) {
        for(unsigned i=0;i<width;++i) {
            if(bits/8>=bytes.size())bytes.push_back(0);
            if((value>>i)&1U)bytes[bits/8]|=static_cast<std::uint8_t>(1U<<(bits%8));
            ++bits;
        }
    }
};
static DynamicHuffmanTree identity(unsigned selector=0) {
    DynamicHuffmanTree t{}; t.selector=static_cast<std::uint8_t>(selector);
    for(unsigned i=0;i<16;++i)t.symbol_permutation[i]=static_cast<std::uint8_t>(i);
    return t;
}
static void literal(Writer& w,unsigned nibble) {
    w.write(0,1);w.write(nibble,4);
}
int main(int argc,char**argv) {
    // Identity test tree selects the verified 4-bit fixed table 0.
    // Symbols are intentionally supplied as compressed Huffman codewords.
    const auto tree=identity();
    Writer w;
    literal(w,2);literal(w,5);literal(w,12);literal(w,7);
    w.write(1,1);w.write(9,4);
    std::array<std::uint8_t,11> decoded{};
    LsbBitReader reader(w.bytes);
    const auto result=expand_bink_value_commands(reader,tree,decoded);
    CHECK(result&&result->filled_remainder&&result->huffman_symbols==4);
    CHECK(result->repeat_commands==1&&result->bits_consumed==w.bits);
    constexpr std::array<std::uint8_t,11> expected{2,5,5,5,5,5,7,9,9,9,9};
    CHECK(decoded==expected&&reader.bit_position()==w.bits);

    Writer runs;
    for(unsigned c=13;c<=15;++c)literal(runs,c);
    std::vector<std::uint8_t> sequence(52,0);
    LsbBitReader r2(runs.bytes);
    const auto all=expand_bink_value_commands(r2,tree,sequence,9);
    CHECK(all&&all->repeat_commands==3&&!all->filled_remainder);
    CHECK(all->bits_consumed==runs.bits);
    CHECK(std::all_of(sequence.begin(),sequence.end(),
                      [](auto x){return x==9;}));

    // Reject output overrun without changing either output or bit cursor.
    Writer bad;literal(bad,15);
    std::array<std::uint8_t,2> intact{0x7e,0x7e};
    LsbBitReader r3(bad.bytes);
    CHECK(!expand_bink_value_commands(r3,tree,intact,3));
    CHECK(r3.bit_position()==0&&intact[0]==0x7e&&intact[1]==0x7e);
    LsbBitReader r4({});
    CHECK(!expand_bink_value_commands(r4,tree,intact));
    CHECK(r4.bit_position()==0);
    std::array<std::uint8_t,1> nop{0xfe};
    CHECK(expand_bink_value_commands(r4,tree,std::span<std::uint8_t>()));
    CHECK(r4.bit_position()==0);

    // Use the variable-length real fixed table 1, rather than only
    // the four-bit identity tree. Locate one prefix decoding to value 5.
    unsigned prefix=0,width=0;
    for(unsigned p=0;p<32;++p) {
        const std::array<std::uint8_t,2> b{
            static_cast<std::uint8_t>(p),0};
        LsbBitReader probe(b);
        const auto sym=read_fixed_huffman_symbol(probe,1);
        if(sym&&sym->value==5){prefix=p;width=sym->consumed_bits;break;}
    }
    CHECK(width>0);
    Writer variable;variable.write(0,1);variable.write(prefix,width);
    std::array<std::uint8_t,1> one{};
    LsbBitReader r5(variable.bytes);
    const auto var=expand_bink_value_commands(r5,identity(1),one);
    CHECK(var&&var->huffman_symbols==1&&one[0]==5);
    CHECK(r5.bit_position()==1+width);
    // Original 30010632..30010651 signed-magnitude nibble composition.
    CHECK(combine_bink_signed_nibbles(0,0)==128);
    CHECK(combine_bink_signed_nibbles(7,15)==255);
    CHECK(combine_bink_signed_nibbles(8,0)==128); // negative zero
    CHECK(combine_bink_signed_nibbles(8,1)==127);
    CHECK(combine_bink_signed_nibbles(15,15)==1);
    for(unsigned h=0;h<16;++h)for(unsigned l=0;l<16;++l) {
        const auto raw=(h<<4)|l;
        const auto magnitude=raw&127;
        const auto expected=raw&128?128-magnitude:128+magnitude;
        CHECK(combine_bink_signed_nibbles(h,l)==expected);
    }
    {
        // Produce byte 127 from two genuine four-bit Huffman codewords.
        Writer signed_value;signed_value.write(8,4);signed_value.write(1,4);
        LsbBitReader sample_bits(signed_value.bytes);
        const auto sample=read_bink_signed_sample(sample_bits,tree,tree);
        CHECK(sample&&*sample==127&&sample_bits.bit_position()==8);
        // Truncated second nibble must leave source bit cursor unchanged.
        LsbBitReader incomplete{std::span<const std::uint8_t>{}};
        CHECK(!read_bink_signed_sample(incomplete,tree,tree));
        CHECK(incomplete.bit_position()==0);
    }
    {
        // Original entropy command: direct 4-bit mode 6 (Fill) repeated
        // across a decoded block-type bundle, then composed to pixels.
        Writer encoded;
        encoded.write(1,1);encoded.write(6,4);
        LsbBitReader bits(encoded.bytes);
        const auto modes=decode_bink_block_mode_values(bits,tree,2);
        CHECK(modes && modes->values.size()==2);
        CHECK(modes->values[0]==6 && modes->values[1]==6);
        CHECK(modes->statistics.filled_remainder);
        CHECK(bits.bit_position()==5);
        std::array<BinkBlockCommand,2> commands{};
        for(std::size_t i=0;i<2;++i) {
            commands[i].mode=static_cast<BinkBlockMode>(modes->values[i]);
            commands[i].fill=44;
        }
        const auto plane=compose_bink_mode_plane(16,8,commands);
        CHECK(plane && plane->pixels.size()==128);
        CHECK(std::all_of(plane->pixels.begin(),plane->pixels.end(),
                          [](auto value){return value==44;}));
        // Alphabet literals 10 and 11 are not supported Bink1 modes.
        Writer invalid;invalid.write(1,1);invalid.write(10,4);
        LsbBitReader bad_bits(invalid.bytes);
        CHECK(!decode_bink_block_mode_values(bad_bits,tree,2));
        CHECK(bad_bits.bit_position()==0);
        LsbBitReader empty_bits(std::span<const std::uint8_t>{});
        CHECK(!decode_bink_block_mode_values(empty_bits,tree,2));
        CHECK(empty_bits.bit_position()==0);
        CHECK(!decode_bink_block_mode_values(bits,tree,0));
        CHECK(bits.bit_position()==5);
    }
    {
        // The first real 640x480 startup frames supply 560 mode entries,
        // then a single fill-zero command. Reconstruct the same bit
        // framing with deterministic synthetic payload bytes.
        Writer picture;
        for(unsigned i=0;i<23;++i)picture.write(0,4); // 23 trees
        picture.write(560,10);
        picture.write(1,1);picture.write(0,4);
        std::vector<std::uint8_t> video(52,0);
        video[0]=48; // alpha section including its size WORD
        video[48]=0x12; video[49]=0x34; // picture control
        video.insert(video.end(),picture.bytes.begin(),picture.bytes.end());
        const auto decoded=decode_first_bink_picture_block_types(
            video,640,480);
        CHECK(decoded&&decoded->first_bundle.encoded_count==560);
        CHECK(decoded->prologue.alpha_section_bytes==48);
        CHECK(decoded->prologue.picture_control_word==0x3412);
        CHECK(decoded->first_bundle.modes.size()==560);
        CHECK(decoded->first_bundle.commands.filled_remainder);
        CHECK(std::all_of(decoded->first_bundle.modes.begin(),
                          decoded->first_bundle.modes.end(),
                          [](auto mode){return mode==0;}));
        CHECK(decoded->next_picture_bit_offset==
              52*8+92+10+5);
        CHECK(!decode_first_bink_picture_block_types(video,640,64));
        CHECK(!decode_first_bink_picture_block_types(video,0,480));
        CHECK(!decode_first_bink_picture_block_types(video,640,0));
        CHECK(!decode_first_bink_picture_block_types(
            std::span<const std::uint8_t>(video).first(64),640,480));
        auto invalid=video;
        invalid[0]=49;
        CHECK(!decode_first_bink_picture_block_types(invalid,640,480));
    }
    {
        // Huffman literal 2, then command 12 repeats it four times.
        // Tests exact count framing, 4/8/12/32-run semantics and
        // parser rollback on malformed final data.
        Writer encoded;
        encoded.write(5,10); // bundle count for width=640
        encoded.write(0,1); // Huffman stream
        encoded.write(2,4); // literal
        encoded.write(0,1);
        encoded.write(12,4); // repeat length 4
        LsbBitReader bits(encoded.bytes);
        const auto bundle=read_bink_block_type_bundle(bits,tree,640,5);
        CHECK(bundle&&!bundle->end_of_bundle&&bundle->encoded_count==5);
        CHECK(bundle->modes.size()==5&&bundle->bits_consumed==20);
        CHECK(bundle->commands.repeat_commands==1);
        CHECK(std::all_of(bundle->modes.begin(),bundle->modes.end(),
                          [](auto value){return value==2;}));
        CHECK(bits.bit_position()==20);
        auto limited=LsbBitReader(encoded.bytes);
        CHECK(!read_bink_block_type_bundle(limited,tree,640,4));
        CHECK(limited.bit_position()==0);
        Writer invalid;
        invalid.write(3,10);invalid.write(1,1);invalid.write(10,4);
        LsbBitReader wrong(invalid.bytes);
        CHECK(!read_bink_block_type_bundle(wrong,tree,640));
        CHECK(wrong.bit_position()==0);
        const std::array<std::uint8_t,2> partial{5,0};
        LsbBitReader truncated(partial);
        CHECK(!read_bink_block_type_bundle(truncated,tree,640));
        CHECK(truncated.bit_position()==0);
        Writer finish;finish.write(0,10);
        LsbBitReader ended(finish.bytes);
        const auto end=read_bink_block_type_bundle(ended,tree,640);
        CHECK(end&&end->end_of_bundle&&end->modes.empty());
        CHECK(end->bits_consumed==10&&ended.bit_position()==10);
    }
    // Optional unmodified retail-movie validation. GitHub CI does not
    // contain commercial assets; pass local BIK filenames to this binary.
    for(int arg=1;arg<argc;++arg) {
        std::ifstream file(argv[arg],std::ios::binary);
        CHECK(file.good());
        const std::vector<std::uint8_t> movie{
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>()};
        const auto index=parse_container_index(movie);
        CHECK(index);
        std::uint64_t frames=0,values=0,zero_fill=0;
        for(std::uint32_t i=1;i<=index->header.frame_count;++i) {
            const auto frame=split_encoded_frame(*index,movie,i);
            CHECK(frame);
            if((index->video_flags&0x00100000U)==0)continue;
            const auto decoded=decode_first_bink_picture_block_types(
                frame->video,index->header.width,index->header.height);
            CHECK(decoded);
            CHECK(decoded->next_picture_bit_offset<=frame->video.size()*8);
            ++frames;
            values+=decoded->first_bundle.modes.size();
            if(decoded->first_bundle.encoded_count==560 &&
               decoded->first_bundle.commands.filled_remainder &&
               !decoded->first_bundle.modes.empty() &&
               std::all_of(decoded->first_bundle.modes.begin(),
                           decoded->first_bundle.modes.end(),
                           [](auto mode){return mode==0;}))
                ++zero_fill;
        }
        std::printf("Retail first-picture block bundles: %llu frames, %llu modes, %llu 560-skip fills\\n",
            static_cast<unsigned long long>(frames),
            static_cast<unsigned long long>(values),
            static_cast<unsigned long long>(zero_fill));
    }
    std::puts("Bink original value-run command expansion: PASS");
}
