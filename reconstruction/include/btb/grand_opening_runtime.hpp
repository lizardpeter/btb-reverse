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

inline constexpr std::int32_t kDeleteWallMinYExclusive = 289;
inline constexpr std::int32_t kDeleteWallMaxYExclusive = 401;

// Exact state-0 post-input cleanup. Delete mode survives only while the pointer
// is strictly inside Y 289..401, unless a deletion raised the one-frame latch.
// The latch is cleared unconditionally at the end of the check.
[[nodiscard]] inline bool maintain_delete_mode_after_input(
    EditorRuntimeState& state,
    std::int32_t mouse_y) noexcept {

    if (!state.delete_mode) {
        state.delete_interaction_latch = false;
        return false;
    }

    bool cancelled = false;
    if (!state.delete_interaction_latch &&
        (mouse_y <= kDeleteWallMinYExclusive ||
         mouse_y >= kDeleteWallMaxYExclusive)) {
        state.delete_mode = false;
        state.cursor = {};
        cancelled = true;
    }

    state.delete_interaction_latch = false;
    return cancelled;
}

[[nodiscard]] inline bool apply_clear_all_confirmation(
    Composition& composition,
    bool confirmed_yes) noexcept {
    if (!confirmed_yes) {
        return false;
    }
    composition.clear();
    return true;
}

struct MachineVisualSpec {
    std::int32_t short_x{};
    std::int32_t short_y{};
    std::int32_t long_x{};
    std::int32_t long_y{};
    std::int32_t short_frame_width{};
    std::int32_t short_frame_height{};
    std::int32_t long_frame_width{};
    std::int32_t long_frame_height{};
};

inline constexpr std::array<MachineVisualSpec,5> kMachineVisualSpecs{{
    {0,130,3,130,189,168,246,177},     // Roley
    {86,93,86,93,200,162,222,177},     // Muck
    {225,2,225,2,165,238,177,242},     // Lofty
    {342,105,342,105,84,102,85,103},   // Dizzy
    {393,75,393,75,132,143,138,167},   // Scoop
}};

inline constexpr std::int32_t kShortMachineAnimationFrames = 15;
inline constexpr std::int32_t kLongMachineAnimationFrames = 30;
inline constexpr std::int32_t kMachineAnimationTickThreshold = 3;

struct MachineVisualState {
    std::int32_t animation_state{}; // 0 static, 1 short, 2 long
    std::int32_t frame{};
    std::int32_t tick{};
};

struct MachineDrawStep {
    std::int32_t draw_animation_state{};
    std::int32_t frame{};
    std::int32_t source_left{};
    std::int32_t source_top{};
    std::int32_t source_right{};
    std::int32_t source_bottom{};
    std::int32_t x{};
    std::int32_t y{};
    bool returns_to_static_after_draw{};
};

// Exact machine renderer in 0x0041F090. State 0 draws frame zero from the
// short sheet. State 1 uses the short sheet, state 2 the long sheet. Active
// animations advance after tick >3. When the frame reaches 15/30, retail
// rewinds to frame 0, draws that animated-sheet frame once, then clears the
// machine state to static for the next update.
[[nodiscard]] constexpr MachineDrawStep tick_machine_visual(
    Machine machine,
    MachineVisualState& state,
    std::int32_t animation_tick_delta = 1) noexcept {

    const auto index = static_cast<std::size_t>(machine);
    const auto& spec = kMachineVisualSpecs[index];

    const auto draw_state = state.animation_state;
    bool complete_after_draw = false;

    if (draw_state != 0) {
        state.tick += animation_tick_delta;
        if (state.tick > kMachineAnimationTickThreshold) {
            state.tick = 0;
            ++state.frame;
            const auto frame_count =
                draw_state == 1
                    ? kShortMachineAnimationFrames
                    : kLongMachineAnimationFrames;
            if (state.frame >= frame_count) {
                state.frame = 0;
                complete_after_draw = true;
            }
        }
    }

    const bool long_sheet = draw_state == 2;
    const auto width =
        long_sheet ? spec.long_frame_width : spec.short_frame_width;
    const auto height =
        long_sheet ? spec.long_frame_height : spec.short_frame_height;
    const auto x = long_sheet ? spec.long_x : spec.short_x;
    const auto y = long_sheet ? spec.long_y : spec.short_y;
    const auto left = state.frame * width;

    MachineDrawStep step{
        draw_state,
        state.frame,
        left,
        0,
        left + width,
        height,
        x,
        y,
        complete_after_draw,
    };

    if (complete_after_draw) {
        state.animation_state = 0;
    }

    return step;
}

