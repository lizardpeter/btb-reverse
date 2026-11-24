#pragma once
// Source-derived Bink1 audio track/packet preflight. No PCM decoder yet.
// The old container stores each audio track as three 32-bit words:
// maximum decoded bytes, [flags:16|sample-rate:16], stream identifier.
// Each frame carries length-prefixed compressed audio track payloads.
// See 2002 BinkDoFrame at 0x30006140; independently compare the
// bink demuxer and decoder for packet format, not algorithm substitution.
#include "btb/bink32_container.hpp"
#include "btb/bink32_packet_decode.hpp"
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
enum class BinkAudioTransform {rdft,dct};
struct BinkAudioTrackSpec {
    std::uint32_t track_id{};
    std::uint32_t maximum_decoded_bytes{};
    std::uint16_t sample_rate{};
    std::uint16_t flags{};
    std::uint8_t channels{};
    BinkAudioTransform transform{};
    bool preferred_int16{};
};
struct BinkAudioBlockPacket {
    std::uint32_t decoded_bytes{};
    std::span<const std::uint8_t> compressed_transform{};
    std::uint32_t track_id{};
    std::uint16_t sample_rate{};
    std::uint8_t channels{};
    BinkAudioTransform transform{};
};
// Returns no value if a track has invalid sampling or an impossible
// maximum decoded size. Empty vector is valid for a video-only movie.
// Codec flags with unknown bits are preserved and not interpreted.
[[nodiscard]] std::optional<std::vector<BinkAudioTrackSpec>>
describe_bink_audio_tracks(const ContainerIndex& index) noexcept;

// Source AudioPacket::compressed includes a LE32 decoded-byte count
// followed by the actual transform bitstream. Empty packets mean no audio
// block on that track for this video frame. Never invent PCM or samples.
// Validates block limit and bounds before returning a borrowed view.
[[nodiscard]] std::optional<BinkAudioBlockPacket>
inspect_bink_audio_packet(const AudioPacket& packet,
                          const BinkAudioTrackSpec& track) noexcept;
} // namespace btb::bink32_recovered
