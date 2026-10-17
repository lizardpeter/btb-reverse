#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/spud_skate_runtime.hpp"
#include "btb/spud_skate_presentation.hpp"
#include "btb/retail_crt_random.hpp"

#include <array>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

namespace btb::full_game {

// The score/time contract is clocked by Bink's DECODED retail frame,
// not host wall-clock, 60Hz timers, or an invented frame counter.
// A production implementation must own all five original movie handles,
// render the selected quality surface, and report the true end.bik state.
struct SpudSkateMovieFrame {
    int primary_decoded_frame{-1};
    bool quality_stream_reached_end{};
    bool end_movie_finished{};
    bool four_quality_streams_synchronized{};
};

class SpudSkateMovieSource {
public:
    virtual ~SpudSkateMovieSource()=default;
    [[nodiscard]] virtual bool open(
        const std::array<std::filesystem::path,5>& original_binks,
        std::string& error)=0;
    [[nodiscard]] virtual bool observe(
        SpudSkateMovieFrame& frame,std::string& error)=0;
    [[nodiscard]] virtual bool select_quality(
        spud_skate::StuntQuality quality,std::string& error)=0;
    [[nodiscard]] virtual bool resynchronize_all(
        int decoded_frame,std::string& error)=0;
    [[nodiscard]] virtual bool begin_end_movie(std::string& error)=0;
    [[nodiscard]] virtual bool close(std::string& error)=0;
};

// The tenth retail 68-state activity driver. It can run with a real
// five-Bink decoder, but fails CLOSED without one: no synthetic frames,
// simulated stunts, dummy movie surface, or counterfeit progress writes.
class SpudSkateDriver final : public ActivityDriver {
public:
    SpudSkateDriver(
        std::filesystem::path original_root,
        std::unique_ptr<SpudSkateMovieSource> original_movies,
        retail::OriginalRetailRandom* game_random=nullptr,
        std::function<bool(int)> spud_maze_completed_for_player={})
        : root_(std::move(original_root)),
          movies_(std::move(original_movies)),random_(game_random),
          spud_maze_completed_(std::move(spud_maze_completed_for_player)) {}

    void configure_menu_state(
        const RetailMenuState& menu) noexcept override;
    [[nodiscard]] bool initialize(
        int player_index,std::string& error) override;
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override;
    [[nodiscard]] bool unload(std::string& error) override;

    [[nodiscard]] bool initialized() const noexcept {return active_;}
    [[nodiscard]] int score() const noexcept {return state_.score;}
    [[nodiscard]] spud_skate::PlaybackPhase phase() const noexcept {
        return state_.phase;
    }
    [[nodiscard]] int pass_count() const noexcept {
        return state_.loop_marker_hits;
    }

private:
    [[nodiscard]] int draw_original_rand() noexcept {
        return random_ ? random_->next_rand() : fallback_rng_.next_rand();
    }
    [[nodiscard]] bool queue_stunt_sound(
        const spud_skate::StuntSoundRequest& choice,
        ActivityFrameOutput& out,std::string& error);
    [[nodiscard]] bool queue_result_sounds(
        const spud_skate::ResultPresentation& result,
        ActivityFrameOutput& out,std::string& error);
    void append_authentic_bitmap_draws(ActivityFrameOutput& out) const;

    std::filesystem::path root_{};
    std::unique_ptr<SpudSkateMovieSource> movies_{};
    retail::OriginalRetailRandom* random_{};
    retail::OriginalRetailRandom fallback_rng_{};
    // Source 0x424690 reads retail player-progress slot 59 using the
    // selected player's 400-byte record. The native Windows host
    // provides that source of truth rather than a constant false.
    std::function<bool(int)> spud_maze_completed_{};
    spud_skate::TimingData timing_{};
    spud_skate::SoundData sounds_{};
    spud_skate::RuntimeState state_{};
    int difficulty_{};
    int player_index_{-1};
    bool active_{};
    bool first_audio_{};
};

} // namespace btb::full_game
