#include "btb/win32_native_spud_skate_movies.hpp"
#if defined(_WIN32)
#include "btb/bink32_container.hpp"
#include "btb/spud_skate_data.hpp"
#include "btb/win32_native_bink_surface.hpp"
#include <fstream>
#include <thread>
#include <utility>
#include <vector>
namespace btb::full_game {
Win32NativeSpudSkateMovies::Win32NativeSpudSkateMovies(Win32OriginalDirectDraw& d)
 :draw_(d),bank_([this](std::uint64_t time,std::string& error){return pace(time,error);},
 [this](std::size_t i,const NativeSpudSkateMovieBank::Frame& frame,std::string& error){
  if(i>=surfaces_.size()||!surfaces_[i]){error="native Skate movie surface unavailable";return false;}
  return surfaces_[i].write(frame,error);
 }){}
Win32NativeSpudSkateMovies::~Win32NativeSpudSkateMovies(){
 std::string ignored;(void)close(ignored);
}
bool Win32NativeSpudSkateMovies::open(
 const std::array<std::filesystem::path,5>& paths,std::string& error){
 if(active_){error="native Spud Skate bank already open";return false;}
 if(!bank_.open(paths,error))return false;
 for(auto& surface:surfaces_)if(!surface.allocate(draw_,600,380,error)){
  std::string ignored;(void)close(ignored);return false;
 }
 active_=true;clock_started_=false;error.clear();return true;
}
bool Win32NativeSpudSkateMovies::pace(std::uint64_t due,std::string& error){
 using Clock=std::chrono::steady_clock;
 const auto now=Clock::now();
 if(!clock_started_){origin_=now-std::chrono::microseconds(due);clock_started_=true;}
 const auto target=origin_+std::chrono::microseconds(due);
 if(target>now){
  if(target-now>std::chrono::seconds(1)){error="native Spud Skate source frame clock exceeded one second";return false;}
  std::this_thread::sleep_until(target);
 }
 error.clear();return true;
}
bool Win32NativeSpudSkateMovies::observe(SpudSkateMovieFrame& result,std::string& error){return bank_.observe(result,error);}
bool Win32NativeSpudSkateMovies::select_quality(spud_skate::StuntQuality q,std::string& error){return bank_.select_quality(q,error);}
bool Win32NativeSpudSkateMovies::resynchronize_all(int frame,std::string& error){
 clock_started_=false;return bank_.resynchronize_all(frame,error);
}
bool Win32NativeSpudSkateMovies::begin_end_movie(std::string& error){
 clock_started_=false;return bank_.begin_end_movie(error);
}
bool Win32NativeSpudSkateMovies::blit_selected(std::string& error){
 const auto i=bank_.displayed_stream();
 if(!active_||!i){error.clear();return true;}
 if(*i>=surfaces_.size()){error="native Skate displayed stream out of range";return false;}
 return surfaces_[*i].blit(draw_,spud_skate::kMovieScreenX,spud_skate::kMovieScreenY,error);
}
bool Win32NativeSpudSkateMovies::close(std::string& error){
 for(auto& surface:surfaces_)surface.reset();
 active_=clock_started_=false;return bank_.close(error);
}
SpudSkateMovieSource* Win32HybridSpudSkateMovies::source() noexcept {
 if(!active_)return nullptr;
 return using_native_?static_cast<SpudSkateMovieSource*>(&native_)
                     :static_cast<SpudSkateMovieSource*>(&retail_);
}
bool Win32HybridSpudSkateMovies::open(
 const std::array<std::filesystem::path,5>& files,std::string& error){
 if(active_){error="Spud Skate movie backend already open";return false;}
 std::string native_error;
 if(enabled_&&native_.open(files,native_error)){
  using_native_=active_=true;error.clear();return true;
 }
 std::string ignored;(void)native_.close(ignored);
 if constexpr(sizeof(void*)==8){
  error="native-only x64 Spud Skate Bink: "+native_error;return false;
 }
 if(retail_.open(files,error)){
  using_native_=false;active_=true;return true;
 }
 if(enabled_&&!native_error.empty())
  error="native: "+native_error+"; retail: "+error;
 return false;
}
bool Win32HybridSpudSkateMovies::observe(
 SpudSkateMovieFrame& frame,std::string& error){
 auto* s=source();
 if(!s){error="Spud Skate movie backend was not opened";return false;}
 return s->observe(frame,error);
}
bool Win32HybridSpudSkateMovies::select_quality(
 spud_skate::StuntQuality q,std::string& error){
 auto* s=source();return s?s->select_quality(q,error):
  (error="Spud Skate movie backend was not opened",false);
}
bool Win32HybridSpudSkateMovies::resynchronize_all(
 int frame,std::string& error){
 auto* s=source();return s?s->resynchronize_all(frame,error):
  (error="Spud Skate movie backend was not opened",false);
}
bool Win32HybridSpudSkateMovies::begin_end_movie(std::string& error){
 auto* s=source();return s?s->begin_end_movie(error):
  (error="Spud Skate movie backend was not opened",false);
}
bool Win32HybridSpudSkateMovies::blit_selected(std::string& error){
 if(!active_){error.clear();return true;}
 return using_native_?native_.blit_selected(error):retail_.blit_selected(error);
}
bool Win32HybridSpudSkateMovies::close(std::string& error){
 std::string n,r;
 const bool a=native_.close(n),b=retail_.close(r);
 active_=using_native_=false;
 if(!a||!b){error="native: "+n+"; retail: "+r;return false;}
 error.clear();return true;
}
} // namespace btb::full_game
#endif
