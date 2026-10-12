#include "btb/native_gameplay_batch_6.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
using namespace btb::native_gameplay_6;
static int count(const Plan& p,Op op){
 return static_cast<int>(std::count_if(p.actions.begin(),p.actions.end(),
    [&](const Effect& e){return e.op==op;}));}
static bool has(const Plan& p,Op op,I32 slot){
 return std::any_of(p.actions.begin(),p.actions.end(),
    [&](const Effect& e){return e.op==op&&e.a==slot;});}
int main(){
 // 004145F0 original Golf asset setup, difficulty and 18 bitmap slots.
 GolfInit golf{};
 btb::golf::Data data{};
 data.bob_position={100,200};data.ball_offset_from_bob_retail={11,12};
 auto p=initialize_golf(golf,data);
 assert((golf.ball==btb::golf::Vec2i{111,212}));
 assert(golf.difficulty_seeds[0]==5&&golf.difficulty_seeds[2]==9);
 assert(golf.course_state==5&&golf.intro_phase==1);
 assert(count(p,Op::LoadBitmap)==18);
 assert(count(p,Op::RegisterBitmap)==18);
 assert(count(p,Op::MarkColorKey)==17);
 assert(has(p,Op::InstallUnload,0x00414410));
 assert(p.actions.back().op==Op::EnableIntro);
 // 00424360 five retail Bink resources each <=7MB, 160 sound slots.
 SkateInit skate{};
 btb::spud_skate::TimingData timing{};
 timing.stunt_count=8;timing.loop_start_frame=100;
 std::array<std::string,5> paths{"bad.bik","okay.bik","good.bik","end.bik","special.bik"};
 int visited=0;
 p=initialize_skate(skate,timing,paths,
   [&](const std::string& name,std::size_t capacity)->std::optional<std::vector<U8>>{
    assert(capacity==7000000);
    assert(name==paths[static_cast<std::size_t>(visited++)]);
    return std::vector<U8>(static_cast<std::size_t>(visited),0x42);
   });
 assert(skate.valid&&skate.intro_pending);
 assert(skate.timing.loop_start_frame==100);
 assert(visited==5&&count(p,Op::LoadMovie)==5);
 assert(count(p,Op::CreateMovieSurface)==5&&count(p,Op::OpenBink)==5);
 assert(count(p,Op::LoadBitmap)==5);
 for(const auto id:skate.sound_ids)assert(id==-1);
 SkateInit missing{};
 p=initialize_skate(missing,timing,paths,
   [](const std::string&,std::size_t)->std::optional<std::vector<U8>>{
      return std::nullopt;
   });
 assert(p.failed&&!missing.valid);
 // 0041F820 native Band four-button source toolbar and sound latch
 BandToolbar toolbar{};
 toolbar.buttons={{{0,0,100,100},{100,0,200,100},
                  {200,0,300,100},{300,0,400,100}}};
 toolbar.cursor_x=50;toolbar.cursor_y=50;toolbar.start_voice=700;
 p=update_band_toolbar(toolbar);
 assert(has(p,Op::StartManagedVoice,706));
 assert(toolbar.last_hover==0);
 p=update_band_toolbar(toolbar);
 assert(count(p,Op::StartManagedVoice)==0);
 toolbar.pressed=true;
 p=update_band_toolbar(toolbar);
 assert(toolbar.play_pending&&has(p,Op::StartManagedVoice,701));
 toolbar.pressed=false;toolbar.managed_playing=true;
 p=update_band_toolbar(toolbar);
 assert(toolbar.state==0&&toolbar.play_pending);
 toolbar.managed_playing=false;
 p=update_band_toolbar(toolbar);
 assert(toolbar.state==8&&!toolbar.play_pending&&p.early_return);
 BandToolbar erase{};
 erase.buttons=toolbar.buttons;erase.cursor_x=350;erase.cursor_y=50;
 erase.pressed=true;erase.start_voice=800;
 p=update_band_toolbar(erase,1);
 assert(erase.delete_mode&&has(p,Op::SetCursor,1));
 assert(has(p,Op::StartManagedVoice,810));
 p=update_band_toolbar(erase,1);
 assert(!erase.delete_mode&&has(p,Op::SetCursor,0));
 BandToolbar confirm{};
 confirm.buttons=toolbar.buttons;confirm.cursor_x=250;confirm.cursor_y=50;
 confirm.pressed=true;
 p=update_band_toolbar(confirm);
 assert(has(p,Op::OpenConfirmation,2));
 // 0041FC90 native composition audio and note region clipping
 BandPlayback playback{};
 assert(playback.score.place({0,0},0));
 assert(playback.score.place({1,1},1));
 assert(playback.score.place({4,2},8));
 playback.elapsed_centiseconds=10;
 p=play_draw_band(playback);
 assert(count(p,Op::DrawBandGrid)==1);
 assert(has(p,Op::BlitNote,0));assert(count(p,Op::PlaySoundDirect)==0);
 playback.elapsed_centiseconds=100;
 p=play_draw_band(playback);
 assert(count(p,Op::PlaySoundDirect)==1);
 assert(has(p,Op::PlaySoundDirect,31));
 assert(count(p,Op::ResetSound)==1);
 playback.elapsed_centiseconds=200;
 p=play_draw_band(playback);
 assert(has(p,Op::PlaySoundDirect,8));
 assert(playback.last_centiseconds==200);
 playback.elapsed_centiseconds=2400;
 p=play_draw_band(playback);
 assert(count(p,Op::BlitNote)==0);
 // 00413B60 fireworks editor controls click and hover, 10 tick debounce
 FireControls fire{};
 fire.regions[0]={btb::fireworks::Recti{0,0,100,100},27};
 fire.regions[1]={btb::fireworks::Recti{100,0,200,100},26};
 fire.regions[2]={btb::fireworks::Recti{200,0,300,100},25};
 fire.cursor_x=50;fire.cursor_y=40;
 p=update_firework_controls(fire);
 assert(has(p,Op::StartManagedVoice,881));
 fire.click=true;
 p=update_firework_controls(fire);
 assert(fire.mode==14&&has(p,Op::StartManagedVoice,880));
 FireControls deletion{};
 deletion.regions=fire.regions;deletion.cursor_x=150;deletion.cursor_y=50;
 deletion.click=true;
 p=update_firework_controls(deletion);
 assert(deletion.mode==13&&has(p,Op::SetCursor,1));
 p=update_firework_controls(deletion);
 assert(deletion.mode==0&&has(p,Op::SetCursor,0));
 deletion.selected_left=1;deletion.object_selected=true;
 deletion.cursor_x=110;deletion.cursor_y=30;
 p=update_firework_controls(deletion);
 assert(deletion.debounce==10);
 p=update_firework_controls(deletion);
 assert(deletion.debounce==9&&p.early_return);
 FireControls clear{};
 clear.regions=fire.regions;clear.cursor_x=250;clear.cursor_y=30;
 clear.click=true;
 p=update_firework_controls(clear);
 assert(clear.confirmation_open&&has(p,Op::OpenConfirmation,3));
 // 0041F090 120-cell Bob's Band draw and five machine animations.
 BandDraw draw{};
 assert(draw.score.place({0,0},0));
 assert(draw.score.place({3,5},3));
 draw.machines[0].mode=1;draw.machines[0].frame_count=2;
 draw.machines[0].tick=3;draw.frame_delta=1;
 p=draw_band_machines(draw);
 assert(count(p,Op::DrawSprite)==2);
 assert(count(p,Op::DrawBandMachine)==5);
 assert(draw.machines[0].frame==1);
 assert(p.actions.front().op==Op::DrawBitmap);
 assert(p.actions.back().op==Op::DrawBitmap);
 draw.machines[0].tick=3;
 p=draw_band_machines(draw);
 assert(draw.machines[0].frame==0&&draw.machines[0].mode==0);
}
