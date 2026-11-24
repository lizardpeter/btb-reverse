#pragma once
// Original visual-control functions. Rendering is a replayable ordered plan;
// the native game's DirectDraw host owns the actual surfaces and blits.
#include "btb/gameplay_control_batch_10.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::gameplay_control_10 {

// 00422DD0: Spud Maze authored east/west sliding compositor, preserving
// separate source rects, player offsets and speed ramp (+1 to max 200).
struct SpudSlide{
  I32 phase{1};                  // 00512128
  I32 destination_screen{},source_screen{};
  I32 transition_direction{};    // 005144B4 == 2 moves toward east
  I32 running_direction{};       // 00514534 -1 west, +1 east
  I32 left{},right{},new_left{},new_right{};
  I32 speed{10};
  I32 actor_offset_a{},actor_offset_b{};
  bool slide_visual_active{true};
};
struct SlideBlit {
  I32 screen{},destination_x{},destination_y{},source_left{},source_top{},
      source_right{},source_bottom{};
  friend bool operator==(const SlideBlit&,const SlideBlit&)=default;
};
struct SlideFrame{
  std::array<SlideBlit,2> blits{};
  I32 step{};
  bool completed{};
};
[[nodiscard]] inline SlideFrame update_spud_slide(SpudSlide& s){
  SlideFrame out{};
  if(s.phase==1) {
    s.phase=2;s.speed=10;
    if(s.transition_direction!=2){
      s.left=618;s.right=619;s.new_left=20;s.new_right=620;
      s.running_direction=-1;
    } else{
      s.left=20;s.right=619;s.new_left=20;s.new_right=22;
      s.running_direction=1;
    }
  }
  out.step=s.speed/5;
  if(s.running_direction==1){
    s.left+=out.step;s.new_right+=out.step;
    const I32 remaining=619-s.new_right;
    s.actor_offset_a=20-s.left;
    s.actor_offset_b=remaining;
    if(s.new_right>618){
      s.running_direction=0;s.phase=0;s.slide_visual_active=false;
      out.completed=true;
    }
    out.blits[0]={s.source_screen,20,20,s.left,20,s.right,400};
    out.blits[1]={s.destination_screen,(s.right-s.left)+20,20,
                  s.new_left,20,s.new_right,400};
    if(s.speed<200)++s.speed;
    if(s.speed>remaining)s.speed=remaining;
  } else {
    s.left-=out.step;s.new_right-=out.step;
    s.actor_offset_a=s.right-s.left;
    s.actor_offset_b=20-s.left;
    const I32 remaining=s.new_right-20;
    if(s.new_right<21){
      s.running_direction=0;s.phase=0;s.slide_visual_active=false;
      out.completed=true;
    }
    out.blits[0]={s.source_screen,20,20,s.left,20,s.right,400};
    out.blits[1]={s.destination_screen,(s.right-s.left)+20,20,
                  s.new_left,20,s.new_right,400};
    if(s.speed<200)++s.speed;
    if(s.speed>remaining)s.speed=remaining;
  }
  if(s.speed<10)s.speed=10;
  return out;
}

// 00425780: Squirrel-level transition has a distinct speed acceleration,
// level-index reset, and 4 full-screen border surfaces (not Spud's slide).
struct SquirrelSlide{
  I32 phase{1},motion{0};       // 005150D8, 00515108
  I32 destination_y{},source_y{}; // 00514F40, 00514F48
  I32 slide_origin{},sprite_offsets{}; // 0051510C, 005150F0
  I32 speed{10};                // 00446A48
  I32 distance_limit{600};      // 004467D4-004467BC, source-authored
  I32 layer_index{},actor_x{},actor_y{},level_index{};
  bool fast_complete{};
  bool screen_cleared{};
  I32 next_actor_mode{};
  bool idle_flag{};
};
struct SquirrelSlideFrame{
  bool completed{},draw_part{},draw_borders{};
  I32 part_source_y{},requested_y{};
  std::array<std::pair<I32,I32>,4> border_origins{};
};
[[nodiscard]] inline SquirrelSlideFrame update_squirrel_slide(
    SquirrelSlide& s,I32 incoming_step_hint) {
  SquirrelSlideFrame out{};
  if(s.phase==1){
    if(s.fast_complete){
      s.motion=0;s.phase=0;s.layer_index=0;
      s.level_index++;s.actor_x=46;s.next_actor_mode=1;
      s.idle_flag=true;out.completed=true;return out;
    }
    s.phase=2;s.sprite_offsets=0;s.motion=1;
    s.slide_origin=s.destination_y;
  } else if(s.motion<1) {
    // This branch immediately jumps into the original speed ramp, without
    // altering scroll state.
    // Retail only increments speed while it is BELOW 10, rather than
    // accelerating unconditionally. The incoming param controls the cap.
    if(s.speed<10)++s.speed;
    if(incoming_step_hint<s.speed)s.speed=incoming_step_hint;
    if(s.speed<10)s.speed=10;
    s.sprite_offsets+=s.speed;
    out.draw_borders=true;
    out.border_origins={{{0,0},{0,20},{620,20},{20,400}}};
    return out;
  }
  s.layer_index++;
  const I32 delta=s.speed/5;
  s.source_y+=delta;s.destination_y+=delta;
  const I32 remainder=s.distance_limit-s.destination_y+s.slide_origin;
  out.requested_y=remainder;
  if(s.distance_limit+s.slide_origin<=s.destination_y){
    s.level_index++;s.motion=0;s.phase=0;s.layer_index=0;
    s.actor_x+=(-(s.sprite_offsets/5));
    s.next_actor_mode=1;s.idle_flag=true;out.completed=true;
  }
  out.draw_part=true;out.part_source_y=s.destination_y;
  if(s.speed<10)++s.speed;
  if(s.speed>remainder)s.speed=remainder;
  if(s.speed<10)s.speed=10;
  s.sprite_offsets+=s.speed;
  out.draw_borders=true;
  out.border_origins={{{0,0},{0,20},{620,20},{20,400}}};
  (void)incoming_step_hint;
  return out;
}

