#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_source_attraction.hpp"

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
    std::string& error,
    bool independent_tracked_membership=false,
    const herding::RetailEntityRecord32* pre_patrol_dog=nullptr,
    const herding::RetailEntityRecord32* pre_outer_animal=nullptr,
    bool verified_pre_move_arrival=false) {

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
        const auto* collision_dog=pre_patrol_dog ? pre_patrol_dog :
            (dog==records.end()?nullptr:&*dog);
        const auto& collision_animal=pre_outer_animal?*pre_outer_animal:records[event.entity_index];
        if (!collision_dog ||
            !herding::original_herding_animal_hits_scruffty(
                collision_animal,*collision_dog)) {
            error="Scruffty collision was not established by original "
                  "animal-first four-corner contact";
            return false;
        }
        if (event.retail_rect_contact &&
            (event.retail_rect_contact->first !=
                herding::original_herding_entity_rect(collision_animal) ||
             event.retail_rect_contact->second !=
                herding::original_herding_entity_rect(*collision_dog))) {
            error="Scruffty source rectangles differ from original "
                  "animal-first four-corner contact";
            return false;
        }
        // The original reaction branch 0x417A52..0x417AF7 can
        // only emit one of these five sound IDs. A caller-supplied
        // unrelated ID must not bypass the recovered event contract.
        if(event.source_verified_sound_id>=0 &&
           event.source_verified_sound_id!=0x250 &&
           event.source_verified_sound_id!=0x251 &&
           event.source_verified_sound_id!=0x264 &&
           event.source_verified_sound_id!=0x266 &&
           event.source_verified_sound_id!=0x267) {
            error="Scruffty sound selection is outside retail reaction set";
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
        if (!staged.confirm_home_route_trigger(event.entity_index,true,
                                               independent_tracked_membership)) {
            error="source tracked arrival requires a registered animal in state zero";
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
        step=staged.arrive_at_home_waypoint(event.entity_index,true,verified_pre_move_arrival);
        break;
    case HerdingObservedKind::AnimalChatter:
        if(event.source_verified_sound_id<0x331 || event.source_verified_sound_id>0x339) {
            error="invalid native animal chatter ID";return false;
        }
        sounds.push_back({AudioOperation::ManagedSoundId,{},event.source_verified_sound_id,10,0});
        return true;
    }
    if (!step.accepted) {
        error="Herding source event conflicts with retail behavior state";
        return false;
    }

    for (auto& sound : speech_for_id(step.sound_id)) {
        if(event.kind==HerdingObservedKind::PickUpFood ||
           event.kind==HerdingObservedKind::EnterHomeRoute) sound.priority=90;
        if(event.kind==HerdingObservedKind::JoinFollower) sound.priority=10;
        sounds.push_back(std::move(sound));
    }
    // Scruffty's randomized reaction has no fixed ID. The exact source
    // event may provide it after a verified native sound branch.
    if (event.kind==HerdingObservedKind::ScrufftyCollision &&
        event.source_verified_sound_id>=0) {
        for (auto& sound : speech_for_id(event.source_verified_sound_id)) {
            sound.priority=90;
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
    const auto& records=candidate->entities();
    const auto dog=std::find_if(records.begin(),records.end(),
        [](const auto& e) {
            return e.entity_type()==herding::EntityType::Scruffty;
        });
    const auto dog_count=std::count_if(records.begin(),records.end(),
        [](const auto& e) {
            return e.entity_type()==herding::EntityType::Scruffty;
        });
    const auto& dog_waypoints=herding::scruffty_patrol_path(original_data);
    const bool has_dog=herding::scruffty_enabled(difficulty);
    bool dog_valid=dog_count==(has_dog ? 1 : 0);
    if (has_dog) {
        const auto expected=dog==records.end()
            ? std::optional<herding::RetailEntityRecord32>{}
            : herding::original_scruffty_initial_record(
                difficulty,dog->entity_id,dog_waypoints);
        dog_valid=dog_valid && dog_waypoints.size()==5 &&
            expected.has_value() &&
            dog->x==expected->x && dog->y==expected->y &&
            dog->x_float==expected->x_float &&
            dog->y_float==expected->y_float &&
            dog->source_left==expected->source_left &&
            dog->source_top==expected->source_top &&
            dog->source_right==expected->source_right &&
            dog->source_bottom==expected->source_bottom;
    }
    if (!dog_valid) {
        error="original Herding Medium/Hard Scruffty entity does not "
              "match five-point group-1 start, 101x116 sprite, or "
              "source difficulty-dependent population";
        std::string cleanup_error;
        static_cast<void>(motion_->unload(cleanup_error));
        return false;
    }
    data_=original_data;
    tracked_slots_=herding::RetailTrackedTargetSlots{};
    registration_counter_=0;
    // The original initializer resets 0x51076C to waypoint zero,
    // but does NOT reset 0x443B20, the process-global sprite latch.
    scruffty_patrol_state_.waypoint_index=0;
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
    if (!motion_->begin_frame(error)) return false;
    // The source is not allowed to commit its own tables or shared
    // random stream before the independent verifier has accepted
    // the full frame. All early returns (including source failure)
    // roll it back along with the staged game-state candidate.
    struct ProvisionalMotionFrame {
        OriginalHerdingMotionSource& motion;
        bool committed{};
        ~ProvisionalMotionFrame() {
            if(!committed) motion.rollback_frame();
        }
        void accept() noexcept {
            motion.commit_frame();
            committed=true;
        }
    } provisional{*motion_};
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
    std::vector<Audio> sounds{};
    auto staged_tracked=tracked_slots_;
    int staged_registration_counter=registration_counter_;
    bool food_applied=false;
    std::vector<std::optional<herding::Vec2i>> perception_anchors(candidate.entities().size());
    std::vector<std::optional<herding::Vec2i>> saved_positions(candidate.entities().size());
    std::vector<std::optional<herding::RetailEntityRecord32>> pre_outer_animals(candidate.entities().size());
    if(observed.source_ordered_food_and_tracking) {
        if(!observed.require_native_event_proofs ||
           !candidate.apply_source_motion(0,observed.motion_records.front())) {
            error="native ordered-food dispatch requires verified hero motion";
            return false;
        }
        // Food selection is before the entire animal loop in 004182B0.
        // Validate it on the staged copy before comparing motion proofs.
        for(const auto& event:observed.events) {
            if(event.kind!=HerdingObservedKind::PickUpFood)continue;
            if(food_applied || !apply_confirmed_event(candidate,event,sounds,error)) {
                if(error.empty())error="multiple native food changes in one frame";
                return false;
            }
            food_applied=true;
        }
        // Reconstruct the pair scan from already-validated food-release
        // state. Only higher-index actors receive changed prestates;
        // the current actor retains its own pre-update record for proofs.
        // No movement or membership is committed until the whole frame passes.
        if(data_.coordinate_groups.size()!=4 || data_.coordinate_groups[2].size()<3) {
            error="native ordered group-two polygon missing";return false;
        }
        auto pair_records=candidate.entities();
        for(auto& record:pair_records)record.movement_active=0;
        std::size_t fixed_cursor=0,pair_cursor=0;
        for(std::size_t i=1;i<pair_records.size();++i) {
            auto before=pair_records[i];
            const bool following=candidate.followers().contains(static_cast<int>(i));
            // Replay the original pre-dispatch recovery for every
            // eligible record, not only those the source chose to report.
            if(herding::is_herd_animal(before.entity_type()) &&
               before.behavior_state==0 && !following &&
               before.temporary_target_timer<=1) {
                before.previous_x=before.x;before.previous_y=before.y;
                if(before.temporary_target_timer==1)before.temporary_target_timer=0;
                herding::OriginalFixedHerdingRecovery fixed{.entity=before};
                const auto status=herding::resume_original_herding_fixed_recovery(
                    fixed,data_.coordinate_groups[2],4096);
                if(status!=herding::OriginalRoamingLoopStatus::AlreadyInside &&
                   status!=herding::OriginalRoamingLoopStatus::Accepted) {
                    error="native fixed recovery replay did not converge";return false;
                }
                if(fixed.attempts) {
                    if(fixed_cursor>=observed.fixed_recovery_evidence.size()) {
                        error="native fixed group-two recovery omitted";return false;
                    }
                    const auto& proof=observed.fixed_recovery_evidence[fixed_cursor++];
                    if(proof.entity_index!=i || proof.attempts!=fixed.attempts ||
                       proof.before.entity_id!=before.entity_id || proof.before.type!=before.type ||
                       proof.before.x!=before.x || proof.before.y!=before.y ||
                       proof.before.x_float!=before.x_float || proof.before.y_float!=before.y_float ||
                       proof.before.previous_x!=before.previous_x ||
                       proof.before.previous_y!=before.previous_y ||
                       proof.before.temporary_target_timer!=before.temporary_target_timer) {
                        error="native fixed group-two recovery evidence invalid";return false;
                    }
                    perception_anchors[i]=herding::original_herding_animal_tracking_anchor(before);
                    saved_positions[i]=herding::Vec2i{before.previous_x,before.previous_y};
                    before=fixed.entity;
                    if(!candidate.apply_source_motion(i,before)) {
                        error="native fixed recovery changed entity identity";return false;
                    }
                }
                pair_records[i]=before;
            }
            if(pair_cursor>=observed.pair_scan_evidence.size() ||
               observed.pair_scan_evidence[pair_cursor].entity_index!=i)continue;
            const auto& probe=observed.pair_scan_evidence[pair_cursor++];
            if(probe.separated_count==0 || following ||
               tracked_slots_.contains(static_cast<int>(i))) {
                error="native pair scan order or membership invalid";return false;
            }
            if(before.behavior_state!=0 || before.temporary_target_timer>0 ||
               before.movement_active!=0 ||
               before.entity_id!=probe.before.entity_id ||
               before.type!=probe.before.type ||
               before.x!=probe.before.x || before.y!=probe.before.y ||
               before.x_float!=probe.before.x_float ||
               before.y_float!=probe.before.y_float ||
               before.direction!=probe.before.direction ||
               before.movement_speed!=probe.before.movement_speed ||
               before.source_left!=probe.before.source_left ||
               before.source_right!=probe.before.source_right ||
               before.source_top!=probe.before.source_top ||
               before.source_bottom!=probe.before.source_bottom ||
               before.temporary_target_timer!=probe.before.temporary_target_timer) {
                error="native ordered pair scan prestate differs from source";return false;
            }
            const auto far=herding::original_far_untracked_animal_roam_step(
                before,observed.motion_records.front(),data_.retail_transformed_group0(),perception_anchors[i],saved_positions[i]);
            if(far.coverage==herding::RetailFarAnimalRoamCoverage::NotCovered ||
               far.coverage==herding::RetailFarAnimalRoamCoverage::InvalidInput) {
                error="native pair scan bypassed an original higher-priority branch";return false;
            }
            pair_records[i]=before;
            const auto count=herding::original_herding_pair_scan(pair_records,i,false);
            if(!count || *count!=probe.separated_count) {
                error="native pair scan count differs from retail";return false;
            }
            herding::OriginalRoamingLoop loop{.entity=pair_records[i]};
            const auto inside=retail_geometry::original_polygon_contains(
                data_.coordinate_groups[2],loop.entity.x,loop.entity.y);
            if(!inside || probe.randomized_recovery.has_value()==*inside) {
                error="native pair recovery contradicted group-two polygon";return false;
            }
            if(probe.randomized_recovery) {
                const auto& retry=*probe.randomized_recovery;
                if(retry.entity_index!=i || retry.attempts==0 || retry.attempts>4096 ||
                   retry.before.x_float!=loop.entity.x_float ||
                   retry.before.y_float!=loop.entity.y_float ||
                   retry.before.previous_x!=loop.entity.previous_x ||
                   retry.before.previous_y!=loop.entity.previous_y) {
                    error="native pair randomized recovery prestate invalid";return false;
                }
                retail::OriginalRetailRandom replay{retry.rng_state_before};
                const auto status=herding::resume_original_herding_roaming_loop(
                    loop,replay,data_.coordinate_groups[2],static_cast<std::size_t>(retry.attempts));
                if(status!=herding::OriginalRoamingLoopStatus::Accepted ||
                   loop.attempts!=retry.attempts ||
                   loop.consumed_random_calls!=2*retry.attempts ||
                   replay.state()!=retry.rng_state_after) {
                    error="native pair randomized recovery RNG differs";return false;
                }
            }
            const auto outer=herding::original_herding_nonfollowing_outer_step(
                before,loop.entity,data_.retail_transformed_group0(),
                herding::animal_navigation_recovery_target(data_),saved_positions[i]);
            const auto& actual=observed.motion_records[i];
            if(!outer || outer->boundary_recovered!=probe.outer_boundary_recovered ||
               actual.x!=outer->animal.x || actual.y!=outer->animal.y ||
               actual.direction!=outer->animal.direction ||
               actual.previous_x!=outer->animal.previous_x ||
               actual.previous_y!=outer->animal.previous_y ||
               actual.movement_speed!=outer->animal.movement_speed ||
               actual.movement_active!=outer->animal.movement_active ||
               !std::isfinite(actual.x_float)||!std::isfinite(actual.y_float)||
               std::fabs(actual.x_float-outer->animal.x_float)>0.001f||
               std::fabs(actual.y_float-outer->animal.y_float)>0.001f) {
                error="native pair scan final motion differs from retail";return false;
            }
            for(std::size_t j=i+1;j<pair_records.size();++j) {
                if(pair_records[j].movement_active &&
                   !candidate.apply_source_motion(j,pair_records[j])) {
                    error="native pair scan later entity prestate invalid";return false;
                }
            }
        }
        if(fixed_cursor!=observed.fixed_recovery_evidence.size() ||
           pair_cursor!=observed.pair_scan_evidence.size()) {
            error="native fixed/pair recovery evidence out of order";return false;
        }
        for(const auto& probe:observed.tracked_admission_evidence) {
            if(probe.entity_index==0 || probe.entity_index>=candidate.entities().size()) {
                error="invalid native tracked admission entity";
                return false;
            }
            const auto& prior=candidate.entities()[probe.entity_index];
            const auto target=herding::original_herding_species_registration_point(prior.entity_type());
            const auto anchor=herding::original_herding_animal_tracking_anchor(prior);
            const auto& actual=observed.motion_records[probe.entity_index];
            if(!target || !anchor || prior.behavior_state!=0 ||
               prior.temporary_target_timer>1 ||
               prior.entity_id!=probe.before.entity_id ||
               prior.x!=probe.before.x || prior.y!=probe.before.y ||
               actual.x!=prior.x || actual.y!=prior.y ||
               actual.x_float!=prior.x_float || actual.y_float!=prior.y_float) {
                error="native tracked admission lacks original stationary prestate";
                return false;
            }
            const auto admission=staged_tracked.admit_from_follower(
                static_cast<int>(probe.entity_index),*anchor,*target,
                candidate.followers(),staged_registration_counter);
            if(admission.status!=herding::RetailTrackedAdmissionStatus::Inserted) {
                error="native tracked admission violates the 150-unit or first-vacant rule";
                return false;
            }
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
        if(observed.source_ordered_food_and_tracking) {
            const auto& prior=candidate.entities()[probe.entity_index];
            if(prior.temporary_target_timer!=probe.before.temporary_target_timer ||
               prior.target_x!=probe.before.target_x || prior.target_y!=probe.before.target_y ||
               prior.x_float!=probe.before.x_float || prior.y_float!=probe.before.y_float) {
                error="native escape target did not originate in prior state or ordered food release";
                return false;
            }
        }
        const auto expected=
            herding::original_herding_temporary_target_update(probe.before);
        if (!expected || !expected->branch_taken ||
            expected->early_return!=probe.source_returned_early) {
            error="original temporary-target branch selection or "
                  "120-unit early return differs from retail";
            return false;
        }
        auto expected_motion=expected->moved;
        const bool should_run_outer=observed.source_ordered_food_and_tracking &&
            !candidate.followers().contains(static_cast<int>(probe.entity_index));
        if(probe.ran_nonfollowing_outer_pass!=should_run_outer) {
            error="native temporary target omitted or invented original outer dispatch";return false;
        }
        if(should_run_outer) {
            const auto outer=herding::original_herding_nonfollowing_outer_step(
                probe.before,expected_motion,data_.retail_transformed_group0(),
                herding::animal_navigation_recovery_target(data_));
            if(!outer || outer->boundary_recovered!=probe.outer_boundary_recovered) {
                error="native temporary target outer boundary differs from retail";return false;
            }
            expected_motion=outer->animal;
        }
        const auto& moved=observed.motion_records[probe.entity_index];
        // Exact per-axis FSTP values may differ by the original x87
        // extended precision. The source-equivalent direction, timer
        // transition and integer positions must still agree.
        constexpr float kOriginalTrigTolerance=0.001f;
        if (!std::isfinite(moved.x_float) ||
            !std::isfinite(moved.y_float) ||
            std::fabs(moved.x_float-expected_motion.x_float)>
                kOriginalTrigTolerance ||
            std::fabs(moved.y_float-expected_motion.y_float)>
                kOriginalTrigTolerance ||
            moved.x!=expected_motion.x ||
            moved.y!=expected_motion.y ||
            moved.previous_x!=expected_motion.previous_x ||
            moved.previous_y!=expected_motion.previous_y ||
            moved.direction!=expected_motion.direction ||
            moved.temporary_target_timer!=
                expected_motion.temporary_target_timer ||
            moved.movement_speed!=expected_motion.movement_speed ||
            moved.movement_active!=expected_motion.movement_active) {
            error="original 0x416F57 temporary target movement differs "
                  "from source 3-unit steering and 120-unit gate";
            return false;
        }
    }
    // Replay the exact retail type-7 branch against the source-owned
    // dog entity and activity-global waypoint/latch state. A failed
    // later animal event must not commit a new patrol index or latch.
    auto staged_scruffty_state=scruffty_patrol_state_;
    if (observed.scruffty_patrol_evidence) {
        const auto& probe=*observed.scruffty_patrol_evidence;
        if (probe.entity_index>=candidate.entities().size() ||
            probe.entity_index>=observed.motion_records.size() ||
            probe.before.entity_type()!=herding::EntityType::Scruffty ||
            candidate.entities()[probe.entity_index].entity_type()!=
                herding::EntityType::Scruffty ||
            probe.state_before.waypoint_index!=
                scruffty_patrol_state_.waypoint_index ||
            probe.state_before.movement_frame_latch!=
                scruffty_patrol_state_.movement_frame_latch) {
            error="original Scruffty waypoint/latch source evidence "
                  "is missing, stale or inconsistent";
            return false;
        }
        const auto& actual_before=candidate.entities()[probe.entity_index];
        if (probe.before.entity_id!=actual_before.entity_id ||
            probe.before.x!=actual_before.x ||
            probe.before.y!=actual_before.y ||
            probe.before.x_float!=actual_before.x_float ||
            probe.before.y_float!=actual_before.y_float ||
            probe.before.source_left!=actual_before.source_left ||
            probe.before.source_top!=actual_before.source_top ||
            probe.before.source_right!=actual_before.source_right ||
            probe.before.source_bottom!=actual_before.source_bottom ||
            probe.before.animation_frame!=actual_before.animation_frame ||
            probe.before.animation_frame_countdown!=
                actual_before.animation_frame_countdown ||
            probe.before.movement_speed!=actual_before.movement_speed) {
            error="original Scruffty patrol before-state differs from "
                  "committed native entity";
            return false;
        }
        const auto expected=herding::original_scruffty_patrol_step(
            probe.before,herding::scruffty_patrol_path(data_),
            probe.state_before);
        if (!expected ||
            expected->state.waypoint_index!=
                probe.state_after.waypoint_index ||
            expected->state.movement_frame_latch!=
                probe.state_after.movement_frame_latch) {
            error="original Scruffty patrol waypoint or movement "
                  "latch does not match 0x418F23 source code";
            return false;
        }
        const auto& moved=observed.motion_records[probe.entity_index];
        constexpr float kNativeTrigTolerance=0.001f;
        if (moved.x!=expected->animal.x ||
            moved.y!=expected->animal.y ||
            moved.direction!=expected->animal.direction ||
            moved.animation_frame!=expected->animal.animation_frame ||
            moved.animation_frame_countdown!=
                expected->animal.animation_frame_countdown ||
            moved.movement_active!=expected->animal.movement_active ||
            !std::isfinite(moved.x_float) ||
            !std::isfinite(moved.y_float) ||
            !std::isfinite(moved.movement_speed) ||
            std::fabs(moved.x_float-expected->animal.x_float)>
                kNativeTrigTolerance ||
            std::fabs(moved.y_float-expected->animal.y_float)>
                kNativeTrigTolerance ||
            std::fabs(moved.movement_speed-
                expected->animal.movement_speed)>kNativeTrigTolerance) {
            error="source Scruffty movement contradicts original "
                  "five-waypoint animation-gated patrol";
            return false;
        }
        staged_scruffty_state=expected->state;
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
            data_.coordinate_groups[2],
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
    // The original behavior-99 sprite update runs every animation
    // frame even after delivery. Reject source attempts to mutate
    // completed-animal position or re-enter an earlier route state.
    for(const auto& probe:observed.delivered_animation_evidence) {
        if(probe.entity_index==0 ||
           probe.entity_index>=candidate.entities().size()) {
            error="retail delivered animal animation references invalid entity";
            return false;
        }
        const auto& before=candidate.entities()[probe.entity_index];
        if(before.entity_id!=probe.before.entity_id ||
           before.type!=probe.before.type ||
           before.behavior_state!=99 ||
           before.x!=probe.before.x || before.y!=probe.before.y ||
           before.animation_frame!=probe.before.animation_frame ||
           before.animation_frame_countdown!=
               probe.before.animation_frame_countdown) {
            error="delivered animal animation previous source state stale";
            return false;
        }
        const auto expected=
            herding::original_herding_delivered_animal_animation(before);
        if(!expected) {
            error="original delivered animal animation branch invalid";
            return false;
        }
        const auto& after=observed.motion_records[probe.entity_index];
        if(after.behavior_state!=99 ||
           after.direction!=4 ||
           after.animation_frame!=expected->updated.animation_frame ||
           after.animation_frame_countdown!=
               expected->updated.animation_frame_countdown ||
           after.x!=before.x || after.y!=before.y ||
           after.x_float!=before.x_float ||
           after.y_float!=before.y_float) {
            error="native delivered sprite counters or stillness differ from retail";
            return false;
        }
    }
    // Replay the original two-stage route from the PRE-FRAME entity
    // and its signed sprite anchor. A forged 10px arrival must never
    // deliver an animal or decrement the global count.
    for(const auto& probe:observed.home_motion_evidence) {
        if(probe.entity_index==0 ||
           probe.entity_index>=candidate.entities().size()) {
            error="original home motion source references missing animal";
            return false;
        }
        const auto& prior=candidate.entities()[probe.entity_index];
        if(prior.entity_id!=probe.before.entity_id ||
           prior.type!=probe.before.type ||
           prior.behavior_state!=probe.before.behavior_state ||
           prior.x!=probe.before.x || prior.y!=probe.before.y ||
           prior.x_float!=probe.before.x_float ||
           prior.y_float!=probe.before.y_float ||
           prior.movement_speed!=probe.before.movement_speed) {
            error="original home-route motion before-state is stale";
            return false;
        }
        const auto expected=herding::original_herding_home_route_motion(prior);
        if(!expected ||
           expected->source_anchor.x!=probe.source_actor_anchor.x ||
           expected->source_anchor.y!=probe.source_actor_anchor.y ||
           expected->source_target.x!=probe.source_target.x ||
           expected->source_target.y!=probe.source_target.y ||
           expected->source_arrived!=probe.arrived) {
            error="retail signed home-route anchor/10px gate mismatches source";
            return false;
        }
        const auto& actual=observed.motion_records[probe.entity_index];
        constexpr float kHomeX87Tolerance=0.001f;
        if(actual.x!=expected->moved.x ||
           actual.y!=expected->moved.y ||
           actual.direction!=expected->moved.direction ||
           actual.movement_active!=expected->moved.movement_active ||
           actual.animation_frame!=expected->moved.animation_frame ||
           actual.animation_frame_countdown!=
               expected->moved.animation_frame_countdown ||
           actual.movement_speed!=expected->moved.movement_speed ||
           std::fabs(actual.x_float-expected->moved.x_float)>
               kHomeX87Tolerance ||
           std::fabs(actual.y_float-expected->moved.y_float)>
               kHomeX87Tolerance) {
            error="original home-route steering/animation contradicts native frame";
            return false;
        }
        const bool arrival_event=std::any_of(
            observed.events.begin(),observed.events.end(),
            [&](const auto& event) {
                return event.kind==HerdingObservedKind::ReachHomeWaypoint &&
                    event.entity_index==probe.entity_index;
            });
        if(arrival_event!=probe.arrived) {
            error="native home delivery event lacks original pre-move 10px gate";
            return false;
        }
    }
    // Independent replay of the retail 0x41721E tracked-target branch.
    // Its <30 arrival burns an original RNG call, removes only tracked
    // membership, emits a distinct 0->1 event, and never advances the
    // same animal's route state 10+N on this tick.
    for(const auto& probe:observed.tracked_approach_evidence) {
        if(probe.entity_index==0 ||
           probe.entity_index>=candidate.entities().size()) {
            error="tracked home approach source references invalid animal";
            return false;
        }
        const auto& prior=candidate.entities()[probe.entity_index];
        const auto target=
            herding::original_herding_species_registration_point(
                prior.entity_type());
        const auto anchor=perception_anchors[probe.entity_index]?perception_anchors[probe.entity_index]:
            herding::original_herding_animal_tracking_anchor(prior);
        if(!target || !anchor ||
           target->x!=probe.registered_point.x ||
           target->y!=probe.registered_point.y ||
           anchor->x!=probe.source_actor_anchor.x ||
           anchor->y!=probe.source_actor_anchor.y ||
           prior.entity_id!=probe.before.entity_id ||
           prior.x!=probe.before.x || prior.y!=probe.before.y ||
           prior.x_float!=probe.before.x_float ||
           prior.y_float!=probe.before.y_float ||
           prior.behavior_state!=0 ||
           !(observed.source_ordered_food_and_tracking
             ? tracked_slots_.contains(static_cast<int>(probe.entity_index))
             : candidate.followers().contains(static_cast<int>(probe.entity_index)))) {
            error="tracked home original anchor, point or follower prestate invalid";
            return false;
        }
        const auto source_step=herding::original_herding_follower_approach(
            probe.before,*target,*anchor);
        if(!source_step ||
           source_step->reached_tracked_point!=probe.arrived) {
            error="retail tracked 30-unit approach branch disagrees with source";
            return false;
        }
        const auto& actual=observed.motion_records[probe.entity_index];
        const bool has_arrival=std::any_of(
            observed.events.begin(),observed.events.end(),
            [&](const auto& event) {
                return event.kind==HerdingObservedKind::TrackedTargetArrived &&
                    event.entity_index==probe.entity_index;
            });
        if(has_arrival!=probe.arrived) {
            error="tracked arrival event missing original <30 proof";
            return false;
        }
        const bool should_run_outer=observed.source_ordered_food_and_tracking && !probe.arrived &&
            !candidate.followers().contains(static_cast<int>(probe.entity_index));
        if(probe.ran_nonfollowing_outer_pass!=should_run_outer) {
            error="native tracked approach outer membership gate invalid";return false;
        }
        if(probe.arrived) {
            if(actual.x!=prior.x || actual.y!=prior.y ||
               actual.behavior_state!=0) {
                error="tracked arrival must not move or allocate route same tick";
                return false;
            }
        } else {
            auto expected_motion=source_step->updated;
            if(should_run_outer) {
                const auto outer=herding::original_herding_nonfollowing_outer_step(
                    probe.before,expected_motion,data_.retail_transformed_group0(),
                    herding::animal_navigation_recovery_target(data_),saved_positions[probe.entity_index]);
                if(!outer || outer->boundary_recovered!=probe.outer_boundary_recovered) {
                    error="native tracked outer polygon pass differs";return false;
                }
                expected_motion=outer->animal;
            }
            constexpr float kOriginalX87Tolerance=0.001f;
            if(actual.x!=expected_motion.x ||
               actual.y!=expected_motion.y ||
               actual.direction!=expected_motion.direction ||
               actual.animation_frame!=expected_motion.animation_frame ||
               actual.animation_frame_countdown!=
                   expected_motion.animation_frame_countdown ||
               actual.movement_active!=expected_motion.movement_active ||
               std::fabs(actual.x_float-
                    expected_motion.x_float)>kOriginalX87Tolerance ||
               std::fabs(actual.y_float-
                    expected_motion.y_float)>kOriginalX87Tolerance ||
               actual.movement_speed!=expected_motion.movement_speed) {
                error="native tracked-target movement differs from original 0x4172B0";
                return false;
            }
        }
    }
    // Independently replay EVERY real ordinary follower formation
    // step before importing movement and Scruffty distraction events.
    // Original 0x510770 starts at zero for each activity update, so
    // the reported formation order itself is checked, not trusted.
    int expected_following_ordinal=0;
    for(const auto& probe:observed.ordinary_follower_evidence) {
        const bool was_following=candidate.followers().contains(
            static_cast<int>(probe.entity_index));
        const bool joined_on_this_frame=std::any_of(
            observed.events.begin(),observed.events.end(),
            [&](const auto& event) {
                return event.kind==HerdingObservedKind::JoinFollower &&
                    event.entity_index==probe.entity_index &&
                    event.retail_trigger_confirmed;
            });
        if(probe.entity_index==0 ||
           probe.entity_index>=candidate.entities().size() ||
           observed.motion_records.empty() ||
           probe.formation_ordinal!=expected_following_ordinal++ ||
           (!was_following && !joined_on_this_frame)) {
            error="original ordinary follower membership/formation slot invalid";
            return false;
        }
        const auto& original=candidate.entities()[probe.entity_index];
        // A source target timeout from 1 -> 0 is handled before
        // ordinary dispatch on that same frame. There is no stale
        // target steering: allow exactly that pre-dispatch decrement.
        const bool expired_this_frame=
            original.temporary_target_timer==1 &&
            probe.before.temporary_target_timer==0;
        if(original.entity_id!=probe.before.entity_id ||
           original.type!=probe.before.type ||
           original.behavior_state!=0 ||
           original.x!=probe.before.x ||
           original.y!=probe.before.y ||
           original.x_float!=probe.before.x_float ||
           original.y_float!=probe.before.y_float ||
           original.direction!=probe.before.direction ||
           original.movement_speed!=probe.before.movement_speed ||
           original.animation_frame!=probe.before.animation_frame ||
           original.animation_frame_countdown!=
                probe.before.animation_frame_countdown ||
           (!expired_this_frame &&
            original.temporary_target_timer!=
                probe.before.temporary_target_timer)) {
            error="ordinary follower frame prior state contradicted by source";
            return false;
        }
        const auto expected=
            herding::original_herding_ordinary_follower_step(
                probe.before,observed.motion_records.front(),
                probe.formation_ordinal,perception_anchors[probe.entity_index]);
        if(!expected ||
           expected->source_formation_target.x!=
               probe.formation_target.x ||
           expected->source_formation_target.y!=
               probe.formation_target.y ||
           expected->holds_formation_position!=probe.held_position) {
            error="native ordinary follower orbit and source spacing invalid";
            return false;
        }
        auto expected_motion=expected->moved;
        pre_outer_animals[probe.entity_index]=expected_motion;
        const bool should_run_outer=observed.source_ordered_food_and_tracking && !was_following;
        if(probe.ran_nonfollowing_outer_pass!=should_run_outer) {
            error="native ordinary follower outer membership gate invalid";return false;
        }
        if(should_run_outer) {
            const auto outer=herding::original_herding_nonfollowing_outer_step(
                probe.before,expected_motion,data_.retail_transformed_group0(),
                herding::animal_navigation_recovery_target(data_),saved_positions[probe.entity_index]);
            if(!outer || outer->boundary_recovered!=probe.outer_boundary_recovered) {
                error="native ordinary follower outer polygon pass differs";return false;
            }
            expected_motion=outer->animal;
        }
        const auto& actual=observed.motion_records[probe.entity_index];
        constexpr float kX87ApproximationTolerance=0.001f;
        if(actual.x!=expected_motion.x ||
           actual.y!=expected_motion.y ||
           actual.direction!=expected_motion.direction ||
           actual.movement_active!=1 ||
           actual.animation_frame!=expected_motion.animation_frame ||
           actual.animation_frame_countdown!=
               expected_motion.animation_frame_countdown ||
           actual.behavior_state!=0 ||
           std::fabs(actual.x_float-expected_motion.x_float)>
               kX87ApproximationTolerance ||
           std::fabs(actual.y_float-expected_motion.y_float)>
               kX87ApproximationTolerance ||
           std::fabs(actual.movement_speed-
               expected_motion.movement_speed)>
               kX87ApproximationTolerance) {
            error="native animal ordinary formation differs from retail source";
            return false;
        }
    }

    // Source 0x419167 boundary recovery is a DIFFERENT update from
    // the original group-2 random retry path. Independently replay
    // the failed group-0 candidate AND the fixed one-point return
    // steering. A source cannot skip group-0 rejection, replace its
    // target with a synthetic point or invent an accepted inside move.
    for(const auto& probe:observed.far_boundary_evidence) {
        if(probe.entity_index==0 ||
           probe.entity_index>=candidate.entities().size() ||
           observed.motion_records.empty() ||
           observed.motion_records.front().entity_type()!=
               herding::EntityType::FarmerPickles ||
           data_.coordinate_groups.size()!=4 ||
           data_.coordinate_groups[3].size()!=1 ||
           probe.group3_target!=data_.coordinate_groups[3].front()) {
            error="invalid original group-three boundary source evidence";
            return false;
        }
        const auto& original=candidate.entities()[probe.entity_index];
        if(original.entity_id!=probe.before.entity_id ||
           original.type!=probe.before.type ||
           original.x!=probe.before.x ||
           original.y!=probe.before.y ||
           original.x_float!=probe.before.x_float ||
           original.y_float!=probe.before.y_float ||
           original.movement_speed!=probe.before.movement_speed ||
           original.direction!=probe.before.direction) {
            error="stale original animal pre-boundary entity";
            return false;
        }
        const auto preliminary=
            herding::original_far_untracked_animal_roam_step(
                probe.before,observed.motion_records.front(),
                data_.retail_transformed_group0(),perception_anchors[probe.entity_index],saved_positions[probe.entity_index]);
        if(preliminary.coverage!=
               herding::RetailFarAnimalRoamCoverage::
                   RequiresOriginalBoundaryRecovery) {
            error="original free-roam frame never crossed group-zero polygon";
            return false;
        }
        const auto verified=
            herding::original_far_untracked_animal_boundary_recovery(
                probe.before,preliminary.animal,probe.group3_target,saved_positions[probe.entity_index]);
        if(!verified) {
            error="original group-three recovered heading invalid";
            return false;
        }
        const auto& moved=observed.motion_records[probe.entity_index];
        constexpr float kRecoveryTrigTolerance=0.001f;
        if(moved.x!=verified->animal.x ||
           moved.y!=verified->animal.y ||
           moved.previous_x!=verified->animal.previous_x ||
           moved.previous_y!=verified->animal.previous_y ||
           moved.direction!=verified->animal.direction ||
           moved.movement_active!=verified->animal.movement_active ||
           moved.movement_speed!=0.0f ||
           std::fabs(moved.x_float-
               verified->animal.x_float)>kRecoveryTrigTolerance ||
           std::fabs(moved.y_float-
               verified->animal.y_float)>kRecoveryTrigTolerance) {
            error="native out-of-nav animal recovery differs from retail";
            return false;
        }
    }
    // The source's 80-unit non-follower gate and default eight-vector
    // locomotion are checked before any state mutation is committed.
    // A close-range/edge-of-map candidate MUST NOT masquerade as
    // recovered ordinary roaming or fabricate new followers.
    // Replay the close-range wrong-food movement and the exact 1/2-draw
    // voice decision using the host's physical sound snapshot.
    std::vector<bool> near_seen(candidate.entities().size(),false);
    for(const auto& probe:observed.near_retreat_evidence) {
        const auto i=probe.entity_index;
        if(i==0 || i>=candidate.entities().size() || near_seen[i] ||
           candidate.followers().contains(static_cast<int>(i)) ||
           (observed.source_ordered_food_and_tracking && tracked_slots_.contains(static_cast<int>(i)))) {
            error="native near-retreat membership or duplicate evidence invalid";return false;
        }
        near_seen[i]=true;
        const auto& prior=candidate.entities()[i];
        if(prior.entity_id!=probe.before.entity_id || prior.type!=probe.before.type ||
           prior.x!=probe.before.x || prior.y!=probe.before.y ||
           prior.x_float!=probe.before.x_float || prior.y_float!=probe.before.y_float ||
           prior.movement_speed!=probe.before.movement_speed ||
           prior.animation_frame!=probe.before.animation_frame ||
           prior.animation_frame_countdown!=probe.before.animation_frame_countdown ||
           prior.behavior_state!=0 ||
           (prior.temporary_target_timer!=probe.before.temporary_target_timer &&
            !(prior.temporary_target_timer==1 && probe.before.temporary_target_timer==0)) ||
           herding::required_food_for(prior.entity_type())==candidate.food().selected) {
            error="native near-retreat contradicts original animal prestate";return false;
        }
        const auto expected=herding::original_herding_near_retreat(probe.before,observed.motion_records.front(),perception_anchors[i]);
        if(!expected) {error="near-retreat lacks original strict 80-unit gate";return false;}
        const auto outer=herding::original_herding_nonfollowing_outer_step(
            probe.before,expected->moved,data_.retail_transformed_group0(),
            herding::animal_navigation_recovery_target(data_),saved_positions[i]);
        if(!outer || outer->boundary_recovered!=probe.boundary_recovered) {
            error="native near-retreat boundary gate invalid";return false;
        }
        const auto& moved=outer->animal;
        const auto species=prior.type-1;
        const auto offset=static_cast<std::size_t>(species*3);
        retail::OriginalRetailRandom replay{probe.rng_before};
        const auto chatter=herding::original_herding_animal_chatter(replay,species,{
            input.herding_animal_sound_playing[offset],
            input.herding_animal_sound_playing[offset+1],
            input.herding_animal_sound_playing[offset+2]});
        const auto voice_count=std::count_if(observed.events.begin(),observed.events.end(),[&](const auto& event){
            return event.kind==HerdingObservedKind::AnimalChatter && event.entity_index==i &&
                   probe.sound_id && event.source_verified_sound_id==*probe.sound_id;
        });
        const auto& actual=observed.motion_records[i];
        if(!chatter || chatter->requested_sound_id!=probe.sound_id || replay.state()!=probe.rng_after ||
           voice_count!=(probe.sound_id?1:0) ||
           actual.x!=moved.x || actual.y!=moved.y || actual.direction!=moved.direction ||
           actual.previous_x!=moved.previous_x || actual.previous_y!=moved.previous_y ||
           actual.movement_active!=moved.movement_active || actual.movement_speed!=moved.movement_speed ||
           actual.animation_frame!=moved.animation_frame || actual.animation_frame_countdown!=moved.animation_frame_countdown ||
           !std::isfinite(actual.x_float) || !std::isfinite(actual.y_float) ||
           std::fabs(actual.x_float-moved.x_float)>0.001f || std::fabs(actual.y_float-moved.y_float)>0.001f) {
            error="native near-retreat motion, voice, or RNG differs from original";return false;
        }
    }
    for (const auto& probe : observed.far_roam_evidence) {
        if (probe.entity_index==0 ||
            probe.entity_index>=candidate.entities().size() ||
            observed.motion_records.empty() ||
            observed.motion_records.front().entity_type()!=
                herding::EntityType::FarmerPickles) {
            error="invalid native far-roam original entity evidence";
            return false;
        }
        const auto& original=candidate.entities()[probe.entity_index];
        if (original.entity_id!=probe.before.entity_id ||
            original.type!=probe.before.type ||
            original.x!=probe.before.x ||
            original.y!=probe.before.y ||
            original.x_float!=probe.before.x_float ||
            original.y_float!=probe.before.y_float ||
            original.direction!=probe.before.direction ||
            original.movement_speed!=probe.before.movement_speed) {
            error="native far-roam pre-frame state differs from retail entity";
            return false;
        }
        const auto verified=herding::original_far_untracked_animal_roam_step(
            probe.before,observed.motion_records.front(),
            data_.retail_transformed_group0(),perception_anchors[probe.entity_index],saved_positions[probe.entity_index]);
        if (verified.coverage!=
                herding::RetailFarAnimalRoamCoverage::AdvancedWithinPolygon) {
            error="far-roam movement lacked original 80-unit/inside-polygon evidence";
            return false;
        }
        const auto& moved=observed.motion_records[probe.entity_index];
        if (moved.x!=verified.animal.x ||
            moved.y!=verified.animal.y ||
            moved.previous_x!=verified.animal.previous_x ||
            moved.previous_y!=verified.animal.previous_y ||
            moved.x_float!=verified.animal.x_float ||
            moved.y_float!=verified.animal.y_float ||
            moved.direction!=verified.animal.direction ||
            moved.movement_speed!=verified.animal.movement_speed) {
            error="animal default movement differs from original 8-way source table";
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
    std::vector<bool> arrived_at_tracked_point_this_update(
        candidate.entities().size(),false);
    for (const auto& event : observed.events) {
        if(food_applied && event.kind==HerdingObservedKind::PickUpFood)continue;
        if(event.kind==HerdingObservedKind::AnimalChatter &&
           !std::any_of(observed.near_retreat_evidence.begin(),observed.near_retreat_evidence.end(),[&](const auto& p){return p.entity_index==event.entity_index && p.sound_id && *p.sound_id==event.source_verified_sound_id;})) {
            error="animal chatter lacks original retreat/RNG evidence";return false;
        }
        if(observed.require_native_event_proofs) {
            // A caller-supplied confirmed=true assertion is NOT proof
            // that an animal physically reached the original contact.
            // Require the matching independently replayed motion
            // branch. This makes a fabricated completion impossible
            // even if an external native adapter asserts an event.
            const auto ordinary_join=std::any_of(
                observed.ordinary_follower_evidence.begin(),
                observed.ordinary_follower_evidence.end(),
                [&](const auto& e){return e.entity_index==event.entity_index;});
            const auto tracked_arrival=std::any_of(
                observed.tracked_approach_evidence.begin(),
                observed.tracked_approach_evidence.end(),
                [&](const auto& e){
                    return e.entity_index==event.entity_index && e.arrived;
                });
            const auto home_arrival=std::any_of(
                observed.home_motion_evidence.begin(),
                observed.home_motion_evidence.end(),
                [&](const auto& e){
                    return e.entity_index==event.entity_index && e.arrived;
                });
            bool genuine_attraction=ordinary_join;
            if(event.kind==HerdingObservedKind::JoinFollower &&
               ordinary_join) {
                // A plausible formation motion is insufficient by
                // itself: the joining animal must also have the
                // source's strict 80px signed-anchor food contact.
                // Candidate food already reflects a same-frame
                // PickUpFood event (which retail dispatches first).
                const auto proof=std::find_if(
                    observed.ordinary_follower_evidence.begin(),
                    observed.ordinary_follower_evidence.end(),
                    [&](const auto& e){
                        return e.entity_index==event.entity_index;
                    });
                const auto contact=
                    herding::original_herding_food_attraction_contact(
                        proof->before,observed.motion_records.front(),
                        candidate.food().selected,false,perception_anchors[event.entity_index]);
                genuine_attraction=contact && *contact;
            }
            const bool source_proven=
                (event.kind!=HerdingObservedKind::JoinFollower ||
                 genuine_attraction) &&
                (event.kind!=HerdingObservedKind::TrackedTargetArrived ||
                 tracked_arrival) &&
                (event.kind!=HerdingObservedKind::ReachHomeWaypoint ||
                 home_arrival) &&
                (event.kind!=HerdingObservedKind::ScrufftyCollision ||
                 event.retail_rect_contact.has_value());
            if(!source_proven) {
                error="native animal state change has no correlated original "
                      "movement/collision evidence";
                return false;
            }
        }
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
        bool independent_tracked_membership=false;
        if(observed.source_ordered_food_and_tracking &&
           event.kind==HerdingObservedKind::TrackedTargetArrived) {
            independent_tracked_membership=staged_tracked.remove(static_cast<int>(event.entity_index));
            if(!independent_tracked_membership) {
                error="native tracked arrival missing independent registration slot";
                return false;
            }
        }
        const auto* pre_patrol_dog=observed.source_ordered_food_and_tracking &&
            observed.scruffty_patrol_evidence ? &observed.scruffty_patrol_evidence->before : nullptr;
        const auto& pre_outer=pre_outer_animals[event.entity_index];
        const bool pre_move_arrival=observed.require_native_event_proofs &&
            event.kind==HerdingObservedKind::ReachHomeWaypoint &&
            std::any_of(observed.home_motion_evidence.begin(),observed.home_motion_evidence.end(),
                [&](const auto& p){return p.entity_index==event.entity_index && p.arrived;});
        if (!apply_confirmed_event(candidate,event,sounds,error,independent_tracked_membership,pre_patrol_dog,
                                   pre_outer?&*pre_outer:nullptr,pre_move_arrival)) {
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
    tracked_slots_=staged_tracked;
    registration_counter_=staged_registration_counter;
    scruffty_patrol_state_=staged_scruffty_state;
    provisional.accept();
    error.clear();
    return true;
}

bool HerdingEventSimulation::unload(std::string& error) {
    if (state_ && motion_ && !motion_->unload(error)) {
        return false;
    }
    state_.reset();
    data_={};
    tracked_slots_=herding::RetailTrackedTargetSlots{};
    registration_counter_=0;
    error.clear();
    return true;
}

} // namespace btb::full_game
