#include "btb/original_motion4.hpp"
#include "btb/maze_input.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
namespace btb::original_motion4 {
namespace {
const maze::Node& get_node(const maze::ScreenGraph& g,I32 id){
 for(const auto& n:g.nodes)if(n.id==id)return n;
 throw std::out_of_range("original Maze node is absent from source graph");
}
void sound(IO& io,I32 id,I32 prio=10){io.sound(id,prio,1);}
void advance_direction(Player& p,IO& io,const Input& in,bool spud){
 if(in.dx>1||in.dx< -1||in.dy>1||in.dy< -1)
  throw std::invalid_argument("invalid original keyboard axis");
 const I32 dx=in.dx,dy=in.dy;
 const auto& graph=p.graphs[static_cast<std::size_t>(p.screen)];
 const I32 ix=static_cast<I32>(p.x),iy=static_cast<I32>(p.y);
 // 0041B140/00421360 scan even on idle and blocked input.
 const auto scan=maze::scan_player_navigation_nodes(graph,{ix,iy},p.mouse_mode,
     {p.node,p.last_captured_node},spud?20.0L:30.0L,spud?6.0L:30.0L);
 p.last_captured_node=scan.state.last_captured_node;
 if(scan.captured_node>=0)p.node=scan.captured_node;
 // Original node flag priority: 0x80 first, 0x100 next, 0x200 last.
 // A portal changes the game screen only if node capture occurred.
 if(scan.captured_node>=0){
  const auto& captured=get_node(graph,p.node);
  if((captured.node_type&0xf80)!=0){
   // 0041B140/00421360: entering a neighboring screen preserves
   // the actual paired portal, with the original inward displacement.
   // Landing at dst.nodes.front() incorrectly teleports East->Middle
   // to the west entrance and both Spud tracks to the same exit.
   const I32 old_screen=p.screen;
   I32 destination=old_screen,target_id=-1;
   float inward=0.f;
   if(!spud){
    if(const auto dest=captured.portal_destination()){
     destination=static_cast<I32>(*dest);
     if(destination==0){target_id=0;inward=-25.f;}
     else if(destination==2){target_id=0;inward=25.f;}
     else if(old_screen==0){target_id=0;inward=25.f;}
     else {target_id=15;inward=-25.f;}
    }
   }else{
    if((captured.node_type&0x480)!=0){
     destination=old_screen-1;target_id=captured.id-2;inward=-15.f;
    }else if((captured.node_type&0x900)!=0){
     destination=old_screen+1;target_id=captured.id+2;inward=15.f;
    }
   }
   if(target_id<0)return;
   if(destination<0||destination>=(spud?5:3))
    throw std::out_of_range("original Maze portal destination");
   const auto& paired=get_node(p.graphs[static_cast<std::size_t>(destination)],target_id);
   p.screen=destination;
   p.node=target_id;
   p.x=static_cast<float>(paired.retail_position.x)+inward;
   p.y=static_cast<float>(paired.retail_position.y);
   p.last_captured_node=-1;
   p.transition=1;p.movement_mask=10;
   if(spud)io.spud_spawn_delay_reset();
   p.movement_latch=10;
   return;
  }
  p.movement_mask=captured.direction_bits;
 }
 const auto& current=get_node(graph,p.node);
 const auto step=(p.speed>0&&std::isfinite(p.speed))?p.speed:0.f;
 // 0041B4EA/00421733: keyboard attraction to a nearby turn
 // corrects the perpendicular coordinate, with tolerance 1.9.
 if(scan.attraction_node>=0 && (dx==0)!=(dy==0)) {
  const auto& a=get_node(graph,scan.attraction_node);
  const auto dir=dx>0?maze::Right:dx<0?maze::Left:dy<0?maze::Up:maze::Down;
  if(a.linked_node(dir)>=0){
   auto& v=dx!=0?p.y:p.x;
   const float target=static_cast<float>(dx!=0?a.retail_position.y:a.retail_position.x);
   if(std::fabs(v-target)>1.9f&&(!spud||p.can_move_x))
    v+=v<target?step:-step;
  }
 }
 const auto allowed=[&](maze::Direction d){return (p.movement_mask&d)!=0;};
 const auto horizontal=[&](){
  if(dx==0||!allowed(dx>0?maze::Right:maze::Left))return false;
  if(!spud||p.can_move_x)p.x+=step*static_cast<float>(dx);
  p.facing=dx>0?1:3;p.movement_mask=10;p.movement_latch=10;
  return true;
 };
 const auto vertical=[&](){
  if(dy==0||!allowed(dy<0?maze::Up:maze::Down))return false;
  const auto dir=dy<0?maze::Up:maze::Down;
  const auto link=current.linked_node(dir);
  // Vertical corridors are not always axis-aligned in the original
  // maps. Use the source integer heading and separate float stores.
  const auto* target=&current;
  if(link>=0){
   const auto& linked=get_node(graph,link);
   if((dy<0&&p.y>=linked.retail_position.y)||
      (dy>0&&p.y<=linked.retail_position.y))target=&linked;
  }else if((dy<0&&p.y<=current.retail_position.y)||
           (dy>0&&p.y>=current.retail_position.y))return false;
  if(!spud||p.can_move_x){
   const maze::Vec2i from{static_cast<I32>(p.x),static_cast<I32>(p.y)};
   const auto to=target->retail_position;
   if(std::hypot(double(from.x)-to.x,double(from.y)-to.y)<=2.0)
    p.y+=step*static_cast<float>(dy);
   else{
    const auto degrees=maze::angle_between_integer_points_degrees(from,to);
    const long double rad=degrees*static_cast<long double>(0.017453530803322792f);
    p.y=static_cast<float>(p.y-std::cos(rad)*step);
    p.x=static_cast<float>(p.x+std::sin(rad)*step);
   }
  }
  p.facing=dy<0?0:2;p.movement_mask=5;p.movement_latch=5;
  return true;
 };
 const bool vertical_first=scan.repeated_current_capture?
    (p.movement_latch&1)!=0:(p.movement_latch&2)!=0;
 const bool moved=vertical_first?(vertical()||horizontal()):(horizontal()||vertical());
 if(moved){++p.input_ticks;if(spud)++p.animation;}
 // 0041BD09/004220D0: one-pixel orthogonal correction inside 8 units.
 if(std::hypot(double(static_cast<I32>(p.x))-current.retail_position.x,
               double(static_cast<I32>(p.y))-current.retail_position.y)<8.0){
  auto& v=(p.movement_latch&1)?p.x:p.y;
  const float target=static_cast<float>((p.movement_latch&1)?current.retail_position.x:current.retail_position.y);
  if(std::abs(static_cast<I32>(v-target))>1)v+=v<target?1.f:-1.f;
 }
}
void update_player(Player& p,IO& io,bool spud){
 if(p.screen<0||p.screen>=(spud?5:3))
  throw std::out_of_range("original Maze screen");
 p.previous_x=p.x;p.previous_y=p.y;
 if(p.frozen)return;
 const Input input=io.player_input(spud);
 if(spud){
  p.occupied=(input.dx!=0||input.dy!=0);
  if(p.hazard_lock)return;
 }
 advance_direction(p,io,input,spud);
 if(spud){
  auto& repairs=p.repairs[static_cast<std::size_t>(p.screen)];
  for(std::size_t i=0;i<repairs.size();++i){
   auto& r=repairs[i];
   if(r.counter<=0)continue;
   // Original 004221A? reads the individual sprite width/height:
   // center X, bottom Y plus 20; 0043B4B0 contains float32 40.
   const I32 cx=r.x+r.width/2,cy=r.y+r.height+20;
   const I32 px=static_cast<I32>(p.x),py=static_cast<I32>(p.y);
   const auto delta=io.actor_distance(static_cast<float>(px),static_cast<float>(py),
                                     static_cast<float>(cx),static_cast<float>(cy));
   if(delta>=40.f||std::abs(cx-px)>=15||!input.action_pressed)continue;
   if(!p.repair_tool_engaged){
    const I32 voice=0x2c5 + static_cast<I32>(io.random32()%2);
    sound(io,voice,50);
   }else{
    I32 voice=0x2be+static_cast<I32>(io.random32()%7);
    // Source rejects the previous repair voice (0051453C).
    // Guard a broken random provider without inventing a substitute.
    for(int retry=0;voice==p.repair_sound;++retry){
     if(retry>=4096)throw std::runtime_error("original repair voice RNG exhausted");
     voice=0x2be+static_cast<I32>(io.random32()%7);
    }
    p.repair_sound=voice;
    sound(io,voice,50);
    r.counter=0;p.repair_hit_count+=1;p.animation=9;p.hazard_lock=true;
    io.spud_repair_completed(p.screen,static_cast<I32>(i));
   }
  }
  // The decompiled final 0x0051451c flag is always cleared after
  // repair/collision handling. Maintain no latent permission latch.
  p.can_move_x=true;
 }
}
const maze::Node& npc_node(const Npc& s,I32 n){
 if(s.screen<0||s.screen>=3)throw std::out_of_range("NPC Maze screen");
 return get_node(s.graphs[static_cast<std::size_t>(s.screen)],n);
}
void set_npc_path(Npc& s,IO& io,I32 destination){
 auto& g=s.graphs[static_cast<std::size_t>(s.screen)];
 s.path=io.shortest_path(g,s.current_node,destination);
 if(s.path.empty())throw std::runtime_error("original Maze shortest path returned no nodes");
 s.path_index=0;
}
bool npc_step(Npc& s,IO& io,I32 speed){
 if(s.path_index<0||s.path_index>=static_cast<I32>(s.path.size()))
  throw std::out_of_range("original Spud path-index");
 const auto target=get_node(s.graphs[static_cast<std::size_t>(s.screen)],s.path[static_cast<std::size_t>(s.path_index)]);
 maze::move_actor_toward_node(s.graphs[static_cast<std::size_t>(s.screen)],target.id,s.position,speed);
 const float distance=io.actor_distance(s.position.x,s.position.y,
   static_cast<float>(target.retail_position.x),static_cast<float>(target.retail_position.y));
 return distance<3.f;
}
void decrement_herd_timer(HerdAnimal& animal){
 if(animal.source.animation_frame_countdown>0)
  --animal.source.animation_frame_countdown;
}
}
void update_maze_player(Player& p,IO& io){update_player(p,io,false);}
void update_spud_maze_bob(Player& p,IO& io){update_player(p,io,true);}
void update_maze_spud_npc(Npc& s,IO& io){
 if(s.screen<0||s.screen>=3||s.player_screen<0||s.player_screen>=3)
  throw std::out_of_range("Spud NPC source screen");
 const auto gate=maze::spud_offscreen_gate(s.phase,s.screen,s.player_screen,s.offscreen_wait);
 s.offscreen_wait=gate.frames_remaining;
 if(gate.restore_selected_package_to_available&&s.selected_package>=0&&s.selected_package<4)
  s.packages[static_cast<std::size_t>(s.screen)][static_cast<std::size_t>(s.selected_package)].status=1;
 s.phase=gate.next_phase;
 if(!gate.dispatch_state)return;
 switch(s.phase){
 case maze::SpudNpcPhase::SpawnWait:{
  const auto step=maze::advance_spud_spawn_wait(s.spawn_interval,s.spawn_wait);
  s.spawn_wait=step.frames_remaining;s.phase=step.next_phase;
  break;
 }
 case maze::SpudNpcPhase::SelectPackage:{
  // Source 0x0041CCB0 state 1 sets 0x00444220 to the PLAYER'S
  // current 0x00510D18 screen before spawning/selecting a target.
  // Otherwise an off-screen reset can strand Spud in the previous map.
  s.screen=s.player_screen;
  s.animation_frame=7;
  if(s.screen==1)s.current_node=maze::spud_random_edge_node(1,static_cast<I32>(io.random32()%2));
  else s.current_node=0;
  const auto& first=npc_node(s,s.current_node);
  s.position={static_cast<float>(first.retail_position.x),
              static_cast<float>(first.retail_position.y)};
  const auto& bank=s.packages[static_cast<std::size_t>(s.screen)];
  const auto n=maze::available_spud_package_count(bank);
  if(n==0){s.phase=maze::SpudNpcPhase::SpawnWait;s.spawn_wait=s.spawn_interval;break;}
  const auto chosen=maze::select_spud_package_by_rank(bank,static_cast<I32>(io.random32()%static_cast<U32>(n)));
  if(!chosen)throw std::runtime_error("NPC package rank unresolved");
  s.selected_package=*chosen;
  sound(io,0x58);
  const auto location=bank[static_cast<std::size_t>(*chosen)].node_id;
  if(location<0||location>=4)
   throw std::out_of_range("original Maze Spud package location 0..3");
  const auto target=s.has_box_target_nodes
    ?s.package_target_nodes[static_cast<std::size_t>(s.screen)]
                           [static_cast<std::size_t>(location)]
    :location;
  set_npc_path(s,io,target);
  s.phase=maze::SpudNpcPhase::TravelToPackage;
  s.animation_countdown=7;
  break;
 }
 case maze::SpudNpcPhase::TravelToPackage:{
  const bool arrived=npc_step(s,io,s.difficulty==2?2:1);
  if(!arrived)break;
  auto& bank=s.packages[static_cast<std::size_t>(s.screen)];
  const auto& slot=bank[static_cast<std::size_t>(s.selected_package)];
  const auto result=maze::advance_spud_package_route(0,s.path_index,
     static_cast<I32>(s.path.size()),slot.status);
  s.path_index=result.next_path_index;s.phase=result.next_phase;
  if(result.mark_package_taken){
   bank[static_cast<std::size_t>(s.selected_package)].status=2;
   if(result.play_pickup_voice)sound(io,0x59);
   s.animation_frame+=result.animation_advance;s.carry_active=1;
  }
  break;
 }
 case maze::SpudNpcPhase::BeginReturnPreparation:
  s.phase=maze::spud_begin_return_phase();
  break;
 case maze::SpudNpcPhase::ChooseReturnRoute:{
  const auto& node=npc_node(s,s.current_node);
  const auto choice=maze::select_spud_return_neighbor(node.links,s.current_direction,
       static_cast<I32>(io.random32()%4),-1,{{-1,-1,-1,-1}});
  s.current_direction=choice.direction;
  s.nearest_node_distance=999999;
  if(!choice.found_eligible_neighbor){
   s.phase=maze::SpudNpcPhase::ResetPath;break;
  }
  s.path={choice.node_id};s.path_index=0;
  s.phase=maze::SpudNpcPhase::TravelCarryingPackage;
  s.animation_countdown=20;
  break;
 }
 case maze::SpudNpcPhase::TravelCarryingPackage:{
  const bool arrived=npc_step(s,io,maze::spud_carry_speed(s.difficulty));
  const auto player_gap=io.npc_distance_to_player(s);
  const auto plan=maze::advance_spud_carry_frame(arrived?0:10,
    s.nearest_node_distance,player_gap,s.collision_latch!=0,
    1,0,0);
  s.nearest_node_distance=plan.nearest_node_distance;
  if(plan.captured_waypoint){s.current_node=s.path[static_cast<std::size_t>(s.path_index)];s.phase=plan.next_phase;}
  if(plan.drop_package){
   s.carry_active=0;s.collision_latch=1;
   // Retail 0x0041D1CE stores status 3 and the Spud's actual
   // truncated x/y into package-record offsets +12 and +16.
   if(s.selected_package<0||s.selected_package>=4)
    throw std::out_of_range("Spud dropped package index");
   auto& dropped=s.packages[static_cast<std::size_t>(s.screen)]
                           [static_cast<std::size_t>(s.selected_package)];
   dropped.status=maze::kSpudDroppedPackageStatus;
   dropped.dropped_x=static_cast<I32>(s.position.x);
   dropped.dropped_y=static_cast<I32>(s.position.y);
   if(plan.collision_voice) sound(io,
       0x5e + static_cast<I32>(io.random32()%2),10);
  }
  break;
 }
 case maze::SpudNpcPhase::ResetPath:{
  const auto step=maze::spud_return_route_build(s.screen,
    static_cast<I32>(io.random32()%2),20);
  s.collision_latch=0;s.carry_active=0;s.animation_countdown=step.animation_countdown;
  set_npc_path(s,io,step.target_node);
  s.phase=step.next_phase;
  break;
 }
 case maze::SpudNpcPhase::TravelHome:{
  const bool arrived=npc_step(s,io,s.difficulty==2?2:1);
  if(!arrived)break;
  const auto step=maze::advance_spud_home_route(0,s.path_index,
    static_cast<I32>(s.path.size()));
  s.path_index=step.next_path_index;s.phase=step.next_phase;
  if(step.reset_spawn_countdown)s.spawn_wait=*step.reset_spawn_countdown;
  break;
 }
 }
}
I32 update_herding_animal(HerdAnimal& s,IO& io){
 auto& a=s.source;
 const auto type=a.entity_type();
 if(a.entity_id<0)return 0;
 if(!herding::is_herd_animal(type))return 0;
 decrement_herd_timer(s);
 a.previous_x=a.x;a.previous_y=a.y;
 const I32 behavior=a.behavior_state;
 if(behavior==99){
  a.direction=4;
  if(s.direction_countdown>0)--s.direction_countdown;
  else{
   a.animation_frame+=1;
   if(a.animation_frame>12)a.animation_frame=9;
   s.direction_countdown=150;
  }
  return 0;
 }
 // Retail entrance and home-route behaviors 10..15 and 20..25 use
 // original waypoints recovered in herding_runtime, not guessed targets.
 if((behavior>=10&&behavior<=15)||(behavior>=20&&behavior<=25)){
  const auto target=behavior<20?herding::home_entrance_target(type):
                       herding::home_entry_target(type,behavior).value_or(herding::Vec2i{-1,-1});
  if(target.x<0)throw std::runtime_error("unknown original herd home waypoint");
  const I32 angle=io.integer_angle(a.x,a.y,target.x,target.y);
  a.direction=((angle+22)/45)%8;
  if(a.direction<0)a.direction+=8;
  a.movement_speed=std::min(s.max_speed,a.movement_speed+s.acceleration);
  const auto move=io.movement_delta(angle,a.movement_speed);
  a.x_float+=move[0];a.y_float+=move[1];
  a.x=static_cast<I32>(a.x_float);a.y=static_cast<I32>(a.y_float);
  a.movement_active=1;
  const auto distance=io.actor_distance(a.x_float,a.y_float,
                             static_cast<float>(target.x),static_cast<float>(target.y));
  if(distance<s.arrival_radius){
   if(auto result=herding::home_route_arrival_step(behavior,s.undelivered)){
    a.behavior_state=result->next_behavior_state;
    s.undelivered=result->next_undelivered_count;
    if(result->delivered)s.collision_latch=true;
   }
  }
  return 0;
 }
 if(behavior==1){
  // Source case 1 increments a per-species transition counter. The
  // user-facing managed animal sound is type+0x253 at difficulty+3.
  if(++s.home_counter==s.difficulty+3)
   sound(io,a.type+0x253,90);
  // Retail's state-one behavior counter does not immediately force a
  // home-route transition; the later collision decision owns that change.
  return 0;
 }
 // Other retail states (roam, follow, brake, temporary targets) already
 // have recovered per-branch modules. Their collision-dependent decisions
 // are not approximated or silently skipped here.
 if(io.original_herding_boundary_collision(a))
  io.animal_collision_recovery(a);
 return 0;
}
}
