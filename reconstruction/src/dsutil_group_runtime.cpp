#include "btb/dsutil_group_runtime.hpp"

namespace btb::dsutil {
namespace {

[[nodiscard]] constexpr std::uint32_t not_initialized() noexcept {
    return static_cast<std::uint32_t>(kCoENotInitialized);
}

[[nodiscard]] bool invalid_dispatch(
    const RetailCSound32& sound,
    std::span<const BufferDispatch> buffers) noexcept {
    return sound.m_dwNumBuffers > buffers.size();
}

} // namespace

std::uint32_t original_sound_stop(
    const RetailCSound32& sound,
    std::span<const BufferDispatch> buffers) noexcept {

    if (sound.m_apDSBuffer == 0 || invalid_dispatch(sound, buffers)) {
        return not_initialized();
    }
    std::uint32_t combined = 0;
    for (std::size_t i = 0; i <sound.m_dwNumBuffers; ++i) {
        const auto& item = buffers[i];
        if (item.buffer == nullptr || item.stop == nullptr) {
            return not_initialized(); // invalid host provider, never silently skip
        }
        combined |= item.stop(item.buffer);
    }
    return combined;
}

std::uint32_t original_sound_reset(
    const RetailCSound32& sound,
    std::span<const BufferDispatch> buffers) noexcept {

    if (sound.m_apDSBuffer == 0 || invalid_dispatch(sound, buffers)) {
        return not_initialized();
    }
    std::uint32_t combined = 0;
    for (std::size_t i = 0; i < sound.m_dwNumBuffers; ++i) {
        const auto& item = buffers[i];
        if (item.buffer == nullptr || item.set_current_position == nullptr) {
            return not_initialized();
        }
        combined |= item.set_current_position(item.buffer, 0);
    }
    return combined;
}

std::uint32_t original_sound_is_playing(
    const RetailCSound32& sound,
    std::span<const BufferDispatch> buffers) noexcept {

    if (sound.m_apDSBuffer == 0 || invalid_dispatch(sound, buffers)) {
        return 0;
    }
    std::uint32_t playing = 0;
    for (std::size_t i = 0; i < sound.m_dwNumBuffers; ++i) {
        const auto& item = buffers[i];
        if (item.buffer == nullptr || item.get_status == nullptr) {
            continue;
        }
        std::uint32_t status = 0;
        static_cast<void>(item.get_status(item.buffer, &status));
        playing |= status & 1U;
    }
    return playing;
}

} // namespace btb::dsutil
