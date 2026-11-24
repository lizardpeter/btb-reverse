#include "btb/maze_controller.hpp"

#include <cassert>

using namespace btb::maze;

int main() {
    static_assert(classify_end_condition(1,100,0) == EndCondition::None);
    static_assert(classify_end_condition(0,100,0) == EndCondition::Success);
    static_assert(classify_end_condition(-1,100,0) == EndCondition::Success);
    static_assert(classify_end_condition(3,-1,0) == EndCondition::Timeout);

    // A screen slide suppresses both success and timeout until it is complete.
    static_assert(classify_end_condition(0,100,1) == EndCondition::None);
    static_assert(classify_end_condition(3,-1,2) == EndCondition::None);

    static_assert(!should_play_low_time_warning(16));
    static_assert(should_play_low_time_warning(15));
    static_assert(!should_play_low_time_warning(14));
    static_assert(kLowTimeWarningSoundId == 83);

    constexpr auto fast =
        begin_outcome_feedback(EndCondition::Success,16);
    static_assert(fast.has_value());
    static_assert(fast->sound_id == 84);
    static_assert(fast->priority == 90);
    static_assert(fast->arbitration_class == 1);
    static_assert(fast->draw_activity);
    static_assert(fast->next_outcome_phase == 1);

    constexpr auto boundary =
        begin_outcome_feedback(EndCondition::Success,15);
    static_assert(boundary.has_value());
    static_assert(boundary->sound_id == 85);

    constexpr auto late =
        begin_outcome_feedback(EndCondition::Success,-5);
    static_assert(late.has_value());
    static_assert(late->sound_id == 85);

    constexpr auto timeout =
        begin_outcome_feedback(EndCondition::Timeout,-1);
    static_assert(timeout.has_value());
    static_assert(timeout->sound_id == 86);

    constexpr auto none =
        begin_outcome_feedback(EndCondition::None,100);
    static_assert(!none.has_value());

    constexpr auto wait = outcome_phase_one(true);
    static_assert(wait.draw_activity);
    static_assert(wait.wait_for_managed_feedback);
    static_assert(!wait.advance_phase);

    constexpr auto advance = outcome_phase_one(false);
    static_assert(advance.draw_activity);
    static_assert(!advance.wait_for_managed_feedback);
    static_assert(advance.advance_phase);

    static_assert(success_transition_code(0) == -1);
    static_assert(success_transition_code(1) == 3);
    static_assert(success_transition_code(2) == -1);
    static_assert(success_transition_code(-1) == -1);

    static_assert(kPlayAgainPlan.prepare_play_again);
    static_assert(kPlayAgainPlan.remaining_seconds == 999);
    static_assert(kPlayAgainPlan.outer_state == 0x3C);
    static_assert(kPlayAgainPlan.ui_context == 0x20);
    static_assert(kPlayAgainPlan.unload_maze_resources);
    static_assert(kPlayAgainPlan.set_shared_transition_flag);

    constexpr auto easy = startup_voice(0);
    constexpr auto medium = startup_voice(1);
    constexpr auto hard = startup_voice(2);
    static_assert(easy.sound_id == 73);
    static_assert(medium.sound_id == 74);
    static_assert(hard.sound_id == 75);
    static_assert(hard.priority == 90);
    static_assert(hard.arbitration_class == 1);
    static_assert(hard.shared_sound_group == 5);
    static_assert(hard.mark_input_interruptible);

    constexpr auto forced = leave_plan(false,-1);
    static_assert(forced.unload_maze_resources);
    static_assert(!forced.clear_leave_request);
    static_assert(!forced.outer_state.has_value());

    constexpr auto leave = leave_plan(true,-1);
    static_assert(leave.clear_leave_request);
    static_assert(leave.outer_state == 0x1C);

    constexpr auto movie = leave_plan(false,3);
    static_assert(!movie.clear_leave_request);
    static_assert(movie.outer_state == 0x40);

    constexpr auto leave_movie = leave_plan(true,3);
    static_assert(leave_movie.clear_leave_request);
    static_assert(leave_movie.outer_state == 0x40);
}
