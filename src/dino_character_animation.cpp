#include "btb/dino_character_animation.hpp"

#include <algorithm>

namespace btb::dino {
namespace {

constexpr std::int32_t mode_index(CharacterAnimationMode mode) noexcept {
    return static_cast<std::int32_t>(mode);
}

constexpr Character character_from_feedback_roll(std::int32_t roll) noexcept {
    // The original uses signed divide-by-3 arithmetic. For the retail
    // rand()%6 domain this is exactly: 0..2 Ellis, 3..5 Bob.
    return roll < 3 ? Character::Ellis : Character::Bob;
}

} // namespace

void CharacterAnimationChannel::queue(CharacterAnimationMode mode) noexcept {
    pending_mode_ = mode_index(mode);
}

void CharacterAnimationChannel::advance() noexcept {
    ++tick_counter_;
    if (tick_counter_ > 6) {
        tick_counter_ = 0;
        ++frame_;
    }

    if (pending_mode_ >= 0) {
        mode_ = static_cast<CharacterAnimationMode>(pending_mode_);
        pending_mode_ = -1;
        frame_ = kCharacterFrameRanges[static_cast<std::size_t>(mode_)].first;
    }

    const auto range = kCharacterFrameRanges[static_cast<std::size_t>(mode_)];
    if (frame_ < range.first || frame_ >= range.end_exclusive) {
        // The retail routine checks pending again here. In the single-threaded
        // frame path there is normally no new request between the two checks,
        // so a completed non-idle animation returns to Idle.
        if (pending_mode_ >= 0) {
            mode_ = static_cast<CharacterAnimationMode>(pending_mode_);
            pending_mode_ = -1;
        } else {
            mode_ = CharacterAnimationMode::Idle;
        }
        frame_ = kCharacterFrameRanges[static_cast<std::size_t>(mode_)].first;
    }
}

void CharacterAnimations::queue_pickup(
    std::int32_t mode_random_bit,
    std::int32_t character_random_bit) noexcept {
    const auto mode = mode_random_bit & 1
        ? CharacterAnimationMode::PickupB
        : CharacterAnimationMode::PickupA;
    const auto character = character_random_bit & 1
        ? Character::Ellis
        : Character::Bob;
    channel(character).queue(mode);
}

Character CharacterAnimations::queue_correct_drop(std::int32_t feedback_roll) noexcept {
    const auto character = character_from_feedback_roll(feedback_roll);
    channel(character).queue(CharacterAnimationMode::CorrectDrop);
    return character;
}

Character CharacterAnimations::queue_wrong_drop(
    std::int32_t feedback_roll,
    std::int32_t animation_random_bit) noexcept {
    const auto character = character_from_feedback_roll(feedback_roll);
    auto& selected = channel(character);

    if (selected.mode() == CharacterAnimationMode::Idle) {
        const auto mode = animation_random_bit & 1
            ? CharacterAnimationMode::WrongDropB
            : CharacterAnimationMode::WrongDropA;
        selected.queue(mode);
    }

    return character;
}

void CharacterAnimations::advance() noexcept {
    channels_[0].advance();
    channels_[1].advance();
}

} // namespace btb::dino
