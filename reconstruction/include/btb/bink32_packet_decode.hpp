#pragma once
// Bink32 2002: per-frame audio/video routing recovered from original
// BinkDoFrame 0x30006140-0x30006355, plus bounded LSB-first bitstream reads.
// Original game assets remain read-only. This is not yet the pixel decoder.
#include "btb/bink32_container.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::bink32_recovered {
struct AudioPacket {
    std::uint32_t track_number{};
    std::span<const std::uint8_t> compressed{};
};
struct EncodedFrame {
    FramePacket frame{};
    std::vector<AudioPacket> audio{};
    std::span<const std::uint8_t> video{};
};
// Views into the original file. Each audio track has an LE32 *compressed*
// payload length followed by that many bytes. Remaining bytes are video.
[[nodiscard]] std::optional<EncodedFrame> split_encoded_frame(
    const ContainerIndex&, std::span<const std::uint8_t> source,
    std::uint32_t frame_number) noexcept;

class LsbBitReader final {
public:
    explicit LsbBitReader(std::span<const std::uint8_t> data) noexcept
        : bytes_(data) {}
    [[nodiscard]] std::optional<std::uint32_t> peek(unsigned width) const noexcept;
    [[nodiscard]] std::optional<std::uint32_t> read(unsigned width) noexcept;
    [[nodiscard]] bool skip(std::size_t bits) noexcept;
    [[nodiscard]] bool align(unsigned byte_multiple) noexcept;
    [[nodiscard]] std::size_t bit_position() const noexcept {return pos_;}
    [[nodiscard]] std::size_t bits_remaining() const noexcept {
        return bytes_.size()*8 - pos_;
    }
private:
    std::span<const std::uint8_t> bytes_;
    std::size_t pos_{};
};
struct DecoderFrontier {
    static constexpr std::uint32_t bink_do_frame=0x30005F40U;
    static constexpr std::uint32_t video_dispatch=0x3000E270U;
    static constexpr std::uint32_t video_block_decode=0x3000E400U;
    static constexpr std::uint32_t entropy_setup=0x3000FC00U;
    static constexpr std::uint32_t entropy_tree=0x3000FCB0U;
};
} // namespace btb::bink32_recovered
