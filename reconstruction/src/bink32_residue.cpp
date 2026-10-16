#include "btb/bink32_residue.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
namespace {
// Original Bink1 8x8 pixel-domain residue/transform scan, 0..63.
constexpr std::array<std::uint8_t,64> kResidueScan{
     0, 1, 8, 9, 2, 3,10,11,
     4, 5,12,13, 6, 7,14,15,
    20,21,28,29,22,23,30,31,
    16,17,24,25,32,33,40,41,
    34,35,42,43,48,49,56,57,
    50,51,58,59,18,19,26,27,
    36,37,44,45,38,39,46,47,
    52,53,60,61,54,55,62,63
};
consteval bool valid_scan() {
    std::array<bool,64> seen{};
    for(const auto index:kResidueScan) {
        if(index>=64 || seen[index])return false;
        seen[index]=true;
    }
    return true;
}
static_assert(valid_scan());
struct Node { std::uint8_t offset{}; std::uint8_t mode{}; };
} // namespace

std::optional<BinkDecodedResidue> decode_bink_residue_coefficients(
    LsbBitReader& bits,unsigned event_quota) noexcept {
    if(event_quota>127)return std::nullopt;
    auto probe=bits;
    const auto begin=probe.bit_position();
    const auto exponent=probe.read(3);
    if(!exponent)return std::nullopt;
    std::int32_t quota=static_cast<std::int32_t>(event_quota);
    BinkDecodedResidue decoded{};
    std::array<Node,128> nodes{};
    std::size_t front=64,back=64;
    nodes[back++]={4,0};
    nodes[back++]={24,0};
    nodes[back++]={44,0};
    nodes[back++]={0,2};
    std::array<std::uint8_t,64> nonzero{};
    std::size_t nonzero_count=0;
    const auto finish=[&]() -> std::optional<BinkDecodedResidue> {
        decoded.consumed_bits=probe.bit_position()-begin;
        bits=probe;
        return decoded;
    };
    // Each bit plane may refine previously emitted coefficients and
    // subdivide at most 64 new leaves; fixed-size arrays bound memory.
    for(unsigned shift=*exponent+1;shift>0;--shift) {
        const std::int16_t mask=static_cast<std::int16_t>(
            1U<<(shift-1));
        for(std::size_t n=0;n<nonzero_count;++n) {
            const auto refine=probe.read(1);
            if(!refine)return std::nullopt;
            if(!*refine)continue;
            auto& coefficient=decoded.coefficients[nonzero[n]];
            coefficient=static_cast<std::int16_t>(
                coefficient+(coefficient<0?-mask:mask));
            ++decoded.refinement_events;
            if(--quota<0)return finish();
        }
        std::size_t position=front;
        while(position<back) {
            const Node current=nodes[position];
            if((current.offset==0 && current.mode==0)) {
                ++position;
                continue;
            }
            const auto active=probe.read(1);
            if(!active)return std::nullopt;
            if(!*active) {
                ++position;
                continue;
            }
            unsigned first=current.offset;
            switch(current.mode) {
            case 0: // Expand four adjacent coefficient candidates.
                if(first>60)return std::nullopt;
                nodes[position]={static_cast<std::uint8_t>(first+4),1};
                break;
            case 2: // Explicit group of four and retire current node.
                if(first>60)return std::nullopt;
                nodes[position]={0,0};
                ++position;
                break;
            case 1: // Split a stride-four group into three more groups.
                nodes[position].mode=2;
                if(back>125)return std::nullopt;
                for(unsigned i=0;i<3;++i) {
                    first+=4;
                    if(first>60)return std::nullopt;
                    nodes[back++]={static_cast<std::uint8_t>(first),2};
                }
                continue;
            case 3: { // Materialize a single deferred coefficient.
                if(first>=64 || nonzero_count>=64)
                    return std::nullopt;
                const auto sign=probe.read(1);
                if(!sign)return std::nullopt;
                const auto index=kResidueScan[first];
                nonzero[nonzero_count++]=index;
                decoded.coefficients[index]=*sign?-mask:mask;
                ++decoded.coefficient_events;
                nodes[position]={0,0};
                ++position;
                if(--quota<0)return finish();
                continue;
            }
            default:
                return std::nullopt;
            }
            for(unsigned i=0;i<4;++i) {
                if(first>=64)return std::nullopt;
                const auto defer=probe.read(1);
                if(!defer)return std::nullopt;
                if(*defer) {
                    if(front==0)return std::nullopt;
                    nodes[--front]={static_cast<std::uint8_t>(first),3};
                } else {
                    if(nonzero_count>=64)return std::nullopt;
                    const auto sign=probe.read(1);
                    if(!sign)return std::nullopt;
                    const auto index=kResidueScan[first];
                    nonzero[nonzero_count++]=index;
                    decoded.coefficients[index]=*sign?-mask:mask;
                    ++decoded.coefficient_events;
                    if(--quota<0)return finish();
                }
                ++first;
            }
        }
    }
    return finish();
}

std::optional<BinkDecodedResidue>
decode_bink_residue_packet(LsbBitReader& bits) noexcept {
    auto probe=bits;
    const auto begin=probe.bit_position();
    const auto budget=probe.read(7);
    if(!budget)return std::nullopt;
    auto decoded=decode_bink_residue_coefficients(probe,*budget);
    if(!decoded)return std::nullopt;
    decoded->consumed_bits=probe.bit_position()-begin;
    bits=probe;
    return decoded;
}
} // namespace btb::bink32_recovered
