#pragma once

#include <cstdint>

namespace btb::full_game {

// 0x0042CFD0 PreparePlayAgainTransition, called before the individual
// activity's final resource-release and persisted-progress exit path.
//
// The native routine:
// 1. closes/relinquishes the prior activity music group at 0x51C2B4;
// 2. draws the shared replay underlay through 0x51C298's draw method;
// 3. stores 1 into the replay-active latch at 0x51C300;
// 4. calls StopAllManagedSounds at 0x402C90;
// 5. calls PlayManagedSoundById(573, 50, 1) at 0x402CF0;
// 6. marks that sound's managed slot input-interruptible (+0xD98 = 1).
//
// ID 573 is PA_BOB_01.wav in the original Data/sound/binklist.txt.
// Actual DirectDraw/DirectSound calls are still performed by the platform
// host. This is the original source-level order / policy contract.
struct ReplayPreparationPlan {
    bool stop_and_release_activity_music{true};
    bool draw_replay_underlay{true};
    bool set_replay_active_latch{true};
    bool stop_all_managed_sounds{true};
    std::int32_t voice_sound_id{573};
    std::int32_t voice_priority{50};
    std::int32_t voice_arbitration_class{1};
    bool voice_input_interruptible{true};
};

[[nodiscard]] constexpr ReplayPreparationPlan
prepare_retail_play_again_transition() noexcept {
    return {};
}

} // namespace btb::full_game
