#pragma once
// Native Windows interpretations of original bitmap table entry points:
//   004036E0 mark keyed, 00403720 unregister, 00403770 register,
//   00403830 reload. These are real pointer-to-COM-surface records, not
// 32-bit fake references or effect-plan events. Retail unsafe overflow,
// filename overflow and dangling-reference behavior is guarded.
// Executable x86 ABI/differential parity has not been proven.
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <ddraw.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace btb::original_win32 {
class NativeBitmapRegistry final {
public:
    using SurfaceReference=IDirectDrawSurface7**;
    static constexpr std::size_t kSlots=800;
    static constexpr std::size_t kFilenameBytes=0x104;

    [[nodiscard]] bool register_surface_reference(
        SurfaceReference location,std::string_view filename) noexcept;
    [[nodiscard]] bool unregister_surface_reference(
        SurfaceReference location) noexcept;
    [[nodiscard]] bool mark_registered_surface_color_keyed(
        SurfaceReference location) noexcept;
    [[nodiscard]] bool reload_registered_surfaces(
        IDirectDraw7* directdraw) noexcept;

    [[nodiscard]] std::size_t high_water_count() const noexcept {
        return high_water_;
    }
    [[nodiscard]] bool reload_in_progress() const noexcept {
        return reloading_;
    }

private:
    // Exactly the three *logical* tables seen at retail addresses
    // 0044EB1C, 0044DE9C and 0044F79C, extended to native host
    // pointer width. A slot contains an address of a surface pointer.
    std::array<SurfaceReference,kSlots> locations_{};
    std::array<std::uint32_t,kSlots> keyed_{};
    std::array<std::array<char,kFilenameBytes>,kSlots> filenames_{};
    std::size_t high_water_{};
    bool reloading_{};
};
} // namespace btb::original_win32
#endif
