#pragma once

#include "btb/park_designer_data.hpp"

#include <cstdint>

namespace btb::park_designer {

inline constexpr std::int32_t kFountainFrameWidth = 152;
inline constexpr std::int32_t kFountainFramesPerVariant = 13;
inline constexpr std::int32_t kFountainLoopFirstFrame = 8;
inline constexpr std::int32_t kFountainLoopEndExclusive = 13;
inline constexpr std::int32_t kFountainCountdownReset = 9;

struct FountainAnimationStep {
    bool active{};
    bool frame_advanced{};
    bool entered_loop_phase{};
    bool linked_primary_frame_advanced{};
    std::int32_t frame{};
    std::int32_t phase{};
    std::int32_t countdown{};
};

// Exact object-code-7 state update from 0x0040D900. The selected primary Pond
// record's +0x40 value is passed by reference because retail advances that
// linked record during the fountain's phase-0 startup.
[[nodiscard]] constexpr FountainAnimationStep update_fountain_animation(
    RetailObjectRecord32& fountain,
    std::int32_t& linked_primary_visual_frame) noexcept {

    FountainAnimationStep result;
    result.active = true;

    if (fountain.fountain_phase == 0) {
        --fountain.fountain_frame_countdown;
        if (fountain.fountain_frame_countdown <= 0) {
            fountain.fountain_frame_countdown = kFountainCountdownReset;
            ++fountain.visual_frame_or_segment;
            result.frame_advanced = true;

            if (fountain.visual_frame_or_segment >= 5) {
                ++linked_primary_visual_frame;
                result.linked_primary_frame_advanced = true;
            }

            if (fountain.visual_frame_or_segment >= kFountainLoopFirstFrame) {
                fountain.fountain_phase = 1;
                result.entered_loop_phase = true;

                if (linked_primary_visual_frame < 4) {
                    ++linked_primary_visual_frame;
                    result.linked_primary_frame_advanced = true;
                }
            }
        }
    } else if (fountain.fountain_phase == 1) {
        --fountain.fountain_frame_countdown;
        if (fountain.fountain_frame_countdown <= 0) {
            fountain.fountain_frame_countdown = kFountainCountdownReset;
            ++fountain.visual_frame_or_segment;
            result.frame_advanced = true;
            if (fountain.visual_frame_or_segment >=
                kFountainLoopEndExclusive) {
                fountain.visual_frame_or_segment =
                    kFountainLoopFirstFrame;
            }
        }
    }

    result.frame = fountain.visual_frame_or_segment;
    result.phase = fountain.fountain_phase;
    result.countdown = fountain.fountain_frame_countdown;
    return result;
}

struct FountainSourceRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

// Retail packs 13 fountain frames per bound_variant, each 152 pixels wide.
[[nodiscard]] constexpr FountainSourceRect fountain_source_rect(
    const RetailObjectRecord32& fountain) noexcept {
    const auto left =
        (fountain.bound_variant * kFountainFramesPerVariant +
         fountain.visual_frame_or_segment) *
        kFountainFrameWidth;
    return {left, 0, left + kFountainFrameWidth, kFountainFrameWidth};
}

enum class CompletionAction : std::int32_t {
    None,
    PlayClosingLine,
    SaveUnloadAndExit,
};

struct CompletionStep {
    std::int32_t stage{};
    CompletionAction action{CompletionAction::None};
    std::int32_t sound_id{-1};
};

// Retail global 0x00509368:
// 0 -> play one of DYP_G_BOB_12..14 (246..248)
// 1 -> wait for managed audio to finish
// 2 -> save/unload and return from the activity
[[nodiscard]] constexpr CompletionStep completion_step(
    std::int32_t stage,
    bool any_managed_sound_playing,
    std::int32_t random_mod_3) noexcept {

    if (stage == 0) {
        if (random_mod_3 < 0 || random_mod_3 > 2) {
            return {stage, CompletionAction::None, -1};
        }
        return {
            1,
            CompletionAction::PlayClosingLine,
            246 + random_mod_3,
        };
    }

    if (stage == 1) {
        if (any_managed_sound_playing) {
            return {1, CompletionAction::None, -1};
        }
        return {2, CompletionAction::None, -1};
    }

    if (stage == 2) {
        return {2, CompletionAction::SaveUnloadAndExit, -1};
    }

    return {stage, CompletionAction::None, -1};
}

[[nodiscard]] bool has_any_placed_object(const SaveData& data) noexcept;

} // namespace btb::park_designer
