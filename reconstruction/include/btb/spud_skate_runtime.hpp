#pragma once

#include "btb/spud_skate_data.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::spud_skate {

inline constexpr std::int32_t kRetailPlaybackPasses = 2;
inline constexpr std::int32_t kRetailScoringOpportunities =
    static_cast<std::int32_t>(kStuntCount) +
    static_cast<std::int32_t>(kStuntCount - 1);

// Retail scores all eight stunts on the first pass. The final stunt's normal
// sound trigger is frame 665, beyond the 660 pass marker, so the first frame-
// 660 branch scores it manually. On pass two, frame 660 enters end.bik before
// that final-stunt score path, leaving 8 + 7 = 15 scoring opportunities.
inline constexpr std::int32_t kRetailMaximumScore =
    kRetailScoringOpportunities *
    static_cast<std::int32_t>(StuntQuality::Good);

enum class PlaybackPhase : std::int32_t {
    SynchronizedRun = 0,
    EndMovie = 1,
    Complete = 2,
};

struct StuntSoundRequest {
    std::size_t stunt{};
    StuntQuality quality{StuntQuality::Bad};
    bool first_pass_final_stunt_special{};
};

struct RuntimeState {
    StuntQuality quality{StuntQuality::Bad}; // 0x00514EA4
    std::int32_t loop_marker_hits{};          // 0x00514EA8
    PlaybackPhase phase{PlaybackPhase::SynchronizedRun}; // 0x00514EAC
    std::int32_t score{};                    // 0x00514EB0
    std::int32_t active_stunt{-1};           // 0x0044656C
    bool result_voice_started{};              // 0x005148B8 != 0
};

struct PlaybackFrameInput {
    std::int32_t frame{};
    bool stunt_press{};
};

struct PlaybackFrameStep {
    std::int32_t frame{};
    StuntQuality quality_before{StuntQuality::Bad};
    StuntQuality quality_after{StuntQuality::Bad};
    std::int32_t active_stunt_before{-1};
    std::int32_t active_stunt_after{-1};

    std::optional<std::size_t> accepted_stunt{};
    std::optional<std::size_t> returned_stunt{};
    std::optional<StuntSoundRequest> sound_request{};

    std::int32_t score_delta{};
    std::int32_t score_after{};

    bool loop_marker_seen{};
    bool first_pass_final_stunt_scored{};
    bool start_end_movie{};

    // The selected quality is also the index of the synchronized Bink stream:
    // 0 bad.bik, 1 normal.bik, 2 ok.bik, 3 good.bik.
    std::int32_t selected_bink_stream{};

    // Retail draws 1.bmp/2.bmp/3.bmp at (267,415) when quality > 0.
    std::optional<std::int32_t> quality_overlay_index{};
};

// Exact state ordering from 0x00424690 UpdateAndDrawSpudSkatePlayback for the
// synchronized-run branch. The caller supplies the current primary Bink frame
// and whether the stunt input was active on this update.
[[nodiscard]] PlaybackFrameStep update_synchronized_run(
    RuntimeState& state,
    const TimingData& timing,
    const SoundData& sounds,
    const PlaybackFrameInput& input) noexcept;

// Returns which stunt's configured trigger frame equals the current frame.
// Retail finds this by locating the most recent stunt start then testing that
// stunt's sound-trigger table entry.
[[nodiscard]] constexpr std::optional<std::size_t> sound_trigger_stunt_at_frame(
    const TimingData& timing,
    const SoundData& sounds,
    std::int32_t frame) noexcept {

    std::optional<std::size_t> prior;
    for (std::size_t i = 0;
         i < static_cast<std::size_t>(timing.stunt_count) && i < kStuntCount;
         ++i) {
        // Retail uses a strict frame > normal_start walk.
        if (frame > timing.windows[i].normal_start) {
            prior = i;
        } else {
            break;
        }
    }

    if (prior && sounds.trigger_frames[*prior] == frame) {
        return prior;
    }
    return std::nullopt;
}

struct ResultPresentation {
    ResultTier tier{ResultTier::Low};
    std::int32_t random_sound_first{};
    std::int32_t random_sound_count{};
    bool mark_spud_skate_complete{true};
};

// The one-shot result branch uses score <10 / <20 / >=20 and the exact
// SS2_SPU_16..22 pools already exposed by result_sound_id().
[[nodiscard]] constexpr ResultPresentation result_presentation(
    std::int32_t score) noexcept {
    switch (result_tier(score)) {
        case ResultTier::Low:
            return {ResultTier::Low, 773, 2, true};
        case ResultTier::Medium:
            return {ResultTier::Medium, 775, 3, true};
        case ResultTier::High:
            return {ResultTier::High, 778, 2, true};
    }
    return {};
}

} // namespace btb::spud_skate
