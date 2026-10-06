#include "btb/fireworks_sequence.hpp"

#include <cassert>

using namespace btb::fireworks;

int main() {
    static_assert(static_cast<int>(InternalState::Certificate) == 16);
    static_assert(static_cast<int>(InternalState::LegacyCompleteAndExit) == 17);
    static_assert(internal_state_has_direct_retail_writer(
        InternalState::Certificate));
    static_assert(!internal_state_has_direct_retail_writer(
        InternalState::LegacyCompleteAndExit));

    constexpr auto no_external_exit =
        external_exit_decision(false, false);
    static_assert(!no_external_exit.exit_activity);

    constexpr auto leave_exit =
        external_exit_decision(false, true);
    static_assert(leave_exit.exit_activity);
    static_assert(leave_exit.save_and_unload);
    static_assert(leave_exit.clear_leave_activity_request);
    static_assert(
        leave_exit.outer_state &&
        *leave_exit.outer_state == kActivitySelectOuterState);

    constexpr auto app_quit_exit =
        external_exit_decision(true, false);
    static_assert(app_quit_exit.exit_activity);
    static_assert(app_quit_exit.save_and_unload);
    static_assert(!app_quit_exit.clear_leave_activity_request);
    static_assert(!app_quit_exit.outer_state);

    static_assert(kRetailTimelineTickMs == 10);
    static_assert(kTimelineColumnPeriodTicks == 400);
    static_assert(kTimelineColumnPeriodMs == 4000);
    static_assert(kActiveEventCapacity == 40);
    static_assert(sizeof(ActiveEventRecord) == 16);

    static_assert(static_cast<int>(InternalState::PreShowMovieSetup) == 14);
    static_assert(static_cast<int>(InternalState::PreShowMoviePlayback) == 15);
    static_assert(should_open_pre_show_movie(true));
    static_assert(!should_open_pre_show_movie(false));

    constexpr auto pre_show_done = pre_show_movie_completion_action();
    static_assert(pre_show_done.next_state == InternalState::ShowSetup);
    static_assert(pre_show_done.enable_input);
    static_assert(pre_show_done.stop_all_managed_sounds);
    static_assert(pre_show_done.sound_id == 142);
    static_assert(pre_show_done.sound_priority == 90);
    static_assert(pre_show_done.arbitration_class == 1);
    static_assert(pre_show_done.mark_sound_slot_input_interruptible);

    static_assert(kTopMiddleMovieIndex == 20);
    static_assert(kCrowdLoopMovieIndex == 21);
    static_assert(kCrowdEndMovieIndex == 22);
    static_assert(kCrowdLoopCompletions == 3);
    static_assert(kCrowdTerminalPhase == 7);
    static_assert(kCertificateEarliestColumn == 2);

    constexpr auto crowd0 = advance_crowd_phase(0, false, 0);
    static_assert(crowd0.movie_index == kCrowdLoopMovieIndex);
    static_assert(crowd0.phase_after == 0);
    static_assert(crowd0.random_sound_window);
    static_assert(!crowd0.ensure_end_sound);
    static_assert(!crowd0.enter_certificate);

    constexpr auto crowd0_done = advance_crowd_phase(0, true, 0);
    static_assert(crowd0_done.phase_after == 1);
    static_assert(crowd0_done.restart_movie);

    constexpr auto crowd2_done = advance_crowd_phase(2, true, 8);
    static_assert(crowd2_done.phase_after == 3);
    static_assert(crowd2_done.movie_index == kCrowdLoopMovieIndex);
    static_assert(crowd2_done.random_sound_window);
    static_assert(!crowd2_done.ensure_end_sound);
    static_assert(!crowd2_done.enter_certificate);

    constexpr auto crowd3 = advance_crowd_phase(3, false, 1);
    static_assert(crowd3.movie_index == kCrowdEndMovieIndex);
    static_assert(crowd3.ensure_end_sound);
    static_assert(!crowd3.restart_movie);
    static_assert(!crowd3.enter_certificate);

    constexpr auto crowd_end_too_early = advance_crowd_phase(3, true, 1);
    static_assert(crowd_end_too_early.phase_after == 7);
    static_assert(!crowd_end_too_early.enter_certificate);

    constexpr auto crowd_end_ready = advance_crowd_phase(3, true, 2);
    static_assert(crowd_end_ready.phase_after == 7);
    static_assert(crowd_end_ready.enter_certificate);

    constexpr auto random_crowd =
        random_crowd_sound_id(true, false, 0, 24);
    static_assert(random_crowd && *random_crowd == 347);
    static_assert(!random_crowd_sound_id(true, true, 0, 0));
    static_assert(!random_crowd_sound_id(true, false, 1, 0));
    static_assert(!random_crowd_sound_id(false, false, 0, 0));

    Sequence sequence;
    assert(sequence.occupied_count() == 0);

    assert(sequence.validate_placement(0, 0, FireworkType::RedAirbomb));
    assert(sequence.commit_placement(0, 0, FireworkType::RedAirbomb));
    assert(!sequence.validate_placement(0, 0, FireworkType::LargeBlue));

    assert(sequence.commit_placement(1, 0, FireworkType::SmallGreen));
    assert(sequence.commit_placement(2, 0, FireworkType::RedCandle));

    // Full state-9 frame ordering: launch row 0 at t=0, row 1 at +1s,
    // process a completed old slot before allocating the +2s row into the
    // newly freed first slot.
    RetailShowRuntime show_runtime;

    ShowFrameInput frame0;
    frame0.elapsed_ticks = 0;
    frame0.top_middle_movie_finished = true;
    auto show0 = show_runtime.update(sequence, frame0);
    assert(show0.timeline_column == 0);
    assert(show0.restart_top_middle_movie);
    assert(show0.launched_events.size() == 1);
    assert(show0.launched_events[0].pool_slot == 0);
    assert(show0.launched_events[0].event.row == 0);

    ShowFrameInput frame1;
    frame1.elapsed_ticks = 100;
    auto show1 = show_runtime.update(sequence, frame1);
    assert(show1.launched_events.size() == 1);
    assert(show1.launched_events[0].pool_slot == 1);
    assert(show1.launched_events[0].event.row == 1);

    ShowFrameInput frame2;
    frame2.elapsed_ticks = 200;
    frame2.active_event_movie_finished[0] = true;
    auto show2 = show_runtime.update(sequence, frame2);
    assert(show2.completed_event_slots.size() == 1);
    assert(show2.completed_event_slots[0] == 0);
    assert(show2.launched_events.size() == 1);
    assert(show2.launched_events[0].pool_slot == 0);
    assert(show2.launched_events[0].event.row == 2);

    // Three completed crowd-loop passes advance phase 0 -> 1 -> 2 -> 3.
    // The third completion still belongs to the loop branch; crowd-end audio
    // and movie behavior begins on the following frame.
    RetailShowRuntime finale_runtime;
    Sequence empty_show;

    ShowFrameInput crowd_frame;
    crowd_frame.crowd_movie_finished = true;
    crowd_frame.elapsed_ticks = 0;
    auto crowd_out = finale_runtime.update(empty_show, crowd_frame);
    assert(finale_runtime.crowd_phase() == 1);
    assert(crowd_out.crowd.restart_movie);
    assert(!crowd_out.crowd.ensure_end_sound);

    crowd_frame.elapsed_ticks = 1;
    crowd_out = finale_runtime.update(empty_show, crowd_frame);
    assert(finale_runtime.crowd_phase() == 2);

    crowd_frame.elapsed_ticks = 2;
    crowd_out = finale_runtime.update(empty_show, crowd_frame);
    assert(finale_runtime.crowd_phase() == 3);
    assert(crowd_out.crowd.movie_index == kCrowdLoopMovieIndex);
    assert(!crowd_out.crowd.ensure_end_sound);

    // At >=8 seconds, completing crowdend.bik starts/ensures sound 349 first,
    // then the certificate early-return immediately stops all managed sounds.
    ShowFrameInput end_frame;
    end_frame.elapsed_ticks = 800;
    end_frame.crowd_movie_finished = true;
    end_frame.crowd_end_sound_playing = false;
    auto end_out = finale_runtime.update(empty_show, end_frame);
    assert(finale_runtime.crowd_phase() == 7);
    assert(end_out.crowd.movie_index == kCrowdEndMovieIndex);
    assert(end_out.stop_all_before_sound);
    assert(end_out.sound_id && *end_out.sound_id == 349);
    assert(end_out.sound_priority == 50);
    assert(end_out.sound_arbitration_class == 1);
    assert(end_out.stop_all_for_certificate);
    assert(end_out.enter_certificate);
    assert(end_out.launched_events.empty());

    auto events = sequence.events_for_column(0);
    assert(events.size() == 3);

    assert(events[0].row == 0);
    assert(events[0].movie_index == 0);
    assert(events[0].x == 0 && events[0].y == 0);
    assert(events[0].scheduled_launch_ms == 0);

    assert(events[1].row == 1);
    assert(events[1].movie_index == 13);
    assert(events[1].x == 440 && events[1].y == 0);
    assert(events[1].scheduled_launch_ms == 1000);

    assert(events[2].row == 2);
    assert(events[2].movie_index == 8);
    assert(events[2].x == 220 && events[2].y == 200);
    assert(events[2].scheduled_launch_ms == 2000);

    assert(retail_scheduled_launch_ms(0, 5) == 20000);
    assert(retail_scheduled_launch_ms(1, 5) == 21000);
    assert(retail_scheduled_launch_ms(2, 5) == 22000);
    assert(retail_scheduled_launch_ms(3, 0) == -1);

    // Exact retail threshold/gate scheduling in 10 ms units.
    RetailShowScheduler scheduler;

    auto step = scheduler.advance(0);
    assert(step.column == 0);
    assert(step.launch_rows[0]);
    assert(!step.launch_rows[1]);
    assert(!step.launch_rows[2]);

    step = scheduler.advance(99);
    assert(step.column == 0);
    assert(!step.launch_rows[0]);
    assert(!step.launch_rows[1]);
    assert(!step.launch_rows[2]);

    step = scheduler.advance(100);
    assert(step.column == 0);
    assert(!step.launch_rows[0]);
    assert(step.launch_rows[1]);
    assert(!step.launch_rows[2]);

    step = scheduler.advance(200);
    assert(step.column == 0);
    assert(!step.launch_rows[0]);
    assert(!step.launch_rows[1]);
    assert(step.launch_rows[2]);

    step = scheduler.advance(400);
    assert(step.column == 1);
    assert(step.launch_rows[0]);
    assert(!step.launch_rows[1]);
    assert(!step.launch_rows[2]);

    // If a frame crosses a column boundary after the +1 s threshold, retail
    // emits row 0 and row 1 together on that observed frame.
    scheduler.reset();
    step = scheduler.advance(550);
    assert(step.column == 1);
    assert(step.launch_rows[0]);
    assert(step.launch_rows[1]);
    assert(!step.launch_rows[2]);

    step = scheduler.advance(650);
    assert(step.column == 1);
    assert(!step.launch_rows[0]);
    assert(!step.launch_rows[1]);
    assert(step.launch_rows[2]);

    // Apply the exact scheduler to the first-free 40-record event pool.
    scheduler.reset();
    ActiveEventPool pool;

    step = scheduler.advance(0);
    auto launched = launch_scheduled_events(sequence, step, pool);
    assert(launched.size() == 1);
    assert(launched[0].pool_slot == 0);
    assert(launched[0].event.row == 0);
    assert(launched[0].event.movie_index == 0);

    step = scheduler.advance(100);
    launched = launch_scheduled_events(sequence, step, pool);
    assert(launched.size() == 1);
    assert(launched[0].pool_slot == 1);
    assert(launched[0].event.row == 1);
    assert(launched[0].event.movie_index == 13);

    step = scheduler.advance(200);
    launched = launch_scheduled_events(sequence, step, pool);
    assert(launched.size() == 1);
    assert(launched[0].pool_slot == 2);
    assert(launched[0].event.row == 2);
    assert(launched[0].event.movie_index == 8);
    assert(pool.active_count() == 3);

    assert(pool.complete(1));
    assert(pool.records()[1].type == -1);
    assert(pool.records()[1].row == 1); // stale fields are retained by retail
    assert(pool.active_count() == 2);

    const auto reused = pool.allocate(FireworkType::LargeBlue, 2);
    assert(reused && *reused == 1);
    assert(pool.active_count() == 3);

    ActiveEventPool full_pool;
    for (std::size_t i = 0; i < kActiveEventCapacity; ++i) {
        const auto slot = full_pool.allocate(FireworkType::SmallBlue, i % 3);
        assert(slot && *slot == i);
    }
    assert(!full_pool.allocate(FireworkType::RedAirbomb, 0));
    assert(full_pool.active_count() == kActiveEventCapacity);

    // Preserve the retail row-1 arithmetic even when it selects a special
    // bank entry rather than a corresponding right-facing type.
    assert(sequence.replace(1, 0, FireworkType::RedCandle));
    events = sequence.events_for_column(0);
    assert(events[1].movie_index == 20);
    assert(events[1].movie_index_in_verified_bank());

    assert(sequence.replace(1, 0, FireworkType::BlueCandle));
    events = sequence.events_for_column(0);
    assert(events[1].movie_index == 23);
    assert(!events[1].movie_index_in_verified_bank());

    const auto removed = sequence.remove(0, 0);
    assert(removed == FireworkType::RedAirbomb);
    assert(sequence.empty(0, 0));

    sequence.clear();
    assert(sequence.occupied_count() == 0);

    for (std::size_t row = 0; row < kTimelineRows; ++row) {
        for (std::size_t column = 0; column < kTimelineColumns; ++column) {
            assert(sequence.commit_placement(row, column, FireworkType::SmallBlue));
        }
    }
    assert(sequence.full());
}
