#include "btb/original_draw4.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <numeric>
#include <stdexcept>
namespace btb::original_draw4 {
namespace {
Clip clip(I32 l,I32 t,I32 w,I32 h){return {l,t,l+w,t+h};}
void emit(IO& io,U32 surface,I32 x,I32 y,bool keyed=false,
          std::optional<Clip> c=std::nullopt){
 if(!surface)throw std::runtime_error("original drawing surface is absent");
 io.draw({surface,x,y,c,keyed});
}
void advance(I32& frame,I32& countdown,I32 length,I32 first=0,I32 cadence=5){
 if(length<=first||cadence<=0)throw std::invalid_argument("invalid original sprite animation");
 if(--countdown<1){countdown=cadence;++frame;if(frame>=length)frame=first;}
}
I32 boundindex(I32 value,I32 count){
 if(value<0||value>=count)throw std::out_of_range("guest animation array index");
 return value;
}
}
void draw_dino(DinoState& s,IO& io){
 // The retail prologue fills a DirectDraw blit descriptor rooted at
 // DAT_004fc9e8, then increments two six-phase animation tracks.
 static constexpr std::array<I32,6> frame_ends{100,20,60,35,75,131};
 emit(io,s.backdrop,0,0);
 for(auto& t:s.tracks){
  if(t.animation<0||t.animation>=6)t.animation=0;
  if(++t.phase>6){t.phase=0;++t.animation;}
  if(t.requested>=0){
   const auto next=boundindex(t.requested,6);
   t.animation=next;t.requested=-1;t.phase=0;
  }
  if(t.phase>=frame_ends[static_cast<std::size_t>(t.animation)]||t.phase<0){
   t.animation=0;t.phase=0;
  }
 }
 // Foreground animal is indexed by 0x71 per source sprite column. The
 // legacy clip row switch comes from DAT_004fc43c; variant width is 366.
 const I32 row=boundindex(s.frame_variant,2);
 emit(io,s.character,row*366,20,true,clip(s.tracks[0].animation*113,0,113,165));
 emit(io,s.interface_surface,22,20,true,clip(366+s.tracks[1].animation*93,0,93,105));
 if(s.entities.size()>static_cast<std::size_t>(s.max_entities))
  throw std::runtime_error("original Dino entity census is corrupt");
 // Retail performs two loops over the same 48-byte entity records,
 // drawing mode 4 first and then mode 2 + active 0..3.
 for(const auto& e:s.entities)
  if(e.mode==4)emit(io,static_cast<U32>(e.surface_slot),e.x,e.y,true,e.source);
 for(const auto& e:s.entities){
  if(e.mode==2)
   emit(io,static_cast<U32>(e.surface_slot),e.picture_x,e.picture_y,true,e.source);
  else if(e.mode>=0&&e.mode<=3&&e.kind!=0)
   emit(io,static_cast<U32>(e.surface_slot),e.x,e.y,true,e.source);
 }
}
void draw_maze(MazeState& s,IO& io){
 const int screen=boundindex(s.screen,static_cast<I32>(s.backdrops.size()));
 if(s.foreground_active)emit(io,s.backdrops[static_cast<std::size_t>(screen)],0,0);
 // The source displays the visible actor table twice in two drawing
 // branches. The node-index branch reads the original 12-byte node table.
 auto draw_boxes=[&](I32 offset){
  for(const auto& a:s.actors){
   if(a.state!=3&&(a.node<0||a.state!=1))continue;
   I32 x=a.x,y=a.y;
   if(a.state==1){
    const auto node=a.node+screen*4;
    if(node<0||static_cast<std::size_t>(node)>=s.original_node_positions.size())
     throw std::out_of_range("original Maze actor node index");
    x=s.original_node_positions[static_cast<std::size_t>(node)][0];
    y=s.original_node_positions[static_cast<std::size_t>(node)][1];
   }
   emit(io,s.box_surface,x-8+offset,y-8);
  }
 };
 draw_boxes(s.view_offset);
 auto draw_spud=[&]{
  if(s.other_enabled<=1||s.other_screen!=s.screen)return;
  I32 pose=(s.spud_pose==4?3:s.spud_pose);
  const auto c=clip(pose*128,s.spud_frame*128,128,128);
  s.spud_previous_x=s.spud_x;s.spud_previous_y=s.spud_y;
  emit(io,s.spud_surface,s.spud_x,s.spud_y,true,c);
  if(s.progress>0&&s.timer_left>=0){
   if(--s.spud_timer<1){
    ++s.spud_frame;
    if(s.spud_animation_mode==1){
     if(s.spud_frame>13)s.spud_frame=7;
    }else if(s.spud_frame>6)s.spud_frame=0;
    s.spud_timer=s.frame_interval;
   }
  }
 };
 if(!s.foreground_active){
  s.view_offset=s.right_edge;
  draw_spud();draw_boxes(s.view_offset);return;
 }
 s.frame_interval=10;
 if(s.bob_x==s.bob_previous_x&&s.bob_y==s.bob_previous_y){
  if(s.sprite_moving&&--s.bob_timer<1){
   ++s.bob_frame;
   if(s.bob_frame>7){
    s.sprite_moving=false;s.bob_frame=0;
    io.sound(s.bob_sound_count<6?0x4d+s.bob_sound_count
                              :0x51+static_cast<I32>(io.random32()%2),10,1);
    ++s.bob_sound_count;
   }
   s.bob_timer=20;
  }
 }else if(!s.sprite_moving&&--s.bob_timer<1){
  ++s.bob_frame;
  if(s.bob_frame>=s.bob_animation_length)s.bob_frame=0;
  s.bob_timer=10;
 }
 const auto source=clip((s.bob_pose==4?3:s.bob_pose)*128,s.bob_frame*128,128,128);
 emit(io,s.player_surface,s.bob_x,s.bob_y,true,source);
 s.bob_previous_x=s.bob_x;s.bob_previous_y=s.bob_y;
 draw_spud();
 const I32 digits=io.maze_clock_digits();
 s.timer_digits=digits;
 s.view_x=digits*20+126;
 s.view_y=s.view_x+s.progress*20;
}
void draw_golf(GolfState& s,IO& io){
 emit(io,s.backdrop,0,0);
 if(s.score<0||s.score>999)throw std::out_of_range("retail Golf score range");
 if(s.score>=10)
  emit(io,s.digits,510,426,false,clip((s.score/10)*16,0,16,20));
 emit(io,s.digits,535,426,false,clip((s.score%10)*16,0,16,20));
 if(s.markers<0||s.markers>128)throw std::out_of_range("golf marker count");
 for(int i=0;i<s.markers;++i)
  emit(io,s.marker,373+i*20,437);
 const auto velocity=io.polar(s.swing_angle-2,140);
 if(s.swing_state==0&&s.markers>0)
  emit(io,s.ball,s.ball_x+velocity[0],s.ball_y+velocity[1]);
 if(s.trail_gate>s.trail_threshold&&s.markers>0){
  emit(io,s.trail,s.ball_x+velocity[0],s.ball_y+velocity[1]);
  emit(io,s.trail_secondary,s.ball_x+velocity[0],s.ball_y+velocity[1]);
 }
 emit(io,s.club,s.club_x,s.club_y,true,
      clip(s.background_phase*15,s.wind_index*15,45,41));
 for(auto& actor:s.actors){
  if(actor.frame_width<=0||actor.frame_height<=0)
   throw std::invalid_argument("original Golf character animation dimensions");
  advance(actor.frame,actor.counter,actor.length,0,5);
  auto c=clip(actor.frame*actor.frame_width,
              actor.source_y,actor.frame_width,actor.frame_height);
  // DirectDraw's DDERR_SURFACELOST is restored and retried once.
  try{emit(io,actor.slot,actor.x,actor.y,true,c);}
  catch(const std::runtime_error&){
   io.restore_surface(actor.slot);
   emit(io,actor.slot,actor.x,actor.y,true,c);
  }
 }
 for(std::size_t i=0;i<s.extras.size();++i){
  auto& extra=s.extras[i];
  if(extra.frame_width<=0||extra.frame_height<=0)
   throw std::invalid_argument("original Golf extra sprite dimensions");
  advance(extra.frame,extra.counter,extra.length,
          i==0?0:std::min(13,extra.length-1),4);
  emit(io,extra.slot,extra.x,extra.y,true,
       clip(extra.frame*extra.frame_width,extra.source_y,
            extra.frame_width,extra.frame_height));
 }
 if(s.swing_state!=0){
  const int w=(s.meter_value*257)/1000;
  emit(io,s.celebration,83,416,true,clip(0,0,w,53));
 }
}
void draw_herding(HerdState& s,IO& io){
 // Original camera clamp: world x begins at 300 and extends to 973.
 s.view_x=std::clamp(s.camera_x-300,0,672);
 s.view_y=std::clamp(s.camera_y-110,0,502);
 emit(io,s.landscape,20,20,false,
      clip(s.view_x,s.view_y,600,380));
 // Retail constructs a depth array and qsorts pointers, not the entity
 // source array; stable sorting here avoids mutating original world order.
 std::vector<std::size_t> order(s.entities.size());
 std::iota(order.begin(),order.end(),std::size_t{0});
 std::stable_sort(order.begin(),order.end(),[&](auto a,auto b){
  if(s.entities[a].depth!=s.entities[b].depth)
   return s.entities[a].depth<s.entities[b].depth;
  return a<b;
 });
 for(auto idx:order){
  auto& e=s.entities[idx];
  if(e.type==0x11||e.id==-1)continue;
  switch(e.type){
   case 1:case 2:case 3:{
    if(!e.pause){
     if(--e.counter<1){
      e.counter=100;e.animation_event=true;++e.frame;
      if(e.type==3&&e.frame>5)e.frame=0;
      if(e.type==2&&e.frame>6)e.frame=3;
      if(e.type==1&&e.frame>9)e.frame=6;
     }
    }
    e.width=e.type==1?0x62:e.type==2?0x49:0x35;
    e.height=e.type==1?0x67:e.type==2?0x45:0x32;
    e.source_x=(e.frame+e.direction*(e.type==3?12:10))*e.width;
    e.source_y=0;
    break;
   }
   case 7:
    e.width=0x56;e.height=0x57;
    e.source_x=(e.direction*7+e.frame)*e.width;e.source_y=0;
    break;
   case 0x0d:
    if(--e.counter<1){e.counter=10;if(++e.frame>11)e.frame=0;}
    e.width=0xe6;e.height=0xf0;
    e.source_x=e.frame*e.width;e.source_y=0;
    break;
   case 0x0c:
    e.source_x=0;e.source_y=0;e.width=0x97;e.height=0xf0;break;
   case 0x0e:
    e.source_x=0;e.source_y=0;e.width=0x2b;e.height=0x4c;
    e.x=0x17c;e.y=0x16c;break;
   case 0x0f:case 0x10:
    if(e.counter!=-1&&--e.counter==0){
     ++e.frame;if(e.frame<3)e.counter=50;
    }
    e.x=e.type==0x0f?0x2cd:0x35b;
    e.y=e.type==0x0f?0xa2:0x65;
    e.width=0x78;e.height=0x46;
    e.source_x=e.frame*e.width;e.source_y=0;break;
   default:
    // Original helper has no additional clip writer for other classes.
    if(e.width<=0||e.height<=0)continue;
    break;
  }
  emit(io,e.surface,e.x,e.y,true,
       clip(e.source_x,e.source_y,e.width,e.height));
 }
 for(const auto& prop:s.props)
  if(prop.enabled)emit(io,prop.surface,prop.x,prop.y,true);
 emit(io,s.foreground,0,0,true);
 if(s.overlay_index>=0)
  emit(io,0x005105a0U+static_cast<U32>(s.overlay_index)*4U,284,417,true);
}
}
