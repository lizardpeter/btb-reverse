#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string_view>

namespace btb::bobs_band {

inline constexpr std::size_t kPitchRows = 5;
inline constexpr std::size_t kTimelineColumns = 24;
inline constexpr std::size_t kSoundTypeCount = 10;
inline constexpr std::int32_t kEmptyCell = -1;
inline constexpr std::int32_t kContinuationCell = 10;

enum class Machine : std::int32_t {
    Roley = 0,
    Muck = 1,
    Lofty = 2,
    Dizzy = 3,
    Scoop = 4,
};

enum class Duration : std::int32_t {
    Short = 0,
    Long = 1,
};

enum class Conductor : std::int32_t {
    Bob = 0,
    Wendy = 1,
    FarmerPickles = 2,
};

inline constexpr std::array<std::string_view, kSoundTypeCount>
    kSoundFileStems{{
        "roley1", "roley2",
        "muck1", "muck2",
        "lofty1", "lofty2",
        "dizzy1", "dizzy2",
        "scoop1", "scoop2",
    }};

inline constexpr std::array<std::string_view, 3>
    kConductorBackingTracks{{
        "bobmt.wav",
        "Wendymt.wav",
        "fpmt.wav",
    }};

enum class ToolbarControl : std::int32_t {
    Play = 0,
    Stop = 1,
    ClearAll = 2,
    Delete = 3,
};

enum class ActivityState : std::int32_t {
    EditIdle = 0,
    BrickSelected = 1,
    DrawOnly = 2,
    Unknown3 = 3,
    Unknown4 = 4,
    Unknown5 = 5,
    Unknown6 = 6,
    Unknown7 = 7,
    BeginPlayback = 8,
    Playback = 9,
    ExitToPlayAgain = 10,
};

[[nodiscard]] constexpr std::int32_t sound_type(
    Machine machine,
    Duration duration) noexcept {
    return static_cast<std::int32_t>(machine) * 2
         + static_cast<std::int32_t>(duration);
}

[[nodiscard]] constexpr Machine machine_for_sound_type(
    std::int32_t type) noexcept {
    return static_cast<Machine>(type / 2);
}

[[nodiscard]] constexpr Duration duration_for_sound_type(
    std::int32_t type) noexcept {
    return (type & 1) != 0 ? Duration::Long : Duration::Short;
}

[[nodiscard]] constexpr std::int32_t sound_span(
    std::int32_t type) noexcept {
    return (type & 1) != 0 ? 2 : 1;
}

[[nodiscard]] constexpr std::int32_t pitch_wav_suffix(
    std::size_t row) noexcept {
    return row < kPitchRows ? static_cast<std::int32_t>(5 - row) : -1;
}

struct GridCell {
    std::size_t row{};
    std::size_t column{};
    friend bool operator==(const GridCell&, const GridCell&) = default;
};

[[nodiscard]] constexpr std::optional<GridCell> decode_grid_region(
    std::int32_t region_index) noexcept {
    if (region_index < 0 ||
        region_index >= static_cast<std::int32_t>(
            kPitchRows * kTimelineColumns)) {
        return std::nullopt;
    }

    return GridCell{
        static_cast<std::size_t>(region_index) / kTimelineColumns,
        static_cast<std::size_t>(region_index) % kTimelineColumns,
    };
}

[[nodiscard]] constexpr std::int32_t playback_column_from_centiseconds(
    std::int32_t elapsed_centiseconds) noexcept {
    if (elapsed_centiseconds < 0) return -1;
    const auto column = elapsed_centiseconds / 100;
    return column < static_cast<std::int32_t>(kTimelineColumns)
        ? column
        : -1;
}

[[nodiscard]] constexpr std::int32_t playback_buffer_index(
    std::size_t row,
    std::int32_t type) noexcept {
    if (row >= kPitchRows || type < 0 ||
        type >= static_cast<std::int32_t>(kSoundTypeCount)) {
        return -1;
    }

    return static_cast<std::int32_t>((kPitchRows - 1 - row) * 10)
         + type;
}

inline constexpr std::int32_t kTimelineSeconds = 24;

class SequencerGrid {
public:
    using Row = std::array<std::int32_t, kTimelineColumns>;
    using Storage = std::array<Row, kPitchRows>;

    SequencerGrid() noexcept;

    [[nodiscard]] const Storage& cells() const noexcept { return cells_; }
    [[nodiscard]] std::int32_t at(
        std::size_t row,
        std::size_t column) const noexcept;

    [[nodiscard]] bool can_place(
        std::size_t row,
        std::size_t column,
        std::int32_t type) const noexcept;

    [[nodiscard]] bool place(
        std::size_t row,
        std::size_t column,
        std::int32_t type) noexcept;

    [[nodiscard]] std::int32_t remove_at(
        std::size_t row,
        std::size_t column) noexcept;

    void clear() noexcept;

private:
    Storage cells_{};
};

} // namespace btb::bobs_band
