#pragma once
// Bink1 block-mode pixel compositor. Commands arrive after entropy
// decoding; no assumptions about retail packet ordering are made.
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>
namespace btb::bink32_recovered {
enum class BinkBlockMode : std::uint8_t {
    Skip=0, Scaled=1, Motion=2, Run=3, Residue=4,
    Intra=5, Fill=6, Inter=7, Pattern=8, Raw=9,
    Covered=255
};
struct BinkBlockCommand {
    BinkBlockMode mode{BinkBlockMode::Skip};
    BinkBlockMode scaled_submode{BinkBlockMode::Raw};
    std::int16_t motion_x{};
    std::int16_t motion_y{};
    std::uint8_t fill{};
    std::array<std::uint8_t,2> pattern_colors{};
    std::array<std::uint8_t,8> pattern_rows{};
    std::array<std::uint8_t,64> samples{};
    std::array<std::int8_t,64> residuals{};
};
struct BinkModePlane {
    std::size_t width{};
    std::size_t height{};
    std::vector<std::uint8_t> pixels{};
};
[[nodiscard]] std::optional<BinkModePlane> compose_bink_mode_plane(
    std::size_t width,std::size_t height,
    std::span<const BinkBlockCommand> commands,
    std::span<const std::uint8_t> previous={});
} // namespace btb::bink32_recovered
