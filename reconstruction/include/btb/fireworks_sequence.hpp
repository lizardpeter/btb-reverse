#pragma once

#include "btb/fireworks_layout.hpp"

#include <array>
#include <cstddef>
#include <optional>
#include <vector>

namespace btb::fireworks {

inline constexpr std::size_t kTimelineRows = 3;
inline constexpr std::size_t kTimelineColumns = 6;
inline constexpr std::size_t kTimelineSlots = kTimelineRows * kTimelineColumns;

enum class InternalState : std::int32_t {
    Editor = 0,
    EditorDragRelease = 1,
    PreviewSetup = 2,
    PreviewPlayback = 3,
    PreviewFinish = 4,
    ShowSetup = 8,
    ShowPlayback = 9,
    DeleteSelected = 13,
    CompletionMovieSetup = 14,
    CompletionMoviePlayback = 15,
    Certificate = 16,
    CompleteAndExit = 17,
};

[[nodiscard]] constexpr bool internal_state_is_retail_noop(
    std::int32_t state) noexcept {
    return state == 5 || state == 6 || state == 7 ||
           state == 10 || state == 11 || state == 12;
}

struct PlaybackEvent {
    std::size_t row{};
    std::size_t column{};
    FireworkType type{};
    std::int32_t movie_index{};
    std::int32_t x{};
    std::int32_t y{};

    [[nodiscard]] bool movie_index_in_verified_bank() const noexcept {
        return movie_index_is_in_verified_bank(movie_index);
    }
};

class Sequence {
public:
    Sequence();

    [[nodiscard]] const std::array<std::optional<FireworkType>, kTimelineSlots>&
    slots() const noexcept {
        return slots_;
    }

    [[nodiscard]] static constexpr std::size_t index(
        std::size_t row,
        std::size_t column) noexcept {
        return row * kTimelineColumns + column;
    }

    [[nodiscard]] bool in_bounds(std::size_t row, std::size_t column) const noexcept;
    [[nodiscard]] bool empty(std::size_t row, std::size_t column) const noexcept;
    [[nodiscard]] std::optional<FireworkType> at(
        std::size_t row,
        std::size_t column) const noexcept;

    // Retail CanPlace/Place helpers support per-type spans, but the verified
    // span table for all 12 shipped types is 1. This therefore tests one cell.
    [[nodiscard]] bool can_place(
        std::size_t row,
        std::size_t column,
        FireworkType type) const noexcept;

    // Mirrors the commit=false / commit=true split in 0x00411010.
    [[nodiscard]] bool validate_placement(
        std::size_t row,
        std::size_t column,
        FireworkType type) const noexcept;
    bool commit_placement(
        std::size_t row,
        std::size_t column,
        FireworkType type);

    // Editor replacement behavior removes the existing item before validating
    // the current palette selection.
    bool replace(
        std::size_t row,
        std::size_t column,
        FireworkType type);

    std::optional<FireworkType> remove(
        std::size_t row,
        std::size_t column) noexcept;

    void clear() noexcept;

    [[nodiscard]] std::size_t occupied_count() const noexcept;
    [[nodiscard]] bool full() const noexcept;

    // Retail show playback advances by column. Up to three events (one per
    // row) can launch during each timeline column.
    [[nodiscard]] std::vector<PlaybackEvent> events_for_column(
        std::size_t column) const;

private:
    std::array<std::optional<FireworkType>, kTimelineSlots> slots_{};
};

} // namespace btb::fireworks
