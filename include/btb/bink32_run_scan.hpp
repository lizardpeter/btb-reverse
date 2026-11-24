#pragma once
// Original Bink1 8x8 run pixel orders, source-compatible lookup data.
// Independent reference: FFmpeg libavcodec/binkdata.h bink_patterns.
// Each pattern is a permutation of 0..63, with x=position&7 and
// y=position>>3. The byte contents are a codec-defined lookup table,
// not a dependency on FFmpeg at runtime.
#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::bink32_recovered {
namespace detail {
constexpr std::uint8_t bink_hex_digit(char c) noexcept {
    return static_cast<std::uint8_t>(
        c<='9'? c-'0' : c-'a'+10);
}
constexpr char kBinkRunPatternsHex[] =
    "00081018202830383931292119110901020a121a222a323a3b332b231b130b03040c141c242c343c3d352d251d150d05060e161e262e363e3f372f271f170f07"
    "3b3a3938303132332b2a2928202122231b1a1918101112130b0a090800010203040506070f0e0d0c141516171f1e1d1c242526272f2e2d2c343536373f3e3d3c"
    "1911121a1b130b03020a090100081018202830383931292a323a3b332b2322211d15161e1f170f07060e0d05040c141c242c343c3d352d2e363e3f372f272625"
    "030b020a0109000810181119121a131b232b222a2129202830383139323a333b3c343d353e363f372f272e262d252c241c141d151e161f170f070e060d050c04"
    "181910110809000102030a0b12131a1b1c1d14150c0d040506070e0f16171e1f27262f2e37363f3e3d3c35342d2c252423222b2a33323b3a3938313029282120"
    "0001020308090a0b1011121318191a1b2021222328292a2b3031323338393a3b040506070c0d0e0f141516171c1d1e1f242526272c2d2e2f343536373c3d3e3f"
    "06070f0e0d050c04030b020a0901000810181119121a131b141c151d161e171f272f262e252d242c232b222a21292028313038393a323b333c343d3536373f3e"
    "00010203040506070f0e0d0c0b0a090810111213141516171f1e1d1c1b1a191820212223242526272f2e2d2c2b2a292830313233343536373f3e3d3c3b3a3938"
    "0008090102030b0a12131b1a191110182028292122232b2a32313038393a3b33343c3d3e3f3736352d2c2425262e2f271f17161e1d1c14150d0c0405060e0f07"
    "181910110809000102030a0b12131a1b1c1d14150c0d040506070e0f16171e1f26272e2f36373e3f3c3d34352c2d242522232a2b32333a3b3839303128292021"
    "00080109020a030b131b121a1119101820282129222a232b333b323a313930383c343d353e363f372f272e262d252c241f171e161d151c140c040d050e060f07"
    "00081018191a1b130b0302010911120a040c141c1d1e1f170f0706050d15160e242c343c3d3e3f372f2726252d35362e20283038393a3b332b2322212931322a"
    "0008090102030b0a131b1a1211101819212028292a22232b333b3a3231393830343c3d35363e3f372f27262e2d2c24251d1c1415161e1f170e0f0706050d0c04"
    "181008000102030b131b1a19110a09121c140c040506070f171f1e1d150e0d163c342c242526272f373f3e3d352e2d36383028202122232b333b3a39312a2932"
    "00080901020a12111018191a1b130b0307060e0f1716150d05040c141c1d1e1f3f3e36372f2e2d353d3c342c24252627383031393a322a2928202122232b333b"
    "000108091011181920212829303138393a3b32332a2b22231a1b12130a0b020304050c0d14151c1d24252c2d34353c3d3e3f36372e2f26271e1f16170e0f0607";
consteval std::array<std::array<std::uint8_t,64>,16>
make_bink_run_patterns() {
    std::array<std::array<std::uint8_t,64>,16> result{};
    for(std::size_t i=0;i<16;++i)
        for(std::size_t j=0;j<64;++j) {
            const std::size_t p=(i*64+j)*2;
            result[i][j]=static_cast<std::uint8_t>(
                (bink_hex_digit(kBinkRunPatternsHex[p])<<4) |
                 bink_hex_digit(kBinkRunPatternsHex[p+1]));
        }
    return result;
}
inline constexpr auto kBinkRunPatterns=make_bink_run_patterns();
consteval bool valid_bink_run_patterns() {
    for(const auto& pattern:kBinkRunPatterns) {
        std::array<bool,64> seen{};
        for(auto pixel:pattern) {
            if(pixel>=64 || seen[pixel])return false;
            seen[pixel]=true;
        }
    }
    return true;
}
static_assert(valid_bink_run_patterns());
static_assert(kBinkRunPatterns[0][0]==0);
static_assert(kBinkRunPatterns[0][8]==57);
static_assert(kBinkRunPatterns[15][63]==7);
} // namespace detail
[[nodiscard]] constexpr std::uint8_t bink_run_scan_pixel(
    std::uint8_t pattern,std::size_t ordinal) noexcept {
    return detail::kBinkRunPatterns[pattern][ordinal];
}
} // namespace btb::bink32_recovered
