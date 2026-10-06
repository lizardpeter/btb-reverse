#pragma once

#include "btb/fireworks_layout.hpp"
#include "btb/fireworks_sequence.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>
#include <vector>

namespace btb::fireworks {

enum class PlacementActorChannel : std::size_t {
    Bob = 0,
    Wendy = 1,
};

enum class PlacementActorState : std::int32_t {
    Idle = 0,
    PaletteLatched = 1,
    PlacementQueued = 10,
    PlacementCommit = 11,
    DormantLegacyMotion = 12,
};

[[nodiscard]] constexpr bool placement_actor_state_has_direct_retail_writer(
    PlacementActorState state) noexcept {
    return state != PlacementActorState::DormantLegacyMotion;
}

[[nodiscard]] constexpr PlacementActorChannel placement_actor_for_row(
    std::size_t row) noexcept {
    return row == 2
        ? PlacementActorChannel::Wendy
        : PlacementActorChannel::Bob;
}

[[nodiscard]] constexpr std::size_t placement_actor_index(
    PlacementActorChannel channel) noexcept {
    return static_cast<std::size_t>(channel);
}

inline constexpr std::int32_t kBobPaletteVoiceFirst = 862;   // ZFE_BOB_08
inline constexpr std::int32_t kWendyPaletteVoiceFirst = 899; // ZFE_WEN_08
inline constexpr std::int32_t kPaletteVoiceCount = 5;

inline constexpr std::int32_t kBobPlacementVoiceFirst = 867;   // ZFE_BOB_13
inline constexpr std::int32_t kWendyPlacementVoiceFirst = 904; // ZFE_WEN_13
inline constexpr std::int32_t kPlacementVoiceCount = 4;

inline constexpr std::int32_t kBobFullGridVoice = 860;   // ZFE_BOB_06
inline constexpr std::int32_t kWendyFullGridVoice = 897; // ZFE_WEN_06

[[nodiscard]] constexpr std::int32_t palette_voice_sound_id(
    FireworkType type,
    std::int32_t random_mod_5) noexcept {
    if (random_mod_5 < 0 || random_mod_5 >= kPaletteVoiceCount) {
        return -1;
    }
    return static_cast<std::int32_t>(type) < 8
        ? kBobPaletteVoiceFirst + random_mod_5
        : kWendyPaletteVoiceFirst + random_mod_5;
}

[[nodiscard]] constexpr std::int32_t placement_voice_sound_id(
    PlacementActorChannel channel,
    std::int32_t random_mod_4) noexcept {
    if (random_mod_4 < 0 || random_mod_4 >= kPlacementVoiceCount) {
        return -1;
    }
    return channel == PlacementActorChannel::Bob
        ? kBobPlacementVoiceFirst + random_mod_4
        : kWendyPlacementVoiceFirst + random_mod_4;
}

[[nodiscard]] constexpr std::int32_t full_grid_voice_sound_id(
    PlacementActorChannel channel) noexcept {
    return channel == PlacementActorChannel::Bob
        ? kBobFullGridVoice
        : kWendyFullGridVoice;
}

enum class EditorCursorKind {
    Normal,
    FireworkType,
    Delete,
};

struct EditorCursor {
    EditorCursorKind kind{EditorCursorKind::Normal};
    std::optional<FireworkType> firework_type{};
};

struct PendingPlacement {
    std::size_t row{};
    std::size_t column{};
};

struct PlacementActorVisual {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t source_column{};
    std::int32_t source_row{};
    std::int32_t frame_tick{};
};

inline constexpr std::array<PlacementActorVisual, 2>
kInitialPlacementActorVisuals{{
    // Bob globals: x=0x104, y=0xA8, source column 4, source row/tick 0.
    {260, 168, 4, 0, 0},
    // Wendy globals: x=0x104, y=0, source column 4, source row/tick 0.
    {260, 0, 4, 0, 0},
}};

// Static .data targets consumed only by the dormant state-12 movement branch.
// No executable writer for these target globals exists.
inline constexpr std::array<std::pair<std::int32_t, std::int32_t>, 2>
kDormantPlacementActorTargets{{
    {260, 210},
    {260, 20},
}};

struct ActorFollowPoint {
    std::int32_t x{};
    std::int32_t y{};
};

// Exact stack-constructed paths inside DrawFireworksEditor. Coordinates are
// actor-center path coordinates; the sprite top-left Y is path_y - 128.
inline constexpr std::array<ActorFollowPoint, 4> kBobFollowPath{{
    {0, 177},
    {240, 296},
    {400, 296},
    {640, 177},
}};

inline constexpr std::array<ActorFollowPoint, 4> kWendyFollowPath{{
    {0, 128},
    {300, 128},
    {500, 128},
    {640, 128},
}};

inline constexpr std::int32_t kActorSpriteSize = 128;
inline constexpr std::int32_t kActorSpriteHalfSize = 64;
inline constexpr std::int32_t kActorMouseMinX = 61;
inline constexpr std::int32_t kActorMouseMaxX = 575;
inline constexpr std::int32_t kActorMouseSplitY = 160;
inline constexpr std::int32_t kActorMouseSplitYWhenPaletteLatched = 35;
inline constexpr std::int32_t kActorFollowBaseThreshold = 30;
inline constexpr std::int32_t kActorFollowTightThresholdBias = 25;

inline constexpr std::int32_t kDormantMotionSpeed = 5;
inline constexpr float kDormantMotionArrivalDistance = 10.0F;
inline constexpr std::int32_t kDormantMotionFirstRow = 13;
inline constexpr std::int32_t kDormantMotionRowEndExclusive = 25;

struct EditorRuntimeState;

struct DormantMotionStep {
    bool active{};
    bool arrived{};
    std::int32_t angle_degrees{};
    std::int32_t delta_x{};
    std::int32_t delta_y{};
    float distance_after_move{};
};

// Exact no-writer state-12 branch in DrawFireworksEditor. It uses the shared
// retail angle helper, rounds that integer angle to one of eight 45-degree
// sprite columns, moves by sin/cos at speed 5, cycles source rows 13..24 after
// the existing row reaches the legacy range, and returns the channel to idle
// once post-move Euclidean distance is strictly less than 10 pixels.
[[nodiscard]] DormantMotionStep tick_dormant_legacy_motion(
    EditorRuntimeState& state,
    PlacementActorChannel channel) noexcept;

struct ActorSourceRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

[[nodiscard]] constexpr ActorSourceRect placement_actor_source_rect(
    const PlacementActorVisual& visual) noexcept {
    const auto left = visual.source_column * 128;
    const auto top = visual.source_row * 128;
    return {left, top, left + 128, top + 128};
}

// The ordinary idle animation path forces source column 4. Its frame timer
// increments once per DrawFireworksEditor call; after values 1..5 it advances
// the row on tick 6, clears the timer, and wraps row 9 back to 0.
constexpr void tick_idle_actor_animation(
    PlacementActorVisual& visual) noexcept {
    visual.source_column = 4;
    ++visual.frame_tick;
    if (visual.frame_tick > 5) {
        visual.frame_tick = 0;
        ++visual.source_row;
        if (visual.source_row >= 9) {
            visual.source_row = 0;
        }
    }
}

struct EditorRuntimeState {
    InternalState internal_state{InternalState::Editor};
    FireworkType selected_type{FireworkType::RedAirbomb};

