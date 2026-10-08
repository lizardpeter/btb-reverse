#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::maze {

// The state indices below are the eight jump-table entries at 0x0041D4AC.
enum class SpudNpcPhase : std::int32_t {
    SpawnWait = 0,              // 0x0041CD19
    SelectPackage = 1,          // 0x0041CD4A
    TravelToPackage = 2,        // 0x0041CEED
    BeginReturnPreparation = 3, // 0x0041CFE4
    ChooseReturnRoute = 4,      // 0x0041CFFA
    TravelCarryingPackage = 5,  // 0x0041D1CE
    ResetPath = 6,              // 0x0041D33D
    TravelHome = 7,             // 0x0041D3DE
};

// The leading gate in UpdateMazeSpudNPC: on a different screen, phases
// 2..7 decrement 0x5120E8 and return. When the counter becomes negative,
// retail restores the selected package to status 1 and enters phase 1.
// This early return also skips the previous-position snapshot.
struct SpudOffscreenGate {
    SpudNpcPhase next_phase{};
    std::int32_t frames_remaining{};
    bool restore_selected_package_to_available{};
    bool dispatch_state{};
    bool snapshot_position{};
};

[[nodiscard]] constexpr SpudOffscreenGate spud_offscreen_gate(
    SpudNpcPhase phase,
    std::int32_t spud_screen,
    std::int32_t player_screen,
    std::int32_t frames_remaining) noexcept {
    if (spud_screen == player_screen ||
        static_cast<std::int32_t>(phase) <= 1) {
        return {phase, frames_remaining, false, true, true};
    }
    --frames_remaining;
    if (frames_remaining >= 0) {
        return {phase, frames_remaining, false, false, false};
    }
    return {
        SpudNpcPhase::SelectPackage, frames_remaining,
        true, false, false,
    };
}

// Phase 0 tests the per-difficulty sentinel (-1), then decrements its
// delay BEFORE deciding whether to enter phase 1 at <=0.
struct SpudSpawnWait {
    SpudNpcPhase next_phase{SpudNpcPhase::SpawnWait};
    std::int32_t frames_remaining{};
    bool snapshot_position{};
};

[[nodiscard]] constexpr SpudSpawnWait advance_spud_spawn_wait(
    std::int32_t difficulty_spawn_interval,
    std::int32_t frames_remaining) noexcept {
    if (difficulty_spawn_interval == -1) {
        return {SpudNpcPhase::SpawnWait, frames_remaining, false};
    }
    --frames_remaining;
    return {
        frames_remaining > 0
            ? SpudNpcPhase::SpawnWait
            : SpudNpcPhase::SelectPackage,
        frames_remaining,
        true,
    };
}

// Four 20-byte package records per screen. The first field (+0x510A18)
// is node_id; the second (+0x510A1C) is the package status.
struct SpudPackageSlot {
    std::int32_t node_id{-1};
    std::int32_t status{};
};

inline constexpr std::int32_t kSpudAvailablePackageStatus = 1;
inline constexpr std::int32_t kSpudTakenPackageStatus = 2;
inline constexpr std::int32_t kSpudSpawnVoice = 88;
inline constexpr std::int32_t kSpudPickupVoice = 89;
inline constexpr std::int32_t kSpudHomeCooldownFrames = 500;
inline constexpr std::int32_t kSpudPickupAnimationAdvance = 7;
inline constexpr std::int32_t kSpudPathArrivalDistanceExclusive = 3;

[[nodiscard]] constexpr bool spud_package_available(
    SpudPackageSlot slot) noexcept {
    return slot.node_id >= 0 &&
        slot.status == kSpudAvailablePackageStatus;
}

[[nodiscard]] constexpr std::int32_t available_spud_package_count(
    const std::array<SpudPackageSlot,4>& packages) noexcept {
    std::int32_t count = 0;
    for (const auto& slot : packages) {
        if (spud_package_available(slot)) {
            ++count;
        }
    }
    return count;
}

// Phase 1 chooses rand() % available_count and then takes the Nth
// available slot in original 0..3 order, not rand()%4.
[[nodiscard]] constexpr std::optional<std::int32_t>
select_spud_package_by_rank(
    const std::array<SpudPackageSlot,4>& packages,
    std::int32_t rank) noexcept {
    if (rank < 0) {
        return std::nullopt;
    }
    for (std::int32_t i = 0; i < 4; ++i) {
        if (!spud_package_available(
                packages[static_cast<std::size_t>(i)])) {
            continue;
        }
        if (rank == 0) {
            return i;
        }
        --rank;
    }
    return std::nullopt;
}

// Phase 2: actor advances its current path only when distance <3.
// On completing the final node, it rechecks the selected package status.
// Status 1 -> 2, phase 4, sound 89 and animation +=7; a contested box
// goes directly to the route-reset phase 6.
struct SpudPackageArrival {
    SpudNpcPhase next_phase{SpudNpcPhase::TravelToPackage};
    std::int32_t next_path_index{};
    bool mark_package_taken{};
    bool play_pickup_voice{};
    std::int32_t animation_advance{};
};

[[nodiscard]] constexpr SpudPackageArrival advance_spud_package_route(
    std::int32_t distance_to_node,
    std::int32_t current_path_index,
    std::int32_t path_node_count,
    std::int32_t selected_package_status) noexcept {
    if (distance_to_node >= kSpudPathArrivalDistanceExclusive) {
        return {SpudNpcPhase::TravelToPackage, current_path_index};
    }
    const auto next_index = current_path_index + 1;
    if (next_index < path_node_count) {
        return {SpudNpcPhase::TravelToPackage, next_index};
    }
    if (selected_package_status == kSpudAvailablePackageStatus) {
        return {
            SpudNpcPhase::ChooseReturnRoute, next_index,
            true, true, kSpudPickupAnimationAdvance,
        };
    }
    return {SpudNpcPhase::ResetPath, next_index};
}

// Phase 7 returns to state 0 after the last route node and resets the
// cooldown to a fixed 500 ticks, regardless of the difficulty table.
struct SpudHomeArrival {
    SpudNpcPhase next_phase{SpudNpcPhase::TravelHome};
    std::int32_t next_path_index{};
    std::optional<std::int32_t> reset_spawn_countdown{};
};

[[nodiscard]] constexpr SpudHomeArrival advance_spud_home_route(
    std::int32_t distance_to_node,
    std::int32_t current_path_index,
    std::int32_t path_node_count) noexcept {
    if (distance_to_node >= kSpudPathArrivalDistanceExclusive) {
        return {SpudNpcPhase::TravelHome, current_path_index, std::nullopt};
    }
    const auto next_index = current_path_index + 1;
    if (next_index < path_node_count) {
        return {SpudNpcPhase::TravelHome, next_index, std::nullopt};
    }
    return {
        SpudNpcPhase::SpawnWait, next_index, kSpudHomeCooldownFrames,
    };
}

// The phase-3 handler zeroes 0x00512154 and enters phase 4 immediately.
[[nodiscard]] constexpr SpudNpcPhase spud_begin_return_phase() noexcept {
    return SpudNpcPhase::ChooseReturnRoute;
}

} // namespace btb::maze
