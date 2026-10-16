#include "btb/spud_maze_pilchard_native.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace btb::spud_maze {
namespace {
const Node& find_node(const Data& data,int screen,int id) {
    if(screen<0||screen>=static_cast<int>(kScreenCount))
        throw std::out_of_range("Spud Maze Pilchard screen");
    const auto& bank=data.screens[static_cast<std::size_t>(screen)];
    for(const auto& node:bank)
        if(node.index==id)return node;
    throw std::runtime_error("original Pilchard graph node missing");
}

void set_path(PilchardNative& s,const Data& data,int target) {
    auto result=shortest_graph_path(
        data.screens[static_cast<std::size_t>(s.screen)],
        s.current_node,target);
    if(!result.found||result.node_count==0)
        throw std::runtime_error("original Pilchard shortest path failed");
    s.route.assign(result.nodes.begin(),
                   result.nodes.begin()+result.node_count);
    s.route_index=1;
}

bool advance_path(PilchardNative& s,const Data& data,int speed) {
    if(s.route_index<0||s.route_index>=static_cast<int>(s.route.size()))
        return true;
    const auto& next=find_node(data,s.screen,
                              s.route[static_cast<std::size_t>(s.route_index)]);
    const auto dx=static_cast<float>(next.x)-s.x;
    const auto dy=static_cast<float>(next.y)-s.y;
    const auto dist=std::hypot(dx,dy);
    if(dist>0.0f) {
        const float travel=std::min(dist,static_cast<float>(speed));
        s.x+=dx/dist*travel;
        s.y+=dy/dist*travel;
        s.roaming_direction=dx>=0?1:3;
    }
    if(std::hypot(static_cast<float>(next.x)-s.x,
                  static_cast<float>(next.y)-s.y)<3.0f) {
        s.current_node=next.index;
        ++s.route_index;
    }
    return s.route_index>=static_cast<int>(s.route.size());
}

int entrance_node(int screen,std::uint32_t random) {
    if(screen==0)return 0;
    if(screen==4)return 2;
    return static_cast<int>(random%4U);
}
} // namespace

PilchardFrame update_pilchard_native(
    PilchardNative& s,maze_spud_batch::CollisionState& collision,
    const Data& data,int difficulty,const PilchardBob& bob,
    const OriginalRandom& random,
    bool bob_reaction_sound_playing,
    bool pilchard_reaction_sound_playing) {
    if(difficulty<0||difficulty>=3)
        throw std::out_of_range("original Spud Maze difficulty");
    if(!random)
        throw std::invalid_argument("original Pilchard requires CRT random");
    PilchardFrame out;
    const int start_delay=data.spud_spawn_times[
        static_cast<std::size_t>(difficulty)];
    if(s.spawn_wait==-1&&s.phase==0)
        s.spawn_wait=start_delay;
    if(s.phase!=0 && s.screen!=bob.screen) {
        // Retail 00423630 keeps its off-screen actor briefly, then resets
        // it to ready-to-spawn; never collide with Bob on another screen.
        if(++s.offscreen_frames>500) {
            s.phase=1;
            s.offscreen_frames=0;
        }
        return out;
    }
    s.offscreen_frames=0;
    switch(s.phase) {
    case 0:
        if(start_delay<0)return out; // the retail Easy -1 sentinel
        if(--s.spawn_wait<1)s.phase=1;
        return out;
    case 1:{
        s.screen=bob.screen;
        const int start=entrance_node(s.screen,random());
        const auto& n=find_node(data,s.screen,start);
        s.current_node=start;s.x=static_cast<float>(n.x);
        s.y=static_cast<float>(n.y);
        s.animation_frame=9;s.animation_wait=7;
        // 00423630 entry route heads for source 005109E0 (node 2).
        set_path(s,data,2);
        s.phase=2;
        out.sound_and_hammer.push_back({
            maze_spud_batch::Op::PlayManagedSound,
            718+static_cast<int>(random()%2U),50,0
        });
        out.actor_visible=true;
        return out;
    }
    case 2:{
        const int speed=data.spud_speed_regular[
            static_cast<std::size_t>(difficulty)]/2;
        if(advance_path(s,data,std::max(1,speed)))s.phase=4;
        break;
    }
    case 3:s.phase=4;break;
    case 4:{
        const auto& from=find_node(data,s.screen,s.current_node);
        const auto offset=static_cast<int>(random()%4U);
        for(int i=0;i<4;++i) {
            const int dir=(offset+i)%4;
            const int target=from.links[static_cast<std::size_t>(dir)];
            if(target<0)continue;
            s.route={from.index,target};
            s.route_index=1;
            s.phase=5;
            break;
        }
        if(s.phase==4)s.phase=6;
        break;
    }
    case 5:{
        const int speed=data.spud_speed_package[
            static_cast<std::size_t>(difficulty)];
        if(advance_path(s,data,std::max(1,speed)))s.phase=4;
        break;
    }
    case 6:
        set_path(s,data,s.screen==4?2:0);
        s.phase=7;
        break;
    case 7:
        if(advance_path(s,data,std::max(1,data.spud_speed_regular[
              static_cast<std::size_t>(difficulty)]))) {
            s.phase=0;
            s.spawn_wait=500;
        }
        break;
    default:
        throw std::logic_error("unknown original Pilchard phase");
    }
    out.actor_visible=s.phase>=2&&s.screen==bob.screen;
    if(!out.actor_visible)return out;

    collision.current_screen=bob.screen;
    collision.bob_direction=bob.direction;
    collision.bob_animation=bob.in_repair?9:0;
    collision.collision_already_active=bob.in_repair;
    const auto effects=maze_spud_batch::handle_pilchard_collision(
        collision,{
            static_cast<int>(bob.x),static_cast<int>(bob.y),
            static_cast<int>(s.x),static_cast<int>(s.y),
            60.0f,360.0f,random(),random(),
            random,bob_reaction_sound_playing,
            pilchard_reaction_sound_playing
        });
    if(!effects.error.empty())
        throw std::runtime_error(effects.error);
    out.sound_and_hammer.insert(
        out.sound_and_hammer.end(),effects.effects.begin(),
        effects.effects.end());
    out.hammer_dropped=collision.hammer_dropped;
    if(--s.animation_wait<=0) {
        s.animation_wait=7;
        s.animation_frame=(s.animation_frame+1)%36;
    }
    return out;
}

bool pickup_dropped_hammer(
    maze_spud_batch::CollisionState& collision,
    const PilchardBob& bob) {
    // Original 0x004227F0 recovery requires the hammer's screen, a
    // strictly <20 horizontal gap and Bob below source float 350.0.
    // The normal track-repair state blocks collection.
    if(collision.hammer_held||!collision.hammer_dropped||
       bob.in_repair||collision.hammer.screen!=bob.screen ||
       bob.y<=350.0f)
        return false;
    if(!can_pick_up_hammer_x(
         static_cast<int>(bob.x),collision.hammer.x))
        return false;
    collision.hammer_held=true;
    collision.hammer_dropped=false;
    return true;
}

} // namespace btb::spud_maze
