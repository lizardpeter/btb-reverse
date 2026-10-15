#include "btb/original_activity4.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <stdexcept>
namespace btb::original_activity4 {
namespace {
void emit(IO& io,U32 slot,I32 x,I32 y,bool keyed=false,std::optional<Rect> clip=std::nullopt){
 if(slot==0)throw std::runtime_error("original source requires a nonzero surface handle");
 io.draw({slot,x,y,keyed,clip});
}
Rect rect(I32 x,I32 y,I32 width,I32 height){
 if(width<0||height<0)throw std::invalid_argument("negative sprite dimension");
 return {x,y,x+width,y+height};
}
bool inside(Rect r,I32 x,I32 y){return r.l<x&&x<r.r&&r.t<y&&y<r.b;}
I32 bounded(I32 x,I32 size,const char* label){
 if(x<0||x>=size)throw std::out_of_range(label);
 return x;
}
I32 roll(IO& io,I32 n){return static_cast<I32>(io.random32()%static_cast<U32>(n));}
void draw_repair_set(std::vector<RepairSprite>& v,I32 offset,IO& io){
 for(auto& r:v){
  if(r.counter>0){
   if(r.counter<30)emit(io,r.surface,r.x+offset,r.y,true);
   if(++r.counter>10)r.counter=1; // original repair animation wraps at 10
  }
 }
}
I32 validated_frame(const SquirrelScene& s,I32 screen,I32 index,bool second=false){
 if(index<0||index>=4)throw std::out_of_range("Squirrel sprite index");
 const auto& arr=second?s.second_frame_ids:s.sprite_frame_ids;
 const I32 value=arr[static_cast<std::size_t>(screen)][static_cast<std::size_t>(index)];
 if(value<0)throw std::runtime_error("unresolved Squirrel frame");
 return value;
}
void help_voice(ContextHelp& st,IO& io,I32 key,I32 voice){
 if(st.previous!=key){io.voice_help(voice,1);st.previous=key;}
}
bool maybe_hotspot(ContextHelp& s,IO& io,const HelpHotspot& h){
 if(h.enabled&&inside(h.area,s.mouse_x,s.mouse_y)){
  help_voice(s,io,h.id,h.sound);return true;
 }
 return false;
}
}
void draw_spud_maze_scene(SpudMazeScene& s,IO& io){
 const auto current=bounded(s.screen,5,"Spud Maze screen");
 const auto other=bounded(s.spud_screen,5,"Spud Maze spud screen");
 if(!s.background_mode)emit(io,s.backgrounds[static_cast<std::size_t>(current)],0,0);
 else draw_repair_set(s.repairs[static_cast<std::size_t>(other)],s.old_screen_offset,io);
 draw_repair_set(s.repairs[static_cast<std::size_t>(current)],s.screen_offset,io);
 if(s.pilchard_present&&s.bob_screen==current){
  const auto pose=(s.pilchard_facing==4?3:s.pilchard_facing);
  io.update_pilchard_animation();
  emit(io,s.pilchard,s.pilchard_x,s.pilchard_y,true,
       rect(s.pilchard_frame*156,pose>=2?128:0,156,128));
 }
 I32 bob_pose=s.bob_facing==4?3:s.bob_facing;
 if(s.hammer_present&&!s.bob_locked)bob_pose+=30;
 if(bob_pose<0)throw std::out_of_range("invalid Spud Maze pose");
 emit(io,s.bob,s.bob_x,s.bob_y,true,
      rect(s.bob_frame*83,bob_pose==1?0:bob_pose==3?83:166,83,83));
 io.update_bob_animation();
 if(!s.bob_locked&&s.bob_screen==current){
  if(s.raise_hammer){
   if(s.hammer_velocity<100)s.hammer_velocity=std::min(100,s.hammer_velocity+2);
   s.hammer_y+=s.hammer_velocity/10;
   if(s.hammer_y>369)s.raise_hammer=false;
  }
  emit(io,s.hammer,s.hammer_x+s.screen_offset,s.hammer_y-30,true);
  s.hammer_x=std::min(s.hammer_x,555);
  s.hammer_y=std::min(s.hammer_y,370);
 }
 io.maybe_spud_voice();
 if(s.pilchard_talking)
  emit(io,s.helper,380,364,true,rect(s.blink_frame*76,0,76,80));
 emit(io,s.border,0,0,true);
 // The original calculates elapsed time using Win32 SYSTEMTIME->FILETIME
 // then divides the delta by 10,000,000 (seconds).
 const std::uint64_t now=io.filetime_100ns();
 if(now<s.baseline_filetime)throw std::runtime_error("Spud Maze clock went backwards");
 const auto elapsed=(now-s.baseline_filetime)/10000000ULL;
 if(elapsed>static_cast<std::uint64_t>(0x7fffffff))
  throw std::overflow_error("Spud Maze elapsed seconds exceed source int");
 s.last_seconds=s.clock_seconds+s.time_bias_seconds-static_cast<I32>(elapsed);
 const I32 timer=io.spud_maze_timer_digits(s.last_seconds);
 emit(io,s.digits,422,430,true,rect(timer,0,25,25));
 if(s.remaining_hammers<0||s.remaining_hammers>100)
  throw std::out_of_range("Spud Maze hammer counter");
 for(I32 i=0;i<s.remaining_hammers;++i){
  const I32 x=((s.remaining_hammers/2)*-20+244)+(i%12)*20;
  const I32 y=429+(i/12)*20;
  emit(io,i<s.remaining_hammers-s.used_hammers?s.hammer_on:s.hammer_off,x,y,true);
 }
}
void draw_spud_skate_playback(SpudSkateScene& s,IO& io){
 if(s.score<0||s.score>=1000)throw std::out_of_range("Skate score outside three-digit source UI");
 emit(io,s.background,0,0);
 const I32 tens=s.score/10,ones=s.score%10;
 if(tens>0)emit(io,s.digits,450,415,true,rect(tens*16,0,16,20));
 emit(io,s.digits,475,415,true,rect(ones*16,0,16,20));
 if(s.phase==1){
  if(s.last_sound_gate==0){
   io.stop_sounds();
   if(s.score<10)io.sound(0x305+roll(io,2),50,1);
   if(s.score<20)io.sound(0x307+roll(io,3),50,1);
   else io.sound(0x30a+roll(io,2),50,1);
   s.ending_profile_flag=1;
   if(s.ending_profile==1)s.current_path=2;
   s.last_sound_gate=-1;
  }
  if(io.bink_end(4)){
   emit(io,s.end_surface,20,20);
   if(!io.sound_active())s.phase=2;
   return;
  }
  io.bink_decode(4);io.bink_next(4);io.bink_wait(4);
  emit(io,s.end_surface,20,20);
  return;
 }
 if(s.phase==2)return;
 const I32 now=s.current_skate_time;
 if(s.segment_count<0||s.segment_count>static_cast<I32>(s.segments.size()))
  throw std::out_of_range("Spud Skate segment count");
 I32 segment=-1;
 for(I32 i=0;i<s.segment_count;++i){
  const auto& seg=s.segments[static_cast<std::size_t>(i)];
  if(seg.first<=now&&now<seg.last){segment=i;break;}
 }
 if(segment>=0){
  const auto& seg=s.segments[static_cast<std::size_t>(segment)];
  if(now==seg.first){
   const auto index=bounded(s.active_channel,4,"Spud Skate sound channel");
   const auto base=index+s.score_step*4;
   if(base<0||base>=static_cast<I32>(s.event_sounds.size()))
    throw std::out_of_range("Spud Skate event sound row");
   I32 sound=-1;
   for(I32 attempts=0;attempts<64;++attempts){
    const auto candidate=s.event_sounds[static_cast<std::size_t>(base)][static_cast<std::size_t>(roll(io,5))];
    if(candidate>=0){sound=candidate;break;}
   }
   if(sound<0)throw std::runtime_error("no source Skate event sound");
   io.sound(sound,50,1);s.score+=s.score_step;
  }
  if((s.input_select||s.button_press)&&s.pause_index<0){
   s.pause_index=segment;
   for(int i=3;i>=0;--i)
    if(now>=seg.thresholds[static_cast<std::size_t>(i)]){
     s.score_step=i+1;break;
    }
  }
 }
 if(s.pause_index>=0&&s.pause_index<s.segment_count){
  if(now>=s.segments[static_cast<std::size_t>(s.pause_index)].last){
   s.pause_index=-1;s.score_step=0;
  }
 }
 if(now==s.terminal_time){
  if(++s.ending_counter>1){
   s.phase=1;
   io.bink_decode(4);io.bink_next(4);io.bink_wait(4);
   emit(io,s.end_surface,20,20);
   return;
  }
  I32 sound=-1;
  for(int attempts=0;attempts<64;++attempts){
   const auto id=s.terminal_sounds[static_cast<std::size_t>(roll(io,5))];
   if(id>=0){sound=id;break;}
  }
  if(sound<0)throw std::runtime_error("Skate terminal sound bank empty");
  io.sound(sound,50,1);s.score+=s.score_step;
 }
 for(std::size_t i=0;i<s.frame_surfaces.size();++i){
  if(io.bink_end(i))io.bink_seek(i,s.rewind_frame);
  io.bink_decode(i);io.bink_next(i);
  if(static_cast<I32>(i)==s.active_channel){
   io.bink_wait(i);
   emit(io,s.frame_surfaces[i],20,20);
  }
 }
 if(s.score_step>0){
  const auto i=bounded(s.score_step,4,"Skate connection arrow");
  emit(io,s.link_arrows[static_cast<std::size_t>(i)],267,415,true);
 }
}
void draw_squirrel_scene(SquirrelScene& s,IO& io){
 const auto screen=bounded(s.screen,4,"Squirrel screen");
 const I32 offset=s.replay?s.secondary_scroll:s.scroll;
 if(!s.replay)emit(io,s.backdrop,0,0,false,rect(offset,0,620,480));
 emit(io,s.foreground,0,417,false,rect(s.sprite_layer*640,0,640,44));
 if(s.replay){
  ++s.background_counter;
  if(s.background_counter>=3){
   s.background_counter=0;
   if(s.overlay_phase==0){
    s.overlay_phase=1;s.overlay_frame=21;
   }else if(s.overlay_phase<=3){
    ++s.overlay_frame;
    if(s.overlay_frame>=42){
     ++s.overlay_phase;
     if(s.overlay_phase>3)s.overlay_phase=4;
     else s.overlay_frame=21;
    }
   }else if(s.overlay_phase==4){s.overlay_phase=0;}
  }
  if(s.overlay_phase!=0)
   emit(io,s.overlay,110,0,true,rect((s.overlay_frame-21)*463,0,463,480));
 }else{
  emit(io,s.character,s.panel_x+offset,s.panel_y,true);
  emit(io,s.panel,s.panel_x+offset,s.panel_y,true);
  if(s.speech_sound>=0){
   if(!io.sound_id_active(s.speech_sound))s.speech_sound=-1;
   if(--s.sound_delay<1){
    const I32 old=s.last_audio_variant;
    I32 next=old;
    for(int tries=0;tries<64&&next==old;++tries)next=roll(io,3);
    if(next==old)throw std::runtime_error("Squirrel source voice choice repeated");
    s.last_audio_variant=next;s.sound_delay=10+roll(io,10);
   }
  }
  emit(io,s.foreground_alt,s.panel_x+offset,s.panel_y,true);
  emit(io,s.frame_surface,s.panel_x,s.panel_y,true);
 }
 const I32 count=s.drawable_per_screen[static_cast<std::size_t>(screen)]
   +((s.squirrel_actor_state==6||s.squirrel_actor_state==7)?1:0);
 if(count<0||count>4)throw std::out_of_range("invalid Squirrel actor count");
 for(int i=0;i<count;++i){
  const auto& p=s.positions[static_cast<std::size_t>(i)];
  const I32 frame=validated_frame(s,screen,i);
  emit(io,s.squirrel_sheet,p.x+offset,p.y,true,rect(frame*89,0,89,133));
 }
 for(int i=0;i<4;++i){
  const auto& p=s.secondary_positions[static_cast<std::size_t>(i)];
  const I32 frame=validated_frame(s,screen,i,true);
  emit(io,s.squirrel_sheet,p.x+offset,p.y,true,rect(frame*89,0,89,133));
 }
 if(s.player_selected)emit(io,s.mouse_pointer,s.panel_x,s.panel_y,true);
 if(s.replay){
  for(int i=1;i<4;++i){
   const auto& p=s.secondary_positions[static_cast<std::size_t>(i)];
   emit(io,s.squirrel_sheet,p.x+(s.panel_x-100)+offset,p.y,true,
       rect(validated_frame(s,screen,i,true)*89,0,89,133));
  }
 }
 io.squirrel_run_assembly();
 if(!s.replay||s.overlay_phase==0)
  emit(io,s.frame_surface,s.panel_x+28+offset,0,true,rect(0,0,47,44));
 if(s.active_sprites.size()!=s.active_frame_ids.size())
  throw std::runtime_error("Squirrel active sprite/frame array mismatch");
 for(std::size_t i=0;i<s.active_sprites.size();++i){
  const auto& actor=s.active_sprites[i];
  const I32 frame=s.active_frame_ids[i];
  if(frame<0)continue;
  I32 x=actor.x+offset;
  auto c=rect(frame*89,0,89,133);
  if(x<0){c.l-=x;x=0;}
  if(x+89>640)c.r-=x+89-640;
  if(c.r>c.l)emit(io,s.character,x,actor.y,true,c);
 }
 for(std::size_t i=0;i<s.borders.size();++i){
  const I32 x=i==0?0:i==1?0:i==2?620:20;
  const I32 y=i==1?20:i==3?400:0;
  emit(io,s.borders[i],x,y,true);
 }
 ++s.animation_counter;
}
void update_contextual_help(ContextHelp& s,IO& io){
 const I32 mode=bounded(s.activity,32,"invalid original Help activity");
 // Original help mode is an overlay pass; the platform does the draw.
 // Specialized game-specific hotboxes precede common menu records.
 if(mode==0x12){
  if(s.progress>7){
   for(const auto& h:s.special_hotspots)
    if(maybe_hotspot(s,io,h)){
     if(h.id==0&&s.click){io.stop_sounds();io.leave_help_mode();}
     return;
    }
   if(300<s.mouse_x&&s.mouse_x<346&&420<s.mouse_y&&s.mouse_y<468){
    help_voice(s,io,3,0xa3);return;
   }
   return;
  }
 }
 if(mode==0xc&&s.dino_alternate!=0){
  if(inside({297,420,345,468},s.mouse_x,s.mouse_y)){
   help_voice(s,io,0x32,200);return;
  }
 }
 if(mode==0xe){
  for(int i=0;i<2;++i)
   if(inside(s.maze_help[static_cast<std::size_t>(i)],s.mouse_x,s.mouse_y)){
    help_voice(s,io,0xb+i,i==0?0x62:0x61);return;
   }
 }
 if(mode==0x10){
  for(int i=0;i<10;++i)
   if(inside(s.dino_hotspots[static_cast<std::size_t>(i)],s.mouse_x,s.mouse_y)){
    help_voice(s,io,i+10,0x231+i%2);return;
   }
 }
 if(mode==0x11){
  for(int i=0;i<4;++i){
   auto r=s.herding_hotspots[static_cast<std::size_t>(i)];
   r.l-=s.camera_x;r.r-=s.camera_x;
   r.t-=s.camera_y;r.b-=s.camera_y;
   if(inside(r,s.mouse_x,s.mouse_y)){
    help_voice(s,io,i+10,0x269+i);return;
   }
  }
 }
 const auto& h=s.generic_hotspots[static_cast<std::size_t>(mode)];
 for(const auto& region:h){
  if(!region.enabled||!inside(region.area,s.mouse_x,s.mouse_y))continue;
  if(region.id==0&&s.click){
   io.stop_sounds();io.leave_help_mode();s.previous=-1;return;
  }
  if(mode==9){
   if(s.mode_index==3&&region.id!=6&&region.id!=7&&region.id!=9)continue;
   if(s.mode_index!=3&&region.id<10)continue;
  }
  if(mode==0xc&&region.id==2&&s.dino_alternate<1)continue;
  help_voice(s,io,region.id,region.sound);return;
 }
}
}
