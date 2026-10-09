#include "btb/full_game_herding_event_simulation.hpp"

#include <algorithm>
#include <cmath>
#include <iterator>
#include <utility>

namespace btb::full_game {
namespace {
constexpr int kOriginalManagedSpeechPriority = 50;
constexpr int kOriginalManagedSpeechClass = 1;

std::vector<Audio> speech_for_id(int sound_id) {
    if (sound_id < 0) return {};
    return {{
        AudioOperation::ManagedSoundId,{},
        sound_id,kOriginalManagedSpeechPriority,kOriginalManagedSpeechClass
    }};
}

bool apply_confirmed_event(
    herding::HerdingRecoveredBehavior& staged,
    const HerdingObservedEvent& event,
    std::vector<Audio>& sounds,
    std::string& error) {

    if (!event.retail_trigger_confirmed) {
        error="Herding source event has no original collision/trigger evidence";
        return false;
    }
    herding::HerdingBehaviorEvent step{};
    switch (event.kind) {
    case HerdingObservedKind::PickUpFood: {
        const auto player=std::find_if(
            staged.entities().begin(),staged.entities().end(),
            [](const auto& entity) {
                return entity.entity_type() ==
                    herding::EntityType::FarmerPickles;
            });
        if (player==staged.entities().end()) {
            error="Herding food event missing original Farmer Pickles";
            return false;
        }
        step=staged.select_food(
            event.food,player->x,player->y,event.random_mod_2,
            event.follower_release_random);
        break;
    }
    case HerdingObservedKind::JoinFollower:
        step=staged.join_follower(
            event.entity_index,event.random_mod_2,true);
        break;
    case HerdingObservedKind::ScrufftyCollision: {
        const auto& records=staged.entities();
        if (event.entity_index>=records.size()) {
            error="Scruffty collision refers to unknown original animal";
            return false;
        }
        const auto dog=std::find_if(
            records.begin(),records.end(),[](const auto& entity) {
                return entity.entity_type()==herding::EntityType::Scruffty;
            });
        if (dog==records.end() ||
            !herding::original_herding_animal_hits_scruffty(
                records[event.entity_index],*dog)) {
            error="Scruffty collision was not established by original "
                  "animal-first four-corner contact";
            return false;
        }
        if (event.retail_rect_contact &&
            (event.retail_rect_contact->first !=
                herding::original_herding_entity_rect(
                    records[event.entity_index]) ||
             event.retail_rect_contact->second !=
                herding::original_herding_entity_rect(*dog))) {
            error="Scruffty source rectangles differ from original "
                  "animal-first four-corner contact";
            return false;
        }
        step=staged.scruffty_distraction(
            event.entity_index,event.random_mod_400,true);
        break;
    }
    case HerdingObservedKind::TrackedTargetArrived:
        // Retail 0x417314 writes behavior_state=1 and returns from
        // this entity update. The route counter/state 10+N allocation
        // happens on a LATER UpdateHerdingAnimal dispatch.
        if (!staged.confirm_home_route_trigger(event.entity_index,true)) {
            error="source tracked arrival requires an ordinary follower "
                  "in state zero";
            return false;
        }
        step.accepted=true;
        break;
    case HerdingObservedKind::EnterHomeRoute:
        // State one already came from the previous tracked arrival;
        // never combine both retail control-flow steps in one tick.
        step=staged.begin_home_route(event.entity_index);
        break;
    case HerdingObservedKind::ReachHomeWaypoint:
        step=staged.arrive_at_home_waypoint(event.entity_index,true);
        break;
    }
    if (!step.accepted) {
        error="Herding source event conflicts with retail behavior state";
        return false;
    }

    for (auto& sound : speech_for_id(step.sound_id)) {
        sounds.push_back(std::move(sound));
    }
    // Scruffty's randomized reaction has no fixed ID. The exact source
    // event may provide it after a verified native sound branch.
    if (event.kind==HerdingObservedKind::ScrufftyCollision &&
        event.source_verified_sound_id>=0) {
        for (auto& sound : speech_for_id(event.source_verified_sound_id)) {
            sounds.push_back(std::move(sound));
        }
    }
    return true;
}
} // namespace

bool HerdingEventSimulation::initialize(
    const herding::Data& original_data,
    int difficulty,
    std::string& error) {

    if (state_) {
        error="original Herding event simulation is already initialized";
        return false;
    }
    if (!motion_) {
        error="missing continuous-motion/collision implementation "
              "for original Herding entities";
        return false;
    }

    std::vector<herding::RetailEntityRecord32> original_entities;
    if (!motion_->initialize(
            original_data,difficulty,original_entities,error)) {
        return false;
    }
    auto candidate=std::make_unique<herding::HerdingRecoveredBehavior>(
        difficulty,std::move(original_entities));
    if (!candidate->valid()) {
        error="original movement source supplied an invalid "
              "Easy/Medium/Hard Herding entity population";
        std::string cleanup_error;
        static_cast<void>(motion_->unload(cleanup_error));
        return false;
    }
    data_=original_data;
    state_=std::move(candidate);
    error.clear();
    return true;
}

bool HerdingEventSimulation::advance(
    const ActivityFrameInput& input,
    const herding::PicklesKeyboardMotion& normalized_motion,
    HerdingScene& out,
    int& undelivered_animals,
    std::vector<Audio>& source_audio_events,
    std::string& error) {

    if (!state_ || !motion_) {
        error="original Herding simulation frame requested before init";
        return false;
    }

    // Copy-on-frame is deliberate: malformed/duplicated source events
    // cannot partially mutate followers, RNG, route counters or progress.
    auto candidate=*state_;
    if (!out.entities.empty() &&
        !candidate.synchronize_render_counters(out.entities)) {
        error="rendered Herding entity identity/count changed unexpectedly";
        return false;
    }
    HerdingObservedFrame observed{};
    if (!motion_->advance(
            data_,input,normalized_motion,candidate.entities(),
            observed,error)) {
        return false;
    }
    if (observed.motion_records.size()!=candidate.entities().size()) {
        error="Herding motion source must return every retail entity record";
        return false;
    }
    if (observed.pickles_boundary_evidence) {
        const auto& observed_motion=*observed.pickles_boundary_evidence;
        const auto expected=herding::retail_herding_navigation_boundary_step(
            data_.retail_transformed_group0(),
            observed_motion.attempted_x,observed_motion.attempted_y,
            observed_motion.previous_x,observed_motion.previous_y,
            herding::retail_herding_axis_recovery_enabled(
                observed_motion.active_directional_axes,
                observed_motion.source_mouse_navigation_mode_443a9c));
        if (!expected) {
            error="source's original Pickles polygon movement probe "
                  "could not be evaluated without x86 division failure";
            return false;
        }
        const auto hero=std::find_if(
            observed.motion_records.begin(),
            observed.motion_records.end(),
            [](const auto& e) {
                return e.entity_type()==herding::EntityType::FarmerPickles;
            });
        if (hero==observed.motion_records.end() ||
            hero->x_float!=expected->x ||
            hero->y_float!=expected->y) {
            error="original Pickles navigation differs from binary-verified "
                  "0x418C2D polygon and axis-slide result";
            return false;
        }
    }
    for (const auto& probe : observed.temporary_target_evidence) {
        if (probe.entity_index>=observed.motion_records.size() ||
            probe.before.entity_id !=
                observed.motion_records[probe.entity_index].entity_id ||
            probe.before.type !=
                observed.motion_records[probe.entity_index].type ||
            probe.before.temporary_target_timer<=0) {
            error="invalid original positive temporary-target branch evidence";
            return false;
        }
        const auto expected=
            herding::original_herding_temporary_target_update(probe.before);
        if (!expected || !expected->branch_taken ||
            expected->early_return!=probe.source_returned_early) {
            error="original temporary-target branch selection or "
                  "120-unit early return differs from retail";
            return false;
        }
        const auto& moved=observed.motion_records[probe.entity_index];
        // Exact per-axis FSTP values may differ by the original x87
        // extended precision. The source-equivalent direction, timer
        // transition and integer positions must still agree.
        constexpr float kOriginalTrigTolerance=0.001f;
        if (!std::isfinite(moved.x_float) ||
            !std::isfinite(moved.y_float) ||
            std::fabs(moved.x_float-expected->moved.x_float)>
                kOriginalTrigTolerance ||
            std::fabs(moved.y_float-expected->moved.y_float)>
                kOriginalTrigTolerance ||
            moved.x!=expected->moved.x ||
            moved.y!=expected->moved.y ||
            moved.previous_x!=expected->moved.previous_x ||
            moved.previous_y!=expected->moved.previous_y ||
            moved.direction!=expected->moved.direction ||
            moved.temporary_target_timer!=
                expected->moved.temporary_target_timer ||
            moved.movement_speed!=expected->moved.movement_speed) {
            error="original 0x416F57 temporary target movement differs "
                  "from source 3-unit steering and 120-unit gate";
            return false;
        }
    }
    for (const auto& probe : observed.recovery_evidence) {
        // A reported recovery needs a completed same-frame retry
        // sequence. This is a validation budget, not a limit imposed
        // by the retail executable on how long it can retry.
        constexpr std::uint64_t kMaxVerificationAttempts=4096;
        if (probe.entity_index>=observed.motion_records.size() ||
            probe.attempts==0 ||
            probe.attempts>kMaxVerificationAttempts ||
            probe.before.entity_id !=
                observed.motion_records[probe.entity_index].entity_id ||
            probe.before.type !=
                observed.motion_records[probe.entity_index].type) {
            error="invalid original roam recovery evidence metadata";
            return false;
        }
        herding::OriginalRoamingLoop reconstructed{
            .entity=probe.before
        };
        retail::OriginalRetailRandom replay_rng{
            probe.rng_state_before};
        const auto result=herding::resume_original_herding_roaming_loop(
            reconstructed,replay_rng,
            data_.retail_transformed_group0(),
            static_cast<std::size_t>(probe.attempts));
        const auto& moved=observed.motion_records[probe.entity_index];
        // Portable trigonometry has not been shown to match x87's
        // 80-bit FSIN/FCOS bit-for-bit. Use the same explicit tolerance
        // as other original steering probes, never assume equality.
        constexpr float kOriginalTrigComparisonTolerance=0.001f;
        if (result!=herding::OriginalRoamingLoopStatus::Accepted ||
            reconstructed.attempts!=probe.attempts ||
            reconstructed.consumed_random_calls!=2*probe.attempts ||
            replay_rng.state()!=probe.rng_state_after ||
            moved.x!=reconstructed.entity.x ||
            moved.y!=reconstructed.entity.y ||
            moved.previous_x!=reconstructed.entity.previous_x ||
            moved.previous_y!=reconstructed.entity.previous_y ||
            moved.direction!=reconstructed.entity.direction ||
            moved.movement_speed!=reconstructed.entity.movement_speed ||
            !std::isfinite(moved.x_float) ||
            !std::isfinite(moved.y_float) ||
            std::fabs(moved.x_float-reconstructed.entity.x_float)>
                kOriginalTrigComparisonTolerance ||
            std::fabs(moved.y_float-reconstructed.entity.y_float)>
                kOriginalTrigComparisonTolerance) {
            error="original roam recovery differs from x86 same-frame "
                  "retry / sprite-anchor / shared rand sequence";
            return false;
        }
    }
    for (const auto& probe : observed.steering_evidence) {
        if (probe.entity_index>=observed.motion_records.size()) {
            error="source steering probe references an absent entity";
            return false;
        }
        if (probe.original_target) {
            const auto recovered_heading=
                herding::original_herding_integer_heading(
                    probe.original_actor_x,probe.original_actor_y,
                    probe.original_target->x,probe.original_target->y);
            if (!recovered_heading ||
                *recovered_heading!=probe.native_heading_degrees) {
                error="source steering heading contradicts retail "
                      "0x415D70 target/quadrant/truncation rules";
                return false;
            }
        }
        const auto expected=herding::original_herding_steering_step(
            probe.original_x,probe.original_y,
            probe.native_heading_degrees,probe.magnitude_before_step,
            probe.native_roaming_speed_ramp);
        if (!expected) {
            error="original animal steering probe has invalid input";
            return false;
        }
        const auto& moved=observed.motion_records[probe.entity_index];
        // The original executes x87 FSIN/FCOS, while this source helper
        // uses std::sin/std::cos; tolerate less than 1/1000 world unit
        // here until an x87-matching differential backend is available.
        constexpr float kTrigTolerance=0.001f;
        if (!std::isfinite(moved.x_float) ||
            !std::isfinite(moved.y_float) ||
            std::fabs(moved.x_float-expected->x)>kTrigTolerance ||
            std::fabs(moved.y_float-expected->y)>kTrigTolerance ||
            moved.x!=expected->rounded_x ||
            moved.y!=expected->rounded_y ||
            moved.direction!=expected->facing_index ||
            std::fabs(moved.movement_speed-expected->next_speed)>
                kTrigTolerance) {
            error="source animal motion contradicts original "
                  "0x416C45/0x416DCE native steering kernel";
            return false;
        }
    }
    for (std::size_t i=0;i<observed.motion_records.size();++i) {
        if (!candidate.apply_source_motion(i,observed.motion_records[i])) {
            error="Herding movement source attempted to replace entity "
                  "identity or supplied invalid source coordinates";
            return false;
        }
    }
    std::vector<Audio> sounds{};
    std::vector<bool> arrived_at_tracked_point_this_update(
        candidate.entities().size(),false);
    for (const auto& event : observed.events) {
        if (event.entity_index>=candidate.entities().size()) {
            error="original Herding event references absent entity index";
            return false;
        }
        if (event.kind==HerdingObservedKind::EnterHomeRoute &&
            arrived_at_tracked_point_this_update[event.entity_index]) {
            error="retail tracked-target arrival returns immediately; "
                  "home-route allocation cannot occur in that same "
                  "animal update";
            return false;
        }
        if (!apply_confirmed_event(candidate,event,sounds,error)) {
            return false;
        }
        if (event.kind==HerdingObservedKind::TrackedTargetArrived) {
            arrived_at_tracked_point_this_update[event.entity_index]=true;
        }
    }

    HerdingScene presented{};
    presented.entities=candidate.entities();
    presented.selected_food=candidate.food().selected;
    for (std::size_t i=0;i<3;++i) {
        presented.visible_world_bags[i] =
            candidate.food().bag_states[i]!=-1;
    }
    out=std::move(presented);
    undelivered_animals=candidate.undelivered();
    source_audio_events.insert(
        source_audio_events.end(),
        std::make_move_iterator(sounds.begin()),
        std::make_move_iterator(sounds.end()));
    *state_=std::move(candidate);
    error.clear();
    return true;
}

bool HerdingEventSimulation::unload(std::string& error) {
    if (state_ && motion_ && !motion_->unload(error)) {
        return false;
    }
    state_.reset();
    data_={};
    error.clear();
    return true;
}

} // namespace btb::full_game
