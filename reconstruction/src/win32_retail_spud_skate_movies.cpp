#include "btb/win32_retail_spud_skate_movies.hpp"

#if defined(_WIN32)
#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <vector>
#include <limits>
#include <string>
#include <system_error>

namespace btb::full_game {
namespace {
// The original executable at 0x424690 reads the Bink1 header's
// Frames at +8 and FrameNum at +12 on all 5 HBINK pointers.
// These are observed decoder fields, not invented host timestamps.
struct Bink1Header {
    std::uint32_t width{},height{},frames{},frame_num{};
};
static_assert(sizeof(Bink1Header)==16);

#if !defined(_WIN64)
using Open=void* (WINAPI*)(const char*,std::uint32_t);
using Close=void (WINAPI*)(void*);
using Frame=std::int32_t (WINAPI*)(void*);
using Next=void (WINAPI*)(void*);
using Goto=void (WINAPI*)(void*,std::uint32_t,std::uint32_t);
using Copy=std::int32_t (WINAPI*)(
    void*,void*,std::int32_t,std::uint32_t,
    std::uint32_t,std::uint32_t,std::uint32_t);
using SurfaceFormat=std::uint32_t (WINAPI*)(IDirectDrawSurface7*);
using SetSound=std::int32_t (WINAPI*)(FARPROC,std::uint32_t);
template<class T>
[[nodiscard]] T dll_function(
    Win32OriginalBinkExports& dll,const char* symbol) noexcept {
    return reinterpret_cast<T>(dll.find(symbol));
}
[[nodiscard]] const Bink1Header& header(void* stream) noexcept {
    return *static_cast<const Bink1Header*>(stream);
}
#endif
}

Win32RetailSpudSkateMovies::~Win32RetailSpudSkateMovies() {
    std::string ignored;
    (void)close(ignored);
}

bool Win32RetailSpudSkateMovies::load_dll(std::string& error) {
#if defined(_WIN64)
    error="Spud Skate original binkw32.dll is 32-bit: use the native "
          "Win32 x86 executable or a separately verified 64-bit decoder";
    return false;
#else
    if(exports_.loaded()) {error.clear();return true;}
    const std::array<std::filesystem::path,2> locations{{
        root_/"Exe"/"binkw32.dll",root_/"binkw32.dll"
    }};
    std::error_code ec;
    for(const auto& location:locations) {
        if(!std::filesystem::is_regular_file(location,ec)||ec) {
            ec.clear();
            continue;
        }
        if(!exports_.load_exact_original(
                std::filesystem::absolute(location),error))
            return false;
        const auto set=dll_function<SetSound>(
            exports_,"BinkSetSoundSystem");
        auto direct=exports_.find("BinkOpenDirectSound");
        if(!set||!direct) {
            error="original five-stream Bink sound system exports unavailable";
            exports_.unload();
            return false;
        }
        (void)set(direct,0); // exact original 0x408D90 audio setup
        error.clear();return true;
    }
    error="cannot find original binkw32.dll under Exe/ or game root";
    return false;
#endif
}

bool Win32RetailSpudSkateMovies::open(
    const std::array<std::filesystem::path,5>& original_binks,
    std::string& error) {
#if defined(_WIN64)
    (void)original_binks;
    return load_dll(error);
#else
    if(active_) {
        error="Spud Skate movie decoder already owns five Bink streams";
        return false;
    }
    if(!load_dll(error))return false;
    const auto open=dll_function<Open>(exports_,"BinkOpen");
    if(!open) {error="original BinkOpen missing";return false;}

    const std::array<std::string,5> expected{{
        "bad.bik","normal.bik","ok.bik","good.bik","end.bik"
    }};
    std::error_code ec;
    for(std::size_t i=0;i<handles_.size();++i) {
        if(original_binks[i].filename().string()!=expected[i] ||
           !std::filesystem::is_regular_file(original_binks[i],ec) || ec) {
            error="Spud Skate requires original "+expected[i]+
                  " (not an arbitrary substituted Bink)";
            std::string ignored;(void)close(ignored);
            return false;
        }
        // Exact retail 0x00424360, per file: read no more than
        // 7,000,000 bytes, malloc an owned copy, then
        // OpenBinkMovieWithFallback(pointer) -> BinkOpen(pointer,
        // 0x04000000). The original byte buffer must remain valid
        // until its particular BinkClose, including resynchronization.
        std::ifstream source(original_binks[i],
            std::ios::binary|std::ios::ate);
        constexpr std::streamoff kRetailBinkCapacity=7000000;
        if(!source) {
            error="cannot read original Spud Skate Bink: "+
                  original_binks[i].string();
            std::string ignored;(void)close(ignored);
            return false;
        }
        const auto size=source.tellg();
        if(size<=0||size>kRetailBinkCapacity) {
            error="original Spud Skate Bink is empty or exceeds "
                  "retail 7MB memory-buffer limit: "+
                  original_binks[i].string();
            std::string ignored;(void)close(ignored);
            return false;
        }
        auto& bytes=movie_bytes_[i];
        bytes.resize(static_cast<std::size_t>(size));
        source.seekg(0,std::ios::beg);
        if(!source.read(
                reinterpret_cast<char*>(bytes.data()),
                static_cast<std::streamsize>(bytes.size()))) {
            error="failed original Spud Skate movie memory-buffer read";
            std::string ignored;(void)close(ignored);
            return false;
        }
        handles_[i]=open(
            reinterpret_cast<const char*>(bytes.data()),0x04000000u);
        if(!handles_[i]) {
            error="original BinkOpen(memory, 0x04000000) failed: "+
                  original_binks[i].string();
            std::string ignored;(void)close(ignored);
            return false;
        }
        const auto& info=header(handles_[i]);
        // Four full-size Bink movie sources plus a similarly-sized
        // terminal movie. No synthetic clock or source BMP substitutes.
        if(info.width!=600||info.height!=380||
           info.frames<660 && i<4||
           info.frames==0||info.frame_num==0||
           info.frame_num>info.frames) {
            error="original Spud Skate Bink geometry or frame count invalid";
            std::string ignored;(void)close(ignored);
            return false;
        }
        if(!draw_.create_bink_offscreen_surface(
                static_cast<int>(info.width),
                static_cast<int>(info.height),
                surfaces_[i],error)) {
            std::string ignored;(void)close(ignored);
            return false;
        }
    }
    selected_=0;
    in_end_movie_=false;
    final_end_frame_decoded_=false;
    has_decoded_frame_=false;
    last_observed_frame_=0;
    active_=true;
    error.clear();
    return true;
#endif
}

bool Win32RetailSpudSkateMovies::decode_to_surface(
    std::size_t stream,std::string& error) {
#if defined(_WIN64)
    (void)stream;
    error="cannot decode original x86 Bink in a 64-bit process";
    return false;
#else
    if(stream>=5 || !handles_[stream] || !surfaces_[stream]) {
        error="Spud Skate Bink handle and video surface are not allocated";
        return false;
    }
    const auto frame=dll_function<Frame>(exports_,"BinkDoFrame");
    const auto copy=dll_function<Copy>(exports_,"BinkCopyToBuffer");
    const auto format=dll_function<SurfaceFormat>(
        exports_,"BinkDDSurfaceType");
    if(!frame||!copy||!format) {
        error="original Bink video conversion exports unavailable";
        return false;
    }
    (void)frame(handles_[stream]);
    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    const auto lock=surfaces_[stream]->Lock(
        nullptr,&desc,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(lock)) {
        error="original Spud Skate Bink DirectDraw offscreen lock failed";
        return false;
    }
    const auto type=format(surfaces_[stream]);
    const auto result=copy(handles_[stream],desc.lpSurface,
        desc.lPitch,desc.dwHeight,0,0,type);
    const auto unlock=surfaces_[stream]->Unlock(nullptr);
    if(FAILED(unlock) || result<0) {
        error="original Spud Skate Bink decoder could not copy to DD7";
        return false;
    }
    error.clear();
    return true;
#endif
}

bool Win32RetailSpudSkateMovies::observe(
    SpudSkateMovieFrame& result,std::string& error) {
    result={};
#if defined(_WIN64)
    error="x64 does not support the original Bink DLL ABI";
    return false;
#else
    if(!active_) {
        error="Spud Skate original five-stream movies are not open";
        return false;
    }
    const auto next=dll_function<Next>(exports_,"BinkNextFrame");
    const auto wait=dll_function<Frame>(exports_,"BinkWait");
    if(!next||!wait) {
        error="original BinkWait or NextFrame unavailable";
        return false;
    }
    if(in_end_movie_) {
        const auto& stream=header(handles_[4]);
        const auto terminal=
            spud_skate::original_end_bink_decode_decision(
                static_cast<int>(stream.frame_num),
                static_cast<int>(stream.frames),
                final_end_frame_decoded_);
        if(terminal.report_finished) {
            result.end_movie_finished=true;
            error.clear();return true;
        }
        if(!terminal.decode_frame) {
            error="original end.bik frame header is outside decoded range";
            return false;
        }
        // Source 0x424690: actual end.bik decompression and DD7
        // presentation, never an artificial completion countdown.
        for(int attempts=0;wait(handles_[4])!=0;++attempts) {
            if(attempts>=1000) {
                error="original end.bik decoder never became ready";
                return false;
            }
            Sleep(1);
        }
        if(!decode_to_surface(4,error))return false;
        // The last Bink frame is a real source frame too. Decode and
        // copy it EXACTLY ONCE, do not advance beyond the last frame,
        // and only report EOS on the next observed host tick so it
        // can actually be submitted through DirectDraw.
        has_decoded_frame_=true;
        if(terminal.next_frame)next(handles_[4]);
        else final_end_frame_decoded_=true;
        error.clear();
        return true;
    }

    const int frame_number=static_cast<int>(header(handles_[0]).frame_num);
    if(frame_number<1) {
        error="original Spud Skate Bink supplied zero frame number";
        return false;
    }
    for(std::size_t i=1;i<4;++i) {
        if(header(handles_[i]).frame_num!=static_cast<std::uint32_t>(frame_number)) {
            error="original four Spud Skate Bink streams became unsynchronized";
            return false;
        }
    }
    // The source waits for the movie frame rate, not 60 Hz game ticks.
    for(int attempts=0;wait(handles_[selected_])!=0;++attempts) {
        if(attempts>=1000) {
            error="original Spud Skate Bink frame was not ready";
            return false;
        }
        Sleep(1);
    }
    for(std::size_t i=0;i<4;++i)
        if(!decode_to_surface(i,error))return false;
    result.primary_decoded_frame=frame_number;
    result.four_quality_streams_synchronized=true;
    result.quality_stream_reached_end=
        header(handles_[selected_]).frame_num >=
        header(handles_[selected_]).frames;
    for(std::size_t i=0;i<4;++i) {
        // Original call 0x424C25 advances every quality stream
        // exactly once per observed decoded frame.
        if(header(handles_[i]).frame_num <
           header(handles_[i]).frames)
            next(handles_[i]);
    }
    has_decoded_frame_=true;
    last_observed_frame_=static_cast<std::uint32_t>(frame_number);
    error.clear();
    return true;
#endif
}

bool Win32RetailSpudSkateMovies::select_quality(
    spud_skate::StuntQuality quality,std::string& error) {
    const auto value=static_cast<int>(quality);
    if(!active_||in_end_movie_||value<0||value>3) {
        error="Spud Skate cannot select unknown or closed Bink quality";
        return false;
    }
    selected_=static_cast<std::size_t>(value);
    error.clear();return true;
}

bool Win32RetailSpudSkateMovies::seek_to_exact_frame(
    std::size_t stream,int target,std::string& error) {
#if defined(_WIN64)
    (void)stream;(void)target;
    error="original Spud Skate Bink seek requires x86";
    return false;
#else
    if(stream>=4 || !handles_[stream]||target<1||target>660) {
        error="Spud Skate original Bink seek target invalid";
        return false;
    }
    const auto go=dll_function<Goto>(exports_,"BinkGoto");
    const auto next=dll_function<Next>(exports_,"BinkNextFrame");
    if(!go||!next) {
        error="original Bink goto/next exports unavailable";
        return false;
    }
    // Always jump to original frame 1 and decode forward to the
    // exact requested frame. BinkGoto near a keyframe alone does
    // NOT guarantee frame-exact seek for original stunt windows.
    go(handles_[stream],1,0);
    for(int i=1;i<target;++i) {
        if(static_cast<int>(header(handles_[stream]).frame_num)!=i) {
            error="BinkGoto seek did not return expected source frame";
            return false;
        }
        if(!decode_to_surface(stream,error))return false;
        next(handles_[stream]);
    }
    if(static_cast<int>(header(handles_[stream]).frame_num)!=target) {
        error="source Bink exact-frame seek did not reach requested frame";
        return false;
    }
    error.clear();return true;
#endif
}

bool Win32RetailSpudSkateMovies::resynchronize_all(
    int decoded_frame,std::string& error) {
    if(!active_||in_end_movie_) {
        error="cannot seek outside original four-stream run";
        return false;
    }
    for(std::size_t i=0;i<4;++i)
        if(!seek_to_exact_frame(i,decoded_frame,error))return false;
    has_decoded_frame_=false;
    last_observed_frame_=0;
    error.clear();return true;
}

bool Win32RetailSpudSkateMovies::begin_end_movie(
    std::string& error) {
    if(!active_||in_end_movie_||!handles_[4]) {
        error="original end.bik transition without five open movies";
        return false;
    }
    in_end_movie_=true;
    final_end_frame_decoded_=false;
    has_decoded_frame_=false;
    error.clear();return true;
}

bool Win32RetailSpudSkateMovies::blit_selected(
    std::string& error) {
    if(!active_||!has_decoded_frame_) {
        error.clear();return true;
    }
    const auto index=in_end_movie_?4:selected_;
    return draw_.blit_bink_offscreen_surface(
        surfaces_[index],600,380,
        spud_skate::kMovieScreenX,spud_skate::kMovieScreenY,
        error);
}

bool Win32RetailSpudSkateMovies::close(
    std::string& error) {
#if !defined(_WIN64)
    auto close=dll_function<Close>(exports_,"BinkClose");
    for(auto& handle:handles_) {
        if(handle && close)close(handle);
        handle=nullptr;
    }
#endif
    for(auto& surface:surfaces_) {
        if(surface)surface->Release();
        surface=nullptr;
    }
    // Release after BinkClose: the original decoder is permitted to
    // retain pointers into every movie's unmodified byte buffer.
    for(auto& bytes:movie_bytes_) {
        std::vector<std::uint8_t>{}.swap(bytes);
    }
    exports_.unload();
    selected_=0;
    in_end_movie_=false;
    final_end_frame_decoded_=false;
    has_decoded_frame_=false;
    last_observed_frame_=0;
    active_=false;
    error.clear();return true;
}

} // namespace btb::full_game
#endif
