#include "btb/recompiled_core_smoke.hpp"
#include "btb/original_runtime16.hpp"
#include "btb/original_float6.hpp"
#include "btb/original_exception9.hpp"
#include "btb/original_editor3.hpp"
#include "btb/original_startup11.hpp"
#include "btb/original_string8.hpp"
#include "btb/original_crt9.hpp"
#include "btb/original_shared_ui8.hpp"
#include "btb/original_services8.hpp"
#include "btb/original_gameplay6.hpp"
#include "btb/original_source9.hpp"
#include "btb/original_gameplay9.hpp"
#include "btb/original_gameplay11.hpp"
#include "btb/original_shared11.hpp"
#include "btb/original_lifecycle12.hpp"
#include "btb/original_activity9.hpp"
#include "btb/original_system14.hpp"
#include "btb/original_media15.hpp"
#include "btb/original_crt10.hpp"
#include "btb/original_runtime8.hpp"
#include "btb/original_active_frame.hpp"
#include "btb/original_runtime5.hpp"
#include "btb/original_crt17.hpp"
#include "btb/original_msvc30.hpp"
#include "btb/original_crt_batch_32.hpp"
#include "btb/original_games_batch_14.hpp"
#include "btb/original_leaf_batch_25.hpp"
#include "btb/original_game_batch_13.hpp"

#include "btb/full_game_audio.hpp"
#include "btb/full_game_bobs_band.hpp"
#include "btb/full_game_fireworks.hpp"
#include "btb/full_game_golf.hpp"
#include "btb/full_game_herding.hpp"
#include "btb/full_game_herding_event_simulation.hpp"
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
    if(!btb::original_editor3::verify_3_original_editor_functions())return false;
    if(!btb::original_exception9::verify_9_original_exception_functions())return false;
    if(!btb::original_startup11::verify_11_original_startup_functions())return false;
    if(!btb::original_float6::verify_6_original_float_functions())return false;
    if(!btb::original_runtime16::verify_16_original_runtime_functions())return false;
    if(!btb::original_string8::verify_8_original_string_functions())return false;
    if(!btb::original_crt9::verify_9_original_crt_functions())return false;
    if(!btb::original_shared_ui8::verify_8_original_shared_ui())return false;
    if(!btb::original_services8::verify_8_original_services())return false;
    if(!btb::original_gameplay6::verify_6_original_gameplay_functions())return false;
    if(!btb::original_source9::verify_9_original_source_functions())return false;
    if(!btb::original_gameplay9::verify_9_original_gameplay_math_functions())return false;
    if(!btb::original_gameplay11::verify_11_gameplay_data_functions())return false;
    if(!btb::original_shared11::verify_11_shared_native_functions())return false;
    if(!btb::original_lifecycle12::verify_12_original_gameplay_lifecycle())return false;
    if(!btb::original_activity9::verify_9_original_activity_functions())return false;
    if(!btb::original_system14::verify_14_original_system_functions())return false;
    if(!btb::original_media15::verify_15_original_media_functions())return false;
    if(!btb::original_crt10::verify_10_original_crt_io_functions())return false;
    if(!btb::original_runtime8::verify_8_original_algorithms())return false;
    if(!btb::original_active_frame::verify_source_frame_control_flow())return false;
    if(!btb::original_runtime5::verify_5_original_runtime_functions())return false;
    if(!btb::original_crt17::verify_17_original_crt_functions())return false;
    if (!btb::original_msvc30::verify_30_original_msvc_functions()) return false;
    if (!btb::original_crt32::verify_32_original_crt_functions()) return false;
    if (!btb::original_games14::verify_14_original_game_systems()) return false;
    if (!btb::original_leaf::verify_25_original_leaf_calls()) return false;
    if (!btb::original_game13::verify_13_original_game_calls()) return false;
    // Force original help loading and its GameRoot caller into game linkage.
    struct HelpValidation final:btb::help::Services {
        bool copied{},voiced{};
        void copy_front_buffer_to_help_surface() override {copied=true;}
        void play_help_voice(std::int32_t sound,std::int32_t priority) override {
            voiced=copied && sound==476 && priority==1;
        }
    } help_services;
    btb::help::State help_state;
    std::istringstream help_data("END");
    btb::help::load_original_help_info(help_state,help_data);
    GameRoot help_root;help_root.globals().dispatcher.current_state=1;
    help_root.begin_original_help(help_state,false,false,help_services);
    if(!help_services.voiced || !help_state.active || help_state.context!=0)return false;
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
    // Force the recovered Herding event/behavior implementation into the
    // executable vtable closure. Its original continuous-motion provider
    // is not available: this must still fail, not synthesize motion.
    root.install(ActivityId::Herding,
                 std::make_unique<HerdingDriver>(
                     std::filesystem::path{},
                     std::make_unique<HerdingEventSimulation>(nullptr),0));

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
        !rejected_missing_source(ActivityId::ParkDesigner) ||
        !rejected_missing_source(ActivityId::Herding)) {
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
