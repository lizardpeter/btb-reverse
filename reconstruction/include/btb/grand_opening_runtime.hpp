#pragma once

#include "btb/grand_opening_sequence.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>

namespace btb::grand_opening {

inline constexpr std::int32_t kPlaybackPreviousStepSentinel = 999999;
inline constexpr std::int32_t kBobsBandSharedCompletionCode = 6;

struct PlaybackTrigger {
    std::size_t pitch_row{};
    std::size_t second{};
    MachineType machine_type{MachineType::Roley1Second};
    std::int32_t loaded_sound_slot{};
    Machine machine{Machine::Roley};
    std::int32_t machine_animation_state_written{};
};

struct PlaybackSecondStep {
    std::int32_t second{-1};
    bool timeline_complete{};
    bool second_changed{};
    std::vector<PlaybackTrigger> triggers{};
};

// Exact one-second sound-trigger branch in
// 0x0041FC90 PlayAndDrawGrandOpeningComposition.
//
// The caller owns the five machine-animation state values corresponding to
// Roley/Muck/Lofty/Dizzy/Scoop. Retail writes parity+1 (1 for short, 2 for
// long) only when that machine's animation state is currently zero.
[[nodiscard]] PlaybackSecondStep update_playback_second(
    const Composition& composition,
    std::int32_t elapsed_centiseconds,
    std::int32_t previous_elapsed_centiseconds,
    std::array<std::int32_t,5>& machine_animation_states);

struct PreparePlaybackStep {
    ActivityState next_state{ActivityState::Playing};
    std::size_t progress_index{};
    std::string_view backing_track{};
    std::int32_t previous_elapsed_centiseconds{
        kPlaybackPreviousStepSentinel};
    std::int32_t shared_completion_code{kBobsBandSharedCompletionCode};
};

// Exact state-8 setup: mark current conductor progress, capture the performance
// counter start externally, start the conductor backing track, initialize the
// previous elapsed-time sentinel, and advance to state 9.
[[nodiscard]] constexpr PreparePlaybackStep prepare_playback(
    std::size_t zero_based_player_profile,
    Conductor conductor) noexcept {
    return {
        ActivityState::Playing,
        conductor_progress_index(zero_based_player_profile, conductor),
        backing_track_filename(conductor),
        kPlaybackPreviousStepSentinel,
        kBobsBandSharedCompletionCode,
    };
}

inline constexpr std::int32_t kConductorPlaybackStartFrame = 50;
inline constexpr std::array<std::int32_t, kConductorCount>
kConductorPlaybackEndExclusive{{90, 85, 105}};
inline constexpr std::int32_t kConductorPlaybackFrameTicks = 4;

struct ConductorAnimationState {
    std::int32_t frame{};
    std::int32_t tick{};
};

struct ConductorAnimationStep {
    bool frame_advanced{};
    bool wrapped{};
    std::int32_t frame{};
    std::int32_t tick{};
};

// Exact playback animation clock in PlayAndDrawGrandOpeningComposition.
// The tick increments every update. When it exceeds 3, retail advances one
// frame, clamps/wraps the active conductor into its playback range, and resets
// the tick to zero. All three conductor playback ranges begin at frame 50.
[[nodiscard]] constexpr ConductorAnimationStep tick_conductor_animation(
    Conductor conductor,
    ConductorAnimationState& state) noexcept {

    ConductorAnimationStep result;
    ++state.tick;

    if (state.tick > kConductorPlaybackFrameTicks - 1) {
        state.tick = 0;
        ++state.frame;
        result.frame_advanced = true;

        const auto index = static_cast<std::size_t>(conductor);
        const auto end = index < kConductorPlaybackEndExclusive.size()
            ? kConductorPlaybackEndExclusive[index]
            : kConductorPlaybackStartFrame + 1;

        if (state.frame >= end) {
            state.frame = kConductorPlaybackStartFrame;
            result.wrapped = true;
        }
        if (state.frame < kConductorPlaybackStartFrame) {
            state.frame = kConductorPlaybackStartFrame;
        }
    }

    result.frame = state.frame;
    result.tick = state.tick;
    return result;
}

enum class EditorCursorKind {
    Normal,
    Machine,
    Delete,
};

struct EditorCursor {
    EditorCursorKind kind{EditorCursorKind::Normal};
    std::optional<MachineType> machine{};
};

struct EditorRuntimeState {
    ActivityState activity_state{ActivityState::Edit};
    MachineType selected_machine{MachineType::Roley1Second};
    bool delete_mode{};
    bool delete_interaction_latch{};
    bool play_pending{};
    std::int32_t previous_toolbar_hover{-1};
    EditorCursor cursor{};
    std::array<std::int32_t,5> machine_animation_states{};
};

struct PaletteActionStep {
    bool toggled_off{};
    bool preview_machine_sound{};
    std::int32_t machine_animation_state_written{};
};

// Exact state-0 palette action path in HandleBobsBandEditorAction. Selecting a
// brick cancels delete mode, installs that brick cursor, previews its direct
// machine WAV, enters state 1, and kicks the matching machine animation only
// when that machine is currently idle.
[[nodiscard]] PaletteActionStep select_palette_from_edit(
    EditorRuntimeState& state,
    MachineType type) noexcept;

// Exact state-1 palette path in UpdateBobsBandBrickCursorAndDelete. Clicking the
// already-selected type toggles the cursor/state off. Selecting a different
// type changes cursor/type and previews its WAV, but unlike the state-0 path it
// does not kick a machine animation.
[[nodiscard]] PaletteActionStep select_palette_while_selected(
    EditorRuntimeState& state,
    MachineType type) noexcept;

struct GridActionStep {
    bool placement_attempted{};
    bool placed{};
    bool removed_existing{};
    bool picked_up_existing{};
    bool swapped_existing{};
    bool restored_existing_after_failed_swap{};
    std::optional<MachineType> removed_type{};
};

// Exact grid action 11 behavior, including edit/delete pickup and the selected
// brick replacement path. Continuation cells are owner-aware in edit/delete;
// in state 1 retail attempts placement directly on continuation cells and the
// occupied-cell check rejects it.
[[nodiscard]] GridActionStep handle_grid_action(
    EditorRuntimeState& state,
    Composition& composition,
    std::size_t row,
    std::size_t second) noexcept;

[[nodiscard]] inline bool apply_clear_all_confirmation(
    Composition& composition,
    bool confirmed_yes) noexcept {
    if (!confirmed_yes) {
        return false;
    }
    composition.clear();
    return true;
}

struct ToolbarRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

inline constexpr std::array<ToolbarRect,4> kToolbarRects{{
    {324,416,378,472}, // Play
    {260,416,314,472}, // Stop
    {103,416,157,472}, // Clear All
    {481,416,535,472}, // Delete
}};

inline constexpr std::int32_t kClearAllConfirmationContext = 2;

[[nodiscard]] constexpr std::int32_t conductor_voice_base(
    Conductor conductor) noexcept {
    return 510 + static_cast<std::int32_t>(conductor) * 13;
}

enum class ToolbarVisual {
    None,
    Hover,
    Pressed,
};

enum class ToolbarActionKind {
    None,
    PlayVoicePending,
    EnterPreparePlayback,
    StopPlayback,
    OpenClearAllConfirmation,
    EnterDeleteMode,
    LeaveDeleteMode,
};

struct ToolbarInput {
    std::int32_t mouse_x{};
    std::int32_t mouse_y{};
    bool click_active{};
    bool pressed_visual{};
    bool any_managed_sound_playing{};
    std::int32_t random_mod_2{};
};

struct ToolbarStep {
    ToolbarActionKind action{ToolbarActionKind::None};
    std::optional<ToolbarControl> control{};
    ToolbarVisual visual{ToolbarVisual::None};
    // Hover is processed before click dispatch, so the first clicked frame on
    // a control can produce both requests.
    std::optional<std::int32_t> hover_sound_id{};
    std::optional<std::int32_t> action_sound_id{};
    bool stop_all_managed_sounds{};
    bool stop_backing_track{};
    bool open_confirmation{};
    std::int32_t confirmation_context{-1};
};

// Exact 0x0041F820 toolbar controller. Play is voice-gated: its click starts
// the conductor-specific Play voice and sets play_pending; state 8 is entered
// only on a later update after all managed sounds have become idle.
[[nodiscard]] ToolbarStep update_toolbar(
    EditorRuntimeState& state,
    Conductor conductor,
    const ToolbarInput& input) noexcept;

struct PlayingStateStep {
    ActivityState next_state{ActivityState::Playing};
    bool stop_backing_track{};
};

// During state 9, the Stop toolbar action returns directly to edit mode.
// Retail also returns to edit mode automatically when the conductor backing
// track is no longer playing.
[[nodiscard]] constexpr PlayingStateStep update_playing_state(
    bool stop_control_activated,
    bool backing_track_is_playing) noexcept {
    if (stop_control_activated) {
        return {ActivityState::Edit, true};
    }
    if (!backing_track_is_playing) {
        return {ActivityState::Edit, false};
    }
    return {};
}

inline constexpr std::int32_t kMusicChooserOuterState = 0x2A;
inline constexpr std::int32_t kPlayAgainOuterState = 0x3C;
inline constexpr std::int32_t kSharedMovieOuterState = 0x40;
inline constexpr std::int32_t kBobsBandPlayAgainContext = 0x2E;

struct OuterActivityInput {
    ActivityState activity_state{ActivityState::Edit};
    bool application_quit_requested{};
    bool leave_activity_requested{};
    bool shared_completion_code_present{};
};

struct OuterActivityStep {
    bool return_abort{};
    bool save_and_unload{};
    bool clear_leave_activity_request{};
    bool prepare_play_again{};
    bool clear_shared_transition_flag{};
    std::optional<std::int32_t> outer_state{};
    std::optional<std::int32_t> saved_frontend_state{};
    std::optional<std::int32_t> play_again_context{};
};

// Exact exit/completion decisions in UpdateBobsBandActivity. All quit/leave
// paths save the current 5x24 composition before returning. A leave request
// normally routes to the Music chooser (0x2A), but any active shared completion
// code overrides that route with the shared movie transition (0x40).
[[nodiscard]] constexpr OuterActivityStep update_outer_activity(
    const OuterActivityInput& input) noexcept {

    OuterActivityStep step;

    if (input.application_quit_requested ||
        input.leave_activity_requested) {
        step.return_abort = true;
        step.save_and_unload = true;

        if (input.leave_activity_requested) {
            step.clear_leave_activity_request = true;
            step.saved_frontend_state = kMusicChooserOuterState;
            if (!input.shared_completion_code_present) {
                step.outer_state = kMusicChooserOuterState;
            }
        }

        if (input.shared_completion_code_present) {
            step.outer_state = kSharedMovieOuterState;
        }
        return step;
    }

    if (input.activity_state == ActivityState::ExitToPlayAgain) {
        step.return_abort = true;
        step.save_and_unload = true;
        step.prepare_play_again = true;
        step.outer_state = kPlayAgainOuterState;
        step.play_again_context = kBobsBandPlayAgainContext;
        step.clear_shared_transition_flag = true;
        return step;
    }

    return step;
}

} // namespace btb::grand_opening
