#include "btb/bink32_audio_packets.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::bink32_recovered {
namespace {
constexpr std::uint16_t stereo=0x2000,usedct=0x1000,int16=0x4000;
[[nodiscard]] std::uint32_t le32(std::span<const std::uint8_t> b) noexcept {
    return std::uint32_t(b[0])|
        std::uint32_t(b[1])<<8|
        std::uint32_t(b[2])<<16|
        std::uint32_t(b[3])<<24;
}
}

std::optional<std::vector<BinkAudioTrackSpec>>
describe_bink_audio_tracks(const ContainerIndex& index) noexcept {
    if(index.tracks.size()>64)return std::nullopt;
    std::vector<BinkAudioTrackSpec> parsed;
    parsed.reserve(index.tracks.size());
    for(const auto& source:index.tracks) {
        // High word = original audio flags; low word = actual Hz.
        // An audio track with zero capacity or impossible sample rate is
        // unsafe to schedule even when its current frame is empty.
        const auto rate=static_cast<std::uint16_t>(source.codec_flags);
        const auto flags=static_cast<std::uint16_t>(source.codec_flags>>16);
        if(rate<1000||rate>192000 || !source.max_decoded_bytes ||
           source.max_decoded_bytes>100'000'000)
            return std::nullopt;
        parsed.push_back(BinkAudioTrackSpec{
            source.track_id,source.max_decoded_bytes,rate,flags,
            static_cast<std::uint8_t>((flags&stereo)?2:1),
            (flags&usedct)?BinkAudioTransform::dct:BinkAudioTransform::rdft,
            (flags&int16)!=0
        });
    }
    return parsed;
}

std::optional<BinkAudioBlockPacket>
inspect_bink_audio_packet(const AudioPacket& packet,
                          const BinkAudioTrackSpec& track) noexcept {
    if(packet.compressed.size()<4)return std::nullopt;
    const auto decoded_bytes=le32(packet.compressed.first(4));
    // Bink's decoded-byte prefix is NOT the compressed packet length and
    // is not an independently decoded sample count. Zero-size packets
    // can appear: the following transform payload must then be empty.
    if(decoded_bytes>track.maximum_decoded_bytes ||
       (decoded_bytes==0 && packet.compressed.size()!=4) ||
       (decoded_bytes!=0 && packet.compressed.size()==4))
        return std::nullopt;
    return BinkAudioBlockPacket{
        decoded_bytes,packet.compressed.subspan(4),
        track.track_id,track.sample_rate,track.channels,track.transform
    };
}
} // namespace btb::bink32_recovered
