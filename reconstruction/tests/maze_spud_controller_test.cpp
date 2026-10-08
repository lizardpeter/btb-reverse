#include "btb/maze_spud_controller.hpp"

#include <array>
#include <cassert>
#include <cstdint>

using namespace btb::maze;

int main() {
    static_assert(static_cast<int>(SpudNpcPhase::SpawnWait) == 0);
    static_assert(static_cast<int>(SpudNpcPhase::TravelHome) == 7);

    // Off-screen phases 2..7 return without updating position. A countdown
    // that becomes zero is still active; reset is one frame later at -1.
    constexpr auto in_view = spud_offscreen_gate(
        SpudNpcPhase::TravelToPackage, 1, 1, 0);
    static_assert(in_view.dispatch_state && in_view.snapshot_position);
    constexpr auto selecting = spud_offscreen_gate(
        SpudNpcPhase::SelectPackage, 0, 2, 0);
    static_assert(selecting.dispatch_state && selecting.frames_remaining == 0);
    constexpr auto zero = spud_offscreen_gate(
        SpudNpcPhase::TravelCarryingPackage, 0, 2, 1);
    static_assert(!zero.dispatch_state && zero.frames_remaining == 0);
    static_assert(!zero.restore_selected_package_to_available);
    static_assert(!zero.snapshot_position);
    constexpr auto reset = spud_offscreen_gate(
        SpudNpcPhase::TravelCarryingPackage, 0, 2, 0);
    static_assert(reset.frames_remaining == -1);
    static_assert(reset.next_phase == SpudNpcPhase::SelectPackage);
    static_assert(reset.restore_selected_package_to_available);
    static_assert(!reset.snapshot_position);

    // Disabled spawn sentinel bypasses the position snapshot as well.
    constexpr auto disabled = advance_spud_spawn_wait(-1, 2);
    static_assert(disabled.frames_remaining == 2);
    static_assert(!disabled.snapshot_position);
    constexpr auto wait = advance_spud_spawn_wait(1500, 2);
    static_assert(wait.frames_remaining == 1);
    static_assert(wait.next_phase == SpudNpcPhase::SpawnWait);
    constexpr auto spawn = advance_spud_spawn_wait(1500, 1);
    static_assert(spawn.frames_remaining == 0);
    static_assert(spawn.next_phase == SpudNpcPhase::SelectPackage);
    static_assert(spawn.snapshot_position);

    // Count statuses only for records having both a valid node and status 1.
    // Rank must choose from that filtered set in stable original order.
    constexpr std::array<SpudPackageSlot,4> packages{{
        {6, 1}, {-1, 1}, {2, 2}, {10, 1},
    }};
    static_assert(available_spud_package_count(packages) == 2);
    static_assert(select_spud_package_by_rank(packages, 0).value() == 0);
    static_assert(select_spud_package_by_rank(packages, 1).value() == 3);
    static_assert(!select_spud_package_by_rank(packages, -1).has_value());
    static_assert(!select_spud_package_by_rank(packages, 2).has_value());
    static_assert(available_spud_package_count({{
        {-1, 1}, {10, 2}, {-1, 0}, {4, 3},
    }}) == 0);

    // Phase 2 uses distance<3 and re-checks package status at route end.
    constexpr auto far = advance_spud_package_route(3, 0, 2, 1);
    static_assert(far.next_path_index == 0);
    constexpr auto intermediate = advance_spud_package_route(2, 0, 2, 1);
    static_assert(intermediate.next_path_index == 1);
    static_assert(intermediate.next_phase == SpudNpcPhase::TravelToPackage);
    constexpr auto pickup = advance_spud_package_route(2, 1, 2, 1);
    static_assert(pickup.next_phase == SpudNpcPhase::ChooseReturnRoute);
    static_assert(pickup.mark_package_taken && pickup.play_pickup_voice);
    static_assert(pickup.animation_advance == 7);
    constexpr auto contested = advance_spud_package_route(2, 1, 2, 2);
    static_assert(contested.next_phase == SpudNpcPhase::ResetPath);
    static_assert(!contested.mark_package_taken);

    // Phase 7 returns to phase 0 with exactly 500 countdown ticks.
    constexpr auto home_far = advance_spud_home_route(3, 1, 2);
    static_assert(home_far.next_path_index == 1);
    static_assert(!home_far.reset_spawn_countdown.has_value());
    constexpr auto home = advance_spud_home_route(2, 1, 2);
    static_assert(home.next_phase == SpudNpcPhase::SpawnWait);
    static_assert(home.reset_spawn_countdown.value() == 500);
    static_assert(spud_begin_return_phase() ==
                  SpudNpcPhase::ChooseReturnRoute);

    assert(available_spud_package_count(packages) == 2);
    assert(advance_spud_package_route(2, 1, 2, 1).mark_package_taken);
}
