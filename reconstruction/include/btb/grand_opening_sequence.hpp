#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <ostream>
#include <optional>
#include <string_view>

namespace btb::grand_opening {

inline constexpr std::size_t kConductorCount = 3;
inline constexpr std::size_t kPlayerProfileCount = 5;
inline constexpr std::size_t kPitchRowCount = 5;
inline constexpr std::size_t kTimelineStepCount = 24;
inline constexpr std::size_t kCompositionCellCount =
    kPitchRowCount * kTimelineStepCount;
inline constexpr std::size_t kCompositionSaveBytes =
    kCompositionCellCount * sizeof(std::int32_t);
inline constexpr std::int32_t kTimelineStepMilliseconds = 1000;
inline constexpr std::int32_t kTimelineDurationSeconds = 24;

inline constexpr std::int32_t kEmptyCell = -1;
inline constexpr std::int32_t kContinuationCell = 10;

enum class Conductor : std::int32_t {
    Bob = 0,
    Wendy = 1,
    FarmerPickles = 2,
};

// Verified from the BEXT metadata embedded in roley1_1.wav .. roley1_5.wav.
// Retail displays row 0 from WAV suffix 5 and row 4 from suffix 1.
enum class Pitch : std::int32_t {
    CSharp = 0,
    B = 1,
    A = 2,
    GSharp = 3,
    FSharp = 4,
};

[[nodiscard]] constexpr Pitch pitch_for_row(std::size_t row) noexcept {
    return static_cast<Pitch>(row);
}

[[nodiscard]] constexpr std::string_view pitch_name(Pitch pitch) noexcept {
    switch (pitch) {
        case Pitch::CSharp: return "C#";
        case Pitch::B: return "B";
        case Pitch::A: return "A";
        case Pitch::GSharp: return "G#";
        case Pitch::FSharp: return "F#";
    }
    return "";
}

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

enum class MachineType : std::int32_t {
    Roley1Second = 0,
    Roley2Second = 1,
    Muck1Second = 2,
    Muck2Second = 3,
    Lofty1Second = 4,
    Lofty2Second = 5,
    Dizzy1Second = 6,
    Dizzy2Second = 7,
    Scoop1Second = 8,
    Scoop2Second = 9,
};

[[nodiscard]] constexpr MachineType machine_type(
    Machine machine,
    Duration duration) noexcept {
    return static_cast<MachineType>(
        static_cast<std::int32_t>(machine) * 2 +
        static_cast<std::int32_t>(duration));
}

[[nodiscard]] constexpr Machine machine_for_type(
    MachineType type) noexcept {
    return static_cast<Machine>(
        static_cast<std::int32_t>(type) / 2);
}

[[nodiscard]] constexpr Duration duration_for_type(
    MachineType type) noexcept {
    return (static_cast<std::int32_t>(type) & 1) != 0
        ? Duration::Long
        : Duration::Short;
}

enum class ToolbarControl : std::int32_t {
    Play = 0,
    Stop = 1,
    ClearAll = 2,
    Delete = 3,
};

enum class ActivityState : std::int32_t {
    Edit = 0,
    MachineSelected = 1,
    Passive2 = 2,
    Passive3 = 3,
    Passive4 = 4,
    Passive5 = 5,
    Passive6 = 6,
    Passive7 = 7,
    PreparePlayback = 8,
    Playing = 9,
    ExitToPlayAgain = 10,
};

[[nodiscard]] constexpr bool is_machine_type(std::int32_t value) noexcept {
    return value >= 0 && value <= 9;
}

[[nodiscard]] constexpr std::string_view machine_name(
    MachineType type) noexcept {
    switch (type) {
        case MachineType::Roley1Second: return "Roley 1 second";
        case MachineType::Roley2Second: return "Roley 2 second";
        case MachineType::Muck1Second: return "Muck 1 second";
        case MachineType::Muck2Second: return "Muck 2 second";
        case MachineType::Lofty1Second: return "Lofty 1 second";
        case MachineType::Lofty2Second: return "Lofty 2 second";
        case MachineType::Dizzy1Second: return "Dizzy 1 second";
        case MachineType::Dizzy2Second: return "Dizzy 2 second";
        case MachineType::Scoop1Second: return "Scoop 1 second";
        case MachineType::Scoop2Second: return "Scoop 2 second";
    }
    return "";
}

[[nodiscard]] constexpr std::int32_t machine_span(MachineType type) noexcept {
    return (static_cast<std::int32_t>(type) & 1) == 0 ? 1 : 2;
}

[[nodiscard]] constexpr std::int32_t wav_suffix_for_pitch_row(
    std::size_t row) noexcept {
    // Embedded WAV BEXT labels prove suffixes 1..5 are pitches:
    // 1=F#, 2=G#, 3=A, 4=B, 5=C#.
    return row < kPitchRowCount
        ? static_cast<std::int32_t>(kPitchRowCount - row)
        : -1;
}

[[nodiscard]] constexpr std::int32_t loaded_sound_slot_index(
    std::size_t row,
    MachineType type) noexcept {
    return 40
        - static_cast<std::int32_t>(row) * 10
        + static_cast<std::int32_t>(type);
}

struct GridCell {
    std::size_t pitch_row{};
    std::size_t second{};
    friend bool operator==(const GridCell&, const GridCell&) = default;
};

[[nodiscard]] constexpr std::optional<GridCell> decode_grid_region(
    std::int32_t region_index) noexcept {
    if (region_index < 0 ||
        region_index >= static_cast<std::int32_t>(
            kPitchRowCount * kTimelineStepCount)) {
        return std::nullopt;
    }
    return GridCell{
        static_cast<std::size_t>(region_index) / kTimelineStepCount,
        static_cast<std::size_t>(region_index) % kTimelineStepCount,
    };
}

[[nodiscard]] constexpr std::int32_t playback_step_from_centiseconds(
    std::int32_t elapsed_centiseconds) noexcept {
    if (elapsed_centiseconds < 0) return -1;
    const auto step = elapsed_centiseconds / 100;
    return step < static_cast<std::int32_t>(kTimelineStepCount)
        ? step
        : -1;
}

[[nodiscard]] constexpr std::string_view conductor_name(
    Conductor conductor) noexcept {
    switch (conductor) {
        case Conductor::Bob: return "bob";
        case Conductor::Wendy: return "wendy";
        case Conductor::FarmerPickles: return "farmer";
    }
    return "";
}

[[nodiscard]] constexpr std::string_view backing_track_filename(
    Conductor conductor) noexcept {
    switch (conductor) {
        case Conductor::Bob: return "bobmt.wav";
        case Conductor::Wendy: return "Wendymt.wav";
        case Conductor::FarmerPickles: return "fpmt.wav";
    }
    return "";
}

struct Composition {
    std::array<
        std::array<std::int32_t, kTimelineStepCount>,
        kPitchRowCount> cells{};

