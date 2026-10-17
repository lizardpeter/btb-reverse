#include "btb/bink32_dct_coefficients.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::bink32_recovered {
namespace {
constexpr std::array<std::uint8_t,64> kBinkDctScan{
     0, 1, 8, 9, 2, 3,10,11,
     4, 5,12,13, 6, 7,14,15,
    20,21,28,29,22,23,30,31,
    16,17,24,25,32,33,40,41,
    34,35,42,43,48,49,56,57,
    50,51,58,59,18,19,26,27,
    36,37,44,45,38,39,46,47,
    52,53,60,61,54,55,62,63
};
consteval bool scan_is_permutation() {
    std::array<bool,64> seen{};
    for(const auto index:kBinkDctScan) {
        if(index>=64 || seen[index])return false;
        seen[index]=true;
    }
    return true;
}
static_assert(scan_is_permutation());
struct Node {std::uint8_t offset{}; std::uint8_t mode{};};
} // namespace

std::optional<BinkDctCoefficients> decode_bink_dct_coefficients(
    LsbBitReader& bits,std::int16_t dc,
    std::optional<std::uint8_t> selector_override) noexcept {
    if(selector_override && *selector_override>=16)
        return std::nullopt;
    auto probe=bits;
    const auto start=probe.bit_position();
    const auto levels=probe.read(4);
    if(!levels)return std::nullopt;
    BinkDctCoefficients output{};
    output.values[0]=dc;
    output.magnitude_planes=static_cast<std::uint8_t>(*levels);
    std::array<Node,128> tree{};
    std::size_t front=64,back=64;
    tree[back++]={4,0};
    tree[back++]={24,0};
    tree[back++]={44,0};
    tree[back++]={1,3};
    tree[back++]={2,3};
    tree[back++]={3,3};
    const auto put_coefficient=[&](unsigned scan_index,
                                  unsigned magnitude_shift)
        -> bool {
        if(scan_index>=64 || output.changed_count>=64)
            return false;
        std::uint32_t magnitude=1U<<magnitude_shift;
        if(magnitude_shift) {
            const auto low=probe.read(magnitude_shift);
            if(!low)return false;
            magnitude|=*low;
        }
        const auto sign=probe.read(1);
        if(!sign)return false;
        const auto value=static_cast<std::int32_t>(magnitude);
        output.values[kBinkDctScan[scan_index]]=
            *sign?-value:value;
        output.changed_scan_indices[output.changed_count++]=
            static_cast<std::uint8_t>(scan_index);
        return true;
    };
    // A value of zero in the original 4-bit AC plane header means
    // DC-only block: skip directly to the quant selector.
    for(unsigned plane=*levels;plane>0;--plane) {
        const unsigned magnitude_shift=plane-1;
        std::size_t cursor=front;
        while(cursor<back) {
            const Node node=tree[cursor];
            if(!node.offset && !node.mode) {
                ++cursor;
                continue;
            }
            const auto active=probe.read(1);
            if(!active)return std::nullopt;
            if(!*active) {
                ++cursor;
                continue;
            }
            unsigned first=node.offset;
            switch(node.mode) {
            case 0:
                // Convert the original group into a stride-four
                // expansion placeholder, keeping this node active.
                if(first>60)return std::nullopt;
                tree[cursor]={static_cast<std::uint8_t>(first+4),1};
                break;
            case 2:
                // A preexpanded four-coefficient leaf retires once
                // its four children have been emitted/deferred.
                tree[cursor]={0,0};
                ++cursor;
                break;
            case 1:
                // Split a stride-16 subgroup into three sibling
                // four-coefficient groups. Process them in a later
                // bit plane, exactly like the original decoder.
                tree[cursor].mode=2;
                if(back>125)return std::nullopt;
                for(unsigned i=0;i<3;++i) {
                    first+=4;
                    if(first>60)return std::nullopt;
                    tree[back++]={static_cast<std::uint8_t>(first),2};
                }
                continue;
            case 3:
                if(!put_coefficient(first,magnitude_shift))
                    return std::nullopt;
                tree[cursor]={0,0};
                ++cursor;
                continue;
            default:
                return std::nullopt;
            }
            if(first>60)return std::nullopt;
            for(unsigned i=0;i<4;++i) {
                const auto deferred=probe.read(1);
                if(!deferred)return std::nullopt;
                if(*deferred) {
                    if(front==0)return std::nullopt;
                    tree[--front]={static_cast<std::uint8_t>(first+i),3};
                } else if(!put_coefficient(first+i,magnitude_shift)) {
                    return std::nullopt;
                }
            }
        }
    }
    if(selector_override) {
        output.quant_selector=*selector_override;
    } else {
        const auto q=probe.read(4);
        if(!q)return std::nullopt;
        output.quant_selector=static_cast<std::uint8_t>(*q);
    }
    output.consumed_bits=probe.bit_position()-start;
    bits=probe;
    return output;
}
} // namespace btb::bink32_recovered
