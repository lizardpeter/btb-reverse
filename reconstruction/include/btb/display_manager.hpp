#pragma once

#include <cstddef>
#include <cstdint>

namespace btb::display {

struct RetailRect32 {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
};

static_assert(sizeof(RetailRect32) == 0x10);

// Exact 32-bit display wrapper constructed at 0x00402FC0.
// All Win32/COM pointer-shaped fields remain uint32_t for host-independent
// layout fidelity.
struct RetailDisplayManager32 {
    std::uint32_t vtable_ptr32{};              // +0x00
    std::uint32_t direct_draw7_ptr32{};        // +0x04
    std::uint32_t primary_surface_ptr32{};     // +0x08
    std::uint32_t render_surface_ptr32{};      // +0x0C
    std::uint32_t auxiliary_surface_ptr32{};   // +0x10
    std::uint32_t hwnd32{};                    // +0x14
    RetailRect32 destination_rect{};           // +0x18
    std::int32_t windowed{};                   // +0x28
    std::int32_t reserved_2c{};                // +0x2C

    [[nodiscard]] constexpr bool is_windowed() const noexcept {
        return windowed != 0;
    }

    [[nodiscard]] constexpr std::int32_t destination_width() const noexcept {
        return destination_rect.right - destination_rect.left;
    }

    [[nodiscard]] constexpr std::int32_t destination_height() const noexcept {
        return destination_rect.bottom - destination_rect.top;
    }
};

static_assert(offsetof(RetailDisplayManager32, vtable_ptr32) == 0x00);
static_assert(offsetof(RetailDisplayManager32, direct_draw7_ptr32) == 0x04);
static_assert(offsetof(RetailDisplayManager32, primary_surface_ptr32) == 0x08);
static_assert(offsetof(RetailDisplayManager32, render_surface_ptr32) == 0x0C);
static_assert(offsetof(RetailDisplayManager32, auxiliary_surface_ptr32) == 0x10);
static_assert(offsetof(RetailDisplayManager32, hwnd32) == 0x14);
static_assert(offsetof(RetailDisplayManager32, destination_rect) == 0x18);
static_assert(offsetof(RetailDisplayManager32, windowed) == 0x28);
static_assert(offsetof(RetailDisplayManager32, reserved_2c) == 0x2C);
static_assert(sizeof(RetailDisplayManager32) == 0x30);

inline constexpr std::int32_t kRetailWidth = 640;
inline constexpr std::int32_t kRetailHeight = 480;
inline constexpr std::int32_t kRetailFullscreenBitsPerPixel = 16;

} // namespace btb::display
