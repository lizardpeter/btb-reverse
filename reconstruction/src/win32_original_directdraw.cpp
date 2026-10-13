#include "btb/win32_original_directdraw.hpp"
#include "btb/bitmap_registry.hpp"
#include "btb/full_game_blt_clip.hpp"
#include "btb/original_recompiled_bitmaps.hpp"
#include "btb/win32_recompiled_graphics.hpp"

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
        (void)registry_->unregister_surface_reference(&entry.surface);
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

    // Actual recovered retail function call chain:
    // 004038D0 LoadBitmapToDirectDrawSurface
    //      -> 004035F0 CopyBitmapToSurface
    //      -> DirectDraw7::GetDC / native GDI BitBlt / ReleaseDC.
    original_win32::LoadedBitmap loaded{};
    const auto hr=original_win32::load_bitmap_to_directdraw_surface(
        draw_,filename,loaded);
    if(FAILED(hr)){
        error=hresult_description("004038D0 retail bitmap load",hr);
        return false;
    }

    // Registration and keyed-flag changes occur after emplacement, so
    // 00403770 receives the stable address of the cached surface pointer.
    out={loaded.surface,loaded.width,loaded.height};
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
        // This is a real original 00403770 table registration, not the
        // earlier uint32-only source-effects representation.
        const auto source_name=requested.source.absolute_path.string();
        if(!registry_->register_surface_reference(
                &it->second.surface,source_name)){
            release_surface(it->second.surface);
            images_.erase(it);
            error="original 800-slot bitmap table cannot register source";
            return false;
        }
        // 004036E0 keys the registered address, and 00403B70 does the
        // native GDI/DirectDraw conversion and SetColorKey call.
        (void)registry_->mark_registered_surface_color_keyed(
            &it->second.surface);
        const auto keyed=original_win32::set_surface_transparency_color_key(
            it->second.surface,RGB(255,0,255));
        if(FAILED(keyed)){
            (void)registry_->unregister_surface_reference(
                &it->second.surface);
            release_surface(it->second.surface);
            images_.erase(it);
            error=hresult_description("00403B70 native surface key",keyed);
            return false;
        }
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
    const auto clipped=retail_clip_blt_to_rect(
        Rect{src.left,src.top,src.right,src.bottom},
        requested.original.x,requested.original.y,
        requested.original.destination_clip.value_or(
            Rect{0,0,display::kRetailWidth,display::kRetailHeight}));
    if (!clipped.visible) {
        // Fully offscreen is a valid original animation frame; it is
        // not a lost bitmap and must not abort the entire game tick.
        return true;
    }
    src={
        clipped.source.left,clipped.source.top,
        clipped.source.right,clipped.source.bottom
    };
    RECT dest{
        clipped.destination.left,clipped.destination.top,
        clipped.destination.right,clipped.destination.bottom
    };

    // 00415E50 is the keyed path; 00403500 is a direct BltFast
    // COM dispatch for unkeyed, preclipped original sprite surfaces.
    const HRESULT hr=requested.original.color_keyed
        ? original_win32::blit_color_keyed_surface_clipped(
            backbuffer_,it->second.surface,dest.left,dest.top,
            &src,0,0)
        : original_win32::blt_fast_to_backbuffer(
            backbuffer_,static_cast<DWORD>(dest.left),
            static_cast<DWORD>(dest.top),it->second.surface,
            &src,DDBLTFAST_WAIT);
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

    auto hr=primary_->Restore();
    if (SUCCEEDED(hr)) hr=backbuffer_->Restore();
    if (SUCCEEDED(hr)) hr=original_win32::restore_all_directdraw_surfaces(draw_);
    if (FAILED(hr)) {
        error=hresult_description("retail Restore surfaces",hr);
        return false;
    }
    // PE32 00403830 reloads live original surface-pointer variables
    // from filenames and restores keyed surfaces after a lost device.
    if(!registry_->reload_registered_surfaces(draw_)){
        release_cached_bitmaps();
        error="00403830 failed to reload original registered bitmap surfaces";
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
        // 00403540 issues the original native DDBLT_COLORFILL.
        auto hr=original_win32::clear_backbuffer(backbuffer_,0);
        // The original operation omits DDBLT_WAIT. Retry busy frames
        // without changing the recovered operation's flag contract.
        for(int spin=0;spin<32&&hr==DDERR_WASSTILLDRAWING;++spin)
            hr=original_win32::clear_backbuffer(backbuffer_,0);
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

    // Execute the original PE32 00402600 -> 00403480 presentation pair,
    // not the formerly hand-authored Flip/Blt approximation.
    const auto try_present=[&](bool& bitmap_reload) -> HRESULT {
        RECT destination{};
        const bool windowed=mode_==display::DisplayMode::Windowed;
        if(windowed && !original_win32::update_display_destination_rect(
                           hwnd_,true,destination))
            return E_FAIL;
        return original_win32::present_game_frame_retail(
            primary_,backbuffer_,draw_,windowed?&destination:nullptr,
            windowed,bitmap_reload);
    };

    bool bitmap_reload=false;
    auto hr=try_present(bitmap_reload);
    if (hr==DDERR_SURFACELOST && bitmap_reload) {
        // 00403480 already Restored both COM surfaces and the DD device.
        // Execute the companion 00403830 bitmap-registry reload (retail's
        // DAT_0051c320 flag), then replay the actual cached source frame.
        if(!registry_->reload_registered_surfaces(draw_)){
            release_cached_bitmaps();
            error="00403480/00403830 failed to reload original bitmaps";
            return false;
        }
        if(!render_ordered(previous_frame_,error))return false;
        bool lost_again=false;
        hr=try_present(lost_again);
        if(lost_again){
            error="00403480 reported a second lost surface during present";
            return false;
        }
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
