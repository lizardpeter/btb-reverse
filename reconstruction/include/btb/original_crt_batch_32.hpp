#pragma once
// Recompiled real PE32 MSVC runtime utility implementations, 32 addresses.
// Evidence: analysis/ghidra_main_exe/functions/<ADDRESS>.c.
// These are out-of-line function bodies linked into btb_game; original
// global/IAT pointers are passed in explicitly. No claim of original
// x86 ABI, CRT injection, or retail differential behavioral verification.
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
namespace btb::original_crt32 {
using U8=std::uint8_t; using U16=std::uint16_t; using U32=std::uint32_t;
using U64=std::uint64_t; using I32=std::int32_t;
using Words96=std::array<U32,3>;
// 00433630
char* copy_cstr(char*,const char*) noexcept;
// 004377BA 0043779F 004377C6
void zero_words(Words96&) noexcept;
void copy_words(Words96&,const Words96&) noexcept;
[[nodiscard]] bool are_words_zero(const Words96&) noexcept;
// 004387C7 00438846 00438874 00437674 004376BD 004387E8
[[nodiscard]] U32 add32_with_carry(U32,U32,U32&) noexcept;
void shift_left_one(Words96&) noexcept;
void shift_right_one(Words96&) noexcept;
[[nodiscard]] bool higher_parts_zero(const Words96&,unsigned bit_index) noexcept;
// Returns final carry in EAX, not success. Original callers use overflow.
[[nodiscard]] bool increment_masked(Words96&,unsigned bit_index) noexcept;
void add_words(Words96&,const Words96&) noexcept;
// 00436640 00436680
[[nodiscard]] U64 msvc_allmul(U32 low_a,U32 high_a,U32 low_b,U32 high_b) noexcept;
[[nodiscard]] U64 msvc_allshl(U32 low,U32 high,unsigned shift) noexcept;
// 00436540 0043991A 00439E9E
void* msvc_memset(void*,int,std::size_t) noexcept;
[[nodiscard]] std::size_t bounded_length(const char*,std::size_t) noexcept;
using Alloc=void* (*)(std::size_t,void*);
[[nodiscard]] char* duplicate_cstr(const char*,Alloc,void*) noexcept;
// 00436EB6
[[nodiscard]] U32 map_runtime_code(U32) noexcept;
// 00436745 0043670D 00436729 (actual WinAPI provided by host)
using BadCodeProbe=bool(*)(const void*,void*);
using BadRangeProbe=bool(*)(const void*,std::size_t,void*);
[[nodiscard]] bool executable_pointer_valid(const void*,BadCodeProbe,void*) noexcept;
[[nodiscard]] bool readable_pointer_valid(const void*,std::size_t,BadRangeProbe,void*) noexcept;
[[nodiscard]] bool writable_pointer_valid(void*,std::size_t,BadRangeProbe,void*) noexcept;
// 00438241
[[nodiscard]] bool char_class_matches(
    U8 code,U32 mask,U8 category,std::span<const U8> category_flags,
    std::span<const U16> class_flags) noexcept;
// 00434444. Retail returns an address in the 20-byte table or null.
using RegionRow=std::array<U32,5>;
static_assert(sizeof(RegionRow)==20);
[[nodiscard]] const RegionRow* find_allocation_region(
    U32 target,std::span<const RegionRow>) noexcept;
// 0043091D 00431D8A
using Initializer=void(*)();
void run_initializers(std::span<const Initializer>);
using InputCallback=void(*)(U32,void*);
void forward_if_not_eof(U32,InputCallback,void*);
// 004330B2 004330E7
struct WriteCursor {
    std::span<U8> buffer{};std::size_t offset{};I32 remaining{};I32 total{};
};
using OverflowWrite=U32 (*)(U32,void*);
void write_stream_byte(U32,WriteCursor&,OverflowWrite,void*);
void repeat_stream_byte(U32,I32,WriteCursor&,OverflowWrite,void*);
// 00435FF5
using LocalUnwind=void(*)(U32,U32,void*);
void unwind_frame(const std::array<U32,8>&,LocalUnwind,void*);
// 0043A0DD 0043A1C5
struct LegacyVptrObject{U32 vtable{};};
using ObjectInitializer=void(*)(void*);
LegacyVptrObject* construct_object_a(LegacyVptrObject&,ObjectInitializer,void*);
LegacyVptrObject* construct_object_b(LegacyVptrObject&,ObjectInitializer,void*);
// 004379DA 004379F0 00437EB3 00437478
using TableDecode=void(*)(U16*,U32*,U32,void*);
// Direct linked conversion, using the recovered original format tables.
void decode_first(const U16*,U32*);
void decode_first(U16*,U32*,TableDecode,void*);
void decode_second(const U16*,U32*);
void decode_second(U16*,U32*,TableDecode,void*);
using Decoder=void(*)(void*,U8*,I32*,void*,I32,void*);
void decode_forward_zero(void*,U8*,I32*,void*,Decoder,void*);
using FpuControl=void(*)(U32,U32,void*);
void update_fpu_control(U32,U32,FpuControl,void*);

[[nodiscard]] bool verify_32_original_crt_functions() noexcept;
}
