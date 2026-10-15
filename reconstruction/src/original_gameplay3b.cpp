#include "btb/original_gameplay3b.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
namespace btb::original_gameplay3b {
namespace {
I32 sample(U32 rng,I32 count) {
 if(count<=0)throw std::invalid_argument("source random modulus");
 return static_cast<I32>(rng%static_cast<U32>(count));
}
Point round_point(float x,float y){return {static_cast<I32>(x),static_cast<I32>(y)};}
void sound(GolfIO& io,I32 id){io.play_sound(id,0x32,1);}
I32 golf_band(const Golf& g) {
 if(g.shot_angle<g.radius_a)return 0;
 return g.shot_angle<g.radius_b?1:2;
}
I32 rebound_feedback(Golf& g,GolfIO& io) {
 if(g.remaining_shots==1)return g.score<5?0x87:0x85+sample(io.random32(),2);
 if(g.hole>=0)return 0x66+sample(io.random32(),12);
 const I32 band=golf_band(g);
 const I32 error=std::abs(g.band_angle[static_cast<std::size_t>(band)]-g.shot_angle);
 if(error>(band==2?6:10))return sample(io.random32(),2)==0?0x75:0x72;
 const auto& h=g.holes[static_cast<std::size_t>(band)];
 const float dist=io.original_distance(
   {h.x+h.offset_x,h.y+h.offset_y},round_point(g.ball_x,g.ball_y));
 if(dist>=g.close_threshold) {
  if(g.ball_x<static_cast<float>(h.x+h.offset_x))return 0x83+sample(io.random32(),2);
  return 0x81+sample(io.random32(),2);
 }
 return 0x77+sample(io.random32(),4);
}
void hit_golf_targets(Golf& s,GolfIO& io){
 for(std::size_t i=0;i<s.holes.size();++i) {
  const auto& h=s.holes[i];
  const float d=io.original_distance(round_point(s.ball_x,s.ball_y),{h.x+h.offset_x,h.y+h.offset_y});
  if(d<s.goal_threshold) {
   ++s.phase;++s.hits;s.velocity=0;
  }
 }
}
std::size_t check_index(I32 idx,std::size_t cap,const char* reason){
 if(idx<0||static_cast<std::size_t>(idx)>=cap)throw std::out_of_range(reason);
 return static_cast<std::size_t>(idx);
}
I32 face(I32 angle){return (angle+22)/45%8;}
bool inside_box(const HerdingAnimal& a,Box b,HerdingIO& io,I32 x,I32 y){
 return io.rectangles_intersect({x-20,y-18,x+20,y+18},b);
}
}
I32 update_golf_gameplay(Golf& s,GolfIO& io) {
 switch(s.phase){
 case 0: {
  if(s.mouse_confirm){s.phase=1;s.hole=-1;s.bounce_amount=-1;}
  s.shot_angle=std::clamp(88-s.cursor_x/7,0,88);
  if((s.input_mask&10)!=0){
   const auto next=s.shot_angle-1;
   s.shot_angle=std::max(0,next);
   return next;
  }
  if((s.input_mask&5)!=0){
   const auto next=s.shot_angle+1;
   s.shot_angle=std::min(88,next);
   return next;
  }
  return s.shot_angle;
 }
 case 1: {
  if(s.difficulty<0||s.difficulty>15)throw std::out_of_range("golf difficulty");
  // 00415080: charge increments by difficulty-scaled pulse, reverses
  // when the charge leaves the native 0..1000 range.
  // The original indexes a difficulty-dependent integer table at DAT_0050adf0.
  // Do not invent retail constants: require the native data-table provider.
  const I32 charge=io.original_charge_step(s.difficulty);
  if(charge<0)throw std::invalid_argument("negative original Golf charge increment");
  s.power+=charge*s.power_step;
  if(s.power>1000){s.power=1000;s.power_step=-s.power_step;}
  else if(s.power<0){s.power=0;s.power_step=-s.power_step;}
  if(!s.mouse_down)return s.power_step;
  s.bounce_amount=0;s.phase=2;
  return s.power_step;
 }
 case 2: {
  s.velocity=s.power/3+500;
  s.bounce_power=((s.power/2+500)*16)/1000;
  s.remaining_energy=s.bounce_power;
  s.landed_phase=io.original_launch_angle(s)*2;
  s.delay_count=0;s.phase=99;
  return s.landed_phase;
 }
 case 99:
  if(++s.delay_count<9)return s.delay_count;
  s.delay_count=0;
  if(++s.ball_anim<8)return s.ball_anim;
  s.phase=3;return s.ball_anim;
 case 3:{
  if(++s.delay_count>8){s.delay_count=0;s.ball_anim=(s.ball_anim+1)%std::max(1,s.ball_anim_max);}
  const U32 event_index=static_cast<U32>(s.frame_counter++);
  if((s.mode==0?(event_index%2U==0U):(event_index%10U==0U)))
   return s.frame_counter;
  const Vec2 step=io.original_vector_from_degrees(s.landed_phase,s.velocity/80);
  s.ball_x+=step.x;s.ball_y-=step.y;
  const I32 energy_loss=io.original_energy_loss(s);
  s.velocity-=std::max(10,energy_loss);
  if(s.velocity<20){s.velocity=0;s.phase=4;}
  if(s.remaining_energy>0||s.bounce_amount>0){
   if(s.bounce!=1){
    const I32 decrement=std::min(8,s.remaining_energy);
    s.bounce_amount-=decrement;
    if(s.bounce_amount<=0){
     if(s.hits<static_cast<I32>(s.landing_marks.size()))
      s.landing_marks[static_cast<std::size_t>(s.hits)] = round_point(s.ball_x,s.ball_y);
     ++s.hits;
     s.bounce_power-=s.bounce_power/3;
     s.remaining_energy=0;s.bounce=1;
     s.bounce_amount=s.bounce_power;
    }
   }else{
    s.bounce_amount+=std::min(8,s.remaining_energy);
    if(--s.remaining_energy<=0){s.bounce=-1;s.remaining_energy=s.bounce_power;}
   }
  }
  hit_golf_targets(s,io);
  return s.velocity;
 }
 case 4:{
  s.ball_anim=0;s.hole=-1;
  for(I32 i=0;i<3;++i) {
   const auto& h=s.holes[static_cast<std::size_t>(i)];
   if(io.original_distance(round_point(s.ball_x,s.ball_y),{h.x+h.offset_x,h.y+h.offset_y})<s.goal_threshold){
    s.ball_x=static_cast<float>(h.x+h.offset_x);
    s.ball_y=static_cast<float>(h.y+h.offset_y);s.hole=i;
   }
  }
  s.phase=5;return s.phase;
 }
 case 5:{
  if(s.hole>=0)s.score+=1+s.hole*2;
  const I32 voice=rebound_feedback(s,io);
  sound(io,voice);s.last_voice=voice;
  s.bounce_amount=0;s.phase=6;return s.phase;
 }
 case 6:
  --s.remaining_shots;s.power=0;s.ball_x=static_cast<float>(s.start_x);
  s.ball_y=static_cast<float>(s.start_y);s.phase=0;s.power_step=1;
  return s.remaining_shots;
 default:return s.phase;
 }
}
void update_and_draw_squirrel_run(SquirrelRun& s,SquirrelRunIO& io){
 // Original 00425F40 uses a fixed 91-entry profile/direction mapping.
 // The table is defined explicitly below from local_16c[0..0x5a].
 static constexpr std::array<I32,91> code = {
0,0,3,3,5,5,4,4,2,2,0,0,1,1,1,1,1,1,1,0,0,0,1,6,2,0,0,6,3,3,3,5,7,4,5,0,0,1,0,6,0,0,0,7,5,5,5,5,5,5,7,1,0,0,0,0,0,0,1,1,2,3,3,6,3,3,3,0,1,4,0,0,0,1,0,1,3,3,3,7,5,5,5,0,0,0,1,1,0,0,0
 };
 if(s.cadence<=0||s.sprite_width<=0||s.sprite_height<=0)
  throw std::invalid_argument("invalid source Squirrel animation cadence or sprite");
 if(s.profile<0||s.profile>=4)throw std::out_of_range("Squirrel motion profile");
 auto tick=[&](I32 cap){
  if(--s.countdown>0)return false;
  s.countdown=s.cadence;
  ++s.frame;
  return s.frame>cap;
 };
 auto move=[&](bool descending){
  const I32 shape=code[check_index(s.source_profile[static_cast<std::size_t>(s.direction)]+
                                    s.profile_mark,code.size(),"Squirrel original motion-map index")];
  const auto next=io.original_frame_offset(shape,s.frame,descending);
  s.offset_y+=next;
  s.offset_x+=s.step_x[check_index(s.frame,s.step_x.size(),"Squirrel X animation table")];
 };
 if(s.phase==0){
  if(s.completed_cycles<s.reset_run){
   if(tick(0)){
    s.sequence=std::clamp(s.sequence+1,s.min_run,s.max_run);
    s.frame=0;
    if(s.sequence>s.max_run){
     s.sequence=s.min_run+sample(io.random32(),4);
    }
   }
  }else{
   io.select_original_motion_profile(s);
   s.phase=1;s.sequence=0;s.frame=0;s.direction=0;
   s.countdown=s.cadence;s.profile_mark=s.motion_variant;
  }
 }else if(s.phase==1){
  if(s.countdown==s.cadence)move(false);
  if(tick(2)){
   s.phase=2;s.frame=0;s.sequence=3;
  }
 }else if(s.phase==2){
  io.no_op_legacy_hook();
  s.phase=3;s.frame=0;s.sequence=3;
  s.countdown=s.cadence;
 }else if(s.phase==3){
  if(s.countdown==s.cadence)move(true);
  if(tick(3)){
   s.phase=4;s.frame=0;s.direction=2;s.sequence=0;
  }
 }else if(s.phase==4){
  if(s.countdown==s.cadence)move(false);
  if(tick(2)){
   s.phase=5;s.frame=0;s.direction=3;s.sequence=3;
  }
 }else if(s.phase==5){
  s.phase=6;s.frame=0;s.sequence=3;s.countdown=s.cadence;
 }else if(s.phase==6){
  if(s.countdown==s.cadence)move(true);
  if(tick(3)){
   s.phase=0;++s.completed_cycles;s.frame=0;
  }
 }else throw std::invalid_argument("unknown Squirrel source animation phase");
 const I32 mapped=code[check_index(s.sequence,code.size(),"Squirrel source sequence")];
 const I32 sheet_frame=io.original_sequence_base(mapped)+s.frame;
 s.clip_frame=sheet_frame*s.sprite_width;
 if(sheet_frame>62)s.show_overlay=true;
 if(!io.original_visibility_gate(s.round,s.branch,(s.completed_cycles%2)==0))return;
 if(s.phase>=4&&s.show_overlay)return;
 if(s.sprite_surface==0)throw std::runtime_error("Squirrel original sprite missing");
 const Box rect{s.clip_frame,0,s.clip_frame+s.sprite_width,s.sprite_height};
 s.last_motion_tick=s.sequence+s.frame;
 io.original_blt(s.x+s.offset_x,s.y+s.offset_y,s.sprite_surface,rect,true);
}
I32 update_herding_activity(Herding& s,HerdingIO& io){
 io.enable_input_processing();
 if(s.quit_requested||s.menu_requested){
  io.unload_herding_activity();
  if(s.menu_requested){s.menu_requested=false;s.game_state=4;}
  if(s.saved_progress!=-1)s.game_state=0x40;
  return 1;
 }
 s.prev_position_x=s.position_x;s.prev_position_y=s.position_y;
 s.last_x=s.current_x;s.last_y=s.current_y;
 I32 changed_axes=0;s.direction_mask=0;
 if(s.pointer_controls){
  if(s.input_mask!=0)s.pointer_controls=false;
  const Point target{
   s.current_x - static_cast<I32>(s.pointer_offset_x),
   s.current_y - static_cast<I32>(s.pointer_offset_y)};
  if(io.distance({s.cursor_x,s.cursor_y},target)>s.pointer_tolerance){
   const I32 angle=io.angle_degrees(target,{s.cursor_x,s.cursor_y});
   s.facing=face(angle);
   if(s.motion_enabled){
    const float rad=static_cast<float>(angle)*0.017453292519943295f;
    s.velocity_x+=std::sin(rad)*s.acceleration;
    s.velocity_y-=std::cos(rad)*s.acceleration;
   }
   s.direction_mask|=2;
  }
 }else{
  if(s.cursor_x!=s.last_cursor_x||s.cursor_y!=s.last_cursor_y)
   s.pointer_controls=true;
  s.last_cursor_x=s.cursor_x;s.last_cursor_y=s.cursor_y;
  if((s.input_mask&2)!=0){s.direction_mask|=2;++changed_axes;if(s.motion_enabled)s.velocity_x+=s.acceleration;}
  else if((s.input_mask&1)!=0){s.direction_mask|=8;++changed_axes;if(s.motion_enabled)s.velocity_x-=s.acceleration;}
  if((s.input_mask&8)!=0){s.direction_mask|=4;++changed_axes;if(s.motion_enabled)s.velocity_y+=s.acceleration;}
  else if((s.input_mask&4)!=0){s.direction_mask|=1;++changed_axes;if(s.motion_enabled)s.velocity_y-=s.acceleration;}
 }
 s.velocity_x=std::clamp(s.velocity_x,-s.max_walk_speed,s.max_walk_speed);
 s.velocity_y=std::clamp(s.velocity_y,-s.max_walk_speed,s.max_walk_speed);
 s.position_x+=s.velocity_x;s.position_y+=s.velocity_y;
 for(std::size_t i=0;i<s.pens.size();++i){
  const auto& p=s.pens[i];
  if(p.status==2||io.distance(
     {s.current_x+((i==0)?30:(i==1)?0:0),s.current_y+((i==0)?100:(i==1)?0:0)},
     p.center)>((i==2)?50.f:40.f)||s.selected_pen==static_cast<I32>(i))continue;
  io.play_sound(static_cast<I32>(i)+(sample(io.random32(),2)==0?0x261:0x246),0x5a,1);
  if(s.selected_pen>=0&&s.selected_pen<3)s.pens[static_cast<std::size_t>(s.selected_pen)].status=1;
  s.selected_pen=static_cast<I32>(i);s.pens[i].status=-1;
  for(auto& a:s.animals)if(a.tracked){a.tracked=false;a.behavior=200;}
 }
 const Point proposed=round_point(s.position_x,s.position_y);
 if(io.polygon_excludes(proposed)){
  if(changed_axes<2||!io.polygon_excludes({s.last_x,proposed.y}))
   s.position_x=s.prev_position_x;
  if(io.polygon_excludes(round_point(s.position_x,s.position_y)))
   s.position_y=s.prev_position_y;
 }
 for(std::size_t i=1;i<s.animals.size();++i){
  auto& a=s.animals[i];
  if(a.id<0)continue;
  const Box b{a.x-20,a.y-18,a.x+a.size_x,a.y+a.size_y};
  if((a.behavior>=4&&a.behavior<=6)||a.behavior>=12){
   if(inside_box(a,b,io,s.current_x,s.current_y)){
    s.position_x=s.prev_position_x;s.position_y=s.prev_position_y;
   }
  }
 }
 s.current_x=static_cast<I32>(s.position_x);
 s.current_y=static_cast<I32>(s.position_y);
 if(s.direction_mask!=0){
  if(s.selected_pen<0&&s.frame>9)s.frame=0;
  if(s.selected_pen>=0&&(s.frame<20||s.frame>29))s.frame=20;
  if((s.direction_mask&2)!=0)s.facing=2;
  else if((s.direction_mask&8)!=0)s.facing=6;
  else if((s.direction_mask&1)!=0)s.facing=0;
  else if((s.direction_mask&4)!=0)s.facing=4;
  if((s.anim_tick-=10)<1){
   ++s.frame;s.anim_tick=50;
   if(s.selected_pen<0){if(s.frame>9)s.frame=0;}
   else if(s.frame>29)s.frame=20;
  }
 }else if(--s.anim_tick<1){
  s.anim_tick=100;
  if(s.selected_pen<0){s.frame=(s.frame>=12&&s.frame<15)?s.frame+1:12;}
  else s.frame=(s.frame>=33&&s.frame<38)?s.frame+1:33;
 }
 for(std::size_t i=0;i<s.animals.size();++i){
  auto& a=s.animals[i];
  if(a.id<0)continue;
  a.previous_x=a.x;a.previous_y=a.y;
  if(a.behavior==7){
   a.phase_counter=std::max(0,a.phase_counter-5);
   io.update_original_animal(a,static_cast<I32>(i));
  }else{
   if(!a.locked||io.original_pen_or_animal_collision(a,static_cast<I32>(i)))
    io.update_original_animal(a,static_cast<I32>(i));
  }
  if(io.polygon_excludes({a.x,a.y})){a.x=a.previous_x;a.y=a.previous_y;}
 }
 io.draw_herding_activity();++s.frames_rendered;
 if(s.remaining<1){
  if(s.finish_state==0&&!io.any_managed_sound_playing()){
   io.play_sound(599+sample(io.random32(),2),0x5a,1);s.finish_state=1;
  }else if(s.finish_state==1&&!io.any_managed_sound_playing()){
   io.prepare_play_again();s.game_state=0x3c;s.activity_state=14;
   io.unload_herding_activity();s.saved_progress=1;s.finish_state=2;
  }
 }
 if(s.music_pending){
  io.stop_activity_music();io.play_activity_music(6);s.music_pending=false;
  io.play_sound(0x245,0x5a,1);io.mark_intro_sound(0x245);
 }
 return 1;
}
} // namespace btb::original_gameplay3b
