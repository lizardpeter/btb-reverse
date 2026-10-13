#pragma once
// Eight original PE32 MSVC2003 runtime procedures. Each has a Ghidra source
// at analysis/ghidra_main_exe/functions/<original address>.c.
// Uses explicit process-global state and host callbacks where x86 IAT and
// locale tables are not reconstructable; not original ABI/retail-verified.
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
namespace btb::original_runtime8 {
using U8=std::uint8_t;using U32=std::uint32_t;using I32=std::int32_t;
using Codepage=I32(*)(void*);
using Compare=I32(*)(std::string_view,std::string_view,std::size_t,void*);
using Comparator=I32(*)(const std::byte*,const std::byte*,void*);
// 00436EE9: locale table plus six native globals.
struct Locale{
 std::array<U8,257> table{};
 U32 codepage{},is_multibyte{},locale_id{},flags{},state_a{},state_b{};
};
void reset_locale(Locale&) noexcept;
// 00436E6C: OEM/ANSI/current codepage selection.
[[nodiscard]] I32 choose_codepage(I32 request,I32 current,U32& use_system,
                                  Codepage oem,Codepage ansi,void*);
// 0043277A: errno mapping from native Win32 error-number table.
struct ErrorPair{I32 win{},err{};};
[[nodiscard]] I32 map_win_error(I32,std::span<const ErrorPair>) noexcept;
// 00439E2B: source lead-byte-aware char/pair lookup.
[[nodiscard]] const U8* multibyte_chr(
    const U8*,U32,bool,std::span<const U8>) noexcept;
// 004392DE: source Win32 NLS CompareString return -2 normalization.
[[nodiscard]] I32 mbsnbicoll(
    std::string_view,std::string_view,std::size_t,Compare,void*);
// 00439ACC: direct internal call to 004392DE above.
[[nodiscard]] I32 environment_find(
    std::span<const std::string_view>,std::string_view,Compare,void*);
// 004358F3: retail Win32 argv byte tokenizer; DBCS lead-probe callback.
using LeadByte=bool(*)(U8,void*);
[[nodiscard]] std::size_t argv_token_end(
    std::string_view,std::size_t,LeadByte,void*) noexcept;
// 00430CB0 selection sort calls 00430CFE swap_source_bytes directly.
void selection_sort_bytes(
    std::span<std::byte>,std::size_t,Comparator,void*);
[[nodiscard]] bool verify_8_original_algorithms() noexcept;
}
