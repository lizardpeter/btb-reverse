#include "btb/original_gameplay3b.hpp"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>
using namespace btb::original_gameplay3b;
struct GolfHost:GolfIO {
 U32 next{};I32 calls{},moves{},losses{};
 std::vector<I32> voices{};
 U32 random32() override{return next++;}
 void play_sound(I32 id,I32 priority,I32 type) override{
  assert(priority==50&&type==1);voices.push_back(id);
 }
 Vec2 original_vector_from_degrees(I32 degrees,I32 magnitude) override{
  assert(degrees>=0&&magnitude>=0);++moves;return {float(magnitude),0.f};
 }
 I32 original_charge_step(I32 difficulty) override{assert(difficulty==2);return 8;}
 I32 original_launch_angle(const Golf&) override{return 12;}
 I32 original_energy_loss(const Golf&) override{++losses;return 55;}
 float original_distance(Point a,Point b) override{
  ++calls;return std::hypot(float(a.x-b.x),float(a.y-b.y));
 }
};
struct SquirrelHost:SquirrelRunIO{
 U32 next{};I32 selected{},noop{},frames{},code_calls{};
 U32 random32() override{return next++;}
 void select_original_motion_profile(SquirrelRun& s) override{
  ++selected;s.profile=1;s.motion_variant=1;
 }
 void no_op_legacy_hook() override{++noop;}
 I32 original_frame_offset(I32 profile,I32 frame,bool descending) override{
  assert(profile>=0&&profile<=7&&frame>=0&&frame<=4);
  ++code_calls;return descending?-2:2;
 }
 I32 original_sequence_base(I32 profile) override{
  assert(profile>=0&&profile<=7);return profile*4;
 }
 bool original_visibility_gate(I32,I32,bool) override{return true;}
 void original_blt(I32,I32,U32 surface,Box rect,bool keyed) override{
  assert(surface==9&&keyed&&rect.r>rect.l&&rect.b>rect.t);
  ++frames;
 }
};
struct HerdHost:HerdingIO {
 U32 seed{};I32 input{},unload{},music_stop{},music_play{},draw{},animal_updates{},
   prepare{},intro{},collide{},excluded{},rect_tests{};
 bool playing{},outside{};
 std::vector<I32> voice{};
 void enable_input_processing() override{++input;}
 U32 random32() override{return seed++;}
 float distance(Point a,Point b) override{return std::hypot(float(a.x-b.x),float(a.y-b.y));}
 I32 angle_degrees(Point,Point) override{return 90;}
 bool polygon_excludes(Point) override{++excluded;return outside;}
 bool rectangles_intersect(Box,Box) override{++rect_tests;return false;}
 bool original_pen_or_animal_collision(const HerdingAnimal&,I32) override{++collide;return false;}
 void update_original_animal(HerdingAnimal& a,I32) override{++animal_updates;++a.x;}
 void draw_herding_activity() override{++draw;}
 bool any_managed_sound_playing() override{return playing;}
 void play_sound(I32 id,I32 priority,I32 group) override{
  assert(priority==0x5a&&group==1);voice.push_back(id);
 }
 void unload_herding_activity() override{++unload;}
 void prepare_play_again() override{++prepare;}
 void stop_activity_music() override{++music_stop;}
 void play_activity_music(I32 index) override{assert(index==6);++music_play;}
 void mark_intro_sound(I32 id) override{assert(id==0x245);++intro;}
};
int main(){
 {
  GolfHost host;Golf s{};s.start_x=10;s.start_y=20;s.cursor_x=49;
  s.difficulty=2;s.mode=0;s.remaining_shots=3;s.goal_threshold=1;
  for(auto& h:s.holes){h.x=2000;h.y=2000;}
  assert(update_golf_gameplay(s,host)==81);
  s.mouse_confirm=true;update_golf_gameplay(s,host);
  assert(s.phase==1);
  s.mouse_confirm=false;s.mouse_down=false;
  for(int i=0;i<4;i++)update_golf_gameplay(s,host);
  assert(s.power==32);
  s.mouse_down=true;
  update_golf_gameplay(s,host);
  assert(s.phase==2);
  update_golf_gameplay(s,host);
  assert(s.phase==99&&s.velocity>=500);
  for(int i=0;i<90&&s.phase==99;i++)update_golf_gameplay(s,host);
  assert(s.phase==3);
  for(int i=0;i<200&&s.phase==3;i++)update_golf_gameplay(s,host);
  assert(s.phase==4&&host.moves>0&&host.losses>0);
  update_golf_gameplay(s,host);assert(s.phase==5);
  update_golf_gameplay(s,host);
  assert(s.phase==6&&host.voices.size()==1);
  update_golf_gameplay(s,host);
  assert(s.phase==0&&s.remaining_shots==2&&s.ball_x==10&&s.ball_y==20);
 }
 {
  GolfHost host;Golf s{};s.phase=5;s.remaining_shots=1;s.score=0;s.hole=-1;
  update_golf_gameplay(s,host);
  assert(host.voices.back()==0x87);
  s.phase=5;s.remaining_shots=2;s.hole=1;
  update_golf_gameplay(s,host);
  assert(host.voices.back()>=0x66&&host.voices.back()<=0x71);
 }
 {
  SquirrelHost host;SquirrelRun s;
  s.sprite_surface=9;s.sprite_width=32;s.sprite_height=48;
  s.cadence=1;s.countdown=1;s.reset_run=0;
  update_and_draw_squirrel_run(s,host);
  assert(s.phase==1&&host.selected==1&&host.frames==1);
  for(int i=0;i<30;i++)update_and_draw_squirrel_run(s,host);
  assert(host.noop>=1&&host.frames>=4&&host.code_calls>=2);
  assert(s.completed_cycles>=1);
  s.sprite_surface=0;
  bool threw=false;
  try{update_and_draw_squirrel_run(s,host);}catch(const std::runtime_error&){threw=true;}
  assert(threw);
 }
 {
  HerdHost io;Herding s;
  s.position_x=80;s.position_y=90;s.current_x=80;s.current_y=90;
  s.input_mask=2;s.motion_enabled=true;
  s.remaining=2;
  s.pens[0].center={500,500};s.pens[1].center={550,500};s.pens[2].center={600,500};
  s.animals.push_back({.id=1,.behavior=1,.size_x=20,.size_y=20,.x=30,.y=40});
  s.animals.push_back({.id=2,.behavior=3,.size_x=30,.size_y=30,.x=100,.y=90});
  update_herding_activity(s,io);
  assert(io.input==1&&io.draw==1&&io.animal_updates==2);
  assert(s.current_x>80&&s.frames_rendered==1);
  assert(io.music_play==1&&io.intro==1&&io.voice.back()==0x245);
  io.outside=true;
  const auto old=s.current_x;
  update_herding_activity(s,io);
  assert(s.current_x==old);
  io.outside=false;
  s.remaining=0;io.playing=false;
  update_herding_activity(s,io);
  assert(s.finish_state==1&&io.voice.back()>=599&&io.voice.back()<=600);
  io.playing=true;update_herding_activity(s,io);
  assert(io.prepare==0);
  io.playing=false;update_herding_activity(s,io);
  assert(io.prepare==1&&io.unload==1&&s.game_state==0x3c);
 }
 {
  HerdHost io;Herding s;s.quit_requested=true;s.saved_progress=-1;
  update_herding_activity(s,io);
  assert(io.unload==1&&io.draw==0);
 }
}
