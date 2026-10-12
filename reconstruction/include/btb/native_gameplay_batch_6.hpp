#pragma once
// Six original game-specific PE32 procedures modeled as source-directed
// C++26 behavior with explicit native resource/audio/video host effects.
// Original exe SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Original asset data and physical keyboard/mouse boundaries are caller-owned.
#include "btb/bobs_band.hpp"
#include "btb/fireworks_layout.hpp"
#include "btb/golf_data.hpp"
#include "btb/spud_skate_data.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
namespace btb::native_gameplay_6 {
using I32=std::int32_t;using U32=std::uint32_t;using U8=std::uint8_t;
enum class Op:std::uint8_t{ParseGolf,ParseSkate,LoadBitmap,RegisterBitmap,MarkColorKey,InstallUnload,LoadMovie,CreateMovieSurface,OpenBink,LoadSoundTable,EnableIntro,DrawBitmap,DrawSprite,DrawToolbar,PlaySound,StopSounds,StopLoop,StartManagedVoice,SetCursor,OpenConfirmation,SetAction,SetFlag,DrawBandGrid,DrawBandMachine,DrawBandConductor,BlitNote,ResetSound,PlaySoundDirect,AnimationTick};
struct Effect{Op op{};I32 a{},b{},c{};friend bool operator==(const Effect&,const Effect&)=default;};
struct Plan{std::vector<Effect> actions{};bool failed{},early_return{};std::string detail{};void emit(Op op,I32 a=0,I32 b=0,I32 c=0){actions.push_back({op,a,b,c});}};

// 004145F0 InitializeGolfActivity. All 18 original registered bitmap slots,
// nine static transparency slots and six flag animation images are explicitly
// part of initialization; load of Data/SubGameGolf/golfdata.txt is host-owned.
struct GolfInit {
 btb::golf::Data data{};
 I32 selected_ball{-1},marker{-1},elapsed{},power{},shots{};
 I32 intro_phase{1},course_state{5},sound_ready{1};
 std::array<I32,3> difficulty_seeds{5,7,9};
 btb::golf::Vec2i ball{};
 bool completed{};
};
[[nodiscard]] inline Plan initialize_golf(GolfInit& s,
    const btb::golf::Data& original){
 Plan p{};s=GolfInit{};s.data=original;s.ball=original.initial_ball_position();
 p.emit(Op::ParseGolf);
 p.emit(Op::InstallUnload,0x00414410);
 p.emit(Op::LoadBitmap,0x0050aedc);p.emit(Op::RegisterBitmap,0x0050aedc);
 for(int j=0;j<3;++j){
  for(I32 slot:{0x0050ab88+4*j,0x0050addc+4*j}){
   p.emit(Op::LoadBitmap,slot);p.emit(Op::RegisterBitmap,slot);
  }
 }
 for(I32 slot:{0x0050aee0,0x0050aee4,0x0050aee8,0x0050aeec,
               0x0050aef0,0x0050aef4,0x0050aef8,0x0050adec}){
   p.emit(Op::LoadBitmap,slot);p.emit(Op::RegisterBitmap,slot);
 }
 for(int j=0;j<3;++j){
  I32 slot=0x0050adfc+4*j;
  p.emit(Op::LoadBitmap,slot);p.emit(Op::RegisterBitmap,slot);
 }
 // Original color-keyed images: six flags, ball/stop/toolbar/shadow/power/
 // marker/numbers, plus three animated course overlay sprites.
 for(int j=0;j<3;++j)for(I32 slot:{0x0050ab88+4*j,0x0050addc+4*j})
   p.emit(Op::MarkColorKey,slot,0xff00ff);
 for(I32 slot:{0x0050aedc,0x0050aee0,0x0050aee4,0x0050aee8,
               0x0050aef0,0x0050aef4,0x0050aef8,0x0050adec,
               0x0050adfc,0x0050ae00,0x0050ae04})
   p.emit(Op::MarkColorKey,slot,0xff00ff);
 p.emit(Op::EnableIntro);return p;
}

// 00424360 InitializeSpudSkateActivity. Five original Bink source assets,
// each capped at 7,000,000 bytes and stored into native-memory movies;
// five original bitmap source slots. Sound matrix has 160 initial -1 entries.
struct SkateInit {
 btb::spud_skate::TimingData timing{};
 std::array<std::vector<U8>,5> movie_bytes{};
 std::array<I32,160> sound_ids{};
 I32 score{},state{},animation_stage{},stunt_count{},active_stunt{-1};
 bool intro_pending{},valid{};
};
[[nodiscard]] inline Plan initialize_skate(SkateInit& s,
 const btb::spud_skate::TimingData& source,
 const std::array<std::string,5>& original_movie_paths,
 const std::function<std::optional<std::vector<U8>>(const std::string&,std::size_t)>& read){
 Plan p{};s=SkateInit{};s.sound_ids.fill(-1);
 s.timing=source;p.emit(Op::ParseSkate);p.emit(Op::InstallUnload,0x00424100);
 for(I32 slot:{0x00514ea0,0x005148b4,0x00514b60,0x00514b64,0x00514b68}){
  p.emit(Op::LoadBitmap,slot);p.emit(Op::RegisterBitmap,slot);
  p.emit(Op::MarkColorKey,slot,0xff00ff);
 }
 constexpr std::size_t capacity=7000000;
 for(std::size_t i=0;i<5;++i){
  if(!read){p.failed=true;p.detail="Retail Spud Skate Bink asset source unavailable";return p;}
  const auto content=read(original_movie_paths[i],capacity);
  if(!content||content->size()>capacity){
   p.failed=true;p.detail="Retail Spud Skate Bink asset missing or oversized";return p;
  }
  s.movie_bytes[i]=*content;
  p.emit(Op::LoadMovie,static_cast<I32>(i),static_cast<I32>(content->size()));
  p.emit(Op::CreateMovieSurface,static_cast<I32>(i),600,380);
  p.emit(Op::OpenBink,static_cast<I32>(i));
 }
 p.emit(Op::LoadSoundTable);
 s.intro_pending=true;s.valid=true;return p;
}

// 0041F820 UpdateGrandOpeningToolbar. The four hit rectangles are originally
// stack constants; supply them from the retail UI table instead of guessing.
struct Rect{I32 x0{},y0{},x1{},y1{};};
[[nodiscard]] constexpr bool inside(Rect r,I32 x,I32 y){
 return r.x0<x&&x<r.x1&&r.y0<y&&y<r.y1;
}
struct BandToolbar{
 I32 state{},start_voice{},cursor_x{},cursor_y{},last_hover{-1};
 bool delete_mode{},play_pending{},managed_playing{},pressed{},mouse_down{};
 std::array<Rect,4> buttons{};
};
[[nodiscard]] inline Plan update_band_toolbar(BandToolbar& s,U32 random_roll=0){
 Plan p{};
 if(s.state!=0)return p;
 if(s.play_pending&&!s.managed_playing){
  s.play_pending=false;s.state=8;p.early_return=true;return p;
 }
 I32 hit=-1;
 for(I32 i=0;i<4;++i)if(inside(s.buttons[static_cast<std::size_t>(i)],
                                  s.cursor_x,s.cursor_y)){hit=i;break;}
 if(hit==-1){s.last_hover=-1;return p;}
 p.emit(Op::DrawToolbar,hit);
 if(!s.delete_mode&&s.last_hover!=hit){
  p.emit(Op::StartManagedVoice,s.start_voice+6+hit,50,2);s.last_hover=hit;
 }
 if(s.mouse_down)p.emit(Op::DrawToolbar,hit+4);
 if(!s.pressed)return p;
 switch(hit){
 case 0:
  if(s.state==0&&!s.delete_mode){
   s.play_pending=true;p.emit(Op::StopSounds);
   p.emit(Op::StartManagedVoice,s.start_voice+1,50,1);
  }break;
 case 1:
  if(s.state==9&&!s.delete_mode){
   s.state=0;p.emit(Op::StopLoop);
   p.emit(Op::StartManagedVoice,s.start_voice+3+static_cast<I32>(random_roll%2),50,1);
  }break;
 case 2:
  if(s.state==0&&!s.delete_mode){
   p.emit(Op::OpenConfirmation,2);
  }break;
 case 3:
  if(s.state==0){
   if(!s.delete_mode){
    s.delete_mode=true;p.emit(Op::StartManagedVoice,s.start_voice+9+static_cast<I32>(random_roll%2),50,1);
    p.emit(Op::SetCursor,1);
   }else{
    s.delete_mode=false;p.emit(Op::SetCursor,0);
   }
  }break;
 }
 return p;
}

// 0041FC90 PlayAndDrawGrandOpeningComposition: original 5 rows x 24 seconds
// of 120 cells, 100-centisecond time slots and descending pitch sample banks.
// The original machine sound objects are provided by the native audio host.
struct BandPlayback {
 btb::bobs_band::Composition score{};
 I32 elapsed_centiseconds{},last_centiseconds{};
 I32 conductor_index{},conductor_frame{},conductor_accum{},frame_delta{1};
 bool button_pulse{};
};
[[nodiscard]] inline Plan play_draw_band(BandPlayback& s){
 Plan p{};p.emit(Op::DrawBandGrid);
 const I32 sec=s.elapsed_centiseconds/100;
 if(sec<0||sec>=24)return p;
 for(I32 row=0;row<5;++row){
  for(I32 second=0;second<=sec;++second){
   const I32 type=s.score.at({row,second});
   if(!btb::bobs_band::valid_machine(type))continue;
   // Retail clips only the note at the active second, for even types,
   // or for the last two seconds for odd (two-cell) machinery notes.
   const bool active=(type%2!=0 && (second==sec||second+1==sec)) ||
                     (type%2==0 && second==sec);
   const I32 width=active?((s.elapsed_centiseconds-second*100)*22)/100:22;
   p.emit(Op::BlitNote,row*24+second,type,width);
  }
 }
 if(s.last_centiseconds/100!=sec){
  for(I32 row=0;row<5;++row){
   const I32 type=s.score.at({row,sec});
   if(!btb::bobs_band::valid_machine(type))continue;
   const I32 pitch=btb::bobs_band::pitch_sample_index(row,type);
   p.emit(Op::ResetSound,pitch);
   p.emit(Op::PlaySoundDirect,pitch);
  }
 }
 s.conductor_accum+=s.frame_delta;
 if(s.conductor_accum>3){
  ++s.conductor_frame;s.conductor_accum=0;
  constexpr std::array<I32,3> min_frame{50,50,50};
  constexpr std::array<I32,3> max_frame{90,85,105};
  if(s.conductor_index>=0&&s.conductor_index<3){
   const auto ix=static_cast<std::size_t>(s.conductor_index);
   if(s.conductor_frame>=max_frame[ix]||s.conductor_frame<min_frame[ix])
    s.conductor_frame=min_frame[ix];
  }
 }
 p.emit(Op::DrawBandConductor,s.conductor_index,s.conductor_frame);
 s.last_centiseconds=s.elapsed_centiseconds;
 return p;
}

// 00413B60 UpdateFireworksEditorControls: 3 retail command rectangles
// 0x19 clear, 0x1A delete, 0x1B play. Original hover and mode-dependent
// click branches; preserve 10-tick debounce after dropping an item.
struct FireControls {
 I32 mode{},selected_left{},selected_right{},hover{-1},debounce{},cursor_x{},cursor_y{};
 bool click{},mouse_down{},object_selected{},confirmation_open{};
 std::array<btb::fireworks::InteractiveRegion,33> regions{};
};
[[nodiscard]] inline Plan update_firework_controls(FireControls& s){
 Plan p{};
 if(s.debounce>0){--s.debounce;p.early_return=true;return p;}
 I32 cursor_x=s.cursor_x,cursor_y=s.cursor_y;
 if(s.selected_left>0||s.selected_right>0){cursor_x+=40;cursor_y+=20;}
 for(const auto& region:s.regions){
  const auto action=region.action;
  if(action==-1)continue;
  if(!(region.rect.left<cursor_x&&cursor_x<region.rect.right&&
       region.rect.top<cursor_y&&cursor_y<region.rect.bottom))continue;
  if(action!=25&&action!=26&&action!=27)continue;
  if(action==27){
   if(s.selected_left==0&&s.selected_right==0){
    if(s.click&&s.mode==0){
     s.mode=14;p.emit(Op::StartManagedVoice,880,50,2);p.early_return=true;
    }else if(s.hover!=27&&!s.mouse_down)
     p.emit(Op::StartManagedVoice,881,50,2);
   }
  }else if(action==26){
   if(s.object_selected&&(s.selected_left==1||s.selected_right==1)){
    s.selected_left=0;s.selected_right=0;s.mode=0;
    p.emit(Op::SetCursor,0);s.debounce=10;p.early_return=true;
   }else if(s.click&&s.debounce==0){
    if(s.mode==13){s.mode=0;p.emit(Op::SetCursor,0);}
    else{s.mode=13;p.emit(Op::SetCursor,1);p.emit(Op::StartManagedVoice,890,50,2);}
   }else if(s.hover!=26&&!s.mouse_down)
    p.emit(Op::StartManagedVoice,882,50,2);
  }else if(action==25){
   if(s.click&&s.mode==0){
    s.confirmation_open=true;p.emit(Op::OpenConfirmation,3);
    p.emit(Op::StartManagedVoice,885,50,2);
   }else if(s.hover!=25&&!s.mouse_down)
    p.emit(Op::StartManagedVoice,883,50,2);
  }
  s.hover=action;p.emit(Op::DrawToolbar,action);return p;
 }
 return p;
}

// 0041F090 DrawGrandOpeningCompositionAndMachines: draw all nonnegative
// source machine cells and their five animation records in deterministic
// order. Original per-type sprite geometry remains native-authored data.
struct BandMachine{
 I32 mode{},frame{},tick{},frame_count{1},frame_width{},frame_height{};
 I32 sprite_id{},x{},y{};
};
struct BandDraw{
 btb::bobs_band::Composition score{};
 std::array<BandMachine,5> machines{};
 I32 frame_delta{1},conductor_index{},conductor_frame{},conductor_accum{};
};
[[nodiscard]] inline Plan draw_band_machines(BandDraw& s){
 Plan p{};p.emit(Op::DrawBitmap,0x00513f1c);
 for(I32 row=0;row<5;++row)
  for(I32 second=0;second<24;++second){
   const I32 type=s.score.at({row,second});
   if(btb::bobs_band::valid_machine(type))
    p.emit(Op::DrawSprite,type,row,second);
  }
 for(I32 i=0;i<5;++i){
  auto& m=s.machines[static_cast<std::size_t>(i)];
  if(m.mode!=0){
   m.tick+=s.frame_delta;
   if(m.tick>3){
    m.tick=0;++m.frame;
    if(m.frame>=m.frame_count){m.frame=0;m.mode=0;}
   }
  }
  p.emit(Op::DrawBandMachine,i,m.mode,m.frame);
 }
 if(++s.conductor_accum>5){s.conductor_accum=0;++s.conductor_frame;}
 p.emit(Op::DrawBandConductor,s.conductor_index,s.conductor_frame);
 p.emit(Op::DrawBitmap,0x00513f20);return p;
}
} // namespace btb::native_gameplay_6
