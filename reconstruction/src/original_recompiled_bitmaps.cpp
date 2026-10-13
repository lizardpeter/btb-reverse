#include "btb/original_recompiled_bitmaps.hpp"
#if defined(_WIN32)
#include "btb/full_game_blt_clip.hpp"
#include "btb/bitmap_registry.hpp"

#include <utility>

namespace btb::original_win32 {

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
    return destination->Blt(
        &dst,source,&src,DDBLT_KEYSRC|DDBLT_WAIT,nullptr);
}

} // namespace btb::original_win32
#endif
