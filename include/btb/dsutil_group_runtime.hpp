#pragma once

#include "btb/dsutil.hpp"

#include <cstddef>
#include <cstdint>
#include <span>

namespace btb::dsutil {

// Source-backed host-neutral dispatch for the original CDirectSound SDK-style
// CSound group. Original entry points: 00404430 Stop; 00404470 Reset;
// 004044B0 IsSoundPlaying. The original 20-byte CSound32 layout is in dsutil.hpp.
//
// These callbacks invoke real IDirectSoundBuffer calls when a provider supplies
// them. They neither synthesize audio nor mutate the original pointer-shaped
// 32-bit object. The group is addressed without heap allocation.
struct BufferDispatch {
    void* buffer{};
    std::uint32_t (*stop)(void*) noexcept{};
    std::uint32_t (*set_current_position)(void*, std::uint32_t) noexcept{};
    std::uint32_t (*get_status)(void*, std::uint32_t*) noexcept{};
};

// Original CSound::Stop and CSound::Reset share this contract:
// - m_apDSBuffer==nullptr: CO_E_NOTINITIALIZED (0x800401F0)
// - m_dwNumBuffers==0 with table present: S_OK (0)
// - visit buffers in original index order; OR ALL returned HRESULT bits
// - do not early-out after one failed HRESULT
//
// Retail dereferences missing entries unconditionally (would crash). This
// adapter returns CO_E_NOTINITIALIZED for missing host callbacks instead of
// causing undefined behavior. Buffer dispatch count must cover NumBuffers.
[[nodiscard]] std::uint32_t original_sound_stop(
    const RetailCSound32& original,
    std::span<const BufferDispatch> buffers) noexcept;

[[nodiscard]] std::uint32_t original_sound_reset(
    const RetailCSound32& original,
    std::span<const BufferDispatch> buffers) noexcept;

// Original IsSoundPlaying checks only bit 0 (DSBSTATUS_PLAYING) from each
// GetStatus output; ignores returned HRESULT; skips null buffer pointers;
// does not early-out when it sees a playing buffer.
// This function reproduces that observation policy, not audio playback.
[[nodiscard]] std::uint32_t original_sound_is_playing(
    const RetailCSound32& original,
    std::span<const BufferDispatch> buffers) noexcept;

} // namespace btb::dsutil
