#pragma once

#include "btb/squirrel_data.hpp"

#include <array>
#include <cstdint>

namespace btb::squirrel {

// Global 0x5150E4. Names intentionally describe the observed transition role;
// the exact story label of the two mirrored paths is not assumed.
enum class PlacementState : std::int32_t {
    IdleSelect = 0,
    MoveToSelectedCorrect = 1,
    MoveToSelectedDecoy = 2,
    CommitCorrectPlacement = 3,
    CommitDecoyPlacement = 4,
    ReturnHome = 5,
    ReturnAfterDecoy = 6,
    MoveToDecoyReturnPoint = 7,
    ReturnDecoyToConveyor = 8,
};

enum class Difficulty : std::int32_t {
    Easy = 0,
    Medium = 1,
    Hard = 2,
};

struct ConveyorMix {
    std::int32_t correct_items{};
    std::int32_t decoy_items{};

    [[nodiscard]] constexpr std::int32_t total_items() const noexcept {
        return correct_items + decoy_items;
    }
};

[[nodiscard]] constexpr ConveyorMix conveyor_mix(
    Difficulty difficulty) noexcept {
    switch (difficulty) {
        case Difficulty::Easy: return {3, 0};
        case Difficulty::Medium: return {3, 1};
        case Difficulty::Hard: return {2, 2};
    }
    return {0, 0};
}

inline constexpr std::array<std::array<std::int32_t,4>,7>
    kOuterVariantPermutations{{
        {{0,1,2,3}},
        {{0,3,2,1}},
        {{1,3,2,0}},
        {{1,0,2,3}},
        {{3,2,0,1}},
        {{2,1,3,0}},
        {{3,1,2,0}},
    }};

struct PieceIdParts {
    std::int32_t outer_variant{};
    std::int32_t from_connector{};
    std::int32_t to_connector{};
};

[[nodiscard]] constexpr std::int32_t encode_piece_id(
    PieceIdParts parts) noexcept {
    return parts.outer_variant * 9
         + parts.from_connector * 3
         + parts.to_connector;
}

[[nodiscard]] constexpr PieceIdParts decode_piece_id(
    std::int32_t piece_id) noexcept {
    return {
        piece_id / 9,
        (piece_id % 9) / 3,
        piece_id % 3,
    };
}

inline constexpr std::size_t kMaximumLevelCount = 3;
inline constexpr std::size_t kConnectorsPerLevel = 4;
inline constexpr std::size_t kPiecesPerLevel = 3;

using RunPlan = std::array<
    std::array<std::int32_t, kConnectorsPerLevel>,
    kMaximumLevelCount>;

[[nodiscard]] constexpr RunPlan chain_run_plan(
    RunPlan random_draws) noexcept {
    // Retail generates all 12 rand()%3 values first, then overwrites the
    // first connector of levels 1 and 2 with the preceding level's end.
    random_draws[1][0] = random_draws[0][3];
    random_draws[2][0] = random_draws[1][3];
    return random_draws;
}

struct ConnectorPair {
    std::int32_t from{};
    std::int32_t to{};
    friend bool operator==(const ConnectorPair&, const ConnectorPair&) = default;
};

[[nodiscard]] constexpr ConnectorPair required_connectors(
    const RunPlan& plan,
    std::size_t level,
    std::size_t piece_index) noexcept {
    return {
        plan[level][piece_index],
        plan[level][piece_index + 1],
    };
}

[[nodiscard]] constexpr bool is_correct_connector_pair(
    ConnectorPair offered,
    ConnectorPair required) noexcept {
    return offered == required;
}

[[nodiscard]] constexpr bool is_retail_decoy_pair(
    ConnectorPair offered,
    ConnectorPair required) noexcept {
    // GenerateSquirrelConveyorChoices explicitly retries each random connector
    // until both dimensions differ from the required pair.
    return offered.from != required.from &&
           offered.to != required.to;
}

[[nodiscard]] constexpr std::int32_t level_count(
    Difficulty difficulty) noexcept {
    return static_cast<std::int32_t>(difficulty) + 1;
}

[[nodiscard]] constexpr std::int32_t required_correct_placements(
    Difficulty difficulty) noexcept {
    return level_count(difficulty) *
           static_cast<std::int32_t>(kPiecesPerLevel);
}

inline constexpr std::array<Vec2i,4> kConveyorItemOrigins{{
    {100,300},
    {189,300},
    {370,300},
    {459,300},
}};

inline constexpr std::int32_t kConveyorHitWidth = 89;
inline constexpr std::int32_t kConveyorHitHeight = 133;

inline constexpr std::array<Vec2i,kPiecesPerLevel> kRunPlacementTargets{{
    {116,327},
    {276,327},
    {436,327},
}};

inline constexpr Vec2i kLoftyHomeTarget{93,185};

[[nodiscard]] constexpr bool run_reached_level_end(
    std::int32_t run_section) noexcept {
    return run_section > 6;
}

[[nodiscard]] constexpr bool should_finish_activity(
    Difficulty difficulty,
    std::int32_t level_index,
    std::int32_t run_section) noexcept {
    return run_reached_level_end(run_section) &&
           level_index >= static_cast<std::int32_t>(difficulty);
}

[[nodiscard]] constexpr bool should_advance_level(
    Difficulty difficulty,
    std::int32_t level_index,
    std::int32_t run_section) noexcept {
    return run_reached_level_end(run_section) &&
           level_index < static_cast<std::int32_t>(difficulty);
}

[[nodiscard]] constexpr std::int32_t level_finished_feedback_sound_id(
    std::int32_t random_mod_4) noexcept {
    return random_mod_4 >= 0 && random_mod_4 < 4
        ? 664 + random_mod_4 // SR_WEN_22..25
        : -1;
}

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
