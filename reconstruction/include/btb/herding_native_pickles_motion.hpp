#pragma once

#include "btb/full_game_herding_event_simulation.hpp"
#include "btb/herding_source_tracked_targets.hpp"

namespace btb::full_game {

// A deliberately limited, source-verified native implementation of
// Pickles movement and original Pets Corner population construction.
// Scruffty runs its original patrol; unfollowed animals run their
// covered original far-roam branch. This source confirms 80px food
// attraction but does not invent the remaining follower pursuit,
// Scruffty-animal contacts or delivery.
class NativePicklesMotionSource final : public OriginalHerdingMotionSource {
public:
    [[nodiscard]] bool initialize(
        const herding::Data& data,int difficulty,
        std::vector<herding::RetailEntityRecord32>& originals,
        std::string& error) override;
    [[nodiscard]] bool advance(
        const herding::Data& data,
        const ActivityFrameInput& input,
        const herding::PicklesKeyboardMotion& motion,
        const std::vector<herding::RetailEntityRecord32>& source,
        HerdingObservedFrame& observed,
        std::string& error) override;
    [[nodiscard]] bool unload(std::string& error) override;
private:
    bool active_{};
    herding::FoodType selected_food_{herding::FoodType::None};
    herding::RetailScrufftyPatrolState scruffty_patrol_state_{};
    // Retail's ordinary follower list uses first-vacant insertion,
    // and its food-change release loop consumes RNG in that order.
    std::vector<std::size_t> follower_indices_{};
    herding::RetailTrackedTargetSlots tracked_slots_{};
    int remaining_to_register_{};
};
} // namespace btb::full_game
