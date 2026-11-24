#include "btb/maze_transition.hpp"

#include <algorithm>

namespace btb::maze {
namespace {

[[nodiscard]] TransitionDirection transition_direction(
    Screen previous,
    Screen destination) noexcept {
    return static_cast<std::int32_t>(destination) >
           static_cast<std::int32_t>(previous)
        ? TransitionDirection::TowardEast
        : TransitionDirection::TowardWest;
}

void initialize(ScreenTransitionState& state) noexcept {
    state.phase = 2;
    state.direction =
        transition_direction(
            state.previous_screen,
            state.destination_screen);
    state.speed_numerator = 10;

    if (state.direction == TransitionDirection::TowardEast) {
        // 0x0041C7F4 path.
        state.old_left = 21;
        state.new_right = 21;
    } else {
        // 0x0041C7A3 path.
        state.old_left = 619;
        state.new_right = 620;
    }
}

[[nodiscard]] std::int32_t step_from_numerator(
    std::int32_t numerator) noexcept {
    // The executable uses the 0x66666667 signed-multiply idiom for /5.
    return numerator / 5;
}

void update_speed(
    ScreenTransitionState& state,
    std::int32_t remaining) noexcept {

    if (state.speed_numerator <
        kTransitionMaximumSpeedNumerator) {
        ++state.speed_numerator;
    }

    if (state.speed_numerator > remaining) {
        state.speed_numerator = remaining;
    }

    if (state.speed_numerator < 10) {
        state.speed_numerator = 10;
    }
}

} // namespace

ScreenTransitionFrame advance_screen_transition(
    ScreenTransitionState& state) noexcept {

    if (state.phase == 1) {
        initialize(state);
    }

    ScreenTransitionFrame frame;

    if (state.phase == 0) {
        frame.completed = true;
        frame.speed_numerator_after_frame =
            state.speed_numerator;
        frame.actor_offset_a = state.actor_offset_a;
        frame.actor_offset_b = state.actor_offset_b;
        return frame;
    }

    const auto step =
        step_from_numerator(state.speed_numerator);
    frame.pixel_step = step;

    if (state.direction == TransitionDirection::TowardEast) {
        state.old_left += step;
        state.new_right += step;

        state.actor_offset_a =
            kTransitionRight - state.new_right;
        state.actor_offset_b =
            kTransitionLeft - state.old_left;

        frame.left_blit = {
            state.previous_screen,
            {
                state.old_left,
                kTransitionTop,
                kTransitionRight,
                kTransitionBottom,
            },
            kTransitionLeft,
            kTransitionTop,
        };

        frame.right_blit = {
            state.destination_screen,
            {
                kTransitionLeft,
                kTransitionTop,
                state.new_right,
                kTransitionBottom,
            },
            (kTransitionRight - state.old_left) +
                kTransitionLeft,
            kTransitionTop,
        };

        const auto remaining =
            kTransitionRight - state.new_right;
        if (state.new_right >= kTransitionRight) {
            state.phase = 0;
            state.direction = TransitionDirection::None;
            frame.completed = true;
        }

        update_speed(state, remaining);
    } else {
        state.old_left -= step;
        state.new_right -= step;

        state.actor_offset_a =
            kTransitionLeft - state.old_left;
        state.actor_offset_b =
            kTransitionRight - state.old_left;

        // For westward travel the newly entered screen owns the left strip,
        // while the previous screen is shifted to the right.
        frame.left_blit = {
            state.destination_screen,
            {
                state.old_left,
                kTransitionTop,
                kTransitionRight,
                kTransitionBottom,
            },
            kTransitionLeft,
            kTransitionTop,
        };

        frame.right_blit = {
            state.previous_screen,
            {
                kTransitionLeft,
                kTransitionTop,
                state.new_right,
                kTransitionBottom,
            },
            (kTransitionRight - state.old_left) +
                kTransitionLeft,
            kTransitionTop,
        };

        const auto remaining =
            state.new_right - kTransitionLeft;
        if (state.new_right <= kTransitionLeft) {
            state.phase = 0;
            state.direction = TransitionDirection::None;
            frame.completed = true;
        }

        update_speed(state, remaining);
    }

    frame.speed_numerator_after_frame =
        state.speed_numerator;
    frame.actor_offset_a = state.actor_offset_a;
    frame.actor_offset_b = state.actor_offset_b;
    return frame;
}

} // namespace btb::maze
