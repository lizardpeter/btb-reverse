#pragma once
// Models of eight original game platform functions, PE32 SHA256:
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// The original DirectDraw/DirectSound/Bink/COM calls remain ordered host effects.
// These models are not retail-equivalent x86 ABI replacements.
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <limits>
#include <optional>
#include <string>
#include <utility>
#include <vector>
namespace btb::retail_services {
using I32=std::int32_t;using U32=std::uint32_t;
enum class Kind:std::uint8_t{UnregisterSurface,ReleaseSurface,ReleaseInterface,UnacquireInput,StopSound,UnloadProfiles,BlitSurface,ResetCreditsClock,StopAllManaged,BinkDoFrame,AcquireBinkSurface,BinkDDSurfaceType,BinkCopyToBuffer,ReleaseBinkSurface,BinkNextFrame,BinkWait,BinkBlit,CloseBink,CreateSoundManager,InitializeSoundManager,EndDialog,AcquireSound,ReleaseSound,PlaySound,SoundCreationFailed,SetScaleMode};
struct Effect{Kind op{};I32 a{},b{},c{};friend bool operator==(const Effect&,const Effect&)=default;};
struct Plan{std::vector<Effect> calls{};I32 result{};void emit(Kind op,I32 a=0,I32 b=0,I32 c=0){calls.push_back({op,a,b,c});}};

// 00407B60 ReleaseInputAndCursorResources. Surfaces always unregistered;
// COM release conditional, input devices Unacquire then Release; final
// original caller invokes SaveAndUnloadPlayerProfiles.
struct Inputs{
 std::array<U32,16> registered{};
 U32 animation_surface{},primary_input{},keyboard_input{},cursor_sound{},interface_sound{};
};
[[nodiscard]] inline Plan release_inputs(Inputs& s){
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

// 00406AD0 UpdateCreditsSlideshow; source clock reset at 4 seconds,
// increment index then unregister all old slides when last one expires.
struct Credits{std::vector<U32> slides{};I32 index{},offset_seconds{4},interval_seconds{},counter{};bool quit_requested{};};
[[nodiscard]] inline Plan update_credits(Credits& s,I32 elapsed_seconds,bool mouse_click){
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

// 00408F70 DecodeAndBlitBinkFrame: original DoFrame -> DirectDraw Lock ->
// CopyToBuffer -> Unlock -> optional NextFrame -> BinkWait -> Blit.
// Original pseudocode has unresolved pointer-vs-control comparisons.
struct BinkFrame{I32 current_frame{},total_frames{};bool acquire_surface_failed{},next_enabled{},decoded{};};
[[nodiscard]] inline Plan decode_and_blit_bink(BinkFrame& s){
 Plan p{};p.emit(Kind::BinkDoFrame);p.emit(Kind::AcquireBinkSurface);
 if(s.acquire_surface_failed){p.result=1;return p;}
 p.emit(Kind::BinkDDSurfaceType);p.emit(Kind::BinkCopyToBuffer);p.emit(Kind::ReleaseBinkSurface);
 if(s.next_enabled){p.emit(Kind::BinkNextFrame);if(s.current_frame<s.total_frames)++s.current_frame;}
 p.emit(Kind::BinkWait);p.emit(Kind::BinkBlit);s.decoded=true;return p;
}

// 004090D0 UpdateGlobalBinkMovie: preserve ended/paused branches,
// stream audio cursor max update and global Close on completion.
struct GlobalBink{BinkFrame video{};bool paused_on_input{},input_left{},input_right{},input_keyboard{},audio_present{},audio_frame_ended{},closed{};U32 audio_cursor{},audio_duration{};I32 audio_completed_count{};};
[[nodiscard]] inline Plan advance_global_bink(GlobalBink& s){
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

// 004092D0 SoundManager Win32 dialog bootstrap.
struct SoundManagerBoot{bool created{},initialized{},dialog_closed{};I32 slider_position{100},volume{};};
[[nodiscard]] inline Plan initialize_sound_manager(SoundManagerBoot& s,bool allocation_succeeded,I32 init_hresult){
 Plan p{};
 if(allocation_succeeded){p.emit(Kind::CreateSoundManager);s.created=true;}
 p.emit(Kind::InitializeSoundManager);
 if(init_hresult<0){p.emit(Kind::EndDialog,3);s.dialog_closed=true;return p;}
 s.initialized=true;s.volume=(s.slider_position-100)*42;return p;
}

// 00402E10 AcquireAndPlaySound / 00402CF0 PlayManagedSoundById.
// Original 80 slots; state 0 empty, 1 loaded, 2 playing, 3 replayable.
// Sound-bank available IDs replace original data-at-offset 0x6CC.
struct SoundSlot{I32 sound_id{-1},state{},priority{101},policy{};bool is_playing{},native_created{};};
struct ManagedBank{std::array<SoundSlot,80> slots{};std::vector<I32> available_ids{};};
[[nodiscard]] inline bool sound_available(const ManagedBank& b,I32 id){
 return std::find(b.available_ids.begin(),b.available_ids.end(),id)!=b.available_ids.end();
}
[[nodiscard]] inline I32 find_sound_slot(const ManagedBank& b,I32 id){
 for(I32 i=0;i<80;++i)if(const auto& s=b.slots[static_cast<std::size_t>(i)];s.state!=0&&s.sound_id==id)return i;
 return -1;
}
struct AcquireResult{I32 slot{-1};Plan effects{};};
[[nodiscard]] inline AcquireResult acquire_sound(ManagedBank& b,I32 id,I32 priority,I32 policy,bool native_create_success){
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
[[nodiscard]] inline Plan play_managed_sound(ManagedBank& b,I32 id,I32 priority,I32 policy,bool success=true){
 Plan p{};if(!sound_available(b,id))return p;
 I32 slot=find_sound_slot(b,id);
 if(slot>=0&&b.slots[static_cast<std::size_t>(slot)].state==2){p.result=2;return p;}
 if(policy>0)for(I32 i=0;i<80;++i){
  auto& s=b.slots[static_cast<std::size_t>(i)];
  if(s.state==0||!s.is_playing)continue;
  if(s.policy==1)return p;
  if(s.policy==2&&(policy==1||policy==2)){p.emit(Kind::StopSound,i);s.is_playing=false;s.state=3;}
 }
 if(slot<0){
  auto a=acquire_sound(b,id,priority,policy,success);slot=a.slot;
  p.calls.insert(p.calls.end(),a.effects.calls.begin(),a.effects.calls.end());
  if(slot<0){p.result=-1;return p;}
 }
 auto& rec=b.slots[static_cast<std::size_t>(slot)];
 if(rec.state==1||rec.state==3){p.emit(Kind::PlaySound,slot);rec.is_playing=true;rec.state=2;}
 p.result=1;return p;
}

// 00405380 BitmapPrinterSetScaleMode accepts integer 0..3 and throws
// Invalid scale otherwise; typed return indicates the native throw path.
struct PrinterScale{I32 mode{};bool exception_requested{};};
[[nodiscard]] inline Plan set_printer_scale(PrinterScale& s,I32 requested){
 Plan p{};if(requested<0||requested>=4){s.exception_requested=true;p.result=-1;return p;}
 s.mode=requested;p.emit(Kind::SetScaleMode,requested);return p;
}
} // namespace btb::retail_services
