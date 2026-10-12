#include "btb/recompiled_core_smoke.hpp"

#include "btb/full_game_audio.hpp"
#include "btb/full_game_bobs_band.hpp"
#include "btb/full_game_fireworks.hpp"
#include "btb/full_game_golf.hpp"
#include "btb/full_game_park_designer.hpp"
#include "btb/full_game_runtime.hpp"
#include "btb/sound_manager.hpp"

#include <filesystem>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace btb::full_game {

bool verify_linked_recompiled_core() {
    // The real 68-state GameRoot is called; metadata-only architecture
    // registries or the early Dinosaur-only Session are NOT involved.
    GameRoot root{};
    if (!root.select_profile(0)) {
        return false;
    }

    // Construct four actual game implementations. Constructing and installing
    // their virtual ActivityDriver objects forces their implementation
    // vtables and lifecycle code into the link, instead of merely compiling
    // separate behavioral tests.
    //
    // Empty paths are deliberately used to verify that the real initializer
    // REJECTS unavailable retail game files. Never claim a synthetic activity
    // loaded or substitute imaginary maps/assets.
    root.install(ActivityId::Fireworks,
                 std::make_unique<FireworksDriver>(
                     std::filesystem::path{},std::filesystem::path{}));
    root.install(ActivityId::Golf,
                 std::make_unique<GolfDriver>(
                     std::filesystem::path{},0));
    root.install(ActivityId::BobsBand,
                 std::make_unique<BobsBandDriver>(
                     std::filesystem::path{},std::filesystem::path{}));
    root.install(ActivityId::ParkDesigner,
                 std::make_unique<ParkDesignerDriver>(
                     std::filesystem::path{},std::filesystem::path{}));

    const auto rejected_missing_source = [&](ActivityId id) {
        const auto& entry=kActivities[static_cast<std::size_t>(id)];
        root.set_outer_state(entry.init);
        const auto step=root.advance({});
        // We demand a real file/provider failure, not a fabricated successful
        // "frame" or a missing-driver error. No source files are embedded.
        return step.kind == FrameKind::ActivityFailed &&
               !step.error.empty();
    };

    if (!rejected_missing_source(ActivityId::Fireworks) ||
        !rejected_missing_source(ActivityId::Golf) ||
        !rejected_missing_source(ActivityId::BobsBand) ||
        !rejected_missing_source(ActivityId::ParkDesigner)) {
        return false;
    }

    // Next, execute the SINGLE shared original 80-slot managed-sound engine,
    // reached through the same AudioEffectPlanner consumed by GameEffectPipeline
    // and NativeGameSession. 0x00402CF0 / 0x00402E10 execute here rather than
    // any gameplay-specific replay "event-model" duplicate.
    AudioEffectPlanner audio{};
    std::istringstream original_catalog(
        "ORIGINAL_A.wav 510\n"
        "ORIGINAL_B.wav 511\n");
    if (!audio.load_catalog(original_catalog).success) {
        return false;
    }
    const std::vector<Audio> request{{
        AudioOperation::ManagedSoundId,{},510,50,2
    }};
    const auto effects=audio.plan(request,{});
    if (effects.managed_results.size()!=1 ||
        effects.managed_results.front().status!=1 ||
        effects.operations.size()!=3 ||
        effects.operations.front().kind!=SoundEffectKind::LoadManagedWav ||
        effects.operations[1].kind!=SoundEffectKind::PlayManagedSlot ||
        effects.operations[2].kind!=SoundEffectKind::PlayManagedSlot ||
        audio.manager().mapped_slot(510)!=0 ||
        audio.manager().state(0)!=sound::SlotState::Active) {
        return false;
    }

    // Verify the original 0x00402C20 and 0x00402C60 lookup routines operate
    // on the manager state established by the actual 00402CF0 implementation.
    std::array<bool,sound::kManagedSlotCount> observed{};
    if (sound::any_managed_sound_playing(audio.manager(),observed)) {
        return false;
    }
    observed[0]=true;
    if (!sound::is_sound_id_playing(audio.manager(),510,observed) ||
        !sound::any_managed_sound_playing(audio.manager(),observed)) {
        return false;
    }
    return verify_integrated_activity_fixtures();
}

} // namespace btb::full_game
