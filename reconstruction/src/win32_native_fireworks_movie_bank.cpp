#include "btb/win32_native_fireworks_movie_bank.hpp"
#if defined(_WIN32)
#include "btb/bink32_container.hpp"
#include "btb/fireworks_movie_bank_manifest.hpp"
#include "btb/win32_native_bink_surface.hpp"
#include <fstream>
#include <utility>
namespace btb::full_game {
Win32NativeFireworksMovieBank::~Win32NativeFireworksMovieBank(){
 std::string ignored;(void)close(ignored);
}
bool Win32NativeFireworksMovieBank::open(
 const std::filesystem::path& folder,std::string& error){
 if(ready_){error="native Fireworks movie bank already open";return false;}
 const auto paths=fireworks::original_firework_movie_paths(folder);
 auto fail=[this,&error](std::string reason){
  std::string ignored;(void)close(ignored);
  error=std::move(reason);return false;
 };
 for(std::size_t i=0;i<paths.size();++i){
  std::ifstream f(paths[i],std::ios::binary|std::ios::ate);
  const auto n=f?f.tellg():std::streampos(-1);
  if(n<=0||n>static_cast<std::streamoff>(
       fireworks::kOriginalFireworkReadCapacity))
   return fail("native Fireworks movie missing or above retail limit: "+
               paths[i].string());
  std::vector<std::uint8_t> bytes(static_cast<std::size_t>(n));
  f.seekg(0);
  if(!f.read(reinterpret_cast<char*>(bytes.data()),
             static_cast<std::streamsize>(bytes.size())))
   return fail("native Fireworks movie read failed: "+paths[i].string());
  const auto index=btb::bink32_recovered::parse_container_index(bytes);
  const auto size=fireworks::original_firework_surface_size(i);
  if(!index||index->header.magic!=btb::bink32_recovered::Magic::SJPi||
     index->header.width==0||index->header.height==0||
     index->header.width>static_cast<std::uint32_t>(size.width)||
     index->header.height>static_cast<std::uint32_t>(size.height)||
     index->header.frame_count==0||!index->tracks.empty())
   return fail("native Fireworks requires a valid silent SJPi clip: "+
               paths[i].string());
  streams_[i]=btb::bink32_recovered::BinkNativeVideoStream::open(
      std::move(bytes));
  if(!streams_[i])
   return fail("native Fireworks compressed stream open failed");
  width_[i]=static_cast<std::uint32_t>(size.width);
  height_[i]=static_cast<std::uint32_t>(size.height);
  if(!draw_.create_bink_offscreen_surface(
      size.width,size.height,surfaces_[i],error)){
   const auto reason=error;return fail(reason);
  }
 }
 ready_=true;error.clear();return true;
}
bool Win32NativeFireworksMovieBank::decode_frame(
 std::size_t i,bool& finished,std::string& error){
 finished=false;
 if(!ready_||i>=streams_.size()||!streams_[i]||!surfaces_[i]){
  error="native Fireworks Bink index not open";return false;
 }
 if(final_copied_[i]){finished=true;error.clear();return true;}
 auto& stream=*streams_[i];
 const auto frame=stream.frame_number()+1;
 const auto due=stream.frame_timestamp_us(frame);
 if(!due){error="native Fireworks frame index/timing invalid";return false;}
 const auto now=std::chrono::steady_clock::now();
 if(!clock_started_[i]){
  origins_[i]=now;clock_started_[i]=true;
 }
 if(now-origins_[i]<std::chrono::microseconds(*due)){
  error.clear();return true;
 }
 if(!stream.decode_next()){
  error="native Fireworks source frame failed to decode";return false;
 }
 const auto* pixels=stream.current_frame();
 if(!pixels||!copy_native_bink_frame_to_directdraw(
       surfaces_[i],*pixels,error)){
  if(error.empty())error="native Fireworks DirectDraw upload failed";
  return false;
 }
 has_frame_[i]=true;
 if(stream.at_end())final_copied_[i]=true;
 error.clear();return true;
}
bool Win32NativeFireworksMovieBank::restart(
 std::size_t i,std::string& error){
 if(!ready_||i>=streams_.size()||!streams_[i]){
  error="native Fireworks restart index invalid";return false;
 }
 streams_[i]->rewind();
 final_copied_[i]=has_frame_[i]=clock_started_[i]=false;
 error.clear();return true;
}
void Win32NativeFireworksMovieBank::release_cues() noexcept {
 for(auto& cue:cues_){
  if(cue.frozen)cue.frozen->Release();
  cue.frozen=nullptr;
 }
 cues_.clear();
}
void Win32NativeFireworksMovieBank::begin_show_frame() noexcept {
 release_cues();
}
bool Win32NativeFireworksMovieBank::freeze_earlier(
 std::size_t i,std::string& error){
 if(i>=surfaces_.size()||!surfaces_[i]){
  error="native Fireworks DD7 surface is missing";return false;
 }
 for(auto& cue:cues_){
  if(cue.index!=i||cue.frozen)continue;
  IDirectDrawSurface7* snapshot=nullptr;
  if(!draw_.create_bink_offscreen_surface(
       static_cast<int>(width_[i]),static_cast<int>(height_[i]),
       snapshot,error))return false;
  RECT rect{0,0,static_cast<LONG>(width_[i]),
                  static_cast<LONG>(height_[i])};
  const auto hr=snapshot->BltFast(0,0,surfaces_[i],
                                  &rect,DDBLTFAST_WAIT);
  if(FAILED(hr)){
   snapshot->Release();
   error="native Fireworks DD7 duplicate overlay snapshot failed";
   return false;
  }
  cue.frozen=snapshot;
 }
 error.clear();return true;
}
bool Win32NativeFireworksMovieBank::decode_and_queue(
 std::size_t i,int x,int y,bool& finished,std::string& error){
 // The authored source show may use one clip multiple times per tick.
 // Freeze earlier pixels before overwriting its only source surface.
 if(!freeze_earlier(i,error))return false;
 if(!decode_frame(i,finished,error))return false;
 if(has_frame_[i])cues_.push_back({i,x,y,nullptr});
 error.clear();return true;
}
bool Win32NativeFireworksMovieBank::blit_queued_show(
 std::string& error){
 if(!ready_&&!cues_.empty()){
  error="native Fireworks show queued after bank closed";return false;
 }
 for(const auto& cue:cues_){
  if(cue.index>=surfaces_.size()||!surfaces_[cue.index]){
   error="native Fireworks overlay source missing";return false;
  }
  if(!draw_.blit_bink_offscreen_surface(
       cue.frozen?cue.frozen:surfaces_[cue.index],
       static_cast<int>(width_[cue.index]),
       static_cast<int>(height_[cue.index]),
       cue.x,cue.y,error))return false;
 }
 error.clear();return true;
}
bool Win32NativeFireworksMovieBank::close(std::string& error){
 release_cues();
 for(auto& surface:surfaces_){
  if(surface)surface->Release();
  surface=nullptr;
 }
 for(auto& stream:streams_)stream.reset();
 width_.fill(0);height_.fill(0);has_frame_.fill(false);
 final_copied_.fill(false);clock_started_.fill(false);
 ready_=false;error.clear();return true;
}
bool Win32HybridFireworksMovieBank::open(
 const std::filesystem::path& folder,std::string& error){
 if(active_){error="Fireworks Bink bank already open";return false;}
 std::string native_error;
 if(enabled_&&native_.open(folder,native_error)){
  using_native_=active_=true;error.clear();return true;
 }
 std::string ignored;(void)native_.close(ignored);
 if(retail_.open(folder,error)){
  active_=true;using_native_=false;return true;
 }
 if(enabled_&&!native_error.empty())
  error="native: "+native_error+"; retail: "+error;
 return false;
}
bool Win32HybridFireworksMovieBank::restart(
 std::size_t i,std::string& error){
 if(!active_){error="Fireworks Bink bank not open";return false;}
 return using_native_?native_.restart(i,error):retail_.restart(i,error);
}
bool Win32HybridFireworksMovieBank::close(std::string& error){
 std::string n,r;
 const bool a=native_.close(n),b=retail_.close(r);
 active_=using_native_=false;
 if(!a||!b){error="native: "+n+"; retail: "+r;return false;}
 error.clear();return true;
}
void Win32HybridFireworksMovieBank::begin_show_frame() noexcept {
 if(!active_)return;
 if(using_native_)native_.begin_show_frame();
 else retail_.begin_show_frame();
}
bool Win32HybridFireworksMovieBank::decode_and_queue(
 std::size_t i,int x,int y,bool& finished,std::string& error){
 if(!active_){error="Fireworks Bink bank not open";return false;}
 return using_native_?native_.decode_and_queue(i,x,y,finished,error):
                      retail_.decode_and_queue(i,x,y,finished,error);
}
bool Win32HybridFireworksMovieBank::blit_queued_show(
 std::string& error){
 if(!active_){error.clear();return true;}
 return using_native_?native_.blit_queued_show(error):
                      retail_.blit_queued_show(error);
}
bool Win32HybridFireworksMovieBank::ready() const noexcept {
 return active_&&(using_native_?native_.ready():retail_.ready());
}
} // namespace btb::full_game
#endif
