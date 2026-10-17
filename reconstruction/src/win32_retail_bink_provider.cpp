#include "btb/win32_retail_bink_provider.hpp"

#if defined(_WIN32)
#include <cstdint>
#include <fstream>
#include <limits>
#include <vector>
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
using BinkGotoFn=void (WINAPI*)(void*,std::uint32_t,std::uint32_t);

template<class Fn>
Fn require_export(Win32OriginalBinkExports& dll,
                  const char* name) noexcept {
    return reinterpret_cast<Fn>(dll.find(name));
}
#endif
// An HBINK positioned at Frames is not proof that frame Frames has
// already been copied to DirectDraw. In a normal BinkNextFrame sequence
// the decoder reaches Frames *before* BinkDoFrame decodes that image.
// Only loop after the terminal image was physically decoded once.
[[nodiscard]] constexpr bool walkthrough_terminal_frame_was_copied(
    std::uint32_t current,std::uint32_t frames,
    std::uint32_t last_decoded,bool has_frame) noexcept {
    return frames>0 && current==frames &&
           has_frame && last_decoded==frames;
}
static_assert(!walkthrough_terminal_frame_was_copied(2,3,2,true));
static_assert(!walkthrough_terminal_frame_was_copied(3,3,2,true));
static_assert(!walkthrough_terminal_frame_was_copied(3,3,3,false));
static_assert(walkthrough_terminal_frame_was_copied(3,3,3,true));
static_assert(!walkthrough_terminal_frame_was_copied(0,0,0,true));
static_assert(walkthrough_terminal_frame_was_copied(1,1,1,true));
}

