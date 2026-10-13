#include "btb/original_control6.hpp"
#include <algorithm>
// Six original gameplay controllers compiled independently from retail PE32.
// Original DirectDraw/DirectSound effects are explicit native-host contracts.
namespace btb::original_control6 {
// Original Ghidra-derived update_fireworks function.
Plan update_fireworks(FireState& s,U32 random_roll){
  Plan f;
  if(s.leave||s.shutdown) {
    f.emit(Op::SaveUnloadFireworks);
    if(s.shutdown){s.shutdown=false;s.outer_state=4;}
    f.returns_early=true;return f;
  }
  if(s.grid_reset_requested){
    s.grid.fill(-1);s.grid_reset_requested=false;s.selected_fire_type=0;
    s.phase=0;f.emit(Op::ResetCursor);
  }
  f.emit(Op::NoOp);
  if(!s.intro_completed){
    if(s.intro_pending) {
      f.emit(Op::StopIntro);f.emit(Op::StartMusic,4);
      s.intro_pending=false;
      f.emit(Op::StopManaged);
      f.emit(Op::ManagedVoice,861,50,1);
      f.emit(Op::VoiceInterruptible,861);
    } else if(!s.managed_playing){
      s.intro_completed=true;
      f.emit(Op::ManagedVoice,898,50,1);
      f.emit(Op::VoiceInterruptible,898);
    }
  }
  if(s.attract_counter!=-1 && ++s.attract_counter>2999){
    s.attract_counter=0;f.emit(Op::StopManaged);
    if((random_roll&1U)!=0)f.emit(Op::ManagedVoice,855,50,1);
  }
  f.emit(Op::NoOp);
  switch(s.phase){
    case 0:
      f.emit(Op::DrawFireEditor);f.emit(Op::UpdateFireControls);
      if(s.click&&s.hit_region!=-1){
        s.attract_counter=-1;
        f.emit(Op::BeginFireAction,s.hit_region);
        f.emit(Op::CompleteFireAction,s.hit_region);
        f.returns_early=true;
      }break;
    case 1:
      f.emit(Op::DrawFireEditor);f.emit(Op::UpdateFireControls);
      if(s.click&&s.hit_region!=-1){
        f.emit(Op::CompleteFireAction,s.hit_region);
        f.returns_early=true;
      }break;
    case 2:
      f.emit(Op::DrawFireEditor);
      s.cached_movie_index=s.selected_fire_type;
      ++s.phase;f.returns_early=true;break;
    case 3:
      f.emit(Op::DrawFireEditor);
      f.emit(Op::DrawMovie,s.cached_movie_index);
      if(s.movie_frame_complete){
        ++s.phase;f.emit(Op::RestartMovie,s.cached_movie_index);
        f.returns_early=true;
      }break;
    case 4:
      f.emit(Op::DrawFireEditor);s.phase=0;f.returns_early=true;break;
    case 8:
      f.emit(Op::StopManaged);f.emit(Op::UnloadFireEditor);
      f.emit(Op::StartFireworksMovieBank);
      s.phase=9;f.emit(Op::ResetShowTimer);f.emit(Op::ResetShowSound);
      f.returns_early=s.ambient_sound_present;
      break;
    case 9:
      f.emit(Op::FireworksShowPlayback);f.returns_early=true;break;
    case 13:
      f.emit(Op::DrawFireEditor);f.emit(Op::UpdateFireControls);
      if(s.click&&s.hit_region!=-1)
        f.emit(Op::DeleteFireCell,s.hit_region);
      break;
    case 14:
      if(s.managed_playing)f.emit(Op::DrawFireEditor);
      else{
        f.emit(Op::StopOverlaySound);f.emit(Op::ClearBackBuffer);
        f.emit(Op::DisableInput);f.emit(Op::OpenCompletionMovie);
        s.phase=15;
      }
      f.returns_early=true;break;
    case 15:
      f.emit(Op::UpdateCompletionMovie);
      if(s.global_movie_complete){
        f.emit(Op::EnableInput);s.phase=8;f.emit(Op::StopManaged);
        f.emit(Op::ManagedVoice,142,90,1);f.emit(Op::VoiceInterruptible,142);
        f.returns_early=true;
      }
      break;
    case 16:
      f.emit(Op::DrawFireCertificate);f.returns_early=true;break;
    case 17:
      f.emit(Op::PreparePlayAgain);
      s.outer_state=60;s.ui_context=36;s.replay_latch=0;
      f.emit(Op::SaveUnloadFireworks);f.returns_early=true;break;
  }
  return f;
}

// Original Ghidra-derived cursor function.
void cursor(Plan& p,BandEditor& s,I32 type){
  s.cursor_type=type;p.emit(Op::SetCursorSurface,type);
}

// Original Ghidra-derived begin_band_action function.
Plan begin_band_action(
  BandEditor& s,I32 hit,std::optional<btb::bobs_band::Cell> cell){
  Plan out{};
  if(hit>=0&&hit<10){
    if(s.delete_mode)s.delete_mode=false;
    original_control6::cursor(out,s,hit);s.phase=1;s.selected=hit;
    out.emit(Op::PlayBandPickSound,hit);
    const I32 family=hit/2;
    if(s.machine_voice_latch[static_cast<std::size_t>(family)]==0)
      s.machine_voice_latch[static_cast<std::size_t>(family)]=(hit&1)+1;
    return out;
  }
  if(hit==11&&cell){
    auto c=*cell;
    // The retail click in state 0 resolves continuation cells to their
    // note owner via DecodeBobsBandGridCell and RemoveGrandOpeningEvent.
    const auto value=s.composition.at(c);
    if(value==btb::bobs_band::kContinuation)
      while(c.second>0&&s.composition.at(c)==btb::bobs_band::kContinuation)
        --c.second;
    if(!btb::bobs_band::valid_machine(s.composition.at(c)))return out;
    const auto removed=s.composition.erase_owner(c);
    if(!removed)return out;
    out.emit(Op::RemoveBandEvent,c.row,c.second,removed->machine_type);
    if(!s.delete_mode){
      original_control6::cursor(out,s,removed->machine_type);
      s.selected=removed->machine_type;s.phase=1;
    }else s.delete_handled=true;
  }
  return out;
}

// Original Ghidra-derived update_band_selected function.
Plan update_band_selected(
  BandEditor& s,I32 hit,std::optional<btb::bobs_band::Cell> cell){
  Plan out{};
  if(hit>=0&&hit<10){
    if(s.phase==1&&s.selected==hit){
      original_control6::cursor(out,s,-1);s.phase=0;
    } else {
      original_control6::cursor(out,s,hit);s.phase=1;s.selected=hit;
      out.emit(Op::PlayBandPickSound,hit);
    }
    return out;
  }
  if(hit==11&&cell&&s.selected>=0&&s.selected<10){
    const auto previous=s.composition.at(*cell);
    if(btb::bobs_band::valid_machine(previous)){
      const auto changed=s.composition.replace_selected(*cell,s.selected);
      out.emit(Op::RemoveBandEvent,cell->row,cell->second,previous);
      original_control6::cursor(out,s,previous);
      out.emit(Op::PlaceBandEvent,cell->row,cell->second,s.selected);
      if(changed.placed)s.selected=previous;
      else original_control6::cursor(out,s,s.selected);
      return out;
    }
    if(s.composition.place(*cell,s.selected)){
      out.emit(Op::PlaceBandEvent,cell->row,cell->second,s.selected);
      original_control6::cursor(out,s,-1);s.phase=0;
    }
  } else if(hit==22&&s.phase==0&&s.delete_mode){
    original_control6::cursor(out,s,-1);
  }
  return out;
}

// Original Ghidra-derived update_help_button function.
Plan update_help_button(Help& s){
  Plan out{};
  if(s.current_input_mode!=s.required_input_mode)return out;
  if(s.pointer_state==1){
    out.emit(Op::SetCursorSurface,1);
    s.pointer_state=2;out.emit(Op::StopManaged);
    for(I32 i=0;i<20;++i)if(s.sound_slots[static_cast<std::size_t>(i)])
      out.emit(Op::StopAllActivitySound,i);
    out.returns_early=true;return out;
  }
  if((s.input_flags&8)==0)s.hover_latch=0;
  else{
    if(!s.hover_latch)out.emit(Op::ManagedVoice,455,50,2);
    s.hover_latch=1;
    if(s.outer_state==1&&s.substate==2)
      out.emit(Op::DrawScene,1);
    if(s.progress_state==12)
      out.emit(Op::DrawScene,2);
  }
  if(s.mouse_press){
    const bool intro=s.outer_state==1&&s.substate==2;
    const auto hit=intro?s.intro_hit:s.generic_hit;
    if(strict_inside(hit,s.mouse_x,s.mouse_y))s.pointer_state=1;
  }
  return out;
}

// Original Ghidra-derived update_leave_confirmation function.
Plan update_leave_confirmation(Leave& s){
  Plan out{};
  if(s.phase>1){
    s.phase=0;out.emit(Op::RestoreMouseBounds);
    out.emit(Op::ResumeMovie);out.returns_early=true;return out;
  }
  out.emit(Op::DrawScene,0);out.emit(Op::DrawScene,1);
  // Both rectangles come from the native stack constants and UI backing
  // table. Do not guess the partially recovered second rectangle.
  I32 hit=-1;
  for(I32 i=0;i<2;++i)
    if(strict_inside(s.retail_buttons[static_cast<std::size_t>(i)],s.cursor_x,s.cursor_y)){
      hit=i;break;
    }
  if(hit!=-1){
    out.emit(Op::DrawSurface,hit);
    if(s.hot_index!=hit)out.emit(Op::PlayHover,453+hit,50,2);
  }
  s.hot_index=hit;
  if(s.button_hover&&hit!=-1)out.emit(Op::DrawSurface,hit+2);
  if(s.mouse_press&&hit!=-1){
    s.phase=0;out.emit(Op::ResumeMovie);
    out.emit(Op::RestoreMouseBounds);
    s.selected_response=-1;
    if(hit==0){s.game_shutdown=true;out.emit(Op::SetGlobalShutdown);}
    out.returns_early=true;
  }
  return out;
}

// Original Ghidra-derived update_play_again function.
Plan update_play_again(PlayAgain& s){
  Plan out{};
  if(s.phase>=2){
    out.emit(Op::RestoreMouseBounds);out.emit(Op::ResumeMovie);
    out.emit(Op::DrawSurface,-1);s.old_surface_nonnull=false;
    out.returns_early=true;return out;
  }
  out.emit(Op::DrawScene,0);
  if(s.click_pulse){
    for(I32 i=0;i<6;++i)
      if(inclusive_inside(s.options[static_cast<std::size_t>(i)],
                          s.cursor_x,s.cursor_y)){s.selection=i+1;break;}
  }
  if(s.mouse_press){
    for(I32 i=0;i<6;++i){
      if(!inclusive_inside(s.options[static_cast<std::size_t>(i)],
                           s.cursor_x,s.cursor_y))continue;
      if(s.selection!=i+1)break;
      s.selection=-1;
      if(i==5){out.returns_early=true;return out;}
      s.phase=0;s.confirmed_context=i==0||i==4?2:i+2;
      out.returns_early=true;return out;
    }
  }
  return out;
}

}
