#include "btb/bobs_band_persistence.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <vector>

namespace btb::bobs_band {

SequenceLoad load_sequence_file(
    const std::filesystem::path& directory,
    Conductor conductor,
    int player_index) {

    SequenceLoad out;
    const auto filename = sequence_filename(conductor, player_index);
    if (filename.empty()) {
        out.error = "invalid conductor/player slot";
        return out;
    }

    std::ifstream in(directory / filename, std::ios::binary);
    if (!in) {
        // Normal retail cold start: no previous composition file.
        return out;
    }
    out.opened = true;

    std::array<char,480> buffer{};
    in.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
    const auto count = std::max<std::streamsize>(0, in.gcount());
    out.complete_read = count == static_cast<std::streamsize>(buffer.size());

    // The retail fread loop updates individual cells as they are read.
    // On an unexpected short read the remaining already-cleared -1 cells
    // stay empty, rather than treating the entire file as corrupt.
    auto canonical = out.composition.encode_le_dwords();
    const auto full_dword_bytes =
        static_cast<std::size_t>(count / 4) * 4;
    for (std::size_t i = 0; i < full_dword_bytes; ++i) {
        canonical[i] = static_cast<std::uint8_t>(buffer[i]);
    }
    static_cast<void>(out.composition.decode_le_dwords(canonical));
    out.structurally_valid = out.composition.structurally_valid();
    if (!out.complete_read) {
        out.error = "short native Band sequence read; retained complete cells";
    }
    return out;
}

bool save_sequence_file(
    const std::filesystem::path& directory,
    Conductor conductor,
    int player_index,
    const Composition& composition) {

    const auto filename = sequence_filename(conductor, player_index);
    if (filename.empty()) {
        return false;
    }
    std::ofstream out(directory / filename,
                      std::ios::binary | std::ios::trunc);
    if (!out) {
        return false;
    }
    const auto bytes = composition.encode_le_dwords();
    out.write(
        reinterpret_cast<const char*>(bytes.data()),
        static_cast<std::streamsize>(bytes.size()));
    return static_cast<bool>(out);
}

bool write_native_last_file(
    const std::filesystem::path& directory,
    Conductor conductor) {

    const int id = static_cast<int>(conductor);
    if (id < 0 || id >= kConductorCount) {
        return false;
    }
    std::ofstream out(directory / "last.txt",
                      std::ios::binary | std::ios::trunc);
    if (!out) {
        return false;
    }

    std::array<std::uint8_t,20> encoded{};
    for (int record = 0; record < 5; ++record) {
        const std::size_t off = static_cast<std::size_t>(record) * 4;
        const auto value = static_cast<std::uint32_t>(id);
        encoded[off]     = static_cast<std::uint8_t>(value);
        encoded[off + 1] = static_cast<std::uint8_t>(value >> 8);
        encoded[off + 2] = static_cast<std::uint8_t>(value >> 16);
        encoded[off + 3] = static_cast<std::uint8_t>(value >> 24);
    }
    out.write(
        reinterpret_cast<const char*>(encoded.data()),
        static_cast<std::streamsize>(encoded.size()));
    return static_cast<bool>(out);
}

} // namespace btb::bobs_band
