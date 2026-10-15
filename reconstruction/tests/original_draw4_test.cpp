#include "btb/original_draw4.hpp"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_draw4;
struct Render:IO{
 std::vector<Draw> calls;
 std::vector<I32> sounds;
 int projected{},restores{},clock_calls{},random_calls{};
 bool fail_first{};
 U32 bad_surface{};
 void draw(const Draw& d) override {
  if(fail_first&&bad_surface==d.surface){
   fail_first=false;throw std::runtime_error("DDERR_SURFACELOST");
  }
  assert(d.surface!=0);
  calls.push_back(d);
 }
 void restore_surface(U32 slot) override {
  assert(slot==bad_surface);++restores;
 }
 void sound(I32 id,I32 priority,I32 sound_class) override {
  assert(priority==10&&sound_class==1);
  sounds.push_back(id);
 }
 U32 random32() override {++random_calls;return 3;}
 std::array<I32,2> polar(I32 degrees,I32 speed) override{
  assert(speed==140);assert(degrees==88);++projected;
  return {12,-8};
 }
 I32 maze_clock_digits() override {++clock_calls;return 7;}
};
int main(){
 {
  Render io;DinoState s{};
  s.tracks[0].phase=6;s.tracks[1].phase=0;
  s.tracks[1].requested=2;
  s.entities={
   {1,1,6,7,500,0,0,{0,0,12,14}},
   {4,2,40,50,501,0,0,{0,0,12,14}},
   {2,3,8,9,502,90,100,{0,0,10,10}},
   {0,1,11,12,503,0,0,{0,0,11,11}},
   {3,0,20,25,504,0,0,{0,0,12,14}}
  };
  draw_dino(s,io);
  assert(s.tracks[0].phase==0&&s.tracks[0].animation==1);
  assert(s.tracks[1].animation==2&&s.tracks[1].requested==-1);
  assert(io.calls.size()==7);
  assert(io.calls[0].surface==s.backdrop);
  assert(io.calls[1].surface==s.character);
  assert(io.calls[3].surface==501); // mode-4 entities must draw first
  assert(io.calls[4].surface==500); // active type 1
  assert(io.calls[5].surface==502); // mode-2 picture-space
  assert(io.calls[6].surface==503); // active type 1
  s.frame_variant=3;
  bool bad=false;try{draw_dino(s,io);}catch(const std::out_of_range&){bad=true;}
  assert(bad);
 }
 {
  Render io;MazeState s{};
  s.screen=1;s.view_offset=10;
  s.actors[0]={0,1,0,0};s.actors[1]={-1,3,40,50};
  s.original_node_positions[4]={120,130};
  s.bob_x=80;s.bob_y=90;s.bob_pose=2;
  s.bob_previous_x=80;s.bob_previous_y=90;
  s.sprite_moving=true;s.bob_timer=1;s.bob_frame=7;
  draw_maze(s,io);
  assert(io.calls.size()==4);
  assert(io.calls[0].surface==s.backdrops[1]);
  assert(io.calls[1].x==122&&io.calls[1].y==122);
  assert(io.calls[2].x==42&&io.calls[2].y==42);
  assert(io.calls[3].surface==s.player_surface);
  assert(io.calls[3].clip->l==256);
  assert(!s.sprite_moving&&s.bob_frame==0&&io.sounds.size()==1&&io.sounds[0]==0x4d);
  assert(s.timer_digits==7&&s.view_x==266&&s.view_y==266);
  MazeState back{};
  back.foreground_active=false;back.screen=0;back.right_edge=8;
  back.actors[0]={-1,3,20,30};
  back.other_enabled=2;back.other_screen=0;back.spud_pose=4;
  Render secondary;draw_maze(back,secondary);
  assert(secondary.calls.size()==3);
  assert(secondary.calls[0].surface==back.box_surface);
  assert(secondary.calls[0].x==12&&secondary.calls[0].y==22);
  assert(secondary.calls[1].surface==back.spud_surface);
  assert(secondary.calls[2].x==20&&secondary.calls[2].y==22);
  assert(back.view_offset==8);
 }
 {
  Render io;GolfState s{};
  s.score=27;s.markers=2;s.swing_angle=90;s.swing_state=1;
  s.trail_gate=4;s.trail_threshold=1;s.ball_x=200;s.ball_y=220;
  s.club_x=10;s.club_y=20;s.background_phase=1;
  s.meter_value=300;
  s.actors[0]={500,10,20,1,10,20,1,5,4,0,0};
  s.actors[1]={501,30,40,2,10,20,1,4,4,0,0};
  s.extras[0]={502,50,60,3,15,20,1,8,4,0,0};
  s.extras[1]={503,70,80,13,15,20,1,20,4,0,0};
  s.extras[2]={504,90,100,13,15,20,1,20,4,0,0};
  io.fail_first=true;io.bad_surface=500;
  draw_golf(s,io);
  assert(io.projected==1&&io.restores==1);
  assert(io.calls.size()==14); // ball hidden while swing_state !=0
  assert(io.calls[0].surface==s.backdrop);
  assert(io.calls[1].clip->l==32&&io.calls[2].clip->l==112);
  assert(io.calls[3].x==373&&io.calls[4].x==393);
  assert(io.calls[5].surface==s.trail&&io.calls[6].surface==s.trail_secondary);
  assert(io.calls.back().surface==s.celebration);
  assert(io.calls.back().clip->r==77);
  s.swing_state=0;s.markers=1;s.trail_gate=0;
  io.calls.clear();
  draw_golf(s,io);
  assert(std::any_of(io.calls.begin(),io.calls.end(),[&](const Draw& d){
   return d.surface==s.ball&&d.x==212&&d.y==212;
  }));
 }
 {
  Render io;HerdState s{};
  s.camera_x=500;s.camera_y=200;
  s.entities={
   {1,3,5,1,0,1,25,35,60,0,0,0,0,0,700,false},
   {2,0x0d,11,1,0,0,55,65,15,0,0,0,0,0,701,false},
   {3,0x0e,0,1,0,0,70,80,30,0,0,0,0,0,702,false},
   {4,0x11,0,1,0,0,80,90,40,0,0,0,0,0,703,false},
   {5,7,2,1,0,1,95,105,50,0,0,0,0,0,704,false},
  };
  s.props[0]={true,800,100,110};
  s.overlay_index=2;
  draw_herding(s,io);
  assert(s.view_x==200&&s.view_y==90);
  assert(io.calls.size()==8); // landscape + 4 visible + prop + FG + overlay
  assert(io.calls[0].clip->l==200&&io.calls[0].clip->t==90);
  assert(io.calls[1].surface==701); // depth 15
  assert(io.calls[2].surface==702); // depth 30
  assert(io.calls[3].surface==704); // depth 50
  assert(io.calls[4].surface==700); // depth 60
  assert(s.entities[1].frame==0&&s.entities[1].counter==10);
  assert(s.entities[2].x==380&&s.entities[2].y==364);
  assert(io.calls[1].clip->r==230);
  assert(io.calls.back().surface==0x005105a8U);
  assert(io.calls[io.calls.size()-2].surface==s.foreground);
 }
}
