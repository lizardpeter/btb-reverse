#pragma once
// Original CRT 00432971 formatted writer and 00431314 formatted scanner.
// Source-level C++26 replacement models for original % directive parsing,
// not ABI compatible with raw MSVC x86 varargs or original file streams yet.
// Native argument, byte/Unicode and float adapters are REQUIRED boundaries.
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
namespace btb::original_stdio2 {
using I32=std::int32_t;
using U64=std::uint64_t;
using Value=std::variant<std::int64_t,U64,std::string,long double>;
struct PrintfArg {Value value{};};
struct PrintfResult {std::string text{};I32 written{};I32 consumed_args{};};
class PrintfIO {
public:
 virtual ~PrintfIO()=default;
 virtual std::string original_float_to_string(long double value,char conversion,
                                               I32 precision,bool alternative)=0;
 virtual std::string original_wide_string(U64 native_pointer,I32 precision)=0;
 virtual void store_output_count(U64 native_pointer,I32 count)=0;
};
PrintfResult printf_00432971(std::string_view original_format,
                             const std::vector<PrintfArg>&,PrintfIO&);
struct ScanValue {
 enum class Kind {Signed,Unsigned,Float,Bytes,Count};
 Kind kind{Kind::Bytes};
 std::int64_t signed_value{};U64 unsigned_value{};
 std::string bytes{};long double floating_value{};
};
struct ScanResult {
 I32 assignments{},consumed{},completed_directives{},attempted_directives{};
 std::vector<ScanValue> values{};
 bool input_failure{},format_failure{};
};
class ScanfIO {
public:
 virtual ~ScanfIO()=default;
 virtual long double original_parse_float(std::string_view token,
                                          bool& conversion_success)=0;
 virtual void commit_original_assignment(std::size_t argument_index,
                                          const ScanValue&)=0;
};
// 00431314: whitespace, suppression, width, integer base, scanset, char,
// string and floating directives, %n, assignments/EOF semantics.
// The retail FILE* stream/ungetc and multibyte/wide locale adapters are
// not yet bit-exact and remain external native provider boundaries.
ScanResult scanf_00431314(std::string_view input,std::string_view format,ScanfIO&);
} // namespace btb::original_stdio2
