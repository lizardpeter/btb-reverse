#include "btb/maze_transition.hpp"

#include <cassert>

using namespace btb::maze;

int main() {
    ScreenTransitionState east{
        Screen::West,
        Screen::Middle,
    };

    auto frame = advance_screen_transition(east);
    assert(east.phase == 2);
    assert(east.direction == TransitionDirection::TowardEast);
    assert(frame.pixel_step == 2);
    assert(frame.left_blit.screen == Screen::West);
    assert(frame.left_blit.source ==
           (TransitionRect{23,20,620,400}));
    assert(frame.left_blit.destination_x == 20);
    assert(frame.right_blit.screen == Screen::Middle);
    assert(frame.right_blit.source ==
           (TransitionRect{20,20,23,400}));
    assert(frame.right_blit.destination_x == 617);
    assert(frame.actor_offset_a == 597);
    assert(frame.actor_offset_b == -3);
    assert(frame.speed_numerator_after_frame == 11);
    assert(!frame.completed);

    int east_frames = 1;
    while (!frame.completed && east_frames < 1000) {
        frame = advance_screen_transition(east);
        ++east_frames;
    }
    assert(frame.completed);
    assert(east.phase == 0);
    assert(east.direction == TransitionDirection::None);
    assert(east.new_right >= 620);
    assert(east.speed_numerator >= 10);
    assert(east_frames < 1000);

    // Middle -> East uses the same eastward compositor.
    ScreenTransitionState farther_east{
        Screen::Middle,
        Screen::East,
    };
    frame = advance_screen_transition(farther_east);
    assert(frame.left_blit.screen == Screen::Middle);
    assert(frame.right_blit.screen == Screen::East);

    ScreenTransitionState west{
        Screen::Middle,
        Screen::West,
    };
    frame = advance_screen_transition(west);
    assert(west.phase == 2);
    assert(west.direction == TransitionDirection::TowardWest);
    assert(frame.pixel_step == 2);
    assert(frame.left_blit.screen == Screen::West);
    assert(frame.left_blit.source ==
           (TransitionRect{617,20,620,400}));
    assert(frame.left_blit.destination_x == 20);
    assert(frame.right_blit.screen == Screen::Middle);
    assert(frame.right_blit.source ==
           (TransitionRect{20,20,618,400}));
    assert(frame.right_blit.destination_x == 23);
    assert(frame.actor_offset_a == -597);
    assert(frame.actor_offset_b == 3);
    assert(frame.speed_numerator_after_frame == 11);
    assert(!frame.completed);

    int west_frames = 1;
    while (!frame.completed && west_frames < 1000) {
        frame = advance_screen_transition(west);
        ++west_frames;
    }
    assert(frame.completed);
    assert(west.phase == 0);
    assert(west.direction == TransitionDirection::None);
    assert(west.new_right <= 20);
    assert(west.speed_numerator >= 10);
    assert(west_frames < 1000);

    // East -> Middle is also westward.
    ScreenTransitionState from_east{
        Screen::East,
        Screen::Middle,
    };
    frame = advance_screen_transition(from_east);
    assert(frame.left_blit.screen == Screen::Middle);
    assert(frame.right_blit.screen == Screen::East);

    // Calling an already-finished state is stable.
    const auto frozen = west;
    frame = advance_screen_transition(west);
    assert(frame.completed);
    assert(west.phase == frozen.phase);
    assert(west.old_left == frozen.old_left);
    assert(west.new_right == frozen.new_right);
}
