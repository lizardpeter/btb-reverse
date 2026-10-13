#include "btb/original_media15.hpp"
#include <string>
namespace btb::original_media15 {
void apply_global_bink_volume(
 void* movie,bool paused,I32 volume,SetMovieVolume invoke) noexcept {
 if(movie && !paused && invoke)
   invoke(movie,0,btb::bink::bink_volume_from_game_volume(volume));
}
void* open_bink_movie_with_fallback(
 std::string_view filename,std::string_view cd_root,OpenBink open,
 void* global_movie,bool paused,I32 game_volume,SetMovieVolume set_volume) noexcept {
 if(!open)return nullptr;
 void* result=open(filename,0x04000000U);
 if(!result&&!cd_root.empty()){
  const std::string path=std::string(cd_root)+std::string(filename);
  result=open(path,0x04000000U);
 }
 // Original 00408E10 always calls 00409210 AFTER both BinkOpen attempts.
 original_media15::apply_global_bink_volume(
     global_movie,paused,game_volume,set_volume);
 return result;
}
bool open_global_bink_movie(
 GlobalBink& s,std::string_view filename,std::string_view cd_root,
 OpenBink open,AbortMissingDisc abort,DisableInput disable,
 I32 game_volume,SetMovieVolume set_volume) noexcept {
 if(!open)return false;
 s.movie=open(filename,0);
 if(!s.movie&&!cd_root.empty()){
  const std::string fallback=std::string(cd_root)+std::string(filename);
  s.movie=open(fallback,0);
 }
 if(!s.movie){if(abort)abort();return false;}
 original_media15::apply_global_bink_volume(
    s.movie,s.paused,game_volume,set_volume);
 if(disable)disable();
 s.input_enabled=false;
 return true;
}
void initialize_bink_playback(
 BinkInitialize& s,MakeOffscreenSurface make_surface,
 BindBinkSoundSystem set_sound,void* direct_sound) noexcept {
 s.surface_descriptor.fill(0);
 s.surface_descriptor[0]=0x7c;
 s.surface_descriptor[1]=7;
 s.surface_descriptor[2]=480;
 s.surface_descriptor[3]=640;
 s.surface_descriptor[26]=0x40;
 if(make_surface)make_surface(s.surface_descriptor);
 if(set_sound)set_sound(direct_sound);
 s.paused=false;
}
I32 decode_bink_frame_without_advance(
 void* movie,void* surface,NoAdvanceDecode decode,NoAdvanceLock lock,
 NoAdvanceCopy copy,NoAdvanceUnlock unlock) noexcept {
 if(decode)decode(movie);
 if(lock && lock(surface))return 1;
 if(copy)copy(movie);
 if(unlock)unlock(surface);
 return 0;
}
void play_activity_music_by_index(
 ActivityMusic& s,I32 index,StopSound stop,DeleteSound destroy,
 CreateMusic create,PlaySound play) noexcept {
 if(s.current){
  if(stop)stop(s.current);
  if(destroy)destroy(s.current,1);
  s.current=nullptr;
 }
 if(create)s.current=create(index);
 if(play)play(s.current,0,1);
 s.playback_latch=1;
}
void close_walkthrough_movie(
 Walkthrough& w,VoidCallback close,VoidCallback unregister,
 VoidCallback release,ResetScroll reset) noexcept {
 if(!w.surface)return;
 if(close)close(w.movie);
 if(unregister)unregister(w.surface);
 if(release)release(w.surface);
 w.surface=nullptr;
 if(reset)reset(w.scroll_value);
}
bool update_scene_set_movie(SceneSetMovie& s,
 OpenSceneMovie open,UpdateMovie update) noexcept {
 s.input_gate=1;
 if(s.stage==0){if(open)open("Data/movies/sceneset.bik");++s.stage;}
 else if(s.stage==1 && update&&update()){
  s.stage=0;
  ++s.completed_count;
  if(s.completed_count>0)return true;
 }
 return false;
}
bool update_startup_movies(
 StartupMovies& s,std::span<const I32> input_modes,
 std::span<const std::string_view> names,OpenSceneMovie open,
 UpdateMovie update) noexcept {
 if(s.current_index<0 ||
    s.current_index>=static_cast<I32>(input_modes.size()) ||
    s.current_index>=static_cast<I32>(names.size()))return false;
 s.showing=true;
 s.input_gate=input_modes[static_cast<std::size_t>(s.current_index)]!=1;
 if(s.movie_state==0){
  if(open)open(names[static_cast<std::size_t>(s.current_index)]);
  ++s.movie_state;
 }else if(s.movie_state==1 && update && update()){
  ++s.current_index;s.movie_state=0;
  if(s.count<=s.current_index){
   s.showing=false;s.input_gate=true;return true;
  }
 }
 return false;
}
HelpHoverResult update_generic_help_hover(HelpHover& s) noexcept {
 HelpHoverResult result{};
 if(!s.cursor_is_default)return result;
 if(!s.pointer_pulse_bit4){s.hover_latched=false;return result;}
 if(s.action==12){
  result.draw_help_button=true;
  result.alternate_image=s.alternate_help_surface;
 }
 if(!s.hover_latched){
  if(s.mode==0)result.voice_id=456;
  else if(s.mode==1)result.voice_id=475;
 }
 s.hover_latched=true;
 return result;
}
bool any_rect_corner_inside(const Rect& a,const Rect& b) noexcept {
 const auto inside=[&](I32 x,I32 y){
  return b.left<=x && x<=b.right && b.top<=y && y<=b.bottom;
 };
 return inside(a.left,a.top)||inside(a.right,a.top)||
        inside(a.left,a.bottom)||inside(a.right,a.bottom);
}
void* open_game_data_with_cd_fallback(
 std::string_view file,std::string_view mode,
 std::string_view cd_root,OpenGameFile open,
 AbortMissingDisc abort) noexcept {
 if(!open)return nullptr;
 void* result=open(file,mode);
 if(!result && !cd_root.empty()){
  const std::string fallback=std::string(cd_root)+std::string(file);
  result=open(fallback,mode);
 }
 if(!result && abort)abort();
 return result;
}
std::uint64_t find_maze_shortest_path(
 MazeSearchState& state,I32 target,void* graph,I32 goal,
 RecursiveMazeSearch recurse) noexcept {
 state.count=0;state.best_distance=9999999;state.requested_goal=goal;
 return recurse?recurse(target,graph,nullptr,target,0,0):0;
}
std::uint64_t build_spud_maze_graph_path(
 MazeSearchState& state,I32 target,void* graph,I32 goal,
 RecursiveMazeSearch recurse) noexcept {
 state.count=0;state.best_distance=9999999;state.requested_goal=goal;
 return recurse?recurse(target,graph,nullptr,target,0,0):0;
}
void apply_global_game_volume(
 const GlobalGameAudio& s,StopSound stop,SetSoundVolume set_volume,
 PlaySound play,SetMovieVolume movie_volume) noexcept {
 if(s.ui_context<12){
  if(s.ui_sound&&set_volume)set_volume(s.ui_sound,s.game_volume);
 }else if(s.ui_context==12){
  for(void* sound:s.activity_sounds){
   if(!sound)continue;
   if(stop)stop(sound);
   if(set_volume)set_volume(sound,s.game_volume);
  }
 }
 if(s.background_music){
  if(stop)stop(s.background_music);
  if(set_volume)set_volume(s.background_music,s.game_volume);
  if(play)play(s.background_music,0,s.music_looping?1U:0U);
 }
 // Original 004093C0 calls original 00409210, not a generic library shim.
 original_media15::apply_global_bink_volume(
   s.bink_movie,s.paused,s.game_volume,movie_volume);
 if(set_volume)set_volume(s.help_sound,s.game_volume);
}
} // namespace btb::original_media15
