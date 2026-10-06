#include "btb/grand_opening_runtime.hpp"

namespace btb::grand_opening {
namespace {

[[nodiscard]] constexpr bool strict_hit(
    const ToolbarRect& rect,
    std::int32_t x,
    std::int32_t y) noexcept {
    return x > rect.left && x < rect.right &&
           y > rect.top && y < rect.bottom;
}

[[nodiscard]] constexpr std::size_t machine_index(
    MachineType type) noexcept {
    return static_cast<std::size_t>(machine_for_type(type));
}

[[nodiscard]] constexpr std::int32_t random_bit(
    std::int32_t value) noexcept {
    return value & 1;
}

} // namespace

PlaybackSecondStep update_playback_second(
    const Composition& composition,
    std::int32_t elapsed_centiseconds,
    std::int32_t previous_elapsed_centiseconds,
    std::array<std::int32_t,5>& machine_animation_states) {

    PlaybackSecondStep result;
    result.second = playback_step_from_centiseconds(elapsed_centiseconds);
    if (result.second < 0) {
        result.timeline_complete =
            elapsed_centiseconds >=
            static_cast<std::int32_t>(kTimelineStepCount) * 100;
        return result;
    }

    const auto previous =
        playback_step_from_centiseconds(previous_elapsed_centiseconds);
    if (previous == result.second) {
        return result;
    }

    result.second_changed = true;
    result.triggers.reserve(kPitchRowCount);

    const auto second = static_cast<std::size_t>(result.second);

    for (std::size_t row = 0; row < kPitchRowCount; ++row) {
        const auto value = composition.cells[row][second];
        if (!is_machine_type(value)) {
            continue;
        }

        const auto type = static_cast<MachineType>(value);
        const auto machine = machine_for_type(type);
        const auto machine_index = static_cast<std::size_t>(machine);

        std::int32_t animation_write = 0;
        if (machine_animation_states[machine_index] == 0) {
            animation_write =
                static_cast<std::int32_t>(duration_for_type(type)) + 1;
            machine_animation_states[machine_index] = animation_write;
        }

        result.triggers.push_back({
            row,
            second,
            type,
            loaded_sound_slot_index(row, type),
            machine,
            animation_write,
        });
    }

    return result;
}

PaletteActionStep select_palette_from_edit(
    EditorRuntimeState& state,
    MachineType type) noexcept {

    PaletteActionStep result;

    if (state.delete_mode) {
        state.delete_mode = false;
    }

    state.selected_machine = type;
    state.activity_state = ActivityState::MachineSelected;
    state.cursor = {EditorCursorKind::Machine, type};
    result.preview_machine_sound = true;

    const auto index = machine_index(type);
    if (state.machine_animation_states[index] == 0) {
        const auto animation_state =
            static_cast<std::int32_t>(duration_for_type(type)) + 1;
        state.machine_animation_states[index] = animation_state;
        result.machine_animation_state_written = animation_state;
    }

    return result;
}

PaletteActionStep select_palette_while_selected(
    EditorRuntimeState& state,
    MachineType type) noexcept {

    PaletteActionStep result;

    if (state.activity_state == ActivityState::MachineSelected &&
        state.selected_machine == type) {
        state.activity_state = ActivityState::Edit;
        state.cursor = {};
        result.toggled_off = true;
        return result;
    }

    state.selected_machine = type;
    state.activity_state = ActivityState::MachineSelected;
    state.cursor = {EditorCursorKind::Machine, type};
    result.preview_machine_sound = true;
    return result;
}

GridActionStep handle_grid_action(
    EditorRuntimeState& state,
    Composition& composition,
    std::size_t row,
    std::size_t second) noexcept {

    GridActionStep result;
    if (row >= kPitchRowCount || second >= kTimelineStepCount) {
        return result;
    }

    auto& cell = composition.cells[row][second];

    if (state.activity_state == ActivityState::Edit) {
        if (cell == kEmptyCell) {
            return result;
        }

        const auto removed = remove_event_at(composition, row, second);
        if (!is_machine_type(removed)) {
            return result;
        }

        const auto removed_type = static_cast<MachineType>(removed);
        result.removed_existing = true;
        result.removed_type = removed_type;

        if (state.delete_mode) {
            state.delete_interaction_latch = true;
            return result;
        }

        state.selected_machine = removed_type;
        state.activity_state = ActivityState::MachineSelected;
        state.cursor = {EditorCursorKind::Machine, removed_type};
        result.picked_up_existing = true;
        return result;
    }

    if (state.activity_state != ActivityState::MachineSelected) {
        return result;
    }

    const auto held_type = state.selected_machine;
    result.placement_attempted = true;

    if (cell == kEmptyCell) {
        result.placed = place_event(
            composition, row, second, held_type);
        if (result.placed) {
            state.activity_state = ActivityState::Edit;
            state.cursor = {};
        }
        return result;
    }

    if (cell == kContinuationCell) {
        // State-1 retail code does not owner-resolve a continuation here. It
        // simply attempts placement into the occupied cell, which fails.
        result.placed = place_event(
            composition, row, second, held_type);
        return result;
    }

    if (!is_machine_type(cell)) {
        return result;
    }

    std::size_t restore_second = second;
    if (cell == static_cast<std::int32_t>(MachineType::Scoop2Second)) {
        // Exact retail oddity: type 9 preserves flags from cmp(value,9) and
        // scans left across preceding values >=9 before remembering the restore
        // location used only if replacement later fails.
        while (restore_second > 0 &&
               composition.cells[row][restore_second - 1] >= 9) {
            --restore_second;
        }
    }

    const auto removed = remove_event_at(composition, row, second);
    if (!is_machine_type(removed)) {
        return result;
    }

    const auto removed_type = static_cast<MachineType>(removed);
    result.removed_existing = true;
    result.removed_type = removed_type;

    state.cursor = {EditorCursorKind::Machine, removed_type};

    result.placed = place_event(
        composition, row, second, held_type);
    if (result.placed) {
        state.selected_machine = removed_type;
        result.swapped_existing = true;
        return result;
    }

    // Retail restores the removed event after a failed replacement and returns
    // the cursor to the still-held original type.
    const auto restored = place_event(
        composition, row, restore_second, removed_type);
    result.restored_existing_after_failed_swap = restored;
    state.cursor = {EditorCursorKind::Machine, held_type};
    return result;
}

ToolbarStep update_toolbar(
    EditorRuntimeState& state,
    Conductor conductor,
    const ToolbarInput& input) noexcept {

    ToolbarStep result;

    if (state.play_pending && !input.any_managed_sound_playing) {
        state.play_pending = false;
        state.activity_state = ActivityState::PreparePlayback;
        result.action = ToolbarActionKind::EnterPreparePlayback;
        return result;
    }

    std::optional<std::size_t> hit;
    for (std::size_t i = 0; i < kToolbarRects.size(); ++i) {
        if (strict_hit(kToolbarRects[i], input.mouse_x, input.mouse_y)) {
            hit = i;
            break;
        }
    }

    if (!hit) {
        state.previous_toolbar_hover = -1;
        return result;
    }

    const auto index = *hit;
    const auto control = static_cast<ToolbarControl>(index);
    result.control = control;

    const auto base = conductor_voice_base(conductor);

    if (!state.delete_mode) {
        if (input.pressed_visual) {
            result.visual = ToolbarVisual::Pressed;
        } else {
            result.visual = ToolbarVisual::Hover;
            if (state.previous_toolbar_hover !=
                static_cast<std::int32_t>(index)) {
                result.managed_sound_id =
                    base + static_cast<std::int32_t>(index) + 6;
                result.managed_sound_priority = 50;
                result.managed_sound_flag = 2;
            }
        }
    }

    state.previous_toolbar_hover = static_cast<std::int32_t>(index);

    if (!input.click_active) {
        return result;
    }

    switch (control) {
    case ToolbarControl::Play:
        if (state.activity_state == ActivityState::Edit &&
            !state.delete_mode) {
            state.play_pending = true;
            result.action = ToolbarActionKind::PlayVoicePending;
            result.stop_all_managed_sounds = true;
            result.managed_sound_id = base + 1;
            result.managed_sound_priority = 50;
            result.managed_sound_flag = 1;
        }
        break;

    case ToolbarControl::Stop:
        if (state.activity_state == ActivityState::Playing &&
            !state.delete_mode) {
            state.activity_state = ActivityState::Edit;
            result.action = ToolbarActionKind::StopPlayback;
            result.stop_backing_track = true;
            result.managed_sound_id =
                base + 3 + random_bit(input.random_mod_2);
            result.managed_sound_priority = 50;
            result.managed_sound_flag = 1;
        }
        break;

    case ToolbarControl::ClearAll:
        if (state.activity_state == ActivityState::Edit &&
            !state.delete_mode) {
            result.action =
                ToolbarActionKind::OpenClearAllConfirmation;
            result.open_confirmation = true;
            result.confirmation_context =
                kClearAllConfirmationContext;
        }
        break;

    case ToolbarControl::Delete:
        if (state.activity_state != ActivityState::Edit) {
            break;
        }

        if (!state.delete_mode) {
            state.delete_mode = true;
            state.cursor = {EditorCursorKind::Delete, std::nullopt};
            result.action = ToolbarActionKind::EnterDeleteMode;
            result.managed_sound_id =
                base + 9 + random_bit(input.random_mod_2);
            result.managed_sound_priority = 50;
            result.managed_sound_flag = 1;
        } else {
            state.delete_mode = false;
            state.cursor = {};
            result.action = ToolbarActionKind::LeaveDeleteMode;
        }
        break;
    }

    return result;
}

} // namespace btb::grand_opening
