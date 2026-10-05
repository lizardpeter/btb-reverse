#include "btb/fireworks_editor.hpp"

namespace btb::fireworks {
namespace {

[[nodiscard]] constexpr bool strict_hit(
    const Recti& rect,
    std::int32_t x,
    std::int32_t y) noexcept {
    return x > rect.left && x < rect.right &&
           y > rect.top && y < rect.bottom;
}

[[nodiscard]] constexpr Recti control_rect(EditorAction action) noexcept {
    switch (action) {
    case EditorAction::Play:
        return {292, 416, 346, 472};
    case EditorAction::DeleteAll:
        return {102, 416, 156, 472};
    case EditorAction::DeleteSelected:
        return {483, 416, 537, 472};
    default:
        return {};
    }
}

[[nodiscard]] constexpr std::int32_t action_id(EditorAction action) noexcept {
    return static_cast<std::int32_t>(action);
}

} // namespace

PlacementBeginResult begin_placement_region_action(
    Sequence& sequence,
    std::size_t row,
    std::size_t column) noexcept {

    PlacementBeginResult result;
    const auto existing = sequence.at(row, column);
    if (!existing) {
        return result;
    }

    result.had_existing_start = true;
    result.removed_type = sequence.remove(row, column);
    return result;
}

PlacementReleaseResult complete_placement_release(
    EditorRuntimeState& state,
    const Sequence& sequence,
    std::size_t row,
    std::size_t column,
    std::int32_t random_mod_4) noexcept {

    PlacementReleaseResult result;
    state.cursor = {};

    if (!sequence.in_bounds(row, column)) {
        return result;
    }

    const auto existing = sequence.at(row, column);
    if (existing) {
        // 0x00412D9C..0x00412DA3 rejects a cell containing a start type 0..11.
        return result;
    }

    result.attempted = true;
    result.channel = placement_actor_for_row(row);

    const auto index = placement_actor_index(result.channel);
    state.pending[index] = {row, column};
    state.actor_states[index] = PlacementActorState::PlacementQueued;
    result.sound_id = placement_voice_sound_id(result.channel, random_mod_4);

    result.valid = sequence.validate_placement(
        row, column, state.selected_type);
    if (result.valid) {
        state.internal_state = InternalState::Editor;
        state.cursor = {};
    }

    return result;
}

PlacementActorTickResult tick_placement_actor(
    EditorRuntimeState& state,
    Sequence& sequence,
    PlacementActorChannel channel) {

    PlacementActorTickResult result;
    const auto index = placement_actor_index(channel);
    auto& actor_state = state.actor_states[index];

    if (actor_state == PlacementActorState::PlacementQueued) {
        actor_state = PlacementActorState::PlacementCommit;
        result.queued_to_commit = true;
        return result;
    }

    if (actor_state != PlacementActorState::PlacementCommit) {
        return result;
    }

    result.commit_attempted = true;
    const auto target = state.pending[index];
    result.committed = sequence.commit_placement(
        target.row,
        target.column,
        state.selected_type);

    // Retail clears the channel unconditionally after the commit call.
    actor_state = PlacementActorState::Idle;

    result.grid_full = sequence.full();
    if (result.grid_full) {
        result.sound_id = full_grid_voice_sound_id(channel);
    }

    return result;
}

EditorControlOutput update_editor_controls(
    EditorRuntimeState& state,
    const EditorControlInput& input) noexcept {

    EditorControlOutput output;

    if (state.control_cooldown > 0) {
        --state.control_cooldown;
        return output;
    }

    const auto [x, y] =
        retail_control_hit_point(state, input.mouse_x, input.mouse_y);

    // The table order is Play, Delete All, Delete Selected. Their rectangles
    // do not overlap, so this preserves retail selection without ambiguity.
    constexpr std::array controls{
        EditorAction::Play,
        EditorAction::DeleteAll,
        EditorAction::DeleteSelected,
    };

    std::optional<EditorAction> hit;
    for (const auto control : controls) {
        if (strict_hit(control_rect(control), x, y)) {
            hit = control;
            break;
        }
    }

    if (!hit) {
        // Retail does not reset 0x00442A30 when the pointer leaves all controls.
        return output;
    }

    output.control = *hit;
    const auto previous = state.previous_control_action;

    if (*hit == EditorAction::Play) {
        // Play is completely ignored while either actor channel is nonzero.
        if (state.actor_states[0] != PlacementActorState::Idle ||
            state.actor_states[1] != PlacementActorState::Idle) {
            return output;
        }

        if (input.click_active &&
            state.internal_state == InternalState::Editor) {
            state.internal_state = InternalState::PreShowMovieSetup;
            state.previous_control_action = action_id(*hit);
            output.action = EditorControlActionKind::EnterPreShow;
            output.sound_id = kPlayClickSoundId;
            return output;
        }

        if (input.pressed_visual) {
            state.previous_control_action = action_id(*hit);
            output.visual = EditorControlVisual::Pressed;
            return output;
        }

        state.previous_control_action = action_id(*hit);
        output.visual = EditorControlVisual::Hover;
        if (previous != action_id(*hit)) {
            output.sound_id = kPlayHoverSoundId;
        }
        return output;
    }

    if (*hit == EditorAction::DeleteSelected) {
        // 0x00413CDA checks specifically for actor state == 1 when the shared
        // interaction gate is active, not for all positive actor states.
        if (input.delete_interaction_gate &&
            (state.actor_states[0] == PlacementActorState::PaletteLatched ||
             state.actor_states[1] == PlacementActorState::PaletteLatched)) {
            state.actor_states[0] = PlacementActorState::Idle;
            state.actor_states[1] = PlacementActorState::Idle;
            state.internal_state = InternalState::Editor;
            state.cursor = {};
            state.control_cooldown = kConflictCooldownFrames;
            output.action =
                EditorControlActionKind::CancelConflictingInteraction;
            return output;
        }

        if (input.click_active) {
            if (state.internal_state == InternalState::DeleteSelected) {
                state.internal_state = InternalState::Editor;
                state.cursor = {};
                state.previous_control_action = action_id(*hit);
                output.action = EditorControlActionKind::LeaveDeleteMode;
                return output;
            }

            state.internal_state = InternalState::DeleteSelected;
            state.cursor = {EditorCursorKind::Delete, std::nullopt};
            state.previous_control_action = action_id(*hit);
            output.action = EditorControlActionKind::EnterDeleteMode;
            output.sound_id = kDeleteClickSoundId;
            return output;
        }

        if (input.pressed_visual) {
            state.previous_control_action = action_id(*hit);
            output.visual = EditorControlVisual::Pressed;
            return output;
        }

        state.previous_control_action = action_id(*hit);
        output.visual = EditorControlVisual::Hover;
        if (previous != action_id(*hit)) {
            output.sound_id = kDeleteHoverSoundId;
        }
        return output;
    }

    // Delete All.
    if (input.click_active &&
        state.internal_state == InternalState::Editor) {
        state.previous_control_action = action_id(*hit);
        output.action = EditorControlActionKind::OpenDeleteAllConfirmation;
        output.sound_id = kDeleteAllClickSoundId;
        output.open_yes_no_confirmation = true;
        output.confirmation_context = kDeleteAllConfirmationContext;
        return output;
    }

    if (input.pressed_visual) {
        state.previous_control_action = action_id(*hit);
        output.visual = EditorControlVisual::Pressed;
        return output;
    }

    state.previous_control_action = action_id(*hit);
    output.visual = EditorControlVisual::Hover;
    if (previous != action_id(*hit)) {
        output.sound_id = kDeleteAllHoverSoundId;
    }
    return output;
}

} // namespace btb::fireworks