    std::array<PlacementActorState, 2> actor_states{
        PlacementActorState::Idle,
        PlacementActorState::Idle,
    };
    std::array<PendingPlacement, 2> pending{};
    std::array<PlacementActorVisual, 2> actor_visuals{
        kInitialPlacementActorVisuals[0],
        kInitialPlacementActorVisuals[1],
    };

    EditorCursor cursor{};

    // Retail global 0x0050AB7C. It is either 0 or 25 and narrows the stop
    // threshold from 30 pixels to 5 after an actor begins tracking.
    std::int32_t actor_follow_threshold_bias{};

    std::int32_t control_cooldown{};
    std::int32_t previous_control_action{-1};
};

struct ActorMouseTrackingStep {
    PlacementActorChannel idle_actor{PlacementActorChannel::Bob};
    PlacementActorChannel tracking_actor{PlacementActorChannel::Wendy};
    std::int32_t split_y{};
    std::int32_t clamped_mouse_x{};
    std::int32_t interpolated_path_y{};
    std::int32_t vertical_direction_code{};
    bool standing{};
    bool moved_left{};
    bool moved_right{};
};

// Exact pre-draw Bob/Wendy animation controller in DrawFireworksEditor.
// One actor idles while the other follows mouse X by one pixel per update.
// Palette-latched actor state changes the Y split from 160 to 35. Once motion
// starts, retail stores bias 25 so the stop threshold narrows from 30 to 5.
[[nodiscard]] ActorMouseTrackingStep update_actor_mouse_tracking(
    EditorRuntimeState& state,
    std::int32_t mouse_x,
    std::int32_t mouse_y) noexcept;

struct DeleteAllConfirmationResult {
    bool applied{};
};

// At the top of UpdateFireworksActivity, a nonzero shared Yes/No result clears
// all 18 authored slots, clears both actor channels, resets Fireworks state 0,
// restores the normal cursor, and consumes the modal result before any state
// dispatch. A No result is represented by result=false and does nothing here.
[[nodiscard]] inline DeleteAllConfirmationResult apply_delete_all_confirmation(
    EditorRuntimeState& state,
    Sequence& sequence,
    bool confirmed_yes) noexcept {

    DeleteAllConfirmationResult result;
    if (!confirmed_yes) {
        return result;
    }

    sequence.clear();
    state.actor_states[0] = PlacementActorState::Idle;
    state.actor_states[1] = PlacementActorState::Idle;
    state.internal_state = InternalState::Editor;
    state.cursor = {};
    result.applied = true;
    return result;
}

struct PaletteBeginResult {
    bool selection_latched{};
    std::optional<std::int32_t> sound_id{};
};

// Exact 0x00412BD0 palette branch. The cursor changes even when the channel is
// already non-idle; the selected type and one-shot palette voice update only
// when that channel state is zero.
[[nodiscard]] constexpr PaletteBeginResult begin_palette_selection(
    EditorRuntimeState& state,
    FireworkType type,
    std::int32_t random_mod_5) noexcept {

    state.cursor = {EditorCursorKind::FireworkType, type};

    const auto channel =
        static_cast<std::int32_t>(type) < 8
            ? PlacementActorChannel::Bob
            : PlacementActorChannel::Wendy;
    auto& actor_state = state.actor_states[placement_actor_index(channel)];
    if (actor_state != PlacementActorState::Idle) {
        return {};
    }

    state.selected_type = type;
    actor_state = PlacementActorState::PaletteLatched;
    return {
        true,
        palette_voice_sound_id(type, random_mod_5),
    };
}

// Exact 0x00412D50 palette-release branch. Unlike the begin half above, the
// release path stores the palette action ID to selected_type unconditionally.
// In normal state-0 dispatch retail calls BeginFireworksEditorAction and then
// CompleteFireworksEditorAction for the same hit action, so a second palette
// click can change the selected type even when the actor's state-1 latch
// prevents another palette voice.
constexpr void complete_palette_selection(
    EditorRuntimeState& state,
    FireworkType type) noexcept {
    state.selected_type = type;
}

struct PaletteActionResult {
    PaletteBeginResult begin{};
};

// Exact normal-editor ordering for a palette action: begin first, then complete.
[[nodiscard]] constexpr PaletteActionResult process_palette_action(
    EditorRuntimeState& state,
    FireworkType type,
    std::int32_t random_mod_5) noexcept {

    PaletteActionResult result;
    result.begin = begin_palette_selection(state, type, random_mod_5);
    complete_palette_selection(state, type);
    return result;
}

struct PlacementBeginResult {
    bool had_existing_start{};
    std::optional<FireworkType> removed_type{};
};

// Exact action-12 begin half at 0x00412C82: if the clicked authored cell holds
// a start type 0..11, retail removes that event before the release half runs.
// With the shipped span table every event is one cell, so Sequence::remove is
// the complete retail effect here.
[[nodiscard]] PlacementBeginResult begin_placement_region_action(
    Sequence& sequence,
    std::size_t row,
    std::size_t column) noexcept;

// Exact 0x00412D50 placement-release decision before PlaceFireworkGridItem's
// commit=false validation call.
struct PlacementReleaseResult {
    bool attempted{};
    bool valid{};
    PlacementActorChannel channel{PlacementActorChannel::Bob};
    std::optional<std::int32_t> sound_id{};
};

[[nodiscard]] PlacementReleaseResult complete_placement_release(
    EditorRuntimeState& state,
    const Sequence& sequence,
    std::size_t row,
    std::size_t column,
    std::int32_t random_mod_4) noexcept;

struct PlacementActorTickResult {
    bool queued_to_commit{};
    bool commit_attempted{};
    bool committed{};
    bool grid_full{};
    std::optional<std::int32_t> sound_id{};
};

// Models states 10 -> 11 -> commit in DrawFireworksEditor. State 10 only
// changes to 11. State 11 commits using the *current* selected type, resets the
// channel to idle, then counts all 18 cells and plays the channel-specific
// full-grid line when every cell is occupied.
[[nodiscard]] PlacementActorTickResult tick_placement_actor(
    EditorRuntimeState& state,
    Sequence& sequence,
    PlacementActorChannel channel);

inline constexpr std::int32_t kPreviewX = 199;
inline constexpr std::int32_t kPreviewY = 39;

struct PreviewRuntime {
    FireworkType type{FireworkType::RedAirbomb};
    std::int32_t playback_state0{};
    std::int32_t playback_state1{};
};

struct PreviewStep {
    InternalState state{};
    bool draw_editor{true};
    bool decode_preview_movie{};
    bool restart_preview_movie{};
    std::int32_t movie_index{-1};
    std::int32_t x{kPreviewX};
    std::int32_t y{kPreviewY};
};

// Exact Fireworks states 2/3/4 from UpdateFireworksActivity.
//
// State 2 draws the editor, copies selected_type into the preview record,
// clears its two playback-state dwords, then increments to state 3.
// State 3 draws the editor and decodes bank[selected_type] at (199,39). When
// DecodeAndBlitBinkFrame reports completion, retail increments to state 4 and
// rewinds that same Bink.
// State 4 draws the editor once more and returns directly to state 0.
[[nodiscard]] constexpr PreviewStep update_preview_state(
    EditorRuntimeState& editor,
    PreviewRuntime& preview,
    bool preview_movie_finished) noexcept {

    PreviewStep step;
    step.state = editor.internal_state;

    switch (editor.internal_state) {
    case InternalState::PreviewSetup:
        preview.type = editor.selected_type;
        preview.playback_state0 = 0;
        preview.playback_state1 = 0;
        editor.internal_state = InternalState::PreviewPlayback;
        step.state = editor.internal_state;
        return step;

    case InternalState::PreviewPlayback:
        step.decode_preview_movie = true;
        step.movie_index = static_cast<std::int32_t>(preview.type);
        if (preview_movie_finished) {
            step.restart_preview_movie = true;
            editor.internal_state = InternalState::PreviewFinish;
        }
        step.state = editor.internal_state;
        return step;

    case InternalState::PreviewFinish:
        editor.internal_state = InternalState::Editor;
        step.state = editor.internal_state;
        return step;

    default:
        return step;
    }
}

enum class EditorControlVisual {
    None,
    Hover,
    Pressed,
};

enum class EditorControlActionKind {
    None,
    EnterPreShow,
    EnterDeleteMode,
    LeaveDeleteMode,
    OpenDeleteAllConfirmation,
    CancelConflictingInteraction,
};

struct EditorControlInput {
    std::int32_t mouse_x{};
    std::int32_t mouse_y{};
    bool click_active{};
    bool pressed_visual{};
    bool delete_interaction_gate{};
};

struct EditorControlOutput {
    EditorControlActionKind action{EditorControlActionKind::None};
    std::optional<EditorAction> control{};
    EditorControlVisual visual{EditorControlVisual::None};
    std::optional<std::int32_t> sound_id{};

