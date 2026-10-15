#include "btb/original_maze_init2.hpp"
#include "btb/spud_maze_runtime.hpp"
#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <utility>
namespace btb::original_maze_init2 {
namespace {
void register_surface(InitState& st,IO& io,U32 addr,std::string_view file,bool keyed=false,U32 key=0xff00ff){
 if(file.empty())throw std::runtime_error("unresolved original Maze surface path");
 const U32 handle=io.load_registered_surface(addr,file);
 if(handle==0)throw std::runtime_error("missing original Maze surface");
 st.surfaces.push_back({addr,handle,keyed,key});
 if(keyed)io.color_key_surface(addr,handle,key);
}
void clear_init(InitState& s){
 s.surfaces.clear();
 s.actors.fill({-1,1});
 s.initialized=0;s.tracked_actors=0;s.repairs_remaining=0;
 s.damaged.fill(false);
 s.timer=300;s.startup_flag=0;
}
void choose_distinct(InitState& s,IO& io,int group,int n){
 // The executable seeds each 4-slot actor group with rand()%4 and
 // rejects a duplicate until the required difficulty's active slots fill.
 if(n<0||n>4||group<0||group>=5)throw std::invalid_argument("bad Maze actor group");
 std::array<bool,4> used{};
 for(int k=0;k<n;++k){
  int v=-1;
  for(int attempt=0;attempt<1024;++attempt){
   const auto trial=static_cast<int>(io.next_original_rand()%4);
   if(!used[static_cast<std::size_t>(trial)]){v=trial;break;}
  }
  if(v<0)throw std::runtime_error("original Maze PRNG did not provide distinct actor spawn");
  used[static_cast<std::size_t>(v)]=true;
  s.actors[static_cast<std::size_t>(group*4+k)].node=v;
  ++s.tracked_actors;
 }
}
void begin_clock(InitState& s,IO& io){
 const auto t=io.system_filetime_100ns();
 s.start_clock_100ns=t;s.last_clock_100ns=io.system_filetime_100ns();
 s.start_clock_100ns_low=static_cast<U32>(t);
 s.start_clock_100ns_high=static_cast<U32>(t>>32);
}
constexpr std::array<std::pair<U32,std::string_view>,7> kMazeSprites{{
 {0x0051210cU,R"(Data\SubGameMaze\player.bmp)"},
 {0x00512110U,R"(Data\SubGameMaze\spud.bmp)"},
 {0x00512120U,R"(Data\SubGameMaze\numerals.bmp)"},
 {0x00512114U,R"(Data\SubGameMaze\box.bmp)"},
 {0x00512118U,R"(Data\SubGameMaze\boxghost.bmp)"},
 {0x0051211cU,R"(Data\SubGameMaze\smallbox.bmp)"},
 {0x005144d0U,R"(Data\SubGameSpudMaze\timer.bmp)"}
}};
constexpr std::array<std::pair<U32,std::string_view>,10> kSpudSprites{{
 {0x005144f8U,R"(Data\SubGameSpudMaze\repair.bmp)"},
 {0x005144fcU,R"(Data\SubGameSpudMaze\pilchard.bmp)"},
 {0x0051450cU,R"(Data\SubGameMaze\numerals.bmp)"},
 {0x00514500U,R"(Data\SubGameMaze\box.bmp)"},
 {0x00514504U,R"(Data\SubGameSpudMaze\toolbarhammer.bmp)"},
 {0x00514508U,R"(Data\SubGameSpudMaze\toolbarhammerdep.bmp)"},
 {0x00514494U,R"(Data\SubGameSpudMaze\hammer.bmp)"},
 {0x005144d0U,R"(Data\SubGameSpudMaze\timer.bmp)"},
 {0x005144b8U,R"(Data\SubGameSpudMaze\bentley.bmp)"},
 {0x005144c4U,R"(Data\SubGameSpudMaze\screenborder.bmp)"}
}};
void common_begin(InitState& s,IO& io,bool spud){
 if(s.difficulty<0||s.difficulty>2)
  throw std::out_of_range("original Maze difficulty must be 0..2");
 clear_init(s);
 io.install_unload(spud);
 io.reset_sound_manager();
 if(!spud)io.create_maze_engine_sound(R"(Data\SubGameMaze\lofty_engine.wav)");
 io.set_volume((s.music_volume-100)*0x2a);
}
}
void initialize_maze(InitState& s,IO& io){
 common_begin(s,io,false);
 s.maze_data=io.read_original_maze_data();
 for(const auto& screen:s.maze_data.screens)
  if(screen.nodes.empty())throw std::runtime_error("source Maze graph missing screen");
 // Original filenames are successive 0x104-byte strings in PE .rdata.
 for(std::size_t i=0;i<3;++i)
  register_surface(s,io,0x00512100U+static_cast<U32>(i*4),io.maze_background_path(i));
 for(const auto& [addr,path]:kMazeSprites)
  register_surface(s,io,addr,path,true,addr==0x00512120U?0:0xff00ffU);
 s.mode=8;s.actor_stride=4;s.player_x=247.f;s.player_y=267.f;s.timer=300;
 static constexpr std::array<std::array<int,3>,3> count{{{{1,2,1}},{{3,3,2}},{{4,4,4}}}};
 for(int group=0;group<3;++group)
  choose_distinct(s,io,group,count[static_cast<std::size_t>(s.difficulty)][static_cast<std::size_t>(group)]);
 s.target_count=s.maze_data.timer_by_difficulty[static_cast<std::size_t>(s.difficulty)];
 s.player_count=s.tracked_actors;
 begin_clock(s,io);s.startup_flag=1;s.initialized=1;
}
void initialize_spud_maze(InitState& s,IO& io){
 common_begin(s,io,true);
 s.spud_data=io.read_original_spud_maze_data();
 for(const auto& screen:s.spud_data.screens)
  if(screen.empty())throw std::runtime_error("source Spud Maze graph missing screen");
 for(std::size_t i=0;i<5;++i)
  register_surface(s,io,0x005144e4U+static_cast<U32>(i*4),io.spud_background_path(i));
 for(const auto& [addr,path]:kSpudSprites)
  register_surface(s,io,addr,path,true,addr==0x0051450cU?0:0xff00ffU);
 // Original 32 sprites are indexed by (screen, repair ordinal), in screen
 // order, with 005141EC as the first bitmap registration slot.
 std::size_t sprite=0;
 for(std::size_t screen=0;screen<spud_maze::kScreenCount;++screen){
  if(s.spud_data.repair_points[screen].size()!=spud_maze::kRepairPointCounts[screen])
   throw std::runtime_error("Spud Maze repaired nodes do not match original 32-spot census");
  for(std::size_t i=0;i<spud_maze::kRepairPointCounts[screen];++i,++sprite)
   register_surface(s,io,0x005141ecU+static_cast<U32>(sprite*4),io.repair_sprite_path(screen,i),true);
 }
 s.repair_seed=static_cast<I32>(io.next_original_rand()%4+1);
 const auto repair=spud_maze::select_repair_damage(static_cast<spud_maze::Difficulty>(s.difficulty),s.repair_seed);
 s.damaged=repair.damaged;s.repairs_remaining=repair.repairs_remaining;
 s.player_count=repair.repairs_remaining;
 s.player_x=0.f;s.player_y=0.f;s.mode=2;
 // Source Ghidra actor table is five 4-entry groups; start-node
 // constraints per difficulty are recovered for the shared first groups.
 static constexpr std::array<std::array<int,5>,3> count{{{{1,2,1,1,1}},{{2,3,2,2,1}},{{4,4,4,4,4}}}};
 for(int group=0;group<5;++group)
  choose_distinct(s,io,group,count[static_cast<std::size_t>(s.difficulty)][static_cast<std::size_t>(group)]);
 s.target_count=s.spud_data.timer_values[static_cast<std::size_t>(s.difficulty)];
 begin_clock(s,io);s.startup_flag=1;s.initialized=1;
}
}
