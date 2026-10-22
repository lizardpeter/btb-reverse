#include "btb/win32_native_silent_bink_provider.hpp"
#if defined(_WIN32)
#include "btb/win32_native_bink_surface.hpp"
#include "btb/bink32_audio_movie.hpp"
#include "btb/bink32_container.hpp"
#include <cstdint>
#include <fstream>
#include <memory>
#include <optional>
#include <string>
#include <vector>
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
    reset_global();
}
void Win32NativeSilentBinkProvider::reset_global() noexcept {
    global_.reset();
    voice_.reset();
    voice_started_=false;
    audio_clock_.reset();
}

bool Win32NativeSilentBinkProvider::close_global(std::string& error) {
    reset_global();
    error.clear();
    return true;
}

bool Win32NativeSilentBinkProvider::open_global(
    const std::filesystem::path& movie,std::string& error) {
    reset_global();
    // Safe/default behavior: use this reconstructed renderer ONLY for
    // movies with no Bink compressed audio track.
    auto opened=bink32_recovered::BinkNativeSilentMovie::open_file(
        movie,error);
    if(!opened && experimental_audio_) {
        // Explicit developer opt-in. Decode the original retail media
        // READ ONLY and fully in memory, never to an extracted file/cache.
        std::ifstream input(movie,std::ios::binary|std::ios::ate);
        const auto bytes_count=input?input.tellg():std::streampos(-1);
        if(bytes_count<=0 || bytes_count>100'000'000) {
            error="experimental original Bink movie not readable/bounded";
            return false;
        }
        std::vector<std::uint8_t> bytes(
            static_cast<std::size_t>(bytes_count));
        input.seekg(0);
        if(!input.read(reinterpret_cast<char*>(bytes.data()),
                       static_cast<std::streamsize>(bytes.size()))) {
            error="cannot read experimental original Bink movie";
            return false;
        }
        const auto index=bink32_recovered::parse_container_index(bytes);
        // Audio track selection/mixing/translation must be explicitly
        // recovered. Do not quietly drop any source audio tracks.
        if(!index || index->tracks.size()!=1) {
            error="experimental native A/V requires exactly one source track";
            return false;
        }
        const auto audio=bink32_recovered::decode_bink1_movie_audio(
            bytes,0,error);
        if(!audio)return false;
        auto candidate=bink32_recovered::BinkNativeSilentMovie::open_bytes(
            std::move(bytes),error,true);
        if(!candidate)return false;
        auto sound=std::make_unique<Win32NativeBinkPcmVoice>(
            audio_.device_for_native_bink_pcm());
        if(!sound->load(*audio,error))return false;
        audio_clock_.emplace(audio->sample_rate,audio->sample_frames());
        voice_=std::move(sound);
        opened=std::move(candidate);
    }
    if(!opened)return false;
    if(opened->width()!=kGlobalW||opened->height()!=kGlobalH) {
        reset_global();
        error="native Bink global movie must match original 640x480 backbuffer";
        return false;
    }
    if(!renderer_.bink_render_target()) {
        reset_global();
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
    auto timestamp=clock_us();
    bool audio_finished=true;
    if(voice_) {
        // DirectSound's actual playback cursor, not an invented timer,
        // serves as the master clock for a native A/V movie.
        if(!voice_started_) {
            if(!voice_->start(error))return false;
            voice_started_=true;
        }
        auto position=voice_->observe(error);
        if(!position)return false;
        audio_finished=position->complete;
        if(!audio_clock_) {
            error="experimental native Bink audio clock was not initialized";
            return false;
        }
        const auto master=audio_clock_->observe(
            timestamp,position->decoded_sample_frame,
            position->complete);
        if(!master) {
            error="experimental native Bink audio playback timebase invalid";
            return false;
        }
        timestamp=*master;
    }
    bool video_finished=false;
    const bool ok=global_->service(
        timestamp,
        [this](const bink32_recovered::BinkNativeYuVaFrame& frame,
               std::string& out_error) {
            return copy_native_bink_frame_to_directdraw(
                renderer_.bink_render_target(),frame,out_error);
        },
        video_finished,error);
    if(!ok)return false;
    // Do not close the audio buffer simply because the last video frame
    // was presented: the real sound may still be completing.
    if(video_finished && audio_finished) {
        reset_global();
        global_finished=true;
    }
    error.clear();
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
