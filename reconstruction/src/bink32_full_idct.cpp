#include "btb/bink32_full_idct.hpp"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>

namespace btb::bink32_recovered {
namespace {
// Original x86 uses 32-bit wrapping ADD/SUB/IMUL followed by SAR.
// Model these without signed-overflow UB on either MSVC or Clang/GCC.
[[nodiscard]] constexpr std::uint32_t u32(std::int32_t v) noexcept {
    return std::bit_cast<std::uint32_t>(v);
}
[[nodiscard]] constexpr std::int32_t i32(std::uint32_t v) noexcept {
    return std::bit_cast<std::int32_t>(v);
}
[[nodiscard]] constexpr std::int32_t add(std::int32_t a, std::int32_t b) noexcept {
    return i32(u32(a) + u32(b));
}
[[nodiscard]] constexpr std::int32_t sub(std::int32_t a, std::int32_t b) noexcept {
    return i32(u32(a) - u32(b));
}
[[nodiscard]] constexpr std::int32_t mul(std::int32_t a, std::int32_t b) noexcept {
    return i32(u32(a) * u32(b));
}
[[nodiscard]] constexpr std::int32_t sar(std::int32_t a, unsigned shift) noexcept {
    // C++20 onward specifies arithmetic shift for a negative signed integer.
    return a >> shift;
}
using Vector8 = std::array<std::int32_t, 8>;
// Derived from the even/odd butterfly operations 0x300113FF..30011554
// (vertical) and 0x30011590..300116AA (horizontal). Caller supplies
// already quantized+shifted inputs for the vertical pass and intermediate
// DWORDs for the horizontal pass.
[[nodiscard]] constexpr Vector8 butterfly(Vector8 a) noexcept {
    const auto e0 = add(a[0], a[4]);
    const auto e1 = sub(a[0], a[4]);
    const auto e2 = add(a[2], a[6]);
    const auto e3 = sub(sar(mul(sub(a[2], a[6]), 0xb50), 11), e2);
    const auto even0 = add(e0, e2);
    const auto even1 = add(e1, e3);
    const auto even2 = sub(e1, e3);
    const auto even3 = sub(e0, e2);

    const auto odd_low  = add(a[1], a[7]);
    const auto odd_high = add(a[3], a[5]);
    const auto odd_sum  = add(odd_low, odd_high);
    const auto odd_diff = sub(odd_low, odd_high);
    const auto odd_s1   = sub(a[1], a[7]);
    const auto odd_s3   = sub(a[5], a[3]);
    const auto bridge   = sar(mul(add(odd_s1, odd_s3), 0xec8), 11);
    const auto odd_a = add(sub(sar(mul(odd_s3, -0x14e8), 11), odd_sum), bridge);
    const auto odd_b = sub(sar(mul(odd_diff, 0xb50), 11), odd_a);
    const auto odd_c = add(sub(sar(mul(odd_s1, 0x8a9), 11), bridge), odd_b);
    return {
        add(even0, odd_sum),
        add(even1, odd_a),
        add(even2, odd_b),
        sub(even3, odd_c),
        add(even3, odd_c),
        sub(even2, odd_b),
        sub(even1, odd_a),
        sub(even0, odd_sum)
    };
}
[[nodiscard]] constexpr std::uint8_t rounded_byte(std::int32_t n) noexcept {
    return static_cast<std::uint8_t>(sar(add(n, 127), 8));
}
} // namespace

std::optional<BinkFullIdctPixels> bink_idct_16x16(
    std::span<const std::int16_t> coefficients,
    std::span<const std::int32_t> quant_matrix) noexcept {
    if (coefficients.size() != 64 || quant_matrix.size() != 64)
        return std::nullopt;
    std::array<Vector8, 8> intermediate{}; // vertical 8x8 (DWORD scratch)
    for (std::size_t x = 0; x < 8; ++x) {
        Vector8 input{};
        bool zero_ac = true;
        for (std::size_t y = 0; y < 8; ++y) {
            const auto index = y * 8 + x;
            input[y] = sar(mul(coefficients[index], quant_matrix[index]), 11);
            if (y && coefficients[index] != 0) zero_ac = false;
        }
        if (zero_ac) {
            // Original 0x300113CE..0x300113FA duplicates DC vertically.
            for (auto& row : intermediate) row[x] = input[0];
        } else {
            const auto result = butterfly(input);
            for (std::size_t y = 0; y < 8; ++y) intermediate[y][x] = result[y];
        }
    }
    BinkFullIdctPixels pixels{};
    for (std::size_t y = 0; y < 8; ++y) {
        const auto row = butterfly(intermediate[y]);
        for (std::size_t x = 0; x < 8; ++x) {
            const auto p = rounded_byte(row[x]);
            // Original 0x300116AD..300116F4 duplicates the output
            // horizontally (byte -> word -> DWORD) and vertically (two
            // destination pointers separated by one row stride).
            pixels[ (2*y)      * 16 + 2*x    ] = p;
            pixels[ (2*y)      * 16 + 2*x + 1] = p;
            pixels[ (2*y + 1)  * 16 + 2*x    ] = p;
            pixels[ (2*y + 1)  * 16 + 2*x + 1] = p;
        }
    }
    return pixels;
}
} // namespace btb::bink32_recovered
