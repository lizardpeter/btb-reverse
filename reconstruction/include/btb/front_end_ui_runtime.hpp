#pragma once

#include "btb/front_end_ui_data.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::front_end {

inline constexpr std::int32_t kOptionsTargetCode = -99;
inline constexpr std::int32_t kFireworksPregameTargetCode = 0x22;

struct GenericUiRuntimeState {
    // 0 means no currently hovered area; retail stores one-based index here.
    std::int32_t selected_area_one_based{};

    // Set on release/click and consumed only after managed click audio becomes
    // idle. The zero-based area index is retained in a separate retail global.
    bool transition_pending{};
    std::int32_t pending_area_index{-1};

    // Activity Select lock latch at 0x0051C304.
    bool fireworks_finale_locked{};
};

struct ClickArmStep {
    bool armed{};
    bool stop_all_managed_sounds{};
    std::optional<std::int32_t> click_sound_id{};
    std::int32_t click_sound_priority{50};
    std::int32_t click_sound_arbitration_class{2};
};

// Exact click-release side effects in UpdateGenericUIScreenInteraction.
// A click remembers the area and sets the deferred-transition flag. If the
// record has a click sound, retail stops all managed sounds and starts that
// sound at priority 50 / arbitration class 2. A locked Fireworks target then
// stops managed audio again so the Progress interception is not delayed.
[[nodiscard]] constexpr ClickArmStep arm_generic_ui_click(
    GenericUiRuntimeState& state,
    const ReplacementRecord& record,
    std::int32_t zero_based_area_index) noexcept {

    ClickArmStep step;
    state.transition_pending = true;
    state.pending_area_index = zero_based_area_index;
    step.armed = true;

    if (record.click_sound_id != -1) {
        step.stop_all_managed_sounds = true;
        step.click_sound_id = record.click_sound_id;
    }

    if (state.fireworks_finale_locked &&
        record.target_state_or_action == kFireworksPregameTargetCode) {
        step.stop_all_managed_sounds = true;
    }

    return step;
}

enum class DeferredRoute {
    None,
    WaitForManagedSound,
    OpenOptions,
    OpenProgressScreen,
    ApplyTargetCode,
};

struct DeferredTransitionStep {
    DeferredRoute route{DeferredRoute::None};
    bool consumed_pending{};
    bool stop_all_managed_sounds{};
    bool set_options_flag{};
    bool set_progress_screen_flag{};
    bool set_progress_first_entry_flag{};
    bool clear_fireworks_finale_lock{};
    std::optional<std::int32_t> target_code{};
};

// Exact deferred branch at the top of UpdateGenericUIScreenInteraction.
// The selected record is supplied by the caller from pending_area_index.
[[nodiscard]] constexpr DeferredTransitionStep resolve_generic_ui_click(
    GenericUiRuntimeState& state,
    const ReplacementRecord& record,
    bool any_managed_sound_playing) noexcept {

    DeferredTransitionStep step;
    if (!state.transition_pending) {
        return step;
    }

    if (any_managed_sound_playing) {
        step.route = DeferredRoute::WaitForManagedSound;
        return step;
    }

    state.transition_pending = false;
    step.consumed_pending = true;

    if (record.target_state_or_action == kOptionsTargetCode) {
        step.route = DeferredRoute::OpenOptions;
        step.set_options_flag = true;
        return step;
    }

    if (state.fireworks_finale_locked) {
        if (record.target_state_or_action ==
            kFireworksPregameTargetCode) {
            step.route = DeferredRoute::OpenProgressScreen;
            step.stop_all_managed_sounds = true;
            step.set_progress_screen_flag = true;
            step.set_progress_first_entry_flag = true;
            return step;
        }

        state.fireworks_finale_locked = false;
        step.clear_fireworks_finale_lock = true;
    }

    step.route = DeferredRoute::ApplyTargetCode;
    step.target_code = record.target_state_or_action;
    return step;
}

inline constexpr std::int32_t kLockedFireworksHoverSoundId = 65; // ASH_WEN_09
inline constexpr std::int32_t kGenericUiHoverSoundPriority = 50;
inline constexpr std::int32_t kGenericUiHoverSoundArbitrationClass = 2;
inline constexpr std::int32_t kGenericUiHoverAnimationDelay = 8;

