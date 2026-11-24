#include "btb/original_recompiled_bitmaps.hpp"
#if defined(_WIN32)
#include "btb/full_game_blt_clip.hpp"
#include "btb/bitmap_registry.hpp"
#include "btb/rgb32_color_key.hpp"

#include <utility>

namespace btb::original_win32 {

namespace {
// Compatibility binding for 32-bit desktop surfaces. GDI uploads and
// SetPixel can differ in their unused high byte. Compare actual RGB
// on the real locked DirectDraw surfaces, not an arbitrary alpha byte.
HRESULT blit_rgb32_keyed(IDirectDrawSurface7* dst,IDirectDrawSurface7* src,
    RECT& d,RECT& s) noexcept {
    if(dst==src)return DDERR_INVALIDPARAMS;
    DDCOLORKEY key{};
    HRESULT hr=src->GetColorKey(DDCKEY_SRCBLT,&key);
    if(FAILED(hr))return hr;
    DDSURFACEDESC2 a{},b{};
    a.dwSize=sizeof(a);b.dwSize=sizeof(b);
    hr=src->Lock(&s,&a,DDLOCK_WAIT|DDLOCK_READONLY,nullptr);
    if(FAILED(hr))return hr;
    hr=dst->Lock(&d,&b,DDLOCK_WAIT,nullptr);
    if(FAILED(hr)){src->Unlock(&s);return hr;}
    const DWORD mask=a.ddpfPixelFormat.dwRBitMask|
        a.ddpfPixelFormat.dwGBitMask|a.ddpfPixelFormat.dwBBitMask;
    const auto width=static_cast<std::size_t>(s.right-s.left);
    if(!a.lpSurface||!b.lpSurface||!mask||
       a.ddpfPixelFormat.dwRGBBitCount!=32||
       b.ddpfPixelFormat.dwRGBBitCount!=32) {
        hr=DDERR_INVALIDPIXELFORMAT;
    } else {
        auto* from=static_cast<const unsigned char*>(a.lpSurface);
        auto* to=static_cast<unsigned char*>(b.lpSurface);
        for(LONG y=0;y<s.bottom-s.top;++y) {
            btb::graphics::copy_rgb32_keyed_row(
                from+y*static_cast<std::ptrdiff_t>(a.lPitch),
                to+y*static_cast<std::ptrdiff_t>(b.lPitch),
                width,mask,key.dwColorSpaceLowValue);
        }
    }
    const HRESULT du=dst->Unlock(&d),su=src->Unlock(&s);
    return FAILED(hr)?hr:FAILED(du)?du:su;
}
}

HRESULT copy_bitmap_to_surface(
    IDirectDrawSurface7* destination,HBITMAP source) noexcept {

    // Original 004035F0 returns DDERR_INVALIDPARAMS when either source
    // object is absent. Perform real GDI upload (GetDC -> BitBlt -> ReleaseDC).
    if (!destination || !source) return DDERR_INVALIDPARAMS;

    BITMAP bitmap{};
    if (!GetObjectW(source,sizeof(bitmap),&bitmap) ||
        bitmap.bmWidth <= 0 || bitmap.bmHeight <= 0) {
        return DDERR_INVALIDPARAMS;
    }

    HDC output=nullptr;
    const HRESULT lock=destination->GetDC(&output);
    if(FAILED(lock))return lock;

    HDC input=CreateCompatibleDC(nullptr);
    if(!input){
        destination->ReleaseDC(output);
        return E_FAIL;
    }
    HGDIOBJ old=SelectObject(input,source);
    if(!old||old==HGDI_ERROR){
        DeleteDC(input);
        destination->ReleaseDC(output);
        return E_FAIL;
    }
    const BOOL copied=BitBlt(output,0,0,bitmap.bmWidth,bitmap.bmHeight,
                              input,0,0,SRCCOPY);
    SelectObject(input,old);
    DeleteDC(input);
    const HRESULT unlock=destination->ReleaseDC(output);
    if(!copied)return E_FAIL;
    return unlock;
}

HRESULT load_bitmap_to_directdraw_surface(
    IDirectDraw7* directdraw,const std::filesystem::path& source,
    LoadedBitmap& out) noexcept {

    out={};
    if(!directdraw || source.empty())return DDERR_INVALIDPARAMS;
    // Same real LoadImage flags as 004038D0 (0x2010) and real GetObject.
    const auto handle=reinterpret_cast<HBITMAP>(
        LoadImageW(nullptr,source.c_str(),IMAGE_BITMAP,0,0,
                   LR_LOADFROMFILE|LR_CREATEDIBSECTION));
    if(!handle)return HRESULT_FROM_WIN32(GetLastError());

    BITMAP bitmap{};
    if(!GetObjectW(handle,sizeof(bitmap),&bitmap)||
        bitmap.bmWidth<=0||bitmap.bmHeight<=0){
        DeleteObject(handle);
        return DDERR_INVALIDPARAMS;
    }

    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    desc.dwFlags=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
    desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN;
    if(bitmap.bmWidth>bitmap_registry::kLargeBitmapWidthThreshold)
        desc.ddsCaps.dwCaps|=DDSCAPS_SYSTEMMEMORY;
    desc.dwHeight=static_cast<DWORD>(bitmap.bmHeight);
    desc.dwWidth=static_cast<DWORD>(bitmap.bmWidth);
    IDirectDrawSurface7* surface=nullptr;
    HRESULT hr=directdraw->CreateSurface(&desc,&surface,nullptr);
    if(FAILED(hr)){
        desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
        hr=directdraw->CreateSurface(&desc,&surface,nullptr);
    }
    if(FAILED(hr)){
        DeleteObject(handle);
        return hr;
    }

    // Directly link 004038D0's call to the recovered 004035F0. Not an
    // abstract event or separate file-loader model.
    hr=copy_bitmap_to_surface(surface,handle);
    DeleteObject(handle);
    if(FAILED(hr)){
        surface->Release();
        return hr;
    }

    out={surface,bitmap.bmWidth,bitmap.bmHeight};
    return S_OK;
}

HRESULT blit_color_keyed_surface_clipped(
    IDirectDrawSurface7* destination,IDirectDrawSurface7* source,
    int screen_x,int screen_y,const RECT* source_rectangle,
    int camera_x,int camera_y) noexcept {

    if(!destination||!source)return DDERR_INVALIDPARAMS;
    RECT crop{};
    if(source_rectangle)crop=*source_rectangle;
    else{
        DDSURFACEDESC2 desc{};
        desc.dwSize=sizeof(desc);
        const auto hr=source->GetSurfaceDesc(&desc);
        if(FAILED(hr))return hr;
        crop={0,0,static_cast<LONG>(desc.dwWidth),
                  static_cast<LONG>(desc.dwHeight)};
    }
    // Ghidra 00415E50 stores camera offsets at 0051071C / 00510720.
    // Preserve the full original 640 x 480 viewport and source rectangle.
    const auto clipped=btb::full_game::retail_clip_blt(
        btb::full_game::Rect{crop.left,crop.top,crop.right,crop.bottom},
        screen_x-camera_x,screen_y-camera_y);
    if(!clipped.visible)return S_OK;
    RECT dst{clipped.destination.left,clipped.destination.top,
             clipped.destination.right,clipped.destination.bottom};
    RECT src{clipped.source.left,clipped.source.top,
             clipped.source.right,clipped.source.bottom};
    DDSURFACEDESC2 source_desc{},dest_desc{};
    source_desc.dwSize=sizeof(source_desc);
    dest_desc.dwSize=sizeof(dest_desc);
    auto hr=source->GetSurfaceDesc(&source_desc);
    if(FAILED(hr))return hr;
    hr=destination->GetSurfaceDesc(&dest_desc);
    if(FAILED(hr))return hr;
    const auto& a=source_desc.ddpfPixelFormat;
    const auto& b=dest_desc.ddpfPixelFormat;
    if((a.dwFlags&DDPF_RGB)&&(b.dwFlags&DDPF_RGB)&&
       a.dwRGBBitCount==32&&b.dwRGBBitCount==32&&
       a.dwRBitMask==b.dwRBitMask&&a.dwGBitMask==b.dwGBitMask&&
       a.dwBBitMask==b.dwBBitMask&&
       (a.dwRBitMask|a.dwGBitMask|a.dwBBitMask)!=0) {
        return blit_rgb32_keyed(destination,source,dst,src);
    }
    return destination->Blt(
        &dst,source,&src,DDBLT_KEYSRC|DDBLT_WAIT,nullptr);
}

} // namespace btb::original_win32
#endif
