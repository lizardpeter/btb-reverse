#include "btb/original_last2.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
namespace btb::original_last2 {
namespace {
using Mode=park_designer::EditorMode;
using Control=park_designer::EditorControl;
using StateIndex=park_designer::TrailingStateIndex;
I32& state(park_designer::SaveData& d,StateIndex i){return d.state(i);}
void surface(BandInit& b,BandInitIO& io,U32 slot,I32 group,I32 index,
             U32 source_va,bool keyed){
 const U32 handle=io.load_original_band_bitmap(slot,group,index,source_va);
 if(handle==0)throw std::runtime_error("original Bob's Band bitmap unavailable");
 b.resources.push_back({slot,source_va,handle,keyed});
 if(keyed)io.key_original_bitmap(slot,handle,0x00ff00ffU);
}
}
void update_park_editor_0040e440(ParkEditor& s,ParkEditorIO& io){
 if(!s.save)throw std::invalid_argument("Park editor requires original SaveData");
 auto& save=*s.save;
 I32 mode=state(save,StateIndex::ActiveEditorMode);
 if(mode<0||mode>3)throw std::out_of_range("original Park editor mode 0..3");
 auto current=park_designer::kCurrentPageStateByEditorMode[static_cast<std::size_t>(mode)];
 auto maximum=park_designer::kMaxPageStateByEditorMode[static_cast<std::size_t>(mode)];
 I32& page=state(save,current);
 const I32 max=std::max(0,state(save,maximum));
 page=std::clamp(page,0,max);
 io.draw_original_scene(save);
 if(mode==3)io.process_original_view_controls(s);
 io.draw_original_toolbar(mode,page);
 const I32 hit=io.original_toolbar_hit_test(s.mouse_x,s.mouse_y);
 // Ghidra 0040E440: original toolbar hit controls 5/6 scroll, 7..10 modes,
 // 11 Delete All/quit confirmation, and 0..4 select palette brick.
 if(s.click==1&&s.mouse_latch==0){
  s.old_control=hit;
  if(hit>=7&&hit<=10){
   mode=hit-7;
   state(save,StateIndex::ActiveEditorMode)=mode;
   s.mode_changed=true;s.voice_latch=1;
   s.selected_palette=-1;
   io.set_original_cursor(0x00507a00U);
   s.active_cursor_slot=0x00507a00U;
   current=park_designer::kCurrentPageStateByEditorMode[static_cast<std::size_t>(mode)];
   maximum=park_designer::kMaxPageStateByEditorMode[static_cast<std::size_t>(mode)];
  }else if(hit==5){
   if(page>0&&!(mode<=1&&page==1)){
    --page;s.scroll_offset=0x3d;
   }
  }else if(hit==6){
   if(page<max){++page;s.scroll_offset=-0x3d;}
  }else if(hit==11){
   s.delete_requested=1;
   io.process_original_delete(s);
  }else if(hit>=0&&hit<5&&mode!=3){
   s.selected_palette=hit;
   io.process_original_palette_and_placement(s);
  }
  s.mouse_latch=1;
 }else if(s.click==0){
  s.mouse_latch=0;
 }
 mode=state(save,StateIndex::ActiveEditorMode);
 if(mode<0||mode>3)throw std::out_of_range("Park mode changed out of range");
 current=park_designer::kCurrentPageStateByEditorMode[static_cast<std::size_t>(mode)];
 maximum=park_designer::kMaxPageStateByEditorMode[static_cast<std::size_t>(mode)];
 I32& final_page=state(save,current);
 const I32 final_max=std::max(0,state(save,maximum));
 final_page=std::clamp(final_page,0,final_max);
 // Original changes vertical page animation in five-unit steps until zero.
 if(s.scroll_offset<0)s.scroll_offset=std::min(0,s.scroll_offset+5);
 else if(s.scroll_offset>0)s.scroll_offset=std::max(0,s.scroll_offset-5);
 io.draw_original_palette(mode,final_page,s.scroll_offset);
 s.previous_arrow=final_page>0&&!(mode<=1&&final_page==1);
 s.next_arrow=final_page<final_max;
 io.draw_original_page_arrow(s.previous_arrow,s.next_arrow);
 // Source latches the first-time context voice (DAT_00441F18), with
 // 0x112 for the initially empty Pond and 0xC9 for empty Bandstand.
 if(s.voice_latch==1){
  s.voice_latch=0;
  if(mode==0&&final_page==0&&
     state(save,StateIndex::PondSelectedPrimary)==-1){
   io.play_original_editor_voice(0x112,0);s.last_voice=0x112;
  }else if(mode==1&&final_page==0&&
           state(save,StateIndex::BandstandSelectedRecord)==-1){
   io.play_original_editor_voice(0xc9,0);s.last_voice=0xc9;
  }
 }
 s.season=state(save,StateIndex::Season);
}

void initialize_bobs_band_0041db20(BandInit& b,BandInitIO& io){
 if(b.conductor<0||b.conductor>=3||b.player_profile<0||b.player_profile>20)
  throw std::out_of_range("original Bob's Band conductor/profile");
 const I32 conductor=b.conductor,player=b.player_profile,volume=b.global_volume;
 b=BandInit{};
 b.conductor=conductor;b.player_profile=player;b.global_volume=volume;
 io.initialize_original_crt(); // FUN_00430F00 first in original 0041DB20
 b.machine_data=io.load_original_machine_data();
 io.register_original_band_unload();b.unload_registered=true;
 b.machine_states.fill(0);b.conductor_states.fill(0);
 b.retail_sequence.fill(-1);
 // Original 0x508C0C Delete cursor, magenta keyed.
 surface(b,io,0x00508c0cU,0,0,0x00442500U,true);
 // Source 0x0051276C: eight consecutive pointer slots (Stack local path
 // family at 0x32-byte stride), no explicit color-key registration.
 for(I32 i=0;i<8;++i)
  surface(b,io,0x0051276cU+U32(i)*4U,1,i,0,false);
 // Source 0x00512718: ten original Roley/Muck/Lofty/Dizzy/Scoop
 // 1-second and 2-second texture sheets, all magenta keyed.
 for(I32 i=0;i<10;++i)
  surface(b,io,0x00512718U+U32(i)*4U,2,i,0,true);
 // Three conductor stand images, selected by DAT_0051C284 with 0x3C stride.
 surface(b,io,0x005125a8U,3,conductor,0,true);
 // 120 32-bit entries from profile/conductor-specific
 // musicbob1.txt family in original 0041DB20.
 b.retail_sequence=io.load_original_music_sequence(conductor,player);
 b.sequence_loaded=true;
 // Original source loads background and toolbar separately.
 surface(b,io,0x00513f1cU,4,0,0x00445040U,false);
 surface(b,io,0x00513f20U,4,1,0x00445020U,true);
 // Ten first-sound and ten second-sound foreground/toolbar resources.
 for(I32 i=0;i<10;++i){
  surface(b,io,0x00512390U+U32(i)*4U,5,i,0,true);
  surface(b,io,0x00512744U+U32(i)*4U,6,i,0,true);
 }
 // Retail source at 0041F4xx initializes machine editor slots with
 // DAT_00444BE8 sprite and DAT_00444C38 additional atlas addresses.
 for(I32 i=0;i<10;++i){
  auto& slot=b.editor_slots[static_cast<std::size_t>(i)];
  slot.machine_id=i;slot.sprite_x=static_cast<I32>(io.original_machine_editor_sprite(i));
  slot.sprite_y=static_cast<I32>(io.original_machine_sequence_sprite(i));
  slot.timer=100;
  for(const auto& x:b.resources)
   if(x.slot==0x00512390U+U32(i)*4U){slot.surface=x.handle;break;}
  io.register_original_active_machine(slot,i);
 }
 // Source DAT_0051279C grid has 5 rows x 24 one-second slots.
 // Native original coordinate globals DAT_00444B04/08/0C/10 supplied.
 const I32 x0=io.original_machine_grid_x(),y0=io.original_machine_grid_y();
 const I32 dx=io.original_machine_grid_dx(),dy=io.original_machine_grid_dy();
 for(I32 row=0;row<5;++row)for(I32 col=0;col<24;++col){
  const std::size_t i=static_cast<std::size_t>(row*24+col);
  b.grid[i]={b.retail_sequence[i],x0+col*dx,y0+row*dy,0,0xb};
 }
 b.original_grid_total=120;
 b.active_music_id=conductor*0xd+0x1fe; // DAT_00512740
 io.apply_original_global_volume((volume-100)*0x2a);
 b.active_game_music_flag=1;b.initialized=true;
}
} // namespace btb::original_last2
