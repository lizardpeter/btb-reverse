#pragma once

#include "btb/squirrel_data.hpp"

#include <array>
#include <cstdint>

namespace btb::squirrel {

// Global 0x5150E4. Names intentionally describe the observed transition role;
// the exact story label of the two mirrored paths is not assumed.
enum class PlacementState : std::int32_t {
    IdleSelect = 0,
    MoveToSelectedPrimary = 1,
    MoveToSelectedAlternate = 2,
    CommitPrimaryPlacement = 3,
    CommitAlternatePlacement = 4,
    ReturnHome = 5,
    AlternateReturn = 6,
    MoveToReturnPoint = 7,
    ReturnPieceToConveyor = 8,
};

struct MotionStep {
    Vec2i position{};
    bool reached{};
};

// Exact 0x00427270 movement rule: each axis advances by 2 toward the target
// until its absolute delta is <=3, at which point retail snaps that axis.
[[nodiscard]] constexpr MotionStep step_toward_target(
    Vec2i current,
    Vec2i target) noexcept {

    auto step_axis = [](std::int32_t value, std::int32_t goal) constexpr {
        const auto delta = goal - value;
        const auto magnitude = delta < 0 ? -delta : delta;
        if (magnitude <= 3) {
            return goal;
        }
        return value + (delta > 0 ? 2 : -2);
    };

    current.x = step_axis(current.x, target.x);
    current.y = step_axis(current.y, target.y);

    return {
        current,
        current.x == target.x && current.y == target.y,
    };
}

enum class CompletionAction : std::int32_t {
    None,
    PlayFinalWendyLine,
    ExitToPlayAgain,
};

struct CompletionStep {
    std::int32_t stage{};
    CompletionAction action{CompletionAction::None};
    std::int32_t sound_id{-1};
};

// Global 0x515058 in the complete path of UpdateSquirrelActivity:
// stage 0 stops managed sound and plays SR_WEN_23..25,
// stage 1 waits for managed audio to finish,
// stage 2 exits through the common completion/Play Again path.
[[nodiscard]] constexpr CompletionStep completion_step(
    std::int32_t stage,
    bool any_managed_sound_playing,
    std::int32_t random_mod_3) noexcept {

    if (stage == 0) {
        if (random_mod_3 < 0 || random_mod_3 > 2) {
            return {0, CompletionAction::None, -1};
        }
        return {
            1,
            CompletionAction::PlayFinalWendyLine,
            665 + random_mod_3,
        };
    }

    if (stage == 1) {
        if (any_managed_sound_playing) {
            return {1, CompletionAction::None, -1};
        }
        return {2, CompletionAction::None, -1};
    }

    if (stage >= 2) {
        return {stage, CompletionAction::ExitToPlayAgain, -1};
    }

    return {stage, CompletionAction::None, -1};
}

inline constexpr std::int32_t kStartupWendySoundId = 643; // SR_WEN_01
inline constexpr std::int32_t kStartupLoftySoundId = 632; // SR_LOF_01
inline constexpr std::int32_t kStartupWendyFollowupSoundId = 644; // SR_WEN_02

inline constexpr std::array<std::int32_t,4> kPrimaryPlacementLoftyFeedback{
    633, 634, 635, 636 // SR_LOF_02..05
};

inline constexpr std::array<std::int32_t,3> kPrimaryPlacementWendyFeedback{
    656, 657, 658 // SR_WEN_14..16
};

inline constexpr std::array<std::int32_t,2> kAlternatePlacementLoftyFeedback{
    637, 638 // SR_LOF_06..07
};

inline constexpr std::array<std::int32_t,7> kAlternatePlacementWendyFeedback{
    645, 646, 648, 649, 650, 651, 652
    // SR_WEN_03,04,06,07,08,09,10
};

inline constexpr std::array<std::int32_t,3> kFinalWendyFeedback{
    665, 666, 667 // SR_WEN_23..25
};

} // namespace btb::squirrel
