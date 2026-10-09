#include "btb/full_game_herding_event_simulation.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {
namespace h=btb::herding;
using namespace btb::full_game;

std::vector<h::RetailEntityRecord32> population() {
    std::vector<h::RetailEntityRecord32> records;
    const auto add=[&](h::EntityType type,int x,int y) {
        h::RetailEntityRecord32 record{};
        record.entity_id=static_cast<int>(records.size());
        record.type=static_cast<int>(type);
        record.x=x;record.y=y;
        record.x_float=static_cast<float>(x);
        record.y_float=static_cast<float>(y);
        record.direction=2;
        record.animation_frame_countdown=100;
        records.push_back(record);
    };
    add(h::EntityType::FarmerPickles,240,414);
    for (int i=0;i<3;++i) add(h::EntityType::Sheep,520+i*10,300);
    for (int i=0;i<3;++i) add(h::EntityType::Rabbit,620+i*10,300);
    for (int i=0;i<3;++i) add(h::EntityType::Duck,720+i*10,300);
    add(h::EntityType::GateLeft,717,162);
    add(h::EntityType::GateRight,859,101);
    return records;
}

class EvidenceSource final : public OriginalHerdingMotionSource {
public:
    std::vector<herding::RetailEntityRecord32> originals{};
    std::vector<HerdingObservedEvent> scheduled{};
    bool fail_advance{};
    bool started{};
    int steps{};
    int shutdowns{};
    h::PicklesKeyboardMotion last_motion{};

    EvidenceSource() : originals(population()) {}

    bool initialize(const h::Data& data,int difficulty,
        std::vector<h::RetailEntityRecord32>& out,
        std::string& error) override {
        if (difficulty!=0 || data.coordinate_groups.size()!=4) {
            error="unexpected original herd source";
            return false;
        }
        out=originals;
        started=true;
        error.clear();
        return true;
    }

    bool advance(const h::Data&,const ActivityFrameInput&,
        const h::PicklesKeyboardMotion& motion,
        const std::vector<h::RetailEntityRecord32>& current,
        HerdingObservedFrame& frame,std::string& error) override {
        if (fail_advance) {
            error="original movement source unavailable";
            return false;
        }
        ++steps;
        last_motion=motion;
        frame.motion_records=current;
        frame.events=std::exchange(
            scheduled,std::vector<HerdingObservedEvent>{});
        error.clear();
        return true;
    }

    bool unload(std::string& error) override {
        ++shutdowns;
        started=false;
        error.clear();
        return true;
    }
};

h::Data original_shape_data() {
    h::Data data;
    data.setup_positions[3]={478,471};
    data.coordinate_groups={
        std::vector<h::Vec2i>(12),
        std::vector<h::Vec2i>(5),
        std::vector<h::Vec2i>(6),
        std::vector<h::Vec2i>(1)};
    return data;
}

HerdingObservedEvent event(
    HerdingObservedKind kind,std::size_t index=0) {
    HerdingObservedEvent result{};
    result.kind=kind;
    result.entity_index=index;
    result.retail_trigger_confirmed=true;
    return result;
}
}

int main() {
    using namespace btb::full_game;
    auto evidence=std::make_unique<EvidenceSource>();
    auto* source=evidence.get();
    HerdingEventSimulation simulation(std::move(evidence));
    std::string error;
    assert(simulation.initialize(original_shape_data(),0,error));
    assert(simulation.initialized());
    assert(source->started);

    HerdingScene scene{};
    int remaining=-1;
    std::vector<Audio> sounds{};

    auto pick_food=event(HerdingObservedKind::PickUpFood);
    pick_food.food=h::FoodType::SheepFood;
    pick_food.random_mod_2=1;
    pick_food.follower_release_random.assign(12,{25,40});
    auto join=event(HerdingObservedKind::JoinFollower,1);
    join.random_mod_2=0;
    source->scheduled={pick_food,join};
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0x06),scene,remaining,sounds,error));
    assert(remaining==9);
    assert(source->last_motion.delta_x==1.5f);
    assert(source->last_motion.delta_y==-1.5f);
    assert(scene.entities.size()==12);
    assert(scene.selected_food==h::FoodType::SheepFood);
    assert(!scene.visible_world_bags[2]);
    assert(scene.visible_world_bags[0] && scene.visible_world_bags[1]);
    assert(sounds.size()==2);
    assert(sounds[0].sound_id==584);
    assert(sounds[1].sound_id==589);

    // Original rendering writes animation frame and countdown fields into
    // the same entity array as gameplay. The next simulation frame must
    // preserve these mutated values instead of restoring stale copies.
    scene.entities[1].animation_frame=7;
    scene.entities[1].animation_frame_countdown=73;

    auto home=event(HerdingObservedKind::EnterHomeRoute,1);
    auto waypoint1=event(HerdingObservedKind::ReachHomeWaypoint,1);
    auto waypoint2=event(HerdingObservedKind::ReachHomeWaypoint,1);
    source->scheduled={home,waypoint1,waypoint2};
    sounds.clear();
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(remaining==8);
    assert(scene.entities[1].behavior_state==99);
    assert(scene.entities[1].animation_frame==7);
    assert(scene.entities[1].animation_frame_countdown==73);
    assert(sounds.empty()); // first sheep has no final-species voice

    // A duplicated arrival is not accepted and must not decrement the
    // count twice, alter the last valid scene or emit sound effects.
    source->scheduled={
        event(HerdingObservedKind::ReachHomeWaypoint,1)
    };
    const auto old_scene=scene.entities;
    const auto old_remaining=remaining;
    sounds.clear();
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(!error.empty());
    assert(remaining==old_remaining);
    assert(scene.entities[1].behavior_state==99);
    assert(scene.entities[1].animation_frame==7);
    assert(sounds.empty());

    // The next valid frame succeeds; rejected frames never commit
    // copied follower/AI counters into the active behavior state.
    source->scheduled={};
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(remaining==8);
    assert(error.empty());

    // Missing direct movement equations never default to invented AI:
    // a provider failure blocks the frame, but leaves earlier committed
    // source state untouched.
    source->fail_advance=true;
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(error=="original movement source unavailable");
    assert(remaining==8);

    source->fail_advance=false;
    assert(simulation.unload(error));
    assert(!simulation.initialized());
    assert(source->shutdowns==1);

    HerdingEventSimulation no_motion(nullptr);
    assert(!no_motion.initialize(original_shape_data(),0,error));
    assert(!no_motion.initialized());
}
