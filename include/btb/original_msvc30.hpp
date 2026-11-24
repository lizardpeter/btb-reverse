#pragma once
// Bob Builds a Park PE32: 30 MSVC2003 CRT functions recovered from
// analysis/ghidra_main_exe/functions/<ADDRESS>.c.
// Out-of-line C++ in native game core. Host callbacks stand in for
// original process-global/IAT and SEH dependencies; no retail ABI or
// differential behavior parity is claimed.
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
namespace btb::original_msvc30 {
using U8=std::uint8_t;using U16=std::uint16_t;
using U32=std::uint32_t;using I32=std::int32_t;
using Exit=void(*)(U32,U32,U32,void*);
using Notice=void(*)(U32,void*);
using Classify=U32(*)(U32,U32,U32,void*);
using Control=void(*)(void*,U32,U32,void*);
using Initializer=void(*)(I32,void*);
using Refill=U32(*)(void*);
using Predicate=I32(*)(U32,void*);
using Destructor=void(*)(void*,void*);
using Prepare=void(*)(void*,void*);
using Throw=void(*)(void*,U32,void*);
using ClassifyByte=U32(*)(U32,void*);
// 00430862 00430873
void exit_immediately(U32,Exit,void*);
void exit_with_cleanup(U32,Exit,void*);
// 00438230 0043317B
[[nodiscard]] U32 class_category_four(U8,Classify,void*);
void set_precision_control(void*,Control,void*);
// 00436774 00437097 0043675D
[[nodiscard]] bool initialize_zero(U32&,Initializer,void*);
[[nodiscard]] bool initialize_minus_three(U32&,Initializer,void*);
void fatal_runtime_error(Notice,Notice,Exit,void*);
// 00431D70 00435610
struct InputCursor{const U8* current{};I32 remaining{};void* ctx{};};
[[nodiscard]] U32 read_input_byte(InputCursor&,Refill);
[[nodiscard]] U32 predicate_normalized(U32,Predicate,void*);
// 0043039E 00430479 00439FEE
void* destroy_exception(void*,U8,Destructor,Destructor,void*);
void* destroy_type_info(void*,U8,Destructor,Destructor,void*);
void* destroy_format_exception(void*,U8,Destructor,Destructor,void*);
// 00439F94 0043A0A4 0043A18C
void throw_format(std::array<U32,7>&,Prepare,Throw,void*);
void throw_class_a(std::array<U32,7>&,Prepare,Throw,void*);
void throw_class_b(std::array<U32,7>&,Prepare,Throw,void*);
// 00431DA1 0043360A
[[nodiscard]] U32 scan_until_class_fails(U32&,InputCursor&,Refill,ClassifyByte,void*);
void shift_c_string(char*,I32);
// 004373B5 0043356F
struct HandleRow{U32 value{};U8 flags{};std::array<U8,3> reserved{};};
static_assert(sizeof(HandleRow)==8);
[[nodiscard]] U8 handle_bit40(std::span<const HandleRow>,U32,U32);
using Conversion=void(*)(U32,void*,I32,I32,void*);
void with_conversion_flag(U32&,U32,void*,I32,I32,Conversion,void*);
// 00436598
using MaskClassify=U32(*)(U32,U32,void*);
[[nodiscard]] U32 class_eight(U32,I32,std::span<const U8>,MaskClassify,void*);
// 00437A06 00437A33
using FormatSetup=void(*)(void*,std::array<U16,6>&,U8*,void*);
using FormatFinish=void(*)(std::array<U16,6>&,U32*,U32,void*);
void format_first(void*,U32*,U8*,FormatSetup,FormatFinish,void*);
void format_second(void*,U32*,U8*,FormatSetup,FormatFinish,void*);
// 00437443 00431D39
using ReadControl=U32(*)(void*);
using WriteControl=void(*)(U32,void*);
[[nodiscard]] U32 merge_control_word(U32,U32,ReadControl,WriteControl,void*);
[[nodiscard]] U32 hex_fold(U32,I32,std::span<const U8>,MaskClassify,void*);
// 004301AE
using RuntimeFunction=void(*)();
struct ConversionFunctions{std::array<RuntimeFunction,6> functions{};};
void install_conversion_functions(ConversionFunctions&,const std::array<RuntimeFunction,6>&);
// 00433118 00435732 00436322
using WriteChar=void(*)(U32,I32&,void*);
void write_string_bytes(const char*,I32,I32&,WriteChar,void*);
using Flush=void(*)(void*,void*);
struct StreamState{U32 cursor{},count{},remaining{},flags{},buffer_size{};};
void reset_buffered_stream(I32,StreamState&,void*,Flush,void*);
[[nodiscard]] U32 native_handle_or_error(std::span<const HandleRow>,U32,U32,U32&,U32&);
// 0043318D 004358B0
[[nodiscard]] std::optional<bool> compare_modulo(I32,I32,I32) noexcept;
struct Triple{I32 key{},second{},third{};};
[[nodiscard]] const Triple* find_triple(std::span<const Triple>,I32) noexcept;

[[nodiscard]] bool verify_30_original_msvc_functions();
} // namespace btb::original_msvc30
