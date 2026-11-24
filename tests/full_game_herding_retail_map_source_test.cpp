#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_native_pickles_motion.hpp"
#include "btb/full_game_herding_presentation.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace {
namespace h=btb::herding;
namespace fg=btb::full_game;

h::Data retail_geometry_fixture() {
    // The 6 setup points and 12/5/6/1 geometry groups observed in the
    // installed herd.txt. Includes the duplicate group-two vertex.
    std::istringstream source(
        "719 266\n527 114\n376 170\n478 471\n93 324\n100 120\n"
        "44 99 202 99 353 279 461 263 530 299 966 181 "
        "966 166 966 235 1086 349 1257 405 1257 900 44 900\n-1 -1\n"
        "450 360 600 179 886 139 1028 304 755 360\n-1 -1\n"
        "30 500 500 500 500 500 1200 500 1200 850 30 850\n-1 -1\n"
        "92 324\n-1 -1\n");
    return h::parse_data(source);
}

void require_frame(fg::HerdingEventSimulation& sim,
                   const fg::ActivityFrameInput& input,fg::HerdingScene& scene,
                   int& remaining,int difficulty,int frame) {
    std::vector<fg::Audio> sounds;
    std::string error;
    if(!sim.advance(input,h::pickles_keyboard_motion(input.directional_input_bits),
                    scene,remaining,sounds,error)) {
        std::fprintf(stderr,"retail map: difficulty %d frame %d hero (%d,%d): %s\n",
            difficulty,frame,scene.entities.empty()?-1:scene.entities.front().x,
            scene.entities.empty()?-1:scene.entities.front().y,error.c_str());
        std::abort();
    }
    const auto composed=fg::compose_original_herding_frame(scene);
    assert(!composed.missing_farmer_pickles);
    assert(composed.rejected_unknown_entities==0);
    assert(!composed.draws.empty());
    assert(remaining>=0 && remaining<=h::initial_undelivered_animal_count(difficulty));
}

// Initialize normal native state, changing only the first three sheep's
// starting positions to exercise the exact narrow recovery paths. The
// normal production source still supplies every event and movement.
class CrowdedSource final : public fg::OriginalHerdingMotionSource {
public:
    explicit CrowdedSource(btb::retail::OriginalRetailRandom& rng):native_(rng){}
    bool begin_frame(std::string& e) override {return native_.begin_frame(e);}
    void commit_frame() noexcept override {native_.commit_frame();}
    void rollback_frame() noexcept override {native_.rollback_frame();}
    bool initialize(const h::Data& d,int difficulty,
                    std::vector<h::RetailEntityRecord32>& records,
                    std::string& e) override {
        if(!native_.initialize(d,difficulty,records,e))return false;
        assert(records.size()>3);
        for(std::size_t i=1;i<=3;++i)assert(records[i].entity_type()==h::EntityType::Sheep);
        auto place=[&](std::size_t i,float x,float y) {
            auto& r=records[i];
            r.x_float=x;r.y_float=y;
            r.x=static_cast<int>(x);r.y=static_cast<int>(y);
            r.previous_x=r.x;r.previous_y=r.y;
        };
        place(1,31.1f,600.f);
        place(2,42.f,600.f);
        place(3,600.f,480.f);
        return true;
    }
    bool advance(const h::Data& d,const fg::ActivityFrameInput& i,
                 const h::PicklesKeyboardMotion& motion,
                 const std::vector<h::RetailEntityRecord32>& r,
                 fg::HerdingObservedFrame& out,std::string& e) override {
        if(!native_.advance(d,i,motion,r,out,e))return false;
        last=out;
        if(corrupt) {
            for(auto& scan:out.pair_scan_evidence)
                if(scan.randomized_recovery) {
                    ++scan.randomized_recovery->rng_state_after;
                    break;
                }
        }
        return true;
    }
    bool unload(std::string& e) override {return native_.unload(e);}
    bool corrupt{};
    fg::HerdingObservedFrame last{};
private:
    fg::NativePicklesMotionSource native_;
};

