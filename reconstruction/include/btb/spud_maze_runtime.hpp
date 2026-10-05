#pragma once

#include "btb/spud_maze_data.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::spud_maze {

enum class Difficulty : std::int32_t {
    Easy = 0,
    Medium = 1,
    Hard = 2,
};

enum class ScreenTransition : std::int32_t {
    None = 0,
    LeftUpper = 0x80,
    RightUpper = 0x100,
    LeftLower = 0x400,
    RightLower = 0x800,
    OtherSpecial = -1,
};

[[nodiscard]] constexpr ScreenTransition classify_screen_transition(
    std::int32_t node_type) noexcept {
    if ((node_type & 0x80) != 0) return ScreenTransition::LeftUpper;
    if ((node_type & 0x400) != 0) return ScreenTransition::LeftLower;
    if ((node_type & 0x100) != 0) return ScreenTransition::RightUpper;
    if ((node_type & 0x800) != 0) return ScreenTransition::RightLower;
    return (node_type & 0xF80) != 0
        ? ScreenTransition::OtherSpecial
        : ScreenTransition::None;
}

struct ScreenTransitionResult {
    std::int32_t screen{};
    std::int32_t node{};
    std::int32_t retail_direction{}; // 1=left, 2=right, 0=none
};

[[nodiscard]] constexpr ScreenTransitionResult apply_screen_transition(
    std::int32_t screen,
    std::int32_t node,
    ScreenTransition transition) noexcept {

    switch (transition) {
        case ScreenTransition::LeftUpper:
        case ScreenTransition::LeftLower:
            return {screen - 1, node - 2, 1};

        case ScreenTransition::RightUpper:
        case ScreenTransition::RightLower:
            return {screen + 1, node + 2, 2};

        default:
            return {screen, node, 0};
    }
}

[[nodiscard]] constexpr std::int32_t repair_damage_divisor(
    Difficulty difficulty) noexcept {
    switch (difficulty) {
        case Difficulty::Easy: return 8;
        case Difficulty::Medium: return 5;
        case Difficulty::Hard: return 3;
    }
    return 8;
}

struct RepairSelection {
    std::array<bool, kRepairPointCount> damaged{};
    std::int32_t repairs_remaining{};
};

inline constexpr std::size_t kRetailPathNodeLimit = 20;

struct GraphPath {
    std::array<std::int32_t, kRetailPathNodeLimit> nodes{};
    std::size_t node_count{};
    std::int32_t total_cost{};
    bool found{};
};

// Source-equivalent form of 0x00423030/0x00423220. Retail performs a
// recursive depth-first search in link order 0..3, rejects cycles by scanning
// the current path, accumulates integer Euclidean edge distances, and retains
// only a route whose cost beats the current best.
[[nodiscard]] GraphPath shortest_graph_path(
    const std::vector<Node>& nodes,
    std::int32_t start_node,
    std::int32_t target_node);

// Retail computes phase_seed as rand()%4 + 1, then walks all 32 repair spots
// in screen order. It marks a section broken when counter % divisor == 0 and
// increments counter after each spot.
[[nodiscard]] RepairSelection select_repair_damage(
    Difficulty difficulty,
    std::int32_t phase_seed_1_to_4);

enum class ActivityOutcome : std::int32_t {
    Continue,
    Success,
    Timeout,
};

[[nodiscard]] constexpr ActivityOutcome activity_outcome(
    std::int32_t repairs_remaining,
    std::int32_t timer_value) noexcept {
    if (repairs_remaining <= 0) return ActivityOutcome::Success;
    if (timer_value < 0) return ActivityOutcome::Timeout;
    return ActivityOutcome::Continue;
}

struct HammerDrop {
    std::int32_t screen{};
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t collision_cooldown{};
};

// Exact persistent geometry/state written by HandlePilchardCollisionAndHammerDrop.
// Retail offsets the dropped hammer +/-10 px from Bob according to direction,
// 41 px down, and starts a 500-tick collision cooldown.
[[nodiscard]] constexpr HammerDrop make_hammer_drop(
    std::int32_t bob_x,
    std::int32_t bob_y,
    std::int32_t bob_direction,
    std::int32_t current_screen) noexcept {

    return {
        current_screen,
        bob_x + (bob_direction == 1 ? 10 : -10),
        bob_y + 41,
        500,
    };
}

[[nodiscard]] constexpr bool can_pick_up_hammer_x(
    std::int32_t bob_x,
    std::int32_t hammer_x_adjusted) noexcept {
    const auto delta = bob_x >= hammer_x_adjusted
        ? bob_x - hammer_x_adjusted
        : hammer_x_adjusted - bob_x;
    return delta < 20;
}

[[nodiscard]] constexpr std::int32_t repair_sound_id(
    std::int32_t random_mod_7) noexcept {
    return random_mod_7 >= 0 && random_mod_7 < 7
        ? 702 + random_mod_7  // SS1_BOB_09..15
        : -1;
}

[[nodiscard]] constexpr std::int32_t no_hammer_sound_id(
    std::int32_t random_bit) noexcept {
    return random_bit >= 0 && random_bit < 2
        ? 709 + random_bit  // SS1_BOB_16/17
        : -1;
}

[[nodiscard]] constexpr std::int32_t pilchard_collision_bob_sound_id(
    std::int32_t random_mod_6) noexcept {
    return random_mod_6 >= 0 && random_mod_6 < 6
        ? 696 + random_mod_6  // SS1_BOB_03..08
        : -1;
}

[[nodiscard]] constexpr std::int32_t pilchard_reaction_sound_id(
    std::int32_t random_bit) noexcept {
    return random_bit >= 0 && random_bit < 2
        ? 720 + random_bit  // SS1_PIL_03/04
        : -1;
}

inline constexpr std::int32_t kStartupMusicIndex = 7;
inline constexpr std::int32_t kStartupVoiceSoundId = 694; // SS1_BOB_01
inline constexpr std::int32_t kLowTimerWarningSoundId = 994; // lowtime.wav

} // namespace btb::spud_maze
