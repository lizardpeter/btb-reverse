#pragma once
// Bob Builds a Park, original PE32 SHA-256:
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Ghidra-addressed shared device/lifetime control flow. This is a source-level
// reconstruction with host-provided native operations, NOT an x86 ABI clone.

#include "btb/display_manager.hpp"
#include "btb/dsutil.hpp"
#include "btb/bitmap_registry.hpp"
#include "btb/printing_system.hpp"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>

namespace btb::retail_system {

using U32 = std::uint32_t;
using I32 = std::int32_t;
[[nodiscard]] constexpr U32 retail_hresult(U32 result) noexcept {
    return std::bit_cast<I32>(result)<0 ? result : 0U;
}
using DeviceResult=U32 (*)(void*) noexcept;
using NativeFree=void (*)(void*) noexcept;
using NativeRelease=void (*)(void*) noexcept;
using UnregisterSlot=void (*)(std::size_t) noexcept;

// 00401F80 and 00402600: WinMain wrappers return E_FAIL or zero,
// not the underlying positive success codes.
using WindowInit=U32 (*)(void*,I32) noexcept;
[[nodiscard]] inline U32 initialize_display_for_game_window(
    void* hwnd,I32 mode,WindowInit reset_display) noexcept {
    return reset_display ? retail_hresult(reset_display(hwnd,mode))
                         : 0x80004003U; // invalid provider, not retail null behavior
}
[[nodiscard]] inline U32 present_game_frame(void* display,DeviceResult present) noexcept {
    return present ? retail_hresult(present(display)) : 0x80004003U;
}

// 00402FF0: unregister and COM-Release 3 DirectDraw surfaces in index
// order auxiliary(+0x10), render(+0x0c), primary(+0x08), then restores
// normal cooperative level and releases DirectDraw. 00402FE0 additionally
// restores original vtable pointer first. The original leaves the DD
// pointer nonzero after releasing; do NOT clear it here.
using CooperativeNormal=void (*)(void*,std::uintptr_t,U32) noexcept;
struct DisplayResources {
    btb::display::RetailDisplayManager32 retail{};
    std::array<void*,3> surfaces{}; // auxiliary, render, primary
    void* directdraw{};
};
inline void release_display_resources(
    DisplayResources& d,UnregisterSlot unregister,
    NativeRelease release,CooperativeNormal coop) noexcept {
    for(std::size_t i=0;i<d.surfaces.size();++i) {
        if(unregister) unregister(i);
        if(d.surfaces[i]&&release) release(d.surfaces[i]);
        if(d.surfaces[i]) d.surfaces[i]=nullptr;
    }
    d.retail.unused_legacy_surface_ptr32=0;
    d.retail.render_surface_ptr32=0;
    d.retail.primary_surface_ptr32=0;
    if(d.directdraw) {
        if(coop)coop(d.directdraw,d.retail.hwnd32,8);
        if(release)release(d.directdraw);
    }
}
inline void display_manager_destructor(
    DisplayResources& d,UnregisterSlot unregister,
    NativeRelease release,CooperativeNormal coop) noexcept {
    d.retail.vtable_ptr32=btb::display::kDisplayManagerVtable32;
    release_display_resources(d,unregister,release,coop);
}
// 004020A0: destroy current manager then delete allocated object and null
// the global; idempotent. Treat two native callbacks separately.
using DestroyDisplay=void (*)(void*) noexcept;
inline void destroy_display_manager(
    void*& global,DestroyDisplay destroy,NativeFree free_object) noexcept {
    if(!global)return;
    void* old=global;
    if(destroy)destroy(old);
    if(free_object)free_object(old);
    global=nullptr;
}
// 00404E10: vtable reset followed by delete[] of owned DIB allocation;
// shipped code does not clear the dangling pointer field.
inline void bitmap_printer_destructor(
    btb::printing::RetailBitmapPrinter32& printer,
    void* allocation,NativeFree free_dib) noexcept {
    printer.vtable_ptr32=btb::printing::kBitmapPrinterVtable32;
    if(free_dib)free_dib(allocation);
}
// 00404530: always CWaveFile_Close first; if not memory-backed then free
// format pointer and set original m_pwfx to zero.
using WaveClose=void (*)(void*) noexcept;
inline void wave_file_destructor(
    btb::dsutil::RetailCWaveFile32& wave,void* wave_object,
    void* format_allocation,WaveClose close,NativeFree free_format) noexcept {
    if(close)close(wave_object);
    if(wave.m_bIsReadingFromMemory==0&&wave.m_pwfx!=0) {
        if(free_format)free_format(format_allocation);
        wave.m_pwfx=0;
    }
}

// 00409390: CSoundManager destructor, free manager allocation, clear global,
// and return 1 regardless of whether a sound manager existed.
using SoundManagerDestroy=void (*)(void*) noexcept;
[[nodiscard]] inline I32 shutdown_directsound(
    void*& sound_manager,SoundManagerDestroy destroy,NativeFree free_manager) noexcept {
    if(sound_manager) {
        void* old=sound_manager;
        if(destroy)destroy(old);
        if(free_manager)free_manager(old);
        sound_manager=nullptr;
    }
    return 1;
}

// 00403B70: MarkRegisteredSurfaceColorKeyed first, compute pixel color key,
// then IDirectDrawSurface7::SetColorKey(DDCKEY_SRCBLT=8, low==high).
using MarkRegistered=void (*)(void*) noexcept;
using PixelColorKey=U32 (*)(void*,U32) noexcept;
using SetColorKey=U32 (*)(void*,U32,U32,U32) noexcept;
inline void set_surface_transparency_color_key(
    void* surface,void* surface_slot,U32 color,
    MarkRegistered mark,PixelColorKey to_pixel,SetColorKey set_key) noexcept {
    if(mark)mark(surface_slot);
    const auto pixel=to_pixel ? to_pixel(surface,color) : color;
    if(set_key)set_key(surface,8U,pixel,pixel);
}

// 00403480: runtime provider must perform actual Flip/Blt. Retail repeatedly
// retries DDERR_WASSTILLDRAWING without arbitrary attempt cap. To avoid a
// tight loop on a permanently broken provider this model's caller supplies
// retry permission; disabling it does not falsely claim successful present.
using PresentAttempt=U32 (*)(void*,bool) noexcept;
using RestoreSurface=void (*)(void*) noexcept;
struct PresentState {
    void* primary{};
    void* render{};
    void* directdraw{};
    bool windowed{};
    bool bitmap_reload{};
};
[[nodiscard]] inline U32 present_display(
    PresentState& s,PresentAttempt present,
    RestoreSurface restore_surface,DeviceResult restore_all,
    std::size_t retry_limit=0) noexcept {
    if(!s.primary&&!s.render)return 0x80004003U;
    if(!s.primary||!present)return 0x80004003U; // host safety
    std::size_t retries=0;
    for(;;) {
        const U32 hr=present(s.primary,s.windowed);
        if(hr==static_cast<U32>(btb::display::kDdErrWasStillDrawing)) {
            if(retry_limit!=0&&++retries>=retry_limit)return hr;
            continue;
        }
        if(hr==static_cast<U32>(btb::display::kDdErrSurfaceLost)) {
            if(restore_surface) {
                restore_surface(s.primary);
                restore_surface(s.render);
            }
            if(restore_all) (void)restore_all(s.directdraw);
            s.bitmap_reload=true;
        }
        return hr;
    }
}

// 00403BE0 source policy: release old DirectSound, DirectSoundCreate8,
// SetCooperativeLevel, SetPrimaryBufferFormat. Return HRESULT or zero;
// store new object only after create succeeds.
using CreateDS=U32 (*)(void*,void**) noexcept;
using SetCoop=U32 (*)(void*,std::uintptr_t,U32) noexcept;
using SetPrimaryFormat=void (*)(void*) noexcept;
[[nodiscard]] inline U32 initialize_sound_manager(
    void*& ds,std::uintptr_t hwnd,NativeRelease release,
    CreateDS create,SetCoop coop,SetPrimaryFormat format) noexcept {
    if(ds) {if(release)release(ds);ds=nullptr;}
    if(!create)return 0x80004003U;
    U32 hr=create(nullptr,&ds);
    if(std::bit_cast<I32>(hr)<0)return hr;
    if(!ds||!coop)return 0x80004003U; // host safety
    hr=coop(ds,hwnd,2U);
    if(std::bit_cast<I32>(hr)<0)return hr;
    if(format)format(ds);
    return 0;
}

// 00404070: CSound_Destructor called even when delete flag bit is clear;
// only flag bit 0 triggers operator delete. Returns original this pointer.
using SoundDestroy=void (*)(void*) noexcept;
inline void* sound_scalar_deleting_destructor(
    void* sound,U32 flags,SoundDestroy destroy,NativeFree free_object) noexcept {
    if(destroy)destroy(sound);
    if((flags&1U)!=0 && free_object)free_object(sound);
    return sound;
}

// 00404090: CSound virtual dtor releases every non-null buffer, deletes
// buffer pointer array, and destroys/frees CWaveFile; vptr reset first.
struct SoundObject {
    btb::dsutil::RetailCSound32 retail{};
    std::span<void*> buffers{};
    void* buffer_allocation{};
    void* wave{};
};
inline void sound_destructor(
    SoundObject& object,NativeRelease release,NativeFree free_object,
    SoundDestroy destroy_wave) noexcept {
    object.retail.vtable_ptr32=0x0043B2E4U;
    for(auto& buffer:object.buffers) {
        if(buffer) {if(release)release(buffer);buffer=nullptr;}
    }
    if(object.retail.m_apDSBuffer!=0) {
        if(free_object)free_object(object.buffer_allocation);
        object.retail.m_apDSBuffer=0;
    }
    if(object.retail.m_pWaveFile!=0) {
        if(destroy_wave)destroy_wave(object.wave);
        if(free_object)free_object(object.wave);
        object.retail.m_pWaveFile=0;
    }
}

// 004042F0: scan for first non-playing buffer; if all are busy, original
// PRNG picks rand()%count. Actual GetStatus HRESULT is ignored by native.
using QueryPlaying=bool (*)(void*) noexcept;
[[nodiscard]] inline void* sound_get_free_buffer(
    std::span<void* const> buffers,U32 random_value,QueryPlaying playing) noexcept {
    if(buffers.empty())return nullptr;
    for(auto* buffer:buffers) {
        if(buffer && (!playing || !playing(buffer)))return buffer;
    }
    return buffers[static_cast<std::size_t>(random_value%buffers.size())];
}

// 00406780 is a six-byte leaf, returning exactly -3.
[[nodiscard]] constexpr I32 string_error_code_minus_three() noexcept {return -3;}

// 00406440 MSVC string modifies length and terminates buffer at that
// position. Input storage supplied and capacity validated for host safety.
struct MsvcString {
    std::span<char> chars{};
    std::int32_t length{};
    std::int32_t capacity{};
    unsigned char refcount{};
};
[[nodiscard]] constexpr bool msvc_string_set_end(
    MsvcString& s,std::int32_t end) noexcept {
    if(end<0 || end>=static_cast<std::int32_t>(s.chars.size()))return false;
    s.length=end;
    s.chars[static_cast<std::size_t>(end)]='\0';
    return true;
}
// 00406110 and 00406350: shared MSVC string rep uses refcount byte just
// before data. A count of 0 or FF destroys allocation; all others decrement.
// Both routines clear pointer,length,capacity regardless; tidy flag 0 skips
// release. We expose destructor decision for native allocator.
struct StringRelease {
    bool free_allocation{};
    bool decrement_reference{};
};
[[nodiscard]] constexpr StringRelease msvc_string_tidy(
    MsvcString& str,bool release_data) noexcept {
    StringRelease result{};
    if(release_data && str.chars.data()) {
        if(str.refcount==0||str.refcount==255)result.free_allocation=true;
        else {result.decrement_reference=true;--str.refcount;}
    }
    str.chars={};
    str.length=0;
    str.capacity=0;
    return result;
}
[[nodiscard]] constexpr StringRelease msvc_string_release(
    MsvcString& str) noexcept {
    return msvc_string_tidy(str,true);
}

} // namespace btb::retail_system
