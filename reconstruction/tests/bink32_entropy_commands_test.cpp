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
        CHECK(!decode_first_bink_picture_block_types(video,640,32));
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
        encoded.write(0,1); // one Huffman/RLE flag per bundle
        encoded.write(2,4); // literal
        encoded.write(12,4); // repeat length 4
        LsbBitReader bits(encoded.bytes);
        const auto bundle=read_bink_block_type_bundle(bits,tree,640,5);
        CHECK(bundle&&!bundle->end_of_bundle&&bundle->encoded_count==5);
        CHECK(bundle->modes.size()==5&&bundle->bits_consumed==19);
        CHECK(bundle->commands.repeat_commands==1);
        CHECK(std::all_of(bundle->modes.begin(),bundle->modes.end(),
                          [](auto value){return value==2;}));
        CHECK(bits.bit_position()==19);
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
    {
        // Model the actual 640x480 retail skip-only luma structure:
        // nine compressed blocks of type zero (8x560 + 320), eight
        // empty auxiliary bundles on the first row, and 32-bit plane
        // alignment. This is full source-driven luma *copy* semantics.
        Writer picture;
        // A twenty-bit first tree plus twenty-two zero trees matches
        // the 108-bit header length of the real static retail frames.
        picture.write(1,4);picture.write(1,1);picture.write(2,3);
        picture.write(0,4);picture.write(1,4);picture.write(2,4);
        for(unsigned i=0;i<22;++i)picture.write(0,4);
        constexpr unsigned columns=80,rows=60;
        for(unsigned row=0;row<rows;++row) {
            if(row%7==0) {
                picture.write(row==56?320:560,10);
                picture.write(1,1);picture.write(0,4);
            }
            if(row==0) {
                for(auto width_bits:{10U,13U,11U,10U,10U,10U,10U,13U})
                    picture.write(0,width_bits);
            }
        }
        CHECK(picture.bits==330);
        std::vector<std::uint8_t> video(52,0);
        video[0]=48;
        video.insert(video.end(),picture.bytes.begin(),picture.bytes.end());
        video.resize(96,0); // 22 final alignment bits
        std::vector<std::uint8_t> previous(640*480);
        for(std::size_t i=0;i<previous.size();++i)
            previous[i]=static_cast<std::uint8_t>(i*17U);
        const auto copied=decode_bink_skip_only_picture_luma(
            video,640,480,previous);
        CHECK(copied && copied->luma==previous);
        CHECK(copied->block_types_decoded==4800);
        CHECK(copied->bundles_decoded==9);
        CHECK(copied->next_plane_byte_offset==96);
        CHECK(!decode_bink_skip_only_picture_luma(
            video,640,480,std::span<const std::uint8_t>(previous).first(100)));
        CHECK(!decode_bink_skip_only_picture_luma(
            std::span<const std::uint8_t>(video).first(94),
            640,480,previous));
        auto corrupt=video;
        // First auxiliary bundle length starts at bit 52*8 + 107 =
        // byte 65 bit 3. Change count=0 to count=1, disallowing the
        // skip-only fast path before a false result can be published.
        constexpr std::size_t auxiliary_first_bit=52*8+92+15;
        corrupt[auxiliary_first_bit/8] |=
            static_cast<std::uint8_t>(1U<<(auxiliary_first_bit%8));
        CHECK(!decode_bink_skip_only_picture_luma(
            corrupt,640,480,previous));
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
        std::uint64_t frames=0,values=0,zero_fill=0,complete_skip_luma=0;
        std::vector<std::uint8_t> previous(
            std::size_t(index->header.width)*index->header.height,0x7d);
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
            const auto skip=decode_bink_skip_only_picture_luma(
                frame->video,index->header.width,index->header.height,
                previous);
            if(skip) {
                CHECK(skip->luma==previous);
                CHECK(skip->block_types_decoded==
                      ((std::size_t(index->header.width)/8)*
                       (std::size_t(index->header.height)/8)));
                ++complete_skip_luma;
            }
            if(decoded->first_bundle.encoded_count==560 &&
               decoded->first_bundle.commands.filled_remainder &&
               !decoded->first_bundle.modes.empty() &&
               std::all_of(decoded->first_bundle.modes.begin(),
                           decoded->first_bundle.modes.end(),
                           [](auto mode){return mode==0;}))
                ++zero_fill;
        }
        std::printf("Retail first-picture block bundles: %llu frames, %llu modes, %llu 560-skip fills, %llu full skip-luma planes\\n",
            static_cast<unsigned long long>(frames),
            static_cast<unsigned long long>(values),
            static_cast<unsigned long long>(zero_fill),
            static_cast<unsigned long long>(complete_skip_luma));
    }
    {
        // Original motion bundle: length, one constant flag, one signed
        // four-bit magnitude. Sign follows only NONZERO magnitudes.
        Writer constant;
        constant.write(3,10);
        constant.write(1,1);
        constant.write(7,4);
        constant.write(1,1);
        LsbBitReader bits(constant.bytes);
        const auto three=read_bink_signed_motion_bundle(bits,tree,640,3);
        CHECK(three && three->encoded_count==3);
        CHECK(three->constant_bundle && three->bits_consumed==16);
        CHECK(three->values==std::vector<std::int8_t>(3,-7));
        CHECK(bits.bit_position()==16);

        Writer zero;
        zero.write(2,10);
        zero.write(1,1);
        zero.write(0,4); // no sign bit for zero
        LsbBitReader bz(zero.bytes);
        const auto zz=read_bink_signed_motion_bundle(bz,tree,640);
        CHECK(zz && zz->bits_consumed==15);
        CHECK(zz->values==std::vector<std::int8_t>(2,0));

        Writer distinct;
        distinct.write(3,10);
        distinct.write(0,1); // Huffman stream instead of constant
        distinct.write(5,4);distinct.write(1,1); // -5
        distinct.write(0,4); // zero, consumes no sign
        distinct.write(3,4);distinct.write(0,1); // +3
        LsbBitReader dr(distinct.bytes);
        const auto values=read_bink_signed_motion_bundle(dr,tree,640);
        CHECK(values && !values->constant_bundle);
        CHECK(values->bits_consumed==25);
        CHECK(values->values==std::vector<std::int8_t>({-5,0,3}));
        CHECK(dr.bit_position()==25);
        LsbBitReader incomplete(
            std::span<const std::uint8_t>(distinct.bytes).first(3));
        CHECK(!read_bink_signed_motion_bundle(incomplete,tree,640));
        CHECK(incomplete.bit_position()==0);
        LsbBitReader capped(distinct.bytes);
        CHECK(!read_bink_signed_motion_bundle(capped,tree,640,2));
        CHECK(capped.bit_position()==0);
        Writer empty;empty.write(0,10);
        LsbBitReader er(empty.bytes);
        const auto stop=read_bink_signed_motion_bundle(er,tree,640);
        CHECK(stop && stop->end_of_bundle && stop->values.empty());
        CHECK(er.bit_position()==10);
    }
    {
        // Pattern bundle: each pixel mask uses exactly two original
        // Huffman nibbles, low before high, with NO constant flag.
        Writer patterns;
        patterns.write(2,11); // (640/8)*8+512 -> 11-bit count
        patterns.write(9,4);patterns.write(10,4); // A9
        patterns.write(15,4);patterns.write(1,4); // 1F
        LsbBitReader reader(patterns.bytes);
        const auto decoded=read_bink_pattern_bundle(reader,tree,640,2);
        CHECK(decoded && decoded->encoded_count==2);
        CHECK(decoded->bits_consumed==27);
        CHECK(decoded->values==std::vector<std::uint8_t>({0xA9,0x1F}));
        CHECK(reader.bit_position()==27);
        LsbBitReader truncated(
            std::span<const std::uint8_t>(patterns.bytes).first(3));
        CHECK(!read_bink_pattern_bundle(truncated,tree,640));
        CHECK(truncated.bit_position()==0);
        LsbBitReader capped(patterns.bytes);
        CHECK(!read_bink_pattern_bundle(capped,tree,640,1));
        CHECK(capped.bit_position()==0);
        Writer zero;zero.write(0,11);
        LsbBitReader none(zero.bytes);
        const auto end=read_bink_pattern_bundle(none,tree,640);
        CHECK(end && end->end_of_bundle && end->bits_consumed==11);
    }
    {
        // The run bundle has one immediate/Huffman branch for its whole
        // payload and uses a 13-bit count at a 640-pixel plane width.
        Writer encoded;
        encoded.write(3,13);
        encoded.write(0,1); // Huffman-coded run values
        encoded.write(3,4);encoded.write(12,4);encoded.write(8,4);
        LsbBitReader r(encoded.bytes);
        const auto runs=read_bink_run_bundle(r,tree,640,3);
        CHECK(runs && !runs->constant_bundle);
        CHECK(runs->bits_consumed==26 && r.bit_position()==26);
        CHECK(runs->values==std::vector<std::uint8_t>({3,12,8}));
        LsbBitReader clipped(
            std::span<const std::uint8_t>(encoded.bytes).first(3));
        CHECK(!read_bink_run_bundle(clipped,tree,640));
        CHECK(clipped.bit_position()==0);
        Writer uniform;
        uniform.write(4,13);uniform.write(1,1);uniform.write(14,4);
        LsbBitReader same(uniform.bytes);
        const auto constant=read_bink_run_bundle(same,tree,640);
        CHECK(constant && constant->constant_bundle);
        CHECK(constant->values==std::vector<std::uint8_t>(4,14));
        CHECK(constant->bits_consumed==18);
        Writer zero;zero.write(0,13);
        LsbBitReader ended(zero.bytes);
        const auto end=read_bink_run_bundle(ended,tree,640);
        CHECK(end && end->end_of_bundle && end->bits_consumed==13);
    }
    {
        // Color bundle updates the high-nibble predictor after EVERY
        // sample. Each high-nibble decoder is picked using the previous
        // high value; low nibbles come from the dedicated final tree.
        BlockEntropyTrees contexts{};
        for(auto& t:contexts.trees)t=tree;
        contexts.trees[2+5].symbol_permutation[6]=9; // 5 -> 9
        Writer encoded;
        encoded.write(2,13);
        encoded.write(0,1);
        encoded.write(5,4);encoded.write(1,4); // 0x51, last high=5
        encoded.write(6,4);encoded.write(2,4); // mapped -> 0x92
        LsbBitReader read(encoded.bytes);
        const auto colors=read_bink_color_bundle(
            read,contexts,640);
        CHECK(colors && !colors->constant_bundle);
        CHECK(colors->values==std::vector<std::uint8_t>({0x51,0x92}));
        CHECK(colors->next_high_nibble==9);
        CHECK(colors->bits_consumed==30 && read.bit_position()==30);
        LsbBitReader truncated(
            std::span<const std::uint8_t>(encoded.bytes).first(3));
        CHECK(!read_bink_color_bundle(truncated,contexts,640));
        CHECK(truncated.bit_position()==0);
        Writer uniform;
        uniform.write(3,13);uniform.write(1,1);
        uniform.write(8,4);uniform.write(1,4);
        LsbBitReader fill(uniform.bytes);
        const auto legacy=read_bink_color_bundle(
            fill,contexts,640,0,true);
        CHECK(legacy && legacy->constant_bundle);
        CHECK(legacy->values==std::vector<std::uint8_t>(3,127));
        CHECK(legacy->next_high_nibble==8);
        CHECK(legacy->bits_consumed==22);
        Writer zero;zero.write(0,13);
        LsbBitReader end_reader(zero.bytes);
        const auto end=read_bink_color_bundle(
            end_reader,contexts,640,7,false);
        CHECK(end && end->end_of_bundle && end->values.empty());
        CHECK(end->next_high_nibble==7 && end_reader.bit_position()==13);
        LsbBitReader invalid(encoded.bytes);
        CHECK(!read_bink_color_bundle(invalid,contexts,640,16,false));
        CHECK(invalid.bit_position()==0);
    }
    {
        // Full first-row compressed source order: block types,
        // subtypes, colors, patterns, X/Y, intra/inter DC, runs.
        // This does not claim to resolve per-block scheduling.
        Writer picture;
        for(unsigned i=0;i<23;++i)picture.write(0,4);
        picture.write(2,10);picture.write(1,1);picture.write(6,4);
        picture.write(0,10); // subtypes
        picture.write(2,13);picture.write(0,1); // colors
        picture.write(5,4);picture.write(1,4);
        picture.write(6,4);picture.write(2,4);
        picture.write(1,11);picture.write(9,4);picture.write(10,4);
        picture.write(2,10);picture.write(1,1);
        picture.write(2,4);picture.write(1,1); // X=-2
        picture.write(2,10);picture.write(1,1);
        picture.write(0,4); // Y=0
        picture.write(0,10); // intra DC
        picture.write(0,10); // inter DC
        picture.write(3,13);picture.write(1,1);picture.write(4,4);
        CHECK(picture.bits==235);
        std::vector<std::uint8_t> packet(52,0);
        packet[0]=48;
        packet.insert(packet.end(),picture.bytes.begin(),picture.bytes.end());
        const auto row=decode_first_bink_picture_bundle_set(packet,640,480);
        CHECK(row);
        CHECK(row->types.modes==std::vector<std::uint8_t>(2,6));
        CHECK(row->scaled_subtypes.end_of_bundle);
        CHECK(row->colors.values==std::vector<std::uint8_t>({0x51,0x62}));
        CHECK(row->colors.next_high_nibble==6);
        CHECK(row->patterns.values==std::vector<std::uint8_t>({0xA9}));
        CHECK(row->motion_x.values==std::vector<std::int8_t>(2,-2));
        CHECK(row->motion_y.values==std::vector<std::int8_t>(2,0));
        CHECK(row->intra_dc.signed_words_le.empty());
        CHECK(row->inter_dc.signed_words_le.empty());
        CHECK(row->runs.values==std::vector<std::uint8_t>(3,4));
        CHECK(row->next_picture_bit_offset==52*8+235);
        CHECK(!decode_first_bink_picture_bundle_set(
            std::span<const std::uint8_t>(packet).first(packet.size()-1),
            640,480));
        CHECK(!decode_first_bink_picture_bundle_set(packet,0,480));
    }
    std::puts("Bink original value-run command expansion: PASS");
}
