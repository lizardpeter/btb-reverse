#pragma once

#include "btb/fireworks_layout.hpp"
#include "btb/fireworks_sequence.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

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

struct EditorRuntimeState {
    InternalState internal_state{InternalState::Editor};
    FireworkType selected_type{FireworkType::RedAirbomb};

    std::array<PlacementActorState, 2> actor_states{
        PlacementActorState::Idle,
        PlacementActorState::Idle,
    };
    std::array<PendingPlacement, 2> pending{};

    EditorCursor cursor{};

    std::int32_t control_cooldown{};
    std::int32_t previous_control_action{-1};
};

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
