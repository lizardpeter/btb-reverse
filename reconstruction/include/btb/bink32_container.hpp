#pragma once
// Source-informed on-disk Bink/SJPi container parser. No RAD DLL, ffmpeg or
// external process. Parse-only: compressed video/audio payload is not decoded.
// Layout checked against original 2002 BinkOpen at 0x30003D90 and four
// supplied retail BIK assets. All original assets remain read-only.
#include "btb/bink32_recovered.hpp"
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
struct TrackRecord {
    std::uint32_t max_decoded_bytes{};
    std::uint32_t codec_flags{};
    std::uint32_t track_id{};
};
struct FramePacket {
    std::uint32_t number{}; // one based
    std::uint32_t offset{}, compressed_bytes{};
    bool key_frame{};
};
struct ContainerIndex {
    Header header{};
    std::uint32_t declared_file_size_minus_8{};
    std::uint32_t maximum_frame_bytes{}, video_flags{};
    std::vector<TrackRecord> tracks{};
    std::vector<std::uint32_t> frame_offsets_flags{}; // frames+1
    [[nodiscard]] std::optional<FramePacket> frame(std::uint32_t one_based) const noexcept;
};
// Parses the entire Bink prefix/audio metadata and frame index; it DOES NOT
// make any claims about supporting actual Bink video decompression yet.
[[nodiscard]] std::optional<ContainerIndex> parse_container_index(
    std::span<const std::uint8_t> data) noexcept;
} // namespace btb::bink32_recovered
