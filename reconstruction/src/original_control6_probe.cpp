#include "btb/original_control6.hpp"
#include <algorithm>
#include <array>
namespace btb::original_control6 {
static int count(const Plan& plan,Op op){
 return static_cast<int>(std::count_if(plan.actions.begin(),plan.actions.end(),
     [&](const Event& e){return e.op==op;}));
}
static bool has(const Plan& plan,Op op,I32 value){
 return std::any_of(plan.actions.begin(),plan.actions.end(),
     [&](const Event& e){return e.op==op&&e.a==value;});
}
#define assert(expr) do {if(!(expr))passed=false;}while(false)
bool verify_6_original_gameplay_controls(){
 bool passed=true;

  // 00413F10: verify all eleven original Fireworks activity branches.
  FireState fire{};
  fire.grid[0]=3;fire.grid_reset_requested=true;
  fire.intro_pending=true;
  auto f=original_control6::update_fireworks(fire);
  assert(fire.grid[0]==-1&&count(f,Op::ResetCursor)==1);
  assert(count(f,Op::DrawFireEditor)==1);
  assert(has(f,Op::StartMusic,4)&&has(f,Op::ManagedVoice,861));
  assert(!fire.intro_pending);
  f=original_control6::update_fireworks(fire);
  assert(fire.intro_completed&&has(f,Op::ManagedVoice,898));
  fire.click=true;fire.hit_region=10;
  f=original_control6::update_fireworks(fire);
  assert(f.returns_early&&has(f,Op::BeginFireAction,10));
  assert(has(f,Op::CompleteFireAction,10));
  fire.phase=1;f=original_control6::update_fireworks(fire);
  assert(has(f,Op::CompleteFireAction,10)&&!has(f,Op::BeginFireAction,10));
  fire.phase=2;fire.selected_fire_type=4;
  f=original_control6::update_fireworks(fire);
  assert(fire.phase==3&&fire.cached_movie_index==4&&f.returns_early);
  fire.movie_frame_complete=true;f=original_control6::update_fireworks(fire);
  assert(fire.phase==4&&has(f,Op::RestartMovie,4));
  f=original_control6::update_fireworks(fire);assert(fire.phase==0);
  fire.phase=8;f=original_control6::update_fireworks(fire);
  assert(fire.phase==9&&has(f,Op::StartFireworksMovieBank,0));
  fire.phase=9;f=original_control6::update_fireworks(fire);
  assert(count(f,Op::FireworksShowPlayback)==1);
  fire.phase=13;fire.click=true;fire.hit_region=12;
  f=original_control6::update_fireworks(fire);
  assert(has(f,Op::DeleteFireCell,12));
  fire.phase=14;fire.managed_playing=true;
  f=original_control6::update_fireworks(fire);
  assert(fire.phase==14&&count(f,Op::DrawFireEditor)==1);
  fire.managed_playing=false;f=original_control6::update_fireworks(fire);
  assert(fire.phase==15&&has(f,Op::OpenCompletionMovie,0));
  fire.global_movie_complete=true;f=original_control6::update_fireworks(fire);
  assert(fire.phase==8&&has(f,Op::ManagedVoice,142));
  fire.phase=16;f=original_control6::update_fireworks(fire);
  assert(has(f,Op::DrawFireCertificate,0));
  fire.phase=17;f=original_control6::update_fireworks(fire);
  assert(fire.outer_state==60&&fire.ui_context==36&&fire.replay_latch==0);
  assert(has(f,Op::SaveUnloadFireworks,0));
  FireState leaving{};leaving.shutdown=true;
  f=original_control6::update_fireworks(leaving);
  assert(leaving.outer_state==4&&!leaving.shutdown);
  assert(f.actions.size()==1&&f.returns_early);

  // 0041F6E0: original palette selection and continuation-owner erasure.
  BandEditor band{};
  f=original_control6::begin_band_action(band,3);
  assert(band.phase==1&&band.selected==3);
  assert(band.cursor_type==3&&has(f,Op::PlayBandPickSound,3));
  assert(band.machine_voice_latch[1]==2);
  assert(band.composition.place({1,4},3));
  f=original_control6::begin_band_action(band,11,btb::bobs_band::Cell{1,5});
  assert(band.composition.at({1,4})==-1);
  assert(band.phase==1&&band.selected==3);
  assert(has(f,Op::RemoveBandEvent,1));
  band.delete_mode=true;
  assert(band.composition.place({2,2},0));
  f=original_control6::begin_band_action(band,11,btb::bobs_band::Cell{2,2});
  assert(band.delete_handled&&band.composition.at({2,2})==-1);
  // 0041F550 palette selection toggles current type back to editor state 0.
  band.delete_mode=false;band.phase=1;band.selected=3;
  f=original_control6::update_band_selected(band,3);
  assert(band.phase==0&&band.cursor_type==-1);
  f=original_control6::update_band_selected(band,7);
  assert(band.phase==1&&band.selected==7);
  f=original_control6::update_band_selected(band,11,btb::bobs_band::Cell{3,1});
  assert(band.composition.at({3,1})==7&&band.phase==0);
  band.selected=1;band.phase=1;
  assert(band.composition.place({2,8},4));
  f=original_control6::update_band_selected(band,11,btb::bobs_band::Cell{2,8});
  assert(band.composition.at({2,8})==1);
  assert(band.selected==4&&has(f,Op::RemoveBandEvent,2));

  // 004282C0 help hover and mouse release: original 20 sounds stop.
  Help help{};
  help.current_input_mode=3;help.required_input_mode=3;
  help.sound_slots[0]=true;help.sound_slots[17]=true;
  help.pointer_state=1;
  f=original_control6::update_help_button(help);
  assert(help.pointer_state==2);
  assert(count(f,Op::StopAllActivitySound)==2);
  help.input_flags=8;help.outer_state=1;help.substate=2;
  f=original_control6::update_help_button(help);
  assert(help.hover_latch==1&&has(f,Op::ManagedVoice,455));
  assert(count(f,Op::DrawScene)==1);
  f=original_control6::update_help_button(help);
  assert(count(f,Op::ManagedVoice)==0);
  help.input_flags=0;help.mouse_press=true;
  help.intro_hit={10,10,50,50};help.mouse_x=30;help.mouse_y=30;
  f=original_control6::update_help_button(help);assert(help.pointer_state==1&&help.hover_latch==0);

  // 00429FE0 exact hit geometry comes from native retail table, not guess.
  Leave leave{};
  leave.retail_buttons={{{220,260,305,316},{316,260,390,316}}};
  leave.cursor_x=250;leave.cursor_y=290;leave.mouse_press=true;
  f=original_control6::update_leave_confirmation(leave);
  assert(leave.game_shutdown&&leave.selected_response==-1);
  assert(count(f,Op::SetGlobalShutdown)==1);
  Leave cancel{};
  cancel.retail_buttons=leave.retail_buttons;cancel.cursor_x=350;
  cancel.cursor_y=280;cancel.mouse_press=true;
  f=original_control6::update_leave_confirmation(cancel);
  assert(!cancel.game_shutdown&&count(f,Op::ResumeMovie)==1);
  Leave finished{};finished.phase=2;f=original_control6::update_leave_confirmation(finished);
  assert(f.returns_early&&finished.phase==0);
  assert(count(f,Op::RestoreMouseBounds)==1);

  // 00429500 Play Again 6-region hit test uses inclusive boundaries.
  PlayAgain replay{};
  for(int i=0;i<6;++i)
    replay.options[static_cast<std::size_t>(i)]={i*30,0,i*30+20,20};
  replay.cursor_x=1;replay.cursor_y=0;
  replay.click_pulse=true;
  f=original_control6::update_play_again(replay);assert(replay.selection==1);
  replay.mouse_press=true;f=original_control6::update_play_again(replay);
  assert(replay.confirmed_context==2&&replay.selection==-1);
  PlayAgain replay_last{};
  replay_last.options=replay.options;
  replay_last.click_pulse=true;replay_last.mouse_press=true;
  replay_last.cursor_x=155;replay_last.cursor_y=10;
  f=original_control6::update_play_again(replay_last);
  assert(replay_last.selection==-1&&f.returns_early);
  PlayAgain replay_close{};replay_close.phase=2;
  f=original_control6::update_play_again(replay_close);
  assert(count(f,Op::ResumeMovie)==1&&f.returns_early);

  
 return passed;
}
}
