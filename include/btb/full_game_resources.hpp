#pragma once

#include "btb/full_game_audio.hpp"
#include "btb/full_game_runtime.hpp"
#include "btb/original_asset_resolver.hpp"

#include <cstddef>
#include <optional>
#include <vector>

namespace btb::full_game {

// Asset lookup belongs between recovered activity output and the renderer /
// DirectSound adapters. This layer does not construct substitute bitmaps,
// play audio, or silently skip requested resources.
struct ResolvedDraw {
    Draw original{};
    AssetResolution source{};
};
struct ResolvedSoundEffect {
    SoundEffect original{};
    std::optional<AssetResolution> source{};
};
struct ResolvedGameEffects {
    std::vector<ResolvedDraw> draws{};
    std::vector<ResolvedSoundEffect> sound{};
    std::size_t unresolved_draws{};
    std::size_t unresolved_audio_assets{};

    [[nodiscard]] bool ready_for_submission() const noexcept {
        return unresolved_draws == 0 && unresolved_audio_assets == 0;
    }
};

// Calls are kept in their source order. For the game's transparent-bitmaps,
// the original Draw::color_keyed flag is forwarded unchanged so the eventual
// renderer applies 0x00FF00FF rather than alpha or opaque blitting.
[[nodiscard]] ResolvedGameEffects resolve_game_frame_assets(
    const OriginalAssetResolver& resolver,
    const std::vector<Draw>& draws,
    const AudioFramePlan& audio);

} // namespace btb::full_game
