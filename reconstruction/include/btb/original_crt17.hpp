#pragma once
// 17 independently compiled MSVC2003 original PE32 routines.
// Retail evidence: analysis/ghidra_main_exe/functions/<ADDRESS>.c
// Not original binary x86 ABI replacements; external CRT/IAT
// operations are injected rather than guessed.
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>
namespace btb::original_crt17 {
using U32=std::uint32_t;using I32=std::int32_t;using Ptr=void*;
using Free=void(*)(Ptr) noexcept;
using Allocate=Ptr(*)(std::size_t,I32) noexcept;
using RawMalloc=Ptr(*)(std::size_t) noexcept;
using NewHandler=bool(*)(std::size_t) noexcept;
using OpenNative=Ptr(*)(const char*,const char*,U32,Ptr) noexcept;
using ThreadState=Ptr(*)() noexcept;
using ScanNative=I32(*)(Ptr,const char*,Ptr) noexcept;
using IntegerFormatter=void(*)(U32,char*,U32,bool) noexcept;
using Destroy=void(*)(Ptr) noexcept;
using PrintDialog=bool(*)(Ptr) noexcept;
// 0042FFBA / 0042FFC4
void retail_srand(U32& seed,U32 value) noexcept;
[[nodiscard]] U32 retail_rand(U32& seed) noexcept;
// 0042FBDC / 0042FBCE / 00430D93 / 00430DA5
void delete_proxy(Ptr,Free) noexcept;
[[nodiscard]] Ptr operator_new(U32,Allocate) noexcept;
[[nodiscard]] Ptr crt_malloc(std::size_t,I32,Allocate) noexcept;
[[nodiscard]] Ptr nh_malloc(std::size_t,I32,RawMalloc,NewHandler) noexcept;
// 00430A1F / 00430A3F / 0042FBB8
[[nodiscard]] Ptr fopen_internal(
 const char*,const char*,U32,ThreadState,OpenNative) noexcept;
[[nodiscard]] Ptr fopen_native(
 const char*,const char*,ThreadState,OpenNative) noexcept;
[[nodiscard]] I32 fscanf_native(Ptr,const char*,Ptr,ScanNative) noexcept;
// 0042FF99 / 0042FFA2
struct NlgGlobals{U32 target{},eax{},ebp{};};
void nlg_notify(NlgGlobals&,U32,U32,U32) noexcept;
void nlg_from_stack(NlgGlobals&,U32,U32,U32) noexcept;
// 00430441 / 00430464
struct Exception{U32 vtable{};Ptr message{};U32 owned{};};
struct TypeInfo{U32 vtable{};Ptr decorated_name{};};
void exception_destruct(Exception&,U32,Free) noexcept;
void type_info_destruct(TypeInfo&,U32,Free) noexcept;
// 00430CFE / 00430F2F / 00406600 / 0042FB5C
void swap_forward_bytes(std::span<std::byte>,std::span<std::byte>,std::size_t) noexcept;
[[nodiscard]] char* retail_itoa(U32,char*,U32,IntegerFormatter) noexcept;
[[nodiscard]] Ptr deleting_exception(Ptr,U32,Destroy,Free) noexcept;
[[nodiscard]] bool print_dialog_thunk(Ptr,PrintDialog) noexcept;
[[nodiscard]] bool verify_17_original_crt_functions() noexcept;
}
