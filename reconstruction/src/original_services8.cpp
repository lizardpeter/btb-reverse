#include "btb/original_services8.hpp"
#include <algorithm>
#include <array>
#include <vector>
// Original-source C++ entrypoints, independently linked to native btb_game.
// Bink/COM state has injected providers rather than fictional hardware.
namespace btb::original_services8 {
// Original source-derived release_inputs.
Plan release_inputs(Inputs& s){
 Plan p{};
 constexpr std::array<I32,16> addr{0x004fbf88,0x004fbd38,0x0051c288,0x0051c28c,0x0051c290,0x0051c298,0x00519954,0x0051be40,0x0051be44,0x0051be34,0x0051be38,0x00519944,0x0051c2a0,0x0051c2a4,0x0051c2a8,0x0051c2ac};
 for(std::size_t i=0;i<addr.size();++i){
  p.emit(Kind::UnregisterSurface,addr[i]);
  if(s.registered[i]){p.emit(Kind::ReleaseSurface,addr[i]);s.registered[i]=0;}
  if(i==1){
   if(s.animation_surface){p.emit(Kind::ReleaseSurface,0x004fbf90);s.animation_surface=0;}
   if(s.primary_input){p.emit(Kind::UnacquireInput,0x004fbf94);p.emit(Kind::ReleaseInterface,0x004fbf94);s.primary_input=0;}
   if(s.keyboard_input){p.emit(Kind::UnacquireInput,0x004fbf98);p.emit(Kind::ReleaseInterface,0x004fbf98);s.keyboard_input=0;}
   if(s.cursor_sound){p.emit(Kind::StopSound,0x004fbf8c);s.cursor_sound=0;}
   if(s.interface_sound){p.emit(Kind::StopSound,0x0051c2b8);s.interface_sound=0;}
  }
 }
 p.emit(Kind::UnloadProfiles);return p;
}

// Original source-derived update_credits.
Plan update_credits(Credits& s,I32 elapsed_seconds,bool mouse_click){
 Plan p{};
 if(mouse_click||s.interval_seconds+s.offset_seconds-elapsed_seconds<0){
  s.offset_seconds=4;p.emit(Kind::ResetCreditsClock);++s.index;
  if(s.index>=static_cast<I32>(s.slides.size())){
   ++s.counter;s.quit_requested=true;
   for(std::size_t i=0;i<s.slides.size();++i){
    p.emit(Kind::UnregisterSurface,0x0048d5c8+4*static_cast<I32>(i));
    if(s.slides[i]){p.emit(Kind::ReleaseSurface,0x0048d5c8+4*static_cast<I32>(i));s.slides[i]=0;}
   }return p;
  }
 }
 if(s.index>=0&&static_cast<std::size_t>(s.index)<s.slides.size())p.emit(Kind::BlitSurface,s.index);
 return p;
}

// Original source-derived decode_and_blit_bink.
Plan decode_and_blit_bink(BinkFrame& s){
 Plan p{};p.emit(Kind::BinkDoFrame);p.emit(Kind::AcquireBinkSurface);
 if(s.acquire_surface_failed){p.result=1;return p;}
 p.emit(Kind::BinkDDSurfaceType);p.emit(Kind::BinkCopyToBuffer);p.emit(Kind::ReleaseBinkSurface);
 if(s.next_enabled){p.emit(Kind::BinkNextFrame);if(s.current_frame<s.total_frames)++s.current_frame;}
 p.emit(Kind::BinkWait);p.emit(Kind::BinkBlit);s.decoded=true;return p;
}

// Original source-derived advance_global_bink.
Plan advance_global_bink(GlobalBink& s){
 Plan p{};p.emit(Kind::BinkDoFrame);p.emit(Kind::AcquireBinkSurface);
 if(!s.video.acquire_surface_failed){
  p.emit(Kind::BinkDDSurfaceType);p.emit(Kind::BinkCopyToBuffer);p.emit(Kind::ReleaseBinkSurface);
  const bool advance=s.video.current_frame!=s.video.total_frames&&(!s.paused_on_input||(!s.input_left&&!s.input_right&&!s.input_keyboard));
  if(advance){
   p.emit(Kind::BinkNextFrame);++s.video.current_frame;p.emit(Kind::BinkWait);p.emit(Kind::BinkBlit);
   if(s.audio_present){s.audio_cursor=std::max(s.audio_cursor,s.audio_duration);if(s.audio_frame_ended)++s.audio_completed_count;}
   return p;
  }
  p.emit(Kind::CloseBink);s.closed=true;
 }
 p.result=1;return p;
}

// Original source-derived initialize_sound_manager.
Plan initialize_sound_manager(SoundManagerBoot& s,bool allocation_succeeded,I32 init_hresult){
 Plan p{};
 if(allocation_succeeded){p.emit(Kind::CreateSoundManager);s.created=true;}
 p.emit(Kind::InitializeSoundManager);
 if(init_hresult<0){p.emit(Kind::EndDialog,3);s.dialog_closed=true;return p;}
 s.initialized=true;s.volume=(s.slider_position-100)*42;return p;
}

// Original source-derived sound_available.
bool sound_available(const ManagedBank& b,I32 id){
 return std::find(b.available_ids.begin(),b.available_ids.end(),id)!=b.available_ids.end();
}

// Original source-derived find_sound_slot.
I32 find_sound_slot(const ManagedBank& b,I32 id){
 for(I32 i=0;i<80;++i)if(const auto& s=b.slots[static_cast<std::size_t>(i)];s.state!=0&&s.sound_id==id)return i;
 return -1;
}

// Original source-derived acquire_sound.
AcquireResult acquire_sound(ManagedBank& b,I32 id,I32 priority,I32 policy,bool native_create_success){
 AcquireResult r{};I32 slot=-1;
 for(I32 i=0;i<80;++i)if(b.slots[static_cast<std::size_t>(i)].state==0){slot=i;break;}
 if(slot<0){
  I32 lowest=101;
  for(I32 i=0;i<80;++i){
   const auto& s=b.slots[static_cast<std::size_t>(i)];
   if(!s.is_playing&&s.priority<lowest){lowest=s.priority;slot=i;}
  }
  if(slot<0)return r;
  r.effects.emit(Kind::ReleaseSound,slot);
 }
 auto& target=b.slots[static_cast<std::size_t>(slot)];
 target={.sound_id=id,.state=native_create_success?1:0,.priority=priority,.policy=policy,.is_playing=native_create_success,.native_created=native_create_success};
 r.effects.emit(Kind::AcquireSound,slot,id,priority);
 if(native_create_success)r.effects.emit(Kind::PlaySound,slot);
 else r.effects.emit(Kind::SoundCreationFailed,slot,id);
 r.slot=slot;return r;
}

// Original source-derived play_managed_sound.
Plan play_managed_sound(ManagedBank& b,I32 id,I32 priority,I32 policy,bool success){
 Plan p{};if(!original_services8::sound_available(b,id))return p;
 I32 slot=original_services8::find_sound_slot(b,id);
 if(slot>=0&&b.slots[static_cast<std::size_t>(slot)].state==2){p.result=2;return p;}
 if(policy>0)for(I32 i=0;i<80;++i){
  auto& s=b.slots[static_cast<std::size_t>(i)];
  if(s.state==0||!s.is_playing)continue;
  if(s.policy==1)return p;
  if(s.policy==2&&(policy==1||policy==2)){p.emit(Kind::StopSound,i);s.is_playing=false;s.state=3;}
 }
 if(slot<0){
  auto a=original_services8::acquire_sound(b,id,priority,policy,success);slot=a.slot;
  p.calls.insert(p.calls.end(),a.effects.calls.begin(),a.effects.calls.end());
  if(slot<0){p.result=-1;return p;}
 }
 auto& rec=b.slots[static_cast<std::size_t>(slot)];
 if(rec.state==1||rec.state==3){p.emit(Kind::PlaySound,slot);rec.is_playing=true;rec.state=2;}
 p.result=1;return p;
}

// Original source-derived set_printer_scale.
Plan set_printer_scale(PrinterScale& s,I32 requested){
 Plan p{};if(requested<0||requested>=4){s.exception_requested=true;p.result=-1;return p;}
 s.mode=requested;p.emit(Kind::SetScaleMode,requested);return p;
}

}