    bool open_yes_no_confirmation{};
    std::int32_t confirmation_context{-1};
};

inline constexpr std::int32_t kPlayClickSoundId = 880;       // ZFE_BOB_27
inline constexpr std::int32_t kPlayHoverSoundId = 881;       // ZFE_BOB_28
inline constexpr std::int32_t kDeleteHoverSoundId = 882;     // ZFE_BOB_29
inline constexpr std::int32_t kDeleteAllHoverSoundId = 883;  // ZFE_BOB_30
inline constexpr std::int32_t kDeleteAllClickSoundId = 885;  // ZFE_BOB_33
inline constexpr std::int32_t kDeleteClickSoundId = 890;     // ZFE_D_WEN_01
inline constexpr std::int32_t kDeleteAllConfirmationContext = 3;
inline constexpr std::int32_t kConflictCooldownFrames = 10;

// UpdateFireworksEditorControls shifts the point used to hit-test *all*
// controls by (+40,+20) whenever either placement actor state is > 0.
[[nodiscard]] constexpr std::pair<std::int32_t, std::int32_t>
retail_control_hit_point(
    const EditorRuntimeState& state,
    std::int32_t mouse_x,
    std::int32_t mouse_y) noexcept {

    const bool actor_active =
        static_cast<std::int32_t>(state.actor_states[0]) > 0 ||
        static_cast<std::int32_t>(state.actor_states[1]) > 0;
    return actor_active
        ? std::pair{mouse_x + 40, mouse_y + 20}
        : std::pair{mouse_x, mouse_y};
}

[[nodiscard]] EditorControlOutput update_editor_controls(
    EditorRuntimeState& state,
    const EditorControlInput& input) noexcept;

} // namespace btb::fireworks
