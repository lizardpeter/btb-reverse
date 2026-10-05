#include "btb/bobs_band_runtime.hpp"

namespace btb::bobs_band {

SequencerGrid::SequencerGrid() noexcept {
    clear();
}

std::int32_t SequencerGrid::at(
    std::size_t row,
    std::size_t column) const noexcept {
    if (row >= kPitchRows || column >= kTimelineColumns) {
        return kEmptyCell;
    }
    return cells_[row][column];
}

bool SequencerGrid::can_place(
    std::size_t row,
    std::size_t column,
    std::int32_t type) const noexcept {

    if (row >= kPitchRows ||
        column >= kTimelineColumns ||
        type < 0 ||
        type >= static_cast<std::int32_t>(kSoundTypeCount)) {
        return false;
    }

    const auto span = sound_span(type);
    if (column + static_cast<std::size_t>(span) > kTimelineColumns) {
        return false;
    }

    for (std::int32_t i = 0; i < span; ++i) {
        if (cells_[row][column + static_cast<std::size_t>(i)]
            != kEmptyCell) {
            return false;
        }
    }

    return true;
}

bool SequencerGrid::place(
    std::size_t row,
    std::size_t column,
    std::int32_t type) noexcept {

    if (!can_place(row, column, type)) {
        return false;
    }

    cells_[row][column] = type;
    for (std::int32_t i = 1; i < sound_span(type); ++i) {
        cells_[row][column + static_cast<std::size_t>(i)] =
            kContinuationCell;
    }

    return true;
}

std::int32_t SequencerGrid::remove_at(
    std::size_t row,
    std::size_t column) noexcept {

    if (row >= kPitchRows || column >= kTimelineColumns) {
        return kEmptyCell;
    }

    auto base_column = column;
    while (cells_[row][base_column] == kContinuationCell) {
        if (base_column == 0) {
            return kEmptyCell;
        }
        --base_column;
    }

    const auto type = cells_[row][base_column];
    if (type < 0 ||
        type >= static_cast<std::int32_t>(kSoundTypeCount)) {
        return kEmptyCell;
    }

    const auto span = sound_span(type);
    for (std::int32_t i = 0; i < span; ++i) {
        const auto c = base_column + static_cast<std::size_t>(i);
        if (c < kTimelineColumns) {
            cells_[row][c] = kEmptyCell;
        }
    }

    return type;
}

void SequencerGrid::clear() noexcept {
    for (auto& row : cells_) {
        row.fill(kEmptyCell);
    }
}

} // namespace btb::bobs_band
