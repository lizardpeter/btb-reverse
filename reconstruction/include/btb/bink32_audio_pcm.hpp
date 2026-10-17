#pragma once
// Portable, bounded Bink1 float -> interleaved signed PCM16 conversion.
// This does not activate audio output in the game; independent retail
// sample parity remains a gate for replacing original binkw32.dll playback.
#include "btb/bink32_audio_stream.hpp"
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
struct Bink1Pcm16Block {
    std::uint32_t sample_rate{};
    std::uint8_t channels{};
    // Windows WAVE_FORMAT_PCM uses little-endian signed 16-bit frames.
    // This host-independent sample vector is interleaved L,R,L,R...;
    // serialize each int16 value explicitly as LE when writing bytes.
    std::vector<std::int16_t> interleaved_samples{};
    [[nodiscard]] std::size_t frame_count() const noexcept {
        return channels ? interleaved_samples.size()/channels : 0;
    }
};

// Reject truncated PCM, mismatched byte count, nonfinite float values,
// incorrect RDFT stereo alignment and invalid layouts before output.
// Zero-length compressed packets yield a legitimate empty buffer.
// Round-to-nearest-even and saturate to [-32768,32767] deterministically,
// regardless of caller's floating-point rounding mode.
[[nodiscard]] std::optional<Bink1Pcm16Block>
encode_bink1_pcm16(const Bink1DecodedAudioFrame& decoded,
                   const Bink1AudioLayout& layout);

// Produce raw little-endian WAVE_FORMAT_PCM sample bytes, never wav headers.
[[nodiscard]] std::vector<std::uint8_t>
serialize_bink1_pcm16_le(std::span<const std::int16_t> interleaved);
} // namespace btb::bink32_recovered
