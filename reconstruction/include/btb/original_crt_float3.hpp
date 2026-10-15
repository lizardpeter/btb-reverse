#pragma once
// Source-level recoveries from original runtime functions:
// 004332F2 exponent, 004333F6 fixed, 004334D4 general selection.
// The original uses FUN_00437AD7/FUN_00437A60 to extract and round
// the x87 decimal digit record. A native provider is required; these
// functions DO NOT yet claim bit-exact retail x87 formatting.
#include <cstdint>
#include <string>
#include <string_view>
namespace btb::original_crt_float3 {
using I32=std::int32_t;
using U64=std::uint64_t;
// Representation returned by original 00437AD7: signed digit buffer at
// +0xC and decimal-point position in the second int32 field.
struct DecimalRecord {
 bool negative{};
 I32 decimal_position{1};
 std::string digits{"0"};
 bool special{};
};
class IO {
public:
 virtual ~IO()=default;
 virtual DecimalRecord extract_original_decimal(U64 original_double_bits)=0;
 virtual char original_decimal_separator()=0; // DAT_00449B14
};
struct FormatResult {
 std::string text{};
 I32 decimal_position{};
 bool rounded_carry{};
 bool scientific{};
};
// Original 00437A60 performs right-padded zero copy and carry rounding.
// length is the number of significant digits preceding the next guard digit.
DecimalRecord round_original_digits(DecimalRecord input,I32 length,bool* carry=nullptr);
// 004332F2. precision is the number of digits following the mantissa point.
FormatResult exponent_004332f2(U64 raw,I32 precision,bool uppercase,IO&);
// 004333F6. precision is the number of digits following the fixed point.
FormatResult fixed_004333f6(U64 raw,I32 precision,IO&);
// 004334D4. Uses the recovered exponent switch (-4, significant digits).
FormatResult general_004334d4(U64 raw,I32 precision,bool uppercase,IO&);
} // namespace btb::original_crt_float3
