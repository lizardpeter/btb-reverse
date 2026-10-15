#include "btb/original_activity4.hpp"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_activity4;
struct Mock final:IO {
 std::vector<Sprite> draws;
 std::vector<I32> sounds,help;
 std::vector<std::size_t> decoded,next,waited;
 std::vector<std::pair<std::size_t,I32>> sought;
 I32 stopped{},leave{},pilchard_ticks{},bob_ticks{},spud_voice{},
 squirrel_ticks{},timer_calls{};
 std::uint64_t clock{150000000ULL};
 U32 seed{};
 bool sound_playing{},ending{};
 std::array<bool,5> movie_finished{};
 void draw(const Sprite& d) override{assert(d.surface!=0);draws.push_back(d);}
 void sound(I32 id,I32 priority,I32 klass) override{
  assert(priority==50&&klass==1);sounds.push_back(id);
 }
 void stop_sounds() override {++stopped;}
 bool sound_active() override{return sound_playing;}
 bool sound_id_active(I32 id) override{return id==100&&sound_playing;}
 U32 random32() override{return seed++;}
 bool bink_end(std::size_t i) override{assert(i<5);return movie_finished[i];}
 void bink_decode(std::size_t i) override{decoded.push_back(i);}
 void bink_next(std::size_t i) override{next.push_back(i);}
 void bink_wait(std::size_t i) override{waited.push_back(i);}
 void bink_seek(std::size_t i,I32 f) override{sought.emplace_back(i,f);}
 I32 bink_position(std::size_t) override{return 0;}
 void update_pilchard_animation() override{++pilchard_ticks;}
 void update_bob_animation() override{++bob_ticks;}
 void maybe_spud_voice() override{++spud_voice;}
 std::uint64_t filetime_100ns() override{return clock;}
 I32 spud_maze_timer_digits(I32 v) override{
  ++timer_calls;assert(v==13||v==20);return 25;
 }
 void squirrel_run_assembly() override{++squirrel_ticks;}
 void voice_help(I32 id,I32 type) override {assert(type==1);help.push_back(id);}
 void leave_help_mode() override{++leave;}
};
int main(){
 {
  Mock io;SpudMazeScene s{};
  s.screen=0;s.spud_screen=1;s.bob_screen=0;
  s.backgrounds={10,11,12,13,14};
  s.repairs[0].push_back({20,30,1,100});
  s.repairs[1].push_back({50,60,2,101});
  s.pilchard_present=true;s.pilchard_x=80;s.pilchard_y=90;
  s.bob_x=120;s.bob_y=140;s.hammer_x=200;s.hammer_y=200;
  s.hammer_present=1;s.raise_hammer=true;s.hammer_velocity=4;
  s.pilchard_talking=true;s.remaining_hammers=2;s.used_hammers=1;
  s.baseline_filetime=80000000ULL;s.clock_seconds=20;
  draw_spud_maze_scene(s,io);
  assert(io.draws.size()==10);
  assert(io.draws[0].surface==10);
  assert(io.draws[1].surface==100&&io.draws[1].x==20);
  assert(io.draws[2].surface==s.pilchard);
  assert(io.draws[3].surface==s.bob);
  assert(io.draws[4].surface==s.hammer);
  assert(io.draws[5].surface==s.helper);
  assert(io.draws[6].surface==s.border);
  assert(io.draws[7].surface==s.digits);
  assert(io.draws[8].surface==s.hammer_on);
  assert(io.draws[9].surface==s.hammer_off);
  assert(s.last_seconds==13&&io.timer_calls==1);
  assert(io.pilchard_ticks==1&&io.bob_ticks==1&&io.spud_voice==1);
  assert(s.repairs[0][0].counter==2);
  Mock secondary;SpudMazeScene alt{};
  alt.background_mode=true;alt.screen=0;alt.spud_screen=1;
  alt.backgrounds={10,11,12,13,14};
  alt.repairs[0].push_back({20,30,1,100});
  alt.repairs[1].push_back({50,60,1,101});
  alt.old_screen_offset=12;alt.screen_offset=3;
  alt.baseline_filetime=0;alt.clock_seconds=5;alt.time_bias_seconds=15;
  secondary.clock=0;
  draw_spud_maze_scene(alt,secondary);
  assert(secondary.draws[0].surface==101&&secondary.draws[0].x==62);
  assert(secondary.draws[1].surface==100&&secondary.draws[1].x==23);
  assert(alt.last_seconds==20);
  Mock backwards;SpudMazeScene bad{};
  bad.backgrounds={10,11,12,13,14};bad.baseline_filetime=500;
  backwards.clock=200;
  bool rejected=false;
  try{draw_spud_maze_scene(bad,backwards);}
  catch(const std::runtime_error&){rejected=true;}
  assert(rejected);
 }
 {
  Mock io;SpudSkateScene s{};
  s.background=10;s.digits=11;s.end_surface=12;s.bonus_surface=13;
  s.frame_surfaces={20,21,22,23};s.link_arrows={30,31,32,33};
  s.score=7;s.active_channel=0;s.score_step=1;
  s.segment_count=1;s.segments[0]={100,200,{0,70,150,199}};
  s.event_sounds[4]={0x280,0x281,0x282,0x283,0x284};
  s.terminal_sounds={0x290,0x291,0x292,0x293,0x294};
  s.current_skate_time=100;s.terminal_time=500;s.input_select=true;
  draw_spud_skate_playback(s,io);
  assert(s.score==8&&s.pause_index==0&&s.score_step==2);
  assert(io.sounds.size()==1&&io.sounds[0]==0x280);
  assert(io.decoded.size()==4&&io.next.size()==4&&io.waited.size()==1);
  assert(io.draws[0].surface==s.background);
  assert(io.draws[1].surface==s.digits);
  assert(io.draws[2].surface==s.frame_surfaces[0]);
  assert(io.draws[3].surface==s.link_arrows[2]);
  s.current_skate_time=150;io.movie_finished[0]=true;
  draw_spud_skate_playback(s,io);
  assert(io.sought.size()==1&&io.sought[0].first==0);
  s.current_skate_time=500;s.score_step=0;
  draw_spud_skate_playback(s,io);
  assert(s.ending_counter==1);
  draw_spud_skate_playback(s,io);
  assert(s.ending_counter==2&&s.phase==1);
  s.score=25;s.last_sound_gate=0;
  io.movie_finished[4]=false;
  draw_spud_skate_playback(s,io);
  assert(io.stopped==1&&s.last_sound_gate==-1&&s.ending_profile_flag==1);
  assert(io.sounds.back()>=0x30a&&io.sounds.back()<=0x30b);
  io.movie_finished[4]=true;io.sound_playing=true;
  draw_spud_skate_playback(s,io);
  assert(s.phase==1);
  io.sound_playing=false;
  draw_spud_skate_playback(s,io);
  assert(s.phase==2);
 }
 {
  Mock io;SquirrelScene s{};
  s.screen=0;s.backdrop=1;s.foreground=2;s.foreground_alt=3;
  s.character=4;s.overlay=5;s.panel=6;s.frame_surface=7;
  s.squirrel_sheet=8;s.mouse_pointer=9;s.borders={10,11,12,13};
  s.drawable_per_screen={1,2,3,4};
  s.positions[0]={30,40,0};
  for(int i=0;i<4;++i){
   s.secondary_positions[static_cast<std::size_t>(i)]={100+i*10,200,0};
   s.sprite_frame_ids[0][static_cast<std::size_t>(i)]=i;
   s.second_frame_ids[0][static_cast<std::size_t>(i)]=i+1;
  }
  s.active_sprites={{-20,330,0},{625,350,0},{300,320,0}};
  s.active_frame_ids={3,2,-1};
  s.scroll=10;s.panel_x=40;s.panel_y=60;
  draw_squirrel_scene(s,io);
  assert(io.squirrel_ticks==1&&s.animation_counter==1);
  assert(io.draws[0].surface==s.backdrop);
  assert(io.draws[1].surface==s.foreground);
  assert(std::count_if(io.draws.begin(),io.draws.end(),
     [&](const Sprite& d){return d.surface==s.squirrel_sheet;})==5);
  assert(std::count_if(io.draws.begin(),io.draws.end(),
     [&](const Sprite& d){return d.surface==s.character;})==3);
  assert(io.draws.back().surface==13);
  assert(io.draws[io.draws.size()-2].surface==12);
  io.draws.clear();s.replay=true;s.background_counter=2;
  s.overlay_phase=0;s.drawable_per_screen[0]=1;
  draw_squirrel_scene(s,io);
  assert(s.overlay_phase==1&&s.overlay_frame==21);
  assert(io.draws[0].surface==s.foreground);
  assert(io.draws[1].surface==s.overlay);
  assert(io.squirrel_ticks==2&&s.animation_counter==2);
 }
 {
  Mock io;ContextHelp s{};
  s.activity=0xe;s.mouse_x=20;s.mouse_y=20;
  s.maze_help[0]={10,10,30,30};
  update_contextual_help(s,io);
  update_contextual_help(s,io);
  assert(io.help.size()==1&&io.help[0]==0x62&&s.previous==0xb);
  s.mouse_x=40;
  s.generic_hotspots[0xe].push_back({{30,10,50,40},0x101,8,true});
  update_contextual_help(s,io);
  assert(io.help.size()==2&&io.help.back()==0x101);
  s.mouse_x=70;update_contextual_help(s,io);
  assert(io.help.size()==2);
  s.activity=0x12;s.progress=8;s.mouse_x=40;s.mouse_y=40;
  s.special_hotspots.push_back({{20,20,60,60},0x98,0,true});
  update_contextual_help(s,io);
  assert(io.help.back()==0x98);
  s.click=true;
  update_contextual_help(s,io);
  assert(io.stopped==1&&io.leave==1);
  s.click=false;s.mouse_x=320;s.mouse_y=450;
  update_contextual_help(s,io);
  assert(io.help.back()==0xa3);
  s.activity=0x11;s.mouse_x=25;s.mouse_y=25;s.camera_x=10;s.camera_y=10;
  s.herding_hotspots[0]={30,30,60,60};
  update_contextual_help(s,io);
  assert(io.help.back()==0x269);
  s.activity=9;s.mode_index=3;s.mouse_x=80;s.mouse_y=80;
  s.generic_hotspots[9].push_back({{60,60,100,100},0x155,7,true});
  update_contextual_help(s,io);
  assert(io.help.back()==0x155);
 }
}
