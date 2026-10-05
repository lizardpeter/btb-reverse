#pragma once

#include "btb/fireworks_layout.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::fireworks {

inline constexpr std::size_t kTimelineRows = 3;
inline constexpr std::size_t kTimelineColumns = 6;
inline constexpr std::size_t kTimelineSlots = kTimelineRows * kTimelineColumns;

// UpdateFireworksShowPlayback uses FileTimeDeltaCentiseconds (10 ms units)
// for the authored-show scheduler. The retail constants are 400/100/200
// ticks, so one timeline column is four seconds with rows staggered by one
// second. A separate millisecond helper is called by the function but its
// return value is discarded.
inline constexpr std::int32_t kRetailTimelineTickMs = 10;
inline constexpr std::int32_t kTimelineColumnPeriodTicks = 400;
inline constexpr std::int32_t kTimelineColumnPeriodMs =
    kTimelineColumnPeriodTicks * kRetailTimelineTickMs;
inline constexpr std::int32_t kInitialPreviousElapsedTicks = -401;

[[nodiscard]] constexpr std::int32_t retail_row_launch_offset_ticks(
    std::size_t row) noexcept {
    return row == 0 ? 0 : row == 1 ? 100 : row == 2 ? 200 : -1;
}

[[nodiscard]] constexpr std::int32_t retail_row_launch_offset_ms(
    std::size_t row) noexcept {
    const auto ticks = retail_row_launch_offset_ticks(row);
    return ticks < 0 ? -1 : ticks * kRetailTimelineTickMs;
}

[[nodiscard]] constexpr std::int32_t retail_scheduled_launch_ms(
    std::size_t row,
    std::size_t column) noexcept {
    const auto offset = retail_row_launch_offset_ms(row);
    return offset < 0 || column >= kTimelineColumns
        ? -1
        : static_cast<std::int32_t>(column) * kTimelineColumnPeriodMs + offset;
}

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
    LegacyCompleteAndExit = 17,
};

[[nodiscard]] constexpr bool internal_state_is_retail_noop(
    std::int32_t state) noexcept {
    return state == 5 || state == 6 || state == 7 ||
           state == 10 || state == 11 || state == 12;
}

// The jump table contains a state-17 handler, but exhaustive references to the
// retail state global show no write of value 17. State 16 is written directly
// by the show player; state 17 is therefore retained as dormant/legacy code.
[[nodiscard]] constexpr bool internal_state_has_direct_retail_writer(
    InternalState state) noexcept {
    return state != InternalState::LegacyCompleteAndExit;
}

struct PlaybackEvent {
    std::size_t row{};
    std::size_t column{};
    FireworkType type{};
    std::int32_t movie_index{};
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t scheduled_launch_ms{};

    [[nodiscard]] bool movie_index_in_verified_bank() const noexcept {
        return movie_index_is_in_verified_bank(movie_index);
    }
};

struct ScheduleStep {
    std::int32_t column{-1};
    std::array<bool, kTimelineRows> launch_rows{};

    [[nodiscard]] bool in_authored_timeline() const noexcept {
        return column >= 0 &&
               column < static_cast<std::int32_t>(kTimelineColumns);
    }
};

// Exact retail threshold/gate scheduler from 0x00413450.
//
// previous_elapsed_ticks starts at -401 in show setup. Crossing a 400-tick
// boundary emits row 0 and arms row 1/2. Those rows fire once the current
// window reaches remainder 100/200. If a frame skips over a threshold, retail
// fires it on the first frame observed after the threshold.
class RetailShowScheduler {
public:
    RetailShowScheduler() noexcept = default;

    void reset() noexcept;
    [[nodiscard]] ScheduleStep advance(std::int32_t elapsed_ticks) noexcept;

    [[nodiscard]] std::int32_t previous_elapsed_ticks() const noexcept {
        return previous_elapsed_ticks_;
    }

private:
    std::int32_t previous_elapsed_ticks_{kInitialPreviousElapsedTicks};
    bool row1_pending_{false};
    bool row2_pending_{false};
};

inline constexpr std::size_t kActiveEventCapacity = 40;

// State 8 clears exactly 40 records at 0x005093F0..0x0050966F. Each record is
// 16 bytes. The show loop consumes only type and row; the middle dwords are
// zeroed on allocation and otherwise left as retail-owned auxiliary state.
struct ActiveEventRecord {
    std::int32_t type{-1};
    std::int32_t auxiliary0{};
    std::int32_t auxiliary1{};
    std::int32_t row{};

    [[nodiscard]] bool free() const noexcept {
        return type == -1;
    }
};

static_assert(sizeof(ActiveEventRecord) == 16);

class ActiveEventPool {
public:
    ActiveEventPool() noexcept;

    void reset() noexcept;

    [[nodiscard]] const std::array<ActiveEventRecord, kActiveEventCapacity>&
    records() const noexcept {
        return records_;
    }

    [[nodiscard]] std::optional<std::size_t> allocate(
        FireworkType type,
        std::size_t row) noexcept;

    // Retail rewinds the completed Bink, then frees the event by changing only
    // its type field back to -1. The other fields remain stale.
    bool complete(std::size_t slot) noexcept;

    [[nodiscard]] std::size_t active_count() const noexcept;

private:
    std::array<ActiveEventRecord, kActiveEventCapacity> records_{};
};

struct LaunchedEvent {
    std::size_t pool_slot{};
    PlaybackEvent event{};
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

    // Returns all authored events in one column, independent of scheduler
    // timing. Runtime launch gating is modeled by RetailShowScheduler below.
    [[nodiscard]] std::vector<PlaybackEvent> events_for_column(
        std::size_t column) const;

private:
    std::array<std::optional<FireworkType>, kTimelineSlots> slots_{};
};

// Applies one exact scheduler step to the authored sequence and the first-free
// 40-record active-event pool. If the pool is full, retail silently drops that
// launch attempt.
[[nodiscard]] std::vector<LaunchedEvent> launch_scheduled_events(
    const Sequence& sequence,
    const ScheduleStep& step,
    ActiveEventPool& pool);

} // namespace btb::fireworks
