#pragma once
// Source-decoded Bink1 PCM16 -> actual Win32 DirectSound8 buffer.
// This class is deliberately NOT selected by the runtime hybrid Bink
// provider until retail BIK audio parity and video/audio sync are proven.
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include "btb/bink32_audio_movie.hpp"
#include <cstdint>
#include <optional>
#include <string>

namespace btb::full_game {
class Win32NativeBinkPcmVoice final {
public:
    // Borrows game-owned DirectSound8 during this call and AddRefs it;
    // the voice lifetime is therefore not tied to temporary lock spans.
    explicit Win32NativeBinkPcmVoice(IDirectSound8* device) noexcept;
    ~Win32NativeBinkPcmVoice();
    Win32NativeBinkPcmVoice(const Win32NativeBinkPcmVoice&)=delete;
    Win32NativeBinkPcmVoice& operator=(
        const Win32NativeBinkPcmVoice&)=delete;
    // Bounded static buffer for entire 2002 Bink1 movie audio in memory,
    // with no asset extraction or user-visible temporary PCM file.
    [[nodiscard]] bool load(
        const btb::bink32_recovered::Bink1MoviePcm16& audio,
        std::string& error);
    [[nodiscard]] bool start(std::string& error);
    [[nodiscard]] bool stop(std::string& error) noexcept;
    // Seek in decoded audio sample frames, not arbitrary BIK packet offsets;
    // rewinds/stops prior to setting the actual DirectSound byte cursor.
    [[nodiscard]] bool seek_frame(
        std::uint64_t frame,std::string& error);
    struct Playhead {
        std::uint64_t decoded_sample_frame{};
        bool playing{},complete{};
    };
    [[nodiscard]] std::optional<Playhead>
    observe(std::string& error) noexcept;
    void close() noexcept;
private:
    IDirectSound8* device_{};
    IDirectSoundBuffer* buffer_{};
    std::uint32_t rate_{},bytes_per_frame_{};
    std::uint64_t sample_frames_{};
    bool started_{},ever_played_{};
};
}
#endif
