#include "btb/fireworks_sequence.hpp"

#include <stdexcept>

namespace btb::fireworks {

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

        PlaybackEvent event;
        event.row = row;
        event.column = column;
        event.type = *type;
        event.movie_index = retail_movie_index_for_row(*type, row);
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

        events.push_back(event);
    }

    return events;
}

} // namespace btb::fireworks
