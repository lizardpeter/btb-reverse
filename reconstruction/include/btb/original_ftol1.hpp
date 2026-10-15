#pragma once
#include "btb/original_float6.hpp"
namespace btb::original_ftol1 {
// 004304D0: masked x87 FISTP to signed int64, rounding toward zero.
// Raw 80-bit input avoids losing precision where native long double is binary64.
// Invalid input/overflow produces the original integer indefinite 0x8000...0000.
std::uint64_t truncate_integer(const original_float6::Extended10&) noexcept;
}
