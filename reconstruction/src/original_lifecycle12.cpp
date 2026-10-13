#include "btb/original_lifecycle12.hpp"
namespace btb::original_lifecycle12 {
void unload_fireworks_editor(ActivityResources& s){
 for(U32 a:{0x0050ab18U,0x0050ab1cU,0x0050aaa8U,0x0050ab08U})
  s.bitmap(a);
 s.bitmap_range(0x0050a49cU,12);
}
void unload_fireworks_movie_bank(ActivityResources& s){
 for(U32 i=0;i<23;++i){
  const U32 movie=0x0050aaacU+i*4U;
  const U32 surface=0x0050937cU+i*4U;
  const U32 bytes=0x0050a50cU+i*4U;
  s.emit(Op::CloseBink,movie,static_cast<I32>(s.handle(movie)));
  s.bitmap(surface);
  s.emit(Op::FreeMovieBuffer,bytes,static_cast<I32>(s.handle(bytes)));
  s.slot[movie]=0;
 }
 for(U32 a:{0x0050a4b4U,0x0050a5ccU,0x0050a5c4U,0x0050a5c8U})
  s.bitmap(a);
}
void unload_dino(ActivityResources& s,I32 surfaces){
 for(U32 a:{0x004fc9e8U,0x0050a5c4U,0x0050a5c8U})s.bitmap(a);
 for(I32 i=0;i<surfaces;++i)
  s.bitmap(0x004fc9ecU+static_cast<U32>(i)*4U);
 s.sound_slots();
 for(U32 a:{0x004fc2acU,0x004fc434U,0x00508ad0U})s.bitmap(a);
 s.emit(Op::ResetCursor);s.finish_music();
}
void unload_herding(ActivityResources& s){
 for(U32 a:{0x00510724U,0x00510728U})s.bitmap(a);
 s.sound_slots();
 for(U32 a:{0x0051072cU,0x00510730U,0x00510734U,
            0x0051073cU,0x00510740U,0x00510744U,
            0x00510738U,0x00510748U,0x0051074cU,
            0x00510750U,0x00510754U,0x00510758U,0x0051075cU})
  s.bitmap(a);
 s.bitmap_range(0x005105a0U,3);
 s.herding_character_latch_a=0;s.herding_character_latch_b=0;
 s.finish_music();
}
void unload_maze(ActivityResources& s){
 s.bitmap_range(0x00512100U,3);
 for(U32 a:{0x0051210cU,0x00512110U,0x00512120U,
            0x00512114U,0x00512118U,0x0051211cU,0x005144d0U})
  s.bitmap(a);
 s.sound_slots();s.finish_music();s.shared_ui_latch=0;
}
void unload_golf(ActivityResources& s){
 s.skate_input_blocked=1;
 s.bitmap(0x0050aedcU);
 for(U32 i=0;i<3;++i){
  s.bitmap(0x0050ab88U+i*4U);
  s.bitmap(0x0050adfcU+i*4U);
  s.bitmap(0x0050addcU+i*4U);
 }
 for(U32 a:{0x0050aee0U,0x0050aee4U,0x0050aee8U,
            0x0050aef0U,0x0050aef4U,0x0050aef8U,
            0x0050adecU,0x0050aeecU})
  s.bitmap(a);
 s.sound_slots();s.emit(Op::ResetCursor);s.finish_music();
}
void unload_spud_skate(ActivityResources& s){
 s.skate_input_blocked=1;
 s.bitmap(0x00514ea0U);s.sound_slots();s.audio_latch=0;
 for(U32 i=0;i<5;++i){
  const U32 movie=0x00514b2cU+i*4U;
  const U32 surface=0x00514e8cU+i*4U;
  const U32 allocation=0x00514620U+i*4U;
  s.emit(Op::CloseBink,movie,static_cast<I32>(s.handle(movie)));
  s.bitmap(surface);
  s.emit(Op::FreeMovieBuffer,allocation,
        static_cast<I32>(s.handle(allocation)));
 }
 for(U32 a:{0x005148b4U,0x00514b60U,0x00514b64U,0x00514b68U})
  s.bitmap(a);
}
void unload_squirrel(ActivityResources& s){
 for(U32 a:{0x00515068U,0x0051506cU,0x00515070U,0x00515074U,
            0x0051509cU,0x005150a0U,0x005150a4U,0x005150a8U,
            0x005150acU,0x005150b0U,0x005150b4U,0x005150b8U,
            0x005150bcU,0x005150c0U,0x005150c4U,0x005150c8U})
  s.bitmap(a);
 s.sound_slots();s.emit(Op::ResetCursor);s.finish_music();
}
bool unload_spud_maze(ActivityResources& s,const std::array<I32,5>& counts){
 for(const I32 count:counts)if(count<0||count>11)return false;
 s.bitmap_range(0x005144e4U,5);
 for(U32 a:{0x005144f8U,0x005144fcU,0x0051450cU,
            0x00514500U,0x00514504U,0x00514508U,
            0x00514494U,0x005144d0U,0x005144b8U,0x005144c4U})
  s.bitmap(a);
 s.sound_slots();
 for(U32 i=0;i<5;++i)
  s.bitmap_range(0x005141ecU+i*44U,static_cast<U32>(counts[i]));
 s.finish_music();return true;
}
void begin_fireworks_action(
 FireState& s,I32 control,I32 row,I32 col,U32 random,
 const RemoveCell& remove){
 if(control>=0&&control<12){
  s.cursor_type=control;s.emit(FireAction::CursorFirework,control);
  if(control<8){
   if(s.bob_actor_state==0){
    s.emit(FireAction::Sound,862+static_cast<I32>(random%5U),50,2);
    s.selected_type=control;s.bob_actor_state=1;
   }
  }else if(s.wendy_actor_state==0){
   s.emit(FireAction::Sound,899+static_cast<I32>(random%5U),50,2);
   s.selected_type=control;s.wendy_actor_state=1;
  }
  return;
 }
 if(control==12){
  if(row>=0&&row<3&&col>=0&&col<6){
   const I32 fire=s.cells[static_cast<std::size_t>(row*6+col)];
   if(fire>=0&&fire<12){
    s.emit(FireAction::RemoveCell,row,col);
    if(remove)remove(row,col);
   }
  }
 }else if(control==25){
  s.open_delete_all=true;s.emit(FireAction::DeleteAllConfirm);
 }else if(control==27){
  s.editor_state=14;s.emit(FireAction::StartShow);
 }
}
void complete_fireworks_action(
 FireState& s,I32 control,I32 row,I32 col,U32 random,
 const PlaceCell& place){
 if(control>=0&&control<12){
  s.selected_type=control;s.emit(FireAction::SelectPalette,control);
  return;
 }
 if(control!=12)return;
 s.cursor_type=-1;s.emit(FireAction::CursorDefault);
 if(row<0||row>=3||col<0||col>=6)return;
 const I32 old=s.cells[static_cast<std::size_t>(row*6+col)];
 if(old>=0&&old<12)return;
 if(row==2){
  s.wendy_actor_state=10;
  s.emit(FireAction::Sound,904+static_cast<I32>(random%4U),50,2);
 }else{
  s.bob_actor_state=10;
  s.emit(FireAction::Sound,867+static_cast<I32>(random%4U),50,2);
 }
 s.emit(FireAction::PlaceCell,row,col,s.selected_type);
 if(place && place(row,col,s.selected_type)){
  s.actor_follow_bias=0;s.cursor_cleared=true;
  s.emit(FireAction::ClearPlacementTool);
 }
}
I32 advance_spud_skate_activity(SkateLoop& s,std::vector<Action>& out){
 out.push_back({Op::EnableInput});
 if(s.global_leave_flag==0&&s.global_shutdown==0){
  if(s.phase==2){
   s.ui_input_blocked=1;
   out.push_back({Op::PreparePlayAgain});
   out.push_back({Op::UnloadSkate});
   s.outer_state=0x3c;s.game_selection=0x16;
   s.replay_class=0x1a;s.playback_latch=0;
   return 0;
  }
  s.ui_input_blocked=s.cursor_y>400?1:0;
  out.push_back({Op::UpdatePlayback});
  if(s.startup_sound_pending==1){
   out.push_back({Op::StopIntroVoice});
   out.push_back({Op::StartMusic,0,8});
   s.startup_sound_pending=0;
   out.push_back({Op::ManagedSound,0,0x2f6});
  }
  return 0;
 }
 s.ui_input_blocked=1;
 out.push_back({Op::UnloadMaze}); // Original 00424D10, NOT Skate.
 if(s.global_shutdown){
  s.global_shutdown=0;s.outer_state=0x16;
 }
 if(s.shared_completion!=-1)s.outer_state=0x40;
 return 1;
}
} // namespace btb::original_lifecycle12
