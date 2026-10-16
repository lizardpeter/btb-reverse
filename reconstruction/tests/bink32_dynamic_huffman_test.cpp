#include "btb/bink32_dynamic_huffman.hpp"
#include <algorithm>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <span>
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
int main() {
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
    std::puts("Original Bink dynamic Huffman permutation and merge: PASS");
}
