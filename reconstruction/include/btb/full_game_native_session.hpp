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
    MovieClose, AssetResolve, SubmitAudio, SubmitDraw, Present
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
    [[nodiscard]] virtual bool open_walkthrough_movie(
        const std::filesystem::path& original_movie,
        const std::filesystem::path& original_help_wav,
        std::string& error) = 0;
    [[nodiscard]] virtual bool close_walkthrough_movie(
        std::string& error) = 0;
    [[nodiscard]] virtual bool apply_sound(
        const ResolvedSoundEffect& effect,
        std::string& error) = 0;
    [[nodiscard]] virtual bool draw_ordered(
        const std::vector<ResolvedDraw>& draws,
        std::string& error) = 0;
    [[nodiscard]] virtual bool present(std::string& error) = 0;
};

struct NativeSessionFrame {
    PreparedGameFrame prepared{};
    bool submitted{};
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
        OriginalWalkthroughCatalog walkthroughs)
        : root_(root),device_(device),
          effects_(std::move(resolver)),
          startups_(std::move(startups)),
          walkthroughs_(std::move(walkthroughs)) {}

    [[nodiscard]] CatalogLoadResult load_sound_catalog(
        std::istream& original_list) {
        return effects_.load_sound_catalog(original_list);
    }

    [[nodiscard]] NativeSessionFrame tick(
        ActivityFrameInput input,
        bool primary_input_pulse,
        bool secondary_input_pulse);

    [[nodiscard]] bool failed() const noexcept { return failed_; }
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
    bool walkthrough_open_{};
    bool failed_{};
};

} // namespace btb::full_game