struct ConductorVisualSpec {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t frame_width{};
    std::int32_t frame_height{};
};

inline constexpr std::array<ConductorVisualSpec,kConductorCount>
kConductorVisualSpecs{{
    {499,133,85,78},
    {504,132,68,80},
    {493,136,78,85},
}};

inline constexpr std::int32_t kConductorIdleNormalEndExclusive = 20;
inline constexpr std::int32_t kConductorIdleSpecialEndExclusive = 50;
inline constexpr std::int32_t kConductorIdleTickThreshold = 5;

struct ConductorIdleStep {
    bool frame_advanced{};
    bool entered_special_idle{};
    bool normal_idle_restarted{};
    bool special_idle_completed{};
    std::int32_t frame{};
    std::int32_t tick{};
};

// Exact editor conductor clock inside DrawGrandOpeningCompositionAndMachines.
// Frames 0..19 are the normal idle. On first reaching frame 20, rand()%3 == 0
// enters the longer special idle 20..49; the other 2/3 of outcomes restart at
// frame 0. Frame 50 always wraps to frame 0.
[[nodiscard]] constexpr ConductorIdleStep tick_idle_conductor_animation(
    Conductor conductor,
    ConductorAnimationState& state,
    std::int32_t random_mod_3,
    std::int32_t animation_tick_delta = 1) noexcept {

    (void)conductor; // all three use the same 0..19 / 20..49 idle ranges
    ConductorIdleStep result;

    state.tick += animation_tick_delta;
    if (state.tick > kConductorIdleTickThreshold) {
        state.tick = 0;
        ++state.frame;
        result.frame_advanced = true;

        if (state.frame == kConductorIdleNormalEndExclusive) {
            if (random_mod_3 == 0) {
                result.entered_special_idle = true;
            } else {
                state.frame = 0;
                result.normal_idle_restarted = true;
            }
        }

        if (state.frame >= kConductorIdleSpecialEndExclusive) {
            state.frame = 0;
            result.special_idle_completed = true;
        }
    }

    result.frame = state.frame;
    result.tick = state.tick;
    return result;
}

struct VisualSourceRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

[[nodiscard]] constexpr VisualSourceRect conductor_source_rect(
    Conductor conductor,
    std::int32_t frame) noexcept {
    const auto& spec =
        kConductorVisualSpecs[static_cast<std::size_t>(conductor)];
    const auto left = frame * spec.frame_width;
    return {left,0,left + spec.frame_width,spec.frame_height};
}

struct ToolbarRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

struct EditorHitRegion {
    ToolbarRect rect{};
    std::int32_t action{-1};
};

inline constexpr std::size_t kWallHitRegionCount =
    kPitchRowCount * kTimelineStepCount;
inline constexpr std::size_t kPaletteHitRegionCount = 10;
inline constexpr std::size_t kSpecialHitRegionCount = 2;
inline constexpr std::size_t kEditorHitRegionCount =
    kWallHitRegionCount + kPaletteHitRegionCount + kSpecialHitRegionCount;

inline constexpr std::int32_t kWallAction = 11;
inline constexpr std::int32_t kSecondaryStopAction = 22;
inline constexpr std::int32_t kLegacyPlayRegionAction = 23;

inline constexpr std::int32_t kWallOriginX = 44;
inline constexpr std::int32_t kWallOriginY = 310;
inline constexpr std::int32_t kWallStepWidth = 23;
inline constexpr std::int32_t kWallRowHeight = 19;

inline constexpr std::array<ToolbarRect, kPaletteHitRegionCount>
kPaletteHitRects{{
    {232,284,274,306},
    {191,262,231,288},
    {286,249,325,273},
    {248,233,285,260},
    {353,221,387,245},
    {316,205,351,232},
    {411,210,446,231},
    {372,195,408,219},
    {481,194,512,222},
    {447,193,479,220},
}};

inline constexpr ToolbarRect kSecondaryStopHitRect{251,415,301,463};
inline constexpr ToolbarRect kLegacyPlayHitRect{340,415,395,463};

[[nodiscard]] constexpr std::array<EditorHitRegion,kEditorHitRegionCount>
make_editor_hit_regions() noexcept {
    std::array<EditorHitRegion,kEditorHitRegionCount> out{};
    std::size_t index = 0;

    for (std::size_t row = 0; row < kPitchRowCount; ++row) {
        for (std::size_t second = 0; second < kTimelineStepCount; ++second) {
            const auto left =
                kWallOriginX +
                static_cast<std::int32_t>(second) * kWallStepWidth;
            const auto top =
                kWallOriginY +
                static_cast<std::int32_t>(row) * kWallRowHeight;
            out[index++] = {
                {left, top, left + kWallStepWidth, top + kWallRowHeight},
                kWallAction,
            };
        }
    }

    for (std::size_t i = 0; i < kPaletteHitRects.size(); ++i) {
        out[index++] = {
            kPaletteHitRects[i],
            static_cast<std::int32_t>(i),
        };
    }

    out[index++] = {kSecondaryStopHitRect, kSecondaryStopAction};
    out[index++] = {kLegacyPlayHitRect, kLegacyPlayRegionAction};
    return out;
}

