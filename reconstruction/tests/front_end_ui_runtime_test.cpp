#include "btb/front_end_ui_runtime.hpp"

#include <cassert>

using namespace btb::front_end;

int main() {
    ReplacementRecord activity;
    activity.hover_sound_ids = {7,39,-1};
    activity.target_state_or_action = 0x0C;
    activity.click_sound_id = 451;

    GenericUiRuntimeState state;
    auto hover = enter_generic_ui_area(state, activity, 3, 1);
    assert(hover.selected);
    assert(hover.selected_area_one_based == 4);
    assert(hover.hover_animation_delay == 8);
    assert(hover.hover_animation_frame == 0);
    assert(hover.hover_sound_id && *hover.hover_sound_id == 39);
    assert(state.selected_area_one_based == 4);

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
    hover = enter_generic_ui_area(state, malformed, 0, 1);
    assert(hover.selected);
    assert(!hover.hover_sound_id);
}
