// Optional end-to-end source gameplay diagnostic. The caller supplies the
// original installed retail data; the test never writes to that directory.
// The driver receives only normal keyboard input. The route planner reads
// observed scene positions, but cannot alter entity state or completion.
#include "btb/full_game_herding.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"
#include "btb/herding_source_pickles_controls.hpp"
#include "btb/herding_source_registration_point.hpp"

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

namespace fg=btb::full_game;
namespace h=btb::herding;
using V=h::Vec2i;

namespace {

std::vector<V> plan_route(const fg::HerdingScene& scene,
                          const std::vector<V>& nav,V target,h::FoodType food) {
    constexpr int N=160,M=110,S=8;
    const auto& p=scene.entities.front();
    const int start=std::clamp(p.x/S,0,N-1)+std::clamp(p.y/S,0,M-1)*N;
    std::array<int,N*M> prev;
    prev.fill(-1);
    std::queue<int> q;
    q.push(start);
    prev[start]=start;
    int best=start,best_score=INT_MAX;
    auto can=[&](int x,int y) {
        // Don't inadvertently change food while guiding a flock.
        for(auto other:{h::FoodType::DuckFood,h::FoodType::RabbitFood,
                        h::FoodType::SheepFood})
            if(other!=food&&other!=scene.selected_food&&
               h::food_pickup_in_range(other,x,y))return false;
        const auto inside=btb::retail_geometry::original_polygon_contains(nav,x,y);
        if(!inside||!*inside)return false;
        const auto feet=h::original_pickles_feet_rect(p,float(x),float(y));
        if(!feet)return false;
        for(const auto& e:scene.entities) {
            if(!h::original_pickles_has_obstacle(e.entity_type()))continue;
            auto obstacle=h::original_pickles_obstacle_rect(e);
            if(obstacle&&h::original_herding_rect_contact(*feet,*obstacle))return false;
        }
        return true;
    };
    while(!q.empty()) {
        const int a=q.front();q.pop();
        const int x=a%N,y=a/N;
        const int dx=x*S-target.x,dy=y*S-target.y;
        const int score=dx*dx+dy*dy;
        if(score<best_score){best=a;best_score=score;}
        if(score<64)break;
        for(auto [u,v]:std::array<V,8>{{{1,0},{-1,0},{0,1},{0,-1},
                                       {1,1},{1,-1},{-1,1},{-1,-1}}}) {
            const int nx=x+u,ny=y+v;
            if(nx<0||nx>=N||ny<0||ny>=M)continue;
            const int n=nx+ny*N;
            if(prev[n]>=0||!can(nx*S,ny*S)||
               !can((x+nx)*S/2,(y+ny)*S/2)||
               (u&&v&&(!can(nx*S,y*S)||!can(x*S,ny*S))))continue;
            prev[n]=a;q.push(n);
        }
    }
    std::vector<V> path;
    for(int a=best;a!=start;a=prev[a])path.push_back({a%N*S,a/N*S});
    std::reverse(path.begin(),path.end());
    return path;
}

int play(const std::filesystem::path& data,int difficulty,unsigned seed) {
    btb::retail::OriginalRetailRandom rng{seed};
    fg::HerdingDriver driver(data,
        std::make_unique<fg::HerdingEventSimulation>(
            std::make_unique<fg::NativePicklesMotionSource>(rng)),
        difficulty,&rng);
    std::string error;
    if(!driver.initialize(0,error))throw std::runtime_error(error);
    std::ifstream f(data/"herd.txt");
    const auto nav=h::parse_data(f).retail_transformed_group0();
    fg::ActivityFrameInput input;
    input.pointer_position_valid=true;
    input.pointer_x=0;input.pointer_y=0;
    // No audio device is hosted here: managed voice observations are idle.
    // This tests gameplay and the emitted voice/progress commands, not WAV
    // timing or the interactive Windows presentation.
    int tick=0,phase=0,kind=1,phase_start=0,animal_index=-1;
    V target{240,414},prior_target{-999,-999};
    std::vector<V> path;
    int path_tick=-100;
    auto step=[&](int keys) {
        input.directional_input_bits=static_cast<std::uint8_t>(keys);
        input.elapsed_centiseconds=tick++;
        auto frame=driver.advance(input);
        if(frame.fatal_error)throw std::runtime_error(
            "tick "+std::to_string(tick)+": "+*frame.fatal_error);
        return frame;
    };
    step(2);step(2); // switch to keyboard using the retail input controller
    const int expected=h::initial_undelivered_animal_count(difficulty);
    for(int iteration=0;iteration<50000;++iteration) {
        const auto& scene=driver.scene();
        const auto p=scene.entities.front();
        const auto species=static_cast<h::EntityType>(kind);
        const auto food=h::required_food_for(species);
        const auto hotspot=h::food_pickup_hotspot(food);
        int remaining=0,delivered=0;
        for(const auto& e:scene.entities) {
            if(h::is_herd_animal(e.entity_type())&&e.behavior_state==99)++delivered;
            if(e.entity_type()==species&&e.behavior_state!=99)++remaining;
        }
        if(delivered==expected) {
            auto out=step(0);
            for(int i=0;i<30&&!out.save_and_unload;++i)out=step(0);
            const bool complete=out.save_and_unload&&
                out.next_outer_state==static_cast<int>(btb::game_flow::State::PlayAgainYesNoSetup)&&
                out.next_saved_state==static_cast<int>(btb::game_flow::State::HerdingInit)&&
                out.next_replay_class==1&&out.shared_completion_code==0&&
                out.progress_writes.size()==1&&
                out.progress_writes[0].slot==btb::progress::Slot::PetsCorner&&
                out.progress_writes[0].value==1;
            std::cout<<"difficulty="<<difficulty<<" seed="<<seed<<" ticks="<<tick
                     <<" delivered="<<delivered<<"/"<<expected
                     <<" completion="<<complete<<"\n";
            if(!driver.unload(error))throw std::runtime_error(error);
            return complete?0:4;
        }
        if(!remaining) {
            ++kind;phase=0;animal_index=-1;phase_start=tick;
            if(kind>3)throw std::runtime_error("delivery count disagrees with species states");
            continue;
        }
        if(phase!=0&&scene.selected_food!=food){phase=0;phase_start=tick;}
        if(phase==0) {
            target={hotspot.target.x-hotspot.pickles_test_offset.x,
                    hotspot.target.y-hotspot.pickles_test_offset.y};
            if(scene.selected_food==food){phase=1;phase_start=tick;}
        }
        if(phase==1) {
            int distance=1000000000;
            for(int i=1;i<static_cast<int>(scene.entities.size());++i) {
                const auto& e=scene.entities[i];
                if(e.entity_type()!=species||e.behavior_state!=0)continue;
                const auto anchor=h::original_herding_animal_tracking_anchor(e);
                if(!anchor)throw std::runtime_error("invalid observed animal anchor");
                const int x=anchor->x,y=anchor->y-(p.source_bottom-p.source_top-20);
                const int dd=(x-p.x)*(x-p.x)+(y-p.y)*(y-p.y);
                if(dd<distance){distance=dd;target={x,y};animal_index=i;}
            }
            if(distance<2500){phase=2;phase_start=tick;}
            else if(distance==1000000000){phase=3;phase_start=tick;}
        }
        if(phase==2) {
            // Reach a nearby player position, not the animal's final pen
            // position. The original follower AI performs the actual delivery.
            if(kind==1)target={780,120};
            if(kind==2)target={900,95};
            if(kind==3)target={1060,280};
            if(scene.entities[animal_index].behavior_state!=0){phase=3;phase_start=tick;}
            else if(tick-phase_start>2500){phase=1;phase_start=tick;}
        }
        if(phase==3) {
            target={p.x,p.y};
            bool pending=false;
            for(const auto& e:scene.entities)
                if(e.entity_type()==species&&e.behavior_state>0&&e.behavior_state!=99)pending=true;
            if(!pending||tick-phase_start>2500){phase=1;phase_start=tick;}
        }
        if(tick-path_tick>=50||
           std::abs(target.x-prior_target.x)+std::abs(target.y-prior_target.y)>60||
           path.empty()) {
            path=plan_route(scene,nav,target,food);
            prior_target=target;path_tick=tick;
        }
        while(!path.empty()&&std::abs(path.front().x-p.x)<=2&&
              std::abs(path.front().y-p.y)<=2)path.erase(path.begin());
        const V goal=path.empty()?target:path.front();
        int keys=0;
        if(goal.x>p.x+2)keys|=2;
        if(goal.x<p.x-2)keys|=1;
        if(goal.y>p.y+2)keys|=8;
        if(goal.y<p.y-2)keys|=4;
        if(phase==3)keys=0;
        auto frame=step(keys);
        if(frame.save_and_unload)throw std::runtime_error("premature activity exit");
    }
    int delivered=0;
    for(const auto& e:driver.scene().entities)
        if(h::is_herd_animal(e.entity_type())&&e.behavior_state==99)++delivered;
    std::cerr<<"input budget exhausted: delivered="<<delivered<<"/"<<expected<<"\n";
    return 3;
}

} // namespace

int main(int argc,char** argv) {
    if(argc<3||argc>4) {
        std::cerr<<"usage: btb_retail_herding_playthrough <SubGame1 dir> <difficulty 0..2> [seed]\n";
        return 2;
    }
    try {
        const int difficulty=std::stoi(argv[2]);
        const unsigned seed=argc==4?static_cast<unsigned>(std::stoul(argv[3])):1;
        if(difficulty<0||difficulty>2)return 2;
        return play(argv[1],difficulty,seed);
    } catch(const std::exception& e) {
        std::cerr<<e.what()<<"\n";return 1;
    }
}
