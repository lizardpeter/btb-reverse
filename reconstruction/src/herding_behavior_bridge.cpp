#include "btb/herding_behavior_bridge.hpp"

#include <algorithm>
#include <cmath>
#include <utility>

namespace btb::herding {

HerdingRecoveredBehavior::HerdingRecoveredBehavior(
    int difficulty,
    std::vector<RetailEntityRecord32> original_entities)
    : entities_(std::move(original_entities)),
      difficulty_(difficulty) {

    if (difficulty_<0 || difficulty_>2) {
        return;
    }
    const int per_species=animals_per_species(difficulty_);
    std::array<int,4> present{};
    for (const auto& entity : entities_) {
        switch (entity.entity_type()) {
        case EntityType::FarmerPickles: ++present[0]; break;
        case EntityType::Sheep: ++present[1]; break;
        case EntityType::Rabbit: ++present[2]; break;
        case EntityType::Duck: ++present[3]; break;
        default: break;
        }
    }
    valid_=present[0]==1 &&
           present[1]==per_species &&
           present[2]==per_species &&
           present[3]==per_species;
    if (valid_) {
        undelivered_=initial_undelivered_animal_count(difficulty_);
    }
}

RetailEntityRecord32* HerdingRecoveredBehavior::entity(
    std::size_t index) noexcept {
    return index<entities_.size() ? &entities_[index] : nullptr;
}

bool HerdingRecoveredBehavior::apply_source_motion(
    std::size_t index,
    const RetailEntityRecord32& source) noexcept {

    auto* target=entity(index);
    if (!valid_ || !target ||
        source.entity_id != target->entity_id ||
        source.type != target->type ||
        source.direction<0 || source.direction>7 ||
        !std::isfinite(source.x_float) ||
        !std::isfinite(source.y_float) ||
        !std::isfinite(source.movement_speed)) {
        return false;
    }
    // The frame-by-frame movement engine is responsible for these fields.
    // Do not accept external overwrites of food, follower, AI, delivery,
    // route allocation or sprite/DirectDraw ownership state.
    target->previous_x=target->x;
    target->previous_y=target->y;
    target->x=source.x;
    target->y=source.y;
    target->x_float=source.x_float;
    target->y_float=source.y_float;
    target->direction=source.direction;
    target->direction_degrees=source.direction_degrees;
    target->movement_direction_mask=source.movement_direction_mask;
    target->movement_active=source.movement_active;
    target->movement_speed=source.movement_speed;
    return true;
}

bool HerdingRecoveredBehavior::synchronize_render_counters(
    const std::vector<RetailEntityRecord32>& rendered) noexcept {
    if (!valid_ || rendered.size()!=entities_.size()) return false;
    for (std::size_t i=0;i<rendered.size();++i) {
        if (rendered[i].entity_id!=entities_[i].entity_id ||
            rendered[i].type!=entities_[i].type) {
            return false;
        }
    }
    for (std::size_t i=0;i<rendered.size();++i) {
        entities_[i].animation_frame=rendered[i].animation_frame;
        entities_[i].animation_frame_countdown=
            rendered[i].animation_frame_countdown;
    }
    return true;
}

bool HerdingRecoveredBehavior::confirm_home_route_trigger(
    std::size_t index,
    bool original_trigger_confirmed) noexcept {

    auto* animal=entity(index);
    if (!valid_ || !animal || !original_trigger_confirmed ||
        animal->behavior_state!=0 ||
        !is_herd_animal(animal->entity_type()) ||
        !followers_.contains(static_cast<int>(index))) {
        return false;
    }
    animal->behavior_state=1;
    return true;
}

std::optional<std::size_t> HerdingRecoveredBehavior::species_index(
    EntityType type) noexcept {
    switch (type) {
    case EntityType::Sheep: return 0;
    case EntityType::Rabbit: return 1;
    case EntityType::Duck: return 2;
    default: return std::nullopt;
    }
}

HerdingBehaviorEvent HerdingRecoveredBehavior::select_food(
    FoodType selected,int pickles_x,int pickles_y,int random_mod_2,
    const std::vector<WanderRandom>& random_by_entity) {

    if (!valid_) return {};
    // Validate ALL followers and their supplied original random draws
    // before mutating food or entity state. A missing caller RNG sample
    // must not partially release an original follower group.
    for (const auto index : followers_.slots()) {
        if (index<0) continue;
        if (static_cast<std::size_t>(index)>=entities_.size() ||
            static_cast<std::size_t>(index)>=random_by_entity.size()) {
            return {};
        }
        const auto& random=random_by_entity[
            static_cast<std::size_t>(index)];
        if (random.x_mod_400<0 || random.x_mod_400>=400 ||
            random.y_mod_400<0 || random.y_mod_400>=400) {
            return {};
        }
    }
    const auto selected_step=try_select_food(
        food_,selected,pickles_x,pickles_y,random_mod_2);
    if (!selected_step.changed) return {};

    std::size_t released=0;
    const auto held=followers_.slots();
    for (const auto index : held) {
        if (index<0) continue;
        const auto random=random_by_entity[
            static_cast<std::size_t>(index)];
        if (release_follower_for_food_change(
                entities_[static_cast<std::size_t>(index)],
                followers_,index,
                random.x_mod_400,random.y_mod_400)) {
            ++released;
        }
    }
    return {true,selected_step.sound_id,released};
}

HerdingBehaviorEvent HerdingRecoveredBehavior::join_follower(
    std::size_t index,int random_mod_2,
    bool original_attraction_collision_confirmed) {

    auto* animal=entity(index);
    if (!valid_ || !animal ||
        !original_attraction_collision_confirmed ||
        animal->behavior_state!=0 ||
        !is_herd_animal(animal->entity_type()) ||
        required_food_for(animal->entity_type())!=food_.selected ||
        (random_mod_2!=0 && random_mod_2!=1)) {
        return {};
    }
    if (!followers_.add(static_cast<int>(index))) return {};
    const auto voice=attraction_sound_ids(food_.selected);
    return {true,random_mod_2==0 ? voice.a : voice.b};
}

HerdingBehaviorEvent HerdingRecoveredBehavior::scruffty_distraction(
    std::size_t index,int random_mod_400,
    bool original_scruffty_collision_confirmed) {

    auto* animal=entity(index);
    if (!valid_ || !animal ||
        !original_scruffty_collision_confirmed) {
        return {};
    }
    if (!apply_scruffty_distraction(
            *animal,followers_,static_cast<int>(index),
            random_mod_400)) {
        return {};
    }
    return {true};
}

HerdingBehaviorEvent HerdingRecoveredBehavior::begin_home_route(
    std::size_t index) {

    auto* animal=entity(index);
    if (!valid_ || !animal || animal->behavior_state!=1) {
        return {};
    }
    const auto species=species_index(animal->entity_type());
    if (!species) return {};
    const auto step=begin_home_route_step(
        animal->entity_type(),home_route_counter_[*species],difficulty_);
    if (!step) return {};

    // Behavior state 1 consumes a species-local route counter and assigns
    // original state 10+counter. This is NOT an instantaneous home delivery.
    home_route_counter_[*species]=step->next_species_route_counter;
    animal->behavior_state=step->assigned_behavior_state;
    static_cast<void>(followers_.remove(static_cast<int>(index)));

    if (step->gate_trigger!=HomeRouteGateTrigger::None) {
        const auto gate_type=step->gate_trigger ==
            HomeRouteGateTrigger::LeftGate
            ? EntityType::GateLeft : EntityType::GateRight;
        const auto gate=std::find_if(
            entities_.begin(),entities_.end(),
            [&](const auto& record) {
                return record.entity_type()==gate_type;
            });
        if (gate!=entities_.end()) {
            gate->animation_frame_countdown=step->gate_animation_timer;
        }
    }
    return {true,step->sound_id,0,step->gate_trigger,false};
}

HerdingBehaviorEvent HerdingRecoveredBehavior::arrive_at_home_waypoint(
    std::size_t index,bool original_arrival_confirmed) {

    auto* animal=entity(index);
    if (!valid_ || !animal || !original_arrival_confirmed ||
        !is_herd_animal(animal->entity_type())) {
        return {};
    }
    const auto step=home_route_arrival_step(
        animal->behavior_state,undelivered_);
    if (!step || (step->delivered && undelivered_<=0)) return {};
    animal->behavior_state=step->next_behavior_state;
    undelivered_=step->next_undelivered_count;
    return {true,-1,0,HomeRouteGateTrigger::None,step->delivered};
}

} // namespace btb::herding
