#include "btb/original_coordinators2.hpp"
#include <cassert>
#include <stdexcept>
#include <vector>
using namespace btb::original_coordinators2;
struct DinoHost final:DinoIO{
 U32 seed{};bool playing{},near{},inside{};
 I32 processing{},loads{},draws{},prints{},unloads{},prepared{},stops{},
     cursor{},music_stops{},music_starts{},intro_marked{-1},award_variant{-1};
 std::vector<I32> sounds{};
 void enable_input_processing() override{++processing;}
 bool any_managed_sound_playing() override{return playing;}
 void stop_all_managed_sounds() override{++stops;}
 void sound(I32 id,I32 priority,I32 group) override{
  assert((priority==0x32||priority==0x5a)&&(group==1));sounds.push_back(id);
 }
 U32 random32() override{return seed++;}
 bool cursor_in_rectangle(Rect r) override{
  assert(r.right>=r.left&&r.bottom>=r.top);return inside;
 }
 bool cursor_near_point(I32,I32,I32 radius) override{
  assert(radius==9);return near;
 }
 void set_cursor_surface(U32 surface) override{cursor=static_cast<I32>(surface);}
 void load_completion_sprite(I32 variant) override{++loads;award_variant=variant;}
 void draw_dino_activity() override{++draws;}
 void update_print_button() override{++prints;}
 void unload_dino_activity() override{++unloads;}
 void prepare_play_again() override{++prepared;}
 void stop_activity_music() override{++music_stops;}
 void play_activity_music(I32 index) override{assert(index==1);++music_starts;}
 void mark_activity_intro_sound(I32 stage) override{intro_marked=stage;}
};
struct UiHost final:UiIO{
 I32 input{},reset{},noop{},load{},stops{},blits{};
 bool playing{},polygon_hit{};
 std::vector<I32> sounds{};
 void enable_input_processing() override{++input;}
 bool any_managed_sound_playing() override{return playing;}
 void stop_all_managed_sounds() override{++stops;}
 void play_sound(I32 id,I32 p,I32 group) override{
  assert(p==0x32&&group==2);sounds.push_back(id);
 }
 bool original_point_in_polygon(const std::vector<std::pair<I32,I32>>& poly,
                                I32,I32) override{
  assert(!poly.empty());return !polygon_hit;
 }
 void blit(U32 sprite,I32,I32,bool) override{assert(sprite!=0);++blits;}
 void invalidate_or_reset_primary_display() override{++reset;}
 void no_op_legacy_hook() override{++noop;}
 void load_progress_screen_assets() override{++load;}
};
int main(){
 {
  DinoHost io;DinoState s;
  s.profile_completed.resize(300);
  s.score_index=6;s.profile_index=1;s.level_variant=2;
  s.pieces={{10,20,20,20,50,60,0,0,0,4},
            {110,20,20,20,150,60,0,0,1,5}};
  io.inside=true;s.input_down=true;
  assert(update_dino_activity(s,io)==1);
  assert(s.phase==1&&s.selected==0&&s.pieces[0].state==1);
  assert(io.cursor==4&&io.music_starts==1&&io.intro_marked==2);
  io.near=false;
  update_dino_activity(s,io);
  assert(s.phase==1&&s.pieces[0].state==3&&!io.sounds.empty());
  io.near=true;
  update_dino_activity(s,io);
  assert(s.phase==2&&s.pieces[0].state==2&&s.stage_progress==1);
  update_dino_activity(s,io);
  assert(s.phase==0&&s.pieces[0].state==4&&s.completed==1);
  s.input_down=false;update_dino_activity(s,io);
  assert(s.completed==1);
  s.input_down=true;update_dino_activity(s,io);
  assert(s.selected==1);
  update_dino_activity(s,io);
  assert(s.phase==2);
  update_dino_activity(s,io);
  assert(s.completed==2);
  io.playing=true;
  update_dino_activity(s,io);
  assert(io.loads==0);
  io.playing=false;
  update_dino_activity(s,io);
  assert(io.loads==1&&io.award_variant==2);
  assert(s.profile_completed[102]==1&&s.completion_state==1);
  assert(io.prints==1&&io.unloads==0);
  s.completion_state=10;
  update_dino_activity(s,io);
  assert(io.prepared==1&&io.unloads==1&&s.next_game_state==0x3c);
  assert(s.stage_progress==1);
 }
 {
  DinoHost io;DinoState s;s.quit_requested=true;
  assert(update_dino_activity(s,io)==1);
  assert(io.unloads==1&&io.draws==0&&io.processing==1);
 }
 {
  UiHost io;UiState s;
  s.options[0].push_back({});
  auto& o=s.options[0][0];
  o.polygon={{0,0},{100,0},{100,100},{0,100}};
  o.next_state=17;o.voice=0x42;o.passive_frame_count=2;
  o.passive_surfaces={101,102};o.hover_frame_count=2;
  o.hover_surfaces={103,104};o.pressed_surface=105;
  o.pressed_voices={0x70,0x71};
  io.polygon_hit=true;
  update_generic_ui_screen(s,io);
  assert(s.active_option==1&&s.previous_hover==0);
  s.mouse_down=true;s.mouse_clicked=true;
  update_generic_ui_screen(s,io);
  assert(s.sound_pending&&s.selected_option==0);
  assert(!io.sounds.empty()&&io.sounds.back()==0x42);
  io.playing=true;
  update_generic_ui_screen(s,io);
  assert(s.sound_pending&&s.next_mode==0);
  io.playing=false;s.mouse_clicked=false;
  update_generic_ui_screen(s,io);
  assert(!s.sound_pending&&s.next_mode==17&&io.noop==1);
 }
 {
  UiHost io;UiState s;
  s.options[1].push_back({});
  auto& o=s.options[1][0];o.next_state=0x22;
  s.screen=1;s.selected_option=0;s.sound_pending=true;s.unlocked_mode=1;
  update_generic_ui_screen(s,io);
  assert(s.progress_screen_loaded&&s.activate_progress);
  assert(s.intro_complete==1&&io.load==1&&io.stops==1);
  s.sound_pending=true;o.next_state=-99;
  update_generic_ui_screen(s,io);
  assert(s.requires_display_reset&&io.reset==1);
 }
 {
  UiHost io;UiState s;
  s.mode=5;s.awards[50]=10;s.awards[51]=3;
  update_generic_ui_screen(s,io);
  assert(!s.progress_button_enabled);
  s.awards[51]=2;
  update_generic_ui_screen(s,io);
  assert(s.progress_button_enabled);
  bool threw=false;
  s.screen=5;
  try {update_generic_ui_screen(s,io);}catch(const std::out_of_range&){threw=true;}
  assert(threw);
 }
}
