#pragma once
// Portable C++26 native Bink1 movie stream. This is the video/frame
// and packet lifecycle needed by the game provider, with no original
// RAD DLL or external FFmpeg process. Audio packet data is available
// to a future native audio decoder; it is NOT decoded by this class.
#include "btb/bink32_container.hpp"
#include "btb/bink32_packet_decode.hpp"
#include "btb/bink32_yuva_frame.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
class BinkNativeVideoStream {
public:
    [[nodiscard]] static std::optional<BinkNativeVideoStream>
    open(std::vector<std::uint8_t> original_bik);

    [[nodiscard]] bool decode_next();
    [[nodiscard]] bool seek(std::uint32_t one_based_frame);
    void rewind() noexcept;
    [[nodiscard]] bool at_end() const noexcept {
        return frame_number_>=index_.header.frame_count;
    }
    [[nodiscard]] std::uint32_t frame_number() const noexcept {
        return frame_number_;
    }
    [[nodiscard]] std::uint32_t frame_count() const noexcept {
        return index_.header.frame_count;
    }
    [[nodiscard]] std::uint32_t frame_rate_numerator() const noexcept {
        return index_.header.frame_rate;
    }
    [[nodiscard]] std::uint32_t frame_rate_denominator() const noexcept {
        return index_.header.divisor;
    }
    [[nodiscard]] bool has_encoded_alpha() const noexcept {
        return (index_.video_flags&0x00100000U)!=0;
    }
    [[nodiscard]] const BinkNativeYuVaFrame* current_frame() const noexcept {
        return current_?&*current_:nullptr;
    }
    // Presentation time of a one-based frame, relative to frame 1.
    // Return nullopt on invalid frame or timestamp overflow.
    [[nodiscard]] std::optional<std::uint64_t>
    frame_timestamp_us(std::uint32_t one_based_frame) const noexcept;

    [[nodiscard]] std::optional<EncodedFrame>
    encoded_frame(std::uint32_t one_based_frame) const noexcept {
        return split_encoded_frame(index_,original_bik_,one_based_frame);
    }
    [[nodiscard]] std::size_t audio_track_count() const noexcept {
        return index_.tracks.size();
    }
private:
    BinkNativeVideoStream(std::vector<std::uint8_t> bytes,
                          ContainerIndex index)
        : original_bik_(std::move(bytes)),index_(std::move(index)) {}
    std::vector<std::uint8_t> original_bik_{};
    ContainerIndex index_{};
    std::optional<BinkNativeYuVaFrame> current_{};
    std::uint32_t frame_number_{};
};
} // namespace btb::bink32_recovered
