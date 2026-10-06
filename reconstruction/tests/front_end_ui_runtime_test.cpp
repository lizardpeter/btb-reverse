#include "btb/front_end_ui_runtime.hpp"

#include <cassert>

using namespace btb::front_end;

int main() {
    ReplacementRecord activity;
    activity.hover_sound_ids = {7,39,-1};
    activity.target_state_or_action = 0x0C;
    activity.click_sound_id = 451;

    GenericUiRuntimeState state;
    auto activity_runtime = initialize_replacement_runtime(activity, 0);
    // Two sounds and random seed 0 initializes index 0; retail increments to
    // index 1 on entry, so the first hover plays ID 39.
    auto hover = enter_generic_ui_area(
        state, activity, activity_runtime, 3);
    assert(hover.selected);
    assert(hover.selected_area_one_based == 4);
    assert(hover.hover_animation_delay == 8);
    assert(hover.hover_animation_frame == 0);
    assert(hover.hover_sound_id && *hover.hover_sound_id == 39);
    assert(!hover.locked_fireworks_special);
    assert(activity_runtime.hover_sound_index == 1);
    assert(activity_runtime.hover_animation_delay == 8);
    assert(activity_runtime.hover_animation_frame == 0);
    assert(state.selected_area_one_based == 4);

    // Re-entering the same record after leaving cycles the sound index and
    // wraps it back to ID 7.
    assert(leave_generic_ui_area(state));
    hover = enter_generic_ui_area(
        state, activity, activity_runtime, 3);
    assert(hover.hover_sound_id && *hover.hover_sound_id == 7);
    assert(activity_runtime.hover_sound_index == 0);

    assert(leave_generic_ui_area(state));
    assert(state.selected_area_one_based == 0);
    assert(!leave_generic_ui_area(state));

    auto click = arm_generic_ui_click(state, activity, 3);
    assert(click.armed);
    assert(click.stop_all_managed_sounds);
    assert(click.click_sound_id && *click.click_sound_id == 451);
    assert(click.click_sound_priority == 50);
    assert(click.click_sound_arbitration_class == 2);
    assert(state.transition_pending);
    assert(state.pending_area_index == 3);

    // Click audio gates the actual transition.
    auto deferred = resolve_generic_ui_click(state, activity, true);
    assert(deferred.route == DeferredRoute::WaitForManagedSound);
    assert(!deferred.consumed_pending);
    assert(state.transition_pending);

    deferred = resolve_generic_ui_click(state, activity, false);
    assert(deferred.route == DeferredRoute::ApplyTargetCode);
    assert(deferred.consumed_pending);
    assert(deferred.target_code && *deferred.target_code == 0x0C);
    assert(!state.transition_pending);

    // Target -99 is the special Options intercept, not an outer-state write.
    ReplacementRecord options;
    options.target_state_or_action = kOptionsTargetCode;

    state.transition_pending = true;
    deferred = resolve_generic_ui_click(state, options, false);
    assert(deferred.route == DeferredRoute::OpenOptions);
    assert(deferred.set_options_flag);
    assert(!deferred.target_code);

    // Locked Fireworks target 0x22 is intercepted into Progress. The click
    // sound is stopped immediately, so the deferred branch can run without
    // waiting for it.
    ReplacementRecord fireworks;
    fireworks.target_state_or_action = 0x22;
    fireworks.click_sound_id = 486;

    state = {};
    state.fireworks_finale_locked = true;

    // Activity Select's third tile has a special hover path while the finale
    // is locked: ASH_WEN_09 / ID 65 replaces the normal record hover sound.
    fireworks.hover_sound_ids = {10,20,-1};
    auto fireworks_runtime =
        initialize_replacement_runtime(fireworks, 0);
    hover = enter_generic_ui_area(
        state, fireworks, fireworks_runtime, 2, true);
    assert(hover.selected);
    assert(hover.locked_fireworks_special);
    assert(hover.hover_sound_id &&
           *hover.hover_sound_id == kLockedFireworksHoverSoundId);
    assert(hover.hover_sound_priority == 50);
    assert(hover.hover_sound_arbitration_class == 2);
    // Special path skips normal sound-index advancement.
    assert(fireworks_runtime.hover_sound_index == 0);
    assert(leave_generic_ui_area(state));

    click = arm_generic_ui_click(state, fireworks, 2);
    assert(click.armed);
    assert(click.stop_all_managed_sounds);
    assert(click.click_sound_id && *click.click_sound_id == 486);

    deferred = resolve_generic_ui_click(state, fireworks, false);
    assert(deferred.route == DeferredRoute::OpenProgressScreen);
    assert(deferred.consumed_pending);
    assert(deferred.stop_all_managed_sounds);
    assert(deferred.set_progress_screen_flag);
    assert(deferred.set_progress_first_entry_flag);
    assert(!deferred.target_code);
    // The lock is not cleared on this special interception.
    assert(state.fireworks_finale_locked);

    // If the lock latch is nonzero but another target is chosen, retail clears
    // the latch and applies the ordinary target code.
    ReplacementRecord dino;
    dino.target_state_or_action = 0x10;

    state = {};
    state.fireworks_finale_locked = true;
    state.transition_pending = true;
    deferred = resolve_generic_ui_click(state, dino, false);
    assert(deferred.route == DeferredRoute::ApplyTargetCode);
    assert(deferred.clear_fireworks_finale_lock);
    assert(deferred.target_code && *deferred.target_code == 0x10);
    assert(!state.fireworks_finale_locked);

    // No click sound does not request StopAllManagedSounds.
    ReplacementRecord back;
    back.target_state_or_action = -1;
    back.click_sound_id = -1;

    state = {};
    click = arm_generic_ui_click(state, back, 9);
    assert(click.armed);
    assert(!click.stop_all_managed_sounds);
    assert(!click.click_sound_id);

    deferred = resolve_generic_ui_click(state, back, false);
    assert(deferred.route == DeferredRoute::ApplyTargetCode);
    assert(deferred.target_code && *deferred.target_code == -1);

    // Hover table holes are preserved exactly; a chosen direct slot containing
    // -1 results in no hover sound rather than compacting to the later ID.
    ReplacementRecord malformed;
    malformed.hover_sound_ids = {10,-1,30};
    state = {};
    auto malformed_runtime =
        initialize_replacement_runtime(malformed, 0);
    // Count is two; seeded index 0 increments to direct slot 1, which is -1.
    hover = enter_generic_ui_area(
        state, malformed, malformed_runtime, 0);
    assert(hover.selected);
    assert(!hover.hover_sound_id);
    assert(malformed_runtime.hover_sound_index == 1);

    // Hover animation advances only when the 8-tick countdown expires.
    ReplacementRecord animated;
    animated.hover_frame_count = 3;
    ReplacementRuntimeState animation_runtime;
    for (int i = 0; i < 7; ++i) {
        const auto animation =
            update_hover_animation(animated, animation_runtime);
        assert(!animation.frame_advanced);
        assert(animation.frame == 0);
    }
    auto animation =
        update_hover_animation(animated, animation_runtime);
    assert(animation.frame_advanced);
    assert(animation.frame == 1);
    assert(animation.delay == 8);

    animation_runtime.hover_animation_frame = 2;
    animation_runtime.hover_animation_delay = 1;
    animation = update_hover_animation(animated, animation_runtime);
    assert(animation.frame_advanced);
    assert(animation.frame == 0);
    assert(animation.delay == 8);

    // Loader seed uses rand()%count only when multiple hover sounds exist.
    auto seeded = initialize_replacement_runtime(activity, 5);
    assert(seeded.hover_sound_index == 1);
    ReplacementRecord single;
    single.hover_sound_ids = {42,-1,-1};
    seeded = initialize_replacement_runtime(single, 999);
    assert(seeded.hover_sound_index == 0);
}
