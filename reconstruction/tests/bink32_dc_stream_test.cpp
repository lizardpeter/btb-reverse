#include "btb/bink32_dc_stream.hpp"
#include <array>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <span>

using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)){std::fprintf(stderr,"Bink DC test %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct TestBits {
    std::array<std::uint8_t,64> data{};
    std::size_t bit_count{};
    void put(std::uint32_t value,unsigned width) {
        for(unsigned i=0;i<width;++i) {
            CHECK(bit_count<data.size()*8);
            if((value>>i)&1U)
                data[bit_count/8]|=static_cast<std::uint8_t>(1U<<(bit_count%8));
            ++bit_count;
        }
    }
    LsbBitReader reader() const {
        return LsbBitReader(
            std::span<const std::uint8_t>(data).first((bit_count+7)/8));
    }
};
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
    {
        // Source group 30010D94 consumes width4 and up to 8 signed
        // deltas; zero magnitude does NOT consume a sign bit.
        TestBits packed;
        packed.put(4,4);
        packed.put(1,4);packed.put(0,1); // +1
        packed.put(3,4);packed.put(1,1); // -3
        packed.put(0,4);                 // zero
        packed.put(15,4);packed.put(0,1);// +15
        auto bits=packed.reader();
        std::array<std::int16_t,4> output{};
        const auto previous=decode_original_bink_dc_delta_group(
            bits,100,output);
        constexpr std::array<std::int16_t,4> expected{101,98,98,113};
        CHECK(previous&&*previous==113&&output==expected);
        CHECK(bits.bit_position()==packed.bit_count&&packed.bit_count==23);
        // A subsequent width-zero group starts at current bit offset
        // and repeats the previous cumulative signed WORD.
        packed.put(0,4);
        auto sequential=packed.reader();
        std::array<std::int16_t,4> first_four{};
        const auto carry=decode_original_bink_dc_delta_group(
            sequential,100,first_four);
        CHECK(carry&&*carry==113);
        std::array<std::int16_t,8> next_eight{};
        const auto last=decode_original_bink_dc_delta_group(
            sequential,*carry,next_eight);
        CHECK(last&&*last==113&&sequential.bit_position()==27);
        for(auto v:next_eight)CHECK(v==113);
    }
    {
        // WORD stores in the original x86 wrap exactly at 16 bits.
        TestBits packed;
        packed.put(1,4);packed.put(1,1);packed.put(0,1);
        auto bits=packed.reader();
        std::array<std::int16_t,1> output{};
        const auto wrap=decode_original_bink_dc_delta_group(
            bits,32767,output);
        CHECK(wrap&&*wrap==-32768&&output[0]==-32768);
        CHECK(bits.bit_position()==6);
    }
    {
        // Max encoded width nibble = 15, 15 low magnitude bits
        // and conditional sign, including mixed signed samples.
        TestBits packed;
        packed.put(15,4);
        packed.put(32767,15);packed.put(1,1);
        packed.put(0,15);
        auto bits=packed.reader();
        std::array<std::int16_t,2> out{};
        const auto decoded=decode_original_bink_dc_delta_group(
            bits,1,out);
        CHECK(decoded&&*decoded==-32766);
        CHECK(out[0]==-32766&&out[1]==-32766);
        CHECK(bits.bit_position()==35);
        auto truncated=packed.reader();
        CHECK(truncated.skip(7));
        std::array<std::int16_t,8> values{};
        values.fill(77);
        CHECK(!decode_original_bink_dc_delta_group(truncated,123,values));
        CHECK(truncated.bit_position()==7);
        for(auto x:values)CHECK(x==77);
    }
    {
        // Full controlled pipeline: compressed delta groups produce
        // DC WORD bytes consumed by original AC+quant+iDCT block path.
        TestBits widths;
        widths.put(3,4);widths.put(4,3);widths.put(0,1); // DC4
        widths.put(2,3);widths.put(1,1);               // DC2
        auto stream=widths.reader();
        std::array<std::int16_t,2> words{};
        const auto carry=decode_original_bink_dc_delta_group(
            stream,0,words);
        CHECK(carry&&*carry==2&&words[0]==4&&words[1]==2);
        std::array<std::uint8_t,4> bytes{};
        for(std::size_t i=0;i<words.size();++i) {
            const auto v=static_cast<std::uint16_t>(words[i]);
            bytes[2*i]=static_cast<std::uint8_t>(v);
            bytes[2*i+1]=static_cast<std::uint8_t>(v>>8);
        }
        BinkDcWordStream dc_stream{bytes,0};
        const std::array<std::uint8_t,2> ac_bytes{0,0};
        LsbBitReader ac_bits(ac_bytes);
        for(unsigned i=0;i<2;++i) {
            const auto result=decode_original_bink_block_from_dc_plane(
                dc_stream,ac_bits);
            CHECK(result&&result->dc==words[i]);
            CHECK(result->decoded.ac.mode==0&&result->decoded.quant_selector==0);
        }
        CHECK(dc_stream.position==4&&ac_bits.bit_position()==16);
    }
    {
        const std::array<std::uint8_t,1> one{0x0f};
        LsbBitReader bits(one);
        std::array<std::int16_t,9> too_long{};
        CHECK(!decode_original_bink_dc_delta_group(bits,5,too_long));
        CHECK(bits.bit_position()==0);
        std::array<std::int16_t,1> one_word{88};
        CHECK(!decode_original_bink_dc_delta_group(bits,5,one_word));
        CHECK(bits.bit_position()==0&&one_word[0]==88);
    }
    {
        // Full source 0x30010D94 loop: 8 signed samples of width2
        // followed by the remaining 2 samples from a zero-width
        // group. Validate the exact ten LE WORDs subsequently
        // consumed by the native block DC/AC handoff.
        TestBits packed;
        packed.put(2,4);
        packed.put(1,2);packed.put(0,1); // 3 -> 4
        packed.put(2,2);packed.put(1,1); // 4 -> 2
        packed.put(0,2);                 // 2
        packed.put(3,2);packed.put(0,1); // 2 -> 5
        for(unsigned k=0;k<4;++k)packed.put(0,2);
        packed.put(0,4);                 // remaining 2 -> 5,5
        auto compressed=packed.reader();
        const auto plane=decode_original_bink_dc_delta_plane(
            compressed,3,10);
        CHECK(plane&&plane->final_sample==5);
        CHECK(plane->consumed_bits==packed.bit_count&&packed.bit_count==27);
        CHECK(plane->signed_words_le.size()==20);
        CHECK(compressed.bit_position()==27);
        const std::array<std::int16_t,10> expected{
            4,2,2,5,5,5,5,5,5,5
        };
        BinkDcWordStream unpacked{plane->signed_words_le,0};
        for(auto v:expected) {
            CHECK(read_original_bink_dc_word(unpacked)==v);
        }
        CHECK(unpacked.position==20&&unpacked.remaining_words()==0);
        const std::array<std::uint8_t,2> ac_zero_blocks{0,0};
        LsbBitReader ac_bits(ac_zero_blocks);
        BinkDcWordStream first_two{plane->signed_words_le,0};
        for(unsigned i=0;i<2;++i) {
            const auto block=decode_original_bink_block_from_dc_plane(
                first_two,ac_bits);
            CHECK(block&&block->dc==expected[i]);
            CHECK(block->decoded.ac.mode==0&&block->decoded.quant_selector==0);
        }
        CHECK(first_two.position==4&&ac_bits.bit_position()==16);
        // Requesting one extra sample would need to consume the same
        // group but in a different alignment; use a deliberately
        // truncated stream instead to assert whole-plane rollback.
        auto bad=packed.reader();
        CHECK(bad.skip(29)); // only three padding bits remain
        CHECK(!decode_original_bink_dc_delta_plane(bad,3,10));
        CHECK(bad.bit_position()==29);
    }
    {
        const std::array<std::uint8_t,1> bits{0};
        LsbBitReader empty_plane(bits);
        CHECK(empty_plane.skip(3));
        const auto no_samples=decode_original_bink_dc_delta_plane(
            empty_plane,-32768,0);
        CHECK(no_samples&&no_samples->signed_words_le.empty());
        CHECK(no_samples->final_sample==-32768);
        CHECK(no_samples->consumed_bits==0&&empty_plane.bit_position()==3);
        CHECK(!decode_original_bink_dc_delta_plane(
            empty_plane,0,(1U<<20)+1));
        CHECK(empty_plane.bit_position()==3);
        // Need a complete nibble even to decode 1 DC sample.
        CHECK(empty_plane.skip(4));
        CHECK(!decode_original_bink_dc_delta_plane(empty_plane,0,1));
        CHECK(empty_plane.bit_position()==7);
    }
    {
        // Reconstruct one whole original 30010BF0 DC record.
        // count=3 (4 count bits); signed initial 100
        // (10 magnitude bits + sign 0); then two deltas:
        // +1 and -2, producing [100,101,99].
        TestBits encoded;
        encoded.put(3,4);     // count
        encoded.put(100,10);  // first unsigned magnitude
        encoded.put(0,1);     // positive first value
        encoded.put(2,4);     // delta magnitude width
        encoded.put(1,2);encoded.put(0,1); // +1
        encoded.put(2,2);encoded.put(1,1); // -2
        auto bits=encoded.reader();
        const auto dc=decode_original_bink_dc_context_packet(
            bits,4,11,true);
        CHECK(dc&&dc->signed_words_le.size()==6);
        CHECK(dc->final_sample==99&&dc->consumed_bits==encoded.bit_count);
        CHECK(bits.bit_position()==encoded.bit_count&&encoded.bit_count==25);
        BinkDcWordStream signed_values{dc->signed_words_le,0};
        CHECK(read_original_bink_dc_word(signed_values)==100);
        CHECK(read_original_bink_dc_word(signed_values)==101);
        CHECK(read_original_bink_dc_word(signed_values)==99);
        CHECK(signed_values.position==6);
        // Controlled end-to-end: a decoded original DC context buffer
        // supplies three AC-mode0/quant0 source blocks without the DLL.
        const std::array<std::uint8_t,3> ac_bytes{0,0,0};
        LsbBitReader ac_bits(ac_bytes);
        BinkDcWordStream source_words{dc->signed_words_le,0};
        const std::array<std::int16_t,3> expected{100,101,99};
        for(const auto sample:expected) {
            const auto block=decode_original_bink_block_from_dc_plane(
                source_words,ac_bits);
            CHECK(block&&block->dc==sample);
            CHECK(block->decoded.ac.mode==0&&block->decoded.quant_selector==0);
        }
        CHECK(ac_bits.bit_position()==24&&source_words.position==6);
    }
    {
        // Signed initial value with count=1. The sign is consumed
        // ONLY if the first magnitude is nonzero.
        TestBits negative;
        negative.put(1,4);negative.put(200,10);negative.put(1,1);
        auto bits=negative.reader();
        const auto dc=decode_original_bink_dc_context_packet(bits,4,11,true);
        CHECK(dc&&dc->final_sample==-200&&dc->consumed_bits==15);
        CHECK(dc->signed_words_le.size()==2&&bits.bit_position()==15);
        TestBits signed_zero;
        signed_zero.put(1,4);signed_zero.put(0,10);
        auto z=signed_zero.reader();
        const auto zero=decode_original_bink_dc_context_packet(z,4,11,true);
        CHECK(zero&&zero->final_sample==0&&z.bit_position()==14);
        TestBits unsigned_first;
        unsigned_first.put(1,4);unsigned_first.put(1023,11);
        auto u=unsigned_first.reader();
        const auto pos=decode_original_bink_dc_context_packet(u,4,11,false);
        CHECK(pos&&pos->final_sample==1023&&u.bit_position()==15);
    }
    {
        // Empty count consumes only its count bits, nothing more.
        const std::array<std::uint8_t,1> bytes{0};
        LsbBitReader bits(bytes);
        const auto dc=decode_original_bink_dc_context_packet(bits,4,11,true);
        CHECK(dc&&dc->signed_words_le.empty());
        CHECK(dc->consumed_bits==4&&bits.bit_position()==4);
        LsbBitReader bad(bytes);
        const auto before=bad.bit_position();
        CHECK(!decode_original_bink_dc_context_packet(bad,0,11,true));
        CHECK(!decode_original_bink_dc_context_packet(bad,4,17,true));
        CHECK(!decode_original_bink_dc_context_packet(bad,4,11,true,0));
        CHECK(bad.bit_position()==before);
        // Announced output count exceeds supplied 1-word allocation cap.
        const std::array<std::uint8_t,1> three{0x03};
        LsbBitReader capped(three);
        CHECK(!decode_original_bink_dc_context_packet(capped,4,11,true,1));
        CHECK(capped.bit_position()==0);
        // Missing sign bit must roll back the complete context record.
        TestBits too_short;
        too_short.put(0,2); // two-bit prior stream prefix
        too_short.put(1,4);too_short.put(27,10); // leaves no sign bit
        auto truncated=too_short.reader();
        CHECK(truncated.skip(2)); // now 14 intended bits remain
        CHECK(!decode_original_bink_dc_context_packet(
            truncated,4,11,true));
        CHECK(truncated.bit_position()==2);
    }
    std::puts("Bink original predecoded DC stream -> AC/quant IDCT 16x16 pixels: PASS");
}
