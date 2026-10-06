#include "btb/sound_manager.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

using namespace btb::sound;

int main() {
    static_assert(kManagedSlotCount == 80);
    static_assert(kSoundCatalogCount == 1100);
    static_assert(kFilenameRecordBytes == 20);
    static_assert(kRetailSoundManagerBytes == 0x7738);

    static_assert(sizeof(FilenameRecord) == 20);
    static_assert(sizeof(RetailSoundManager32) == 0x7738);

    static_assert(offsetof(RetailSoundManager32, buffer_group_ptr32) == 0x000);
    static_assert(offsetof(RetailSoundManager32, sound_id_by_slot) == 0x140);
    static_assert(offsetof(RetailSoundManager32, slot_by_sound_id) == 0x280);
    static_assert(offsetof(RetailSoundManager32, sound_enabled) == 0x6CC);
    static_assert(offsetof(RetailSoundManager32, priority) == 0xB18);
    static_assert(offsetof(RetailSoundManager32, slot_state) == 0xC58);
    static_assert(offsetof(RetailSoundManager32, input_interruptible) == 0xD98);
    static_assert(offsetof(RetailSoundManager32, arbitration_class) == 0xED8);
    static_assert(offsetof(RetailSoundManager32, filename_by_sound_id) == 0x1018);
    static_assert(offsetof(RetailSoundManager32, catalog_metadata) == 0x6608);

    static_assert(
        arbitration_action(0, 0) ==
        ArbitrationAction::ContinueScan);
    static_assert(
        arbitration_action(1, 0) ==
        ArbitrationAction::ContinueScan);
    static_assert(
        arbitration_action(1, 1) ==
        ArbitrationAction::RejectIncoming);
    static_assert(
        arbitration_action(1, 2) ==
        ArbitrationAction::RejectIncoming);
    static_assert(
        arbitration_action(1, 3) ==
        ArbitrationAction::RejectIncoming);
    static_assert(
        arbitration_action(2, 1) ==
        ArbitrationAction::StopExisting);
    static_assert(
        arbitration_action(2, 2) ==
        ArbitrationAction::StopExisting);
    static_assert(
        arbitration_action(2, 3) ==
        ArbitrationAction::ContinueScan);
    static_assert(
        arbitration_action(0, 1) ==
        ArbitrationAction::ContinueScan);
    static_assert(
        arbitration_action(1, 1, false) ==
        ArbitrationAction::ContinueScan);

    static_assert(!should_stop_active_slot(
        false, false, false, true));
    static_assert(should_stop_active_slot(
        false, false, false, false));
    static_assert(should_stop_active_slot(
        true, true, false, true));
    static_assert(should_stop_active_slot(
        true, false, true, true));
    static_assert(!should_stop_active_slot(
        true, false, false, true));
    static_assert(should_stop_active_slot(
        true, true, false, false));

    RetailSoundManager32 manager{};
    manager.slot_by_sound_id.fill(static_cast<std::int8_t>(-1));

    assert(manager.valid_sound_id(0));
    assert(manager.valid_sound_id(1099));
    assert(!manager.valid_sound_id(-1));
    assert(!manager.valid_sound_id(1100));

    assert(manager.mapped_slot(0) == -1);
    manager.slot_by_sound_id[700] = 12;
    assert(manager.mapped_slot(700) == 12);

    manager.slot_state[12] = static_cast<std::int32_t>(SlotState::Active);
    assert(manager.state(12) == SlotState::Active);

    std::array<bool, kManagedSlotCount> playing{};

    auto choice = select_slot_for_acquire(manager, playing);
    assert(choice.slot == 0);
    assert(!choice.requires_release);

    for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
        manager.slot_state[i] = static_cast<std::int32_t>(SlotState::Active);
        manager.priority[i] = 50;
        playing[i] = true;
    }

    playing[7] = false;
    manager.priority[7] = 20;
    playing[13] = false;
    manager.priority[13] = 10;

    choice = select_slot_for_acquire(manager, playing);
    assert(choice.slot == 13);
    assert(choice.requires_release);

    manager.priority[7] = 101;
    manager.priority[13] = 101;
    choice = select_slot_for_acquire(manager, playing);
    assert(choice.slot == -1);
    assert(!choice.requires_release);

    manager.sound_id_by_slot[5] = 700;
    manager.slot_by_sound_id[700] = 5;
    manager.buffer_group_ptr32[5] = 0x12345678;
    manager.priority[5] = 44;
    manager.slot_state[5] = static_cast<std::int32_t>(SlotState::Stopped);

    clear_released_slot_metadata(manager, 5);

    assert(manager.buffer_group_ptr32[5] == 0);
    assert(manager.priority[5] == -1);
    assert(manager.slot_state[5] == static_cast<std::int32_t>(SlotState::Free));
    assert(manager.sound_id_by_slot[5] == -1);
    assert(manager.slot_by_sound_id[700] == -1);

    install_acquired_slot_metadata(manager, 9, 711, 50, 2);
    assert(manager.sound_id_by_slot[9] == 711);
    assert(manager.slot_by_sound_id[711] == 9);
    assert(manager.priority[9] == 50);
    assert(manager.input_interruptible[9] == 0);
    assert(manager.arbitration_class[9] == 2);

    RetailSoundManager32 initialized{};
    initialize_retail_metadata(initialized);
    for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
        assert(initialized.buffer_group_ptr32[i] == 0);
        assert(initialized.sound_id_by_slot[i] == -1);
        assert(initialized.priority[i] == 101);
        assert(initialized.slot_state[i] == 0);
        assert(initialized.arbitration_class[i] == -1);
    }
    for (std::size_t i = 0; i < kSoundCatalogCount; ++i) {
        assert(initialized.slot_by_sound_id[i] == -1);
        assert(initialized.sound_enabled[i] == 1);
    }

    // Brand-new acquire: low-level play happens once inside AcquireAndPlaySound
    // and once again when PlayManagedSoundById changes state 1 -> 2.
    ManagedPlayEnvironment env{};
    auto play_result = play_managed_sound(
        initialized, env, 700, 50, 2);
    assert(play_result.status == 1);
    assert(play_result.slot == 0);
    assert(play_result.load_attempted);
    assert(play_result.load_succeeded);
    assert(play_result.low_level_play_calls == 2);
    assert(initialized.slot_state[0] ==
           static_cast<int>(SlotState::Active));
    assert(initialized.sound_id_by_slot[0] == 700);
    assert(initialized.slot_by_sound_id[700] == 0);
    assert(env.group_exists[0]);
    assert(env.group_is_playing[0]);

    // A second request while state 2 is latched returns 2 immediately.
    play_result = play_managed_sound(
        initialized, env, 700, 50, 2);
    assert(play_result.status == 2);
    assert(play_result.already_active);
    assert(play_result.low_level_play_calls == 0);

    // Disabled catalog entries reject before arbitration/acquisition.
    initialized.sound_enabled[701] = 0;
    play_result = play_managed_sound(
        initialized, env, 701, 50, 1);
    assert(play_result.status == 0);
    assert(play_result.disabled);

    // Existing class-1 playback blocks any incoming arbitrated request.
    initialize_retail_metadata(initialized);
    env = {};
    install_acquired_slot_metadata(initialized, 4, 100, 40, 1);
    initialized.slot_state[4] =
        static_cast<int>(SlotState::Active);
    initialized.buffer_group_ptr32[4] = 1;
    env.group_exists[4] = true;
    env.group_is_playing[4] = true;

    play_result = play_managed_sound(
        initialized, env, 200, 50, 2);
    assert(play_result.status == 0);
    assert(play_result.rejected_by_exclusive_blocker);
    assert(initialized.slot_by_sound_id[200] == -1);

    // Existing class 2 is preempted by incoming class 1/2, then the new sound
    // is admitted into the first free slot.
    initialize_retail_metadata(initialized);
    env = {};
    install_acquired_slot_metadata(initialized, 4, 100, 40, 2);
    initialized.slot_state[4] =
        static_cast<int>(SlotState::Active);
    initialized.buffer_group_ptr32[4] = 1;
    env.group_exists[4] = true;
    env.group_is_playing[4] = true;

    play_result = play_managed_sound(
        initialized, env, 200, 50, 1);
    assert(play_result.status == 1);
    assert(play_result.preempted_count == 1);
    assert(play_result.preempted_slots[0] == 4);
    assert(initialized.slot_state[4] ==
           static_cast<int>(SlotState::Stopped));
    assert(play_result.slot == 0);

    // Stopped mapped sounds restart without reloading.
    initialize_retail_metadata(initialized);
    env = {};
    install_acquired_slot_metadata(initialized, 3, 333, 20, 0);
    initialized.slot_state[3] =
        static_cast<int>(SlotState::Stopped);
    initialized.buffer_group_ptr32[3] = 1;
    env.group_exists[3] = true;

    play_result = play_managed_sound(
        initialized, env, 333, 20, 0);
    assert(play_result.status == 1);
    assert(!play_result.load_attempted);
    assert(play_result.low_level_play_calls == 1);
    assert(initialized.slot_state[3] ==
           static_cast<int>(SlotState::Active));
    assert(env.group_is_playing[3]);

    // Retail reports status 1 even when the newly acquired CSound failed to
    // materialize and its slot remains state 0.
    initialize_retail_metadata(initialized);
    env = {};
    env.acquired_group_load_succeeds = false;
    play_result = play_managed_sound(
        initialized, env, 444, 50, 0);
    assert(play_result.status == 1);
    assert(play_result.load_attempted);
    assert(!play_result.load_succeeded);
    assert(play_result.accepted_even_though_load_failed);
    assert(play_result.low_level_play_calls == 0);
    assert(initialized.slot_state[0] ==
           static_cast<int>(SlotState::Free));
    assert(initialized.slot_by_sound_id[444] == 0);

    // Reaper stops state-2 slots either when audio ended, or immediately on an
    // input pulse when the slot's +0xD98 flag is nonzero.
    initialize_retail_metadata(initialized);
    std::array<bool,kManagedSlotCount> runtime_playing{};
    initialized.slot_state[2] =
        static_cast<int>(SlotState::Active);
    initialized.slot_state[3] =
        static_cast<int>(SlotState::Active);
    initialized.input_interruptible[2] = 0;
    initialized.input_interruptible[3] = 1;
    runtime_playing[2] = false;
    runtime_playing[3] = true;

    auto reap = reap_finished_slots(
        initialized, runtime_playing, true, false);
    assert(reap.stopped_count == 2);
    assert(reap.stopped_slots[0] == 2);
    assert(reap.stopped_slots[1] == 3);
    assert(initialized.slot_state[2] ==
           static_cast<int>(SlotState::Stopped));
    assert(initialized.slot_state[3] ==
           static_cast<int>(SlotState::Stopped));
}
