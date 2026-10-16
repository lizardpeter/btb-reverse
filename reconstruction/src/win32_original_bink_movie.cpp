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
// Original 004281D0 creates a 0x11a by 0xb3 DirectDraw7 movie surface.
// The adjacent 00428120 hover region is x=50..364, y=141..349.
// Center the original-sized surface inside that observed source region;
// its pixel-exact destination offset remains to be verified against retail.
constexpr std::uint32_t kRetailInsetWidth=0x11aU;
constexpr std::uint32_t kRetailInsetHeight=0xb3U;
constexpr int kRetailInsetLeft=66;
constexpr int kRetailInsetTop=155;
}
Win32OriginalBinkMovie::~Win32OriginalBinkMovie() {
 close_walkthrough_handle();
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
void Win32OriginalBinkMovie::close_walkthrough_handle() noexcept {
 if(walkthrough_) {
  if(const auto proc=exports_.find("BinkClose")){
   const auto close=reinterpret_cast<BinkCloseFn>(proc);
   close(walkthrough_);
  }
  walkthrough_=nullptr;
 }
 walkthrough_bgra_.clear();
 walk_width_=walk_height_=walk_total_frames_=walk_decoded_frames_=0;
 walk_have_pixels_=walk_restart_on_next_frame_=false;
}
bool Win32OriginalBinkMovie::ensure_decoder(std::string& error){
 // The original DLL is PE32 and cannot load into a 64-bit process.
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
bool Win32OriginalBinkMovie::open_original_file(
 const std::filesystem::path& file,void*& handle,std::string& error) {
 handle=nullptr;
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
 std::error_code ec;
 const auto abs=std::filesystem::absolute(file,ec);
 if(ec){error="invalid original Bink path";return false;}
 const auto narrow=abs.string();
 handle=open(narrow.c_str(),0);
 if(!handle){error="retail BinkOpen failed for "+narrow;return false;}
 error.clear();return true;
}
bool Win32OriginalBinkMovie::open_global(
 const std::filesystem::path& file,std::string& error) {
 close_global();
 if(!open_original_file(file,current_,error))return false;
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
bool Win32OriginalBinkMovie::skip_global(std::string& error) {
 if(!current_){error="no original global Bink is open";return false;}
 close_global();error.clear();return true;
}
bool Win32OriginalBinkMovie::open_walkthrough(
 const std::filesystem::path& movie,const std::filesystem::path&,
 std::string& error) {
 // Walkthrough is a separate Bink stream from global intro mode 13/14.
 // Retail 004281D0 opens the Bink and 00428120 separately triggers the
 // spoken help voice on hover. The optional WAV is not a decoder input.
 close_walkthrough_handle();
 if(!open_original_file(movie,walkthrough_,error))return false;
 const auto* meta=reinterpret_cast<const BinkPrefix*>(walkthrough_);
 if(!meta->width||!meta->height||
    meta->width>kRetailInsetWidth||meta->height>kRetailInsetHeight||
    !meta->frames||meta->frames>100000U||
    !meta->frame_num||meta->frame_num>meta->frames){
  close_walkthrough_handle();
  error="retail instructional Bink does not fit source 282x179 surface";
  return false;
 }
 walk_width_=meta->width;
 walk_height_=meta->height;
 walk_total_frames_=meta->frames;
 const auto bytes=static_cast<std::size_t>(walk_width_)*walk_height_*4U;
 walkthrough_bgra_.assign(bytes,0);
 error.clear();return true;
}
bool Win32OriginalBinkMovie::close_walkthrough(std::string& error) {
 close_walkthrough_handle();
 error.clear();return true;
}
bool Win32OriginalBinkMovie::service(
 bool& global_finished,std::string& error) {
 global_finished=false;
 if(current_){
  if(last_frame_shown_){
   // Last global frame was actually submitted on the previous present.
   global_finished=true;
   close_global();
  }else{
   const auto wait=reinterpret_cast<BinkWaitFn>(exports_.find("BinkWait"));
   const auto decode=reinterpret_cast<BinkDoFrameFn>(exports_.find("BinkDoFrame"));
   const auto copy=reinterpret_cast<BinkCopyFn>(exports_.find("BinkCopyToBuffer"));
   const auto next=reinterpret_cast<BinkNextFn>(exports_.find("BinkNextFrame"));
   if(!wait||!decode||!copy||!next){
    error="original Bink global frame exports incomplete";return false;
   }
   if(wait(current_)==0){
    (void)decode(current_);
    (void)copy(current_,bgra_.data(),static_cast<std::int32_t>(width_*4),
               height_,0,0,kBinkSurface32|kBinkCopyAll);
    ++decoded_frames_;
    have_pixels_=true;
    if(decoded_frames_>=total_frames_) last_frame_shown_=true;
    else next(current_);
   }
  }
 }
 if(walkthrough_ && !current_){
  // Native 00428120 loops the genuine walkthrough using 00408D80 BinkGoto
  // at frame completion. Never fake a completed global intro from its loop.
  const auto wait=reinterpret_cast<BinkWaitFn>(exports_.find("BinkWait"));
  const auto decode=reinterpret_cast<BinkDoFrameFn>(exports_.find("BinkDoFrame"));
  const auto copy=reinterpret_cast<BinkCopyFn>(exports_.find("BinkCopyToBuffer"));
  const auto next=reinterpret_cast<BinkNextFn>(exports_.find("BinkNextFrame"));
  const auto seek=reinterpret_cast<std::int32_t (__stdcall*)(void*,std::uint32_t,std::uint32_t)>(
    exports_.find("BinkGoto"));
  if(!wait||!decode||!copy||!next||!seek){
   error="original Bink walkthrough frame/loop exports incomplete";return false;
  }
  if(walk_restart_on_next_frame_){
   (void)seek(walkthrough_,1U,0U);
   walk_decoded_frames_=0;
   walk_restart_on_next_frame_=false;
  }
  if(wait(walkthrough_)==0){
   (void)decode(walkthrough_);
   (void)copy(walkthrough_,walkthrough_bgra_.data(),
        static_cast<std::int32_t>(walk_width_*4),
        walk_height_,0,0,kBinkSurface32|kBinkCopyAll);
   ++walk_decoded_frames_;
   walk_have_pixels_=true;
   if(walk_decoded_frames_>=walk_total_frames_)
    walk_restart_on_next_frame_=true;
   else next(walkthrough_);
  }
 }
 error.clear();return true;
}
bool Win32OriginalBinkMovie::render_walkthrough_overlay(std::string& error){
 // A global intro is exclusive during intercepted Bink modes 13/14.
 if(current_) {
  if(!have_pixels_){error.clear();return true;}
  return graphics_.blit_decoded_bink32(
    bgra_.data(),width_,height_,0,0,error);
 }
 if(walkthrough_ && walk_have_pixels_){
  // The original offscreen surface is 282x179. These source-derived
  // centered coordinates need final retail screenshot comparison.
  return graphics_.blit_decoded_bink32(
    walkthrough_bgra_.data(),walk_width_,walk_height_,
    kRetailInsetLeft,kRetailInsetTop,error);
 }
 error.clear();return true;
}
} // namespace btb::full_game
#endif
