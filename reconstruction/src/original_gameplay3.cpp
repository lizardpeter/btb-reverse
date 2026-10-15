#include "btb/original_gameplay3.hpp"
#include <algorithm>
#include <limits>
#include <cstdlib>
namespace btb::original_gameplay3 {
namespace {
bool hit(XY pos,I32 x,I32 y){
 return pos.x<x&&x<pos.x+0x59&&pos.y<y&&y<pos.y+0x85;
}
std::size_t checked(I32 value,std::size_t capacity,const char* what){
 if(value<0||static_cast<std::uint64_t>(value)>=capacity)throw std::out_of_range(what);
 return static_cast<std::size_t>(value);
}
I32 roll(SquirrelIO& io,I32 modulus){return static_cast<I32>(io.random32()%static_cast<U32>(modulus));}
void voice(SquirrelIO& io,I32 id){io.sound(id,0x32,1);}
void squirrel_second_feedback(SquirrelState& s,SquirrelIO& io){
 // Original 004273F0 state 4: comparison of selected number of pieces to
 // source-backed expected answer, including distinct equal/higher/lower
 // branches and the source's 0x27d..0x28c sound tables.
 static constexpr std::array<I32,2> equal{{0x27d,0x27e}};
 static constexpr std::array<I32,3> low{{0x28a,0x28b,0x28c}};
 static constexpr std::array<I32,4> high{{0x285,0x286,0x288,0x289}};
 I32 sound{};
 const auto sel=checked(s.selected,s.selection_values.size(),"squirrel feedback selection");
 const I32 target=s.selection_values[sel];
 if(roll(io,5)==0) sound=equal[static_cast<std::size_t>(roll(io,2))];
 else if(s.visible_level<target) sound=low[static_cast<std::size_t>(roll(io,3))];
 else if(s.visible_level>target) sound=high[static_cast<std::size_t>(roll(io,4))];
 else sound=equal[static_cast<std::size_t>(roll(io,2))];
 voice(io,sound);
 if(sound==0x27d||sound==0x27e){
  s.voice_state=roll(io,3);s.voice_delay=roll(io,10)+10;
  s.feedback_index=sound;
 }
}
}
void update_squirrel_placement(SquirrelState& s,I32 mouse_x,I32 mouse_y,
                                bool click,SquirrelIO& io){
 const auto selected=[&](){return checked(s.selected,s.selection_positions.size(),"squirrel selected actor");};
 const auto round=[&](){return checked(s.placement_round,s.round_counts.size(),"squirrel placement round");};
 const auto target=[&](){
  const auto r=round();return checked(s.round_counts[r],s.round_positions.size(),"squirrel target spot");
 };
 switch(s.phase){
 case 0:{
  if(!s.celebration){
   if(s.points>6){
    if(s.placement_round<s.placed_capacity){
     s.placement_finished=1;s.intro_active=1;s.feedback_finished=0;
    }else{
     s.celebration=1;io.stop_all_sounds();
     voice(io,0x298+roll(io,4));
    }
   }
   if(click&&s.points<6){
    for(I32 i=0;i<s.selectable_count;++i){
     const auto at=checked(i,s.selection_positions.size(),"squirrel selectable index");
     if(hit(s.selection_positions[at],mouse_x,mouse_y)){
      s.actor_animation=0;s.selected=i;
      s.phase=(s.selection_enabled[at]==1)?1:2;
     }
    }
   }
  }
  break;
 }
 case 1: case 2:{
  const auto at=selected();
  const auto& p=s.selection_positions[at];
  if(!io.step_actor_toward(p.x,p.y+0x85))return;
  s.target_index=s.selected;s.phase+=2;break;
 }
 case 3: case 4:{
  const auto at=target();const auto& p=s.round_positions[at];
  if(!io.step_actor_toward(p.x,p.y+0x85))return;
  s.target_index=-1;
  const auto sel=selected(),r=round();
  if(s.phase==3){
   s.actor_animation=1;
   if(s.points<5)s.intro_active=1;
   const auto slot=checked(static_cast<I32>(r*4+at),s.written_values.size(),"squirrel answer location");
   const I32 selected_value=s.selection_values[sel];
   if(s.expected_values[slot]==(selected_value%9)%3)s.expected_values[slot]=-1;
   s.written_values[slot]=selected_value;
   if(s.round_counts[r]<4||s.points<5)++s.round_counts[r];
   else if(s.placement_round<s.placed_capacity){
    s.placement_finished=1;s.intro_active=1;s.feedback_finished=0;
   }else s.celebration=1;
   constexpr std::array<I32,7> noises{0x279,0x27a,0x27b,0x27c,0x290,0x291,0x292};
   const I32 n=roll(io,7);s.sound_group=n;
   const I32 sound=noises[static_cast<std::size_t>(n)];voice(io,sound);
   if(n<4){
    s.voice_state=roll(io,3);s.voice_delay=roll(io,10)+10;
    s.feedback_index=sound;
   }
   s.selection_values[sel]=-1;
  }else {
   s.actor_animation=2;
   s.carrying_value=s.selection_values[sel];
   s.selection_values[sel]=-1;
   const auto slot=checked(static_cast<I32>(r*4+at),s.written_values.size(),"squirrel return location");
   s.written_values[slot]=s.carrying_value;
   squirrel_second_feedback(s,io);
  }
  s.phase+=2;break;
 }
 case 5:
  if(io.step_actor_toward(s.home.x,s.home.y))s.phase=0;
  break;
 case 6:
  if(io.step_actor_toward(s.home.x,s.home.y))s.phase=7;
  s.actor_animation=2;
  break;
 case 7:{
  const auto at=target();const auto p=s.round_positions[at];
  if(!io.step_actor_toward(p.x,p.y+0x85))return;
  s.phase=8;s.selection_values[selected()]=s.carrying_value;
  s.target_index=s.selected;break;
 }
 case 8:{
  const auto at=selected();const auto p=s.selection_positions[at];
  s.actor_animation=2;
  if(!io.step_actor_toward(p.x,p.y+0x85))return;
  s.phase=5;s.target_index=-1;break;
 }
 default:throw std::invalid_argument("unknown original Squirrel placement state");
 }
}
void update_spud_maze_pilchard(PilchardState& s,PilchardIO& io){
 if(s.previous_screen!=s.screen&&s.phase>1){
  --s.screen_return_frames;
  if(s.screen_return_frames<0)s.phase=1;
  s.collision_flag=0;
  return;
 }
 const auto neighbors=[&](I32 node){return io.neighbors(s.screen,node);};
 const auto edge_from_path=[&](){
  if(s.shortest_path.empty())throw std::runtime_error("original Spud Maze path empty");
  return s.shortest_path.at(checked(s.path_cursor,s.shortest_path.size(),"Pilchard path index"));
 };
 const auto direction=[&](I32 from,I32 to){
  const auto around=neighbors(from);
  for(I32 i=0;i<static_cast<I32>(around.size());++i)
   if(around[static_cast<std::size_t>(i)]==to)return i;
  return static_cast<I32>(around.size()); // original four-neighbor limit
 };
 switch(s.phase){
 case 0:
  s.collision_flag=0;
  if(--s.wait_frames<1)s.phase=1;
  break;
 case 1:{
  s.previous_screen=s.screen;s.collision_flag=0;
  s.current_node=s.screen==4?2:(s.screen==0?0:static_cast<I32>(io.random32()&3));
  s.wander_direction=1;s.animation_enabled=false;s.path_cursor=0;
  s.position=io.node_position(s.screen,s.current_node);
  s.previous_position=s.position;
  s.traversal_speed=io.pursuit_speed(s.difficulty)/2;
  s.shortest_path=io.shortest_path(s.current_node,s.destination_node);
  s.phase=2;
  if(s.shortest_path.empty()){s.phase=0;s.wait_frames=500;break;}
  s.edge_index=direction(s.current_node,edge_from_path());
  io.sound(0x2ce+static_cast<I32>(io.random32()&1),0x32,0);
  break;
 }
 case 2:{
  if(io.collision_and_hammer_drop()){s.collision_flag=1;return;}
  const I32 next=edge_from_path();
  io.advance_toward_node(next,s.traversal_speed,s.position);
  if(io.distance_to_node(next,s.position)<3){
   ++s.path_cursor;
   if(static_cast<std::size_t>(s.path_cursor)>=s.shortest_path.size()){s.phase=4;break;}
   s.current_node=next;s.edge_index=direction(s.current_node,edge_from_path());
  }
  break;
 }
 case 3:s.phase=4;s.collision_flag=0;break;
 case 4:{
  s.collision_flag=0;
  const auto links=neighbors(s.current_node);
  if(links.empty())throw std::runtime_error("original Spud Maze node has no links");
  const I32 reverse=(s.edge_index+2)%4;
  const I32 seed=static_cast<I32>(io.random32()&3);
  I32 chosen=-1;
  for(I32 j=0;j<4;++j){
   const I32 idx=(seed+j)%4;
   if(idx!=reverse&&static_cast<std::size_t>(idx)<links.size()&&links[idx]>=0){
    chosen=idx;break;
   }
  }
  if(chosen<0)for(I32 j=0;j<4;++j){
   const I32 idx=(seed+j)%4;
   if(static_cast<std::size_t>(idx)<links.size()&&links[idx]>=0){chosen=idx;break;}
  }
  if(chosen<0)throw std::runtime_error("retail Pilchard lacks valid outgoing edge");
  s.last_distance=999999;s.phase=5;s.edge_index=chosen;
  s.animation_enabled=true;s.destination_node=links[static_cast<std::size_t>(chosen)];
  s.traversal_speed=io.wander_speed(s.difficulty)/2;
  break;
 }
 case 5:{
  io.advance_toward_node(s.destination_node,s.traversal_speed,s.position);
  const auto dist=io.distance_to_node(s.destination_node,s.position);
  if(dist<s.last_distance)s.last_distance=dist;
  if(dist<3){s.current_node=s.destination_node;s.phase=s.collision_flag?6:4;}
  if(io.collision_and_hammer_drop()){s.collision_flag=1;return;}
  break;
 }
 case 6:{
  s.shortest_path=io.shortest_path(s.current_node,s.screen==4?2:0);
  if(s.shortest_path.empty())throw std::runtime_error("Pilchard return path absent");
  s.path_cursor=0;s.edge_index=direction(s.current_node,edge_from_path());
  s.phase=7;s.animation_enabled=false;
  s.traversal_speed=io.pursuit_speed(s.difficulty)/2;
  break;
 }
 case 7:{
  const auto next=edge_from_path();
  io.advance_toward_node(next,s.traversal_speed,s.position);
  if(io.distance_to_node(next,s.position)<3){
   ++s.path_cursor;
   if(static_cast<std::size_t>(s.path_cursor)>=s.shortest_path.size()){
    s.phase=0;s.wait_frames=500;
   }else{
    s.current_node=next;s.edge_index=direction(s.current_node,edge_from_path());
   }
   (void)io.collision_and_hammer_drop();
  }
  break;
 }
 default:throw std::invalid_argument("invalid original Pilchard movement state");
 }
 s.previous_position=s.position;
}
void update_fireworks_show(FireworksState& s,FireworksIO& io){
 if(s.current_internal_state!=9)
  throw std::logic_error("00413450 called outside original Fireworks show state 9");
 const I32 ticks=io.elapsed_centiseconds(), column=ticks/400;
 io.draw_background();
 fireworks::ShowFrameInput input{};
 input.elapsed_ticks=ticks;
 const I32 crowd=s.runtime.crowd_phase();
 const I32 movie=fireworks::crowd_movie_index_for_phase(crowd);
 input.crowd_movie_finished=io.decode_crowd_movie(movie,column);
 input.any_managed_sound_playing=io.any_managed_sound_playing();
 input.crowd_end_sound_playing=io.sound_id_playing(0x15d);
 if(crowd<3&&!input.any_managed_sound_playing){
  input.random_chance_roll_0_to_9=static_cast<I32>(io.random32()%10U);
  if(input.random_chance_roll_0_to_9==0)
   input.random_variant_roll_0_to_24=static_cast<I32>(io.random32()%25U);
 }
 const auto predicted=fireworks::advance_crowd_phase(crowd,input.crowd_movie_finished,column);
 if(!predicted.enter_certificate){
  const auto& pool=s.runtime.event_pool().records();
  for(std::size_t slot=0;slot<pool.size();++slot){
   if(pool[slot].free())continue;
   input.active_event_movie_finished[slot]=io.decode_active_movie(
     slot,pool[slot].type,pool[slot].row,column);
  }
  input.top_middle_movie_finished=io.decode_topmiddle_movie(column);
 }
 const auto out=s.runtime.update(s.authored_grid,input);
 if(out.crowd.restart_movie)io.restart_crowd_movie(movie);
 if(out.stop_all_before_sound)io.stop_all_managed_sounds();
 if(out.sound_id)io.play_managed_sound(*out.sound_id,
                          out.sound_priority,out.sound_arbitration_class);
 if(out.enter_certificate){
  io.stop_all_managed_sounds();s.current_internal_state=16;
  io.reset_end_sound();io.stop_end_sound();io.draw_end_overlay();
  return;
 }
 for(const auto slot:out.completed_event_slots)io.restart_active_movie(slot);
 if(out.restart_top_middle_movie)io.restart_topmiddle_movie();
 for(const auto& entry:out.launched_events)
  io.launch_movie(entry.pool_slot,entry.event.movie_index,
                  entry.event.x,entry.event.y);
 if(column>1)io.draw_end_overlay();
 ++s.frame_count;
}
} // namespace btb::original_gameplay3
