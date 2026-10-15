#include "btb/original_maze_init2.hpp"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_maze_init2;
struct Host:IO{
 std::vector<U32> address,keyed;
 std::vector<std::string> events;
 std::size_t load_count{},rand_count{},clock_count{},background_count{},repair_count{};
 U32 next_rand{0},fail_address{0};
 I32 applied_volume{};
 bool invalid_repair{},unknown_file{};
 void install_unload(bool spud) override{events.push_back(spud?"unload-spud":"unload-maze");}
 maze::Data read_original_maze_data() override {
  events.push_back("maze-data");
  maze::Data v{};
  for(auto& graph:v.screens){graph.nodes.push_back({});}
  v.timer_by_difficulty={90,80,70};return v;
 }
 spud_maze::Data read_original_spud_maze_data() override {
  events.push_back("spud-data");
  spud_maze::Data v{};
  for(std::size_t i=0;i<5;++i){
   v.screens[i].push_back({});
   v.repair_points[i].resize(spud_maze::kRepairPointCounts[i]);
  }
  if(invalid_repair)v.repair_points[2].clear();
  v.timer_values={50,60,70};return v;
 }
 void reset_sound_manager() override{events.push_back("reset-sound");}
 void create_maze_engine_sound(std::string_view path) override{
  assert(path=="Data\\SubGameMaze\\lofty_engine.wav");
  events.push_back("sound");}
 void set_volume(I32 v) override{applied_volume=v;events.push_back("volume");}
 U32 load_registered_surface(U32 slot,std::string_view path) override{
  assert(!path.empty());++load_count;address.push_back(slot);
  events.push_back("bitmap");if(slot==fail_address)return 0;
  return 0x1000+static_cast<U32>(load_count);
 }
 void color_key_surface(U32 slot,U32 handle,U32 rgb) override {
  assert(handle>=0x1000);
  if(slot==0x00512120U||slot==0x0051450cU)assert(rgb==0);
  else assert(rgb==0xff00ff);
  keyed.push_back(slot);
 }
 std::string_view maze_background_path(std::size_t i) override{
  assert(i<3);++background_count;return "Data\\SubGameMaze\\LEFT_MAZE.bmp";}
 std::string_view spud_background_path(std::size_t i) override{
  assert(i<5);++background_count;return "Data\\SubGameSpudMaze\\screen1.bmp";}
 std::string_view repair_sprite_path(std::size_t s,std::size_t i) override{
  assert(s<5&&i<spud_maze::kRepairPointCounts[s]);++repair_count;
  return unknown_file&&i==0?std::string_view{}:std::string_view("Data\\SubGameSpudMaze\\0_0.bmp");
 }
 U32 next_original_rand() override{++rand_count;return next_rand++;}
 std::uint64_t system_filetime_100ns() override{
  return 0x1234567800000000ULL+++clock_count*10000000ULL;
 }
};
static bool group_distinct(const InitState& s,int group,int n){
 std::array<bool,4> used{};
 for(int i=0;i<n;++i){
  const auto v=s.actors[static_cast<std::size_t>(group*4+i)].node;
  if(v<0||v>3||used[static_cast<std::size_t>(v)])return false;
  used[static_cast<std::size_t>(v)]=true;
 }
 for(int i=n;i<4;++i)
  if(s.actors[static_cast<std::size_t>(group*4+i)].node!=-1)return false;
 return true;
}
int main(){
 for(int difficulty=0;difficulty<3;++difficulty){
  Host io;InitState state{};
  state.difficulty=difficulty;state.music_volume=80;
  initialize_maze(state,io);
  assert(state.initialized==1&&state.startup_flag==1);
  assert(state.surfaces.size()==10&&io.address.size()==10);
  assert(io.keyed.size()==7&&io.background_count==3);
  assert(state.surfaces.front().address==0x00512100U);
  assert(io.address[3]==0x0051210cU);
  assert(io.applied_volume==-840);
  assert(state.mode==8&&state.timer==300);
  assert(state.player_x==247.f&&state.player_y==267.f);
  const int groups[3][3]={{1,2,1},{3,3,2},{4,4,4}};
  int expected=0;
  for(int group=0;group<3;++group){
   expected+=groups[difficulty][group];
   assert(group_distinct(state,group,groups[difficulty][group]));
  }
  assert(state.tracked_actors==expected&&state.player_count==expected);
  assert(state.target_count==90-difficulty*10);
  assert(state.start_clock_100ns_high==0x12345678);
  assert(state.last_clock_100ns>state.start_clock_100ns);
  assert(io.events[0]=="unload-maze"&&io.events[1]=="reset-sound");
  assert(io.events[2]=="sound");
 }
 for(int difficulty=0;difficulty<3;++difficulty){
  Host io;InitState state{};state.difficulty=difficulty;
  initialize_spud_maze(state,io);
  assert(state.initialized&&state.mode==2&&state.startup_flag);
  assert(state.surfaces.size()==5+10+32);
  assert(io.address.size()==47&&io.keyed.size()==42);
  assert(io.address[0]==0x005144e4U&&io.address[5]==0x005144f8U);
  assert(io.address[15]==0x005141ecU);
  assert(io.repair_count==32&&io.background_count==5);
  assert(state.repair_seed==1&&io.rand_count>0);
  assert(state.repairs_remaining==(difficulty==0?4:difficulty==1?6:11));
  assert(state.player_count==state.repairs_remaining);
  assert(state.target_count==50+difficulty*10);
  const int groups[3][5]={{1,2,1,1,1},{2,3,2,2,1},{4,4,4,4,4}};
  for(int group=0;group<5;++group)
   assert(group_distinct(state,group,groups[difficulty][group]));
  assert(io.events[0]=="unload-spud"&&io.events[1]=="reset-sound");
  assert(io.events[2]=="volume");
 }
 {
  Host io;InitState s{};s.difficulty=3;
  bool rejected=false;
  try{initialize_maze(s,io);}catch(const std::out_of_range&){rejected=true;}
  assert(rejected&&io.events.empty());
 }
 {
  Host io;InitState s{};io.fail_address=0x0051210cU;
  bool rejected=false;
  try{initialize_maze(s,io);}catch(const std::runtime_error&){rejected=true;}
  assert(rejected&&!s.initialized&&io.address.size()==4);
 }
 {
  Host io;InitState s{};io.invalid_repair=true;
  bool rejected=false;
  try{initialize_spud_maze(s,io);}catch(const std::runtime_error&){rejected=true;}
  assert(rejected&&!s.initialized&&io.repair_count<32);
 }
 {
  Host io;InitState s{};io.unknown_file=true;
  bool rejected=false;
  try{initialize_spud_maze(s,io);}catch(const std::runtime_error&){rejected=true;}
  assert(rejected&&!s.initialized);
 }
}
