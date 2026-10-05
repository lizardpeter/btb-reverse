#pragma once

#include "btb/park_designer_data.hpp"

#include <cstdint>

namespace btb::park_designer {

enum class CompletionAction : std::int32_t {
    None,
    PlayClosingLine,
    SaveUnloadAndExit,
};

struct CompletionStep {
    std::int32_t stage{};
    CompletionAction action{CompletionAction::None};
    std::int32_t sound_id{-1};
};

// Retail global 0x00509368:
// 0 -> play one of DYP_G_BOB_12..14 (246..248)
// 1 -> wait for managed audio to finish
// 2 -> save/unload and return from the activity
[[nodiscard]] constexpr CompletionStep completion_step(
    std::int32_t stage,
    bool any_managed_sound_playing,
    std::int32_t random_mod_3) noexcept {

    if (stage == 0) {
        if (random_mod_3 < 0 || random_mod_3 > 2) {
            return {stage, CompletionAction::None, -1};
        }
        return {
            1,
            CompletionAction::PlayClosingLine,
            246 + random_mod_3,
        };
    }

    if (stage == 1) {
        if (any_managed_sound_playing) {
            return {1, CompletionAction::None, -1};
        }
        return {2, CompletionAction::None, -1};
    }

    if (stage == 2) {
        return {2, CompletionAction::SaveUnloadAndExit, -1};
    }

    return {stage, CompletionAction::None, -1};
}

[[nodiscard]] bool has_any_placed_object(const SaveData& data) noexcept;

} // namespace btb::park_designer
