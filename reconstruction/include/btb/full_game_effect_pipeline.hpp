#pragma once

#include "btb/full_game_audio.hpp"
#include "btb/full_game_resources.hpp"
#include "btb/full_game_runtime.hpp"
#include "btb/original_asset_resolver.hpp"

#include <istream>
#include <utility>

namespace btb::full_game {

// A complete *source-level* bridge from GameRoot frame effects to the two
// platform providers used throughout the retail executable. No preview
// renderer, no synthesized WAVs, and no platform COM objects are used here.
//
// A future host owns the actual DirectDraw/DirectSound objects, supplies
// each slot's observed playback state, and consumes the ordered results.
struct PreparedGameFrame {
    GameFrame game{};
    AudioFramePlan audio_policy{};
    ResolvedGameEffects resources{};
    bool missing_catalog_ids{};
};

class GameEffectPipeline {
public:
    explicit GameEffectPipeline(OriginalAssetResolver resolver)
        : resolver_(std::move(resolver)) {}

    [[nodiscard]] CatalogLoadResult load_sound_catalog(std::istream& source) {
        return audio_.load_catalog(source);
    }

    [[nodiscard]] PreparedGameFrame prepare(
        GameFrame frame,
        ManagedSoundObservation observed,
        bool primary_input_pulse,
        bool secondary_input_pulse);

    [[nodiscard]] const AudioEffectPlanner& audio() const noexcept {
        return audio_;
    }
    [[nodiscard]] const OriginalAssetResolver& assets() const noexcept {
        return resolver_;
    }

private:
    OriginalAssetResolver resolver_;
    AudioEffectPlanner audio_{};
};

} // namespace btb::full_game
