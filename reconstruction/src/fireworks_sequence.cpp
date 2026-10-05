#include "btb/fireworks_sequence.hpp"

#include <stdexcept>

namespace btb::fireworks {
namespace {

[[nodiscard]] PlaybackEvent make_playback_event(
    std::size_t row,
    std::size_t column,
    FireworkType type) {
    PlaybackEvent event;
    event.row = row;
    event.column = column;
    event.type = type;
    event.movie_index = retail_movie_index_for_row(type, row);
    event.scheduled_launch_ms = retail_scheduled_launch_ms(row, column);

    // Exact row-specific playback placement recovered from 0x00413450.
    if (row == 0) {
        event.x = 0;
        event.y = 0;
    } else if (row == 1) {
        event.x = 440;
        event.y = 0;
    } else {
        event.x = 220;
        event.y = 200;
    }

    return event;
}

} // namespace

void RetailShowScheduler::reset() noexcept {
    previous_elapsed_ticks_ = kInitialPreviousElapsedTicks;
    row1_pending_ = false;
    row2_pending_ = false;
}

ScheduleStep RetailShowScheduler::advance(std::int32_t elapsed_ticks) noexcept {
    ScheduleStep step;

    const auto previous_column =
        previous_elapsed_ticks_ / kTimelineColumnPeriodTicks;
    const auto current_column = elapsed_ticks / kTimelineColumnPeriodTicks;
    step.column = current_column;

    if (previous_column != current_column) {
        // Retail has a broader <20 guard here even though the authored grid
        // contains only six columns.
        if (current_column < 20) {
            step.launch_rows[0] = true;
        }
        row1_pending_ = true;
        row2_pending_ = true;
    }

    const auto remainder = elapsed_ticks % kTimelineColumnPeriodTicks;

    if (row1_pending_ && remainder >= retail_row_launch_offset_ticks(1)) {
        row1_pending_ = false;
        step.launch_rows[1] = true;
    }

    if (row2_pending_ && remainder >= retail_row_launch_offset_ticks(2)) {
        row2_pending_ = false;
        step.launch_rows[2] = true;
    }

    previous_elapsed_ticks_ = elapsed_ticks;
    return step;
}

ActiveEventPool::ActiveEventPool() noexcept {
    reset();
}

void ActiveEventPool::reset() noexcept {
    for (auto& record : records_) {
        record = ActiveEventRecord{};
    }
}

std::optional<std::size_t> ActiveEventPool::allocate(
    FireworkType type,
    std::size_t row) noexcept {
    const auto type_id = static_cast<std::int32_t>(type);
    if (type_id < 0 || type_id > 11 || row >= kTimelineRows) {
        return std::nullopt;
    }

    for (std::size_t slot = 0; slot < records_.size(); ++slot) {
        auto& record = records_[slot];
        if (!record.free()) {
            continue;
        }

        record.type = type_id;
        record.auxiliary0 = 0;
        record.auxiliary1 = 0;
        record.row = static_cast<std::int32_t>(row);
        return slot;
    }

    return std::nullopt;
}

bool ActiveEventPool::complete(std::size_t slot) noexcept {
    if (slot >= records_.size() || records_[slot].free()) {
        return false;
    }

    // 0x004137AA changes only +0x00 back to -1 after rewinding the Bink.
    records_[slot].type = -1;
    return true;
}

std::size_t ActiveEventPool::active_count() const noexcept {
    std::size_t count = 0;
    for (const auto& record : records_) {
        if (!record.free()) {
            ++count;
        }
    }
    return count;
}

Sequence::Sequence() {
    clear();
}

bool Sequence::in_bounds(std::size_t row, std::size_t column) const noexcept {
    return row < kTimelineRows && column < kTimelineColumns;
}

bool Sequence::empty(std::size_t row, std::size_t column) const noexcept {
    return in_bounds(row, column) && !slots_[index(row, column)].has_value();
}

std::optional<FireworkType> Sequence::at(
    std::size_t row,
    std::size_t column) const noexcept {
    if (!in_bounds(row, column)) {
        return std::nullopt;
    }
    return slots_[index(row, column)];
}

bool Sequence::can_place(
    std::size_t row,
    std::size_t column,
    FireworkType /*type*/) const noexcept {
    return empty(row, column);
}

bool Sequence::validate_placement(
    std::size_t row,
    std::size_t column,
    FireworkType type) const noexcept {
    return can_place(row, column, type);
}

bool Sequence::commit_placement(
    std::size_t row,
    std::size_t column,
    FireworkType type) {
    if (!can_place(row, column, type)) {
        return false;
    }
    slots_[index(row, column)] = type;
    return true;
}

bool Sequence::replace(
    std::size_t row,
    std::size_t column,
    FireworkType type) {
    if (!in_bounds(row, column)) {
        return false;
    }

    // The editor's placement-region handler removes an existing type before
    // the release/commit path validates the current selection.
    slots_[index(row, column)].reset();
    return commit_placement(row, column, type);
}

std::optional<FireworkType> Sequence::remove(
    std::size_t row,
    std::size_t column) noexcept {
    if (!in_bounds(row, column)) {
        return std::nullopt;
    }
    auto& slot = slots_[index(row, column)];
    const auto old = slot;
    slot.reset();
    return old;
}

void Sequence::clear() noexcept {
    for (auto& slot : slots_) {
        slot.reset();
    }
}

std::size_t Sequence::occupied_count() const noexcept {
    std::size_t count = 0;
    for (const auto& slot : slots_) {
        if (slot) {
            ++count;
        }
    }
    return count;
}

bool Sequence::full() const noexcept {
    return occupied_count() == kTimelineSlots;
}

std::vector<PlaybackEvent> Sequence::events_for_column(
    std::size_t column) const {
    if (column >= kTimelineColumns) {
        throw std::out_of_range("Fireworks timeline column");
    }

    std::vector<PlaybackEvent> events;
    events.reserve(kTimelineRows);

    for (std::size_t row = 0; row < kTimelineRows; ++row) {
        const auto type = at(row, column);
        if (!type) {
            continue;
        }
        events.push_back(make_playback_event(row, column, *type));
    }

    return events;
}

std::vector<LaunchedEvent> launch_scheduled_events(
    const Sequence& sequence,
    const ScheduleStep& step,
    ActiveEventPool& pool) {
    std::vector<LaunchedEvent> launched;
    if (!step.in_authored_timeline()) {
        return launched;
    }

    const auto column = static_cast<std::size_t>(step.column);
    launched.reserve(kTimelineRows);

    for (std::size_t row = 0; row < kTimelineRows; ++row) {
        if (!step.launch_rows[row]) {
            continue;
        }

        const auto type = sequence.at(row, column);
        if (!type) {
            continue;
        }

        const auto type_id = static_cast<std::int32_t>(*type);
        if (type_id < 0 || type_id > 11) {
            continue;
        }

        const auto pool_slot = pool.allocate(*type, row);
        if (!pool_slot) {
            continue;
        }

        launched.push_back(
            LaunchedEvent{*pool_slot, make_playback_event(row, column, *type)});
    }

    return launched;
}

} // namespace btb::fireworks
