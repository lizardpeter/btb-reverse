#pragma once
// First native compressed Bink1 audio decoding stage: source transform
// bitstream -> floating-point spectral coefficients. Inverse DCT/RDFT,
// overlap-add and PCM output are NOT provided here, so callers must never
// treat these coefficients as playable audio.
//
// SJPi (2002) is Bink1 with 29-bit signed-float DC coefficients, 8-bit
// quantizer indices and variable-width/RLE AC groups. Bink 'b' and
// version-2 audio are intentionally not admitted by this source parser.
#include "btb/bink32_audio_packets.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
struct Bink1AudioLayout {
    BinkAudioTransform transform{};
    std::uint16_t source_sample_rate{};
    std::uint8_t source_channels{};
    std::uint16_t frame_length{};
    std::uint16_t overlap_length{};
    std::uint8_t transform_channels{}; // DCT=source channels; RDFT=1
    float root{};
    // First band begins at coefficient 2; final sentinel = frame_length.
    std::vector<std::uint16_t> band_starts{};
};
struct Bink1AudioSpectrumBlock {
    // DCT: each source channel independent, RDFT: interleaved spectra.
    std::vector<std::vector<float>> transform_coefficients{};
    // First unread bit after this entire transform block. Subsequent
    // compressed blocks in the packet start on a DWORD-aligned boundary.
    std::size_t end_bit{};
};
[[nodiscard]] std::optional<Bink1AudioLayout>
plan_bink1_audio_layout(const BinkAudioTrackSpec& track) noexcept;
// Does not decode PCM, synthesize missing audio or modify packet memory.
// All coefficient/channels allocations are bounded to retail Bink1 maxima;
// truncated or invalid packets return nullopt instead of partial spectra.
[[nodiscard]] std::optional<Bink1AudioSpectrumBlock>
decode_bink1_audio_spectrum(
    const BinkAudioBlockPacket& packet,
    const Bink1AudioLayout& layout,
    std::size_t starting_bit=0);
} // namespace btb::bink32_recovered
