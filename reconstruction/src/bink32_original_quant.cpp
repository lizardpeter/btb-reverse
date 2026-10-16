#include "btb/bink32_original_quant.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::bink32_recovered {
namespace {
// Faithful reconstruction of the 4,096 bytes at VA 0x30049E40 of the
// original 2002 Bink32 PE32 image. There are 16 contiguous 64-DWORD
// tables and the selector is read as four bits at original 0x3000F0ED;
// internal 0x30011729 computes 0x30049E40 + (selector << 8).
//
// To keep source legible, the exact table #15 is the source anchor.
// All other values are their rational multiples with nearest-even
// rounding, plus the 62 one-unit corrections needed to match the
// original PRECOMPUTED x86 table bytes. Reconstitution is consteval,
// so the runtime has no DLL, file or decoding dependency whatsoever.
constexpr std::array<std::uint32_t,64> anchor_15{
    2883584,4249624,4003058,3814578,3244032,2548820,1853194,944750,
    4749580,6587856,6532237,5878858,4999558,3928127,2705741,1448344,
    4944954,6858849,6460894,5814651,4944954,4070236,2803628,1429277,
    4662262,6466744,6091539,5482241,4874183,3829621,2637889,1344784,
    4145152,5749491,5415902,4874183,4325376,3398426,2340877,1243092,
    3398426,4713753,4440258,4162632,3681628,2892637,1992483,1015759,
    2438413,3652750,3440816,3096652,2633486,2069117,1478019,753488,
    1392263,1931125,1949014,1754066,1491711,1211097,834218,452718
};
// Exact original quant multipliers compared with table 0:
// 1, 4/3, 5/3, 2, 8/3, 7/2, 4, 5, 6, 8, 12, 17, 22, 28, 34, 44.
struct Fraction {std::uint8_t num{},den{};};
constexpr std::array<Fraction,16> factors{{
    {1,1},{4,3},{5,3},{2,1},{8,3},{7,2},{4,1},{5,1},
    {6,1},{8,1},{12,1},{17,1},{22,1},{28,1},{34,1},{44,1}
}};
struct Correction {std::uint8_t table{},coefficient{};std::int8_t delta{};};
// The following integer corrections are measured against the original
// 2002 DLL's bytes. No tolerance or approximate table values are used.
constexpr std::array<Correction,62> corrections{{
    {0,18,1},{0,24,1},
    {2,15,-1},
    {3,33,1},{3,38,-1},{3,41,-1},{3,45,-1},
    {4,12,-1},{4,34,1},
    {5,8,-1},
    {6,27,-1},
    {7,15,-1},{7,18,1},{7,24,1},
    {9,12,-1},{9,22,-1},{9,34,1},{9,61,1},
    {10,3,1},{10,27,-1},{10,37,1},{10,40,1},{10,51,1},{10,52,1},
    {11,13,1},{11,16,1},{11,18,1},{11,20,1},{11,24,1},{11,27,-1},
    {12,14,1},{12,29,1},{12,30,1},{12,46,-1},{12,47,-1},
    {12,48,1},{12,54,-1},{12,60,-1},{12,61,1},
    {13,25,-1},{13,27,-1},{13,48,1},{13,49,1},{13,54,-1},{13,60,-1},
    {14,3,1},{14,5,-1},{14,12,-1},{14,17,-1},{14,19,1},
    {14,21,1},{14,26,1},{14,33,1},{14,37,1},{14,38,-1},
    {14,40,1},{14,41,-1},{14,44,1},{14,45,-1},
    {14,51,1},{14,52,1},{14,55,-1}
}};
constexpr std::uint32_t nearest_even(std::uint64_t numerator,
                                    std::uint32_t divisor) noexcept {
    auto whole=static_cast<std::uint32_t>(numerator/divisor);
    const auto remainder=numerator%divisor;
    if(2*remainder>divisor ||
       (2*remainder==divisor && (whole&1U)!=0))++whole;
    return whole;
}
consteval std::array<BinkOriginalQuantMatrix,16> create_quant_tables() {
    std::array<BinkOriginalQuantMatrix,16> out{};
    for(std::size_t t=0;t<16;++t) {
        for(std::size_t i=0;i<64;++i) {
            const auto numerator=std::uint64_t(anchor_15[i])*factors[t].num;
            const auto denominator=std::uint32_t(44)*factors[t].den;
            out[t][i]=static_cast<std::int32_t>(
                nearest_even(numerator,denominator));
        }
    }
    for(const auto c:corrections)out[c.table][c.coefficient]+=c.delta;
    return out;
}
constexpr auto tables=create_quant_tables();
constexpr std::uint64_t original_dword_fnv64() noexcept {
    std::uint64_t hash=0xcbf29ce484222325ULL;
    for(const auto& table:tables)for(const auto val:table) {
        for(unsigned shift=0;shift<32;shift+=8) {
            const auto byte=static_cast<std::uint8_t>(
                static_cast<std::uint32_t>(val)>>shift);
            hash=(hash^byte)*0x100000001b3ULL;
        }
    }
    return hash;
}
// Raw image bytes 0x30049E40 .. 0x3004AE3F SHA-256:
// 4442cb0c6670d276dde6a5cbf51ba4567819cd916d5036f0f306a34c1351404b
// Checking all original DWORDs catches any accidental data/source drift.
static_assert(original_dword_fnv64()==0x9b813dd7a89e7b3fULL,
              "Recovered quantization tables differ from the original DLL");
static_assert(tables[0][0]==65536 && tables[1][0]==87381);
static_assert(tables[15][0]==2883584 && tables[15][63]==452718);
}
std::optional<std::span<const std::int32_t,64>>
original_quant_matrix(std::uint8_t selector) noexcept {
    if(selector>=kOriginalBinkQuantTables)return std::nullopt;
    return std::span<const std::int32_t,64>(tables[selector]);
}
std::optional<BinkFullIdctPixels> bink_idct_original_quant(
    std::span<const std::int16_t> coefficients,
    std::uint8_t selector) noexcept {
    const auto quant=original_quant_matrix(selector);
    if(!quant)return std::nullopt;
    return bink_idct_16x16(coefficients,*quant);
}
std::optional<BinkFullIdctPixels> decode_original_quantized_block(
    LsbBitReader& compressed,
    std::span<const std::int16_t> coefficients) noexcept {
    // The caller already expanded one block's coefficient bytes;
    // 3000F0ED consumes a FOUR-BIT quantization index and 3000F107
    // invokes original 30011720 with the selected matrix.
    auto cursor=compressed;
    const auto selector=cursor.read(4);
    if(!selector)return std::nullopt;
    const auto pixels=bink_idct_original_quant(
        coefficients,static_cast<std::uint8_t>(*selector));
    if(!pixels)return std::nullopt;
    compressed=cursor;
    return pixels;
}
} // namespace btb::bink32_recovered
