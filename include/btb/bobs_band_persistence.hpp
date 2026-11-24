#pragma once

#include "btb/bobs_band.hpp"
#include "btb/bobs_band_data.hpp"

#include <filesystem>
#include <optional>
#include <string>

namespace btb::bobs_band {

struct SequenceLoad {
    Composition composition{};
    bool opened{};
    bool complete_read{};
    bool structurally_valid{};
    std::string error{};
};

// The original initializer fills the grid with -1, then tries to read 120
// separate four-byte cells from a conductor-major, five-slot .txt file.
// Missing files therefore yield an empty composition rather than an error.
[[nodiscard]] SequenceLoad load_sequence_file(
    const std::filesystem::path& directory,
    Conductor conductor,
    int player_index);

// Native unload 0x41DA78 writes every signed grid dword in row-major order
// through 120 calls to fwrite(ptr, 4, 1, file). The .txt extension is
// misleading: this is little-endian binary, not printable ASCII.
[[nodiscard]] bool save_sequence_file(
    const std::filesystem::path& directory,
    Conductor conductor,
    int player_index,
    const Composition& composition);

// Native teardown writes five copies of the chosen conductor's 32-bit value
// to "last.txt" in binary ("wb") mode. The odd repetition is deliberate:
// original code at 0x41DAF4..0x41DB0C increments a loop counter while each
// fwrite still passes the identical address 0x51C284.
[[nodiscard]] bool write_native_last_file(
    const std::filesystem::path& directory,
    Conductor conductor);

} // namespace btb::bobs_band
