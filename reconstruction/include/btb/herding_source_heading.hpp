#pragma once

#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Original retail shared angle helper at 0x00415D70. Inputs are
// (actor_x,actor_y,target_x,target_y), each signed 32-bit integer.
// Returns the direction used by Herding's 0x416C45 and 0x416DCE
// FSIN/FCOS movement kernels: 0=Up,90=Right,180=Down,270=Left.
//
// Source x87 constants:
//  0x0043B3C4 = 0x42652E14 = 57.29499816894531
//  0x0043B3C8 = 0x461C3C00 = 9999.0
//
// Source uses absolute DELTA-X and DELTA-Y. When delta-X==0,
// it substitutes 9999 instead of computing actual atan2,
// producing about 89.9943 degrees of base angle. That is why
// perfectly vertical UP yields integer 359, not zero.
//
// The original uses FPATAN in x87 extended precision and truncates
// with 0x4304D0; std::atan is an equation-faithful approximation,
// NOT asserted to be bit-perfect near integer degree boundaries.
inline constexpr float kRetailAngleDegreesPerRadian=57.29499816894531f;
inline constexpr float kRetailAngleZeroXFallback=9999.0f;
static_assert(std::bit_cast<std::uint32_t>(
    kRetailAngleDegreesPerRadian)==0x42652E14u);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailAngleZeroXFallback)==0x461C3C00u);

[[nodiscard]] inline std::optional<std::int32_t>
original_herding_integer_heading(
    std::int32_t actor_x,
    std::int32_t actor_y,
    std::int32_t target_x,
    std::int32_t target_y) noexcept {

    const auto signed_dx =
        static_cast<std::int64_t>(actor_x)-target_x;
    const auto signed_dy =
        static_cast<std::int64_t>(actor_y)-target_y;
    // The normal retail gameplay domain never overflows the signed
    // differences. Reject unusual external probes rather than making
    // x86 IMUL/ABS overflow guesswork part of a standard C++ API.
    if (signed_dx<std::numeric_limits<std::int32_t>::min() ||
        signed_dx>std::numeric_limits<std::int32_t>::max() ||
        signed_dy<std::numeric_limits<std::int32_t>::min() ||
        signed_dy>std::numeric_limits<std::int32_t>::max()) {
        return std::nullopt;
    }
    const auto abs_dx=std::abs(signed_dx);
    const auto abs_dy=std::abs(signed_dy);

    // 0x415D84..0x415DBC: the native zero-dX branch selects a
    // fixed 9999.0 value; otherwise FPATAN receives abs(dy)/abs(dx).
    const double ratio=abs_dx==0 ?
        static_cast<double>(kRetailAngleZeroXFallback) :
        static_cast<double>(abs_dy)/static_cast<double>(abs_dx);
    const double base=std::atan(ratio) *
        static_cast<double>(kRetailAngleDegreesPerRadian);

    // 0x415DD6..0x415E43 preserves the original four signed
    // quadrant cases; do not replace with atan2 or degree modulo.
    double heading{};
    if (signed_dx<0) {
        heading = signed_dy<0 ? base+90.0 : 90.0-base;
    } else {
        heading = signed_dy<0 ? 270.0-base : 270.0+base;
    }
    if (!std::isfinite(heading) ||
        heading < std::numeric_limits<std::int32_t>::min() ||
        heading > std::numeric_limits<std::int32_t>::max()) {
        return std::nullopt;
    }
    return static_cast<std::int32_t>(heading);
}

} // namespace btb::herding
