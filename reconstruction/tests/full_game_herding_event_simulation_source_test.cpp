#include "btb/full_game_herding_event_simulation.hpp"

#include <cassert>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace {
namespace h=btb::herding;
namespace r=btb::retail;
using namespace btb::full_game;

std::vector<h::RetailEntityRecord32> population(int difficulty) {
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
        record.source_left=0;record.source_top=0;
        record.source_right=40;record.source_bottom=40;
        records.push_back(record);
    };
    add(h::EntityType::FarmerPickles,240,414);
    for (int i=0;i<difficulty+3;++i) add(h::EntityType::Sheep,520+i*10,300);
    for (int i=0;i<difficulty+3;++i) add(h::EntityType::Rabbit,620+i*10,300);
    for (int i=0;i<difficulty+3;++i) add(h::EntityType::Duck,720+i*10,300);
    add(h::EntityType::GateLeft,717,162);
    add(h::EntityType::GateRight,859,101);
    if (difficulty>0) {
        // Medium/Hard retail instantiates a Scruffty entity. These
        // coordinates are explicit test evidence, NOT retail spawns.
        add(h::EntityType::Scruffty,545,315);
    }
    return records;
}

class EvidenceSource final : public OriginalHerdingMotionSource {
public:
    std::vector<h::RetailEntityRecord32> originals{};
    int source_difficulty{};
    std::vector<HerdingObservedEvent> scheduled{};
    std::optional<h::Vec2i> forced_sheep_position{};
    bool fail_advance{};
    bool emit_navigation_probe{};
    bool contradict_navigation_probe{};
    bool emit_steering_probe{};
    bool contradict_steering_probe{};
    bool contradict_heading_probe{};
    bool emit_recovery_probe{};
    bool contradict_recovery_probe{};
    bool started{};
    int steps{};
    int shutdowns{};
    h::PicklesKeyboardMotion last_motion{};

    explicit EvidenceSource(int difficulty=0)
        : originals(population(difficulty)),source_difficulty(difficulty) {}

    bool initialize(const h::Data& data,int difficulty,
        std::vector<h::RetailEntityRecord32>& out,
        std::string& error) override {
        if (difficulty!=source_difficulty || data.coordinate_groups.size()!=4) {
            error="unexpected original herd source";
            return false;
        }
        out=originals;
        started=true;
        error.clear();
        return true;
    }

