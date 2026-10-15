#include "btb/original_herding_init.hpp"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <vector>
using namespace btb::original_herding_init;
struct Event{U32 slot{},symbol{},handle{};};
struct Keyed{U32 slot{},handle{},color{};};
struct Host final:IO {
 U32 rng{};int unload{},reads{},volume_calls{},volume{};
 std::vector<Event> loaded;
 std::vector<Keyed> keyed;
 btb::herding::Data load_original_coordinate_data() override{
  ++reads;btb::herding::Data d{};
  d.setup_positions[3]={350,390};
  d.coordinate_groups.resize(4);
  for(int i=0;i<12;++i)d.coordinate_groups[0].push_back({i*10+64,i*10+100});
  for(int i=0;i<5;++i)d.coordinate_groups[1].push_back({i*3+100,i*3+200});
  for(int i=0;i<6;++i)d.coordinate_groups[2].push_back({i*4+300,i*4+400});
  d.coordinate_groups[3].push_back({777,999});
  return d;
 }
 void register_original_unload() override{++unload;}
 void set_global_game_volume(I32 value) override{++volume_calls;volume=value;}
 U32 load_registered_bitmap(U32 slot,U32 symbol) override{
  assert(slot&&symbol);
  U32 handle=900+static_cast<U32>(loaded.size());
  loaded.push_back({slot,symbol,handle});
  return handle;
 }
 void key_bitmap(U32 slot,U32 handle,U32 color) override{
  assert(color==0xff00ffU);keyed.push_back({slot,handle,color});
 }
 U32 original_random32() override{return rng++;}
 float original_angle_to_radians(I32 facing) override{
  assert(facing>=0&&facing<=7);
  return static_cast<float>(facing)*0.7853981633974483f;
 }
};
int main(){
 {
  Host io;State s;s.difficulty=0;s.volume=90;
  initialize_herding(s,io);
  assert(s.initialized&&s.difficulty==0&&s.startup_flag==1);
  assert(io.unload==1&&io.reads==1&&io.volume_calls==1);
  assert(io.volume==-420);
  assert(s.bitmaps.size()==18&&io.loaded.size()==18);
  assert(io.keyed.size()==17);
  assert(s.bitmaps[0].guest_slot==0x00510724U&&!s.bitmaps[0].color_keyed);
  assert(s.bitmaps.back().guest_slot==0x00510728U&&s.bitmaps.back().color_keyed);
  assert(s.leader_count==9&&s.herd_remaining==9&&io.rng==9);
  assert(s.entities.size()==15);
  assert(s.entities[0].x==350&&s.entities[0].y==390);
  assert(s.entities[1].x==600&&s.entities[1].y==550);
  assert(s.entities[2].x==750&&s.entities[2].y==550);
  assert(s.entities[1].type==1&&s.entities[4].type==2);
  assert(s.entities[1].bitmap!=0);
  assert(s.original_polygon.size()==12);
  assert(s.original_polygon[0].x==0&&s.original_polygon[0].y==0);
  assert(s.guide_route.size()==5&&s.roaming_path.size()==6);
  assert(s.finish_marker.x==777&&s.finish_marker.y==999);
  assert(s.gate_left_index==13&&s.gate_right_index==14);
  assert(s.entities[13].type==15&&s.entities[14].type==16);
  assert(s.music_start_pending==1);
  for(const int index:s.followers)assert(index==-1);
 }
 {
  Host io;State s;s.difficulty=2;s.volume=100;
  initialize_herding(s,io);
  assert(s.difficulty==2&&s.leader_count==15&&io.rng==15);
  assert(s.entities.size()==22);
  assert(s.entities.back().type==7&&s.entities.back().x==100);
  assert(s.gate_left_index==16&&s.gate_right_index==17);
 }
 {
  Host io;State s;s.difficulty=2;s.player_mode=1;
  initialize_herding(s,io);
  assert(s.difficulty==0&&s.leader_count==9);
 }
 {
  Host io;State s;s.difficulty=-1;bool threw=false;
  try{initialize_herding(s,io);}catch(const std::out_of_range&){threw=true;}
  assert(threw&&io.loaded.empty());
 }
}
