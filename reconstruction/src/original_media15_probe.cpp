#include "btb/original_media15.hpp"
#include <array>
#include <string>
#include <vector>
namespace btb::original_media15 {
namespace {
struct Probe {
 int opens{},aborts{},disabled{},movie_volume{},volume_calls{},
     sound_volume{},stop{},destroy{},play{},close{},unregister{},
     release{},scroll{},opened_scene{},updates{},decode{},locks{},
     copies{},unlocks{},surfaces{},bound{},maze{};
 U32 last_flags{};I32 last_movie_gain{},last_music_index{},last_scroll{};
 bool open_succeeds{true},lock_fails{},movie_done{true};
 std::string opened_path{},scene_path{};
};
Probe g{};
void* const kMovie=reinterpret_cast<void*>(0x1000);
void* const kSurface=reinterpret_cast<void*>(0x2000);
void* const kSound=reinterpret_cast<void*>(0x3000);
void movie_gain(void*,I32,I32 value) noexcept {
 ++g.movie_volume;g.last_movie_gain=value;
}
void* open_movie(std::string_view path,U32 flags) noexcept {
 ++g.opens;g.opened_path=std::string(path);g.last_flags=flags;
 return (g.open_succeeds||g.opens>1)?kMovie:nullptr;
}
void abort_missing() noexcept {++g.aborts;}
void disable_input() noexcept {++g.disabled;}
void surface_create(const std::array<U32,31>& d) noexcept {
 if(d[0]==124&&d[1]==7&&d[2]==480&&d[3]==640&&d[26]==64)
  ++g.surfaces;
}
void bind_sound(void* ptr) noexcept {if(ptr==kSound)++g.bound;}
void decode(void*) noexcept {++g.decode;}
bool lock(void*) noexcept {++g.locks;return g.lock_fails;}
void copy(void*) noexcept {++g.copies;}
void unlock(void*) noexcept {++g.unlocks;}
void stop(void*) noexcept {++g.stop;}
void destroy(void*,I32) noexcept {++g.destroy;}
void* create_music(I32 index) noexcept {g.last_music_index=index;return kSound;}
void play(void*,I32,U32) noexcept {++g.play;}
void close(void*) noexcept {++g.close;}
void unregister(void*) noexcept {++g.unregister;}
void release(void*) noexcept {++g.release;}
void reset_scroll(I32 pos) noexcept {++g.scroll;g.last_scroll=pos;}
void open_scene(std::string_view name) noexcept {
 ++g.opened_scene;g.scene_path=std::string(name);
}
bool update() noexcept {++g.updates;return g.movie_done;}
void* open_file(std::string_view path,std::string_view) noexcept {
 ++g.opens;g.opened_path=std::string(path);
 return (g.open_succeeds||g.opens>1)?kSound:nullptr;
}
std::uint64_t search(I32 start,void*,void* via,I32 end,I32 depth,I32 flags) noexcept {
 ++g.maze;
 return start==end&&via==nullptr&&depth==0&&flags==0?0x12345678ULL:0;
}
void sound_volume(void*,I32) noexcept {++g.sound_volume;}
}
bool verify_15_original_media_functions() noexcept {
 g={};
 // 00409210: paused movie suppresses Bink volume, active clamps.
 original_media15::apply_global_bink_volume(kMovie,true,0,movie_gain);
 if(g.movie_volume!=0)return false;
 original_media15::apply_global_bink_volume(kMovie,false,0,movie_gain);
 if(g.movie_volume!=1)return false;
 const I32 expected=btb::bink::bink_volume_from_game_volume(0);
 if(g.last_movie_gain!=expected)return false;
 // 00408E10: fallback and then actual 00409210 call.
 g.open_succeeds=false;g.opens=0;
 if(original_media15::open_bink_movie_with_fallback(
      "scene.bik","D:/",open_movie,kMovie,false,0,movie_gain)!=kMovie ||
    g.opens!=2||g.opened_path!="D:/scene.bik"||
    g.last_flags!=0x4000000||g.movie_volume!=2)return false;
 // 00408EB0 global Bink: opened and disabled, plus missing-disc path.
 GlobalBink movie{};
 g.opens=0;g.open_succeeds=false;
 if(!original_media15::open_global_bink_movie(
      movie,"a.bik","E:/",open_movie,abort_missing,disable_input,
      0,movie_gain)||movie.movie!=kMovie||movie.input_enabled||
      g.disabled!=1||g.movie_volume!=3)return false;
 // Test abort when Bink function itself unavailable (cannot open).
 movie={};g.aborts=0;
 if(original_media15::open_global_bink_movie(
      movie,"b.bik","",nullptr,abort_missing,disable_input,0,movie_gain))
      return false;
 // 00408D90: original DD7 124-byte surface descriptor.
 BinkInitialize init{};init.paused=true;
 original_media15::initialize_bink_playback(
   init,surface_create,bind_sound,kSound);
 if(init.paused||g.surfaces!=1||g.bound!=1)return false;
 // 00409070: one decode, no NextFrame; failed lock does not copy.
 if(original_media15::decode_bink_frame_without_advance(
     kMovie,kSurface,decode,lock,copy,unlock)!=0||
    g.decode!=1||g.locks!=1||g.copies!=1||g.unlocks!=1)return false;
 g.lock_fails=true;
 if(original_media15::decode_bink_frame_without_advance(
     kMovie,kSurface,decode,lock,copy,unlock)!=1||
    g.decode!=2||g.copies!=1||g.unlocks!=1)return false;
 // 00428010: stop/destroy previous -> open/play replacement.
 ActivityMusic music{};music.current=kMovie;
 original_media15::play_activity_music_by_index(
    music,9,stop,destroy,create_music,play);
 if(g.stop!=1||g.destroy!=1||music.current!=kSound||
    music.playback_latch!=1||g.last_music_index!=9||g.play!=1)return false;
 // 004280D0: close only if real surface exists, release exact order.
 Walkthrough walk{kMovie,kSurface,23};
 original_media15::close_walkthrough_movie(
   walk,close,unregister,release,reset_scroll);
 if(walk.surface!=nullptr||g.close!=1||g.unregister!=1||
    g.release!=1||g.scroll!=1||g.last_scroll!=23)return false;
 original_media15::close_walkthrough_movie(
   walk,close,unregister,release,reset_scroll);
 if(g.close!=1)return false;
 // 0042A240 scene set: open then poll then completion gate.
 SceneSetMovie set{};
 if(original_media15::update_scene_set_movie(set,open_scene,update)||
    set.stage!=1||g.scene_path!="Data/movies/sceneset.bik")return false;
 if(!original_media15::update_scene_set_movie(set,open_scene,update)||
    set.completed_count!=1||set.stage!=0)return false;
 // 0042A1A0 startup list: authored modes and original state transitions.
 std::array<I32,2> modes{1,0};
 std::array<std::string_view,2> films{"intro1.bik","intro2.bik"};
 StartupMovies series{};series.count=2;
 if(original_media15::update_startup_movies(
     series,modes,films,open_scene,update)||
    series.input_gate||series.current_index!=0||
    g.scene_path!="intro1.bik")return false;
 if(original_media15::update_startup_movies(
     series,modes,films,open_scene,update)||
    series.current_index!=1)return false;
 if(original_media15::update_startup_movies(
     series,modes,films,open_scene,update)||
    !series.input_gate||g.scene_path!="intro2.bik")return false;
 if(!original_media15::update_startup_movies(
     series,modes,films,open_scene,update)||series.showing||
    series.current_index!=2)return false;
 // 00428470 hover latch emits voice on first hover only.
 HelpHover help{};help.cursor_is_default=true;
 help.pointer_pulse_bit4=true;help.action=12;help.mode=1;
 auto hover=original_media15::update_generic_help_hover(help);
 if(!hover.draw_help_button||hover.voice_id!=475||
    !help.hover_latched)return false;
 hover=original_media15::update_generic_help_hover(help);
 if(hover.voice_id!=-1)return false;
 help.pointer_pulse_bit4=false;
 (void)original_media15::update_generic_help_hover(help);
 if(help.hover_latched)return false;
 // 004169A0 inclusive corner, not generic overlapping rectangles.
 if(!original_media15::any_rect_corner_inside(
      Rect{1,1,3,3},Rect{3,3,8,8}))return false;
 if(original_media15::any_rect_corner_inside(
      Rect{0,0,10,10},Rect{3,3,8,8}))return false;
 // 00406E10 file relative then CD fallback, plus missing-CD failure.
 g.opens=0;g.open_succeeds=false;
 if(original_media15::open_game_data_with_cd_fallback(
     "Data/menu.txt","rb","D:/",open_file,abort_missing)!=kSound ||
    g.opens!=2||g.opened_path!="D:/Data/menu.txt")return false;
 // 0041CBE0 and 00423220 reset exact graph-search globals.
 MazeSearchState state{3,4,5};
 if(original_media15::find_maze_shortest_path(
      state,11,kSurface,31,search)!=0x12345678ULL||
    state.count!=0||state.best_distance!=9999999||
    state.requested_goal!=31)return false;
 if(original_media15::build_spud_maze_graph_path(
      state,22,kSurface,41,search)!=0x12345678ULL||
    g.maze!=2||state.requested_goal!=41)return false;
 // 004093C0 must dispatch ORIGINAL native 00409210 internally.
 std::array<void*,2> active{kMovie,kSurface};
 GlobalGameAudio global{};global.ui_context=12;
 global.activity_sounds=active;global.background_music=kSound;
 global.help_sound=kSound;global.bink_movie=kMovie;global.game_volume=0;
 const int prev_movie=g.movie_volume,prev_stop=g.stop,prev_play=g.play;
 original_media15::apply_global_game_volume(
     global,stop,sound_volume,play,movie_gain);
 if(g.movie_volume!=prev_movie+1||g.stop!=prev_stop+3||
    g.play!=prev_play+1||g.sound_volume<4)return false;
 return true;
}
} // namespace btb::original_media15
