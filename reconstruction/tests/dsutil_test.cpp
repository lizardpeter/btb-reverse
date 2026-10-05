#include "btb/dsutil.hpp"

#include <cassert>
#include <cstddef>

using namespace btb::dsutil;

int main() {
    static_assert(sizeof(RetailCSoundManager32) == 0x04);
    static_assert(sizeof(RetailCSound32) == 0x14);
    static_assert(sizeof(RetailMMCKINFO32) == 0x14);
    static_assert(sizeof(RetailCWaveFile32) == 0x90);

    static_assert(offsetof(RetailCSound32, m_apDSBuffer) == 0x04);
    static_assert(offsetof(RetailCSound32, m_dwDSBufferSize) == 0x08);
    static_assert(offsetof(RetailCSound32, m_pWaveFile) == 0x0C);
    static_assert(offsetof(RetailCSound32, m_dwNumBuffers) == 0x10);

    static_assert(offsetof(RetailCWaveFile32, m_ck) == 0x08);
    static_assert(offsetof(RetailCWaveFile32, m_ckRiff) == 0x1C);
    static_assert(offsetof(RetailCWaveFile32, m_dwSize) == 0x30);
    static_assert(offsetof(RetailCWaveFile32, m_mmioinfoOut) == 0x34);
    static_assert(offsetof(RetailCWaveFile32, m_dwFlags) == 0x7C);
    static_assert(offsetof(RetailCWaveFile32, m_bIsReadingFromMemory) == 0x80);
    static_assert(offsetof(RetailCWaveFile32, m_pbData) == 0x84);
    static_assert(offsetof(RetailCWaveFile32, m_pbDataCur) == 0x88);
    static_assert(offsetof(RetailCWaveFile32, m_ulDataSize) == 0x8C);

    RetailCSound32 sound{};
    sound.m_dwNumBuffers = 3;
    assert(sound.m_dwNumBuffers == 3);

    RetailCWaveFile32 wave{};
    wave.m_dwFlags = 1;
    wave.m_bIsReadingFromMemory = 0;
    assert(wave.m_dwFlags == 1);
    assert(wave.m_bIsReadingFromMemory == 0);
}
