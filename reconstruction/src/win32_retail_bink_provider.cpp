#include "btb/win32_retail_bink_provider.hpp"

#if defined(_WIN32)
#include <cstdint>
#include <string>
#include <system_error>

namespace btb::full_game {
namespace {
struct OriginalBink32Header {
    std::uint32_t width{},height{},frames{},frame_num{};
    std::uint32_t last_frame{},fps_num{},fps_den{};
};
static_assert(sizeof(OriginalBink32Header)==28);
// Original PE32 reads FrameNum at HBINK+0x0c and Frames at +0x08.
// See 0x004090D0, and the original RAD Bink 1 header ABI.
#if !defined(_WIN64)
using BinkOpenFn=void* (WINAPI*)(const char*,std::uint32_t);
using BinkCloseFn=void (WINAPI*)(void*);
using BinkFrameFn=std::int32_t (WINAPI*)(void*);
using BinkCopyFn=std::int32_t (WINAPI*)(
    void*,void*,std::int32_t,std::uint32_t,
    std::uint32_t,std::uint32_t,std::uint32_t);
using BinkNextFn=void (WINAPI*)(void*);
using BinkSurfaceFn=std::uint32_t (WINAPI*)(IDirectDrawSurface7*);
using BinkSetAudioFn=std::int32_t (WINAPI*)(FARPROC,std::uint32_t);

template<class Fn>
Fn require_export(Win32OriginalBinkExports& dll,
                  const char* name) noexcept {
    return reinterpret_cast<Fn>(dll.find(name));
}
#endif
}

Win32RetailBinkProvider::~Win32RetailBinkProvider() {
    close_global();
}
void Win32RetailBinkProvider::close_global() noexcept {
#if !defined(_WIN64)
    if(global_) {
        if(auto close=require_export<BinkCloseFn>(
            exports_,"BinkClose")) close(global_);
        global_=nullptr;
    }
#endif
}

bool Win32RetailBinkProvider::ensure_original_dll(
    std::string& error) {
#if defined(_WIN64)
    error="the original 2002 binkw32.dll uses an x86 ABI; "
          "launch the native 32-bit Windows game or use a verified "
          "compatible native x64 decoder";
    return false;
#else
    if(exports_.loaded()) {error.clear();return true;}
    std::error_code ec;
    const std::filesystem::path candidates[]{
        root_/"Exe"/"binkw32.dll",
        root_/"binkw32.dll"
    };
    for(const auto& path:candidates) {
        if(std::filesystem::is_regular_file(path,ec) && !ec) {
            if(!exports_.load_exact_original(
                   std::filesystem::absolute(path),error))return false;
            const auto init=require_export<BinkSetAudioFn>(
                exports_,"BinkSetSoundSystem");
            const auto direct_sound=exports_.find("BinkOpenDirectSound");
            if(!init || !direct_sound) {
                error="original Bink audio backend exports unavailable";
                exports_.unload();
                return false;
            }
            // Original InitializeBinkPlaybackSystem at 0x408D90.
            // Its return is not checked by the original executable.
            (void)init(direct_sound,0);
            error.clear();return true;
        }
        ec.clear();
    }
    error="original binkw32.dll not found beside game EXE or root";
    return false;
#endif
}

bool Win32RetailBinkProvider::open_global(
    const std::filesystem::path& movie,std::string& error) {
#if defined(_WIN64)
    return ensure_original_dll(error);
#else
    if(!ensure_original_dll(error))return false;
    close_global();
    if(!renderer_.bink_render_target()) {
        error="original DirectDraw7 Bink render surface unavailable";
        return false;
    }
    std::error_code ec;
    if(!std::filesystem::is_regular_file(movie,ec) || ec) {
        error="original Bink movie missing: "+movie.string();
        return false;
    }
    auto open=require_export<BinkOpenFn>(exports_,"BinkOpen");
    if(!open) {error="original BinkOpen export unavailable";return false;}
    // Retail 0x408EB0 opens global movies with flags 0, unlike
    // the separate Bink-in-memory instruction helper.
    const auto native_path=std::filesystem::absolute(movie).string();
    global_=open(native_path.c_str(),0);
    if(!global_) {
        error="original BinkOpen failed: "+movie.string();
        return false;
    }
    const auto& header=*static_cast<OriginalBink32Header*>(global_);
    if(header.width==0 || header.height==0 ||
       header.width>640 || header.height>480 ||
       header.frames==0 || header.frame_num==0 ||
       header.frame_num>header.frames ||
       header.fps_den==0) {
        close_global();
        error="original BinkOpen returned invalid frame geometry/header";
        return false;
    }
    error.clear();return true;
#endif
}

bool Win32RetailBinkProvider::service(
    bool& global_finished,std::string& error) {
    global_finished=false;
#if defined(_WIN64)
    error.clear();return true; // No movie can open via x86 DLL.
#else
    if(!global_) {error.clear();return true;}
    auto* surface=renderer_.bink_render_target();
    if(!surface) {
        error="Bink render target was destroyed while movie active";
        return false;
    }
    const auto wait=require_export<BinkFrameFn>(exports_,"BinkWait");
    const auto decode=require_export<BinkFrameFn>(exports_,"BinkDoFrame");
    const auto copy=require_export<BinkCopyFn>(
        exports_,"BinkCopyToBuffer");
    const auto format=require_export<BinkSurfaceFn>(
        exports_,"BinkDDSurfaceType");
    const auto next=require_export<BinkNextFn>(exports_,"BinkNextFrame");
    if(!wait||!decode||!copy||!format||!next) {
        error="original Bink frame API is incomplete";
        return false;
    }
    // BinkWait=nonzero means the frame is not yet due. Retain
    // the last REAL DirectDraw frame, not a fake playback clock.
    if(wait(global_)!=0) {error.clear();return true;}

    (void)decode(global_);
    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    const auto hr=surface->Lock(
        nullptr,&desc,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(hr)) {
        error="original Bink DirectDraw7 backbuffer lock failed (HRESULT "+
              std::to_string(static_cast<std::uint32_t>(hr))+")";
        return false;
    }
    const auto pixel_format=format(surface);
    const auto copy_result=copy(global_,desc.lpSurface,
        desc.lPitch,desc.dwHeight,0,0,pixel_format);
    const auto release_hr=surface->Unlock(nullptr);
    if(FAILED(release_hr)) {
        error="original Bink DirectDraw7 backbuffer unlock failed";
        return false;
    }
    if(copy_result<0) {
        error="original BinkCopyToBuffer frame conversion failed";
        return false;
    }
    // Source 0x4090D0 checks the observed HBINK frame header
    // before advancing. Never infer EOS from elapsed wall time.
    const auto& header=*static_cast<OriginalBink32Header*>(global_);
    if(header.frame_num>=header.frames) {
        close_global();
        global_finished=true;
    } else {
        next(global_);
    }
    error.clear();return true;
#endif
}

bool Win32RetailBinkProvider::open_walkthrough(
    const std::filesystem::path&,const std::filesystem::path&,
    std::string& error) {
    error="original instruction walkthrough Bink overlay requires "
          "its separate 282x179 DirectDraw surface; no synthetic overlay";
    return false;
}
bool Win32RetailBinkProvider::close_walkthrough(std::string& error) {
    error="no original Bink walkthrough is currently open";
    return false;
}
bool Win32RetailBinkProvider::render_walkthrough_overlay(
    std::string& error) {
    // A walkthrough cannot be opened until its native surface path
    // is connected. Do not fabricate a Bink overlay or advance audio.
    error.clear();return true;
}

} // namespace btb::full_game
#endif