// 0041BF80: Maze timer computes signed seconds from FILETIME elapsed,
// corrects clock bias when the timer has skipped more than one second,
// then renders exactly the source-authored number of tokens across two rows.
// The original __ftol digit glyph is represented as a host-provided digit.
struct MazeTimer {
  I32 remaining{},previous_remaining{};
  I32 initial_seconds{},clock_bias{},countdown_tokens{},filled_tokens{25};
  I32 seconds_in_tick{};
  I32 glyph_digit{};
};
struct TimerToken{I32 x{},y{},variant{};};
struct MazeTimerFrame {
  std::vector<TimerToken> tokens{};
  I32 remaining{},total_filled{25};
};
[[nodiscard]] inline MazeTimerFrame draw_maze_timer(MazeTimer& state) {
  MazeTimerFrame frame{};
  state.remaining=state.initial_seconds+state.clock_bias-state.seconds_in_tick;
  if(state.previous_remaining-state.remaining>1){
    state.clock_bias+=state.previous_remaining-state.remaining;
    state.remaining=state.initial_seconds+state.clock_bias-state.seconds_in_tick;
  }
  state.previous_remaining=state.remaining;
  frame.remaining=state.remaining;
  // Native loads the bar length from 00444224[difficulty]. Exact source
  // uses 20px spacing and wraps after x offset 250.
  I32 xoffset=0,yoffset=0;
  const I32 start_x=244-(state.countdown_tokens/2)*20;
  for(I32 i=0;i<state.countdown_tokens;++i){
    if(xoffset>250){xoffset=0;yoffset=20;}
    frame.tokens.push_back({start_x+xoffset,430+yoffset,
                           i<state.filled_tokens?1:0});
    xoffset+=20;
  }
  return frame;
}

// 004138A0: certificate header + glyph row centered at x=491, numeric
// category tier at x=471, then hover/print surface controller.
struct Glyph {I32 sprite_index{},left{},right{};};
struct Certificate {
  std::vector<Glyph> name{};
  I32 tier{-1};
  I32 mouse_x{},mouse_y{};
  bool pointer_down{},pointer_pressed{};
  I32 hover_latch{},voice_latch{},last_idle_voice{-1};
  bool managed_sound_playing{};
};
struct CertificateFrame{
  Plan effects{};
  std::vector<I32> glyph_x{};
};
[[nodiscard]] inline CertificateFrame draw_certificate(
    Certificate& s,U32 random_voice_index=0,U32 random_play_index=0){
  CertificateFrame out{};
  auto& f=out.effects;
  f.emit(Op::DrawScene,0); // 0050A5CC background
  I32 total_width=0;
  for(const auto& g:s.name)total_width+=g.right-g.left;
  I32 x=491-total_width/2;
  for(const auto& g:s.name){
    out.glyph_x.push_back(x);
    f.emit(Op::DrawSurface,x,135,g.sprite_index);
    x+=g.right-g.left;
  }
  if(s.tier>=0)f.emit(Op::DrawSurface,471,156,s.tier*50);
  const bool button=s.mouse_x>296&&s.mouse_x<348&&
                    s.mouse_y>418&&s.mouse_y<466;
  if(s.pointer_down){
    if(button){
      f.emit(Op::StopManaged);
      f.emit(Op::DrawScene,1);
      f.emit(Op::PrintCertificate);f.returns_early=true;
    }
  } else if(button){
    f.emit(Op::DrawSurface,292,417,s.pointer_pressed?1:0);
    if(s.hover_latch==0){
      f.emit(Op::ManagedVoice,143,50,2);s.hover_latch=1;
      f.returns_early=true;
    }
  } else {
    s.hover_latch=0;
    if(!s.managed_sound_playing&&s.voice_latch==0){
      const I32 candidate=144+static_cast<I32>(random_voice_index%8U);
      if(candidate!=s.last_idle_voice){
        s.voice_latch=1;s.last_idle_voice=candidate;
        f.emit(Op::ManagedVoice,144+static_cast<I32>(random_play_index%8U),50,0);
      }
    }
  }
  return out;
}

} // namespace btb::gameplay_control_10
