#include "btb/grand_opening_runtime.hpp"

#include <cassert>

using namespace btb::grand_opening;

int main() {
    static_assert(kPlaybackPreviousStepSentinel == 999999);

    constexpr auto prepared =
        prepare_playback(2, Conductor::Wendy);
    static_assert(prepared.next_state == ActivityState::Playing);
    static_assert(prepared.progress_index == 253);
    static_assert(prepared.backing_track == "Wendymt.wav");
    static_assert(
        prepared.previous_elapsed_centiseconds ==
        kPlaybackPreviousStepSentinel);
    static_assert(prepared.shared_completion_code == 6);

    static_assert(kMachineVisualSpecs[0].short_x == 0);
    static_assert(kMachineVisualSpecs[0].short_y == 130);
    static_assert(kMachineVisualSpecs[0].short_frame_width == 189);
    static_assert(kMachineVisualSpecs[0].long_frame_width == 246);
    static_assert(kMachineVisualSpecs[4].short_x == 393);
    static_assert(kMachineVisualSpecs[4].short_frame_height == 143);
    static_assert(kMachineVisualSpecs[4].long_frame_height == 167);
    static_assert(kShortMachineAnimationFrames == 15);
    static_assert(kLongMachineAnimationFrames == 30);

    MachineVisualState static_roley;
    auto machine_draw =
        tick_machine_visual(Machine::Roley, static_roley);
    assert(machine_draw.draw_animation_state == 0);
    assert(machine_draw.frame == 0);
    assert(machine_draw.x == 0 && machine_draw.y == 130);
    assert(machine_draw.source_left == 0);
    assert(machine_draw.source_right == 189);
    assert(machine_draw.source_bottom == 168);
    assert(!machine_draw.returns_to_static_after_draw);

    MachineVisualState short_roley{1, 14, 3};
    machine_draw =
        tick_machine_visual(Machine::Roley, short_roley);
    assert(machine_draw.draw_animation_state == 1);
    assert(machine_draw.frame == 0);
    assert(machine_draw.returns_to_static_after_draw);
    assert(short_roley.animation_state == 0);
    assert(short_roley.frame == 0);
    assert(short_roley.tick == 0);

    MachineVisualState long_muck{2, 29, 3};
    machine_draw =
        tick_machine_visual(Machine::Muck, long_muck);
    assert(machine_draw.draw_animation_state == 2);
    assert(machine_draw.frame == 0);
    assert(machine_draw.x == 86 && machine_draw.y == 93);
    assert(machine_draw.source_right == 222);
    assert(machine_draw.source_bottom == 177);
    assert(machine_draw.returns_to_static_after_draw);
    assert(long_muck.animation_state == 0);

    MachineVisualState moving_lofty{1, 4, 2};
    machine_draw =
        tick_machine_visual(Machine::Lofty, moving_lofty);
    assert(machine_draw.frame == 4);
    assert(moving_lofty.tick == 3);
    machine_draw =
        tick_machine_visual(Machine::Lofty, moving_lofty);
    assert(machine_draw.frame == 5);
    assert(moving_lofty.tick == 0);
    assert(machine_draw.source_left == 5 * 165);

    static_assert(kConductorVisualSpecs[0].x == 499);
    static_assert(kConductorVisualSpecs[0].y == 133);
    static_assert(kConductorVisualSpecs[0].frame_width == 85);
    static_assert(kConductorVisualSpecs[1].x == 504);
    static_assert(kConductorVisualSpecs[1].frame_height == 80);
    static_assert(kConductorVisualSpecs[2].x == 493);
    static_assert(kConductorVisualSpecs[2].frame_width == 78);

    constexpr auto bob_source = conductor_source_rect(Conductor::Bob, 3);
    static_assert(bob_source.left == 255);
    static_assert(bob_source.top == 0);
    static_assert(bob_source.right == 340);
    static_assert(bob_source.bottom == 78);

    ConductorAnimationState idle_bob{19,5};
    auto idle_step =
        tick_idle_conductor_animation(Conductor::Bob, idle_bob, 1);
    assert(idle_step.frame_advanced);
    assert(idle_step.normal_idle_restarted);
    assert(!idle_step.entered_special_idle);
    assert(idle_bob.frame == 0);
    assert(idle_bob.tick == 0);

    ConductorAnimationState special_idle{19,5};
    idle_step =
        tick_idle_conductor_animation(
            Conductor::FarmerPickles, special_idle, 0);
    assert(idle_step.frame_advanced);
    assert(idle_step.entered_special_idle);
    assert(!idle_step.normal_idle_restarted);
    assert(special_idle.frame == 20);

    special_idle.frame = 49;
    special_idle.tick = 5;
    idle_step =
        tick_idle_conductor_animation(
            Conductor::FarmerPickles, special_idle, 0);
    assert(idle_step.special_idle_completed);
    assert(special_idle.frame == 0);

    static_assert(kConductorPlaybackStartFrame == 50);
    static_assert(kConductorPlaybackEndExclusive[0] == 90);
    static_assert(kConductorPlaybackEndExclusive[1] == 85);
    static_assert(kConductorPlaybackEndExclusive[2] == 105);
    static_assert(kConductorPlaybackFrameTicks == 4);

    ConductorAnimationState bob_animation;
    for (int i = 0; i < 3; ++i) {
        const auto anim = tick_conductor_animation(
            Conductor::Bob, bob_animation);
        assert(!anim.frame_advanced);
        assert(bob_animation.frame == 0);
    }
    auto conductor_step = tick_conductor_animation(
        Conductor::Bob, bob_animation);
    assert(conductor_step.frame_advanced);
    assert(!conductor_step.wrapped);
    assert(bob_animation.frame == 50);
    assert(bob_animation.tick == 0);

    bob_animation.frame = 89;
    bob_animation.tick = 3;
    conductor_step = tick_conductor_animation(
        Conductor::Bob, bob_animation);
    assert(conductor_step.frame_advanced);
    assert(conductor_step.wrapped);
    assert(bob_animation.frame == 50);

    ConductorAnimationState wendy_animation{84, 3};
    conductor_step = tick_conductor_animation(
        Conductor::Wendy, wendy_animation);
    assert(conductor_step.wrapped);
    assert(wendy_animation.frame == 50);

    ConductorAnimationState farmer_animation{104, 3};
    conductor_step = tick_conductor_animation(
        Conductor::FarmerPickles, farmer_animation);
    assert(conductor_step.wrapped);
    assert(farmer_animation.frame == 50);

    Composition composition;
    assert(place_event(
        composition, 0, 0, MachineType::Roley1Second));
    assert(place_event(
        composition, 1, 0, MachineType::Muck2Second));
    assert(place_event(
        composition, 2, 1, MachineType::Lofty1Second));
    assert(place_event(
        composition, 4, 23, MachineType::Scoop2Second) == false);
    assert(place_event(
        composition, 4, 23, MachineType::Scoop1Second));

    std::array<std::int32_t,5> animations{};

    // State 8 seeds previous elapsed with 999999, forcing the first second to
    // trigger immediately even though elapsed centiseconds begin at zero.
    auto step = update_playback_second(
        composition,
        0,
        kPlaybackPreviousStepSentinel,
        animations);
    assert(step.second == 0);
    assert(step.second_changed);
    assert(!step.timeline_complete);
    assert(step.triggers.size() == 2);

    assert(step.triggers[0].pitch_row == 0);
    assert(step.triggers[0].machine_type == MachineType::Roley1Second);
    assert(step.triggers[0].loaded_sound_slot == 40);
    assert(step.triggers[0].machine == Machine::Roley);
    assert(step.triggers[0].machine_animation_state_written == 1);

    assert(step.triggers[1].pitch_row == 1);
    assert(step.triggers[1].machine_type == MachineType::Muck2Second);
    assert(step.triggers[1].loaded_sound_slot == 33);
    assert(step.triggers[1].machine == Machine::Muck);
    assert(step.triggers[1].machine_animation_state_written == 2);

    assert(animations[0] == 1);
    assert(animations[1] == 2);

    // No re-trigger occurs while still inside the same one-second bucket.
    step = update_playback_second(
        composition, 99, 0, animations);
    assert(step.second == 0);
    assert(!step.second_changed);
    assert(step.triggers.empty());

    // Crossing to second 1 triggers the row-2 Lofty event.
    step = update_playback_second(
        composition, 100, 99, animations);
    assert(step.second == 1);
    assert(step.second_changed);
    assert(step.triggers.size() == 1);
    assert(step.triggers[0].pitch_row == 2);
    assert(step.triggers[0].machine_type == MachineType::Lofty1Second);
    assert(step.triggers[0].loaded_sound_slot == 24);
    assert(step.triggers[0].machine_animation_state_written == 1);

    // Continuation cells are not machine triggers.
    step = update_playback_second(
        composition, 600, 599, animations);
    assert(step.second == 6);
    assert(step.second_changed);
    assert(step.triggers.empty());

    // If a machine animation is already active, retail still plays the WAV
    // but does not overwrite that machine's current animation state.
    Composition repeated;
    assert(place_event(
        repeated, 4, 2, MachineType::Roley2Second));
    animations[0] = 7;
    step = update_playback_second(
        repeated, 200, 100, animations);
    assert(step.triggers.size() == 1);
    assert(step.triggers[0].loaded_sound_slot == 1);
    assert(step.triggers[0].machine_animation_state_written == 0);
    assert(animations[0] == 7);

    step = update_playback_second(
        composition, 2300, 2299, animations);
    assert(step.second == 23);
    assert(step.triggers.size() == 1);
    assert(step.triggers[0].machine_type == MachineType::Scoop1Second);
    assert(step.triggers[0].loaded_sound_slot == 8);

    step = update_playback_second(
        composition, 2400, 2399, animations);
    assert(step.second == -1);
    assert(step.timeline_complete);
    assert(step.triggers.empty());

    constexpr auto stop =
        update_playing_state(true, true);
    static_assert(stop.next_state == ActivityState::Edit);
    static_assert(stop.stop_backing_track);

    constexpr auto ended =
        update_playing_state(false, false);
    static_assert(ended.next_state == ActivityState::Edit);
    static_assert(!ended.stop_backing_track);

    constexpr auto still_playing =
        update_playing_state(false, true);
    static_assert(still_playing.next_state == ActivityState::Playing);

    // Exact editor palette behavior.
    EditorRuntimeState editor;
    editor.delete_mode = true;
    auto palette_step = select_palette_from_edit(
        editor, MachineType::Muck2Second);
    assert(editor.activity_state == ActivityState::MachineSelected);
    assert(editor.selected_machine == MachineType::Muck2Second);
    assert(!editor.delete_mode);
    assert(editor.cursor.kind == EditorCursorKind::Machine);
    assert(editor.cursor.machine == MachineType::Muck2Second);
    assert(palette_step.preview_machine_sound);
    assert(palette_step.machine_animation_state_written == 2);
    assert(editor.machine_animation_states[1] == 2);

    palette_step = select_palette_while_selected(
        editor, MachineType::Muck2Second);
    assert(palette_step.toggled_off);
    assert(!palette_step.preview_machine_sound);
    assert(editor.activity_state == ActivityState::Edit);
    assert(editor.cursor.kind == EditorCursorKind::Normal);

    editor.activity_state = ActivityState::MachineSelected;
    editor.selected_machine = MachineType::Roley1Second;
    editor.machine_animation_states[2] = 0;
    palette_step = select_palette_while_selected(
        editor, MachineType::Lofty1Second);
    assert(!palette_step.toggled_off);
    assert(palette_step.preview_machine_sound);
    assert(palette_step.machine_animation_state_written == 0);
    assert(editor.selected_machine == MachineType::Lofty1Second);
    assert(editor.machine_animation_states[2] == 0);

    // Edit-state grid click owner-resolves a continuation and picks up the
    // owning brick when delete mode is off.
    Composition editor_grid;
    assert(place_event(
        editor_grid, 0, 5, MachineType::Muck2Second));
    EditorRuntimeState pickup;
    auto grid_step = handle_grid_action(
        pickup, editor_grid, 0, 6);
    assert(grid_step.removed_existing);
    assert(grid_step.picked_up_existing);
    assert(grid_step.removed_type == MachineType::Muck2Second);
    assert(editor_grid.cells[0][5] == kEmptyCell);
    assert(editor_grid.cells[0][6] == kEmptyCell);
    assert(pickup.activity_state == ActivityState::MachineSelected);
    assert(pickup.selected_machine == MachineType::Muck2Second);

    // Delete mode removes the whole event but keeps edit mode and raises the
    // per-frame delete interaction latch.
    Composition delete_grid;
    assert(place_event(
        delete_grid, 1, 3, MachineType::Roley2Second));
    EditorRuntimeState delete_editor;
    delete_editor.delete_mode = true;
    delete_editor.cursor = {EditorCursorKind::Delete, std::nullopt};
    grid_step = handle_grid_action(
        delete_editor, delete_grid, 1, 4);
    assert(grid_step.removed_existing);
    assert(!grid_step.picked_up_existing);
    assert(delete_editor.delete_interaction_latch);
    assert(delete_editor.activity_state == ActivityState::Edit);
    assert(delete_grid.cells[1][3] == kEmptyCell);
    assert(delete_grid.cells[1][4] == kEmptyCell);

    // A selected brick placed into an empty cell returns to edit mode.
    Composition place_grid;
    EditorRuntimeState placer;
    placer.activity_state = ActivityState::MachineSelected;
    placer.selected_machine = MachineType::Dizzy1Second;
    placer.cursor = {
        EditorCursorKind::Machine,
        MachineType::Dizzy1Second,
    };
    grid_step = handle_grid_action(
        placer, place_grid, 2, 7);
    assert(grid_step.placement_attempted);
    assert(grid_step.placed);
    assert(place_grid.cells[2][7] ==
           static_cast<int>(MachineType::Dizzy1Second));
    assert(placer.activity_state == ActivityState::Edit);
    assert(placer.cursor.kind == EditorCursorKind::Normal);

    // Clicking an occupied start while holding another brick swaps them.
    Composition swap_grid;
    assert(place_event(
        swap_grid, 3, 9, MachineType::Muck1Second));
    EditorRuntimeState swapper;
    swapper.activity_state = ActivityState::MachineSelected;
    swapper.selected_machine = MachineType::Roley1Second;
    swapper.cursor = {
        EditorCursorKind::Machine,
        MachineType::Roley1Second,
    };
    grid_step = handle_grid_action(
        swapper, swap_grid, 3, 9);
    assert(grid_step.removed_existing);
    assert(grid_step.placed);
    assert(grid_step.swapped_existing);
    assert(grid_step.removed_type == MachineType::Muck1Second);
    assert(swap_grid.cells[3][9] ==
           static_cast<int>(MachineType::Roley1Second));
    assert(swapper.selected_machine == MachineType::Muck1Second);
    assert(swapper.activity_state == ActivityState::MachineSelected);

    // If the replacement cannot fit, retail restores the removed event and
    // keeps the originally held brick/cursor.
    Composition failed_swap_grid;
    assert(place_event(
        failed_swap_grid, 0, 23, MachineType::Scoop1Second));
    EditorRuntimeState failed_swap;
    failed_swap.activity_state = ActivityState::MachineSelected;
    failed_swap.selected_machine = MachineType::Roley2Second;
    failed_swap.cursor = {
        EditorCursorKind::Machine,
        MachineType::Roley2Second,
    };
    grid_step = handle_grid_action(
        failed_swap, failed_swap_grid, 0, 23);
    assert(grid_step.removed_existing);
    assert(!grid_step.placed);
    assert(grid_step.restored_existing_after_failed_swap);
    assert(failed_swap_grid.cells[0][23] ==
           static_cast<int>(MachineType::Scoop1Second));
    assert(failed_swap.selected_machine == MachineType::Roley2Second);
    assert(failed_swap.cursor.machine == MachineType::Roley2Second);

    // State 1 does not owner-resolve continuation cells; direct placement into
    // that occupied cell simply fails.
    Composition continuation_grid;
    assert(place_event(
        continuation_grid, 4, 2, MachineType::Scoop2Second));
    EditorRuntimeState continuation_editor;
    continuation_editor.activity_state = ActivityState::MachineSelected;
    continuation_editor.selected_machine = MachineType::Roley1Second;
    continuation_editor.cursor = {
        EditorCursorKind::Machine,
        MachineType::Roley1Second,
    };
    grid_step = handle_grid_action(
        continuation_editor, continuation_grid, 4, 3);
    assert(grid_step.placement_attempted);
    assert(!grid_step.placed);
    assert(!grid_step.removed_existing);
    assert(continuation_grid.cells[4][2] ==
           static_cast<int>(MachineType::Scoop2Second));
    assert(continuation_grid.cells[4][3] == kContinuationCell);

    // Delete mode is scoped to the wall's strict Y band 290..400.
    EditorRuntimeState delete_lifetime;
    delete_lifetime.delete_mode = true;
    delete_lifetime.cursor = {EditorCursorKind::Delete, std::nullopt};
    assert(!maintain_delete_mode_after_input(delete_lifetime, 300));
    assert(delete_lifetime.delete_mode);

    assert(maintain_delete_mode_after_input(delete_lifetime, 401));
    assert(!delete_lifetime.delete_mode);
    assert(delete_lifetime.cursor.kind == EditorCursorKind::Normal);

    // A successful deletion sets a one-frame latch that suppresses the
    // outside-wall auto-cancel once, then is consumed.
    delete_lifetime.delete_mode = true;
    delete_lifetime.delete_interaction_latch = true;
    delete_lifetime.cursor = {EditorCursorKind::Delete, std::nullopt};
    assert(!maintain_delete_mode_after_input(delete_lifetime, 417));
    assert(delete_lifetime.delete_mode);
    assert(!delete_lifetime.delete_interaction_latch);
    assert(maintain_delete_mode_after_input(delete_lifetime, 417));
    assert(!delete_lifetime.delete_mode);

    // Clear-All confirmation is the top-of-update 120-cell reset.
    Composition clear_grid;
    assert(place_event(
        clear_grid, 0, 0, MachineType::Roley1Second));
    assert(!apply_clear_all_confirmation(clear_grid, false));
    assert(clear_grid.cells[0][0] == 0);
    assert(apply_clear_all_confirmation(clear_grid, true));
    for (const auto& row : clear_grid.cells) {
        for (const auto cell : row) {
            assert(cell == kEmptyCell);
        }
    }

    // Exact 132-region editor hit table.
    static_assert(kEditorHitRegionCount == 132);
    static_assert(kWallHitRegionCount == 120);
    static_assert(kPaletteHitRegionCount == 10);
    static_assert(kSpecialHitRegionCount == 2);

    constexpr auto first_wall = kEditorHitRegions[0];
    static_assert(first_wall.action == kWallAction);
    static_assert(first_wall.rect.left == 44);
    static_assert(first_wall.rect.top == 310);
    static_assert(first_wall.rect.right == 67);
    static_assert(first_wall.rect.bottom == 329);

    constexpr auto last_wall = kEditorHitRegions[119];
    static_assert(last_wall.action == kWallAction);
    static_assert(last_wall.rect.left == 573);
    static_assert(last_wall.rect.top == 386);
    static_assert(last_wall.rect.right == 596);
    static_assert(last_wall.rect.bottom == 405);

    static_assert(kEditorHitRegions[120].action == 0);
    static_assert(kEditorHitRegions[120].rect.left == 232);
    static_assert(kEditorHitRegions[129].action == 9);
    static_assert(kEditorHitRegions[129].rect.left == 447);
    static_assert(kEditorHitRegions[130].action == 22);
    static_assert(kEditorHitRegions[130].rect.left == 251);
    static_assert(kEditorHitRegions[131].action == 23);
    static_assert(kEditorHitRegions[131].rect.left == 340);

    constexpr auto wall_pos = composition_cell_draw_position(4, 23);
    static_assert(wall_pos.x == 573);
    static_assert(wall_pos.y == 386);

    // Normal editor states use +5/+5 for the hit test. Pointer (40,306)
    // becomes (45,311), strictly inside the first wall cell.
    constexpr auto hit_wall =
        hit_test_editor_regions(ActivityState::Edit, 40, 306);
    static_assert(hit_wall.action == 11);
    static_assert(hit_wall.region_index == 0);
    static_assert(hit_wall.adjusted_x == 45);
    static_assert(hit_wall.adjusted_y == 311);

    // State 1 uses +10/+10 instead. This point resolves palette action 0.
    constexpr auto hit_palette =
        hit_test_editor_regions(
            ActivityState::MachineSelected, 223, 275);
    static_assert(hit_palette.action == 0);
    static_assert(hit_palette.region_index == 120);
    static_assert(hit_palette.adjusted_x == 233);
    static_assert(hit_palette.adjusted_y == 285);

    // Strict bounds reject exact edges after hotspot adjustment.
    constexpr auto edge_region =
        hit_test_editor_regions(ActivityState::Edit, 39, 305);
    static_assert(edge_region.action == -1);

    constexpr auto secondary_stop =
        hit_test_editor_regions(ActivityState::Playing, 247, 411);
    static_assert(secondary_stop.action == 22);
    static_assert(secondary_stop.region_index == 130);

    constexpr auto legacy_play =
        hit_test_editor_regions(ActivityState::Edit, 336, 411);
    static_assert(legacy_play.action == 23);
    static_assert(legacy_play.region_index == 131);

    // Exact conductor-specific toolbar voice bases.
    static_assert(conductor_voice_base(Conductor::Bob) == 510);
    static_assert(conductor_voice_base(Conductor::Wendy) == 523);
    static_assert(conductor_voice_base(Conductor::FarmerPickles) == 536);
    static_assert(kClearAllConfirmationContext == 2);
    static_assert(kToolbarRects[0].left == 324);
    static_assert(kToolbarRects[1].left == 260);
    static_assert(kToolbarRects[2].left == 103);
    static_assert(kToolbarRects[3].left == 481);

    EditorRuntimeState toolbar;
    auto toolbar_step = update_toolbar(
        toolbar,
        Conductor::Bob,
        {325, 417, true, false, false, 0});
    assert(toolbar_step.control == ToolbarControl::Play);
    assert(toolbar_step.visual == ToolbarVisual::Hover);
    assert(toolbar_step.surface &&
           *toolbar_step.surface == ToolbarSurface::PlayHover);
    assert(toolbar_step.surface_x == 324);
    assert(toolbar_step.surface_y == 416);
    assert(toolbar_step.hover_sound_id &&
           *toolbar_step.hover_sound_id == 516); // MP_BOB_07
    assert(toolbar_step.action_sound_id &&
           *toolbar_step.action_sound_id == 511); // MP_BOB_02
    assert(toolbar_step.stop_all_managed_sounds);
    assert(toolbar_step.action == ToolbarActionKind::PlayVoicePending);
    assert(toolbar.play_pending);
    assert(toolbar.activity_state == ActivityState::Edit);

    // While the Play voice is still active, state 8 is not entered.
    toolbar_step = update_toolbar(
        toolbar,
        Conductor::Bob,
        {0, 0, false, false, true, 0});
    assert(toolbar.play_pending);
    assert(toolbar.activity_state == ActivityState::Edit);
    assert(toolbar_step.action == ToolbarActionKind::None);

    // The first update after managed audio becomes idle consumes the latch and
    // enters state 8 before normal toolbar hit processing.
    toolbar_step = update_toolbar(
        toolbar,
        Conductor::Bob,
        {325, 417, false, false, false, 0});
    assert(!toolbar.play_pending);
    assert(toolbar.activity_state == ActivityState::PreparePlayback);
    assert(toolbar_step.action ==
           ToolbarActionKind::EnterPreparePlayback);
    assert(!toolbar_step.control);

    // Stop is state-9-only and chooses MP_*_04/_05 using rand()%2.
    EditorRuntimeState stop_toolbar;
    stop_toolbar.activity_state = ActivityState::Playing;
    toolbar_step = update_toolbar(
        stop_toolbar,
        Conductor::Wendy,
        {261, 417, true, false, false, 1});
    assert(toolbar_step.control == ToolbarControl::Stop);
    assert(toolbar_step.surface &&
           *toolbar_step.surface == ToolbarSurface::StopHover);
    assert(toolbar_step.surface_x == 260);
    assert(toolbar_step.surface_y == 416);
    assert(toolbar_step.hover_sound_id &&
           *toolbar_step.hover_sound_id == 530); // MP_WEN_08
    assert(toolbar_step.action_sound_id &&
           *toolbar_step.action_sound_id == 527); // MP_WEN_05
    assert(toolbar_step.stop_backing_track);
    assert(stop_toolbar.activity_state == ActivityState::Edit);

    // Clear All opens Deletebricks.bmp through confirmation context 2.
    EditorRuntimeState clear_toolbar;
    toolbar_step = update_toolbar(
        clear_toolbar,
        Conductor::FarmerPickles,
        {104, 417, true, false, false, 0});
    assert(toolbar_step.control == ToolbarControl::ClearAll);
    assert(toolbar_step.surface &&
           *toolbar_step.surface == ToolbarSurface::ClearAllHover);
    assert(toolbar_step.surface_x == 103);
    assert(toolbar_step.surface_y == 416);
    assert(toolbar_step.action ==
           ToolbarActionKind::OpenClearAllConfirmation);
    assert(toolbar_step.open_confirmation);
    assert(toolbar_step.confirmation_context == 2);
    assert(toolbar_step.hover_sound_id &&
           *toolbar_step.hover_sound_id == 544); // MP_PIC_09
    assert(!toolbar_step.action_sound_id);

    // Delete toggles its cursor. Entering may emit both first-hover MP_*_10
    // and click MP_*_10/_11; while delete mode is active hover rendering is
    // suppressed, but a second click still toggles it off.
    EditorRuntimeState delete_toolbar;
    toolbar_step = update_toolbar(
        delete_toolbar,
        Conductor::Bob,
        {482, 417, true, false, false, 1});
    assert(toolbar_step.action == ToolbarActionKind::EnterDeleteMode);
    assert(toolbar_step.surface &&
           *toolbar_step.surface == ToolbarSurface::DeleteHover);
    assert(toolbar_step.surface_x == 481);
    assert(toolbar_step.surface_y == 416);
    assert(delete_toolbar.delete_mode);
    assert(delete_toolbar.cursor.kind == EditorCursorKind::Delete);
    assert(toolbar_step.hover_sound_id &&
           *toolbar_step.hover_sound_id == 519); // MP_BOB_10
    assert(toolbar_step.action_sound_id &&
           *toolbar_step.action_sound_id == 520); // MP_BOB_11

    toolbar_step = update_toolbar(
        delete_toolbar,
        Conductor::Bob,
        {482, 417, true, false, false, 0});
    assert(toolbar_step.action == ToolbarActionKind::LeaveDeleteMode);
    assert(!delete_toolbar.delete_mode);
    assert(delete_toolbar.cursor.kind == EditorCursorKind::Normal);
    assert(toolbar_step.visual == ToolbarVisual::None);
    assert(!toolbar_step.hover_sound_id);
    assert(!toolbar_step.action_sound_id);

    // Pressed rendering selects the matching depressed surface.
    EditorRuntimeState pressed_toolbar;
    toolbar_step = update_toolbar(
        pressed_toolbar,
        Conductor::Bob,
        {325, 417, false, true, false, 0});
    assert(toolbar_step.visual == ToolbarVisual::Pressed);
    assert(toolbar_step.surface &&
           *toolbar_step.surface == ToolbarSurface::PlayPressed);
    assert(toolbar_step.surface_x == 324);
    assert(toolbar_step.surface_y == 416);

    // State-9's secondary action-22 region always draws the Stop-red surface
    // at the same retail origin and a click independently stops playback.
    constexpr auto secondary_hover =
        update_secondary_stop_region(22, false);
    static_assert(secondary_hover.draw_stop_hover_surface);
    static_assert(secondary_hover.surface_x == 260);
    static_assert(secondary_hover.surface_y == 416);
    static_assert(!secondary_hover.stop_backing_track);
    static_assert(
        secondary_hover.next_state == ActivityState::Playing);

    constexpr auto secondary_click =
        update_secondary_stop_region(22, true);
    static_assert(secondary_click.draw_stop_hover_surface);
    static_assert(secondary_click.stop_backing_track);
    static_assert(
        secondary_click.next_state == ActivityState::Edit);

    constexpr auto legacy_secondary =
        update_secondary_stop_region(23, true);
    static_assert(!legacy_secondary.draw_stop_hover_surface);
    static_assert(!legacy_secondary.stop_backing_track);
    static_assert(
        legacy_secondary.next_state == ActivityState::Playing);

    // Strict hit testing excludes rectangle edges.
    EditorRuntimeState edge_toolbar;
    toolbar_step = update_toolbar(
        edge_toolbar,
        Conductor::Bob,
        {324, 417, false, false, false, 0});
    assert(!toolbar_step.control);

    static_assert(kMusicChooserOuterState == 0x2A);
    static_assert(kPlayAgainOuterState == 0x3C);
    static_assert(kSharedMovieOuterState == 0x40);
    static_assert(kBobsBandPlayAgainContext == 0x2E);

    constexpr auto normal_edit =
        update_outer_activity({ActivityState::Edit, false, false, false});
    static_assert(!normal_edit.return_abort);
    static_assert(!normal_edit.save_and_unload);

    constexpr auto leave_to_music =
        update_outer_activity({ActivityState::Edit, false, true, false});
    static_assert(leave_to_music.return_abort);
    static_assert(leave_to_music.save_and_unload);
    static_assert(leave_to_music.clear_leave_activity_request);
    static_assert(
        leave_to_music.saved_frontend_state &&
        *leave_to_music.saved_frontend_state == 0x2A);
    static_assert(
        leave_to_music.outer_state &&
        *leave_to_music.outer_state == 0x2A);

    constexpr auto leave_to_shared_movie =
        update_outer_activity({ActivityState::Edit, false, true, true});
    static_assert(
        leave_to_shared_movie.outer_state &&
        *leave_to_shared_movie.outer_state == 0x40);

    constexpr auto quit_with_shared_movie =
        update_outer_activity({ActivityState::Edit, true, false, true});
    static_assert(quit_with_shared_movie.return_abort);
    static_assert(quit_with_shared_movie.save_and_unload);
    static_assert(
        quit_with_shared_movie.outer_state &&
        *quit_with_shared_movie.outer_state == 0x40);

    constexpr auto completion =
        update_outer_activity(
            {ActivityState::ExitToPlayAgain, false, false, false});
    static_assert(completion.return_abort);
    static_assert(completion.save_and_unload);
    static_assert(completion.prepare_play_again);
    static_assert(
        completion.outer_state &&
        *completion.outer_state == 0x3C);
    static_assert(
        completion.play_again_context &&
        *completion.play_again_context == 0x2E);
    static_assert(completion.clear_shared_transition_flag);
}
