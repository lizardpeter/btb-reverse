#pragma once

#include "btb/application_state.hpp"

#include <cstdint>
#include <string_view>

namespace btb::window {

inline constexpr std::string_view kWindowClassName = "WINNAME";
inline constexpr std::string_view kWindowTitle = "Bob the Builder";

inline constexpr std::uint32_t kWindowClassStyle = 0x0003;
inline constexpr std::uint16_t kMainIconResource = 101;
inline constexpr std::uint16_t kAcceleratorResource = 103;
inline constexpr std::uint32_t kArrowCursorResource = 0x7F00;
inline constexpr std::uint32_t kMainWindowStyle = 0x00CE0000;
inline constexpr std::uint32_t kUseDefaultPosition = 0x80000000U;

inline constexpr std::uint32_t kWmDestroy = 0x0002;
inline constexpr std::uint32_t kWmMove = 0x0003;
inline constexpr std::uint32_t kWmSize = 0x0005;
inline constexpr std::uint32_t kWmSetCursor = 0x0020;
inline constexpr std::uint32_t kWmGetMinMaxInfo = 0x0024;
inline constexpr std::uint32_t kWmCommand = 0x0111;
inline constexpr std::uint32_t kWmSysCommand = 0x0112;
inline constexpr std::uint32_t kWmSysKeyDown = 0x0104;

inline constexpr std::uint32_t kSizeMinimized = 1;
inline constexpr std::uint32_t kSizeMaxHide = 4;

inline constexpr std::uint16_t kCommandBeginExitCredits = 1001;
inline constexpr std::uint16_t kCommandToggleDisplayMode = 1002;
inline constexpr std::uint16_t kCommandOptionsOverlay = 0x9C41;

struct CreateWindowPlan {
    bool find_existing_window_first{true};
    std::string_view class_name{kWindowClassName};
    std::string_view title{kWindowTitle};
    std::uint32_t class_size{0x30};
    std::uint32_t class_style{kWindowClassStyle};
    std::uint16_t icon_resource{kMainIconResource};
    std::uint32_t cursor_resource{kArrowCursorResource};
    std::uint16_t accelerator_resource{kAcceleratorResource};
    std::uint32_t window_style{kMainWindowStyle};
    std::uint32_t x{kUseDefaultPosition};
    std::uint32_t y{kUseDefaultPosition};
    std::int32_t client_width{640};
    std::int32_t client_height{480};
    bool show_window{true};
    bool update_window{true};
};

inline constexpr CreateWindowPlan kCreateWindowPlan{};

// Retail manually derives the outer CreateWindow dimensions from non-client
// metrics rather than calling AdjustWindowRect in this startup helper.
[[nodiscard]] constexpr std::int32_t outer_window_width(
    std::int32_t size_frame_x) noexcept {
    return 640 + size_frame_x * 2;
}

[[nodiscard]] constexpr std::int32_t outer_window_height(
    std::int32_t size_frame_y,
    std::int32_t menu_height,
    std::int32_t caption_height) noexcept {
    return 480 +
        size_frame_y * 2 +
        menu_height +
        caption_height;
}

enum class WindowAction {
    DefWindowProc,
    ReturnZero,
    ReturnOne,
    RefreshDestinationRect,
    RefreshAcquireState,
    SetActiveFrameAndRefreshRect,
    HideCursor,
    SetShutdownPhase3,
    BeginExitCredits,
    ToggleDisplayMode,
    OpenOptionsOverlay,
    SuppressSystemCommand,
    RefreshTimeBaseline,
};

struct WindowMessageInput {
    std::uint32_t message{};
    std::uint32_t wparam{};
    std::uint32_t lparam{};
    application::DisplayMode display_mode{
        application::DisplayMode::Fullscreen};

