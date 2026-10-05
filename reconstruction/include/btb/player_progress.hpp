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
    Unknown64 = 64,
};

inline constexpr std::size_t kPrerequisiteFirst = 50;
inline constexpr std::size_t kPrerequisiteLast = 62;
inline constexpr std::size_t kPrerequisiteCount =
    kPrerequisiteLast - kPrerequisiteFirst + 1;

inline constexpr std::size_t kRetailProgressFirst = 50;
inline constexpr std::size_t kRetailProgressLast = 64;
inline constexpr std::int32_t kRetailFinaleThreshold = 13;

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

} // namespace btb::progress