inline constexpr auto kEditorHitRegions = make_editor_hit_regions();

struct EditorHitTestResult {
    std::int32_t action{-1};
    std::int32_t region_index{-1};
    std::int32_t adjusted_x{};
    std::int32_t adjusted_y{};
};

// Exact 0x0041FB90 behavior. State 1 tests with a +10/+10 pointer hotspot;
// all other states use +5/+5. Every rectangle uses strict interior bounds.
[[nodiscard]] constexpr EditorHitTestResult hit_test_editor_regions(
    ActivityState activity_state,
    std::int32_t mouse_x,
    std::int32_t mouse_y) noexcept {

    const auto offset =
        activity_state == ActivityState::MachineSelected ? 10 : 5;
    const auto x = mouse_x + offset;
    const auto y = mouse_y + offset;

    for (std::size_t i = 0; i < kEditorHitRegions.size(); ++i) {
        const auto& region = kEditorHitRegions[i];
        if (region.action != -1 &&
            x > region.rect.left && x < region.rect.right &&
            y > region.rect.top && y < region.rect.bottom) {
            return {
                region.action,
                static_cast<std::int32_t>(i),
                x,
                y,
            };
        }
    }
    return {-1,-1,x,y};
}

struct WallDrawPosition {
    std::int32_t x{};
    std::int32_t y{};
};

[[nodiscard]] constexpr WallDrawPosition composition_cell_draw_position(
    std::size_t pitch_row,
    std::size_t second) noexcept {
    return {
        kWallOriginX +
            static_cast<std::int32_t>(second) * kWallStepWidth,
        kWallOriginY +
            static_cast<std::int32_t>(pitch_row) * kWallRowHeight,
    };
}

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

enum class ToolbarSurface {
    PlayHover,       // mpplayred.bmp
    PlayPressed,     // mpplaydep.bmp
    StopHover,       // mpstopred.bmp
    StopPressed,     // mpstopdep.bmp
    ClearAllHover,   // mpclearallred.bmp
    ClearAllPressed, // mpclearalldep.bmp
    DeleteHover,     // mpdeletered.bmp
    DeletePressed,   // mpdeletedep.bmp
};

[[nodiscard]] constexpr std::optional<ToolbarSurface> toolbar_surface(
    ToolbarControl control,
    ToolbarVisual visual) noexcept {

    if (visual == ToolbarVisual::None) {
        return std::nullopt;
    }

    const bool pressed = visual == ToolbarVisual::Pressed;
    switch (control) {
    case ToolbarControl::Play:
        return pressed
            ? ToolbarSurface::PlayPressed
            : ToolbarSurface::PlayHover;
    case ToolbarControl::Stop:
        return pressed
            ? ToolbarSurface::StopPressed
            : ToolbarSurface::StopHover;
    case ToolbarControl::ClearAll:
        return pressed
            ? ToolbarSurface::ClearAllPressed
            : ToolbarSurface::ClearAllHover;
    case ToolbarControl::Delete:
        return pressed
            ? ToolbarSurface::DeletePressed
            : ToolbarSurface::DeleteHover;
    }
    return std::nullopt;
}

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
    std::optional<ToolbarSurface> surface{};
    std::int32_t surface_x{};
    std::int32_t surface_y{};
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

struct SecondaryStopRegionStep {
    bool draw_stop_hover_surface{};
    std::int32_t surface_x{260};
    std::int32_t surface_y{416};
    bool stop_backing_track{};
    ActivityState next_state{ActivityState::Playing};
};

// State 9 performs a second hit-test path after the toolbar controller. Action
// 22 always draws the Stop-red overlay at (260,416); a click stops/resets the
// backing track and returns the internal state to Edit. Action 23 is ignored.
[[nodiscard]] constexpr SecondaryStopRegionStep update_secondary_stop_region(
    std::int32_t region_action,
    bool click_active) noexcept {

    SecondaryStopRegionStep step;
    if (region_action != kSecondaryStopAction) {
        return step;
    }

    step.draw_stop_hover_surface = true;
    if (click_active) {
        step.stop_backing_track = true;
        step.next_state = ActivityState::Edit;
    }
    return step;
}

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
