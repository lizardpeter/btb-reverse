#include "btb/application_runtime.hpp"

#include <cassert>

using namespace btb::application;

int main() {
    static_assert(kCdValidationThreshold == 100);
    static_assert(kExpectedCdDriveType == 5);

    constexpr auto cd100 =
        advance_cd_validation(99, 5, true);
    static_assert(cd100.next_counter == 100);
    static_assert(!cd100.query_drive_type);

    // Retail increments first and checks strictly >100.
    constexpr auto cd101 =
        advance_cd_validation(100, 5, true);
    static_assert(cd101.next_counter == 0);
    static_assert(cd101.query_drive_type);
    static_assert(cd101.query_volume_information);
    static_assert(!cd101.abort_for_removed_cd);

    constexpr auto bad_drive =
        advance_cd_validation(100, 3, true);
    static_assert(bad_drive.query_drive_type);
    static_assert(!bad_drive.query_volume_information);
    static_assert(bad_drive.abort_for_removed_cd);

    constexpr auto bad_label =
        advance_cd_validation(100, 5, false);
    static_assert(bad_label.query_volume_information);
    static_assert(bad_label.abort_for_removed_cd);

    constexpr auto no_reload =
        surface_reload_step(false, true);
    static_assert(!no_reload.reload_registered_bitmaps);

    constexpr auto plain_reload =
        surface_reload_step(true, false);
    static_assert(plain_reload.reload_registered_bitmaps);
    static_assert(plain_reload.clear_reload_flag);
    static_assert(!plain_reload.clear_help_mode);

    constexpr auto help_reload =
        surface_reload_step(true, true);
    static_assert(help_reload.reload_registered_bitmaps);
    static_assert(help_reload.clear_reload_flag);
    static_assert(help_reload.clear_help_mode);
    static_assert(help_reload.clear_primary_input_pulse);
    static_assert(help_reload.stop_all_managed_sounds);
    static_assert(help_reload.restore_default_cursor);
    static_assert(help_reload.set_shared_delay_50);

    static_assert(kDdErrExclusiveModeAlreadySet ==
                  static_cast<std::int32_t>(0x887600E1U));
    static_assert(kDdErrNoExclusiveMode ==
                  static_cast<std::int32_t>(0x88760245U));
    static_assert(kDdErrWrongMode ==
                  static_cast<std::int32_t>(0x8876024BU));
    static_assert(kDdErrSurfaceLost ==
                  static_cast<std::int32_t>(0x887601C2U));

    constexpr auto coop_ok = cooperative_level_step(0);
    static_assert(coop_ok.continue_to_present);

    constexpr auto coop_busy =
        cooperative_level_step(kDdErrExclusiveModeAlreadySet);
    static_assert(coop_busy.sleep_10ms_and_return);
    static_assert(!coop_busy.recreate_display_and_return);

    constexpr auto coop_noexclusive =
        cooperative_level_step(kDdErrNoExclusiveMode);
    static_assert(coop_noexclusive.sleep_10ms_and_return);

    constexpr auto coop_wrong =
        cooperative_level_step(kDdErrWrongMode);
    static_assert(coop_wrong.recreate_display_and_return);

    constexpr auto coop_other =
        cooperative_level_step(
            static_cast<std::int32_t>(0x88760001U));
    static_assert(coop_other.return_error);

    constexpr auto present_ok = present_frame_step(0);
    static_assert(!present_ok.restore_all_surfaces);
    static_assert(!present_ok.return_error);

    constexpr auto present_lost =
        present_frame_step(kDdErrSurfaceLost);
    static_assert(present_lost.restore_all_surfaces);
    static_assert(!present_lost.return_error);

    constexpr auto present_error =
        present_frame_step(
            static_cast<std::int32_t>(0x88760002U));
    static_assert(!present_error.restore_all_surfaces);
    static_assert(present_error.return_error);

    // Duplicate timeGetTime timestamps return before reload/sound/game flow.
    constexpr auto duplicate = run_active_frame_step({
        0, 5, true,
        1234, 1234,
        true, true, 2,
        0, 0,
    });
    static_assert(duplicate.duplicate_timestamp_return);
    static_assert(duplicate.return_success);
    static_assert(!duplicate.update_previous_time);
    static_assert(!duplicate.reload.reload_registered_bitmaps);
    static_assert(!duplicate.reap_finished_sounds);
    static_assert(!duplicate.poll_direct_input);
    static_assert(!duplicate.present);

    // A pending reload while contextual help is active tears help state down
    // before dispatch, therefore this frame goes through MainGameFlow.
    constexpr auto reloaded_help = run_active_frame_step({
        1, 5, true,
        1235, 1234,
        true, true, 2,
        0, 0,
    });
    static_assert(reloaded_help.reload.reload_registered_bitmaps);
    static_assert(reloaded_help.reload.clear_help_mode);
    static_assert(reloaded_help.update_previous_time);
    static_assert(reloaded_help.next_previous_time == 1235);
    static_assert(reloaded_help.reap_finished_sounds);
    static_assert(reloaded_help.clear_shared_frame_flag);
    static_assert(
        reloaded_help.dispatch == ActivityDispatch::MainGameFlow);
    static_assert(reloaded_help.enter_contextual_help_overlay);
    static_assert(reloaded_help.poll_direct_input);
    static_assert(reloaded_help.present);
    static_assert(reloaded_help.return_success);

    // Without the surface-reload reset, active contextual help dispatches
    // UpdateContextualHelpMode instead of the 68-state main flow.
    constexpr auto help = run_active_frame_step({
        1, 5, true,
        1235, 1234,
        false, true, 0,
        0, 0,
    });
    static_assert(
        help.dispatch == ActivityDispatch::ContextualHelp);

    // Cooperative-level temporary exclusive-mode failures sleep 10 ms and
    // return without presenting.
    constexpr auto temporary_coop = run_active_frame_step({
        1, 5, true,
        1235, 1234,
        false, false, 0,
        kDdErrNoExclusiveMode, 0,
    });
    static_assert(temporary_coop.cooperative.sleep_10ms_and_return);
    static_assert(!temporary_coop.present);
    static_assert(temporary_coop.return_success);

    // Wrong mode recreates the display with the current global mode and exits
    // this frame before PresentDisplay.
    constexpr auto wrong_mode = run_active_frame_step({
        1, 5, true,
        1235, 1234,
        false, false, 0,
        kDdErrWrongMode, 0,
    });
    static_assert(wrong_mode.cooperative.recreate_display_and_return);
    static_assert(!wrong_mode.present);
    static_assert(wrong_mode.return_success);

    // Surface-lost from PresentDisplay invokes RestoreAllSurfaces but still
    // returns success from RunActiveGameFrame.
    constexpr auto lost_present = run_active_frame_step({
        1, 5, true,
        1235, 1234,
        false, false, 0,
        0, kDdErrSurfaceLost,
    });
    static_assert(lost_present.present);
    static_assert(lost_present.present_result.restore_all_surfaces);
    static_assert(lost_present.return_success);
    static_assert(!lost_present.return_error);

    // CD validation abort has highest precedence in the active-frame body.
    constexpr auto removed_cd = run_active_frame_step({
        100, 3, true,
        1235, 1234,
        true, true, 2,
        0, 0,
    });
    static_assert(removed_cd.abort_for_removed_cd);
    static_assert(!removed_cd.duplicate_timestamp_return);
    static_assert(!removed_cd.reap_finished_sounds);
}
