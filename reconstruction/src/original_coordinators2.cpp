#include "btb/original_coordinators2.hpp"
#include <algorithm>
#include <cstddef>
#include <stdexcept>
namespace btb::original_coordinators2 {
namespace {
std::size_t index(I32 id,std::size_t count,const char* context) {
 if(id<0||static_cast<std::size_t>(id)>=count)throw std::out_of_range(context);
 return static_cast<std::size_t>(id);
}
I32 roll(U32 seed,I32 n){return static_cast<I32>(seed%static_cast<U32>(n));}
void score_completed(DinoState& s){
 const I32 slot=(s.score_index/3)+s.profile_index*100;
 if(slot<0||static_cast<std::size_t>(slot)>=s.profile_completed.size())
  throw std::out_of_range("Dino profile completion slot");
 if(s.profile_completed[static_cast<std::size_t>(slot)]==0)
  s.profile_completed[static_cast<std::size_t>(slot)]=1;
 s.marked_profile=slot;
}
bool hit(const UiOption& option,const UiState& s,UiIO& io){
 return !io.original_point_in_polygon(option.polygon,s.mouse_x,s.mouse_y);
}
void blit_if(UiIO& io,U32 surface,I32 x,I32 y,bool keyed=false){
 if(surface)io.blit(surface,x,y,keyed);
}
void passive(UiState& s,UiIO& io){
 for(std::size_t i=0;i<s.options[static_cast<std::size_t>(s.screen)].size();++i){
  if(s.active_option==static_cast<I32>(i)+1)continue;
  auto& o=s.options[static_cast<std::size_t>(s.screen)][i];
  if(s.passive_clock==5&&o.passive_frame_count>0){
   if(++o.passive_tick>=o.passive_delay){
    o.passive_tick=0;
    o.passive_frame=(o.passive_frame+1)%o.passive_frame_count;
   }
  }
  if(o.passive_frame_count>0){
   const auto frame=index(o.passive_frame,o.passive_surfaces.size(),"UI passive animation frame");
   blit_if(io,o.passive_surfaces[frame],o.normal_x,o.normal_y);
  }else blit_if(io,o.static_surface,o.static_x,o.static_y);
 }
}
} // namespace
I32 update_dino_activity(DinoState& s,DinoIO& io){
 io.enable_input_processing();
 if(s.quit_requested||s.menu_requested){
  io.unload_dino_activity();
  if(s.menu_requested){
   s.menu_requested=false;
   s.next_game_state=(s.marked_profile==-1)?0x10:0x40;
  }
  return 1;
 }
 if(s.actor_moving){
  s.cursor_x=std::min(s.cursor_x,0x244);
  s.cursor_y=std::min(s.cursor_y,400);
 }
 if(s.completed>=static_cast<I32>(s.pieces.size())){
  if(!io.any_managed_sound_playing()||s.completion_state==1){
   if(s.completion_state==10){
    io.prepare_play_again();
    s.next_game_state=0x3c;
    s.stage_progress=1;
    io.unload_dino_activity();
    score_completed(s);
    return 1;
   }
   if(s.completion_state==0){
    io.stop_all_managed_sounds();
    io.sound(s.level_variant+0xb9,0x32,1);
    score_completed(s);
    s.marked_profile=1;
    io.load_completion_sprite(s.level_variant);
    s.completion_sprite_loaded=true;
    s.completion_state=1;
   }
   io.update_print_button();
   return 1;
  }
  io.draw_dino_activity();
 }
 if(s.phase==0){
  if(s.input_down){
   for(std::size_t i=0;i<s.pieces.size();++i){
    auto& p=s.pieces[i];
    if(p.state!=0)continue;
    if(!io.cursor_in_rectangle({p.x,p.y,p.x+p.width,p.y+p.height}))continue;
    s.phase=1;
    io.set_cursor_surface(p.cursor_surface);
    s.cursor_x=p.x;s.cursor_y=p.y;
    p.state=1;s.selected=static_cast<I32>(i);p.visible=0;
    const I32 a=roll(io.random32(),2);
    const I32 b=roll(io.random32(),2);
    s.feedback_frames[static_cast<std::size_t>(b)]=a+1;
    break;
   }
  }
 }else if(s.phase==1){
  if(s.input_down){
   const std::size_t selected=index(s.selected,s.pieces.size(),"Dino dragged piece");
   bool accepted=false;
   for(std::size_t i=0;i<s.pieces.size();++i){
    if(s.pieces[selected].state==4)break;
    const auto& dst=s.pieces[i];
    if(io.cursor_near_point(dst.target_x,dst.target_y,s.radius)&&
       i==selected){
      s.phase=2;s.stage_progress=1;io.set_cursor_surface(0);
      s.success_variant=roll(io.random32(),6);
      s.feedback_frames[static_cast<std::size_t>(1-s.success_variant/3)]=5;
      s.pieces[selected].state=2;
      io.stop_all_managed_sounds();
      io.sound(s.success_variant+0xa4,0x32,1);
      accepted=true;
      break;
    }
   }
   if(!accepted){
    s.pieces[selected].state=3;
    s.failure_variant=roll(io.random32(),6);
    io.sound(s.failure_variant+0xaa,0x32,1);
    const std::size_t side=static_cast<std::size_t>(1-s.failure_variant/3);
    if(s.feedback_gates[side]==0)
     s.feedback_frames[side]=roll(io.random32(),2)+3;
   }
  }
 }else if(s.phase==2){
  auto& piece=s.pieces[index(s.selected,s.pieces.size(),"Dino completed piece")];
  if(piece.state==2){
   piece.state=4;piece.visible=1;s.stage_progress+=2;
   ++s.completed;
   (void)io.random32();
  }else{
   piece.state=0;piece.visible=1;
  }
  s.stage_progress=0;
  s.selected=-1;s.phase=0;s.cursor_reset_countdown=0;
  s.animation_state=4;
 }else throw std::invalid_argument("Dino activity source phase");
 io.draw_dino_activity();
 if(s.music_pending){
  io.stop_activity_music();
  io.play_activity_music(1);
  s.music_pending=false;
  const I32 stage=s.score_index/3;
  io.sound(stage+0xbc,0x5a,1);
  io.mark_activity_intro_sound(stage);
 }
 return 1;
}
I32 update_generic_ui_screen(UiState& s,UiIO& io){
 io.enable_input_processing();
 if(s.screen<0||s.screen>=static_cast<I32>(s.options.size()))
  throw std::out_of_range("original generic UI screen");
 if(--s.passive_clock<0)s.passive_clock=5;
 auto& buttons=s.options[static_cast<std::size_t>(s.screen)];
 if(buttons.size()>32)throw std::out_of_range("original UI max 32 options");
 if(s.sound_pending){
  if(io.any_managed_sound_playing())return 1;
  s.sound_pending=false;
  auto& option=buttons[index(s.selected_option,buttons.size(),"UI voiced selection")];
  if(option.next_state==-99){
   s.requires_display_reset=true;
   io.invalidate_or_reset_primary_display();
   return 1;
  }
  io.no_op_legacy_hook();
  if(s.unlocked_mode!=0){
   if(option.next_state!=0x22){
    s.next_mode=option.next_state;s.unlocked_mode=0;return 1;
   }
   io.stop_all_managed_sounds();
   io.load_progress_screen_assets();
   s.progress_screen_loaded=true;
   s.activate_progress=true;
   s.intro_complete=1;
   return 1;
  }
  s.next_mode=option.next_state;
  return 1;
 }
 // Draw the source's restricted (-4,-3,-2) entries in profile mode.
 for(const auto& button:buttons)
  if(s.render_enabled&&button.next_state>=-4&&button.next_state<=-2&&
     (-2-button.next_state)==s.profile)
   blit_if(io,button.static_surface,button.static_x,button.static_y);
 if(s.active_option==0){
  for(std::size_t i=0;i<buttons.size();++i){
   auto& o=buttons[i];
   if(!hit(o,s,io))continue;
   // The source's mouse-down latch permits only the previously captured
   // hotspot to remain selected while a button is held.
   if(s.mouse_down&&s.previous_hover!=static_cast<I32>(i))continue;
   if(!s.mouse_down)s.previous_hover=static_cast<I32>(i);
   s.active_option=static_cast<I32>(i)+1;
   o.hover_delay=8;o.hover_frame=0;
   if(s.screen==1&&i==2&&s.unlocked_mode==1)
    io.play_sound(0x41,0x32,2);
   else if(!o.pressed_voices.empty()){
    o.sound_cursor=(o.sound_cursor+1)%static_cast<I32>(o.pressed_voices.size());
    io.play_sound(o.pressed_voices[static_cast<std::size_t>(o.sound_cursor)],0x32,2);
   }
   break;
  }
 }else{
  const I32 button_id=s.active_option-1;
  auto& o=buttons[index(button_id,buttons.size(),"UI highlighted option")];
  if(hit(o,s,io)){
   if(!s.mouse_down||s.previous_hover==button_id){
    if(o.hover_frame_count>0&&--o.hover_delay<1){
     o.hover_delay=8;
     o.hover_frame=(o.hover_frame+1)%o.hover_frame_count;
    }
    if(o.hover_frame_count>0){
     const auto frame=index(o.hover_frame,o.hover_surfaces.size(),"UI hover sprite");
     blit_if(io,o.hover_surfaces[frame],o.hover_x,o.hover_y);
    }
   }
  }else s.active_option=0;
  if(s.mouse_down){
   blit_if(io,o.pressed_surface,o.pressed_x,o.pressed_y);
   s.previous_hover=button_id;
  }
  if(s.mouse_clicked&&s.previous_hover==button_id){
   s.previous_hover=-1;s.sound_pending=true;
   s.selected_option=button_id;
   if(o.voice!=-1){
    io.stop_all_managed_sounds();
    io.play_sound(o.voice,0x32,2);
    if(s.unlocked_mode&&o.next_state==0x22)io.stop_all_managed_sounds();
   }
  }
 }
 passive(s,io);
 if(s.mode==5){
  I32 total=0;
  for(std::size_t i=50;i<=64;++i)total+=std::max(0,s.awards[i]);
  if(total>12)s.progress_button_enabled=false;
  else s.progress_button_enabled=true;
 }
 return 1;
}
} // namespace btb::original_coordinators2
