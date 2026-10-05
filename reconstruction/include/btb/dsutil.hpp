#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::dsutil {

// The executable contains a lightly customized DirectX SDK DSUtil family.
// Pointer-shaped Win32/COM members remain uint32_t so these layouts are exact
// regardless of the host architecture used to build the reconstruction.

struct RetailCSoundManager32 {
    std::uint32_t m_pDS{}; // IDirectSound8*
};
static_assert(sizeof(RetailCSoundManager32) == 0x04);

struct RetailCSound32 {
    std::uint32_t vtable_ptr32{};   // +0x00
    std::uint32_t m_apDSBuffer{};   // +0x04 IDirectSoundBuffer**
    std::uint32_t m_dwDSBufferSize{}; // +0x08
    std::uint32_t m_pWaveFile{};    // +0x0C CWaveFile*
    std::uint32_t m_dwNumBuffers{}; // +0x10
};

static_assert(offsetof(RetailCSound32, m_apDSBuffer) == 0x04);
static_assert(offsetof(RetailCSound32, m_dwDSBufferSize) == 0x08);
static_assert(offsetof(RetailCSound32, m_pWaveFile) == 0x0C);
static_assert(offsetof(RetailCSound32, m_dwNumBuffers) == 0x10);
static_assert(sizeof(RetailCSound32) == 0x14);

struct RetailMMCKINFO32 {
    std::uint32_t ckid{};
    std::uint32_t cksize{};
    std::uint32_t fccType{};
    std::uint32_t dwDataOffset{};
    std::uint32_t dwFlags{};
};
static_assert(sizeof(RetailMMCKINFO32) == 0x14);

struct RetailCWaveFile32 {
    std::uint32_t m_pwfx{};          // +0x00 WAVEFORMATEX*
    std::uint32_t m_hmmio{};         // +0x04 HMMIO
    RetailMMCKINFO32 m_ck{};         // +0x08
    RetailMMCKINFO32 m_ckRiff{};     // +0x1C
    std::uint32_t m_dwSize{};        // +0x30
    std::array<std::byte, 0x48> m_mmioinfoOut{}; // +0x34 MMIOINFO
    std::uint32_t m_dwFlags{};       // +0x7C
    std::int32_t m_bIsReadingFromMemory{}; // +0x80 BOOL
    std::uint32_t m_pbData{};        // +0x84 BYTE*
    std::uint32_t m_pbDataCur{};     // +0x88 BYTE*
    std::uint32_t m_ulDataSize{};    // +0x8C ULONG
};

static_assert(offsetof(RetailCWaveFile32, m_pwfx) == 0x00);
static_assert(offsetof(RetailCWaveFile32, m_hmmio) == 0x04);
static_assert(offsetof(RetailCWaveFile32, m_ck) == 0x08);
static_assert(offsetof(RetailCWaveFile32, m_ckRiff) == 0x1C);
static_assert(offsetof(RetailCWaveFile32, m_dwSize) == 0x30);
static_assert(offsetof(RetailCWaveFile32, m_mmioinfoOut) == 0x34);
static_assert(offsetof(RetailCWaveFile32, m_dwFlags) == 0x7C);
static_assert(offsetof(RetailCWaveFile32, m_bIsReadingFromMemory) == 0x80);
static_assert(offsetof(RetailCWaveFile32, m_pbData) == 0x84);
static_assert(offsetof(RetailCWaveFile32, m_pbDataCur) == 0x88);
static_assert(offsetof(RetailCWaveFile32, m_ulDataSize) == 0x8C);
static_assert(sizeof(RetailCWaveFile32) == 0x90);

// This retail build is a smaller/customized DSUtil variant:
// - CSound has no m_dwCreationFlags member.
// - CSound::Play takes only priority + flags; volume is applied from the
//   game's global volume immediately after starting the DirectSound buffer.
// - CSoundManager::Initialize additionally receives channels/frequency/bits
//   and calls SetPrimaryBufferFormat internally.

} // namespace btb::dsutil
