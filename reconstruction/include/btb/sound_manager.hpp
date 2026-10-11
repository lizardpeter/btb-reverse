#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

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

// Exact +0xED8 arbitration classes used by PlayManagedSoundById.
// Class 0 bypasses the cross-sound admission scan. An already-playing class-1
// sound rejects any incoming request whose class is >0. An already-playing
// class-2 sound is preempted by incoming class 1 or 2.
enum class ArbitrationClass : std::int32_t {
    None = 0,
    ExclusiveBlocker = 1,
    Preemptible = 2,
};

enum class ArbitrationAction : std::int32_t {
    ContinueScan,
    RejectIncoming,
    StopExisting,
};

[[nodiscard]] constexpr ArbitrationAction arbitration_action(
    std::int32_t existing_class,
    std::int32_t incoming_class,
    bool existing_is_playing = true) noexcept {

    if (!existing_is_playing || incoming_class <= 0) {
        return ArbitrationAction::ContinueScan;
    }

    if (existing_class ==
        static_cast<std::int32_t>(ArbitrationClass::ExclusiveBlocker)) {
        return ArbitrationAction::RejectIncoming;
    }

    if (existing_class ==
            static_cast<std::int32_t>(ArbitrationClass::Preemptible) &&
        (incoming_class ==
             static_cast<std::int32_t>(ArbitrationClass::ExclusiveBlocker) ||
         incoming_class ==
             static_cast<std::int32_t>(ArbitrationClass::Preemptible))) {
        return ArbitrationAction::StopExisting;
    }

    return ArbitrationAction::ContinueScan;
}

// Exact state-2 reap rule. +0xD98 does not make a sound persistent: value 1
// makes the active slot additionally stop on either shared user-input pulse.
// All active slots are stopped/rewound once their DirectSound group finishes.
[[nodiscard]] constexpr bool should_stop_active_slot(
    bool input_interruptible,
    bool primary_input_pulse,
    bool secondary_input_pulse,
    bool buffer_group_is_playing) noexcept {

    if (input_interruptible &&
        (primary_input_pulse || secondary_input_pulse)) {
        return true;
    }

    return !buffer_group_is_playing;
}

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

    std::array<std::int32_t, kManagedSlotCount> priority{};              // +0xB18
    std::array<std::int32_t, kManagedSlotCount> slot_state{};            // +0xC58
    std::array<std::int32_t, kManagedSlotCount> input_interruptible{};   // +0xD98
    std::array<std::int32_t, kManagedSlotCount> arbitration_class{};     // +0xED8

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
static_assert(offsetof(RetailSoundManager32, priority) == 0xB18);
static_assert(offsetof(RetailSoundManager32, slot_state) == 0xC58);
static_assert(offsetof(RetailSoundManager32, input_interruptible) == 0xD98);
static_assert(offsetof(RetailSoundManager32, arbitration_class) == 0xED8);
static_assert(offsetof(RetailSoundManager32, filename_by_sound_id) == 0x1018);
static_assert(offsetof(RetailSoundManager32, catalog_metadata) == 0x6608);
static_assert(sizeof(RetailSoundManager32) == kRetailSoundManagerBytes);

// Exact 0x00402C60: resolve a requested sound ID to its signed 8-bit
// reverse-mapped slot and query that group's native status. The caller
// provides real observed DSBSTATUS_PLAYING bits; this does not predict audio.
// Retail does not bounds-check IDs or corrupt slot indices. The source-level
// reconstruction refuses invalid host metadata rather than reading out of
// bounds, while preserving valid retail behavior.
[[nodiscard]] constexpr bool is_sound_id_playing(
    const RetailSoundManager32& manager,
    std::int32_t sound_id,
    const std::array<bool,kManagedSlotCount>& observed_playing) noexcept {

    const auto slot = manager.mapped_slot(sound_id);
    if (slot < 0 || slot >= static_cast<int>(kManagedSlotCount)) {
        return false;
    }
    const auto index = static_cast<std::size_t>(slot);
    return manager.buffer_group_ptr32[index] != 0 && observed_playing[index];
}

