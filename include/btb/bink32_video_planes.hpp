#pragma once
// Stateful native pixel-plane integration for resolved Bink block commands.
// This is downstream of the not-yet-reconstructed retail 0x3000E400 bundle
// scheduler. It does not interpret compressed video bits or color matrices.
#include "btb/bink32_block_modes.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>

namespace btb::bink32_recovered {
struct BinkYuvDimensions {
    std::size_t width{}, height{};
    std::size_t chroma_width{}, chroma_height{};
};
struct BinkYuvCommands {
    std::span<const BinkBlockCommand> y{}, u{}, v{};
};
struct BinkYuvFrame {
    BinkModePlane y{}, u{}, v{};
};

// Atomic three-plane assembly: one malformed block in any channel
// rejects the entire candidate without changing the prior frame.
// Chroma dimensions are explicit because original per-version sampling,
// cropping and plane order are not yet authenticated from the 2002 DLL.
[[nodiscard]] std::optional<BinkYuvFrame> compose_bink_yuv_planes(
    const BinkYuvDimensions& dimensions, const BinkYuvCommands& commands,
    const BinkYuvFrame* previous=nullptr);

class BinkYuvFrameHistory final {
public:
    explicit BinkYuvFrameHistory(BinkYuvDimensions dimensions) noexcept
        : dimensions_(dimensions) {}
    // Keyframes must decode independently: no stale reference is supplied.
    // Failed decode (including bad keyframes) leaves history unchanged.
    [[nodiscard]] std::optional<BinkYuvFrame> advance(
        const BinkYuvCommands& commands, bool key_frame=false);
    void reset() noexcept { previous_.reset(); }
    [[nodiscard]] const BinkYuvFrame* previous() const noexcept {
        return previous_ ? &*previous_ : nullptr;
    }
private:
    BinkYuvDimensions dimensions_{};
    std::optional<BinkYuvFrame> previous_{};
};
} // namespace btb::bink32_recovered
