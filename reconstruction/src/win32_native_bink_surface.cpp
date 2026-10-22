#include "btb/win32_native_bink_surface.hpp"
#if defined(_WIN32)
#include "btb/bink32_surface_convert.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
namespace btb::full_game {
bool copy_native_bink_frame_to_directdraw(
    IDirectDrawSurface7* surface,
    const btb::bink32_recovered::BinkNativeYuVaFrame& frame,
    std::string& error) {
    if(!surface) {error="native Bink DirectDraw target is null";return false;}
    DDSURFACEDESC2 info{};
    info.dwSize=sizeof(info);
    const HRESULT hr=surface->Lock(nullptr,&info,
        DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(hr)) {
        error="cannot lock native Bink DirectDraw target (HRESULT "+
              std::to_string(static_cast<std::uint32_t>(hr))+")";
        return false;
    }
    btb::bink32_recovered::BinkSurfaceResult result=
        btb::bink32_recovered::BinkSurfaceResult::invalid_surface;
    // DirectDraw lPitch is signed. Never reinterpret negative pitch as a
    // huge unsigned buffer, or write beyond an independently sized surface.
    if(info.lpSurface && info.lPitch>0 &&
       info.dwWidth>=frame.width && info.dwHeight>=frame.height &&
       (info.ddpfPixelFormat.dwRGBBitCount==16 ||
        info.ddpfPixelFormat.dwRGBBitCount==32) &&
       (info.ddpfPixelFormat.dwFlags&DDPF_RGB)) {
        const auto row_pitch=static_cast<std::size_t>(info.lPitch);
        const auto bpp=static_cast<std::uint8_t>(
            info.ddpfPixelFormat.dwRGBBitCount/8);
        if(frame.height>0 && row_pitch<=std::numeric_limits<std::size_t>::max()/frame.height) {
            // Fireworks allocates the original per-index surface envelope,
            // which can exceed the compressed frame's visible dimensions.
            // Only the visible top-left image is decoded; do not write
            // outside it or reinterpret the larger surface as source pixels.
            const auto total=row_pitch*frame.height;
            btb::bink32_recovered::BinkPackedSurface target{
                {static_cast<std::uint8_t*>(info.lpSurface),total},
                row_pitch,frame.width,frame.height,bpp,
                info.ddpfPixelFormat.dwRBitMask,
                info.ddpfPixelFormat.dwGBitMask,
                info.ddpfPixelFormat.dwBBitMask};
            result=btb::bink32_recovered::blit_bink_yuva_to_surface(
                frame,target);
        }
    }
    const HRESULT unlock=surface->Unlock(nullptr);
    if(FAILED(unlock)) {
        error="cannot unlock native Bink DirectDraw target";
        return false;
    }
    if(result!=btb::bink32_recovered::BinkSurfaceResult::ok) {
        error="unsupported or mismatched native Bink DirectDraw pixel surface";
        return false;
    }
    error.clear();
    return true;
}
}
#endif
