#include "btb/window_runtime.hpp"

#include <cassert>

using namespace btb;
using namespace btb::window;

int main() {
    static_assert(kWindowClassName == "WINNAME");
    static_assert(kWindowTitle == "Bob the Builder");
    static_assert(kWindowClassStyle == 3);
    static_assert(kMainIconResource == 101);
    static_assert(kAcceleratorResource == 103);
    static_assert(kArrowCursorResource == 0x7F00);
    static_assert(kMainWindowStyle == 0x00CE0000);
    static_assert(kCreateWindowPlan.class_size == 0x30);
    static_assert(kCreateWindowPlan.client_width == 640);
    static_assert(kCreateWindowPlan.client_height == 480);

    static_assert(outer_window_width(4) == 648);
    static_assert(outer_window_height(4,19,23) == 530);

    constexpr auto destroy = main_window_message_step({
        kWmDestroy,0,0,application::DisplayMode::Fullscreen});
    static_assert(destroy.force_final_teardown);
    static_assert(destroy.action == WindowAction::SetShutdownPhase3);

    constexpr auto moved = main_window_message_step({
        kWmMove,0,0,application::DisplayMode::Windowed});
    static_assert(moved.consume_message);
    static_assert(moved.return_value == 0);
    static_assert(moved.refresh_destination_rect);

    constexpr auto minimized = main_window_message_step({
        kWmSize,kSizeMinimized,0,application::DisplayMode::Windowed});
    static_assert(minimized.set_active_frame);
    static_assert(!minimized.active_frame_value);
    static_assert(minimized.refresh_destination_rect);

    constexpr auto maxhide = main_window_message_step({
        kWmSize,kSizeMaxHide,0,application::DisplayMode::Windowed});
    static_assert(!maxhide.active_frame_value);

    constexpr auto restored = main_window_message_step({
        kWmSize,0,0,application::DisplayMode::Windowed});
    static_assert(restored.active_frame_value);

    constexpr auto fullscreen_cursor = main_window_message_step({
        kWmSetCursor,0,0,application::DisplayMode::Fullscreen});
    static_assert(fullscreen_cursor.consume_message);
    static_assert(fullscreen_cursor.return_value == 1);
    static_assert(fullscreen_cursor.hide_win32_cursor);

    constexpr auto windowed_cursor = main_window_message_step({
        kWmSetCursor,0,0,application::DisplayMode::Windowed});
    static_assert(!windowed_cursor.consume_message);
    static_assert(!windowed_cursor.hide_win32_cursor);

    constexpr auto syskey = main_window_message_step({
        kWmSysKeyDown,0,0,application::DisplayMode::Fullscreen});
    static_assert(syskey.consume_message);
    static_assert(syskey.return_value == 0);

    constexpr auto exit_command = main_window_message_step({
        kWmCommand,kCommandBeginExitCredits,0,
        application::DisplayMode::Fullscreen});
    static_assert(exit_command.begin_exit_credits);
    static_assert(exit_command.consume_message);

    constexpr auto toggle_command = main_window_message_step({
        kWmCommand,kCommandToggleDisplayMode,0,
        application::DisplayMode::Fullscreen});
    static_assert(toggle_command.toggle_display_mode);
    static_assert(toggle_command.consume_message);

    constexpr auto options_command = main_window_message_step({
        kWmCommand,kCommandOptionsOverlay,0,
        application::DisplayMode::Fullscreen,
        false,false,false});
    static_assert(options_command.open_options_overlay);

    constexpr auto options_blocked_by_help = main_window_message_step({
        kWmCommand,kCommandOptionsOverlay,0,
        application::DisplayMode::Fullscreen,
        true,false,false});
    static_assert(!options_blocked_by_help.open_options_overlay);

    constexpr auto options_blocked_by_quit = main_window_message_step({
        kWmCommand,kCommandOptionsOverlay,0,
        application::DisplayMode::Fullscreen,
        false,true,false});
    static_assert(!options_blocked_by_quit.open_options_overlay);

    constexpr auto timing_message = main_window_message_step({
        0x0212,0,0,application::DisplayMode::Fullscreen});
    static_assert(timing_message.refresh_time_baseline);

    constexpr auto default_message = main_window_message_step({
        0x7777,0,0,application::DisplayMode::Fullscreen});
    static_assert(default_message.action == WindowAction::DefWindowProc);
    static_assert(!default_message.consume_message);

    static_assert(!active_frame_from_wm_size(kSizeMinimized));
    static_assert(!active_frame_from_wm_size(kSizeMaxHide));
    static_assert(active_frame_from_wm_size(0));

    constexpr auto fullscreen_to_windowed =
        toggle_display_mode(application::DisplayMode::Fullscreen);
    static_assert(
        fullscreen_to_windowed.new_mode ==
        application::DisplayMode::Windowed);
    static_assert(!fullscreen_to_windowed.show_cursor_before_leaving_windowed);
    static_assert(fullscreen_to_windowed.recreate_display);

    constexpr auto windowed_to_fullscreen =
        toggle_display_mode(application::DisplayMode::Windowed);
    static_assert(
        windowed_to_fullscreen.new_mode ==
        application::DisplayMode::Fullscreen);
    static_assert(windowed_to_fullscreen.show_cursor_before_leaving_windowed);
    static_assert(
        windowed_to_fullscreen
            .destroy_failed_display_and_show_message_box_on_failure);
}
