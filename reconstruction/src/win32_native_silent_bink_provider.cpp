#include "btb/win32_native_silent_bink_provider.hpp"
#if defined(_WIN32)
#include "btb/win32_native_bink_surface.hpp"
#include <cstdint>
#include <filesystem>
#include <utility>
#include <windows.h>

namespace btb::full_game {
namespace {
// Actual original PE32 0x428195 walks a dedicated 282x179 DD7 movie
// rectangle at (0x3F,0x9D) AFTER drawing the instruction artwork.
constexpr int kWalkthroughX=0x3f,kWalkthroughY=0x9d;
constexpr int kWalkthroughW=282,kWalkthroughH=179;
constexpr int kGlobalW=640,kGlobalH=480;
[[nodiscard]] std::uint64_t clock_us() noexcept {
    return static_cast<std::uint64_t>(GetTickCount64())*1000;
}
}

Win32NativeSilentBinkProvider::~Win32NativeSilentBinkProvider() {
    release_walkthrough();
}

bool Win32NativeSilentBinkProvider::open_global(
    const std::filesystem::path& movie,std::string& error) {
    global_.reset();
    auto opened=bink32_recovered::BinkNativeSilentMovie::open_file(
        movie,error);
    if(!opened)return false;
    if(opened->width()!=kGlobalW||opened->height()!=kGlobalH) {
        error="native Bink global movie must match original 640x480 backbuffer";
        return false;
    }
    if(!renderer_.bink_render_target()) {
        error="native Bink global DirectDraw7 target is unavailable";
        return false;
    }
    global_=std::move(*opened);
    error.clear();
    return true;
}

bool Win32NativeSilentBinkProvider::service(
    bool& global_finished,std::string& error) {
    global_finished=false;
    if(!global_) {error.clear();return true;}
    // The *actual* decoded frame enters the real original DD7
    // backbuffer; neither an image placeholder nor a guessed movie timer.
    const bool ok=global_->service(
        clock_us(),
        [this](const bink32_recovered::BinkNativeYuVaFrame& frame,
               std::string& out_error) {
            return copy_native_bink_frame_to_directdraw(
                renderer_.bink_render_target(),frame,out_error);
        },
        global_finished,error);
    if(!ok)return false;
    if(global_finished)global_.reset();
    return true;
}

void Win32NativeSilentBinkProvider::release_walkthrough() noexcept {
    walkthrough_.reset();
    if(walkthrough_surface_) {
        walkthrough_surface_->Release();
        walkthrough_surface_=nullptr;
    }
}

bool Win32NativeSilentBinkProvider::open_walkthrough(
    const std::filesystem::path& movie,
    const std::filesystem::path& help_wav,
    std::string& error) {
    // Narration is a separate original managed-sound WAV; it is NOT
    // a reason to fabricate compressed audio in the BIK decoder.
    (void)help_wav;
    auto opened=bink32_recovered::BinkNativeSilentMovie::open_file(
        movie,error);
    if(!opened)return false;
    if(opened->width()!=kWalkthroughW||
       opened->height()!=kWalkthroughH) {
        error="native walkthrough requires original 282x179 geometry";
        return false;
    }
    release_walkthrough();
    if(!renderer_.create_bink_offscreen_surface(
           kWalkthroughW,kWalkthroughH,walkthrough_surface_,error))
        return false;
    walkthrough_=std::move(*opened);
    error.clear();
    return true;
}

bool Win32NativeSilentBinkProvider::close_walkthrough(std::string& error) {
    release_walkthrough();
    error.clear();
    return true;
}

bool Win32NativeSilentBinkProvider::render_walkthrough_overlay(
    std::string& error) {
    if(!walkthrough_) {error.clear();return true;}
    const auto play=[this](
        const bink32_recovered::BinkNativeYuVaFrame& frame,
        std::string& out_error) {
        return copy_native_bink_frame_to_directdraw(
            walkthrough_surface_,frame,out_error);
    };
    bool finished=false;
    const auto now=clock_us();
    if(!walkthrough_->service(now,play,finished,error))return false;
    if(finished) {
        // Loop only AFTER terminal frame pixels were physically uploaded
        // and held for their complete presentation interval.
        walkthrough_->rewind();
        if(!walkthrough_->service(now,play,finished,error))return false;
    }
    if(!walkthrough_->has_presented_frame()) {
        error.clear();
        return true;
    }
    // render_ordered() clears the game backbuffer every update. Reblit
    // unchanged source pixels every frame, not only on decoder advances.
    return renderer_.blit_bink_offscreen_surface(
        walkthrough_surface_,kWalkthroughW,kWalkthroughH,
        kWalkthroughX,kWalkthroughY,error);
}
}
#endif
