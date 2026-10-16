#pragma once

#include "btb/full_game_herding_event_simulation.hpp"

namespace btb::full_game {

// A deliberately limited, source-verified native implementation of
// Pickles movement and original Pets Corner population construction.
// Scruffty runs the recovered original patrol. Sheep/rabbits/ducks
// retain their last known source states until the remaining animal
// motion is connected; no fake follower contacts or deliveries.
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
};
} // namespace btb::full_game