// Exact 0x00402C20: iterate original 80-slot sound_id_by_slot array and
// return 1 on first playing sound; do not infer activity from allocated buffers.
[[nodiscard]] constexpr bool any_managed_sound_playing(
    const RetailSoundManager32& manager,
    const std::array<bool,kManagedSlotCount>& observed_playing) noexcept {

    for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
        if (is_sound_id_playing(
                manager, manager.sound_id_by_slot[i], observed_playing)) {
            return true;
        }
    }
    return false;
}

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

        const auto priority = manager.priority[i];
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
    manager.priority[slot] = -1;
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
    std::int32_t arbitration_class) noexcept {

    manager.sound_id_by_slot[slot] = sound_id;
    manager.slot_by_sound_id[static_cast<std::size_t>(sound_id)] =
        static_cast<std::int8_t>(slot);
    manager.priority[slot] = priority;
    manager.input_interruptible[slot] = 0;
    manager.arbitration_class[slot] = arbitration_class;
}

struct ConstructorPlan {
    std::int32_t initial_priority{101};
    std::int32_t initial_sound_id{-1};
    std::int32_t initial_slot_state{0};
    std::int32_t initial_arbitration_class{-1};
    std::int8_t initial_reverse_map{-1};
    std::uint8_t initial_sound_enabled{1};
};

inline constexpr ConstructorPlan kConstructorPlan{};

constexpr void initialize_retail_metadata(
    RetailSoundManager32& manager) noexcept {

    for (std::size_t i = 0; i < kManagedSlotCount; ++i) {
        manager.buffer_group_ptr32[i] = 0;
        manager.sound_id_by_slot[i] = -1;
        manager.priority[i] = 101;
        manager.slot_state[i] = 0;
        manager.arbitration_class[i] = -1;
    }

    for (std::size_t i = 0; i < kSoundCatalogCount; ++i) {
        manager.slot_by_sound_id[i] = -1;
        manager.sound_enabled[i] = 1;
    }
}

struct StopSlotResult {
    bool stop_group{};
    bool rewind_group{};
};

[[nodiscard]] constexpr StopSlotResult stop_slot_metadata(
    RetailSoundManager32& manager,
    std::size_t slot) noexcept {
    manager.slot_state[slot] =
        static_cast<std::int32_t>(SlotState::Stopped);
    return {true,true};
}

struct ReapResult {
    std::array<std::int32_t,kManagedSlotCount> stopped_slots{};
    std::size_t stopped_count{};
};

[[nodiscard]] ReapResult reap_finished_slots(
    RetailSoundManager32& manager,
    const std::array<bool,kManagedSlotCount>& slot_group_is_playing,
    bool primary_input_pulse,
    bool secondary_input_pulse) noexcept;

struct ManagedPlayEnvironment {
    std::array<bool,kManagedSlotCount> group_exists{};
    std::array<bool,kManagedSlotCount> group_is_playing{};
    bool acquired_group_load_succeeds{true};
};

struct ManagedPlayResult {
    // Exact public return values from PlayManagedSoundById:
    // 0 = rejected/disabled, 1 = accepted path, 2 = already state-2 active.
    std::int32_t status{};
    bool disabled{};
    bool rejected_by_exclusive_blocker{};
    bool already_active{};
    std::int32_t slot{-1};
    std::optional<std::int32_t> released_slot{};
    std::array<std::int32_t,kManagedSlotCount> preempted_slots{};
    std::size_t preempted_count{};
    bool load_attempted{};
    bool load_succeeded{};
    std::int32_t low_level_play_calls{};
    bool would_index_negative_slot{};
    bool accepted_even_though_load_failed{};
};

// Source-level policy for 0x00402CF0 PlayManagedSoundById plus
// 0x00402E10 AcquireAndPlaySound. COM/WAV creation is supplied through the
// environment, while the retail metadata/arbitration/lifecycle transitions are
// performed exactly.
//
// For host safety this validates the sound ID and the -1 acquisition result;
// the shipped x86 routine has no such bounds guard and would index invalid
// memory in those impossible/error states.
[[nodiscard]] ManagedPlayResult play_managed_sound(
    RetailSoundManager32& manager,
    ManagedPlayEnvironment& environment,
    std::int32_t sound_id,
    std::int32_t priority,
    std::int32_t arbitration_class) noexcept;

} // namespace btb::sound
