#include "btb/original_gameplay6.hpp"
#include <algorithm>
namespace btb::original_gameplay6 {
namespace {
int count(const Plan& p,Op op){
 return static_cast<int>(std::count_if(p.actions.begin(),p.actions.end(),
    [&](const Effect& e){return e.op==op;}));
}
bool has(const Plan& p,Op op,I32 slot){
 return std::any_of(p.actions.begin(),p.actions.end(),
    [&](const Effect& e){return e.op==op&&e.a==slot;});
}
bool equal(const Plan& a,const Plan& b){
 return a.actions==b.actions&&a.failed==b.failed&&
        a.early_return==b.early_return&&a.detail==b.detail;
}
}
bool verify_6_original_gameplay_functions() noexcept {
 GolfInit golf{};
 btb::golf::Data data{};
 data.bob_position={100,200};
 data.ball_offset_from_bob_retail={11,12};
 auto p=initialize_golf_activity(golf,data);
 if(!(golf.ball==btb::golf::Vec2i{111,212})||
    golf.difficulty_seeds[0]!=5||golf.difficulty_seeds[2]!=9||
    golf.course_state!=5||golf.intro_phase!=1||
    count(p,Op::LoadBitmap)!=18||count(p,Op::RegisterBitmap)!=18||
    count(p,Op::MarkColorKey)!=17||
    !has(p,Op::InstallUnload,0x00414410)||
    p.actions.back().op!=Op::EnableIntro)return false;
 GolfInit golf_model{};
 if(!equal(p,btb::native_gameplay_6::initialize_golf(golf_model,data)))
   return false;
 SkateInit skate{};
 btb::spud_skate::TimingData timing{};
 timing.stunt_count=8;timing.loop_start_frame=100;
 std::array<std::string,5> paths{
    "bad.bik","okay.bik","good.bik","end.bik","special.bik"};
 int visited=0;
 const auto read=[&](const std::string& path,std::size_t capacity)
    ->std::optional<std::vector<U8>>{
   if(capacity!=7000000||path!=paths[static_cast<std::size_t>(visited)])
      return std::nullopt;
   ++visited;
   return std::vector<U8>(static_cast<std::size_t>(visited),0x42);
 };
 p=initialize_spud_skate(skate,timing,paths,read);
 if(!skate.valid||!skate.intro_pending||
    skate.timing.loop_start_frame!=100||visited!=5||
    count(p,Op::LoadMovie)!=5||count(p,Op::CreateMovieSurface)!=5||
    count(p,Op::OpenBink)!=5||count(p,Op::LoadBitmap)!=5)return false;
 for(const auto id:skate.sound_ids)if(id!=-1)return false;
 SkateInit unavailable{};
 p=initialize_spud_skate(unavailable,timing,paths,{});
 if(!p.failed||unavailable.valid)return false;
 BandToolbar t{};
 t.buttons={{{0,0,100,100},{100,0,200,100},
              {200,0,300,100},{300,0,400,100}}};
 t.cursor_x=50;t.cursor_y=50;t.start_voice=700;
 p=update_grand_opening_toolbar(t);
 if(!has(p,Op::StartManagedVoice,706)||t.last_hover!=0)return false;
 p=update_grand_opening_toolbar(t);
 if(count(p,Op::StartManagedVoice)!=0)return false;
 t.pressed=true;p=update_grand_opening_toolbar(t);
 if(!t.play_pending||!has(p,Op::StartManagedVoice,701))return false;
 t.pressed=false;t.managed_playing=true;p=update_grand_opening_toolbar(t);
 if(t.state!=0||!t.play_pending)return false;
 t.managed_playing=false;p=update_grand_opening_toolbar(t);
 if(t.state!=8||t.play_pending||!p.early_return)return false;
 BandToolbar erase{};
 erase.buttons=t.buttons;erase.cursor_x=350;erase.cursor_y=50;
 erase.pressed=true;erase.start_voice=800;
 p=update_grand_opening_toolbar(erase,1);
 if(!erase.delete_mode||!has(p,Op::SetCursor,1)||
    !has(p,Op::StartManagedVoice,810))return false;
 p=update_grand_opening_toolbar(erase,1);
 if(erase.delete_mode||!has(p,Op::SetCursor,0))return false;
 BandToolbar confirm{};
 confirm.buttons=t.buttons;confirm.cursor_x=250;confirm.cursor_y=50;
 confirm.pressed=true;p=update_grand_opening_toolbar(confirm);
 if(!has(p,Op::OpenConfirmation,2))return false;
 BandPlayback playback{};
 if(!playback.score.place({0,0},0)||
    !playback.score.place({1,1},1)||
    !playback.score.place({4,2},8))return false;
 playback.elapsed_centiseconds=10;
 p=play_draw_grand_opening(playback);
 if(count(p,Op::DrawBandGrid)!=1||!has(p,Op::BlitNote,0)||
    count(p,Op::PlaySoundDirect)!=0)return false;
 playback.elapsed_centiseconds=100;p=play_draw_grand_opening(playback);
 if(count(p,Op::PlaySoundDirect)!=1||
    !has(p,Op::PlaySoundDirect,31)||count(p,Op::ResetSound)!=1)
   return false;
 playback.elapsed_centiseconds=200;p=play_draw_grand_opening(playback);
 if(!has(p,Op::PlaySoundDirect,8)||playback.last_centiseconds!=200)
   return false;
 playback.elapsed_centiseconds=2400;p=play_draw_grand_opening(playback);
 if(count(p,Op::BlitNote)!=0)return false;
 FireControls fire{};
 fire.regions[0]={btb::fireworks::Recti{0,0,100,100},27};
 fire.regions[1]={btb::fireworks::Recti{100,0,200,100},26};
 fire.regions[2]={btb::fireworks::Recti{200,0,300,100},25};
 fire.cursor_x=50;fire.cursor_y=40;
 p=update_fireworks_editor_controls(fire);
 if(!has(p,Op::StartManagedVoice,881))return false;
 fire.click=true;p=update_fireworks_editor_controls(fire);
 if(fire.mode!=14||!has(p,Op::StartManagedVoice,880))return false;
 FireControls deletion{};
 deletion.regions=fire.regions;deletion.cursor_x=150;
 deletion.cursor_y=50;deletion.click=true;
 p=update_fireworks_editor_controls(deletion);
 if(deletion.mode!=13||!has(p,Op::SetCursor,1))return false;
 p=update_fireworks_editor_controls(deletion);
 if(deletion.mode!=0||!has(p,Op::SetCursor,0))return false;
 deletion.selected_left=1;deletion.object_selected=true;
 deletion.cursor_x=110;deletion.cursor_y=30;
 p=update_fireworks_editor_controls(deletion);
 if(deletion.debounce!=10)return false;
 p=update_fireworks_editor_controls(deletion);
 if(deletion.debounce!=9||!p.early_return)return false;
 FireControls clear{};
 clear.regions=fire.regions;clear.cursor_x=250;
 clear.cursor_y=30;clear.click=true;
 p=update_fireworks_editor_controls(clear);
 if(!clear.confirmation_open||!has(p,Op::OpenConfirmation,3))
  return false;
 BandDraw draw{};
 if(!draw.score.place({0,0},0)||!draw.score.place({3,5},3))
  return false;
 draw.machines[0].mode=1;draw.machines[0].frame_count=2;
 draw.machines[0].tick=3;draw.frame_delta=1;
 p=draw_grand_opening_machines(draw);
 if(count(p,Op::DrawSprite)!=2||count(p,Op::DrawBandMachine)!=5||
    draw.machines[0].frame!=1||
    p.actions.front().op!=Op::DrawBitmap||
    p.actions.back().op!=Op::DrawBitmap)return false;
 draw.machines[0].tick=3;
 p=draw_grand_opening_machines(draw);
 if(draw.machines[0].frame!=0||draw.machines[0].mode!=0)return false;
 // Compare independent native implementations against recovered typed
 // Ghidra source models across deterministic mode/hover combinations.
 for(int i=0;i<128;++i){
   BandToolbar lhs{};
   lhs.buttons=t.buttons;
   lhs.cursor_x=25+100*(i%4);lhs.cursor_y=50;
   lhs.state=(i%9==0)?8:0;
   lhs.start_voice=200;lhs.last_hover=(i%3)-1;
   lhs.pressed=(i%2)==0;lhs.mouse_down=(i%4)==0;
   lhs.delete_mode=(i%5)==0;
   auto rhs=lhs;
   const auto left=update_grand_opening_toolbar(lhs,static_cast<U32>(i));
   const auto right=btb::native_gameplay_6::update_band_toolbar(
       rhs,static_cast<U32>(i));
   if(!equal(left,right)||lhs.delete_mode!=rhs.delete_mode||
      lhs.play_pending!=rhs.play_pending||lhs.state!=rhs.state||
      lhs.last_hover!=rhs.last_hover)return false;
 }
 return true;
}
}
