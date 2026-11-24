#pragma once
#include "btb/original_crt_batch_32.hpp"
namespace btb::original_float6 {
using namespace btb::original_crt32;
// Original memory order, including the extra low 16 mantissa bits in LD12.
using Extended12=std::array<U16,6>;
struct Extended10 {U32 low{},high{};U16 sign_exponent{};};
using Format=std::array<I32,6>;
inline constexpr Format double_format{1024,-1023,53,11,64,1023};
inline constexpr Format float_format{128,-127,24,8,32,127};
// 00437520, 00437713, 004377E1, 0043786E, 00437B3B, 004388A1.
U32 control_word_from_runtime(U32) noexcept;
U32 round_mantissa(Words96&,unsigned retained_bits);
void shift_mantissa_right(Words96&,unsigned count) noexcept;
U32 convert_extended(const Extended12&,std::span<U32> result,const Format&);
Extended10 double_to_extended(U64 bits) noexcept;
// Numeric digit bytes, not ASCII. Zero would loop forever in the retail routine.
Words96 decimal_digits_to_extended(std::span<const U8> digits);
// Direct original 004379DA / 004379F0 callers, now with real dependencies.
void decode_double(const Extended12&,std::span<U32>);
void decode_float(const Extended12&,std::span<U32>);
bool verify_6_original_float_functions();
}
