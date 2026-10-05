#include "btb/sound_manager.hpp"

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
    static_assert(offsetof(RetailSoundManager32, persistent) == 0xD98);
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

    manager.slot_state[12] = static_cast<std::int32_t>(SlotState::Playing);
    assert(manager.state(12) == SlotState::Playing);
}
