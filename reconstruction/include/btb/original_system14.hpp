#pragma once
// 14 original PE32 display, DirectSound, printing and MSVC lifetime
// implementations, emitted as independently linked C++ bodies rather
// than header-only source models. Genuine native allocations, COM objects
// and x86 ABI still require original executable verification.
#include "btb/retail_system_closure.hpp"
namespace btb::original_system14 {
using namespace btb::retail_system;
void release_display_resources(DisplayResources&,UnregisterSlot,
    NativeRelease,CooperativeNormal) noexcept; // 00402FF0
void display_manager_destructor(DisplayResources&,UnregisterSlot,
    NativeRelease,CooperativeNormal) noexcept; // 00402FE0
void destroy_display_manager(void*&,DestroyDisplay,NativeFree) noexcept; // 004020A0
void bitmap_printer_destructor(btb::printing::RetailBitmapPrinter32&,
    void*,NativeFree) noexcept; // 00404E10
void wave_file_destructor(btb::dsutil::RetailCWaveFile32&,void*,
    void*,WaveClose,NativeFree) noexcept; // 00404530
[[nodiscard]] I32 shutdown_directsound(void*&,
    SoundManagerDestroy,NativeFree) noexcept; // 00409390
[[nodiscard]] U32 initialize_sound_manager(void*&,std::uintptr_t,
    NativeRelease,CreateDS,SetCoop,SetPrimaryFormat) noexcept; // 00403BE0
void* sound_scalar_deleting_destructor(void*,U32,SoundDestroy,NativeFree) noexcept; // 00404070
void sound_destructor(SoundObject&,NativeRelease,NativeFree,SoundDestroy) noexcept; // 00404090
[[nodiscard]] void* sound_get_free_buffer(
    std::span<void* const>,U32,QueryPlaying) noexcept; // 004042F0
[[nodiscard]] I32 string_error_code_minus_three() noexcept; // 00406780
[[nodiscard]] bool msvc_string_set_end(MsvcString&,I32) noexcept; // 00406440
[[nodiscard]] StringRelease msvc_string_release(MsvcString&) noexcept; // 00406110
[[nodiscard]] StringRelease msvc_string_tidy(MsvcString&,bool) noexcept; // 00406350
[[nodiscard]] bool verify_14_original_system_functions() noexcept;
}
