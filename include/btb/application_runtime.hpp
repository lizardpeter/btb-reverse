#pragma once

#include "btb/application_state.hpp"

#include <cstdint>

namespace btb::application {

inline constexpr std::int32_t kCdValidationThreshold = 100;
inline constexpr std::uint32_t kExpectedCdDriveType = 5;
inline constexpr char kExpectedCdVolumeLabel[] = "BTB-BBP";

inline constexpr std::int32_t kDdErrExclusiveModeAlreadySet =
    static_cast<std::int32_t>(0x887600E1U);
inline constexpr std::int32_t kDdErrNoExclusiveMode =
    static_cast<std::int32_t>(0x88760245U);
inline constexpr std::int32_t kDdErrWrongMode =
    static_cast<std::int32_t>(0x8876024BU);
inline constexpr std::int32_t kDdErrSurfaceLost =
    static_cast<std::int32_t>(0x887601C2U);

struct CdValidationStep {
    std::int32_t next_counter{};
    bool query_drive_type{};
    bool query_volume_information{};
    bool abort_for_removed_cd{};
};

// Exact leading cadence in 0x00402440. Retail increments the counter first and
// validates only when the new value is >100, then resets it to zero.
[[nodiscard]] constexpr CdValidationStep advance_cd_validation(
    std::int32_t current_counter,
    std::uint32_t drive_type = kExpectedCdDriveType,
    bool volume_label_matches = true) noexcept {

    const auto incremented = current_counter + 1;
    if (incremented <= kCdValidationThreshold) {
        return {incremented,false,false,false};
    }

    if (drive_type != kExpectedCdDriveType) {
        return {0,true,false,true};
    }

    return {
        0,
        true,
        true,
        !volume_label_matches,
    };
}

struct SurfaceReloadStep {
    bool reload_registered_bitmaps{};
    bool clear_reload_flag{};
    bool clear_help_mode{};
    bool clear_primary_input_pulse{};
    bool stop_all_managed_sounds{};
    bool restore_default_cursor{};
    bool set_shared_delay_50{};
};

[[nodiscard]] constexpr SurfaceReloadStep surface_reload_step(
    bool reload_flag,
    bool contextual_help_active) noexcept {

    if (!reload_flag) {
        return {};
    }

    return {
        true,
        true,
        contextual_help_active,
        contextual_help_active,
        contextual_help_active,
        contextual_help_active,
        contextual_help_active,
    };
}

enum class ActivityDispatch {
    MainGameFlow,
    ContextualHelp,
};

struct CooperativeLevelStep {
    bool continue_to_present{};
    bool sleep_10ms_and_return{};
    bool recreate_display_and_return{};
    bool return_error{};
    std::int32_t error{};
};

[[nodiscard]] constexpr CooperativeLevelStep cooperative_level_step(
    std::int32_t hresult) noexcept {

    if (hresult >= 0) {
        return {true,false,false,false,0};
    }

    if (hresult == kDdErrExclusiveModeAlreadySet ||
        hresult == kDdErrNoExclusiveMode) {
        return {false,true,false,false,0};
    }

    if (hresult == kDdErrWrongMode) {
        return {false,false,true,false,0};
    }

    return {false,false,false,true,hresult};
}

struct PresentFrameStep {
    bool restore_all_surfaces{};
    bool return_error{};
    std::int32_t error{};
};

[[nodiscard]] constexpr PresentFrameStep present_frame_step(
    std::int32_t present_hresult) noexcept {

    if (present_hresult >= 0) {
        return {};
    }

    if (present_hresult == kDdErrSurfaceLost) {
        return {true,false,0};
    }

    return {false,true,present_hresult};
}

struct ActiveFrameInput {
    std::int32_t cd_validation_counter{};
    std::uint32_t cd_drive_type{kExpectedCdDriveType};
    bool cd_volume_label_matches{true};

    std::uint32_t current_time_ms{};
    std::uint32_t previous_time_ms{};

    bool bitmap_reload_requested{};
    bool contextual_help_active{};
    std::int32_t contextual_help_activation_phase{};

    std::int32_t cooperative_level_hresult{};
    std::int32_t present_hresult{};
};

struct ActiveFrameStep {
    CdValidationStep cd{};
    bool abort_for_removed_cd{};

    bool duplicate_timestamp_return{};
    bool update_previous_time{};
    std::uint32_t next_previous_time{};

    SurfaceReloadStep reload{};

    bool reap_finished_sounds{};
    bool clear_shared_frame_flag{};
    ActivityDispatch dispatch{ActivityDispatch::MainGameFlow};
    bool enter_contextual_help_overlay{};
    bool poll_direct_input{};

    CooperativeLevelStep cooperative{};
    bool present{};
    PresentFrameStep present_result{};

    bool return_success{};
    bool return_error{};
    std::int32_t error{};
};

// Pure control-flow reconstruction of 0x00402440 RunActiveGameFrame. External
// Win32/DirectX calls are represented as input results; this preserves exact
// retail ordering and early-return precedence without binding the C++26 model
// to Windows.
[[nodiscard]] constexpr ActiveFrameStep run_active_frame_step(
    const ActiveFrameInput& input) noexcept {

    ActiveFrameStep out;

    out.cd = advance_cd_validation(
        input.cd_validation_counter,
        input.cd_drive_type,
        input.cd_volume_label_matches);

    if (out.cd.abort_for_removed_cd) {
        out.abort_for_removed_cd = true;
        return out;
    }

    if (input.current_time_ms - input.previous_time_ms == 0U) {
        out.duplicate_timestamp_return = true;
        out.return_success = true;
        return out;
    }

    out.reload = surface_reload_step(
        input.bitmap_reload_requested,
        input.contextual_help_active);

    const bool help_after_reload =
        input.contextual_help_active && !out.reload.clear_help_mode;

    out.update_previous_time = true;
    out.next_previous_time = input.current_time_ms;
    out.reap_finished_sounds = true;
    out.clear_shared_frame_flag = true;
    out.dispatch = help_after_reload
        ? ActivityDispatch::ContextualHelp
        : ActivityDispatch::MainGameFlow;
    out.enter_contextual_help_overlay =
        input.contextual_help_activation_phase >= 2;
    out.poll_direct_input = true;

    out.cooperative =
        cooperative_level_step(input.cooperative_level_hresult);

    if (out.cooperative.sleep_10ms_and_return ||
        out.cooperative.recreate_display_and_return) {
        out.return_success = true;
        return out;
    }

    if (out.cooperative.return_error) {
        out.return_error = true;
        out.error = out.cooperative.error;
        return out;
    }

    out.present = true;
    out.present_result =
        present_frame_step(input.present_hresult);

    if (out.present_result.return_error) {
        out.return_error = true;
        out.error = out.present_result.error;
        return out;
    }

    out.return_success = true;
    return out;
}

} // namespace btb::application