    bool advance(const h::Data& data,const ActivityFrameInput&,
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
        if (forced_sheep_position) {
            auto& animal=frame.motion_records[1];
            animal.x=forced_sheep_position->x;
            animal.y=forced_sheep_position->y;
            animal.x_float=static_cast<float>(animal.x);
            animal.y_float=static_cast<float>(animal.y);
        }
        frame.events=std::exchange(
            scheduled,std::vector<HerdingObservedEvent>{});
        if (emit_steering_probe) {
            const auto original=current[1];
            const h::Vec2i original_target{
                original.x,original.y-100
            };
            const auto source_heading=h::original_herding_integer_heading(
                original.x,original.y,
                original_target.x,original_target.y);
            assert(source_heading==359);
            const auto predicted=h::original_herding_steering_step(
                original.x_float,original.y_float,
                *source_heading,0.5f,true);
            assert(predicted);
            auto& result=frame.motion_records[1];
            result.x_float=predicted->x;
            result.y_float=predicted->y;
            result.x=predicted->rounded_x;
            result.y=predicted->rounded_y;
            result.direction=predicted->facing_index;
            result.movement_speed=predicted->next_speed;
            if (contradict_steering_probe) result.y_float += 3.0f;
            frame.steering_evidence.push_back(HerdingSteeringEvidence{
                .entity_index=1,
                .original_x=original.x_float,
                .original_y=original.y_float,
                .native_heading_degrees=contradict_heading_probe ?
                    90 : *source_heading,
                .original_target=original_target,
                .original_actor_x=original.x,
                .original_actor_y=original.y,
                .magnitude_before_step=0.5f,
                .native_roaming_speed_ramp=true
            });
        }
        if (emit_recovery_probe) {
            auto before=current[1];
            before.x=520;
            before.y=801; // below the source world polygon's y=800 edge
            before.x_float=520.0f;
            before.y_float=801.0f;
            r::OriginalRetailRandom exact_rng{1};
            const auto initial_rng=exact_rng.state();
            h::OriginalRoamingLoop replay{.entity=before};
            const auto result=h::resume_original_herding_roaming_loop(
                replay,exact_rng,
                data.retail_transformed_group0(),8);
            assert(result==h::OriginalRoamingLoopStatus::Accepted);
            assert(replay.attempts==1);
            frame.motion_records[1]=replay.entity;
            if (contradict_recovery_probe) {
                frame.motion_records[1].y_float+=1.0f;
            }
            frame.recovery_evidence.push_back({
                1,before,initial_rng,exact_rng.state(),replay.attempts
            });
        }
        if (emit_navigation_probe) {
            frame.pickles_boundary_evidence =
                HerdingPicklesBoundaryEvidence{240.0f,414.0f,240,414,0,1};
            if (contradict_navigation_probe) {
                frame.motion_records[0].x_float=-99.0f;
            }
        }
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
        {{44,99},{202,99},{353,279},{461,263},
         {530,299},{966,181},{966,166},{966,235},
         {1086,349},{1257,405},{1257,900},{44,900}},
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

    // Native dog/animal collision is not symmetric AABB overlap.
    // Even an externally asserted event must contain 0x4169A0 ordered
    // rectangle evidence before the follower may be distracted.
    source->scheduled={event(HerdingObservedKind::ScrufftyCollision,1)};
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(error.find("four-corner")!=std::string::npos);
    assert(remaining==9);
    assert(scene.entities[1].behavior_state==0);

    auto wrong_contact=event(HerdingObservedKind::ScrufftyCollision,1);
    wrong_contact.retail_rect_contact=HerdingCollisionEvidence{
        {0,0,10,10},{4,4,6,6}
    }; // nested rectangle; FIRST corners are not inside SECOND
    source->scheduled={wrong_contact};
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(error.find("four-corner")!=std::string::npos);
    assert(remaining==9);

    // The 0x416C45 steering branch is checked independently, including
    // 90-degree-relative facing, movement speed ramp, and integer position.
    source->emit_steering_probe=true;
    sounds.clear();
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(scene.entities[1].direction==0);
    assert(scene.entities[1].movement_speed>0.5f);
    const auto previous_steering_y=scene.entities[1].y_float;

    source->contradict_steering_probe=true;
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(error.find("native steering kernel")!=std::string::npos);
    assert(scene.entities[1].y_float==previous_steering_y);
    assert(remaining==9);
    source->contradict_steering_probe=false;

    source->contradict_heading_probe=true;
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(error.find("0x415D70")!=std::string::npos);
    assert(scene.entities[1].y_float==previous_steering_y);
    assert(remaining==9);
    source->contradict_heading_probe=false;
    source->emit_steering_probe=false;

    // Source's same-frame out-of-bounds recovery must preserve both
    // the original global LCG state and the observed retry count. One
    // candidate starting just beyond herd.txt polygon Y=800 returns
    // inside after exactly one pair of original random rolls.
    source->emit_recovery_probe=true;
    const auto before_recovery=scene.entities[1].y_float;
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(scene.entities[1].y==798);
    assert(scene.entities[1].previous_x==520);
    assert(scene.entities[1].previous_y==801);
    assert(scene.entities[1].y_float>798.0f);
    assert(scene.entities[1].y_float<800.0f);
    assert(remaining==9);
    assert(scene.entities[1].y_float!=before_recovery);

    source->contradict_recovery_probe=true;
    const auto accepted_y=scene.entities[1].y_float;
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(error.find("same-frame")!=std::string::npos);
    assert(scene.entities[1].y_float==accepted_y);
    assert(remaining==9);
    source->contradict_recovery_probe=false;
    source->emit_recovery_probe=false;

    // Original rendering writes animation frame and countdown fields into
    // the same entity array as gameplay. The next simulation frame must
    // preserve these mutated values instead of restoring stale copies.
    scene.entities[1].animation_frame=7;
    scene.entities[1].animation_frame_countdown=73;

    auto home=event(HerdingObservedKind::EnterHomeRoute,1);
    auto waypoint1=event(HerdingObservedKind::ReachHomeWaypoint,1);
    auto waypoint2=event(HerdingObservedKind::ReachHomeWaypoint,1);
    source->scheduled={home};
    sounds.clear();
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(remaining==9);
    assert(scene.entities[1].behavior_state==10);

    // An arrival event from the old animal coordinates cannot skip
    // the retail 10-unit distance and prematurely deliver the animal.
    source->scheduled={waypoint1};
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(remaining==9);
    assert(scene.entities[1].behavior_state==10);

    source->forced_sheep_position=h::home_entrance_target(
        h::EntityType::Sheep);
    source->scheduled={waypoint1};
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(remaining==9);
    assert(scene.entities[1].behavior_state==20);

    source->forced_sheep_position=*h::home_entry_target(
        h::EntityType::Sheep,20);
    source->scheduled={waypoint2};
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    source->forced_sheep_position.reset();
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

    // The exact shared x86 integer polygon routine and the retail
    // axis-slide decision are independently compared to the upstream
    // movement coordinates before any event changes are committed.
    source->emit_navigation_probe=true;
    assert(simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(remaining==8);
    source->contradict_navigation_probe=true;
    assert(!simulation.advance(
        {},h::pickles_keyboard_motion(0),scene,remaining,sounds,error));
    assert(error.find("binary-verified")!=std::string::npos);
    assert(remaining==8);
    source->contradict_navigation_probe=false;

    assert(simulation.unload(error));
    assert(!simulation.initialized());
    assert(source->shutdowns==1);

    // Medium difficulty includes Scruffty; the adapter derives the
    // original animal-FIRST, dog-SECOND ordered rectangles directly
    // from the source 0x64-byte entity records, then removes a follower.
    {
        auto dog_source=std::make_unique<EvidenceSource>(1);
        auto* dog=dog_source.get();
        HerdingEventSimulation with_dog(std::move(dog_source));
        assert(with_dog.initialize(original_shape_data(),1,error));
        HerdingScene dog_scene{};
        int dog_remaining=-1;
        std::vector<Audio> dog_audio{};
        auto select=event(HerdingObservedKind::PickUpFood);
        select.food=h::FoodType::SheepFood;
        select.random_mod_2=1;
        select.follower_release_random.assign(
            dog->originals.size(),{25,40});
        auto follow=event(HerdingObservedKind::JoinFollower,1);
        follow.random_mod_2=0;
        dog->scheduled={select,follow};
        assert(with_dog.advance(
            {},h::pickles_keyboard_motion(0),
            dog_scene,dog_remaining,dog_audio,error));
        assert(dog_remaining==12);

        auto contact=event(HerdingObservedKind::ScrufftyCollision,1);
        contact.random_mod_400=3;
        // Sheep [520,300]-[560,340] corner (560,340) is inside
        // Scruffty [545,315]-[585,355]. Reversed order is not
        // interchangeable, and no heuristic AABB shortcut is used.
        dog->scheduled={contact};
        dog_audio.clear();
        assert(with_dog.advance(
            {},h::pickles_keyboard_motion(0),
            dog_scene,dog_remaining,dog_audio,error));
        assert(dog_remaining==12);
        assert(dog_scene.entities[1].temporary_target_timer==200);
        assert(dog_scene.entities[1].target_x==289);
        assert(with_dog.unload(error));
    }

    HerdingEventSimulation no_motion(nullptr);
    assert(!no_motion.initialize(original_shape_data(),0,error));
    assert(!no_motion.initialized());
}
