#include "btb/original_motion4.hpp"
#include <cassert>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_motion4;
static btb::maze::Node node(int id,int x,int y,int bits=15,int flags=0){
 btb::maze::Node n{};n.id=id;n.retail_position={x,y};
 n.source_position={x,y};n.direction_bits=bits;n.node_type=flags;
 return n;
}
struct Host final:IO {
 Input in{}; U32 seed{};
 std::vector<I32> sounds,completions,paths;
 int input_count{},spud_inputs{},resets{},recovered{},clock{};
 float player_gap{9999.f};
 bool boundary_collision{};
 Input player_input(bool spud) override{++input_count;spud_inputs+=spud;return in;}
 U32 random32() override {return seed++;}
 void sound(I32 id,I32 priority,I32 category) override{
  assert(category==1&&((priority==10)||(priority==50)||(priority==90)));
  sounds.push_back(id);
 }
 void spud_spawn_delay_reset() override{++resets;}
 float npc_distance_to_player(const Npc&) override{return player_gap;}
 void spud_repair_completed(I32 screen,I32 index) override{
  completions.push_back(screen*10+index);
 }
 float actor_distance(float x,float y,float tx,float ty) override{
  return std::hypot(x-tx,y-ty);
 }
 I32 integer_angle(I32,I32,I32,I32) override{return 90;}
 std::array<float,2> movement_delta(I32 deg,float speed) override{
  assert(deg==90);return {speed,0.0f};
 }
 std::vector<I32> shortest_path(const btb::maze::ScreenGraph&,I32,I32 to) override{
  paths.push_back(to);return {to};
 }
 void animal_collision_recovery(btb::herding::RetailEntityRecord32& a) override{
  ++recovered;a.x=a.previous_x;a.y=a.previous_y;
 }
 bool original_herding_boundary_collision(const btb::herding::RetailEntityRecord32&) override{
  return boundary_collision;
 }
};
int main(){
 {
  Host io;Player p{};p.screen=0;p.node=0;p.x=10;p.y=0;p.speed=2;
  auto a=node(0,0,0);a.links={-1,1,-1,-1};
  p.graphs[0].nodes={a,node(1,20,0)};
  io.in={1,0,false,false};
  update_maze_player(p,io);
  assert(p.x==12&&p.previous_x==10&&p.y==0);
  assert(p.facing==1&&p.movement_mask==10&&io.input_count==1);
  p.frozen=true;update_maze_player(p,io);
  assert(io.input_count==1&&p.previous_x==12);
  p.frozen=false;p.node=0;p.x=0;p.y=0;
  p.graphs[0].nodes[0].node_type=0x100;
  p.graphs[2].nodes={node(0,300,100)};
  update_maze_player(p,io);
  assert(p.transition==1&&p.screen==2&&p.x==300&&p.y==100);
 }
 {
  Host io;Player p{};p.screen=0;p.node=0;p.x=0;p.y=0;
  p.graphs[0].nodes={node(0,0,0)};
  p.repairs[0].push_back({1,-30,40,1});
  p.repair_tool_engaged=true;
  io.in={0,0,false,true};
  update_spud_maze_bob(p,io);
  assert(p.occupied==false&&p.repair_hit_count==1);
  assert(p.repairs[0][0].counter==0&&p.hazard_lock);
  assert(io.completions.size()==1&&io.completions[0]==0);
  assert(io.sounds.size()==1&&io.sounds[0]==0x2be);
  p.hazard_lock=false;p.repair_tool_engaged=false;
  p.repairs[0][0].counter=1;
  update_spud_maze_bob(p,io);
  assert(p.repairs[0][0].counter==1&&io.sounds.back()>=0x2c5);
  assert(io.spud_inputs==2);
 }
 {
  Host io;Npc s{};s.screen=0;s.player_screen=0;s.spawn_wait=1;s.difficulty=0;
  auto a=node(0,0,0);a.links={1,-1,-1,-1};
  s.graphs[0].nodes={a,node(1,0,-2)};
  s.packages[0][0]={1,1};
  update_maze_spud_npc(s,io);
  assert(s.phase==btb::maze::SpudNpcPhase::SelectPackage&&s.spawn_wait==0);
  update_maze_spud_npc(s,io);
  assert(s.phase==btb::maze::SpudNpcPhase::TravelToPackage&&s.selected_package==0);
  assert(!io.sounds.empty()&&io.sounds.back()==0x58&&io.paths.back()==1);
  update_maze_spud_npc(s,io);
  assert(s.phase==btb::maze::SpudNpcPhase::ChooseReturnRoute);
  assert(s.packages[0][0].status==2&&io.sounds.back()==0x59);
  update_maze_spud_npc(s,io);
  assert(s.phase==btb::maze::SpudNpcPhase::TravelCarryingPackage);
  s.player_screen=1;s.offscreen_wait=2;
  update_maze_spud_npc(s,io);
  assert(s.offscreen_wait==1&&s.phase==btb::maze::SpudNpcPhase::TravelCarryingPackage);
  update_maze_spud_npc(s,io);
  assert(s.offscreen_wait==0);
  update_maze_spud_npc(s,io);
  assert(s.phase==btb::maze::SpudNpcPhase::SelectPackage);
  assert(s.packages[0][0].status==1);
 }
 {
  Host io;HerdAnimal s{};
  s.source.entity_id=1;s.source.type=static_cast<I32>(btb::herding::EntityType::Sheep);
  s.source.behavior_state=10;
  s.source.x=637;s.source.y=135;s.source.x_float=637;s.source.y_float=135;
  s.max_speed=2;s.acceleration=0.1f;s.arrival_radius=10;
  s.undelivered=9;
  assert(update_herding_animal(s,io)==0);
  assert(s.source.behavior_state==20&&s.undelivered==9);
  s.source.x=503;s.source.y=142;s.source.x_float=503;s.source.y_float=142;
  s.source.movement_speed=0;
  assert(update_herding_animal(s,io)==0);
  assert(s.source.behavior_state==99&&s.undelivered==8&&s.collision_latch);
  s.direction_countdown=0;s.source.animation_frame=12;
  update_herding_animal(s,io);
  assert(s.source.animation_frame==9&&s.direction_countdown==150);
  s.source.behavior_state=1;s.home_counter=4;s.difficulty=2;
  update_herding_animal(s,io);
  assert(io.sounds.back()==s.source.type+0x253&&s.source.behavior_state==1);
  s.source.behavior_state=0;io.boundary_collision=true;
  update_herding_animal(s,io);
  assert(io.recovered==1);
  s.source.entity_id=-1;update_herding_animal(s,io);
  assert(io.recovered==1);
 }
}
