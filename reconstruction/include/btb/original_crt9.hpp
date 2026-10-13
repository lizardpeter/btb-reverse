#pragma once
#include "btb/runtime_dependency_32.hpp"
namespace btb::original_crt9 {
using namespace btb::runtime_batch_32;
void runtime_processor_warning(I32,I32,const Callback&,const std::function<void()>&);
void runtime_amessage_exit(I32,I32,const std::function<void()>&,const Callback&,const Callback&);
void runtime_fatal_process(I32,I32,const std::function<void()>&,const Callback&,const Callback&);
[[nodiscard]] bool check_processor_feature(const std::function<std::optional<bool>()>&,
 const std::function<bool()>&);
void release_heap_slot(U32&,U8&,U8&,U32&,const Callback&);
void call_setting_frame(I32,const Callback&,const std::function<void()>&,const Callback&);
[[nodiscard]] std::array<U32,2> assign_float_result(bool,
 const std::function<std::array<U32,2>()>&,const std::function<U32()>&);
[[nodiscard]] std::optional<std::vector<std::string>> duplicate_environment(
 const std::vector<std::string>*,const std::function<std::optional<std::string>(const std::string&)>&);
[[nodiscard]] I32 convert_wide_character(char*,U16,bool,I32&,
 const std::function<std::optional<std::string>(U16)>&);
[[nodiscard]] bool verify_9_original_crt_functions();
}
