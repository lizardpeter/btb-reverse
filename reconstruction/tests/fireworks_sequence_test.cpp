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

    static_assert(kRetailTimelineTickMs == 10);
    static_assert(kTimelineColumnPeriodTicks == 400);
    static_assert(kTimelineColumnPeriodMs == 4000);
    static_assert(kActiveEventCapacity == 40);
    static_assert(sizeof(ActiveEventRecord) == 16);

    Sequence sequence;
    assert(sequence.occupied_count() == 0);

    assert(sequence.validate_placement(0, 0, FireworkType::RedAirbomb));
    assert(sequence.commit_placement(0, 0, FireworkType::RedAirbomb));
    assert(!sequence.validate_placement(0, 0, FireworkType::LargeBlue));

    assert(sequence.commit_placement(1, 0, FireworkType::SmallGreen));
    assert(sequence.commit_placement(2, 0, FireworkType::RedCandle));

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
