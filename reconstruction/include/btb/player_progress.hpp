#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <ostream>

namespace btb::progress {

inline constexpr std::size_t kPlayerCount = 5;
inline constexpr std::size_t kValuesPerPlayer = 100;
inline constexpr std::size_t kRecordBytes = kValuesPerPlayer * sizeof(std::int32_t);

enum class Slot : std::size_t {
    PetsCorner = 50,
    SquirrelRun = 51,

    BobsBandBob = 52,
    BobsBandWendy = 53,
    BobsBandFarmerPickles = 54,

    ParkDesigner = 55,

    DinoRaptor = 56,
    DinoTriceratops = 57,
    DinoTyrannosaurus = 58,

    SpudMaze = 59,
    SpudSkate = 60,
    Maze = 61,
    Golf = 62,

    // Written to 1 by InitializeFireworksActivity as soon as the finale starts.
    FireworkFinaleEntered = 63,

    // Included by the retail progress-screen sum, but no writer has yet been
    // identified in this executable. Preserve it without inventing semantics.
    ReservedUnused = 64,
};

inline constexpr std::size_t kPrerequisiteFirst = 50;
inline constexpr std::size_t kPrerequisiteLast = 62;
inline constexpr std::size_t kPrerequisiteCount =
    kPrerequisiteLast - kPrerequisiteFirst + 1;

inline constexpr std::size_t kRetailProgressFirst = 50;
inline constexpr std::size_t kRetailProgressLast = 64;
inline constexpr std::int32_t kRetailFinaleThreshold = 13;

// Exact table at 0x00446FA4 used by UpdateProgressScreen while it walks
// progress slots 50..64. Values index the star-position table.
inline constexpr std::array<std::int32_t, 15>
kProgressStarCoordinateIndices{{
    12, 11, 8, 9, 10,
    0, 3, 4, 5, 1,
    2, 6, 7, -1, 1,
}};

struct StarPosition {
    std::int32_t x{};
    std::int32_t y{};
};

// Coordinate indices 0..12 come from the table rooted at 0x00446F3C.
// Index -1 deliberately addresses the immediately preceding pair.
inline constexpr std::array<StarPosition, 14> kProgressStarPositions{{
    {357, -1}, // lookup index -1
    {282, 102},
    {282, 143},
    {331, 143},
    {282, 184},
    {331, 184},
    {381, 184},
    {282, 225},
    {331, 225},
    {381, 225},
    {282, 266},
    {331, 266},
    {381, 266},
    {282, 307},
}};

[[nodiscard]] constexpr StarPosition progress_star_position(
    std::size_t progress_slot) noexcept {
    if (progress_slot < kRetailProgressFirst ||
        progress_slot > kRetailProgressLast) {
        return {-1, -1};
    }

    const auto table_index =
        kProgressStarCoordinateIndices[progress_slot - kRetailProgressFirst];

    // kProgressStarPositions[0] represents retail lookup index -1.
    return kProgressStarPositions[static_cast<std::size_t>(table_index + 1)];
}

struct Record {
    std::array<std::int32_t, kValuesPerPlayer> values{};

    [[nodiscard]] constexpr std::int32_t get(Slot slot) const noexcept {
        return values[static_cast<std::size_t>(slot)];
    }

    constexpr void set(Slot slot, std::int32_t value) noexcept {
        values[static_cast<std::size_t>(slot)] = value;
    }
};

Record read_record(std::istream& in);
void write_record(std::ostream& out, const Record& record);

[[nodiscard]] constexpr std::int32_t retail_progress_sum(
    const Record& record) noexcept {

    std::int32_t total = 0;
    for (std::size_t i = kRetailProgressFirst;
         i <= kRetailProgressLast;
         ++i) {
        const auto value = record.values[i];
        if (value > 0) {
            total += value;
        }
    }
    return total;
}

[[nodiscard]] constexpr bool retail_finale_available(
    const Record& record) noexcept {
    return retail_progress_sum(record) >= kRetailFinaleThreshold;
}

[[nodiscard]] constexpr bool intended_prerequisites_complete(
    const Record& record) noexcept {

    for (std::size_t i = kPrerequisiteFirst;
         i <= kPrerequisiteLast;
         ++i) {
        if (record.values[i] <= 0) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] constexpr std::array<Slot,3> dino_species_slots() noexcept {
    return {
        Slot::DinoRaptor,
        Slot::DinoTriceratops,
        Slot::DinoTyrannosaurus,
    };
}

[[nodiscard]] constexpr std::array<Slot,3> bobs_band_conductor_slots() noexcept {
    return {
        Slot::BobsBandBob,
        Slot::BobsBandWendy,
        Slot::BobsBandFarmerPickles,
    };
}

[[nodiscard]] constexpr std::int32_t completed_count(
    const Record& record,
    std::size_t first,
    std::size_t count) noexcept {

    std::int32_t completed = 0;
    for (std::size_t i = 0; i < count; ++i) {
        if (record.values[first + i] >= 1) {
            ++completed;
        }
    }
    return completed;
}

// Mr Bentley progress narration. Retail uses 955/956 as interchangeable
// positive-completion lines and 957..959 for one/two/three items remaining.
[[nodiscard]] constexpr std::int32_t progress_feedback_sound(
    std::int32_t completed,
    std::int32_t group_size,
    std::int32_t random_bit) noexcept {

    if (group_size < 1 || group_size > 3 ||
        completed < 0 || completed > group_size) {
        return -1;
    }

    if (completed == group_size) {
        return random_bit >= 0 && random_bit < 2
            ? 955 + random_bit
            : -1;
    }

    const auto remaining = group_size - completed;
    return 956 + remaining; // 1 left=957, 2=958, 3=959
}

[[nodiscard]] constexpr std::int32_t bobs_band_feedback_sound(
    const Record& record,
    std::int32_t random_bit) noexcept {
    return progress_feedback_sound(
        completed_count(record, 52, 3), 3, random_bit);
}

[[nodiscard]] constexpr std::int32_t dino_feedback_sound(
    const Record& record,
    std::int32_t random_bit) noexcept {
    return progress_feedback_sound(
        completed_count(record, 56, 3), 3, random_bit);
}

[[nodiscard]] constexpr std::int32_t spud_pair_feedback_sound(
    const Record& record,
    std::int32_t random_bit) noexcept {
    return progress_feedback_sound(
        completed_count(record, 59, 2), 2, random_bit);
}

[[nodiscard]] constexpr std::int32_t adventure_pair_feedback_sound(
    const Record& record,
    std::int32_t random_bit) noexcept {
    return progress_feedback_sound(
        completed_count(record, 61, 2), 2, random_bit);
}

struct FinaleGate {
    bool locked{true};
    bool unlocked{false};
};

[[nodiscard]] constexpr FinaleGate update_finale_gate_from_progress(
    const Record& record) noexcept {

    if (retail_finale_available(record)) {
        return {false, true};
    }
    return {true, false};
}

// Activity Select only intercepts the Firework Finale action while locked.
// Retail action 0x22 is the Firework pre-game route.
inline constexpr std::int32_t kFireworkFinaleAction = 0x22;

[[nodiscard]] constexpr bool should_open_progress_screen(
    const FinaleGate& gate,
    std::int32_t selected_action) noexcept {
    return gate.locked && selected_action == kFireworkFinaleAction;
}

} // namespace btb::progress