Win32RetailBinkProvider::~Win32RetailBinkProvider() {
    release_walkthrough();
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

void Win32RetailBinkProvider::release_walkthrough() noexcept {
#if !defined(_WIN64)
    if(walkthrough_) {
        if(auto close=require_export<BinkCloseFn>(
               exports_,"BinkClose"))
            close(walkthrough_);
        walkthrough_=nullptr;
    }
#endif
    if(walkthrough_surface_) {
        walkthrough_surface_->Release();
        walkthrough_surface_=nullptr;
    }
    walkthrough_bytes_.clear();
    walkthrough_has_frame_=false;
    walkthrough_last_decoded_=0;
    walkthrough_help_wav_.clear();
}

bool Win32RetailBinkProvider::open_walkthrough(
    const std::filesystem::path& movie,
    const std::filesystem::path& help_wav,
    std::string& error) {
#if defined(_WIN64)
    (void)movie;(void)help_wav;
    return ensure_original_dll(error);
#else
    if(!ensure_original_dll(error))return false;
    // The Ghidra-recovered 0x4281D0 loads at most 7,000,000
    // original bytes into a process-owned memory allocation and
    // opens Bink with exactly 0x04000000 (memory input).
    // Keeping this buffer alive is mandatory until BinkClose.
    release_walkthrough();
    std::error_code ec;
    if(!std::filesystem::is_regular_file(movie,ec)||ec ||
       !std::filesystem::is_regular_file(help_wav,ec)||ec) {
        error="original walkthrough Bink or accompanying speech asset missing";
        return false;
    }
    std::ifstream source(movie,std::ios::binary|std::ios::ate);
    if(!source) {
        error="original walkthrough Bink file cannot be read";
        return false;
    }
    const auto length=source.tellg();
    constexpr std::streamoff kRetailMaxBytes=7000000;
    if(length<=0||length>kRetailMaxBytes) {
        error="original walkthrough Bink exceeds retail 7MB memory limit";
        return false;
    }
    walkthrough_bytes_.resize(static_cast<std::size_t>(length));
    source.seekg(0,std::ios::beg);
    if(!source.read(
            reinterpret_cast<char*>(walkthrough_bytes_.data()),
            static_cast<std::streamsize>(walkthrough_bytes_.size()))) {
        release_walkthrough();
        error="failed to read original in-memory walkthrough Bink";
        return false;
    }
    if(!renderer_.create_bink_offscreen_surface(
           kOriginalWalkthroughMovieWidth,
           kOriginalWalkthroughMovieHeight,walkthrough_surface_,error)) {
        release_walkthrough();
        return false;
    }
    const auto open=require_export<BinkOpenFn>(exports_,"BinkOpen");
    if(!open) {
        release_walkthrough();
        error="original walkthrough BinkOpen export missing";
        return false;
    }
    walkthrough_=open(
        reinterpret_cast<const char*>(walkthrough_bytes_.data()),
        0x04000000u);
    if(!walkthrough_) {
        release_walkthrough();
        error="retail BinkOpen failed for 7MB-bounded walkthrough bytes";
        return false;
    }
    const auto& header=*static_cast<OriginalBink32Header*>(walkthrough_);
    if(header.width==0||header.height==0||
       header.width>kOriginalWalkthroughMovieWidth||
       header.height>kOriginalWalkthroughMovieHeight||
       header.frames==0||header.frame_num==0||
       header.frame_num>header.frames) {
        release_walkthrough();
        error="original in-memory walkthrough Bink header invalid";
        return false;
    }
    walkthrough_help_wav_=help_wav;
    walkthrough_has_frame_=false;
    walkthrough_last_decoded_=0;
    error.clear();
    return true;
#endif
}

bool Win32RetailBinkProvider::close_walkthrough(std::string& error) {
    // The retail 0x4280D0 release path is safe even when an
    // instruction screen was never opened or already exited.
    release_walkthrough();
    error.clear();
    return true;
}

bool Win32RetailBinkProvider::decode_walkthrough_frame(
    std::string& error) {
#if defined(_WIN64)
    error="walkthrough playback requires original x86 Bink DLL";
    return false;
#else
    if(!walkthrough_) {
        error="original walkthrough decoder is not active";
        return false;
    }
    const auto wait=require_export<BinkFrameFn>(exports_,"BinkWait");
    const auto decode=require_export<BinkFrameFn>(exports_,"BinkDoFrame");
    const auto copy=require_export<BinkCopyFn>(
        exports_,"BinkCopyToBuffer");
    const auto format=require_export<BinkSurfaceFn>(
        exports_,"BinkDDSurfaceType");
    const auto next=require_export<BinkNextFn>(
        exports_,"BinkNextFrame");
    const auto goto_frame=require_export<BinkGotoFn>(
        exports_,"BinkGoto");
    if(!wait||!decode||!copy||!format||!next||!goto_frame) {
        error="original walkthrough Bink frame/loop exports incomplete";
        return false;
    }
    auto& header=*static_cast<OriginalBink32Header*>(walkthrough_);
    // 0x428120 loops at the observed final Bink frame, using
    // original BinkGoto(handle,1,0), NOT a synthetic time reset.
    if(walkthrough_terminal_frame_was_copied(
           header.frame_num,header.frames,
           walkthrough_last_decoded_,walkthrough_has_frame_)) {
        goto_frame(walkthrough_,1,0);
        walkthrough_has_frame_=false;
    }
    if(wait(walkthrough_)!=0) {
        error.clear();
        return true; // use previously decoded pixels this frame
    }
    (void)decode(walkthrough_);
    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    const auto hr=walkthrough_surface_->Lock(
        nullptr,&desc,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(hr)) {
        error="original walkthrough Bink DD7 surface lock failed";
        return false;
    }
    const auto type=format(walkthrough_surface_);
    const auto copied=copy(walkthrough_,desc.lpSurface,
        desc.lPitch,desc.dwHeight,0,0,type);
    const auto unlocked=walkthrough_surface_->Unlock(nullptr);
    if(FAILED(unlocked)||copied<0) {
        error="original walkthrough Bink pixel copy/unlock failed";
        return false;
    }
    walkthrough_has_frame_=true;
    walkthrough_last_decoded_=header.frame_num;
    if(header.frame_num<header.frames)next(walkthrough_);
    error.clear();return true;
#endif
}

bool Win32RetailBinkProvider::render_walkthrough_overlay(
    std::string& error) {
    if(!walkthrough_) {
        error.clear();
        return true;
    }
    if(!decode_walkthrough_frame(error))return false;
    if(!walkthrough_has_frame_) {
        error.clear();return true;
    }
    // These are NOT guessed center coordinates: actual retail
    // PE32 0x00428195..0x004281A5 pushes (x=0x3F,y=0x9D)
    // when invoking DecodeAndBlitBinkFrame(0x408F70). The
    // original DD7 playback surface is 282x179 (0x4281D0).
    return renderer_.blit_bink_offscreen_surface(
        walkthrough_surface_,
        kOriginalWalkthroughMovieWidth,kOriginalWalkthroughMovieHeight,
        kOriginalWalkthroughMovieX,kOriginalWalkthroughMovieY,error);
}

} // namespace btb::full_game
#endif