struct ReplacementRuntimeState {
    // LoadGenericUIScreenResources seeds this from rand()%sound_count when
    // there are multiple available hover sounds, otherwise zero.
    std::int32_t hover_sound_index{};
    std::int32_t hover_animation_delay{kGenericUiHoverAnimationDelay};
    std::int32_t hover_animation_frame{};
    std::int32_t secondary_animation_frame{};
};

[[nodiscard]] constexpr ReplacementRuntimeState
initialize_replacement_runtime(
    const ReplacementRecord& record,
    std::int32_t random_value) noexcept {

    ReplacementRuntimeState state;
    const auto count = available_hover_sound_count(record);
    if (count > 1) {
        auto selected = random_value % count;
        if (selected < 0) {
            selected += count;
        }
        state.hover_sound_index = selected;
    }
    return state;
}

struct HoverEnterStep {
    bool selected{};
    bool locked_fireworks_special{};
    std::int32_t selected_area_one_based{};
    std::int32_t hover_animation_delay{kGenericUiHoverAnimationDelay};
    std::int32_t hover_animation_frame{};
    std::optional<std::int32_t> hover_sound_id{};
    std::int32_t hover_sound_priority{kGenericUiHoverSoundPriority};
    std::int32_t hover_sound_arbitration_class{
        kGenericUiHoverSoundArbitrationClass};
};

// Exact new-hover branch in UpdateGenericUIScreenInteraction.
//
// Retail increments/wraps the per-record hover-sound index *before* playing it;
// the loader's random value is therefore only a seed. Activity Select area 2
// (the third tile, Fireworks) has a separate locked-finalé hover path: sound 65
// is played and the normal record hover sound is skipped.
[[nodiscard]] constexpr HoverEnterStep enter_generic_ui_area(
    GenericUiRuntimeState& state,
    const ReplacementRecord& record,
    ReplacementRuntimeState& record_runtime,
    std::int32_t zero_based_area_index,
    bool activity_select_screen = false) noexcept {

    HoverEnterStep step;
    state.selected_area_one_based = zero_based_area_index + 1;

    record_runtime.hover_animation_delay = kGenericUiHoverAnimationDelay;
    record_runtime.hover_animation_frame = 0;

    step.selected = true;
    step.selected_area_one_based = state.selected_area_one_based;

    if (activity_select_screen &&
        zero_based_area_index == 2 &&
        state.fireworks_finale_locked) {
        step.locked_fireworks_special = true;
        step.hover_sound_id = kLockedFireworksHoverSoundId;
        return step;
    }

    const auto count = available_hover_sound_count(record);
    if (count <= 0) {
        return step;
    }

    ++record_runtime.hover_sound_index;
    if (record_runtime.hover_sound_index >= count ||
        record_runtime.hover_sound_index < 0) {
        record_runtime.hover_sound_index = 0;
    }

    const auto sound_id = selected_hover_sound_id(
        record, record_runtime.hover_sound_index);
    if (sound_id != -1) {
        step.hover_sound_id = sound_id;
    }
    return step;
}

struct HoverAnimationStep {
    bool frame_advanced{};
    std::int32_t frame{};
    std::int32_t delay{};
};

// While an area remains selected, its hover animation countdown decrements.
// At zero/negative, retail restores delay 8, increments the frame and wraps by
// the record's loaded hover_frame_count.
[[nodiscard]] constexpr HoverAnimationStep update_hover_animation(
    const ReplacementRecord& record,
    ReplacementRuntimeState& runtime) noexcept {

    HoverAnimationStep step;
    --runtime.hover_animation_delay;

    if (runtime.hover_animation_delay <= 0) {
        runtime.hover_animation_delay = kGenericUiHoverAnimationDelay;
        if (record.hover_frame_count > 0) {
            ++runtime.hover_animation_frame;
            if (runtime.hover_animation_frame >=
                record.hover_frame_count) {
                runtime.hover_animation_frame = 0;
            }
            step.frame_advanced = true;
        }
    }

    step.frame = runtime.hover_animation_frame;
    step.delay = runtime.hover_animation_delay;
    return step;
}

[[nodiscard]] constexpr bool leave_generic_ui_area(
    GenericUiRuntimeState& state) noexcept {
    if (state.selected_area_one_based == 0) {
        return false;
    }
    state.selected_area_one_based = 0;
    return true;
}

} // namespace btb::front_end
