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

 // Source model of internal 0x3000FC00: initializes entropy-plane state
 // for the block decoder at 0x3000E400.
 struct EntropyPlaneSetup {
     std::uint32_t read_state{}, extra_state{}, symbol_bits{}, prefix_mask{};
     std::uint32_t table_bit_width{};
     std::uintptr_t lookup_table_address{};
 };
 [[nodiscard]] EntropyPlaneSetup init_entropy_plane(
     std::uintptr_t table_address, std::uint32_t symbol_base,
     std::uint32_t plane_width, std::uint32_t symbol_bits,
     std::uint32_t plane_multiplier, bool install_prefix_mask) noexcept;


 // Original internal 0x3000FCB0: read one variable-length symbol through the
 // fixed tables at 0x30047800..0x30047BCF. This does not yet decode
 // dynamic tree permutations or complete a decoded video block.
 struct FixedHuffmanSymbol {
     std::uint8_t value{}, consumed_bits{};
 };
 [[nodiscard]] std::optional<FixedHuffmanSymbol> read_fixed_huffman_symbol(
     LsbBitReader& source, unsigned selector) noexcept;
} // namespace btb::bink32_recovered
