#pragma once

#include "btb/full_game_herding_event_simulation.hpp"

namespace btb::full_game {

// A deliberately limited, source-verified native implementation of
// Pickles movement and original Pets Corner population construction.
// It does not invent the unreconstructed autonomous animal source:
// animals remain in their last source state, with no fabricated
// proximity triggers, follower transitions, or successful deliveries.
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
};
} // namespace btb::full_game
