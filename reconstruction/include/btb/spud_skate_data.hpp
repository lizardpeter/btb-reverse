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

} // namespace btb::spud_skate
