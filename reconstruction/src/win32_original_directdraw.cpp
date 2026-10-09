#include "btb/win32_original_directdraw.hpp"

#if defined(_WIN32)
#include <cstdint>
#include <string>
#include <utility>

namespace btb::full_game {
namespace {

std::string hresult_description(const char* op,HRESULT hr) {
    return std::string(op)+" failed (HRESULT "+
           std::to_string(static_cast<std::uint32_t>(hr))+")";
}

void release_surface(IDirectDrawSurface7*& surface) noexcept {
    if (surface) {
        surface->Release();
        surface=nullptr;
    }
}

} // namespace

Win32OriginalDirectDraw::~Win32OriginalDirectDraw() {
    shutdown();
}

void Win32OriginalDirectDraw::release_cached_bitmaps() noexcept {
    for (auto& [_,entry] : images_) {
        release_surface(entry.surface);
    }
    images_.clear();
}

void Win32OriginalDirectDraw::shutdown() noexcept {
    release_cached_bitmaps();
    previous_frame_.clear();
    release_surface(backbuffer_);
    release_surface(primary_);
    if (clipper_) {
        clipper_->Release();
        clipper_=nullptr;
    }
    if (draw_) {
        if (mode_ == display::DisplayMode::Fullscreen) {
            draw_->RestoreDisplayMode();
            draw_->SetCooperativeLevel(hwnd_,DDSCL_NORMAL);
        }
        draw_->Release();
        draw_=nullptr;
    }
    hwnd_=nullptr;
    surface_lost_during_draw_=false;
}

bool Win32OriginalDirectDraw::initialize(
    HWND hwnd,display::DisplayMode mode,std::string& error) {

    shutdown();
    if (!hwnd || !IsWindow(hwnd)) {
        error="DirectDraw7 requires a valid original-game HWND";
        return false;
    }
    hwnd_=hwnd;
    mode_=mode;

    auto hr=DirectDrawCreateEx(
        nullptr,reinterpret_cast<void**>(&draw_),IID_IDirectDraw7,nullptr);
    if (FAILED(hr)) {
        error=hresult_description("DirectDrawCreateEx",hr);
        shutdown();return false;
    }
    if (mode_ == display::DisplayMode::Fullscreen) {
        hr=draw_->SetCooperativeLevel(
            hwnd_,DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE);
        if (SUCCEEDED(hr)) {
            hr=draw_->SetDisplayMode(
                display::kRetailWidth,display::kRetailHeight,
                display::kRetailFullscreenBitsPerPixel,0,0);
        }
        if (FAILED(hr)) {
            error=hresult_description("fullscreen 640x480x16 setup",hr);
            shutdown();return false;
        }
        DDSURFACEDESC2 desc{};
        desc.dwSize=sizeof(desc);
        desc.dwFlags=DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
        desc.ddsCaps.dwCaps=
            DDSCAPS_PRIMARYSURFACE|DDSCAPS_COMPLEX|DDSCAPS_FLIP;
        desc.dwBackBufferCount=1;
        hr=draw_->CreateSurface(&desc,&primary_,nullptr);
        if (SUCCEEDED(hr)) {
            DDSCAPS2 caps{};
            caps.dwCaps=DDSCAPS_BACKBUFFER;
            hr=primary_->GetAttachedSurface(&caps,&backbuffer_);
        }
        if (FAILED(hr)) {
            error=hresult_description("retail fullscreen flip chain",hr);
            shutdown();return false;
        }
    } else {
        hr=draw_->SetCooperativeLevel(hwnd_,DDSCL_NORMAL);
        if (FAILED(hr)) {
            error=hresult_description("windowed cooperative level",hr);
            shutdown();return false;
        }
        DDSURFACEDESC2 primary_desc{};
        primary_desc.dwSize=sizeof(primary_desc);
        primary_desc.dwFlags=DDSD_CAPS;
        primary_desc.ddsCaps.dwCaps=DDSCAPS_PRIMARYSURFACE;
        hr=draw_->CreateSurface(&primary_desc,&primary_,nullptr);
        if (FAILED(hr)) {
            error=hresult_description("windowed primary surface",hr);
            shutdown();return false;
        }
        DDSURFACEDESC2 render_desc{};
        render_desc.dwSize=sizeof(render_desc);
        render_desc.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
        render_desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_VIDEOMEMORY;
        render_desc.dwWidth=display::kRetailWidth;
        render_desc.dwHeight=display::kRetailHeight;
        hr=draw_->CreateSurface(&render_desc,&backbuffer_,nullptr);
        if (FAILED(hr)) {
            error=hresult_description("windowed 640x480 render surface",hr);
            shutdown();return false;
        }
        hr=draw_->CreateClipper(0,&clipper_,nullptr);
        if (SUCCEEDED(hr)) hr=clipper_->SetHWnd(0,hwnd_);
        if (SUCCEEDED(hr)) hr=primary_->SetClipper(clipper_);
        if (FAILED(hr)) {
            error=hresult_description("native windowed clipper",hr);
            shutdown();return false;
        }
    }

    error.clear();
    return true;
}

bool Win32OriginalDirectDraw::load_bitmap(
    const std::filesystem::path& filename,
    OriginalSurface& out,std::string& error) {

    auto* hbitmap=reinterpret_cast<HBITMAP>(
        LoadImageW(nullptr,filename.c_str(),IMAGE_BITMAP,0,0,
                   LR_LOADFROMFILE|LR_CREATEDIBSECTION));
    if (!hbitmap) {
        error="original bitmap failed Win32 LoadImage: "+
              filename.generic_string();
        return false;
    }

    BITMAP bitmap{};
    if (!GetObjectW(hbitmap,sizeof(bitmap),&bitmap) ||
        bitmap.bmWidth <= 0 || bitmap.bmHeight <= 0) {
        DeleteObject(hbitmap);
        error="invalid native bitmap dimensions";
        return false;
    }

    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    desc.dwFlags=DDSD_CAPS|DDSD_WIDTH|DDSD_HEIGHT;
    desc.ddsCaps.dwCaps= bitmap.bmWidth <=
        display::kRetailWidth ? DDSCAPS_OFFSCREENPLAIN :
        DDSCAPS_OFFSCREENPLAIN;
    desc.dwWidth=static_cast<DWORD>(bitmap.bmWidth);
    desc.dwHeight=static_cast<DWORD>(bitmap.bmHeight);

    IDirectDrawSurface7* surface=nullptr;
    auto hr=draw_->CreateSurface(&desc,&surface,nullptr);
    if (FAILED(hr)) {
        // Matches the recovered registry's retry in system memory when
        // the default surface allocation cannot be provided.
        desc.ddsCaps.dwCaps=DDSCAPS_OFFSCREENPLAIN|DDSCAPS_SYSTEMMEMORY;
        hr=draw_->CreateSurface(&desc,&surface,nullptr);
    }
    if (FAILED(hr)) {
        DeleteObject(hbitmap);
        error=hresult_description("retail bitmap surface creation",hr);
        return false;
    }

    HDC dst=nullptr;
    hr=surface->GetDC(&dst);
    if (FAILED(hr)) {
        surface->Release();
        DeleteObject(hbitmap);
        error=hresult_description("GetDC bitmap destination",hr);
        return false;
    }
    HDC source=CreateCompatibleDC(dst);
    if (!source) {
        surface->ReleaseDC(dst);
        surface->Release();
        DeleteObject(hbitmap);
        error="CreateCompatibleDC failed during original bitmap upload";
        return false;
    }
    const auto old=SelectObject(source,hbitmap);
    const BOOL copied=BitBlt(
        dst,0,0,bitmap.bmWidth,bitmap.bmHeight,
        source,0,0,SRCCOPY);
    SelectObject(source,old);
    DeleteDC(source);
    surface->ReleaseDC(dst);
    DeleteObject(hbitmap);
    if (!copied) {
        surface->Release();
        error="BitBlt failed to upload original BMP pixels";
        return false;
    }

    // The recovered bitmap registry uses a magenta source color key.
    // The actual stored pixel value differs in 16-bit fullscreen RGB
    // and 32-bit/windowed modes: GetPixelFormat supplies the masks.
    DDPIXELFORMAT pf{};
    pf.dwSize=sizeof(pf);
    hr=surface->GetPixelFormat(&pf);
    if (FAILED(hr) || !(pf.dwFlags&DDPF_RGB)) {
        surface->Release();
        error="original bitmap surface has unknown DirectDraw RGB masks";
        return false;
    }
    DDCOLORKEY magenta{};
    magenta.dwColorSpaceLowValue=pf.dwRBitMask|pf.dwBBitMask;
    magenta.dwColorSpaceHighValue=magenta.dwColorSpaceLowValue;
    hr=surface->SetColorKey(DDCKEY_SRCBLT,&magenta);
    if (FAILED(hr)) {
        surface->Release();
        error=hresult_description("native magenta color key",hr);
        return false;
    }

    out={surface,bitmap.bmWidth,bitmap.bmHeight};
    error.clear();
    return true;
}

bool Win32OriginalDirectDraw::blit(
    const ResolvedDraw& requested,std::string& error) {

    if (!requested.source.found()) {
        error="cannot blit an unresolved original bitmap";
        return false;
    }
    const auto key=requested.source.absolute_path.wstring();
    auto it=images_.find(key);
    if (it == images_.end()) {
        OriginalSurface loaded{};
        if (!load_bitmap(requested.source.absolute_path,loaded,error)) {
            return false;
        }
        it=images_.emplace(key,loaded).first;
    }

    RECT src{0,0,it->second.width,it->second.height};
    if (requested.original.source_rectangle) {
        const auto& r=*requested.original.source_rectangle;
        src={r.left,r.top,r.right,r.bottom};
    }
    if (src.left<0 || src.top<0 ||
        src.right>it->second.width || src.bottom>it->second.height ||
        src.right<=src.left || src.bottom<=src.top) {
        error="retail sprite source rectangle extends beyond original BMP";
        return false;
    }
    RECT dest{
        requested.original.x,
        requested.original.y,
        requested.original.x+(src.right-src.left),
        requested.original.y+(src.bottom-src.top)
    };

    DWORD flags=DDBLT_WAIT;
    if (requested.original.color_keyed) {
        flags|=DDBLT_KEYSRC;
    }
    const auto hr=backbuffer_->Blt(
        &dest,it->second.surface,&src,flags,nullptr);
    if (hr==DDERR_SURFACELOST) {
        surface_lost_during_draw_=true;
        error="DirectDraw surface lost during original sprite blit";
        return false;
    }
    if (FAILED(hr)) {
        error=hresult_description("retail sprite Blt",hr);
        return false;
    }
    return true;
}

bool Win32OriginalDirectDraw::restore_lost_surfaces(
    std::string& error) {

    release_cached_bitmaps();
    auto hr=primary_->Restore();
    if (SUCCEEDED(hr)) hr=backbuffer_->Restore();
    if (FAILED(hr)) {
        error=hresult_description("retail Restore surfaces",hr);
        return false;
    }
    error.clear();
    return true;
}

bool Win32OriginalDirectDraw::render_ordered(
    const std::vector<ResolvedDraw>& stream,std::string& error) {

    if (!initialized()) {
        error="DirectDraw renderer has not created original primary/backbuffer";
        return false;
    }
    // Retry the ENTIRE ordered frame after losing a surface. A partial
    // retry would omit earlier blits whose pixels were lost on Restore.
    for (int attempt=0;attempt<2;++attempt) {
        surface_lost_during_draw_=false;
        DDBLTFX clear{};
        clear.dwSize=sizeof(clear);
        clear.dwFillColor=0;
        const auto hr=backbuffer_->Blt(
            nullptr,nullptr,nullptr,DDBLT_COLORFILL|DDBLT_WAIT,&clear);
        if (hr==DDERR_SURFACELOST) {
            surface_lost_during_draw_=true;
            error="DirectDraw backbuffer lost during clear";
        } else if (FAILED(hr)) {
            error=hresult_description("original frame color-fill",hr);
            return false;
        } else {
            bool valid=true;
            for (const auto& draw : stream) {
                if (!blit(draw,error)) {
                    valid=false;
                    break;
                }
            }
            if (valid) {
                previous_frame_=stream;
                error.clear();
                return true;
            }
        }
        if (!surface_lost_during_draw_ ||
            !restore_lost_surfaces(error)) {
            return false;
        }
    }
    error="DirectDraw surface was lost twice during one native frame";
    return false;
}

bool Win32OriginalDirectDraw::present(std::string& error) {
    if (!initialized()) {
        error="cannot present before original display initialization";
        return false;
    }

    const auto try_present=[&]() -> HRESULT {
        if (mode_==display::DisplayMode::Fullscreen) {
            return primary_->Flip(nullptr,DDFLIP_WAIT);
        }
        RECT dest{};
        if (!GetClientRect(hwnd_,&dest)) return E_FAIL;
        POINT top_left{dest.left,dest.top};
        POINT bottom_right{dest.right,dest.bottom};
        if (!ClientToScreen(hwnd_,&top_left) ||
            !ClientToScreen(hwnd_,&bottom_right)) return E_FAIL;
        dest={top_left.x,top_left.y,bottom_right.x,bottom_right.y};
        return primary_->Blt(&dest,backbuffer_,nullptr,DDBLT_WAIT,nullptr);
    };

    auto hr=try_present();
    if (hr==DDERR_SURFACELOST) {
        if (!restore_lost_surfaces(error) ||
            !render_ordered(previous_frame_,error)) {
            return false;
        }
        hr=try_present();
    }
    if (FAILED(hr)) {
        error=hresult_description("DirectDraw7 Blt/Flip present",hr);
        return false;
    }
    error.clear();
    return true;
}

} // namespace btb::full_game
#endif
