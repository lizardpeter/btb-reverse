#pragma once

#include "btb/full_game_effect_pipeline.hpp"
#include "btb/full_game_startup_movies.hpp"
#include "btb/full_game_walkthrough_catalog.hpp"

#include <filesystem>
#include <optional>
#include <string>
#include <utility>

namespace btb::full_game {

// Observations are measurements from the running device, never inferred
// from prior GameRoot effects. The executable uses global Bink and the
// independently looping instruction walkthrough simultaneously.
struct PhysicalFrameObservation {
    ManagedSoundObservation managed{};
    bool any_managed_voice_playing{};
    bool global_movie_finished{};
};

enum class PhysicalPresentationStage {
    ObserveDevice, GameAdvance, MovieResolve, MovieOpen,
    MovieClose, AssetResolve, SubmitAudio, SubmitDraw, Print, Present
};

struct PhysicalPresentationError {
    PhysicalPresentationStage stage{};
    std::string message{};
};

// Strict native-provider boundary. The final Windows DirectDraw7 /
// DirectSound / binkw32.dll implementations satisfy these calls.
// All paths passed to movie operations have already been resolved from the
// authentic installed/disc assets and have no synthetic fallbacks.
//
// Audio commands are delivered BEFORE ordered draws, and the single final
// present belongs to the retail display manager (windowed Blt or Flip).
class NativeGameDevice {
public:
    virtual ~NativeGameDevice() = default;
    [[nodiscard]] virtual bool observe(
        PhysicalFrameObservation& out,
        std::string& error) = 0;
    [[nodiscard]] virtual bool open_global_movie(
        const std::filesystem::path& original_movie,
        std::string& error) = 0;
    [[nodiscard]] virtual bool close_global_movie(std::string& error) = 0;
    [[nodiscard]] virtual bool open_walkthrough_movie(
        const std::filesystem::path& original_movie,
        std::string& error) = 0;
    [[nodiscard]] virtual bool close_walkthrough_movie(
        std::string& error) = 0;
    [[nodiscard]] virtual bool apply_sound(
        const ResolvedSoundEffect& effect,
        std::string& error) = 0;
    // Stateless source-test devices have no physical movie clock.
    [[nodiscard]] virtual bool set_global_movie_paused(bool,std::string& error) {
        error.clear();return true;
    }
    [[nodiscard]] virtual bool set_original_volume(int,std::string& error) {
        error="native volume provider unavailable";return false;
    }
    [[nodiscard]] virtual bool draw_ordered(
        const std::vector<ResolvedDraw>& draws,
        std::string& error) = 0;
    // 0042CFD0 captures the real 640x480 backbuffer into 51C298;
    // 00428E30 redraws that snapshot before the keyed replay art.
    [[nodiscard]] virtual bool capture_replay_underlay(std::string& error) {
        error="native replay underlay capture unavailable";return false;
    }
    [[nodiscard]] virtual bool draw_replay_ordered(
        const std::vector<ResolvedDraw>&,std::string& error) {
        error="native replay underlay rendering unavailable";return false;
    }
    // Called ONLY after the authentic ordered DD7 frame has been drawn.
    // Platform implementations must report actual printing errors. A user
    // cancelling the native dialog is normal, and is not called 'printed'.
    [[nodiscard]] virtual bool print_current_frame(std::string& error) {
        error="native original-frame printer unavailable";
        return false;
    }
    // Activity initializers/unloaders in retail release their owned bitmap
    // groups. The native cache must not accumulate old activities forever.
    // Stateless devices need no cache cleanup. Preserve the composed frame
    // and captured replay underlay; neither is a cached source bitmap.
    virtual void release_activity_bitmaps() noexcept {}
    [[nodiscard]] virtual bool present(std::string& error) = 0;
};

struct NativeSessionFrame {
    PreparedGameFrame prepared{};
    bool submitted{};
    bool ready_for_present{}; // render complete; original frame driver owns present
    bool global_movie_opened{};
    bool walkthrough_opened{};
    bool walkthrough_closed{};
    std::optional<PhysicalPresentationError> failure{};
};

// Owns the actual outer-state / audio / asset / independent movie contract
// for one retail game session. Its error state is sticky: physical failure
// cannot be converted into fabricated 'played' sounds or 'drawn' frames.
// The GameRoot itself remains the sole authority for menu/gameplay state.
class NativeGameSession {
public:
    NativeGameSession(
        GameRoot& root,
        NativeGameDevice& device,
        OriginalAssetResolver resolver,
        OriginalStartupMovieCatalog startups,
        OriginalWalkthroughCatalog walkthroughs,
        bool skip_video = false)
        : root_(root),device_(device),
          effects_(std::move(resolver)),
          startups_(std::move(startups)),
          walkthroughs_(std::move(walkthroughs)),
          skip_video_(skip_video) {}

    [[nodiscard]] CatalogLoadResult load_sound_catalog(
        std::istream& original_list) {
        return effects_.load_sound_catalog(original_list);
    }

    [[nodiscard]] NativeSessionFrame tick(
        ActivityFrameInput input,
        bool primary_input_pulse,
        bool secondary_input_pulse,
        bool defer_present=false);

    // Split native render from final Flip/Blt so retail 00402440 can
    // check DirectDraw cooperative status in between. Existing callers
    // retain normal tick(...)-and-present behavior by default.
    [[nodiscard]] bool present_prepared(std::string& error);
    void discard_prepared() noexcept { pending_present_=false; }


    [[nodiscard]] bool failed() const noexcept { return failed_; }
    // Explicit non-retail development mode: no movie is decoded or drawn.
    [[nodiscard]] bool skipping_video() const noexcept { return skip_video_; }
    [[nodiscard]] bool has_walkthrough() const noexcept {
        return walkthrough_open_;
    }

private:
    [[nodiscard]] bool fail(
        NativeSessionFrame& frame,
        PhysicalPresentationStage stage,
        std::string message);

    GameRoot& root_;
    NativeGameDevice& device_;
    GameEffectPipeline effects_;
    OriginalStartupMovieCatalog startups_{};
    OriginalWalkthroughCatalog walkthroughs_{};
    bool skip_video_{};
    bool walkthrough_open_{};
    bool global_movie_paused_{};
    // Spud Skate tears down its movie surfaces during GameRoot::advance.
    // Keep its last actually composed frame before that teardown.
    bool skate_snapshot_valid_{};
    bool failed_{};
    bool pending_present_{};
};

} // namespace btb::full_game
