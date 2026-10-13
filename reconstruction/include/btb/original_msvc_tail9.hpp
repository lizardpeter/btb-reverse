#pragma once
// Nine remaining MSVC2003/Win32 CRT functions reconstructed from exact
// original Ghidra records as native out-of-line C++ implementations.
#include "btb/runtime_dependency_32.hpp"
namespace btb::original_msvc_tail9 {
using namespace btb::runtime_batch_32;
void runtime_processor_warning(I32,I32,const Callback&,const std::function<void()>&); // 00436010
void runtime_amessage_exit(I32,I32,const std::function<void()>&,const Callback&,const Callback&); // 004310E0
void runtime_fatal_process(I32,I32,const std::function<void()>&,const Callback&,const Callback&); // 00431105
[[nodiscard]] bool check_processor_feature(const std::function<std::optional<bool>()>&,const std::function<bool()>&); // 004331CB
void release_heap_slot(U32&,U8&,U8&,U32&,const Callback&); // 004351F6
void call_setting_frame(I32,const Callback&,const std::function<void()>&,const Callback&); // 004325C0
[[nodiscard]] std::array<U32,2> assign_float_result(bool,const std::function<std::array<U32,2>()>&,const std::function<U32()>&); // 004332B4
[[nodiscard]] std::optional<std::vector<std::string>> duplicate_environment(const std::vector<std::string>*,const std::function<std::optional<std::string>(const std::string&)>&); // 00439B24
[[nodiscard]] I32 convert_wide_character(char*,U16,bool,I32&,const std::function<std::optional<std::string>(U16)>&); // 004373DB
[[nodiscard]] bool verify_9_original_msvc_tail() noexcept;
}
