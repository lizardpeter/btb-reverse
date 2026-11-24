#include "btb/original_motion4.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"original_motion4: %s at %s:%d\\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)
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
  // Capturing a reached node must precede testing its permissions.
  Host io;Player p{};p.speed=2;p.x=20;p.y=0;
  auto a=node(0,0,0,2);a.links={-1,1,-1,-1};
  auto b=node(1,20,0,9);b.links={2,-1,-1,0};
  auto c=node(2,30,-80,4);c.links={-1,-1,1,-1};
  p.graphs[0].nodes={a,b,c};
  update_maze_player(p,io);
  assert(p.node==1&&p.movement_mask==9);
  io.in={0,-1,false,false};
  for(int frame=0;frame<30;++frame)update_maze_player(p,io);
  assert(p.x>20&&p.y< -50&&p.movement_mask==5);
  for(int frame=0;frame<20&&p.node!=2;++frame)update_maze_player(p,io);
  assert(p.node==2);
  auto y=p.y;
  for(int frame=0;frame<10;++frame)update_maze_player(p,io);
  assert(p.y==y);
 }
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
  assert(p.transition==1&&p.screen==2&&p.x==325&&p.y==100);
 }
 {
  // Original 0041B140: East->Middle uses node 15, not node 0.
  // 0043B488 stores double 25.0, inward from every receiving edge.
  Host io;io.in={-1,0,false,false};
  Player p{};p.screen=2;p.node=0;p.x=40;p.y=169;
  p.graphs[2].nodes={node(0,40,169,15,0x200)};
  p.graphs[1].nodes={node(0,40,171,15,0x80),
                     node(15,603,171,15,0x100)};
  update_maze_player(p,io);
  assert(p.screen==1&&p.node==15&&p.x==578&&p.y==171);
  assert(p.last_captured_node==-1&&p.transition==1);
  p.screen=1;p.node=0;p.x=40;p.y=171;
  p.graphs[0].nodes={node(0,587,177,15,0x200)};
  update_maze_player(p,io);
  assert(p.screen==0&&p.node==0&&p.x==562&&p.y==177);
  p.screen=0;p.node=0;p.x=587;p.y=177;io.in={1,0,false,false};
  update_maze_player(p,io);
  assert(p.screen==1&&p.node==0&&p.x==65&&p.y==171);
 }
 {
  // 00421360: rightward Spud portals 0/1 pair with 2/3;
  // leftward 2/3 pair with 0/1. 0043B4B8 is double 15.0.
  Host io;Player p{};
  p.graphs[0].nodes={node(0,606,289,15,0x100),node(1,606,375,15,0x800)};
  p.graphs[1].nodes={node(0,606,289,15,0x100),node(1,606,375,15,0x800),
                    node(2,33,289,15,0x80),node(3,33,375,15,0x400)};
  for(int lane=0;lane<2;++lane){
   p.screen=0;p.node=lane;p.x=606;p.y=lane?375:289;
   io.in={1,0,false,false};
   update_spud_maze_bob(p,io);
   assert(p.screen==1&&p.node==lane+2&&p.x==48&&p.y==(lane?375:289));
   p.x=33;io.in={-1,0,false,false};
   update_spud_maze_bob(p,io);
   assert(p.screen==0&&p.node==lane&&p.x==591&&p.y==(lane?375:289));
  }
  assert(io.resets==4);
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
  // Confirm source 0x0041CCB0 uses the BOX-LOCATION table's node ID,
  // rather than the 0..3 random location index as the path target.
  Host io;Npc s{};s.screen=0;s.player_screen=0;s.difficulty=2;
  s.spawn_wait=1;s.spawn_interval=800;
  auto a=node(0,0,0);a.links={4,-1,-1,-1};
  s.graphs[0].nodes={a,node(4,0,-2)};
  s.packages[0][0]={2,1};
  s.package_target_nodes[0][2]=4;s.has_box_target_nodes=true;
  update_maze_spud_npc(s,io);
  assert(s.phase==btb::maze::SpudNpcPhase::SelectPackage);
  update_maze_spud_npc(s,io);
  assert(s.phase==btb::maze::SpudNpcPhase::TravelToPackage);
  assert(io.paths.back()==4);
  s.phase=btb::maze::SpudNpcPhase::TravelCarryingPackage;
  s.current_node=0;s.path={4};s.path_index=0;
  s.selected_package=0;s.packages[0][0].status=2;
  s.position={0.f,0.f};io.player_gap=10.0f;
  update_maze_spud_npc(s,io);
  assert(s.packages[0][0].status==btb::maze::kSpudDroppedPackageStatus);
  assert(s.packages[0][0].dropped_x==
         static_cast<I32>(s.position.x));
  assert(s.packages[0][0].dropped_y==
         static_cast<I32>(s.position.y));
  assert(s.carry_active==0&&s.collision_latch==1);
  assert(io.sounds.back()>=0x5e&&io.sounds.back()<=0x5f);
  static_assert(btb::maze::kSourceNpcSpawnFrames[0]==3000);
  static_assert(btb::maze::kSourceNpcSpawnFrames[1]==2000);
  static_assert(btb::maze::kSourceNpcSpawnFrames[2]==800);
 }
 {
  // Source 0x0041CCB0 phase 1 respawns Spud on Bob's current screen.
  Host io;Npc s{};s.screen=0;s.player_screen=1;
  s.phase=btb::maze::SpudNpcPhase::SelectPackage;
  s.graphs[1].nodes={node(0,40,182),node(15,603,182)};
  s.packages[1][0]={0,1};
  s.package_target_nodes[1][0]=0;s.has_box_target_nodes=true;
  update_maze_spud_npc(s,io);
  assert(s.screen==1);
  assert(s.phase==btb::maze::SpudNpcPhase::TravelToPackage);
  assert(s.selected_package==0);
  assert(!io.paths.empty()&&io.paths.back()==0);
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