    bool contextual_help_active{};
    bool whole_game_quit_active{};
    bool options_active{};
};

struct WindowMessageStep {
    WindowAction action{WindowAction::DefWindowProc};
    bool consume_message{};
    std::int32_t return_value{};
    bool set_active_frame{};
    bool active_frame_value{};
    bool refresh_destination_rect{};
    bool update_input_acquire_state{};
    bool hide_win32_cursor{};
    bool begin_exit_credits{};
    bool force_final_teardown{};
    bool toggle_display_mode{};
    bool open_options_overlay{};
    bool refresh_time_baseline{};
};

// Core message decisions from 0x004020D0 MainWindowProc. Unmodeled messages
// fall through to DefWindowProcA.
[[nodiscard]] constexpr WindowMessageStep main_window_message_step(
    const WindowMessageInput& input) noexcept {

    WindowMessageStep out;

    switch (input.message) {
    case kWmDestroy:
        out.action = WindowAction::SetShutdownPhase3;
        out.force_final_teardown = true;
        return out;

    case kWmMove:
        out.action = WindowAction::RefreshDestinationRect;
        out.consume_message = true;
        out.return_value = 0;
        out.refresh_destination_rect = true;
        return out;

    case kWmSize:
        out.action = WindowAction::SetActiveFrameAndRefreshRect;
        out.set_active_frame = true;
        out.active_frame_value =
            input.wparam != kSizeMinimized &&
            input.wparam != kSizeMaxHide;
        out.refresh_destination_rect = true;
        return out;

    case kWmSetCursor:
        if (input.display_mode == application::DisplayMode::Fullscreen) {
            out.action = WindowAction::HideCursor;
            out.consume_message = true;
            out.return_value = 1;
            out.hide_win32_cursor = true;
        }
        return out;

    case kWmSysKeyDown:
        // Retail explicitly returns zero for WM_SYSKEYDOWN before the generic
        // DefWindowProc path; fullscreen system combos are additionally
        // filtered by the WH_KEYBOARD_LL hook.
        out.action = WindowAction::ReturnZero;
        out.consume_message = true;
        out.return_value = 0;
        return out;

    case kWmCommand: {
        const auto command =
            static_cast<std::uint16_t>(input.wparam & 0xFFFFU);

        if (command == kCommandBeginExitCredits) {
            out.action = WindowAction::BeginExitCredits;
            out.consume_message = true;
            out.begin_exit_credits = true;
            return out;
        }

        if (command == kCommandToggleDisplayMode) {
            out.action = WindowAction::ToggleDisplayMode;
            out.consume_message = true;
            out.toggle_display_mode = true;
            return out;
        }

        if (command == kCommandOptionsOverlay &&
            !input.contextual_help_active &&
            !input.whole_game_quit_active &&
            !input.options_active) {
            out.action = WindowAction::OpenOptionsOverlay;
            out.open_options_overlay = true;
            return out;
        }
        break;
    }

    case 0x0212:
    case 0x0232:
        out.action = WindowAction::RefreshTimeBaseline;
        out.refresh_time_baseline = true;
        return out;

    default:
        break;
    }

    return out;
}

// WM_SIZE's global-active flag exactly matches the already recovered helper.
[[nodiscard]] constexpr bool active_frame_from_wm_size(
    std::uint32_t wparam) noexcept {
    return application::frame_active_for_size_state(wparam);
}

struct DisplayToggleStep {
    application::DisplayMode old_mode{};
    application::DisplayMode new_mode{};
    bool show_cursor_before_leaving_windowed{};
    bool recreate_display{true};
    bool destroy_failed_display_and_show_message_box_on_failure{true};
};

[[nodiscard]] constexpr DisplayToggleStep toggle_display_mode(
    application::DisplayMode current) noexcept {

    const auto next =
        current == application::DisplayMode::Fullscreen
            ? application::DisplayMode::Windowed
            : application::DisplayMode::Fullscreen;

    return {
        current,
        next,
        current == application::DisplayMode::Windowed,
        true,
        true,
    };
}

} // namespace btb::window
