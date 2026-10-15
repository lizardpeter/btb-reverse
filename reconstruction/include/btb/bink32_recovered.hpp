#pragma once
// Reconstructed from 2002 binkw32.dll (SHA-256
// 16515bffb88d058edf3553a76bf8d4a696061a58ab750afccfd5173ccd5b0d7f).
// Portable C++ source models, NOT ABI-compatible DLL or a video decoder.
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>

namespace btb::bink32_recovered {
struct Globals {
    // Original DLL global offsets:
    // 30054000/004 alloc/free; 30054014/018/01C buffer resolution;
    // 30050DE8 error; 30054064 frame rate; 30050DE0 divisor;
    // 30054068 IO size; 30054094 IO callback; 3005406C simulate;
    // 30054070/074 sound-track count and IDs.
    std::uintptr_t allocation_callback{}, free_callback{};
    std::uint32_t buffer_width{}, buffer_height{}, buffer_bpp{};
    std::string error;
    std::uint32_t frame_rate{}, frame_rate_divisor{}, io_buffer_size{};
    std::uintptr_t io_callback{};
    std::uint32_t simulation{}, sound_track_count{};
    std::array<std::uint32_t,8> sound_track_ids{};
    std::string buffer_error;
};
struct Movie {
    // HBINK observed offsets: +0x08 frame count, +0x10C frame offsets/flags,
    // +0x260/264/268 track info, +0x2A4 video switch.
    std::uint32_t frame_count{};
    std::span<const std::uint32_t> frame_offset_flags{};
    std::span<const std::uint32_t> track_max_sizes{}, track_types{}, track_ids{};
    std::uint32_t video_on_off{};
};
void rad_set_memory(Globals&, std::uintptr_t alloc, std::uintptr_t release) noexcept;
void buffer_set_resolution(Globals&, std::uint32_t width,
                           std::uint32_t height, std::uint32_t bpp) noexcept;
const char* buffer_get_error(const Globals&) noexcept;
void set_error(Globals&, const char* message);
const char* get_error(const Globals&) noexcept;
void set_frame_rate(Globals&, std::uint32_t rate, std::uint32_t divisor) noexcept;
void set_io_size(Globals&, std::uint32_t bytes) noexcept;
void set_io(Globals&, std::uintptr_t callback) noexcept;
void set_simulate(Globals&, std::uint32_t amount) noexcept;
void set_sound_track(Globals&, std::uint32_t count, std::span<const std::uint32_t> ids);
std::uint32_t get_track_type(const Movie*, std::size_t index) noexcept;
std::uint32_t get_track_max_size(const Movie*, std::size_t index) noexcept;
std::uint32_t get_track_id(const Movie*, std::size_t index) noexcept;
void set_video_on_off(Movie*, std::uint32_t enabled) noexcept;
// Original _BinkGetKeyFrame@12 at 0x300066B0; modes 0/1/2 and 0x80 bit.
std::uint32_t get_key_frame(const Movie*, std::uint32_t frame,
                            std::uint32_t mode) noexcept;
} // namespace btb::bink32_recovered
