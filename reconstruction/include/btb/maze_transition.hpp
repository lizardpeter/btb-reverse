#pragma once

#include "btb/maze_data.hpp"

#include <cstdint>

namespace btb::maze {

struct TransitionRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};

    friend bool operator==(const TransitionRect&, const TransitionRect&) = default;
};

struct TransitionBlit {
    Screen screen{Screen::Middle};
    TransitionRect source{};
    std::int32_t destination_x{};
    std::int32_t destination_y{20};

    friend bool operator==(const TransitionBlit&, const TransitionBlit&) = default;
};

enum class TransitionDirection : std::int32_t {
    TowardWest = -1,
    None = 0,
    TowardEast = 1,
};

struct ScreenTransitionState {
    Screen previous_screen{Screen::Middle};
    Screen destination_screen{Screen::Middle};

    // Retail state 0x00512128: 1 = initialize, 2 = active, 0 = complete.
    std::int32_t phase{1};
    TransitionDirection direction{TransitionDirection::None};

    std::int32_t old_left{};
    std::int32_t new_right{};
    std::int32_t speed_numerator{10};

    // Globals 0x005144D8 / 0x005144DC used by actor presentation while sliding.
    std::int32_t actor_offset_a{};
    std::int32_t actor_offset_b{};
};

struct ScreenTransitionFrame {
    TransitionBlit left_blit{};
    TransitionBlit right_blit{};

    std::int32_t pixel_step{};
    std::int32_t speed_numerator_after_frame{};
    std::int32_t actor_offset_a{};
    std::int32_t actor_offset_b{};

    bool completed{};
};

inline constexpr std::int32_t kTransitionLeft = 20;
inline constexpr std::int32_t kTransitionRight = 620;
inline constexpr std::int32_t kTransitionTop = 20;
inline constexpr std::int32_t kTransitionBottom = 400;
inline constexpr std::int32_t kTransitionMaximumSpeedNumerator = 200;

// Exact source-rectangle and acceleration state from
// 0x0041C710 DrawMazeScreenTransition. Legal retail transitions are between
// adjacent West/Middle/East screens.
[[nodiscard]] ScreenTransitionFrame advance_screen_transition(
    ScreenTransitionState& state) noexcept;

} // namespace btb::maze
