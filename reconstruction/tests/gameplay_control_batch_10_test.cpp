#include "btb/gameplay_presentation_batch_10.hpp"
#include <algorithm>
#include <cassert>
#include <array>
#include <vector>
using namespace btb::gameplay_control_10;
namespace {
int count(const Plan& f,Op op){
  return static_cast<int>(std::count_if(f.actions.begin(),f.actions.end(),
    [&](const Event& e){return e.op==op;}));
}
bool has(const Plan& p,Op op,I32 a){
  return std::any_of(p.actions.begin(),p.actions.end(),
    [&](const Event& e){return e.op==op&&e.a==a;});
}
}
int main(){
  // 00413F10: verify all eleven original Fireworks activity branches.
  FireState fire{};
  fire.grid[0]=3;fire.grid_reset_requested=true;
  auto f=update_fireworks(fire);
  assert(fire.grid[0]==-1&&count(f,Op::ResetCursor)==1);
  assert(count(f,Op::DrawFireEditor)==1);
  fire.intro_pending=true;f=update_fireworks(fire);
  assert(has(f,Op::StartMusic,4)&&has(f,Op::ManagedVoice,861));
  assert(!fire.intro_pending);
  f=update_fireworks(fire);
  assert(fire.intro_completed&&has(f,Op::ManagedVoice,898));
  fire.click=true;fire.hit_region=10;
  f=update_fireworks(fire);
  assert(f.returns_early&&has(f,Op::BeginFireAction,10));
  assert(has(f,Op::CompleteFireAction,10));
  fire.phase=1;f=update_fireworks(fire);
  assert(has(f,Op::CompleteFireAction,10)&&!has(f,Op::BeginFireAction,10));
  fire.phase=2;fire.selected_fire_type=4;
  f=update_fireworks(fire);
  assert(fire.phase==3&&fire.cached_movie_index==4&&f.returns_early);
  fire.movie_frame_complete=true;f=update_fireworks(fire);
  assert(fire.phase==4&&has(f,Op::RestartMovie,4));
  f=update_fireworks(fire);assert(fire.phase==0);
  fire.phase=8;f=update_fireworks(fire);
  assert(fire.phase==9&&has(f,Op::StartFireworksMovieBank,0));
  fire.phase=9;f=update_fireworks(fire);
  assert(count(f,Op::FireworksShowPlayback)==1);
  fire.phase=13;fire.click=true;fire.hit_region=12;
  f=update_fireworks(fire);
  assert(has(f,Op::DeleteFireCell,12));
  fire.phase=14;fire.managed_playing=true;
  f=update_fireworks(fire);
  assert(fire.phase==14&&count(f,Op::DrawFireEditor)==1);
  fire.managed_playing=false;f=update_fireworks(fire);
  assert(fire.phase==15&&has(f,Op::OpenCompletionMovie,0));
  fire.global_movie_complete=true;f=update_fireworks(fire);
  assert(fire.phase==8&&has(f,Op::ManagedVoice,142));
  fire.phase=16;f=update_fireworks(fire);
  assert(has(f,Op::DrawFireCertificate,0));
  fire.phase=17;f=update_fireworks(fire);
  assert(fire.outer_state==60&&fire.ui_context==36&&fire.replay_latch==0);
  assert(has(f,Op::SaveUnloadFireworks,0));
  FireState leaving{};leaving.shutdown=true;
  f=update_fireworks(leaving);
  assert(leaving.outer_state==4&&!leaving.shutdown);
  assert(f.actions.size()==1&&f.returns_early);

  // 0041F6E0: original palette selection and continuation-owner erasure.
  BandEditor band{};
  f=begin_band_action(band,3);
  assert(band.phase==1&&band.selected==3);
  assert(band.cursor_type==3&&has(f,Op::PlayBandPickSound,3));
  assert(band.machine_voice_latch[1]==2);
  assert(band.composition.place({1,4},3));
  f=begin_band_action(band,11,btb::bobs_band::Cell{1,5});
  assert(band.composition.at({1,4})==-1);
  assert(band.phase==1&&band.selected==3);
  assert(has(f,Op::RemoveBandEvent,1));
  band.delete_mode=true;
  assert(band.composition.place({2,2},0));
  f=begin_band_action(band,11,btb::bobs_band::Cell{2,2});
  assert(band.delete_handled&&band.composition.at({2,2})==-1);
  // 0041F550 palette selection toggles current type back to editor state 0.
  band.delete_mode=false;band.phase=1;band.selected=3;
  f=update_band_selected(band,3);
  assert(band.phase==0&&band.cursor_type==-1);
  f=update_band_selected(band,7);
  assert(band.phase==1&&band.selected==7);
  f=update_band_selected(band,11,btb::bobs_band::Cell{3,1});
  assert(band.composition.at({3,1})==7&&band.phase==0);
  band.selected=1;band.phase=1;
  assert(band.composition.place({2,8},4));
  f=update_band_selected(band,11,btb::bobs_band::Cell{2,8});
  assert(band.composition.at({2,8})==1);
  assert(band.selected==4&&has(f,Op::RemoveBandEvent,2));

  // 004282C0 help hover and mouse release: original 20 sounds stop.
  Help help{};
  help.current_input_mode=3;help.required_input_mode=3;
  help.sound_slots[0]=true;help.sound_slots[17]=true;
  help.pointer_state=1;
  f=update_help_button(help);
  assert(help.pointer_state==2);
  assert(count(f,Op::StopAllActivitySound)==2);
  help.input_flags=8;help.outer_state=1;help.substate=2;
  f=update_help_button(help);
  assert(help.hover_latch==1&&has(f,Op::ManagedVoice,455));
  assert(count(f,Op::DrawScene)==1);
  f=update_help_button(help);
  assert(count(f,Op::ManagedVoice)==0);
  help.input_flags=0;help.mouse_press=true;
  help.intro_hit={10,10,50,50};help.mouse_x=30;help.mouse_y=30;
  f=update_help_button(help);assert(help.pointer_state==1&&help.hover_latch==0);

  // 00429FE0 exact hit geometry comes from native retail table, not guess.
  Leave leave{};
  leave.retail_buttons={{{220,260,305,316},{316,260,390,316}}};
  leave.cursor_x=250;leave.cursor_y=290;leave.mouse_press=true;
  f=update_leave_confirmation(leave);
  assert(leave.game_shutdown&&leave.selected_response==-1);
  assert(count(f,Op::SetGlobalShutdown)==1);
  Leave cancel{};
  cancel.retail_buttons=leave.retail_buttons;cancel.cursor_x=350;
  cancel.cursor_y=280;cancel.mouse_press=true;
  f=update_leave_confirmation(cancel);
  assert(!cancel.game_shutdown&&count(f,Op::ResumeMovie)==1);
  Leave finished{};finished.phase=2;f=update_leave_confirmation(finished);
  assert(f.returns_early&&finished.phase==0);
  assert(count(f,Op::RestoreMouseBounds)==1);

  // 00429500 Play Again 6-region hit test uses inclusive boundaries.
  PlayAgain replay{};
  for(int i=0;i<6;++i)
    replay.options[static_cast<std::size_t>(i)]={i*30,0,i*30+20,20};
  replay.cursor_x=1;replay.cursor_y=0;
  replay.click_pulse=true;
  f=update_play_again(replay);assert(replay.selection==1);
  replay.mouse_press=true;f=update_play_again(replay);
  assert(replay.confirmed_context==2&&replay.selection==-1);
  PlayAgain replay_last{};
  replay_last.options=replay.options;
  replay_last.click_pulse=true;replay_last.mouse_press=true;
  replay_last.cursor_x=155;replay_last.cursor_y=10;
  f=update_play_again(replay_last);
  assert(replay_last.selection==-1&&f.returns_early);
  PlayAgain replay_close{};replay_close.phase=2;
  f=update_play_again(replay_close);
  assert(count(f,Op::ResumeMovie)==1&&f.returns_early);

  // 00422DD0 Spud Maze slides west and east, and completes at bounds.
  SpudSlide sp{};
  sp.source_screen=3;sp.destination_screen=2;
  sp.transition_direction=1;
  auto frame=update_spud_slide(sp);
  assert(sp.phase==2&&frame.step==2);
  assert(sp.left==616&&sp.new_right==618);
  assert(frame.blits[0].screen==3&&frame.blits[1].screen==2);
  for(int i=0;i<600&&!frame.completed;++i)frame=update_spud_slide(sp);
  assert(frame.completed&&sp.phase==0&&sp.speed>=10);
  SpudSlide sp_east{};sp_east.transition_direction=2;
  sp_east.source_screen=2;sp_east.destination_screen=3;
  frame=update_spud_slide(sp_east);
  assert(frame.step==2&&sp_east.left==22&&sp_east.new_right==24);
  for(int i=0;i<600&&!frame.completed;++i)frame=update_spud_slide(sp_east);
  assert(frame.completed&&sp_east.phase==0);

  // 00425780 squirrel scroll has four HUD border passes and reset latches.
  SquirrelSlide squirrel{};
  squirrel.fast_complete=true;
  auto squirrel_frame=update_squirrel_slide(squirrel,0);
  assert(squirrel_frame.completed&&squirrel.phase==0);
  assert(squirrel.actor_x==46&&squirrel.level_index==1);
  SquirrelSlide regular{};
  regular.distance_limit=50;
  squirrel_frame=update_squirrel_slide(regular,0);
  assert(squirrel_frame.draw_part&&squirrel_frame.draw_borders);
  assert(regular.motion==1&&regular.phase==2);
  for(int i=0;i<200&&!squirrel_frame.completed;++i)
    squirrel_frame=update_squirrel_slide(regular,0);
  assert(squirrel_frame.completed&&regular.level_index==1);

  // 0041BF80 source time is seconds from Windows 64-bit timestamp delta;
  // 20 px spacing, 250 px row rollover, 25 completed/empty bar cells.
  MazeTimer timer{};
  timer.initial_seconds=60;timer.seconds_in_tick=10;
  timer.countdown_tokens=15;timer.filled_tokens=7;
  auto timer_frame=draw_maze_timer(timer);
  assert(timer.remaining==50&&timer_frame.tokens.size()==15);
  assert(timer_frame.tokens[0].variant==1);
  assert(timer_frame.tokens[7].variant==0);
  assert(timer_frame.tokens[13].y==450);
  timer.previous_remaining=50;timer.seconds_in_tick=15;
  timer_frame=draw_maze_timer(timer);
  assert(timer.clock_bias==5&&timer.remaining==50);

  // 004138A0: certificate centered name glyphs; hover voice, print action.
  Certificate cert{};
  cert.name={{0,0,12},{1,0,8}};
  cert.tier=2;
  auto result=draw_certificate(cert,2,3);
  assert(result.glyph_x.size()==2&&result.glyph_x[0]==481);
  assert(result.glyph_x[1]==493);
  assert(has(result.effects,Op::DrawSurface,471));
  cert.mouse_x=320;cert.mouse_y=430;
  result=draw_certificate(cert);
  assert(has(result.effects,Op::ManagedVoice,143));
  cert.pointer_down=true;
  result=draw_certificate(cert);
  assert(count(result.effects,Op::PrintCertificate)==1);
  assert(result.effects.returns_early);
}
