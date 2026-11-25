// Installed-data navigation and repair probe. No activity-state writes.
#include "btb/full_game_spud_maze.hpp"
#include "retail_activity_audit.hpp"
#include <cmath>
#include <functional>
#include <memory>
#include <queue>

namespace {
using namespace btb::retail_probe;
using Key=std::pair<int,int>;
using Player=btb::original_motion4::Player;
constexpr int keys[]{4,2,8,1};

const btb::maze::Node& node(const Player& player,Key key) {
    for(const auto& n:player.graphs.at(key.first).nodes)
        if(n.id==key.second)return n;
    throw std::runtime_error("invalid diagnostic Spud Maze node");
}

// Find a reachable directed track segment near a damaged tile or the
// dropped hammer. Actual collision and repairs remain in the game.
int choose_direction(const fg::SpudMazeDriver& game) {
    const auto& player=*game.bob();
    const Key start{player.screen,player.node};
    std::map<Key,double> distance;
    std::map<Key,int> first;
    using Entry=std::pair<double,Key>;
    std::priority_queue<Entry,std::vector<Entry>,std::greater<Entry>> pending;
    distance[start]=0;first[start]=0;pending.push({0,start});
    while(!pending.empty()) {
        const auto [cost,key]=pending.top();pending.pop();
        if(cost!=distance.at(key))continue;
        const auto& from=node(player,key);
        const auto offer=[&](Key next,double edge,int direction) {
            const auto candidate=cost+edge;
            if(!distance.contains(next)||candidate<distance.at(next)) {
                distance[next]=candidate;
                first[next]=key==start?direction:first.at(key);
                pending.push({candidate,next});
            }
        };
        for(int i=0;i<4;++i)if(from.links[i]>=0) {
            const Key next{key.first,from.links[i]};
            const auto& to=node(player,next);
            offer(next,std::hypot(double(from.retail_position.x-to.retail_position.x),
                                  double(from.retail_position.y-to.retail_position.y)),keys[i]);
        }
        // At an arrival portal Bob is already displaced inward. A return
        // through that portal requires a real corridor traversal, not a
        // zero-input graph edge. Other portals use the source pair IDs.
        if(key!=start&&(from.node_type&0x480))offer({key.first-1,from.id-2},10,0);
        else if(key!=start&&(from.node_type&0x900))offer({key.first+1,from.id+2},10,0);
    }
    double best=std::numeric_limits<double>::infinity();
    int direction=0;
    const auto target=[&](int screen,double x,double y,bool hammer) {
        for(const auto& from:player.graphs.at(screen).nodes) {
            const Key key{screen,from.id};
            if(!distance.contains(key))continue;
            for(int i=0;i<4;++i)if(from.links[i]>=0) {
                const auto& to=node(player,{screen,from.links[i]});
                const double dx=to.retail_position.x-from.retail_position.x;
                const double dy=to.retail_position.y-from.retail_position.y;
                const double length=std::hypot(dx,dy);
                if(length==0)continue;
                const int candidate=key==start?keys[i]:first.at(key);
                if(!candidate)continue;
                // Sample the original corridor, not just its junctions:
                // repair centers are generally BETWEEN graph nodes.
                for(double u=0;u<=1;u+=1.0/std::max(1.,length/2)) {
                    const double xx=from.retail_position.x+dx*u;
                    const double yy=from.retail_position.y+dy*u;
                    const double gap=std::hypot(xx-x,yy-y);
                    const bool eligible=hammer?(std::abs(xx-x)<18&&yy>352):
                                               (std::abs(xx-x)<12&&gap<36);
                    if(!eligible)continue;
                    double cost=distance.at(key)+u*length+gap*.2;
                    if(key==start) {
                        const double projection=((player.x-from.retail_position.x)*dx+
                            (player.y-from.retail_position.y)*dy)/(length*length);
                        if(u+0.02<projection)continue;
                        cost=std::max(0.,(u-projection)*length)+gap*.2;
                    }
                    if(cost<best){best=cost;direction=candidate;}
                }
            }
        }
    };
    if(!player.repair_tool_engaged) {
        const auto& hammer=game.hammer_state().hammer;
        target(hammer.screen,hammer.x,375,true);
    } else {
        for(int screen=0;screen<5;++screen)
            for(const auto& repair:player.repairs[screen])if(repair.counter>0)
                target(screen,repair.x+repair.width/2,repair.y+repair.height+20,false);
    }
    return direction;
}

void play(const fs::path& root,int difficulty,bool windowed,unsigned seed) {
    Audit audit{fg::OriginalAssetResolver(root)};
    VoiceModel voices;
    btb::retail::OriginalRetailRandom rng(seed);
    const int profile=int((seed+difficulty+int(windowed))%btb::progress::kPlayerCount);
    fg::GameRoot game;
    check(game.select_profile(profile),"profile rejected");
    auto driver=std::make_unique<fg::SpudMazeDriver>(root,difficulty,&rng);
    auto* live=driver.get();game.install(fg::ActivityId::SpudMaze,std::move(driver));
    game.set_outer_state(State::SpudMazeInit);
    check(game.advance({.original_windowed_display=windowed}).kind==fg::FrameKind::ActivityInitialized,
          "Spud Maze initialization failed");
    const int total=live->remaining_repairs();
    int remaining=total,direction=0,last_screen=-1,last_node=-1,observed_screen=-1;
    unsigned portals=0,drops=0,recoveries=0;
    bool hammer=true;
    for(int tick=0;tick<30000;++tick) {
        const auto* player=live->bob();check(player!=nullptr,"Spud Maze unloaded early");
        if(observed_screen>=0&&observed_screen!=player->screen)++portals;
        observed_screen=player->screen;
        if(hammer!=player->repair_tool_engaged) {
            if(hammer)++drops;else ++recoveries;
            hammer=player->repair_tool_engaged;
        }
        // Stay on the current axis until the next captured junction,
        // even if the hammer is knocked away midway along a corridor.
        if(!player->hazard_lock&&!player->transition&&
            (player->screen!=last_screen||player->node!=last_node||!direction)) {
            direction=choose_direction(*live);last_screen=player->screen;last_node=player->node;
        }
        bool click=false;
        for(const auto& repair:player->repairs[player->screen]) {
            const int dx=int(player->x)-repair.x-repair.width/2;
            const int dy=int(player->y)-repair.y-repair.height-20;
            if(repair.counter>0&&player->repair_tool_engaged&&std::abs(dx)<14&&
                std::hypot(double(dx),double(dy))<39)click=true;
        }
        fg::ActivityFrameInput input{};
        input.elapsed_centiseconds=tick*2;
        input.original_windowed_display=windowed;
        input.directional_input_bits=click?0:direction;
        input.click_pulse=click;
        voices.sample(input);
        const auto step=game.advance(input);
        check(step.kind==fg::FrameKind::ActivityUpdated,"Spud Maze advance failed: "+step.error);
        audit.observe(step.effects);voices.observe(step.effects);
        if(step.effects.save_and_unload) {
            check(remaining==0,"Spud Maze timed out with outstanding repairs");
            check(!live->bob(),"Spud Maze did not unload");
            completion(game,step.effects,Slot::SpudMaze,State::SpudMazeInit,profile);
            std::cout<<"spud_maze difficulty="<<difficulty<<" windowed="<<windowed
                     <<" seed="<<seed<<" repaired="<<total<<" portals="<<portals
                     <<" hammer_drops="<<drops<<" hammer_recoveries="<<recoveries;
            audit.report();return;
        }
        remaining=live->remaining_repairs();
    }
    throw std::runtime_error("Spud Maze diagnostic exhausted frame budget");
}
}

int main(int argc,char** argv) {
    if(argc<2||argc>3) {
        std::cerr<<"usage: btb_retail_spud_maze_playthrough <installed-root> [seed-count]\n";
        return 2;
    }
    try {
        const int seeds=argc==3?std::stoi(argv[2]):1;
        check(seeds>=1&&seeds<=100,"seed count outside 1..100");
        for(unsigned seed=1;seed<=unsigned(seeds);++seed)
            for(int difficulty=0;difficulty<3;++difficulty)
                for(bool windowed:{false,true})play(argv[1],difficulty,windowed,seed);
        return 0;
    } catch(const std::exception& e) {std::cerr<<e.what()<<'\n';return 1;}
}
