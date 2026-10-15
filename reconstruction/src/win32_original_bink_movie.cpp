#include "btb/win32_original_bink_movie.hpp"
#if defined(_WIN32)
#include <algorithm>
#include <filesystem>
#include <limits>
#include <utility>
namespace btb::full_game {
namespace {
constexpr std::uint32_t kBinkSurface32=3U;
constexpr std::uint32_t kBinkCopyAll=0x80000000U;
constexpr std::uint32_t kMaximumMovieWidth=2048;
constexpr std::uint32_t kMaximumMovieHeight=2048;
}
Win32OriginalBinkMovie::~Win32OriginalBinkMovie() {
 close_global();
}
void Win32OriginalBinkMovie::close_global() noexcept {
 if(current_) {
  if(const auto proc=exports_.find("BinkClose")){
   const auto close=reinterpret_cast<BinkCloseFn>(proc);
   close(current_);
  }
  current_=nullptr;
 }
 bgra_.clear();width_=height_=total_frames_=decoded_frames_=0;
 last_frame_shown_=have_pixels_=false;
}
bool Win32OriginalBinkMovie::ensure_decoder(std::string& error){
 // Original binkw32.dll is a PE32 x86 module, not a Win64 DLL.
 if(sizeof(void*)!=4){
  error="original Bink playback requires the x86 build: 32-bit "
        "binkw32.dll cannot load into Windows x64";
  return false;
 }
 if(exports_.loaded())return true;
 const std::filesystem::path alternatives[]{
  root_/"Exe"/"binkw32.dll",root_/"binkw32.dll"
 };
 std::error_code ec;
 for(const auto& path:alternatives){
  if(std::filesystem::is_regular_file(path,ec)&&!ec){
   auto absolute=std::filesystem::absolute(path,ec);
   if(ec){error="cannot resolve original Bink DLL absolute path";return false;}
   return exports_.load_exact_original(absolute,error);
  }
  ec.clear();
 }
 error="original installed Exe/binkw32.dll missing; "
       "cannot decode video without the actual x86 retail library";
 return false;
}
bool Win32OriginalBinkMovie::open_global(
 const std::filesystem::path& file,std::string& error) {
 close_global();
 if(!ensure_decoder(error))return false;
 if(!sound_system_ready_){
  auto* device=audio_.native_device_for_bink();
  if(!device){
   error="native DirectSound is not initialized for original Bink audio";
   return false;
  }
  const auto callback=exports_.find("BinkOpenDirectSound");
  const auto exported=exports_.find("BinkSetSoundSystem");
  if(!callback||!exported){
   error="original Bink SDK missing DirectSound initialization exports";
   return false;
  }
  using SoundSystemOpenFn=std::int32_t (__stdcall*)(FARPROC,std::uint32_t);
  const auto open_sound=reinterpret_cast<SoundSystemOpenFn>(exported);
  if(open_sound(callback,static_cast<std::uint32_t>(
       reinterpret_cast<std::uintptr_t>(device)))==0){
   error="original BinkSetSoundSystem rejected native DirectSound device";
   return false;
  }
  sound_system_ready_=true;
 }
 if(!std::filesystem::is_regular_file(file)){
  error="original Bink movie missing: "+file.string();return false;
 }
 const auto open=reinterpret_cast<BinkOpenFn>(exports_.find("BinkOpen"));
 if(!open){error="BinkOpen missing from validated DLL";return false;}
 // The 2002 retail API takes an ANSI absolute pathname.
 std::error_code ec;
 const auto abs=std::filesystem::absolute(file,ec);
 if(ec){error="invalid original Bink path";return false;}
 const auto narrow=abs.string();
 current_=open(narrow.c_str(),0);
 if(!current_){error="retail BinkOpen failed for "+narrow;return false;}
 const auto* meta=reinterpret_cast<const BinkPrefix*>(current_);
 if(!meta->width||!meta->height||meta->width>kMaximumMovieWidth||
    meta->height>kMaximumMovieHeight||
    meta->frames==0||meta->frames>100000U ||
    meta->frame_num==0||meta->frame_num>meta->frames){
  close_global();
  error="BinkOpen returned malformed original frame metadata";return false;
 }
 width_=meta->width;height_=meta->height;total_frames_=meta->frames;
 const auto pixels=static_cast<std::size_t>(width_)*height_*4U;
 if(pixels>std::numeric_limits<std::int32_t>::max()){
  close_global();error="retail Bink pitch/buffer overflows 32-bit";return false;
 }
 bgra_.assign(pixels,0);
 error.clear();return true;
}
bool Win32OriginalBinkMovie::service(
 bool& global_finished,std::string& error) {
 global_finished=false;
 if(!current_){error.clear();return true;}
 if(last_frame_shown_){
  // Do not announce completion on the same tick that decodes the last frame.
  // The previous frame was physically submitted in render overlay.
  global_finished=true;close_global();error.clear();return true;
 }
 const auto wait=reinterpret_cast<BinkWaitFn>(exports_.find("BinkWait"));
 const auto decode=reinterpret_cast<BinkDoFrameFn>(exports_.find("BinkDoFrame"));
 const auto copy=reinterpret_cast<BinkCopyFn>(exports_.find("BinkCopyToBuffer"));
 const auto next=reinterpret_cast<BinkNextFn>(exports_.find("BinkNextFrame"));
 if(!wait||!decode||!copy||!next){
  error="original Bink decoding exports incomplete";return false;
 }
 if(wait(current_)!=0){error.clear();return true;}
 // Frame timing, decompression and destination bytes are provided by the
 // original DLL, not by a synthetic still bitmap or timer.
 (void)decode(current_);
 (void)copy(current_,bgra_.data(),static_cast<std::int32_t>(width_*4),
             height_,0,0,kBinkSurface32|kBinkCopyAll);
 ++decoded_frames_;
 have_pixels_=true;
 if(decoded_frames_>=total_frames_) {
  last_frame_shown_=true;
 }else next(current_);
 error.clear();return true;
}
bool Win32OriginalBinkMovie::open_walkthrough(
 const std::filesystem::path&,const std::filesystem::path&,
 std::string& error){
 error="the original instructional Bink inset positioning/audio contract "
       "has not been integrated; refusing an invented full-screen overlay";
 return false;
}
bool Win32OriginalBinkMovie::close_walkthrough(std::string& error){
 error.clear();return true;
}
bool Win32OriginalBinkMovie::render_walkthrough_overlay(std::string& error){
 if(!current_||!have_pixels_){error.clear();return true;}
 // The NativeGameDevice invokes this AFTER game drawing and BEFORE Blt/Flip.
 // It presents global cutscene frames in the real DirectDraw backbuffer.
 return graphics_.blit_decoded_bink32(
   bgra_.data(),width_,height_,0,0,error);
}
} // namespace btb::full_game
#endif
