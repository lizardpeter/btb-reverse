#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/herding_presentation.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace btb::full_game {

// The recovered 0x416370 compositor consumes the original 0x64-byte entity
// records after UpdateHerdingActivity has updated their positions and AI.
// It is deliberately separate from animal AI: a render pass must never
// invent NPC decisions just because the animation code is known.
struct HerdingScene {
    std::vector<herding::RetailEntityRecord32> entities{};
    herding::FoodType selected_food{herding::FoodType::None};
    std::array<bool,3> visible_world_bags{{true,true,true}};
    bool draw_surround{true};
};

struct HerdingComposedFrame {
    std::vector<Draw> draws{};
    herding::CameraPlan camera{};
    std::size_t rejected_unknown_entities{};
    bool missing_farmer_pickles{};
};

[[nodiscard]] HerdingComposedFrame compose_original_herding_frame(
    HerdingScene& scene);

} // namespace btb::full_game
