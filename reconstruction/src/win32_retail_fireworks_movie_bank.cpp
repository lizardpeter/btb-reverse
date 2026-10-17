#include "btb/win32_retail_fireworks_movie_bank.hpp"

#if defined(_WIN32)
#include <array>
#include <cstdint>
#include <fstream>
#include <limits>
#include <string>
#include <system_error>

namespace btb::full_game {
namespace {
struct OriginalBinkHeader {
    std::uint32_t width{},height{},frames{},frame_num{};
};
static_assert(sizeof(OriginalBinkHeader)==16);
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
using SetAudio=std::int32_t (WINAPI*)(FARPROC,std::uint32_t);
template<class Fn>
[[nodiscard]] Fn export_fn(Win32OriginalBinkExports& dll,
                           const char* name) noexcept {
    return reinterpret_cast<Fn>(dll.find(name));
}
[[nodiscard]] const OriginalBinkHeader& header(void* handle) noexcept {
    return *static_cast<const OriginalBinkHeader*>(handle);
}
#endif
}

Win32RetailFireworksMovieBank::~Win32RetailFireworksMovieBank() {
    std::string ignored;
    (void)close(ignored);
}

bool Win32RetailFireworksMovieBank::reject_and_close(
    std::string reason,std::string& error) {
    std::string ignored;
    (void)close(ignored);
    error=std::move(reason);
    return false;
}

bool Win32RetailFireworksMovieBank::load_original_bink_dll(
    std::string& error) {
#if defined(_WIN64)
    error="original Fireworks Bink bank requires the real 32-bit "
          "binkw32.dll; an x64 codec is not installed";
    return false;
#else
    if(dll_.loaded()) {error.clear();return true;}
    const std::array<std::filesystem::path,2> candidates{{
        root_/"Exe"/"binkw32.dll",root_/"binkw32.dll"
    }};
    std::error_code ec;
    for(const auto& dll_path:candidates) {
        if(!std::filesystem::is_regular_file(dll_path,ec)||ec) {
            ec.clear();continue;
        }
        if(!dll_.load_exact_original(
               std::filesystem::absolute(dll_path),error))
            return false;
        const auto sound=export_fn<SetAudio>(
            dll_,"BinkSetSoundSystem");
        const auto direct=dll_.find("BinkOpenDirectSound");
        if(!sound||!direct)
            return reject_and_close(
                "original Fireworks Bink sound exports are missing",error);
        (void)sound(direct,0);
        error.clear();return true;
    }
    error="original x86 binkw32.dll not found under Exe or game root";
    return false;
#endif
}

bool Win32RetailFireworksMovieBank::open(
    const std::filesystem::path& source_subgame,std::string& error) {
#if defined(_WIN64)
    (void)source_subgame;
    return load_original_bink_dll(error);
#else
    if(ready_) {
        error="original Fireworks 23-Bink bank is already open";
        return false;
    }
    if(!load_original_bink_dll(error))return false;
    const auto open_bink=export_fn<Open>(dll_,"BinkOpen");
    if(!open_bink)
        return reject_and_close("missing retail BinkOpen export",error);

    const auto paths=fireworks::original_firework_movie_paths(source_subgame);
    for(std::size_t i=0;i<paths.size();++i) {
        std::error_code ec;
        if(!std::filesystem::is_regular_file(paths[i],ec)||ec)
            return reject_and_close(
                "original Fireworks bank Bink missing: "+paths[i].string(),
                error);
        std::ifstream source(paths[i],std::ios::binary|std::ios::ate);
        if(!source)
            return reject_and_close(
                "cannot open original Fireworks Bink: "+paths[i].string(),
                error);
        const auto count=source.tellg();
        // Retail 0x004115C0 performs fread into one 1,500,000-byte
        // scratch allocation, then copies the bytes it actually read.
        // Reject oversized files instead of silently truncating a user's
        // original Bink and misreporting a valid decoded movie.
        constexpr std::streamoff kRetailReadCapacity=
            static_cast<std::streamoff>(
                fireworks::kOriginalFireworkReadCapacity);
        if(count<=0||count>kRetailReadCapacity)
            return reject_and_close(
                "original Fireworks Bink is empty or exceeds the retail "
                "1,500,000-byte staging limit: "+paths[i].string(),error);
        auto& bytes=bytes_[i];
        bytes.resize(static_cast<std::size_t>(count));
        source.seekg(0,std::ios::beg);
        if(!source.read(
                reinterpret_cast<char*>(bytes.data()),
                static_cast<std::streamsize>(bytes.size())))
            return reject_and_close(
                "original Fireworks Bink could not be read: "+
                paths[i].string(),error);

        // Original LoadFireworkMovieBank opens retained memory through the
        // Bink1 0x04000000 memory-input flag. Keep each bytes_ alive until
        // that exact decoder handle has BinkClosed.
        handles_[i]=open_bink(
            reinterpret_cast<const char*>(bytes.data()),0x04000000u);
        if(!handles_[i])
            return reject_and_close(
                "original Fireworks BinkOpen(memory) failed: "+
                paths[i].string(),error);
        const auto& info=header(handles_[i]);
        const auto surface=fireworks::original_firework_surface_size(i);
        if(info.width==0||info.height==0||
           info.width>static_cast<std::uint32_t>(surface.width)||
           info.height>static_cast<std::uint32_t>(surface.height)||
           info.frames==0||info.frame_num!=1)
            return reject_and_close(
                "original Fireworks Bink exceeds original DD7 surface "
                "geometry or has invalid frame header: "+paths[i].string(),
                error);
        width_[i]=static_cast<std::uint32_t>(surface.width);
        height_[i]=static_cast<std::uint32_t>(surface.height);
        if(!draw_.create_bink_offscreen_surface(
               surface.width,surface.height,surfaces_[i],error)) {
            const auto cause=error;
            return reject_and_close(cause,error);
        }
    }
    ready_=true;
    error.clear();
    return true;
#endif
}

bool Win32RetailFireworksMovieBank::decode_frame(
    std::size_t index,bool& finished,std::string& error) {
    finished=false;
#if defined(_WIN64)
    (void)index;
    error="cannot decode original x86 Fireworks Bink in x64 process";
    return false;
#else
    if(!ready_||index>=handles_.size()||
       !handles_[index]||!surfaces_[index]) {
        error="original Fireworks movie index not open";
        return false;
    }
    if(final_frame_copied_[index]) {
        finished=true;
        error.clear();return true;
    }
    const auto wait=export_fn<Frame>(dll_,"BinkWait");
    const auto decode=export_fn<Frame>(dll_,"BinkDoFrame");
    const auto copy=export_fn<Copy>(dll_,"BinkCopyToBuffer");
    const auto format=export_fn<SurfaceFormat>(dll_,"BinkDDSurfaceType");
    const auto next=export_fn<Next>(dll_,"BinkNextFrame");
    if(!wait||!decode||!copy||!format||!next) {
        error="original Fireworks Bink frame decoder exports absent";
        return false;
    }
    const auto info=header(handles_[index]);
    if(info.frame_num==0||info.frame_num>info.frames) {
        error="original Fireworks HBINK frame outside source range";
        return false;
    }
    if(wait(handles_[index])!=0) {
        error.clear();return true;
    }
    (void)decode(handles_[index]);
    DDSURFACEDESC2 desc{};
    desc.dwSize=sizeof(desc);
    const auto hr=surfaces_[index]->Lock(
        nullptr,&desc,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr);
    if(FAILED(hr)) {
        error="original Fireworks offscreen Bink Lock failed";
        return false;
    }
    const auto type=format(surfaces_[index]);
    const auto copied=copy(handles_[index],desc.lpSurface,
        desc.lPitch,desc.dwHeight,0,0,type);
    const auto unlocked=surfaces_[index]->Unlock(nullptr);
    if(FAILED(unlocked)||copied<0) {
        error="original Fireworks Bink frame copy/unlock failed";
        return false;
    }
    has_frame_[index]=true;
    if(info.frame_num==info.frames)
        final_frame_copied_[index]=true;
    else
        next(handles_[index]);
    error.clear();
    return true;
#endif
}

bool Win32RetailFireworksMovieBank::restart(
    std::size_t index,std::string& error) {
#if defined(_WIN64)
    (void)index;
    error="cannot restart original Fireworks Bink in x64";
    return false;
#else
    if(!ready_||index>=handles_.size()||!handles_[index]) {
        error="original Fireworks rewind index is not open";
        return false;
    }
    const auto go=export_fn<Goto>(dll_,"BinkGoto");
    if(!go) {
        error="original BinkGoto unavailable";
        return false;
    }
    go(handles_[index],1,0);
    const auto& info=header(handles_[index]);
    if(info.frame_num!=1) {
        error="original Fireworks BinkGoto did not seek to frame 1";
        return false;
    }
    has_frame_[index]=false;
    final_frame_copied_[index]=false;
    error.clear();return true;
#endif
}

bool Win32RetailFireworksMovieBank::blit_frame(
    std::size_t index,int x,int y,std::string& error) {
    if(!ready_||index>=surfaces_.size()||
       !surfaces_[index]||!has_frame_[index]) {
        error="original Fireworks movie has no decoded frame to draw";
        return false;
    }
    return draw_.blit_bink_offscreen_surface(
        surfaces_[index],static_cast<int>(width_[index]),
        static_cast<int>(height_[index]),x,y,error);
}

bool Win32RetailFireworksMovieBank::decode_and_queue(
    std::size_t index,int x,int y,bool& finished,
    std::string& error) {

    if(!decode_frame(index,finished,error))return false;
    // Queue also on the decoder's following-physical-tick EOS, because
    // the original final image must be visible BEFORE BinkGoto rewinds.
    if(has_frame(index))
        queued_show_blits_.push_back({index,x,y});
    error.clear();return true;
}

bool Win32RetailFireworksMovieBank::blit_queued_show(
    std::string& error) {
    if(queued_show_blits_.empty()) {
        error.clear();return true;
    }
    if(!ready_) {
        error="Fireworks show DD7 blit queued after Bink bank was closed";
        return false;
    }
    for(const auto& cue:queued_show_blits_) {
        if(cue.index>=surfaces_.size()||!surfaces_[cue.index]) {
            error="original Fireworks queued a nonexistent DD7 surface";
            return false;
        }
        // The pure source show coordinator queues in ORIGINAL overlay order:
        // crowd, existing authored effect records, and topmiddle. Even when
        // BinkGoto has since rewound the decoder, its last copied DD7 pixels
        // remain valid for this single present.
        if(!draw_.blit_bink_offscreen_surface(
               surfaces_[cue.index],
               static_cast<int>(width_[cue.index]),
               static_cast<int>(height_[cue.index]),
               cue.x,cue.y,error))return false;
    }
    error.clear();return true;
}

bool Win32RetailFireworksMovieBank::close(std::string& error) {
#if !defined(_WIN64)
    const auto close_bink=export_fn<Close>(dll_,"BinkClose");
    for(auto& handle:handles_) {
        if(handle && close_bink)close_bink(handle);
        handle=nullptr;
    }
#endif
    for(auto& surface:surfaces_) {
        if(surface)surface->Release();
        surface=nullptr;
    }
    // Decoder handles must close BEFORE freeing their source memory.
    for(auto& bytes:bytes_)
        std::vector<std::uint8_t>{}.swap(bytes);
    queued_show_blits_.clear();
    has_frame_.fill(false);
    final_frame_copied_.fill(false);
    width_.fill(0);
    height_.fill(0);
    ready_=false;
    dll_.unload();
    error.clear();return true;
}
}
#endif
