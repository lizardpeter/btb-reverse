#pragma once

#include <array>
#include <cstdint>

namespace btb::dino {

enum class Character : std::int32_t {
    Bob = 0,
    Ellis = 1,
};

enum class CharacterAnimationMode : std::int32_t {
    Idle = 0,
    PickupA = 1,
    PickupB = 2,
    WrongDropA = 3,
    WrongDropB = 4,
    CorrectDrop = 5,
};

struct FrameRange {
    std::int32_t first{};
    std::int32_t end_exclusive{};
};

inline constexpr std::array<FrameRange, 6> kCharacterFrameRanges{{
    {80, 100},
    {0, 20},
    {40, 60},
    {25, 35},
    {65, 75},
    {110, 131},
}};

struct CharacterSheet {
    std::int32_t frame_width{};
    std::int32_t frame_height{};
};

inline constexpr CharacterSheet kBobSheet{113, 97};
inline constexpr CharacterSheet kEllisSheet{93, 105};

class CharacterAnimationChannel {
public:
    void queue(CharacterAnimationMode mode) noexcept;
    void advance() noexcept;

    [[nodiscard]] CharacterAnimationMode mode() const noexcept { return mode_; }
    [[nodiscard]] std::int32_t frame() const noexcept { return frame_; }
    [[nodiscard]] std::int32_t tick_counter() const noexcept { return tick_counter_; }
    [[nodiscard]] bool has_pending() const noexcept { return pending_mode_ >= 0; }

private:
    std::int32_t frame_{0};
    std::int32_t tick_counter_{0};
    CharacterAnimationMode mode_{CharacterAnimationMode::Idle};
    std::int32_t pending_mode_{-1};
};

class CharacterAnimations {
public:
    [[nodiscard]] CharacterAnimationChannel& channel(Character c) noexcept {
        return channels_[static_cast<std::size_t>(c)];
    }
    [[nodiscard]] const CharacterAnimationChannel& channel(Character c) const noexcept {
        return channels_[static_cast<std::size_t>(c)];
    }

    // Retail selection behavior: first random bit chooses pickup mode 1/2,
    // second random bit chooses Bob/Ellis.
    void queue_pickup(std::int32_t mode_random_bit, std::int32_t character_random_bit) noexcept;

    // Retail correct-drop behavior: feedback_roll is rand() % 6.
    // 0..2 -> Ellis, 3..5 -> Bob, always animation mode 5.
    [[nodiscard]] Character queue_correct_drop(std::int32_t feedback_roll) noexcept;

    // Retail wrong-drop behavior: feedback_roll is rand() % 6 and
    // animation_random_bit is rand() % 2. The reaction is queued only if
    // the selected character is currently in idle mode.
    [[nodiscard]] Character queue_wrong_drop(
        std::int32_t feedback_roll,
        std::int32_t animation_random_bit) noexcept;

    void advance() noexcept;

private:
    std::array<CharacterAnimationChannel, 2> channels_{};
};

[[nodiscard]] constexpr std::int32_t correct_drop_sound_id(std::int32_t roll) noexcept {
    return 164 + roll;
}

[[nodiscard]] constexpr std::int32_t wrong_drop_sound_id(std::int32_t roll) noexcept {
    return 170 + roll;
}

[[nodiscard]] constexpr std::int32_t intro_sound_id(std::int32_t difficulty) noexcept {
    return 185 + difficulty;
}

[[nodiscard]] constexpr std::int32_t completion_sound_id(std::int32_t difficulty) noexcept {
    return 188 + difficulty;
}

} // namespace btb::dino
