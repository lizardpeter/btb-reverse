#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::sound {

inline constexpr std::size_t kManagedSlotCount = 80;
inline constexpr std::size_t kSoundCatalogCount = 1100;
inline constexpr std::size_t kFilenameRecordBytes = 20;
inline constexpr std::size_t kRetailSoundManagerBytes = 0x7738;

enum class SlotState : std::int32_t {
    Free = 0,
    NewlyAcquired = 1,
    Active = 2,
    Stopped = 3,
};

struct FilenameRecord {
    std::array<char, kFilenameRecordBytes> bytes{};
};

static_assert(sizeof(FilenameRecord) == kFilenameRecordBytes);

// Exact 32-bit retail object allocated by WinMain and constructed by
// 0x00402B00. COM / heap pointers are represented as uint32_t so the layout
// remains byte-identical on a 64-bit reconstruction host.
struct RetailSoundManager32 {
    std::array<std::uint32_t, kManagedSlotCount> buffer_group_ptr32{}; // +0x000
    std::array<std::int32_t, kManagedSlotCount> sound_id_by_slot{};    // +0x140

    std::array<std::int8_t, kSoundCatalogCount> slot_by_sound_id{};     // +0x280
    std::array<std::uint8_t, kSoundCatalogCount> sound_enabled{};       // +0x6CC

    std::array<std::int32_t, kManagedSlotCount> priority_or_age{};      // +0xB18
    std::array<std::int32_t, kManagedSlotCount> slot_state{};           // +0xC58
    std::array<std::int32_t, kManagedSlotCount> special_lifetime_flag{};// +0xD98
    std::array<std::int32_t, kManagedSlotCount> playback_policy{};      // +0xED8

    std::array<FilenameRecord, kSoundCatalogCount> filename_by_sound_id{}; // +0x1018
    std::array<std::int32_t, kSoundCatalogCount> catalog_metadata{};       // +0x6608

    [[nodiscard]] constexpr bool valid_sound_id(
        std::int32_t sound_id) const noexcept {
        return sound_id >= 0 &&
               sound_id < static_cast<std::int32_t>(kSoundCatalogCount);
    }

    [[nodiscard]] constexpr std::int32_t mapped_slot(
        std::int32_t sound_id) const noexcept {
        if (!valid_sound_id(sound_id)) {
            return -1;
        }
        return static_cast<std::int32_t>(
            slot_by_sound_id[static_cast<std::size_t>(sound_id)]);
    }

    [[nodiscard]] constexpr SlotState state(
        std::size_t slot) const noexcept {
        return static_cast<SlotState>(slot_state[slot]);
    }
};

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
static_assert(sizeof(RetailSoundManager32) == kRetailSoundManagerBytes);

struct AcquireSlotChoice {
    std::int32_t slot{-1};
    bool requires_release{false};
};

[[nodiscard]] constexpr AcquireSlotChoice select_slot_for_acquire(
    const RetailSoundManager32& manager,
    const std::array<bool, kManagedSlotCount>& slot_is_playing) noexcept {

    for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
        if (manager.slot_state[i] == static_cast<std::int32_t>(SlotState::Free)) {
            return {static_cast<std::int32_t>(i), false};
        }
    }

    std::int32_t best_slot = -1;
    std::int32_t best_priority = 101;

    for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
        if (slot_is_playing[i]) {
            continue;
        }

        const auto priority = manager.priority_or_age[i];
        if (priority < best_priority) {
            best_priority = priority;
            best_slot = static_cast<std::int32_t>(i);
        }
    }

    return {best_slot, best_slot >= 0};
}

constexpr void clear_released_slot_metadata(
    RetailSoundManager32& manager,
    std::size_t slot) noexcept {

    const auto old_sound_id = manager.sound_id_by_slot[slot];

    manager.buffer_group_ptr32[slot] = 0;
    manager.priority_or_age[slot] = -1;
    manager.slot_state[slot] = static_cast<std::int32_t>(SlotState::Free);

    if (manager.valid_sound_id(old_sound_id)) {
        manager.slot_by_sound_id[
            static_cast<std::size_t>(old_sound_id)] =
                static_cast<std::int8_t>(-1);
    }

    manager.sound_id_by_slot[slot] = -1;
}

constexpr void install_acquired_slot_metadata(
    RetailSoundManager32& manager,
    std::size_t slot,
    std::int32_t sound_id,
    std::int32_t priority,
    std::int32_t playback_policy) noexcept {

    manager.sound_id_by_slot[slot] = sound_id;
    manager.slot_by_sound_id[static_cast<std::size_t>(sound_id)] =
        static_cast<std::int8_t>(slot);
    manager.priority_or_age[slot] = priority;
    manager.special_lifetime_flag[slot] = 0;
    manager.playback_policy[slot] = playback_policy;
}

} // namespace btb::sound
