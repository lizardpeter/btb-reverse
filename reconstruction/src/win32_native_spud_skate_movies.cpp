#include "btb/win32_native_spud_skate_movies.hpp"
#if defined(_WIN32)
#include "btb/bink32_container.hpp"
#include "btb/win32_native_bink_surface.hpp"
#include <fstream>
#include <thread>
#include <utility>
#include <vector>
namespace btb::full_game {
using btb::bink32_recovered::parse_container_index;
Win32NativeSpudSkateMovies::~Win32NativeSpudSkateMovies(){
 std::string ignored;(void)close(ignored);
}
bool Win32NativeSpudSkateMovies::open(
 const std::array<std::filesystem::path,5>& paths,std::string& error){
 if(active_){error="native Spud Skate bank already open";return false;}
 static constexpr std::array<const char*,5> names={
  "bad.bik","normal.bik","ok.bik","good.bik","end.bik"};
 for(std::size_t i=0;i<5;++i){
  std::ifstream f(paths[i],std::ios::binary|std::ios::ate);
  const auto n=f?f.tellg():std::streampos(-1);
  if(paths[i].filename().string()!=names[i]||n<=0||n>7000000){
   error="missing or oversized original Spud Skate Bink: "+paths[i].string();
   std::string ignored;(void)close(ignored);return false;
  }
  std::vector<std::uint8_t> bytes(static_cast<std::size_t>(n));
  f.seekg(0);
  if(!f.read(reinterpret_cast<char*>(bytes.data()),
             static_cast<std::streamsize>(bytes.size()))){
   error="cannot read Spud Skate Bink bytes";
   std::string ignored;(void)close(ignored);return false;
  }
  const auto index=parse_container_index(bytes);
  if(!index||index->header.magic!=btb::bink32_recovered::Magic::SJPi||
     index->header.width!=600||index->header.height!=380||
     index->header.frame_count==0||
     (i<4&&index->header.frame_count<660)||
     !index->tracks.empty()){
   error="native Spud Skate requires valid silent 600x380 SJPi streams";
   std::string ignored;(void)close(ignored);return false;
  }
  streams_[i]=btb::bink32_recovered::BinkNativeVideoStream::open(
     std::move(bytes));
  if(!streams_[i]||!draw_.create_bink_offscreen_surface(
      600,380,surfaces_[i],error)){
   if(error.empty())error="cannot open native Spud Skate Bink stream";
   std::string ignored;(void)close(ignored);return false;
  }
 }
 active_=true;selected_=0;clock_started_=false;
 end_movie_=end_decoded_=has_image_=false;
 error.clear();return true;
}
bool Win32NativeSpudSkateMovies::pace(
 std::size_t stream,std::uint32_t frame,std::string& error){
 const auto due=streams_[stream]->frame_timestamp_us(frame);
 if(!due){error="native Spud Skate source frame time is invalid";return false;}
 using Clock=std::chrono::steady_clock;
 const auto now=Clock::now();
 if(!clock_started_){
  origin_=now-std::chrono::microseconds(*due);
  clock_started_=true;
 }
 const auto target=origin_+std::chrono::microseconds(*due);
 if(target>now){
  const auto remaining=target-now;
  if(remaining>std::chrono::seconds(1)){
   error="native Spud Skate source frame clock exceeded one second";
   return false;
  }
  std::this_thread::sleep_until(target);
 }
 error.clear();return true;
}
bool Win32NativeSpudSkateMovies::decode_to_surface(
 std::size_t i,std::string& error){
 if(i>=streams_.size()||!streams_[i]||!surfaces_[i]||
    !streams_[i]->decode_next()){
  error="native Spud Skate source video frame decode failed";return false;
 }
 const auto* frame=streams_[i]->current_frame();
 if(!frame||!copy_native_bink_frame_to_directdraw(surfaces_[i],*frame,error)){
  if(error.empty())error="native Spud Skate DD7 pixel upload failed";
  return false;
 }
 error.clear();return true;
}
bool Win32NativeSpudSkateMovies::observe(
 SpudSkateMovieFrame& result,std::string& error){
 result={};
 if(!active_){error="native Spud Skate streams are not open";return false;}
 if(end_movie_){
  if(end_decoded_){result.end_movie_finished=true;error.clear();return true;}
  auto& s=*streams_[4];
  if(s.at_end()||!pace(4,s.frame_number()+1,error)||
     !decode_to_surface(4,error))return false;
  has_image_=true;
  if(s.at_end())end_decoded_=true;
  error.clear();return true;
 }
 const auto frame=streams_[0]->frame_number()+1;
 for(std::size_t i=0;i<4;++i){
  if(streams_[i]->at_end()||
     streams_[i]->frame_number()+1!=frame){
   error="native Spud Skate four video streams desynchronized";
   return false;
  }
 }
 if(!pace(selected_,frame,error))return false;
 for(std::size_t i=0;i<4;++i)
  if(!decode_to_surface(i,error))return false;
 result.primary_decoded_frame=static_cast<int>(frame);
 result.four_quality_streams_synchronized=true;
 result.quality_stream_reached_end=streams_[selected_]->at_end();
 has_image_=true;error.clear();return true;
}
