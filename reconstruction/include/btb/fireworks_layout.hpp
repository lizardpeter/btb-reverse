#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <optional>

namespace btb::fireworks {

struct Recti {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};

    friend bool operator==(const Recti&, const Recti&) = default;
};

struct LayoutData {
    std::array<Recti, 18> placement_source{};
    std::array<Recti, 18> placement_runtime{};
    std::array<Recti, 12> palette{};
};

enum class FireworkType : std::int32_t {
    RedAirbomb = 0,
    SmallGreen = 1,
    MediumRed = 2,
    LargeBlue = 3,
    LargeRed = 4,
    MediumGreen = 5,
    SmallBlue = 6,
    BlueAirbomb = 7,
    RedCandle = 8,
    RedSpinner = 9,
    GreenSpinner = 10,
    BlueCandle = 11,
};

struct MovieChoice {
    std::int32_t left_or_single{};
    std::optional<std::int32_t> right{};
};

LayoutData parse_layout(std::istream& placement, std::istream& palette);

[[nodiscard]] constexpr MovieChoice movie_choice(FireworkType type) noexcept {
    const auto id = static_cast<std::int32_t>(type);
    if (id >= 0 && id <= 7) {
        return {id, id + 12};
    }
    return {id, std::nullopt};
}

[[nodiscard]] constexpr std::int32_t movie_index(
    FireworkType type,
    bool use_right_variant) noexcept {
    const auto choice = movie_choice(type);
    if (use_right_variant && choice.right) {
        return *choice.right;
    }
    return choice.left_or_single;
}

inline constexpr std::int32_t kTopMiddleMovieIndex = 20;
inline constexpr std::int32_t kCrowdLoopMovieIndex = 21;
inline constexpr std::int32_t kCrowdEndMovieIndex = 22;

} // namespace btb::fireworks
