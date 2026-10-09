#pragma once

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <dsound.h>

#include "btb/full_game_audio.hpp"
#include "btb/full_game_resources.hpp"

#include <array>
#include <filesystem>
#include <string>
#include <vector>

namespace btb::full_game {

// Actual Win32 DirectSound8 buffer provider for the reconstructed original
// game's 80 concurrently managed slots and activity-specific WAV playback.
// Does not claim a buffer is playing: each frame queries GetStatus from the
// real device and passes the results back to the source-level policy.
class Win32OriginalDirectSound final {
public:
    Win32OriginalDirectSound() = default;
    ~Win32OriginalDirectSound();
    Win32OriginalDirectSound(const Win32OriginalDirectSound&) = delete;
    Win32OriginalDirectSound& operator=(
        const Win32OriginalDirectSound&) = delete;

    [[nodiscard]] bool initialize(HWND hwnd,std::string& error);
    [[nodiscard]] bool observe(
        ManagedSoundObservation& managed,
        bool& any_managed_voice_playing,
        std::string& error);
    [[nodiscard]] bool apply(
        const ResolvedSoundEffect& effect,std::string& error);
    void shutdown() noexcept;

private:
    [[nodiscard]] bool create_wave_buffer(
        const std::filesystem::path& original_file,
        IDirectSoundBuffer*& result,
        std::string& error);
    [[nodiscard]] bool play_once(
        IDirectSoundBuffer* buffer,bool loop,
        std::string& error);
    void reap_finished_samples() noexcept;
    void release_buffer(IDirectSoundBuffer*& buffer) noexcept;

    IDirectSound8* sound_{};
    std::array<IDirectSoundBuffer*,sound::kManagedSlotCount> slots_{};
    IDirectSoundBuffer* backing_{};
    std::vector<IDirectSoundBuffer*> samples_{};
};

} // namespace btb::full_game
#endif