    Composition() noexcept {
        clear();
    }

    void clear() noexcept {
        for (auto& row : cells) {
            row.fill(kEmptyCell);
        }
    }
};

Composition read_composition(std::istream& in);
void write_composition(std::ostream& out, const Composition& composition);

[[nodiscard]] bool can_place_event(
    const Composition& composition,
    std::size_t row,
    std::size_t step,
    MachineType type) noexcept;

[[nodiscard]] bool place_event(
    Composition& composition,
    std::size_t row,
    std::size_t step,
    MachineType type) noexcept;

// Removes the owning event even when step points at a continuation cell.
// Returns the removed machine type value, or -1 if no event occupies the cell.
[[nodiscard]] std::int32_t remove_event_at(
    Composition& composition,
    std::size_t row,
    std::size_t step) noexcept;

[[nodiscard]] constexpr std::size_t composition_file_index(
    Conductor conductor,
    std::size_t zero_based_player_profile) noexcept {
    return static_cast<std::size_t>(conductor) * kPlayerProfileCount
         + zero_based_player_profile;
}

// Returns the exact retail filename for one conductor/profile pair.
[[nodiscard]] const char* composition_filename(
    Conductor conductor,
    std::size_t zero_based_player_profile) noexcept;

// Retail's odd legacy last.txt writer emits the current conductor dword five
// times. There is no corresponding read xref in this executable build.
[[nodiscard]] constexpr std::array<std::int32_t,5> legacy_last_payload(
    Conductor conductor) noexcept {
    const auto value = static_cast<std::int32_t>(conductor);
    return {value,value,value,value,value};
}

// The Grand Opening progress block uses 25 dwords per player profile.
// Entering playback marks this conductor slot complete.
[[nodiscard]] constexpr std::size_t conductor_progress_index(
    std::size_t zero_based_player_profile,
    Conductor conductor) noexcept {
    return zero_based_player_profile * 25
         + static_cast<std::size_t>(conductor);
}

[[nodiscard]] constexpr bool all_conductors_complete(
    const std::array<std::int32_t,kConductorCount>& flags) noexcept {
    return flags[0] != 0 && flags[1] != 0 && flags[2] != 0;
}

} // namespace btb::grand_opening
