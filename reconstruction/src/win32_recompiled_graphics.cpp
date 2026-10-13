#include "btb/win32_recompiled_graphics.hpp"

#if defined(_WIN32)
#include <cstdint>
#include <cstring>
#include <limits>

namespace btb::original_win32 {

// PE32 00403500: the object field at +0x0c is a DirectDraw backbuffer.
// This is an actual COM dispatch to IDirectDrawSurface7::BltFast.
HRESULT blt_fast_to_backbuffer(
    IDirectDrawSurface7* backbuffer,DWORD x,DWORD y,
    IDirectDrawSurface7* source,const RECT* source_rect,DWORD flags) noexcept {
    if (!backbuffer) return E_POINTER;
    return backbuffer->BltFast(x,y,source,const_cast<RECT*>(source_rect),flags);
}

// PE32 00403540: original descriptor is 100 bytes and its color fill
// lives in the DDBLTFX dwFillColor union. Do not substitute a CPU clear.
HRESULT clear_backbuffer(IDirectDrawSurface7* backbuffer,DWORD color) noexcept {
    if (!backbuffer) return E_POINTER;
    DDBLTFX effect{};
    effect.dwSize=sizeof(effect);
    effect.dwFillColor=color;
    return backbuffer->Blt(nullptr,nullptr,nullptr,DDBLT_COLORFILL,&effect);
}

// PE32 00403590: source +0x28 selects windowed vs fullscreen destination.
bool update_display_destination_rect(
    HWND window,bool windowed,RECT& destination) noexcept {
    if (windowed) {
        if (!window || !GetClientRect(window,&destination)) return false;
        POINT upper_left{destination.left,destination.top};
        POINT lower_right{destination.right,destination.bottom};
        if (!ClientToScreen(window,&upper_left) ||
            !ClientToScreen(window,&lower_right)) return false;
        destination={upper_left.x,upper_left.y,lower_right.x,lower_right.y};
        return true;
    }
    const int width=GetSystemMetrics(SM_CXSCREEN);
    const int height=GetSystemMetrics(SM_CYSCREEN);
    if (width<=0 || height<=0) return false;
    return SetRect(&destination,0,0,width,height)!=0;
}

// PE32 00403A70: temporarily paint pixel (0,0), read its native surface
// representation through Lock, then restore the original pixel. Ghidra
// indicates precisely this GDI -> Lock -> Unlock -> GDI sequence.
// Error sentinel 0xffffffff is also present in the recovered function.
// Unlike raw bitmask OR, this handles the actual device conversion.
DWORD colorref_to_surface_pixel(
    IDirectDrawSurface7* surface,COLORREF color) noexcept {
    constexpr DWORD invalid=std::numeric_limits<DWORD>::max();
    if (!surface) return invalid;
    HDC dc=nullptr;
    COLORREF original=CLR_INVALID;
    bool painted=false;
    if (color!=CLR_INVALID && SUCCEEDED(surface->GetDC(&dc))) {
        original=GetPixel(dc,0,0);
        if (original!=CLR_INVALID &&
            SetPixel(dc,0,0,color)!=CLR_INVALID) painted=true;
        const HRESULT released=surface->ReleaseDC(dc);
        if (FAILED(released)) return invalid;
    }
    if (color!=CLR_INVALID && !painted) return invalid;

    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    const HRESULT locked=surface->Lock(
        nullptr,&desc,DDLOCK_WAIT,nullptr);
    DWORD native=invalid;
    if (SUCCEEDED(locked)) {
        const auto bpp=desc.ddpfPixelFormat.dwRGBBitCount;
        if (desc.lpSurface && (bpp==8 || bpp==16 || bpp==24 || bpp==32)) {
            std::uint8_t raw[4]{};
            const auto bytes=bpp/8;
            std::memcpy(raw,desc.lpSurface,bytes);
            native=static_cast<DWORD>(raw[0]) |
                (static_cast<DWORD>(raw[1])<<8) |
                (static_cast<DWORD>(raw[2])<<16) |
                (static_cast<DWORD>(raw[3])<<24);
        }
        if (FAILED(surface->Unlock(nullptr))) native=invalid;
    }

    if (painted) {
        HDC restore_dc=nullptr;
        if (FAILED(surface->GetDC(&restore_dc))) return invalid;
        const bool restored=SetPixel(restore_dc,0,0,original)!=CLR_INVALID;
        const auto released=surface->ReleaseDC(restore_dc);
        if (!restored || FAILED(released)) return invalid;
    }
    return native;
}

// PE32 00403B70: call 00403A70, then the real SetColorKey(8,...).
// The retail global registry's keyed flag is maintained separately;
// this function does not claim to reproduce that global write yet.
HRESULT set_surface_transparency_color_key(
    IDirectDrawSurface7* surface,COLORREF color) noexcept {
    if (!surface) return DDERR_INVALIDPARAMS;
    const DWORD native=colorref_to_surface_pixel(surface,color);
    if (native==std::numeric_limits<DWORD>::max()) return E_FAIL;
    DDCOLORKEY key{native,native};
    return surface->SetColorKey(DDCKEY_SRCBLT,&key);
}

} // namespace btb::original_win32
#endif
