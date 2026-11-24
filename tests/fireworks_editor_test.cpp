#include "btb/fireworks_editor.hpp"

#include <cassert>

using namespace btb::fireworks;

int main() {
    static_assert(
        placement_actor_for_row(0) == PlacementActorChannel::Bob);
    static_assert(
        placement_actor_for_row(1) == PlacementActorChannel::Bob);
    static_assert(
        placement_actor_for_row(2) == PlacementActorChannel::Wendy);

    static_assert(placement_actor_state_has_direct_retail_writer(
        PlacementActorState::PlacementQueued));
    static_assert(!placement_actor_state_has_direct_retail_writer(
        PlacementActorState::DormantLegacyMotion));

    static_assert(
        palette_voice_sound_id(FireworkType::RedAirbomb, 0) == 862);
    static_assert(
        palette_voice_sound_id(FireworkType::BlueAirbomb, 4) == 866);
    static_assert(
        palette_voice_sound_id(FireworkType::RedCandle, 0) == 899);
    static_assert(
        palette_voice_sound_id(FireworkType::BlueCandle, 4) == 903);

    static_assert(
        placement_voice_sound_id(PlacementActorChannel::Bob, 0) == 867);
    static_assert(
        placement_voice_sound_id(PlacementActorChannel::Bob, 3) == 870);
    static_assert(
        placement_voice_sound_id(PlacementActorChannel::Wendy, 0) == 904);
    static_assert(
        placement_voice_sound_id(PlacementActorChannel::Wendy, 3) == 907);

    static_assert(kPreviewX == 199);
    static_assert(kPreviewY == 39);

    static_assert(kInitialPlacementActorVisuals[0].x == 260);
    static_assert(kInitialPlacementActorVisuals[0].y == 168);
    static_assert(kInitialPlacementActorVisuals[1].x == 260);
    static_assert(kInitialPlacementActorVisuals[1].y == 0);

    constexpr auto bob_rect =
        placement_actor_source_rect(kInitialPlacementActorVisuals[0]);
    static_assert(bob_rect.left == 512);
    static_assert(bob_rect.top == 0);
    static_assert(bob_rect.right == 640);
    static_assert(bob_rect.bottom == 128);

    PlacementActorVisual idle_anim = kInitialPlacementActorVisuals[0];
    for (int i = 0; i < 5; ++i) {
        tick_idle_actor_animation(idle_anim);
    }
    assert(idle_anim.source_row == 0);
    assert(idle_anim.frame_tick == 5);
    tick_idle_actor_animation(idle_anim);
    assert(idle_anim.source_row == 1);
    assert(idle_anim.frame_tick == 0);

    idle_anim.source_row = 8;
    idle_anim.frame_tick = 5;
    tick_idle_actor_animation(idle_anim);
    assert(idle_anim.source_row == 0);
    assert(idle_anim.frame_tick == 0);

    static_assert(kBobFollowPath[0].x == 0);
    static_assert(kBobFollowPath[1].x == 240);
    static_assert(kBobFollowPath[1].y == 296);
    static_assert(kBobFollowPath[3].x == 640);
    static_assert(kWendyFollowPath[1].x == 300);
    static_assert(kWendyFollowPath[2].x == 500);
    static_assert(kWendyFollowPath[0].y == 128);

    // With no palette latch, mouse Y >160 makes Bob track and Wendy idle.
    // At mouse X 324 Bob's 128-pixel sprite is exactly centered, so both
    // actors remain standing and advance their idle frame timers.
    EditorRuntimeState mouse_follow;
    auto follow = update_actor_mouse_tracking(
        mouse_follow, 324, 200);
    assert(follow.idle_actor == PlacementActorChannel::Wendy);
    assert(follow.tracking_actor == PlacementActorChannel::Bob);
    assert(follow.split_y == 160);
    assert(follow.clamped_mouse_x == 324);
    assert(follow.interpolated_path_y == 168);
    assert(follow.standing);
    assert(!follow.moved_left && !follow.moved_right);
    assert(mouse_follow.actor_visuals[0].x == 260);
    assert(mouse_follow.actor_visuals[0].y == 168);
    assert(mouse_follow.actor_visuals[0].frame_tick == 1);
    assert(mouse_follow.actor_visuals[1].frame_tick == 1);

    // Bob's left path rises from center Y 177 to 296. At X=100 his center is
    // 164, which truncates to top-left Y=130. Moving right uses direction
    // column 1+2=3 and advances by exactly one pixel.
    EditorRuntimeState bob_curve;
    bob_curve.actor_visuals[0].x = 100;
    follow = update_actor_mouse_tracking(bob_curve, 575, 200);
    assert(follow.tracking_actor == PlacementActorChannel::Bob);
    assert(follow.interpolated_path_y == 130);
    assert(follow.vertical_direction_code == 1);
    assert(follow.moved_right);
    assert(bob_curve.actor_visuals[0].x == 101);
    assert(bob_curve.actor_visuals[0].source_column == 3);
    assert(bob_curve.actor_follow_threshold_bias == 25);

    // Once tracking starts, the 25 bias tightens the standing threshold from
    // 30 pixels to 5. A 20-pixel center error therefore keeps Bob moving.
    EditorRuntimeState hysteresis;
    hysteresis.actor_follow_threshold_bias = 25;
    follow = update_actor_mouse_tracking(hysteresis, 304, 200);
    assert(!follow.standing);
    assert(follow.moved_left);
    assert(hysteresis.actor_visuals[0].x == 259);
    assert(hysteresis.actor_visuals[0].source_column == 6);

    // Wendy's path is flat at center Y=128, so her sprite top remains Y=0.
    EditorRuntimeState wendy_follow;
    follow = update_actor_mouse_tracking(wendy_follow, 500, 100);
    assert(follow.idle_actor == PlacementActorChannel::Bob);
    assert(follow.tracking_actor == PlacementActorChannel::Wendy);
    assert(follow.interpolated_path_y == 0);
    assert(follow.vertical_direction_code == 0);
    assert(follow.moved_right);
    assert(wendy_follow.actor_visuals[1].x == 261);
    assert(wendy_follow.actor_visuals[1].source_column == 2);

    // Any state-1 palette latch changes the vertical actor-selection split to
    // 35, so the same Y=100 now chooses Bob rather than Wendy to track.
    EditorRuntimeState palette_split;
    palette_split.actor_states[1] = PlacementActorState::PaletteLatched;
    follow = update_actor_mouse_tracking(palette_split, 324, 100);
    assert(follow.split_y == 35);
    assert(follow.tracking_actor == PlacementActorChannel::Bob);

    // Walking animation jumps from the idle rows into row 13 on its first
    // six-tick advancement and then cycles through 13..24.
    EditorRuntimeState walking_rows;
    walking_rows.actor_visuals[0].x = 100;
    walking_rows.actor_visuals[0].source_row = 8;
    walking_rows.actor_visuals[0].frame_tick = 5;
    follow = update_actor_mouse_tracking(walking_rows, 575, 200);
    assert(walking_rows.actor_visuals[0].source_row == 13);
    assert(walking_rows.actor_visuals[0].frame_tick == 0);

    // Exact DrawFireworksEditor composition and row layering.
    LayoutData render_layout;
    render_layout.placement_runtime[Sequence::index(0, 0)] =
        {10, 100, 0, 0};
    render_layout.placement_runtime[Sequence::index(1, 0)] =
        {20, 200, 0, 0};
    render_layout.placement_runtime[Sequence::index(2, 0)] =
        {30, 300, 0, 0};

    Sequence render_sequence;
    assert(render_sequence.commit_placement(
        0, 0, FireworkType::RedAirbomb));
    assert(render_sequence.commit_placement(
        1, 0, FireworkType::SmallGreen));
    assert(render_sequence.commit_placement(
        2, 0, FireworkType::RedCandle));

    EditorRuntimeState render_state;
    auto frame = run_retail_editor_frame(
        render_state, render_sequence, render_layout, 324, 200);
    assert(frame.draw_commands.size() == 8);

    assert(frame.draw_commands[0].kind == EditorRenderKind::Background);
    assert(!frame.draw_commands[0].color_keyed);

    assert(frame.draw_commands[1].kind == EditorRenderKind::Firework);
    assert(frame.draw_commands[1].firework_type == FireworkType::RedAirbomb);
    assert(frame.draw_commands[1].x == 10);
    assert(frame.draw_commands[1].y == 70);

    assert(frame.draw_commands[2].kind == EditorRenderKind::Firework);
    assert(frame.draw_commands[2].firework_type == FireworkType::RedCandle);
    assert(frame.draw_commands[2].x == 34);
    assert(frame.draw_commands[2].y == 277);

    assert(frame.draw_commands[3].kind == EditorRenderKind::Actor);
    assert(frame.draw_commands[3].actor == PlacementActorChannel::Wendy);
    assert(frame.draw_commands[3].x == 260);
    assert(frame.draw_commands[3].y == 0);

    assert(frame.draw_commands[4].kind == EditorRenderKind::MiddleOverlay);
    assert(frame.draw_commands[4].x == 205);
    assert(frame.draw_commands[4].y == 104);

    assert(frame.draw_commands[5].kind == EditorRenderKind::Actor);
    assert(frame.draw_commands[5].actor == PlacementActorChannel::Bob);
    assert(frame.draw_commands[5].x == 260);
    assert(frame.draw_commands[5].y == 168);

    assert(frame.draw_commands[6].kind == EditorRenderKind::BottomOverlay);
    assert(frame.draw_commands[6].x == 37);
    assert(frame.draw_commands[6].y == 284);

    assert(frame.draw_commands[7].kind == EditorRenderKind::Firework);
    assert(frame.draw_commands[7].firework_type == FireworkType::SmallGreen);
    assert(frame.draw_commands[7].x == 20);
    assert(frame.draw_commands[7].y == 170);

    // State-11 commits occur after row 0/2 were already drawn. A new row-0
    // placement therefore does not appear until the next frame.
    LayoutData commit_layout;
    commit_layout.placement_runtime[Sequence::index(0, 0)] =
        {100, 100, 0, 0};
    commit_layout.placement_runtime[Sequence::index(1, 0)] =
        {200, 200, 0, 0};

    Sequence row0_commit_sequence;
    EditorRuntimeState row0_commit_state;
    row0_commit_state.selected_type = FireworkType::LargeRed;
    row0_commit_state.pending[0] = {0, 0};
    row0_commit_state.actor_states[0] =
        PlacementActorState::PlacementCommit;

    auto row0_commit_frame = run_retail_editor_frame(
        row0_commit_state,
        row0_commit_sequence,
        commit_layout,
        324,
        200);
    assert(row0_commit_frame.placement_ticks[0].committed);
    assert(
        row0_commit_sequence.at(0, 0) ==
        FireworkType::LargeRed);
    bool saw_row0_same_frame = false;
    for (const auto& command : row0_commit_frame.draw_commands) {
        if (command.kind == EditorRenderKind::Firework) {
            saw_row0_same_frame = true;
        }
    }
    assert(!saw_row0_same_frame);

    // Row 1 is drawn after the same commit point, so the identical state-11
    // commit becomes visible immediately in the center row.
    Sequence row1_commit_sequence;
    EditorRuntimeState row1_commit_state;
    row1_commit_state.selected_type = FireworkType::LargeBlue;
    row1_commit_state.pending[0] = {1, 0};
    row1_commit_state.actor_states[0] =
        PlacementActorState::PlacementCommit;

    auto row1_commit_frame = run_retail_editor_frame(
        row1_commit_state,
        row1_commit_sequence,
        commit_layout,
        324,
        200);
    assert(row1_commit_frame.placement_ticks[0].committed);
    assert(
        row1_commit_sequence.at(1, 0) ==
        FireworkType::LargeBlue);

    bool saw_center_same_frame = false;
    for (const auto& command : row1_commit_frame.draw_commands) {
        if (command.kind == EditorRenderKind::Firework &&
            command.firework_type == FireworkType::LargeBlue &&
            command.x == 200 &&
            command.y == 170) {
            saw_center_same_frame = true;
        }
    }
    assert(saw_center_same_frame);

    // Dormant retail state 12 is fully decoded even though no writer enters it.
    EditorRuntimeState dormant;
    dormant.actor_states[0] = PlacementActorState::DormantLegacyMotion;
    auto dormant_step = tick_dormant_legacy_motion(
        dormant, PlacementActorChannel::Bob);
    assert(dormant_step.active);
    assert(!dormant_step.arrived);
    assert(dormant_step.angle_degrees == 180);
    assert(dormant_step.delta_x == 0);
    assert(dormant_step.delta_y == 5);
    assert(dormant.actor_visuals[0].x == 260);
    assert(dormant.actor_visuals[0].y == 173);
    assert(dormant.actor_visuals[0].source_column == 4);

    // The state-12 animation row wraps 25 -> 13, not 25 -> 0.
    dormant.actor_visuals[0].x = 200;
    dormant.actor_visuals[0].y = 210;
    dormant.actor_visuals[0].source_row = 24;
    dormant.actor_visuals[0].frame_tick = 5;
    dormant_step = tick_dormant_legacy_motion(
        dormant, PlacementActorChannel::Bob);
    assert(dormant.actor_visuals[0].source_row == 13);
    assert(dormant.actor_visuals[0].frame_tick == 0);
    assert(dormant_step.angle_degrees == 90);
    assert(dormant.actor_visuals[0].source_column == 2);
    assert(dormant_step.delta_x == 5);
    assert(dormant_step.delta_y == 0);

    // Post-move distance <10 returns the actor to idle and source column 4.
    dormant.actor_visuals[0].x = 260;
    dormant.actor_visuals[0].y = 205;
    dormant.actor_states[0] = PlacementActorState::DormantLegacyMotion;
    dormant_step = tick_dormant_legacy_motion(
        dormant, PlacementActorChannel::Bob);
    assert(dormant_step.arrived);
    assert(dormant.actor_states[0] == PlacementActorState::Idle);
    assert(dormant.actor_visuals[0].source_column == 4);

    EditorRuntimeState preview_editor;
    preview_editor.selected_type = FireworkType::LargeBlue;
    preview_editor.internal_state = InternalState::PreviewSetup;
    PreviewRuntime preview_runtime;

    auto preview_step = update_preview_state(
        preview_editor, preview_runtime, false);
    assert(preview_editor.internal_state == InternalState::PreviewPlayback);
    assert(preview_runtime.type == FireworkType::LargeBlue);
    assert(preview_runtime.playback_state0 == 0);
    assert(preview_runtime.playback_state1 == 0);
    assert(!preview_step.decode_preview_movie);

    preview_step = update_preview_state(
        preview_editor, preview_runtime, false);
    assert(preview_step.decode_preview_movie);
    assert(preview_step.movie_index == 3);
    assert(preview_step.x == 199 && preview_step.y == 39);
    assert(!preview_step.restart_preview_movie);
    assert(preview_editor.internal_state == InternalState::PreviewPlayback);

    preview_step = update_preview_state(
        preview_editor, preview_runtime, true);
    assert(preview_step.decode_preview_movie);
    assert(preview_step.restart_preview_movie);
    assert(preview_editor.internal_state == InternalState::PreviewFinish);

    preview_step = update_preview_state(
        preview_editor, preview_runtime, false);
    assert(preview_editor.internal_state == InternalState::Editor);
    assert(preview_step.draw_editor);

    EditorRuntimeState palette;
    auto palette_result = begin_palette_selection(
        palette, FireworkType::MediumRed, 2);
    assert(palette_result.selection_latched);
    assert(palette_result.sound_id && *palette_result.sound_id == 864);
    assert(palette.selected_type == FireworkType::MediumRed);
    assert(
        palette.actor_states[0] == PlacementActorState::PaletteLatched);
    assert(palette.cursor.kind == EditorCursorKind::FireworkType);

    // The cursor still changes when the channel is already latched, but retail
    // does not replace the selected type or replay the palette voice.
    palette_result = begin_palette_selection(
        palette, FireworkType::LargeBlue, 1);
    assert(!palette_result.selection_latched);
    assert(!palette_result.sound_id);
    assert(palette.selected_type == FireworkType::MediumRed);
    assert(
        palette.cursor.firework_type &&
        *palette.cursor.firework_type == FireworkType::LargeBlue);

    // Normal state-0 dispatch immediately runs the release half too. Even
    // though the already-latched Bob channel suppresses another palette voice,
    // retail still replaces selected_type with the newly clicked palette ID.
    auto combined_palette = process_palette_action(
        palette, FireworkType::LargeBlue, 1);
    assert(!combined_palette.begin.selection_latched);
    assert(!combined_palette.begin.sound_id);
    assert(palette.selected_type == FireworkType::LargeBlue);

    Sequence sequence;
    EditorRuntimeState placement;
    placement.selected_type = FireworkType::LargeRed;

    auto release = complete_placement_release(
        placement, sequence, 1, 2, 3);
    assert(release.attempted);
    assert(release.valid);
    assert(release.channel == PlacementActorChannel::Bob);
    assert(release.sound_id && *release.sound_id == 870);
    assert(
        placement.actor_states[0] ==
        PlacementActorState::PlacementQueued);
    assert(placement.pending[0].row == 1);
    assert(placement.pending[0].column == 2);

    auto tick = tick_placement_actor(
        placement, sequence, PlacementActorChannel::Bob);
    assert(tick.queued_to_commit);
    assert(!tick.commit_attempted);
    assert(
        placement.actor_states[0] ==
        PlacementActorState::PlacementCommit);

    tick = tick_placement_actor(
        placement, sequence, PlacementActorChannel::Bob);
    assert(tick.commit_attempted);
    assert(tick.committed);
    assert(!tick.grid_full);
    assert(
        placement.actor_states[0] == PlacementActorState::Idle);
    assert(sequence.at(1, 2) == FireworkType::LargeRed);

    // Row 2 is the second/Wendy actor channel.
    placement.selected_type = FireworkType::SmallGreen;
    release = complete_placement_release(
        placement, sequence, 2, 5, 1);
    assert(release.channel == PlacementActorChannel::Wendy);
    assert(release.sound_id && *release.sound_id == 905);
    assert(
        placement.actor_states[1] ==
        PlacementActorState::PlacementQueued);

    // Calling only the release half rejects an occupied start cell.
    release = complete_placement_release(
        placement, sequence, 1, 2, 0);
    assert(!release.attempted);

    // Normal state-0 action ordering runs the begin half first. That removes an
    // existing retail start cell, after which the release half can schedule
    // replacement with the current palette selection.
    Sequence replacement_sequence;
    assert(replacement_sequence.commit_placement(
        0, 3, FireworkType::SmallGreen));

    EditorRuntimeState replacement;
    replacement.selected_type = FireworkType::LargeRed;

    const auto replacement_begin = begin_placement_region_action(
        replacement_sequence, 0, 3);
    assert(replacement_begin.had_existing_start);
    assert(
        replacement_begin.removed_type &&
        *replacement_begin.removed_type == FireworkType::SmallGreen);
    assert(replacement_sequence.empty(0, 3));

    auto replacement_release = complete_placement_release(
        replacement, replacement_sequence, 0, 3, 2);
    assert(replacement_release.attempted);
    assert(replacement_release.valid);
    assert(
        replacement.actor_states[0] ==
        PlacementActorState::PlacementQueued);

    auto replacement_tick = tick_placement_actor(
        replacement, replacement_sequence, PlacementActorChannel::Bob);
    assert(replacement_tick.queued_to_commit);
    replacement_tick = tick_placement_actor(
        replacement, replacement_sequence, PlacementActorChannel::Bob);
    assert(replacement_tick.committed);
    assert(
        replacement_sequence.at(0, 3) == FireworkType::LargeRed);

    // Full-grid feedback is channel-specific and happens after commit.
    Sequence almost_full;
    for (std::size_t row = 0; row < kTimelineRows; ++row) {
        for (std::size_t column = 0; column < kTimelineColumns; ++column) {
            if (row == 2 && column == 5) {
                continue;
            }
            assert(almost_full.commit_placement(
                row, column, FireworkType::SmallBlue));
        }
    }

    EditorRuntimeState final_cell;
    final_cell.selected_type = FireworkType::RedCandle;
    release = complete_placement_release(
        final_cell, almost_full, 2, 5, 0);
    assert(release.valid);

    tick = tick_placement_actor(
        final_cell, almost_full, PlacementActorChannel::Wendy);
    assert(tick.queued_to_commit);
    tick = tick_placement_actor(
        final_cell, almost_full, PlacementActorChannel::Wendy);
    assert(tick.committed);
    assert(tick.grid_full);
    assert(tick.sound_id && *tick.sound_id == kWendyFullGridVoice);

    // Exact control hit-point shift when either actor is active.
    EditorRuntimeState controls;
    auto point = retail_control_hit_point(controls, 300, 430);
    assert(point.first == 300 && point.second == 430);
    controls.actor_states[0] = PlacementActorState::PaletteLatched;
    point = retail_control_hit_point(controls, 300, 430);
    assert(point.first == 340 && point.second == 450);
    controls.actor_states[0] = PlacementActorState::Idle;

    // Play hover is one-shot per remembered control, and leaving empty space
    // does not clear retail's previous-control latch.
    EditorControlInput input{300, 430, false, false, false};
    auto output = update_editor_controls(controls, input);
    assert(output.control && *output.control == EditorAction::Play);
    assert(output.visual == EditorControlVisual::Hover);
    assert(output.surface && *output.surface == EditorControlSurface::PlayHover);
    assert(output.surface_x == 292 && output.surface_y == 416);
    assert(output.sound_id && *output.sound_id == kPlayHoverSoundId);

    output = update_editor_controls(controls, input);
    assert(output.visual == EditorControlVisual::Hover);
    assert(!output.sound_id);

    output = update_editor_controls(
        controls, {200, 200, false, false, false});
    assert(!output.control);
    assert(controls.previous_control_action ==
           static_cast<int>(EditorAction::Play));

    output = update_editor_controls(controls, input);
    assert(!output.sound_id);

    // Play click enters state 14 and emits ZFE_BOB_27.
    output = update_editor_controls(
        controls, {300, 430, true, false, false});
    assert(output.action == EditorControlActionKind::EnterPreShow);
    assert(output.sound_id && *output.sound_id == kPlayClickSoundId);
    assert(controls.internal_state == InternalState::PreShowMovieSetup);

    // Play is ignored while either placement channel is active.
    EditorRuntimeState blocked_play;
    blocked_play.actor_states[1] = PlacementActorState::PlacementQueued;
    output = update_editor_controls(
        blocked_play, {300, 430, true, false, false});
    assert(output.action == EditorControlActionKind::None);
    assert(blocked_play.internal_state == InternalState::Editor);

    // Delete toggles mode 13 and its cursor; only entering plays ID 890.
    EditorRuntimeState delete_mode;
    output = update_editor_controls(
        delete_mode, {500, 440, true, false, false});
    assert(output.action == EditorControlActionKind::EnterDeleteMode);
    assert(output.sound_id && *output.sound_id == kDeleteClickSoundId);
    assert(delete_mode.internal_state == InternalState::DeleteSelected);
    assert(delete_mode.cursor.kind == EditorCursorKind::Delete);

    output = update_editor_controls(
        delete_mode, {500, 440, true, false, false});
    assert(output.action == EditorControlActionKind::LeaveDeleteMode);
    assert(!output.sound_id);
    assert(delete_mode.internal_state == InternalState::Editor);
    assert(delete_mode.cursor.kind == EditorCursorKind::Normal);

    // The shared interaction gate cancels only palette-latched state==1 and
    // installs the exact ten-frame cooldown.
    EditorRuntimeState conflict;
    conflict.actor_states[0] = PlacementActorState::PaletteLatched;
    output = update_editor_controls(
        conflict, {450, 420, true, false, true});
    assert(
        output.action ==
        EditorControlActionKind::CancelConflictingInteraction);
    assert(conflict.actor_states[0] == PlacementActorState::Idle);
    assert(conflict.control_cooldown == 10);

    output = update_editor_controls(
        conflict, {500, 440, true, false, false});
    assert(output.action == EditorControlActionKind::None);
    assert(conflict.control_cooldown == 9);

    // Delete All opens generic Yes/No context 3 and emits ZFE_BOB_33.
    EditorRuntimeState delete_all;
    output = update_editor_controls(
        delete_all, {120, 440, true, false, false});
    assert(
        output.action ==
        EditorControlActionKind::OpenDeleteAllConfirmation);
    assert(output.sound_id && *output.sound_id == kDeleteAllClickSoundId);
    assert(output.open_yes_no_confirmation);
    assert(output.confirmation_context == 3);

    Sequence delete_all_sequence;
    assert(delete_all_sequence.commit_placement(
        0, 0, FireworkType::RedAirbomb));
    assert(delete_all_sequence.commit_placement(
        2, 5, FireworkType::BlueCandle));
    delete_all.actor_states[0] = PlacementActorState::PaletteLatched;
    delete_all.actor_states[1] = PlacementActorState::PlacementQueued;
    delete_all.internal_state = InternalState::DeleteSelected;
    delete_all.cursor = {EditorCursorKind::Delete, std::nullopt};

    const auto no_clear = apply_delete_all_confirmation(
        delete_all, delete_all_sequence, false);
    assert(!no_clear.applied);
    assert(delete_all_sequence.occupied_count() == 2);

    const auto did_clear = apply_delete_all_confirmation(
        delete_all, delete_all_sequence, true);
    assert(did_clear.applied);
    assert(delete_all_sequence.occupied_count() == 0);
    assert(delete_all.actor_states[0] == PlacementActorState::Idle);
    assert(delete_all.actor_states[1] == PlacementActorState::Idle);
    assert(delete_all.internal_state == InternalState::Editor);
    assert(delete_all.cursor.kind == EditorCursorKind::Normal);

    // Pressed and hover visuals preserve the exact per-control behavior.
    EditorRuntimeState pressed;
    output = update_editor_controls(
        pressed, {120, 440, false, true, false});
    assert(output.visual == EditorControlVisual::Pressed);
    assert(
        output.surface &&
        *output.surface == EditorControlSurface::DeleteAllPressed);
    assert(output.surface_x == 102 && output.surface_y == 416);
    assert(!output.sound_id);

    EditorRuntimeState play_pressed;
    output = update_editor_controls(
        play_pressed, {300, 430, false, true, false});
    assert(output.visual == EditorControlVisual::Pressed);
    assert(
        output.surface &&
        *output.surface == EditorControlSurface::PlayPressed);
    assert(output.surface_x == 292 && output.surface_y == 416);

    EditorRuntimeState delete_pressed;
    output = update_editor_controls(
        delete_pressed, {500, 440, false, true, false});
    assert(output.visual == EditorControlVisual::Pressed);
    assert(
        output.surface &&
        *output.surface == EditorControlSurface::DeletePressed);
    assert(output.surface_x == 483 && output.surface_y == 416);

    EditorRuntimeState delete_hover;
    output = update_editor_controls(
        delete_hover, {500, 440, false, false, false});
    assert(output.visual == EditorControlVisual::Hover);
    assert(
        output.surface &&
        *output.surface == EditorControlSurface::DeleteHover);
    assert(output.surface_x == 483 && output.surface_y == 416);
    assert(output.sound_id && *output.sound_id == kDeleteHoverSoundId);
}
