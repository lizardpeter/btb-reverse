#include "btb/full_game_resources.hpp"

#include <utility>
#include <unordered_map>

namespace btb::full_game {
namespace {

bool needs_asset(SoundEffectKind kind) noexcept {
    switch (kind) {
    case SoundEffectKind::LoadManagedWav:
    case SoundEffectKind::StartBackingWav:
    case SoundEffectKind::EnsureActivityLoopWav:
    case SoundEffectKind::PlayDirectSample:
        return true;
    case SoundEffectKind::StopRewindManagedSlot:
    case SoundEffectKind::ReleaseManagedSlot:
    case SoundEffectKind::PlayManagedSlot:
    case SoundEffectKind::StopActivityLoopWav:
    case SoundEffectKind::StopBackingWav:
        return false;
    }
    return false;
}

} // namespace

ResolvedGameEffects resolve_game_frame_assets(
    const OriginalAssetResolver& resolver,
    const std::vector<Draw>& draws,
    const AudioFramePlan& audio) {

    ResolvedGameEffects out;
    out.draws.reserve(draws.size());
    out.sound.reserve(audio.operations.size());

    // A frame often draws the same sprite sheet or sound several times.
    // Resolve each exact request once within this frame only, keeping
    // strict path validation, actual spelling and missing-file diagnostics.
    // No persistent negative/positive cache masks filesystem changes.
    std::unordered_map<std::string,AssetResolution> frame_paths;
    const auto resolve_once=[&](const std::string& name)->AssetResolution {
        if(const auto it=frame_paths.find(name);it!=frame_paths.end())
            return it->second;
        return frame_paths.emplace(name,resolver.resolve(name)).first->second;
    };
    // Layer order is significant. A note overlay may be covered by a machine
    // or the toolbar; never reorder by asset filename to reduce I/O.
    for (const auto& draw : draws) {
        auto source = resolve_once(draw.source_asset);
        if (!source.found()) {
            ++out.unresolved_draws;
        }
        out.draws.push_back({draw, std::move(source)});
    }

    // Managed buffer slot commands and backing-stop commands do not carry a
    // filename; trying to resolve them as files would be an integration bug.
    for (const auto& effect : audio.operations) {
        ResolvedSoundEffect resolved;
        resolved.original = effect;
        if (needs_asset(effect.kind)) {
            resolved.source = resolve_once(effect.filename);
            if (!resolved.source->found()) {
                ++out.unresolved_audio_assets;
            }
        }
        out.sound.push_back(std::move(resolved));
    }
    return out;
}

} // namespace btb::full_game
