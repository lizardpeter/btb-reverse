#include "btb/original_herding_init.hpp"
#include <array>
#include <stdexcept>
namespace btb::original_herding_init {
namespace {
constexpr U32 kMagenta=0x00ff00ffU;
struct Resource {U32 slot,name;bool keyed;};
constexpr std::array<Resource,18> kOriginalResources{{
 {0x00510724U,0x00443d40U,false}, // background
 {0x0051072cU,0x00443d24U,true},  // duck
 {0x00510730U,0x00443d0cU,true},  // rabbit
 {0x00510734U,0x00443cecU,true},  // sheep shadow
 {0x00510738U,0x00443cc4U,true},  // Scrufty
 {0x0051073cU,0x00443ca8U,true},  // duck bag
 {0x00510740U,0x00443c8cU,true},  // rabbit bag
 {0x00510744U,0x00443c70U,true},  // sheep bag
 {0x00510748U,0x00443c50U,true},  // UI surround
 {0x0051074cU,0x00443c34U,true},  // Travis cab
 {0x00510750U,0x00443c18U,true},  // trailer1
 {0x00510754U,0x00443bfcU,true},  // trailer2
 {0x00510758U,0x00443be0U,true},  // left gate
 {0x0051075cU,0x00443bc4U,true},  // right gate
 {0x005105a0U,0x00443ba4U,true},  // duck toolbar
 {0x005105a4U,0x00443b84U,true},  // rabbit toolbar
 {0x005105a8U,0x00443b64U,true},  // sheep toolbar
 {0x00510728U,0x00443b40U,true}   // Pickles
}};
U32 get_bitmap(const State& s,U32 slot){
 for(const auto& b:s.bitmaps)if(b.guest_slot==slot)return b.handle;
 throw std::logic_error("required original Herding bitmap not registered");
}
void append_entity(State& st,I32 id,I32 x,I32 y,I32 facing,I32 type,
                   I32 width,I32 height,U32 source_slot,IO& io){
 if(st.entities.size()>=200)throw std::out_of_range("original Herding entity table capacity");
 Entity a{};
 a.id=id;a.x=x;a.y=y;a.facing=facing;a.type=type;
 a.width=width;a.height=height;
 a.precise_x=static_cast<float>(x);a.precise_y=static_cast<float>(y);
 a.heading_radians=io.original_angle_to_radians(facing);
 a.bitmap=get_bitmap(st,source_slot);
 st.entities.push_back(a);
}
}
void initialize_herding(State& s,IO& io){
 // The shipped initialization resets difficulty when the replay flag is 1.
 const I32 input_difficulty=s.difficulty,playermode=s.player_mode,volume=s.volume;
 const I32 difficulty=playermode==1?0:input_difficulty;
 if(difficulty<0||difficulty>2)throw std::out_of_range("retail Herding difficulty 0..2");
 s=State{};s.difficulty=difficulty;s.player_mode=playermode;s.volume=volume;
 s.followers.fill(-1);
 s.retained_indices.fill(-1);
 s.active_target_indices.fill(-1);
 s.common_overlay.fill(0);
 s.pen_status={{1,1,1}};
 s.selected_pen=-1;
 io.register_original_unload();
 io.set_global_game_volume((s.volume-100)*0x2a);
 s.data=io.load_original_coordinate_data();
 if(s.data.coordinate_groups.size()<4||
    s.data.coordinate_groups[0].empty()||
    s.data.coordinate_groups[1].empty()||
    s.data.coordinate_groups[2].empty()||
    s.data.coordinate_groups[3].empty())
  throw std::runtime_error("Herding shipped sentinel-delimited coordinates missing");
 // Original 00419600 calls LoadBitmapToDirectDrawSurface and
 // RegisterBitmapSurface for these exact eighteen slots. All except
 // background are keyed with RGB 0xff00ff.
 for(const auto& asset:kOriginalResources){
  const U32 handle=io.load_registered_bitmap(asset.slot,asset.name);
  if(handle==0)throw std::runtime_error("original Herding bitmap missing");
  s.bitmaps.push_back({asset.slot,asset.name,handle,asset.keyed});
  if(asset.keyed)io.key_bitmap(asset.slot,handle,kMagenta);
 }
 // DAT_005101E0 / E4 correspond to fixed pair #3 of herd.txt.
 const auto& start=s.data.setup_positions[herding::kFarmerPicklesSetupPositionIndex];
 append_entity(s,0,start.x,start.y,0,0,0x66,0x5f,0x00510728U,io);
 // Source initial spawn: one Pickles record followed by three animal
 // groups of (difficulty+3), each with rand()%8 facing.
 I32 x=600,y=0x226;
 for(I32 group=0;group<3;++group)
  for(I32 number=0;number<3+difficulty;++number){
   const I32 facing=static_cast<I32>(io.original_random32()%8U);
   const U32 animal_surface=group==0?0x00510734U:
                            group==1?0x00510730U:0x0051072cU;
   append_entity(s,static_cast<I32>(s.entities.size()),x,y,facing,
                 group+1,0x66,0x5f,animal_surface,io);
   ++s.leader_count;
   x+=0x96;
   if(x>0x44c){x=100;y+=100;}
  }
 s.herd_remaining=s.leader_count;
 // Source registers static background objects with the following coords
 // and type ids. Gate slots are stored separately for runtime movement.
 append_entity(s,static_cast<I32>(s.entities.size()),0,300,0,13,
               230,122,0x0051074cU,io);
 append_entity(s,static_cast<I32>(s.entities.size()),230,300,0,12,
               200,100,0x00510750U,io);
 append_entity(s,static_cast<I32>(s.entities.size()),400,325,0,14,
               45,60,0x00510754U,io);
 s.gate_left_index=static_cast<I32>(s.entities.size());
 append_entity(s,s.gate_left_index,0x2b9,0x8e,0,15,
               0x78,0x46,0x00510758U,io);
 s.gate_right_index=static_cast<I32>(s.entities.size());
 append_entity(s,s.gate_right_index,0x347,0x51,0,16,
               0x78,0x46,0x0051075cU,io);
 // herd.txt group 0 is transformed (-64,-100) in the original before
 // polygon clipping; source then reads the following sentinel groups.
 s.original_polygon=s.data.retail_transformed_group0();
 s.guide_route=s.data.coordinate_groups[1];
 s.roaming_path=s.data.coordinate_groups[2];
 s.finish_marker=s.data.coordinate_groups[3].front();
 s.guide_count=static_cast<I32>(s.guide_route.size());
 if(difficulty>0) {
  const auto& pos=s.guide_route.front();
  append_entity(s,static_cast<I32>(s.entities.size()),pos.x,pos.y,0,7,
                0x65,0x74,0x00510738U,io);
 }
 s.waypoint_index=0;s.return_state=0;
 s.music_start_pending=1;s.startup_flag=1;
 s.initialized=true;
}
} // namespace btb::original_herding_init
