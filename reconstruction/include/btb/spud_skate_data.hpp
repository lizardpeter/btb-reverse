#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <optional>

namespace btb::spud_skate {

inline constexpr std::size_t kStuntCount = 8;
inline constexpr std::size_t kQualityCount = 4;
inline constexpr std::size_t kSoundsPerQuality = 5;

enum class Difficulty : std::int32_t {
    Easy = 0,
    Medium = 1,
    Hard = 2,
};

enum class StuntQuality : std::int32_t {
    Bad = 0,
    Normal = 1,
    Okay = 2,
    Good = 3,
};

struct StuntWindow {
    std::int32_t normal_start{};
    std::int32_t okay_start{};
    std::int32_t good_start{};
    std::int32_t end_exclusive{};

    [[nodiscard]] bool contains(std::int32_t frame) const noexcept {
        return frame >= normal_start && frame < end_exclusive;
    }

    [[nodiscard]] StuntQuality quality_for_press(
        std::int32_t frame) const noexcept {
        if (frame >= good_start) return StuntQuality::Good;
        if (frame >= okay_start) return StuntQuality::Okay;
        return StuntQuality::Normal;
    }
};

struct TimingData {
    std::int32_t stunt_count{};
    std::array<StuntWindow, kStuntCount> windows{};
    std::array<std::int32_t, kStuntCount> return_to_bad_frames{};
    std::int32_t loop_start_frame{};
    std::int32_t loop_end_frame{};
};

TimingData parse_timing(std::istream& in);

struct SoundData {
    // [stunt][quality][candidate]
    std::array<
        std::array<std::array<std::int32_t, kSoundsPerQuality>, kQualityCount>,
        kStuntCount> sound_ids{};

    std::array<std::int32_t, kStuntCount> trigger_frames{};
};

SoundData parse_sound_info(std::istream& in);

[[nodiscard]] constexpr std::optional<std::size_t> stunt_at_frame(
    const TimingData& data,
    std::int32_t frame) noexcept {
    for (std::size_t i = 0; i < kStuntCount; ++i) {
        if (i >= static_cast<std::size_t>(data.stunt_count)) break;
        if (data.windows[i].contains(frame)) return i;
    }
    return std::nullopt;
}

[[nodiscard]] constexpr std::int32_t sound_id_for_random_slot(
    const SoundData& sounds,
    std::size_t stunt,
    StuntQuality quality,
    std::size_t random_slot) noexcept {
    if (stunt >= kStuntCount || random_slot >= kSoundsPerQuality) return -1;
    return sounds.sound_ids[stunt][static_cast<std::size_t>(quality)][random_slot];
}

inline constexpr std::int32_t kMaximumScore =
    static_cast<std::int32_t>(kStuntCount) *
    static_cast<std::int32_t>(StuntQuality::Good);

[[nodiscard]] constexpr std::int32_t score_for_quality(
    StuntQuality quality) noexcept {
    return static_cast<std::int32_t>(quality);
}

enum class ResultTier : std::int32_t {
    Low,
    Medium,
    High,
};

[[nodiscard]] constexpr ResultTier result_tier(
    std::int32_t score) noexcept {
    return score < 10 ? ResultTier::Low
         : score < 20 ? ResultTier::Medium
                      : ResultTier::High;
}

[[nodiscard]] constexpr std::int32_t result_sound_id(
    std::int32_t score,
    std::int32_t random_index) noexcept {

    switch (result_tier(score)) {
        case ResultTier::Low:
            return random_index >= 0 && random_index < 2
                ? 773 + random_index  // SS2_SPU_16 / 17
                : -1;
        case ResultTier::Medium:
            return random_index >= 0 && random_index < 3
                ? 775 + random_index  // SS2_SPU_18..20
                : -1;
        case ResultTier::High:
            return random_index >= 0 && random_index < 2
                ? 778 + random_index  // SS2_SPU_21 / 22
                : -1;
    }
    return -1;
}

} // namespace btb::spud_skate