void exercise_recovery(const h::Data& data) {
    btb::retail::OriginalRetailRandom rng{1};
    auto native=std::make_unique<CrowdedSource>(rng);
    auto* probe=native.get();
    fg::HerdingEventSimulation sim(std::move(native));
    std::string error;
    assert(sim.initialize(data,0,error));
    fg::HerdingScene scene{};
    int remaining=-1;
    std::vector<fg::Audio> sounds;
    const auto before=rng.state();
    probe->corrupt=true;
    assert(!sim.advance({}, {},scene,remaining,sounds,error));
    assert(!error.empty());
    assert(rng.state()==before);
    assert(scene.entities.empty() && remaining==-1 && sounds.empty());
    assert(std::any_of(probe->last.pair_scan_evidence.begin(),
                       probe->last.pair_scan_evidence.end(),
                       [](const auto& s){return s.entity_index==1 && s.randomized_recovery.has_value();}));
    assert(std::any_of(probe->last.fixed_recovery_evidence.begin(),
                       probe->last.fixed_recovery_evidence.end(),
                       [](const auto& s){return s.entity_index==3 && s.attempts>0;}));
    probe->corrupt=false;
    require_frame(sim,{},scene,remaining,0,0);
    assert(rng.state()!=before);
    assert(sim.unload(error));
}

void exercise_map(const h::Data& data,int difficulty) {
    btb::retail::OriginalRetailRandom rng{static_cast<unsigned>(difficulty+1)};
    fg::HerdingEventSimulation sim(std::make_unique<fg::NativePicklesMotionSource>(rng));
    std::string error;
    assert(sim.initialize(data,difficulty,error));
    fg::HerdingScene scene{};
    int remaining=-1;
    fg::ActivityFrameInput input{};
    input.pointer_position_valid=true;
    input.pointer_x=320;input.pointer_y=240;
    require_frame(sim,input,scene,remaining,difficulty,0);
    assert(remaining==h::initial_undelivered_animal_count(difficulty));
    const auto initial=scene.entities.front();
    bool moved=false, food_seen=false, animal_moved=false;
    const auto first_animals=scene.entities;
    // Use actual pointer input and then switch to keyboard input, rather
    // than bypassing the live physical-control branch.
    for(int frame=1;frame<=60;++frame) {
        input.pointer_x=500;input.pointer_y=350;
        require_frame(sim,input,scene,remaining,difficulty,frame);
        moved|=scene.entities.front().x!=initial.x || scene.entities.front().y!=initial.y;
        food_seen|=scene.selected_food!=h::FoodType::None;
    }
    // Navigate via the real map, including several directions and
    // idle stretches. A blocked waypoint times out instead of teleporting.
    constexpr std::array<h::Vec2i,8> route{{
        {240,414},{600,550},{900,550},{900,350},
        {700,350},{365,354},{480,354},{242,232}
    }};
    std::size_t waypoint=0;
    int dwell=0;
    for(int frame=61;frame<=3000;++frame) {
        input.directional_input_bits=0;
        if(waypoint<route.size()) {
            const auto& hero=scene.entities.front();
            const auto target=route[waypoint];
            const auto dx=target.x-hero.x,dy=target.y-hero.y;
            if(dx>3)input.directional_input_bits|=2;
            if(dx< -3)input.directional_input_bits|=1;
            if(dy>3)input.directional_input_bits|=8;
            if(dy< -3)input.directional_input_bits|=4;
            if((std::abs(dx)<=3 && std::abs(dy)<=3) || ++dwell>=360) {
                ++waypoint;dwell=0;
            }
        }
        const auto old_remaining=remaining;
        require_frame(sim,input,scene,remaining,difficulty,frame);
        assert(remaining<=old_remaining);
        moved|=scene.entities.front().x!=initial.x || scene.entities.front().y!=initial.y;
        food_seen|=scene.selected_food!=h::FoodType::None;
        for(std::size_t j=1;j<scene.entities.size();++j) {
            if(scene.entities[j].entity_type()==h::EntityType::Sheep)
                animal_moved|=scene.entities[j].x!=first_animals[j].x ||
                    scene.entities[j].y!=first_animals[j].y;
        }
    }
    assert(moved && animal_moved && food_seen);
    std::printf("retail map: difficulty %d, 3001 frames, %d animals still outside home\n",
                difficulty,remaining);
    assert(sim.unload(error));
}
}

int main() {
    const auto data=retail_geometry_fixture();
    assert(data.coordinate_groups.size()==4);
    assert(data.coordinate_groups[0].size()==12);
    assert(data.coordinate_groups[1].size()==5);
    assert(data.coordinate_groups[2].size()==6);
    assert(data.coordinate_groups[3].size()==1);
    exercise_recovery(data);
    for(int difficulty=0;difficulty<3;++difficulty)exercise_map(data,difficulty);
}
