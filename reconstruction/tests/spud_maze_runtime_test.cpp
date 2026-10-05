#include "btb/spud_maze_runtime.hpp"

#include <cassert>

using namespace btb::spud_maze;

int main() {
    static_assert(
        classify_screen_transition(0x80) == ScreenTransition::LeftUpper);
    static_assert(
        classify_screen_transition(0x100) == ScreenTransition::RightUpper);
    static_assert(
        classify_screen_transition(0x400) == ScreenTransition::LeftLower);
    static_assert(
        classify_screen_transition(0x800) == ScreenTransition::RightLower);
    static_assert(
        classify_screen_transition(0) == ScreenTransition::None);

    constexpr auto left = apply_screen_transition(
        2, 7, ScreenTransition::LeftLower);
    static_assert(left.screen == 1);
    static_assert(left.node == 5);
    static_assert(left.retail_direction == 1);

    constexpr auto right = apply_screen_transition(
        2, 5, ScreenTransition::RightUpper);
    static_assert(right.screen == 3);
    static_assert(right.node == 7);
    static_assert(right.retail_direction == 2);

    for (int seed = 1; seed <= 4; ++seed) {
        const auto easy = select_repair_damage(Difficulty::Easy, seed);
        assert(easy.repairs_remaining == 4);

        const auto medium = select_repair_damage(Difficulty::Medium, seed);
        assert(medium.repairs_remaining == (seed == 4 ? 7 : 6));

        const auto hard = select_repair_damage(Difficulty::Hard, seed);
        assert(hard.repairs_remaining == ((seed == 2 || seed == 3) ? 11 : 10));
    }

    assert(activity_outcome(4, 100) == ActivityOutcome::Continue);
    assert(activity_outcome(0, 100) == ActivityOutcome::Success);
    assert(activity_outcome(-1, 100) == ActivityOutcome::Success);
    assert(activity_outcome(4, -1) == ActivityOutcome::Timeout);

    constexpr auto drop_right = make_hammer_drop(200, 300, 1, 3);
    static_assert(drop_right.screen == 3);
    static_assert(drop_right.x == 210);
    static_assert(drop_right.y == 341);
    static_assert(drop_right.collision_cooldown == 500);

    constexpr auto drop_left = make_hammer_drop(200, 300, 0, 3);
    static_assert(drop_left.x == 190);

    static_assert(can_pick_up_hammer_x(100, 119));
    static_assert(!can_pick_up_hammer_x(100, 120));

    static_assert(kStartupMusicIndex == 7);
    static_assert(kStartupVoiceSoundId == 694);
    static_assert(kLowTimerWarningSoundId == 994);
}
