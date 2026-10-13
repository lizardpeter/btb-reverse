#pragma once
// Five independently compiled original PE32 / MSVC runtime functions.
// Ghidra analysis/ghidra_main_exe/functions/<address>.c.
// Exact register-level assembly optimization/retail parity not established.
#include <cstddef>
#include <cstdint>
namespace btb::original_runtime5 {
using U32=std::uint32_t;using I32=std::int32_t;
[[nodiscard]] std::size_t retail_strlen(const char*) noexcept; // 00433720
[[nodiscard]] I32 retail_strcmp(const char*,const char*) noexcept; // 004337A0
[[nodiscard]] const char* retail_strstr(const char*,const char*) noexcept; // 004381B0
void retail_write_radix(U32,char*,U32,bool) noexcept; // 00430F5C
[[nodiscard]] U32 translate_x87_control(U32) noexcept; // 0043748E
[[nodiscard]] bool verify_5_original_runtime_functions() noexcept;
}
