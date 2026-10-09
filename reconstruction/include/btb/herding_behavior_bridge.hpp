#pragma once

#include "btb/herding_runtime.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace btb::herding {

// Exact event-level Herding gameplay state for source-proven decisions.
// The original continuous steering, nav-polygon collision, and perception
// calculations decide WHEN an event occurs; do not invent their thresholds
// or random sequence here. Event arguments are the actual reduced retail
// rand()%N results from the caller.
struct WanderRandom {
    int x_mod_400{};
    int y_mod_400{};
};

struct HerdingBehaviorEvent {
    bool accepted{};
    int sound_id{-1};
    std::size_t released_followers{};
    HomeRouteGateTrigger gate_trigger{HomeRouteGateTrigger::None};
    bool animal_delivered{};
};

class HerdingRecoveredBehavior {
public:
    explicit HerdingRecoveredBehavior(
        int difficulty,
        std::vector<RetailEntityRecord32> original_entities);

    [[nodiscard]] bool valid() const noexcept { return valid_; }
    [[nodiscard]] int difficulty() const noexcept { return difficulty_; }
    [[nodiscard]] int undelivered() const noexcept {
        return undelivered_;
    }
    [[nodiscard]] const FoodSelectionState& food() const noexcept {
        return food_;
    }
    [[nodiscard]] const FollowerList& followers() const noexcept {
        return followers_;
    }
    [[nodiscard]] const std::vector<RetailEntityRecord32>& entities()
        const noexcept { return entities_; }

    [[nodiscard]] HerdingBehaviorEvent select_food(
        FoodType food,int pickles_x,int pickles_y,int random_mod_2,
        const std::vector<WanderRandom>& random_per_entity_index);

    [[nodiscard]] HerdingBehaviorEvent join_follower(
        std::size_t entity_index,int random_mod_2,
        bool original_attraction_collision_confirmed);

    [[nodiscard]] HerdingBehaviorEvent scruffty_distraction(
        std::size_t entity_index,int random_mod_400,
        bool original_scruffty_collision_confirmed);

    [[nodiscard]] HerdingBehaviorEvent begin_home_route(
        std::size_t entity_index);

    [[nodiscard]] HerdingBehaviorEvent arrive_at_home_waypoint(
        std::size_t entity_index,
        bool original_arrival_confirmed);

private:
    [[nodiscard]] RetailEntityRecord32* entity(
        std::size_t index) noexcept;
    [[nodiscard]] static std::optional<std::size_t> species_index(
        EntityType type) noexcept;

    std::vector<RetailEntityRecord32> entities_{};
    FoodSelectionState food_{};
    FollowerList followers_{};
    std::array<int,3> home_route_counter_{};
    int difficulty_{};
    int undelivered_{};
    bool valid_{};
};

} // namespace btb::herding
