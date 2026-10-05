#pragma once

#include "btb/bobs_band_runtime.hpp"

#include <array>
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>

namespace btb::bobs_band {

inline constexpr std::size_t kPlayerCount = 5;
inline constexpr std::size_t kSerializedGridCellCount =
    kPitchRows * kTimelineColumns;
inline constexpr std::size_t kSerializedGridBytes =
    kSerializedGridCellCount * sizeof(std::int32_t);

using GridStorage = SequencerGrid::Storage;

[[nodiscard]] std::string sequence_filename(
    Conductor conductor,
    std::size_t player_index);

GridStorage read_grid_storage(std::istream& in);
void write_grid_storage(std::ostream& out, const GridStorage& grid);

[[nodiscard]] constexpr std::array<std::int32_t, kPlayerCount>
make_last_file_record(Conductor conductor) noexcept {
    std::array<std::int32_t, kPlayerCount> result{};
    result.fill(static_cast<std::int32_t>(conductor));
    return result;
}

} // namespace btb::bobs_band
