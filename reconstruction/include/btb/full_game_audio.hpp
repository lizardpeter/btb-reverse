#pragma once

#include "btb/full_game_runtime.hpp"
#include "btb/sound_manager.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <istream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace btb::full_game {

// The retail SoundManager constructor (0x00402B00) reads
// Data/sound/binklist.txt as filename, numeric-ID pairs. Sound IDs and
// filename bytes are independent of the 80 simultaneously acquired slots.
// The installed game also ships a shorter loaddata/binklist.txt variant.
struct CatalogEntry {
    bool present{};
    std::string filename{};
};

struct CatalogLoadResult {
    bool success{};
    std::size_t entries{};
    std::size_t error_line{};
    std::string error{};
};

class SoundCatalog {
public:
    [[nodiscard]] CatalogLoadResult read(std::istream& stream);
    [[nodiscard]] const CatalogEntry* find(int sound_id) const noexcept;
    [[nodiscard]] std::size_t size() const noexcept { return count_; }

private:
    std::array<CatalogEntry, sound::kSoundCatalogCount> entries_{};
    std::size_t count_{};
};

// Source-neutral physical DirectSound operations. These are not synthesized
// audio waveforms or stand-in playback. They must be consumed, in order, by
// a real platform audio device. A command can be traced back to one native
// game-frame AudioOperation and the recovered sound-manager policy.
enum class SoundEffectKind {
    StopRewindManagedSlot,
    ReleaseManagedSlot,
    LoadManagedWav,
    PlayManagedSlot,
    StartBackingWav,
    StopBackingWav,
    PlayDirectSample,
};

struct SoundEffect {
    SoundEffectKind kind{};
    int slot{-1};
    int sound_id{-1};
    std::string filename{};
};

struct AudioFramePlan {
    std::vector<SoundEffect> operations{};
    std::vector<sound::ManagedPlayResult> managed_results{};
    std::vector<int> missing_catalog_sound_ids{};
    std::size_t invalid_requests{};
};

// Observations come from the actual audio device BEFORE the game-frame audio
// commands. They are not guessed from submitted effects: a sample may end
// asynchronously while the game is running.
struct ManagedSoundObservation {
    std::array<bool,sound::kManagedSlotCount> playing{};
    // Each native load is allowed to fail independently. The caller supplies
    // the status for a pending *new* buffer-group allocation (if attempted).
    bool next_managed_load_succeeds{true};
};

// Maps GameRoot's source-neutral Audio effects to the original 80-slot
// manager without making fake playing-status or timing assumptions.
//
// The collector is a planning state machine: its output is not proof that
// DirectSound played anything. The device layer must execute operations,
// supply current statuses next frame, and report a real load result.
class AudioEffectPlanner {
public:
    AudioEffectPlanner() noexcept {
        sound::initialize_retail_metadata(manager_);
    }

    void reset() noexcept;

    [[nodiscard]] CatalogLoadResult load_catalog(std::istream& input);

    [[nodiscard]] const SoundCatalog& catalog() const noexcept {
        return catalog_;
    }
    [[nodiscard]] const sound::RetailSoundManager32& manager() const noexcept {
        return manager_;
    }

    [[nodiscard]] AudioFramePlan plan(
        const std::vector<Audio>& requests,
        ManagedSoundObservation observed);

    // Model the original 0x00402B? managed-slot reap pass on a new frame.
    // Actual buffer-group Stop/Rewind calls must be issued from the returned
    // plan, and stopped slots remain cached for replay until reused.
    [[nodiscard]] AudioFramePlan reap(
        const ManagedSoundObservation& observed,
        bool primary_input_pulse,
        bool secondary_input_pulse);

private:
    void populate_retail_filename_records() noexcept;
    [[nodiscard]] std::string sound_file(int sound_id) const;

    SoundCatalog catalog_{};
    sound::RetailSoundManager32 manager_{};
};

} // namespace btb::full_game
