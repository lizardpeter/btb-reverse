#include "btb/fireworks_sequence.hpp"

#include <cassert>

using namespace btb::fireworks;

int main() {
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
    assert(events[1].scheduled_launch_ms == 100);

    assert(events[2].row == 2);
    assert(events[2].movie_index == 8);
    assert(events[2].x == 220 && events[2].y == 200);
    assert(events[2].scheduled_launch_ms == 200);

    assert(retail_scheduled_launch_ms(0, 5) == 2000);
    assert(retail_scheduled_launch_ms(1, 5) == 2100);
    assert(retail_scheduled_launch_ms(2, 5) == 2200);
    assert(retail_scheduled_launch_ms(3, 0) == -1);

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
