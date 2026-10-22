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
