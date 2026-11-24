#include "btb/original_gameplay3.hpp"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_gameplay3;
struct SqIO:SquirrelIO {
 std::vector<I32> sounds;bool reached{true};U32 seed{};int stopped{};
 bool step_actor_toward(I32 x,I32 y) override{
  assert(x>=0&&y>=0);return reached;}
 U32 random32() override{return seed++;}
 void stop_all_sounds() override{++stopped;}
 void sound(I32 id,I32 p,I32 kind) override{
  assert(p==50&&kind==1);sounds.push_back(id);}
};
struct PilIO:PilchardIO {
 int path_calls{},advance_calls{},collisions{},sounds{};
 bool near{true},hit{};
 U32 seed{};
 U32 random32() override {return seed++;}
 std::vector<I32> shortest_path(I32 start,I32 target) override{
  ++path_calls;assert(start>=0&&target>=0);
  return start==target?std::vector<I32>{target}:std::vector<I32>{1,target};
 }
 std::vector<I32> neighbors(I32,I32) override{
  return {1,2,3,-1};
 }
 XY node_position(I32 screen,I32 node) override{return {screen*10+node*20,12};}
 bool advance_toward_node(I32 n,I32 speed,XY& pos) override{
  assert(n>=0&&speed>=0);++advance_calls;pos={n*20,12};return true;}
 I32 distance_to_node(I32,const XY&) override{return near?1:20;}
 bool collision_and_hammer_drop() override{++collisions;return hit;}
 void sound(I32 id,I32 p,I32 kind) override{
  assert(id==0x2ce||id==0x2cf);assert(p==50&&!kind);++sounds;}
 I32 pursuit_speed(I32) override{return 10;}
 I32 wander_speed(I32) override{return 8;}
};
struct MovieIO:FireworksIO {
 I32 elapsed{},movie_seen{-1};int stops{},plays{},draws{},overlays{},end_resets{},end_stops{},restarts{},top_restarts{};
 bool crowd_done{},active_done{},top_done{},any_sound{},end_sound{};
 U32 rng{};std::vector<I32> sound_ids;std::vector<std::size_t> launched,rewound;
 I32 elapsed_centiseconds() override{return elapsed;}
 bool decode_crowd_movie(I32 movie,I32 column) override{
  assert(movie==21||movie==22);assert(column==elapsed/400);
  movie_seen=movie;return crowd_done;}
 bool decode_topmiddle_movie(I32) override{return top_done;}
 bool decode_active_movie(std::size_t,I32 type,I32 row,I32) override{
  assert(type>=0&&type<12&&row>=0&&row<3);return active_done;}
 void restart_crowd_movie(I32 movie) override{assert(movie==21);++restarts;}
 void restart_topmiddle_movie() override{++top_restarts;}
 void restart_active_movie(std::size_t slot) override{rewound.push_back(slot);}
 bool any_managed_sound_playing() override{return any_sound;}
 bool sound_id_playing(I32 id) override{assert(id==0x15d);return end_sound;}
 U32 random32() override{return rng++;}
 void stop_all_managed_sounds() override{++stops;}
 void play_managed_sound(I32 id,I32 priority,I32 kind) override{
  assert(priority==50&&(kind==1||kind==2));++plays;sound_ids.push_back(id);}
 void draw_background() override{++draws;}
 void draw_end_overlay() override{++overlays;}
 void reset_end_sound() override{++end_resets;}
 void stop_end_sound() override{++end_stops;}
 void launch_movie(std::size_t slot,I32 movie,I32 x,I32 y) override{
  assert(movie>=0&&movie<23&&x>=0&&y>=0);launched.push_back(slot);}
};
int main(){
 {
  SqIO io;SquirrelState s;
  s.selectable_count=2;s.selection_positions[0]={10,20};
  s.selection_positions[1]={110,120};
  s.selection_enabled[0]=1;s.selection_enabled[1]=0;
  s.selection_values[0]=4;s.selection_values[1]=5;
  s.round_positions[0]={15,30};s.round_counts[0]=0;
  s.placement_round=0;s.placed_capacity=4;s.points=0;s.home={5,5};
  update_squirrel_placement(s,20,30,true,io);
  assert(s.phase==1&&s.selected==0);
  io.reached=false;
  update_squirrel_placement(s,20,30,false,io);
  assert(s.phase==1);
  io.reached=true;update_squirrel_placement(s,20,30,false,io);
  assert(s.phase==3&&s.target_index==0);
  update_squirrel_placement(s,20,30,false,io);
  assert(s.phase==5&&s.round_counts[0]==1&&s.written_values[0]==4);
  assert(s.selection_values[0]==-1&&!io.sounds.empty());
  update_squirrel_placement(s,20,30,false,io);assert(s.phase==0);
  // Choose disabled source actor, causing retrieval/return 2->4->6->7->8->5.
  update_squirrel_placement(s,120,130,true,io);assert(s.phase==2);
  update_squirrel_placement(s,120,130,false,io);assert(s.phase==4);
  update_squirrel_placement(s,120,130,false,io);assert(s.phase==6);
  update_squirrel_placement(s,120,130,false,io);assert(s.phase==7);
  update_squirrel_placement(s,120,130,false,io);assert(s.phase==8);
  update_squirrel_placement(s,120,130,false,io);assert(s.phase==5);
  s.phase=0;s.points=8;s.placement_round=9;s.placed_capacity=4;
  update_squirrel_placement(s,0,0,false,io);
  assert(s.celebration==1&&io.stopped==1);
 }
 {
  PilIO io;PilchardState s;
  s.phase=0;s.wait_frames=1;s.screen=0;s.previous_screen=0;
  s.destination_node=3;s.difficulty=0;
  update_spud_maze_pilchard(s,io);assert(s.phase==1);
  update_spud_maze_pilchard(s,io);
  assert(s.phase==2&&s.current_node==0&&io.path_calls==1&&io.sounds==1);
  io.hit=true;update_spud_maze_pilchard(s,io);
  assert(s.phase==2&&s.collision_flag==1);
  io.hit=false;
  update_spud_maze_pilchard(s,io);
  assert(s.phase==2&&s.path_cursor==1);
  update_spud_maze_pilchard(s,io);
  assert(s.phase==4&&io.advance_calls==2);
  update_spud_maze_pilchard(s,io);
  assert(s.phase==5&&s.destination_node>=0&&s.traversal_speed==4);
  s.collision_flag=1;update_spud_maze_pilchard(s,io);
  assert(s.phase==6);
  update_spud_maze_pilchard(s,io);
  assert(s.phase==7);
  update_spud_maze_pilchard(s,io);
  update_spud_maze_pilchard(s,io);
  assert(s.phase==0&&s.wait_frames==500);
  s.phase=3;s.previous_screen=4;s.screen=0;s.screen_return_frames=0;
  update_spud_maze_pilchard(s,io);
  assert(s.phase==1&&s.collision_flag==0);
 }
 {
  FireworksState s;MovieIO io;
  s.runtime.reset();
  assert(s.authored_grid.commit_placement(0,0,btb::fireworks::FireworkType::RedAirbomb));
  io.elapsed=0;io.crowd_done=true;io.rng=0;
  update_fireworks_show(s,io);
  assert(s.runtime.crowd_phase()==1&&io.restarts==1);
  assert(io.draws==1&&io.launched.size()==1);
  assert(io.sound_ids.size()==1&&io.sound_ids[0]==0x143+1);
  // Free a previously launched event before admitting subsequent launches.
  io.elapsed=100;io.active_done=true;io.rng=5;
  update_fireworks_show(s,io);
  assert(s.runtime.crowd_phase()==2&&io.restarts==2);
  assert(io.rewound.size()==1);
  io.elapsed=200;update_fireworks_show(s,io);
  assert(s.runtime.crowd_phase()==3&&io.restarts==3);
  io.elapsed=900;io.crowd_done=true;io.end_sound=false;
  update_fireworks_show(s,io);
  assert(s.current_internal_state==16&&s.runtime.crowd_phase()==7);
  assert(io.movie_seen==22&&io.end_resets==1&&io.end_stops==1);
  assert(io.stops>=2&&io.overlays>=1);
  bool blocked=false;
  try{update_fireworks_show(s,io);}catch(const std::logic_error&){blocked=true;}
  assert(blocked);
 }
}
