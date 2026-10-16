#include "btb/bink32_dynamic_huffman.hpp"
#include <array>
#include <cstdint>
#include <span>

namespace btb::bink32_recovered {
bool interleave_huffman_subtrees(
    LsbBitReader& bits, std::span<const std::uint8_t> left,
    std::span<const std::uint8_t> right,
    std::span<std::uint8_t> destination) noexcept {
    if(left.size()!=right.size() || destination.size()!=left.size()*2 ||
       left.empty() || left.size()>8)return false;
    auto work=bits;
    std::size_t i=0,j=0,k=0;
    // Original 30010100..3001019F: choose one item from either input
    // as indicated by a bit, then copy remainder without consuming bits.
    while(i<left.size() && j<right.size()) {
        const auto choose_right=work.read(1);
        if(!choose_right)return false;
        destination[k++]=*choose_right ? right[j++] : left[i++];
    }
    while(i<left.size())destination[k++]=left[i++];
    while(j<right.size())destination[k++]=right[j++];
    bits=work;
    return true;
}
std::optional<DynamicHuffmanTree> read_dynamic_huffman_tree(
    LsbBitReader& bits) noexcept {
    auto input=bits;
    const auto first=input.bit_position();
    const auto prefix=read_entropy_tree_prefix(input);
    if(!prefix)return std::nullopt;
    DynamicHuffmanTree tree{};
    tree.selector=prefix->selector;
    auto& out=tree.symbol_permutation;
    if(!tree.selector) {
        // Original 3000FD32 fills sixteen zeros.
        out.fill(0);
    } else if(prefix->branch_mode==1) {
        // Original 3000FFE7..300100C2: read a 3-bit length minus one,
        // then chosen 4-bit symbols. Fill unused symbols in ascending order.
        const auto count= input.read(3);
        if(!count)return std::nullopt;
        const unsigned explicit_count=*count+1;
        std::uint16_t missing=0xffffU;
        for(unsigned i=0;i<explicit_count;++i) {
            const auto symbol=input.read(4);
            // Original code is not robust against duplicate selections;
            // reject them here so malformed files cannot write past 16.
            if(!symbol||!(missing&(1U<<*symbol)))return std::nullopt;
            out[i]=static_cast<std::uint8_t>(*symbol);
            missing=static_cast<std::uint16_t>(missing&~(1U<<*symbol));
        }
        unsigned n=explicit_count;
        for(unsigned s=0;s<16;++s)
            if(missing&(1U<<s))out[n++]=static_cast<std::uint8_t>(s);
        if(n!=16)return std::nullopt;
    } else {
        // Original 3000FDC1/3000FE36: 8 independently orientable symbol
        // pairs. Submode 0 ends there; modes 1/2/3 perform 1/2/3 levels
        // of stable interleave using the original 0x300100E0 helper.
        std::array<std::uint8_t,16> level{};
        for(unsigned pair=0;pair<8;++pair) {
            const auto swap=input.read(1);
            if(!swap)return std::nullopt;
            level[2*pair]=static_cast<std::uint8_t>(2*pair+(*swap?1:0));
            level[2*pair+1]=static_cast<std::uint8_t>(2*pair+(*swap?0:1));
        }
        for(unsigned stage=0;stage<prefix->submode;++stage) {
            const std::size_t half=std::size_t{2}<<stage;
            std::array<std::uint8_t,16> next{};
            for(std::size_t i=0;i<16;i+=2*half) {
                if(!interleave_huffman_subtrees(
                    input,
                    std::span<const std::uint8_t>(level).subspan(i,half),
                    std::span<const std::uint8_t>(level).subspan(i+half,half),
                    std::span<std::uint8_t>(next).subspan(i,2*half)))
                    return std::nullopt;
            }
            level=next;
        }
        out=level;
    }
    tree.consumed_bits=input.bit_position()-first;
    bits=input;
    return tree;
}
std::optional<FixedHuffmanSymbol> read_dynamic_huffman_symbol(
    LsbBitReader& bits, const DynamicHuffmanTree& tree) noexcept {
    auto probe=bits;
    const auto raw=read_fixed_huffman_symbol(probe,tree.selector);
    if(!raw||raw->value>=16)return std::nullopt;
    FixedHuffmanSymbol decoded{
        tree.symbol_permutation[raw->value],raw->consumed_bits
    };
    bits=probe;
    return decoded;
}
std::optional<BlockEntropyTrees> read_block_entropy_trees(
    LsbBitReader& bits) noexcept {
    // Calls at 3000E59B, 3000E5B9, sixteen iterations at 3000E5DE,
    // then 3000E608/25/43/61/82; no video coefficient decoding yet.
    auto probe=bits;
    const auto first=probe.bit_position();
    BlockEntropyTrees output{};
    for(auto& tree:output.trees) {
        const auto next=read_dynamic_huffman_tree(probe);
        if(!next)return std::nullopt;
        tree=*next;
    }
    output.consumed_bits=probe.bit_position()-first;
    bits=probe;
    return output;
}

} // namespace btb::bink32_recovered
