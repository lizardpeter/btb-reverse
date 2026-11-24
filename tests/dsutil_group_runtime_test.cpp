#include "btb/dsutil_group_runtime.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

namespace {

struct FakeBuffer {
    std::uint32_t stop_result{};
    std::uint32_t reset_result{};
    std::uint32_t status_result{};
    std::uint32_t status_hresult{};
    std::size_t stop_calls{};
    std::size_t reset_calls{};
    std::size_t status_calls{};
    std::uint32_t last_position{0xFFFFFFFFU};
    std::size_t last_order{};
};

std::size_t sequence = 0;

std::uint32_t stop(void* raw) noexcept {
    auto& f = *static_cast<FakeBuffer*>(raw);
    ++f.stop_calls;
    f.last_order = ++sequence;
    return f.stop_result;
}
std::uint32_t reset(void* raw, std::uint32_t position) noexcept {
    auto& f = *static_cast<FakeBuffer*>(raw);
    ++f.reset_calls;
    f.last_position = position;
    f.last_order = ++sequence;
    return f.reset_result;
}
std::uint32_t status(void* raw, std::uint32_t* output) noexcept {
    auto& f = *static_cast<FakeBuffer*>(raw);
    ++f.status_calls;
    f.last_order = ++sequence;
    *output = f.status_result;
    return f.status_hresult;
}

btb::dsutil::BufferDispatch adapt(FakeBuffer& buffer) {
    return {&buffer, stop, reset, status};
}

} // namespace

int main() {
    using namespace btb::dsutil;
    static_assert(sizeof(RetailCSound32) == 0x14);

    RetailCSound32 sound{};
    sound.m_dwNumBuffers = 3;
    std::array<FakeBuffer,3> mock{{{0,0,0,0},{0x80004005U,2,1,0},{0x88780096U,4,3,0x80004005U}}};
    std::array<BufferDispatch,3> ops{adapt(mock[0]),adapt(mock[1]),adapt(mock[2])};

    // Original null m_apDSBuffer takes precedence even when buffer count >0.
    assert(original_sound_stop(sound, ops) == 0x800401F0U);
    assert(original_sound_reset(sound, ops) == 0x800401F0U);
    assert(original_sound_is_playing(sound, ops) == 0U);
    for (const auto& f : mock) assert(f.stop_calls == 0);

    sound.m_apDSBuffer = 0x1000U; // shape marker: real pointers not required
    // All three calls occur; bitwise-OR preserves HRESULT error/status bits.
    sequence = 0;
    assert(original_sound_stop(sound, ops) ==
           (0x80004005U | 0x88780096U));
    for (std::size_t i=0;i<mock.size();++i) {
        assert(mock[i].stop_calls == 1);
        assert(mock[i].last_order == i+1);
    }

    sequence = 0;
    assert(original_sound_reset(sound, ops) == (0U | 2U | 4U));
    for (std::size_t i=0;i<mock.size();++i) {
        assert(mock[i].reset_calls == 1);
        assert(mock[i].last_position == 0U);
        assert(mock[i].last_order == i+1);
    }

    // Ignore HRESULT from GetStatus and OR bit 0 only. All buffers queried,
    // even if buffer 1 already reports playing.
    sequence = 0;
    assert(original_sound_is_playing(sound, ops) == 1U);
    for (std::size_t i=0;i<mock.size();++i) {
        assert(mock[i].status_calls == 1);
        assert(mock[i].last_order == i+1);
    }

    // A null buffer is skipped only by IsSoundPlaying.
    ops[1].buffer = nullptr;
    sequence = 0;
    assert(original_sound_is_playing(sound, ops) == 1U);
    assert(mock[1].status_calls == 1);
    assert(mock[0].status_calls == 2);
    assert(mock[2].status_calls == 2);

    // Explicit host-safety divergence instead of retail crash.
    assert(original_sound_stop(sound, ops) == 0x800401F0U);
    assert(original_sound_reset(sound, ops) == 0x800401F0U);
    assert(original_sound_is_playing(sound, std::span<const BufferDispatch>{}) == 0U);
    assert(original_sound_stop(sound, std::span<const BufferDispatch>{}) == 0x800401F0U);

    sound.m_dwNumBuffers = 0;
    assert(original_sound_stop(sound, ops) == 0U);
    assert(original_sound_reset(sound, ops) == 0U);
    assert(original_sound_is_playing(sound, ops) == 0U);
}
