// Optional x86-64 developer diagnostic (GNU C++ / Clang).
// Build: g++ -std=c++20 -O2 -Iinclude tools/verify_retail_golf_x87.cpp src/golf_runtime.cpp -o /tmp/btb-golf-x87
// Run: /tmp/btb-golf-x87
// Original PE SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
// 00414A30 x87 instruction sequence transcribed from the verified image;
// host addressing only is adapted. This is not whole PE execution.
#include "btb/golf_runtime.hpp"
#include <bit>
#include <cmath>
#include <cstdint>
#include <iostream>

#if !defined(__x86_64__) || (!defined(__GNUC__) && !defined(__clang__))
#error This optional diagnostic requires GNU-style x87 inline assembly on x86-64
#endif

namespace {
using btb::golf::Vec2f;
constexpr double original_constant=0x1.1df5691cb5231p-6;

Vec2f original_vector(int deg,int mag) {
    Vec2f v{};
    asm volatile(
        "fildl %[deg]\n\t"
        "fmull %[constant]\n\t"
        "fildl %[mag]\n\t"
        "fld %%st(1)\n\t"
        "fcos\n\t"
        "fmul %%st(1),%%st\n\t"
        "fstps %[x]\n\t"
        "fxch %%st(1)\n\t"
        "fsin\n\t"
        "fmul %%st(1),%%st\n\t"
        "fxch %%st(1)\n\t"
        "fstp %%st(0)\n\t"
        "fstps %[y]"
        : [x]"=m"(v.x), [y]"=m"(v.y)
        : [deg]"m"(deg), [mag]"m"(mag), [constant]"m"(original_constant)
        : "st","st(1)","st(2)");
    return v;
}
bool same_bits(Vec2f a,Vec2f b) {
    return std::bit_cast<std::uint32_t>(a.x)==std::bit_cast<std::uint32_t>(b.x)
        && std::bit_cast<std::uint32_t>(a.y)==std::bit_cast<std::uint32_t>(b.y);
}
}
int main() {
    int count=0,mismatches=0,old_mismatches=0;
    for(int angle=-2;angle<=88;++angle)for(int magnitude=0;magnitude<=140;++magnitude) {
        const auto expected=original_vector(angle,magnitude);
        const auto actual=btb::golf::vector_from_degrees_and_magnitude(angle,magnitude);
        const double old_rad=angle*(3.14159265358979323846/180.0);
        const Vec2f old{float(std::cos(old_rad))*magnitude,float(std::sin(old_rad))*magnitude};
        ++count;
        mismatches+=!same_bits(expected,actual);
        old_mismatches+=!same_bits(expected,old);
    }
    std::cout<<count<<" cases, revised mismatches "<<mismatches
        <<", prior mismatches "<<old_mismatches<<'\n';
    return mismatches!=0;
}
