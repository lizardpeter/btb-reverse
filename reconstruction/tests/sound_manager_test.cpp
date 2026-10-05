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
    static_assert(offsetof(RetailSoundManager32, priority_or_age) == 0xB18);
    static_assert(offsetof(RetailSoundManager32, slot_state) == 0xC58);
    static_assert(offsetof(RetailSoundManager32, special_lifetime_flag) == 0xD98);
    static_assert(offsetof(RetailSoundManager32, playback_policy) == 0xED8);
    static_assert(offsetof(RetailSoundManager32, filename_by_sound_id) == 0x1018);
    static_assert(offsetof(RetailSoundManager32, catalog_metadata) == 0x6608);

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
        manager.priority_or_age[i] = 50;
        playing[i] = true;
    }

    playing[7] = false;
    manager.priority_or_age[7] = 20;
    playing[13] = false;
    manager.priority_or_age[13] = 10;

    choice = select_slot_for_acquire(manager, playing);
    assert(choice.slot == 13);
    assert(choice.requires_release);

    manager.priority_or_age[7] = 101;
    manager.priority_or_age[13] = 101;
    choice = select_slot_for_acquire(manager, playing);
    assert(choice.slot == -1);
    assert(!choice.requires_release);

    manager.sound_id_by_slot[5] = 700;
    manager.slot_by_sound_id[700] = 5;
    manager.buffer_group_ptr32[5] = 0x12345678;
    manager.priority_or_age[5] = 44;
    manager.slot_state[5] = static_cast<std::int32_t>(SlotState::Stopped);

    clear_released_slot_metadata(manager, 5);

    assert(manager.buffer_group_ptr32[5] == 0);
    assert(manager.priority_or_age[5] == -1);
    assert(manager.slot_state[5] == static_cast<std::int32_t>(SlotState::Free));
    assert(manager.sound_id_by_slot[5] == -1);
    assert(manager.slot_by_sound_id[700] == -1);

    install_acquired_slot_metadata(manager, 9, 711, 50, 2);
    assert(manager.sound_id_by_slot[9] == 711);
    assert(manager.slot_by_sound_id[711] == 9);
    assert(manager.priority_or_age[9] == 50);
    assert(manager.special_lifetime_flag[9] == 0);
    assert(manager.playback_policy[9] == 2);
}
