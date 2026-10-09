#include "btb/full_game_herding_event_simulation.hpp"

#include <algorithm>
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
    case HerdingObservedKind::ScrufftyCollision:
        step=staged.scruffty_distraction(
            event.entity_index,event.random_mod_400,true);
        break;
    case HerdingObservedKind::EnterHomeRoute:
        if (!staged.confirm_home_route_trigger(event.entity_index,true)) {
            error="Herding home trigger requires an existing follower";
            return false;
        }
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
    for (std::size_t i=0;i<observed.motion_records.size();++i) {
        if (!candidate.apply_source_motion(i,observed.motion_records[i])) {
            error="Herding movement source attempted to replace entity "
                  "identity or supplied invalid source coordinates";
            return false;
        }
    }
    std::vector<Audio> sounds{};
    for (const auto& event : observed.events) {
        if (!apply_confirmed_event(candidate,event,sounds,error)) {
            return false;
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
