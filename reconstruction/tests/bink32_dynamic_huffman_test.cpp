#include "btb/bink32_dynamic_huffman.hpp"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <span>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdint>
using namespace btb::bink32_recovered;
#define CHECK(x) do {if(!(x)) {std::fprintf(stderr,"Bink tree line %d: %s\n",__LINE__,#x);std::abort();}}while(0)
struct Bits {
    std::array<std::uint8_t,32> bytes{};
    unsigned count{};
    void put(unsigned n,unsigned width) {
        for(unsigned j=0;j<width;++j){
            if((n>>j)&1U)bytes[count/8]|=1U<<(count%8);
            ++count;
        }
    }
    auto reader() const {
        return LsbBitReader(std::span<const std::uint8_t>(bytes)
                            .first((count+7)/8));
    }
};
static void verify_permutation(const std::array<std::uint8_t,16>& p) {
    auto sorted=p;std::sort(sorted.begin(),sorted.end());
    for(unsigned j=0;j<16;++j)CHECK(sorted[j]==j);
}
int main(int argc,char**argv) {
    {
        Bits b;b.put(0,4);auto r=b.reader();
        const auto tree=read_dynamic_huffman_tree(r);
        CHECK(tree&&tree->selector==0&&tree->consumed_bits==4);
        CHECK(r.bit_position()==4);
        for(const auto x:tree->symbol_permutation)CHECK(x==0);
    }
    for(unsigned submode=0;submode<4;++submode) {
        Bits b;b.put(1,4);b.put(0,1);b.put(submode,2);
        for(unsigned i=0;i<8;++i)b.put(0,1);
        for(unsigned stage=0;stage<submode;++stage)
            for(unsigned g=0;g<(4U>>stage);++g)
                for(unsigned i=0;i<(2U<<stage);++i)b.put(0,1);
        auto r=b.reader();const auto tree=read_dynamic_huffman_tree(r);
        CHECK(tree&&tree->consumed_bits==b.count&&r.bit_position()==b.count);
        verify_permutation(tree->symbol_permutation);
        for(unsigned i=0;i<16;++i)CHECK(tree->symbol_permutation[i]==i);
    }
    {
        Bits b;b.put(1,4);b.put(0,1);b.put(0,2);
        for(unsigned i=0;i<8;++i)b.put(1,1);
        auto r=b.reader();const auto tree=read_dynamic_huffman_tree(r);
        CHECK(tree&&tree->consumed_bits==15);
        for(unsigned i=0;i<16;++i)CHECK(tree->symbol_permutation[i]==(i^1U));
    }
    {
        Bits b;b.put(1,4);b.put(1,1);b.put(0,3);b.put(5,4);
        auto r=b.reader();const auto tree=read_dynamic_huffman_tree(r);
        CHECK(tree&&tree->consumed_bits==12&&r.bit_position()==12);
        CHECK(tree->symbol_permutation[0]==5&&tree->symbol_permutation[1]==0);
        verify_permutation(tree->symbol_permutation);
    }
    {
        Bits b;b.put(1,4);b.put(1,1);b.put(1,3);b.put(5,4);b.put(5,4);
        auto r=b.reader();CHECK(!read_dynamic_huffman_tree(r));
        CHECK(r.bit_position()==0);
    }
    {
        Bits b;b.put(1,4);b.put(0,1);b.put(3,2);b.put(1,1);
        auto r=b.reader();CHECK(!read_dynamic_huffman_tree(r));
        CHECK(r.bit_position()==0);
    }
    {
        std::array<std::uint8_t,4> dst{};
        constexpr std::array<std::uint8_t,2> a{0,2},b{1,3};
        Bits bits;bits.put(1,1);bits.put(0,1);bits.put(1,1);
        auto reader=bits.reader();
        CHECK(interleave_huffman_subtrees(reader,a,b,dst));
        constexpr std::array<std::uint8_t,4> expected{1,0,3,2};
        CHECK(dst==expected&&reader.bit_position()==3);
    }
    {
        std::array<std::uint8_t,1> bits{0};
        LsbBitReader reader(bits);
        DynamicHuffmanTree tree{};
        tree.selector=1;
        for(unsigned i=0;i<16;++i)tree.symbol_permutation[i]=static_cast<std::uint8_t>(i);
        tree.symbol_permutation[0]=9;
        const auto symbol=read_dynamic_huffman_symbol(reader,tree);
        CHECK(symbol&&symbol->value==9&&symbol->consumed_bits==1);
        CHECK(reader.bit_position()==1);
    }
    {
        // The source original calls the 16-symbol tree setup 23 times
        // at 3000E582..3000E682 before decoding block coefficients.
        Bits encoded;
        for(unsigned i=0;i<23;++i)encoded.put(0,4);
        auto reader=encoded.reader();
        const auto set=read_block_entropy_trees(reader);
        CHECK(set&&set->consumed_bits==92&&reader.bit_position()==92);
        for(const auto& tree:set->trees) {
            CHECK(tree.selector==0);
            for(const auto symbol:tree.symbol_permutation)CHECK(symbol==0);
        }
        auto truncated=encoded.reader();
        CHECK(truncated.skip(8));
        CHECK(!read_block_entropy_trees(truncated));
        CHECK(truncated.bit_position()==8);
    }
    // Optional original-asset regression, performed locally using the
    // supplied retail movies; GitHub CI runs the synthetic tests above.
    // Four bytes is the candidate compressed video control word boundary.
    // Passing this is NOT evidence of coefficient/pixel equivalence.
    std::uint64_t frame_headers=0,nonzero_selectors=0;
    for(int arg=1;arg<argc;++arg) {
        std::ifstream input(argv[arg],std::ios::binary);
        CHECK(input.good());
        const std::vector<std::uint8_t> raw{
            std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>()};
        const auto index=parse_container_index(raw);
        CHECK(index);
        for(std::uint32_t frame=1;frame<=index->header.frame_count;++frame) {
            const auto packet=split_encoded_frame(*index,raw,frame);
            CHECK(packet&&packet->video.size()>4);
            LsbBitReader cursor(packet->video.subspan(4));
            const auto entropy=read_block_entropy_trees(cursor);
            CHECK(entropy&&entropy->consumed_bits>0);
            CHECK(cursor.bit_position()==entropy->consumed_bits);
            for(const auto& tree:entropy->trees) {
                nonzero_selectors+=tree.selector!=0;
                CHECK(tree.selector<16);
            }
            ++frame_headers;
        }
    }
    if(argc>1)std::printf("Retail Bink tree-header candidates: %llu frames, %llu nonzero selectors\\n",
        static_cast<unsigned long long>(frame_headers),
        static_cast<unsigned long long>(nonzero_selectors));

    std::puts("Original Bink dynamic Huffman permutation and merge: PASS");
}
