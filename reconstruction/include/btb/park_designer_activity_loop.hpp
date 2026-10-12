#pragma once
// Source-level reconstruction of retail 0x00410D10, Park Designer update.
// Original PE32 SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
// All original state decisions and call order are represented; original
// native rendering / editor / resource destruction are provider-owned.
#include "btb/park_designer_data.hpp"
#include "btb/park_designer_scene.hpp"
#include "btb/park_designer_runtime.hpp"
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace btb::park_designer {

struct LoopState {
    SaveData save{};
    // Original runtime-only globals, separate from the persisted 28-value tail.
    bool reset_all_pending{};             // 0x0051C2DC
    bool intro_music_pending{};           // 0x00509354
    bool intro_managed_voice_pending{};   // 0x0051C30C
    bool exit_requested{};                // 0x0044DDB0
    bool shutdown_requested{};            // 0x0051C2E8
    std::int32_t outro_stage{};           // 0x00509368
    std::int32_t queued_voice{-1};        // 0x0050418C
    std::int32_t queued_voice_context{};  // 0x00507B60
    std::int32_t editor_cursor_mode{};    // 0x00507B14
    std::int32_t pending_voice_slot{};    // 0x00441F18 etc. external
    std::int32_t outer_state{};           // 0x0044DE14, only shutdown changes it
};

struct LoopInput {
    bool global_input_blocked{}; // 0x0044DDB0; also mirrored by exit_requested
    bool global_shutdown{};      // 0x0051C2E8
    bool managed_sound_playing{};
    std::uint32_t random_value{}; // x86 rand() source, used modulo 3
};

enum class LoopSoundKind {
    StopIntroSound,
    StartParkBackingTrack,
    ManagedVoice,
    InputInterruptibleVoice,
};
struct LoopSound {
    LoopSoundKind kind{};
    std::int32_t id{-1};
    std::int32_t context{};
};

struct LoopFrame {
    bool input_processing_enabled{true};
    bool requested_draw{};
    bool requested_editor{};
    bool performed_reset{};
    CompactionResult compaction{};
    bool performed_unload{};
    bool completed{};
    bool mark_park_progress{};
    bool return_to_activity_select{};
    std::vector<LoopSound> sound{};
    std::optional<std::string> error{};
};

// Invoked synchronously at the SAME point as original DrawParkDesignerActivity
// and UpdateParkDesignerEditorInteraction. A false result aborts the current
// host frame with an explicit error, not a fabricated rendered/edited frame.
using SceneDraw = std::function<bool(const SaveData&,std::string&)>;
using EditorUpdate = std::function<bool(SaveData&,std::string&)>;

// Pure outer state control flow of 00410D10. Source-level closures for draw
// and editor allow this real state machine to be tested independently and
// installed into GameRoot without pretending DirectDraw is present.
[[nodiscard]] LoopFrame advance_park_designer_loop(
    LoopState& state,const LoopInput& input,
    const SceneDraw& draw,const EditorUpdate& edit);

// Reproduces the persistent/runtime reset block at 00410D10, including the
// asymmetric 5th linked-pond entry which is zero rather than -1.
void reset_park_designer_runtime(LoopState& state) noexcept;

} // namespace btb::park_designer
