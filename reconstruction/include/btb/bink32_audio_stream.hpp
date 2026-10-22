#pragma once
// Experimental, DLL-free Bink1 compressed audio reconstruction.
// Float decoding and PCM16 serialization now match FFmpeg on the available
// original SJPi audio corpus within one PCM16 LSB. Runtime audio remains
// opt-in until original-DLL PCM and real DirectSound/video-sync parity pass.
#include "btb/bink32_audio_overlap.hpp"
#include "btb/bink32_audio_inverse.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::bink32_recovered {
struct Bink1DecodedAudioFrame {
    // DCT: planar source channels, RDFT: one interleaved sample stream.
    // These are post inverse-transform & overlap float samples. The tested
    // SJPi corpus matches FFmpeg closely; this does not certify original-DLL
    // integer rounding or playback timing.
    std::vector<std::vector<float>> samples{};
    std::uint32_t declared_pcm_bytes{};
    std::size_t block_count{};
};
class Bink1AudioStreamDecoder final {
public:
    [[nodiscard]] static std::optional<Bink1AudioStreamDecoder>
    open(const BinkAudioTrackSpec& track) noexcept;
    // A compressed Bink frame may hold MULTIPLE DWORD-aligned transform
    // blocks. Failure at any point leaves previous overlap state intact.
    [[nodiscard]] std::optional<Bink1DecodedAudioFrame>
    decode(const AudioPacket& frame);
    void rewind() noexcept {overlap_.rewind();}
    [[nodiscard]] const Bink1AudioLayout& layout() const noexcept {
        return layout_;
    }
private:
    Bink1AudioStreamDecoder(BinkAudioTrackSpec track,Bink1AudioLayout layout)
        :track_(track),layout_(layout),overlap_(layout) {}
    BinkAudioTrackSpec track_{};
    Bink1AudioLayout layout_{};
    Bink1AudioOverlap overlap_;
};
} // namespace btb::bink32_recovered
