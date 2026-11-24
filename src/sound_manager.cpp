#include "btb/sound_manager.hpp"

namespace btb::sound {

ReapResult reap_finished_slots(
    RetailSoundManager32& manager,
    const std::array<bool,kManagedSlotCount>& slot_group_is_playing,
    bool primary_input_pulse,
    bool secondary_input_pulse) noexcept {

    ReapResult result;

    for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
        if (manager.slot_state[i] !=
            static_cast<std::int32_t>(SlotState::Active)) {
            continue;
        }

        if (!should_stop_active_slot(
                manager.input_interruptible[i] != 0,
                primary_input_pulse,
                secondary_input_pulse,
                slot_group_is_playing[i])) {
            continue;
        }

        stop_slot_metadata(manager, i);
        result.stopped_slots[result.stopped_count++] =
            static_cast<std::int32_t>(i);
    }

    return result;
}

ManagedPlayResult play_managed_sound(
    RetailSoundManager32& manager,
    ManagedPlayEnvironment& environment,
    std::int32_t sound_id,
    std::int32_t priority,
    std::int32_t arbitration_class) noexcept {

    ManagedPlayResult result;

    if (!manager.valid_sound_id(sound_id)) {
        result.status = 0;
        return result;
    }

    const auto sound_index = static_cast<std::size_t>(sound_id);

    if (manager.sound_enabled[sound_index] == 0) {
        result.disabled = true;
        result.status = 0;
        return result;
    }

    std::int32_t slot =
        static_cast<std::int32_t>(manager.slot_by_sound_id[sound_index]);

    if (slot >= 0 &&
        slot < static_cast<std::int32_t>(kManagedSlotCount) &&
        manager.slot_state[static_cast<std::size_t>(slot)] ==
            static_cast<std::int32_t>(SlotState::Active)) {
        result.status = 2;
        result.already_active = true;
        result.slot = slot;
        return result;
    }

    if (arbitration_class > 0) {
        for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
            if (manager.buffer_group_ptr32[i] == 0) {
                continue;
            }

            if (!environment.group_is_playing[i]) {
                continue;
            }

            const auto action = arbitration_action(
                manager.arbitration_class[i],
                arbitration_class,
                true);

            if (action == ArbitrationAction::RejectIncoming) {
                result.rejected_by_exclusive_blocker = true;
                result.status = 0;
                return result;
            }

            if (action == ArbitrationAction::StopExisting) {
                stop_slot_metadata(manager, i);
                environment.group_is_playing[i] = false;
                result.preempted_slots[result.preempted_count++] =
                    static_cast<std::int32_t>(i);
            }
        }
    }

    if (slot == -1) {
        const auto choice =
            select_slot_for_acquire(manager, environment.group_is_playing);

        if (choice.slot < 0) {
            // The retail caller proceeds to index slot -1 here. The source
            // reconstruction records the condition instead of reproducing
            // invalid memory access.
            result.would_index_negative_slot = true;
            result.status = 0;
            return result;
        }

        slot = choice.slot;
        result.slot = slot;

        if (choice.requires_release) {
            result.released_slot = slot;
            clear_released_slot_metadata(
                manager, static_cast<std::size_t>(slot));
            environment.group_exists[static_cast<std::size_t>(slot)] = false;
            environment.group_is_playing[static_cast<std::size_t>(slot)] = false;
        }

        install_acquired_slot_metadata(
            manager,
            static_cast<std::size_t>(slot),
            sound_id,
            priority,
            arbitration_class);

        result.load_attempted = true;

        if (environment.acquired_group_load_succeeds) {
            // Pointer value 1 is only a host-side sentinel for "non-null".
            manager.buffer_group_ptr32[static_cast<std::size_t>(slot)] = 1;
            environment.group_exists[static_cast<std::size_t>(slot)] = true;

            // 0x00402E10 calls CSound::Play(0,0) immediately after creation.
            ++result.low_level_play_calls;
            environment.group_is_playing[static_cast<std::size_t>(slot)] = true;

            manager.slot_state[static_cast<std::size_t>(slot)] =
                static_cast<std::int32_t>(SlotState::NewlyAcquired);
            result.load_succeeded = true;
        }
    }

    if (slot < 0 ||
        slot >= static_cast<std::int32_t>(kManagedSlotCount)) {
        result.would_index_negative_slot = true;
        result.status = 0;
        return result;
    }

    result.slot = slot;
    const auto index = static_cast<std::size_t>(slot);
    const auto state = static_cast<SlotState>(manager.slot_state[index]);

    if (state == SlotState::NewlyAcquired) {
        manager.slot_state[index] =
            static_cast<std::int32_t>(SlotState::Active);

        // PlayManagedSoundById starts the just-created CSound a second time.
        if (environment.group_exists[index]) {
            ++result.low_level_play_calls;
            environment.group_is_playing[index] = true;
        }

        result.status = 1;
        return result;
    }

    if (state == SlotState::Stopped) {
        if (environment.group_exists[index]) {
            ++result.low_level_play_calls;
            environment.group_is_playing[index] = true;
        }
        manager.slot_state[index] =
            static_cast<std::int32_t>(SlotState::Active);
        result.status = 1;
        return result;
    }

    // Retail returns 1 even if acquisition failed to create a group and the
    // slot remained state 0 after its mappings were installed.
    result.status = 1;
    result.accepted_even_though_load_failed =
        result.load_attempted && !result.load_succeeded;
    return result;
}

} // namespace btb::sound
