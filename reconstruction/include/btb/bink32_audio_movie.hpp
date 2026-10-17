#pragma once
// Read-only in-memory audio reconstruction from an original SJPi BIK file.
// No extracted assets, disk cache, RAD or FFmpeg runtime dependency.
// Remains experimental until real retail movie PCM parity is certified.
#include "btb/bink32_audio_pcm.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace btb::bink32_recovered {
struct Bink1MoviePcm16 {
    std::uint32_t sample_rate{};
    std::uint8_t channels{};
    std::uint32_t track_id{};
    // Interleaved source-rate PCM16 samples for ONE selected audio track.
    std::vector<std::int16_t> samples{};
    // N+1 entries for N video frames. Element zero is sample frame 0,
    // and [i] marks the end of original video frame i. Repeated offsets
    // represent video frames without an audio packet, not fake silence.
    std::vector<std::uint64_t> audio_frame_boundaries{};
    [[nodiscard]] std::uint64_t sample_frames() const noexcept {
        return channels ? samples.size()/channels : 0;
    }
    [[nodiscard]] std::optional<std::uint64_t> duration_us() const noexcept;
    [[nodiscard]] std::optional<std::uint64_t> timestamp_us(
        std::uint32_t original_frame_end) const noexcept;
};

// Decodes an entire SJPi movie's selected audio track, respecting original
// frame order, overlap history and declared PCM16 byte counts. Refuses
// unsupported/corrupt packets transactionally. Input remains owned by caller.
// A 64 MiB upper bound prevents a source-controlled memory explosion.
// Does NOT provide playback, clock drift correction or DirectSound output.
[[nodiscard]] std::optional<Bink1MoviePcm16>
decode_bink1_movie_audio(std::span<const std::uint8_t> original_bik,
                         std::size_t track_index,std::string& error);
} // namespace btb::bink32_recovered
