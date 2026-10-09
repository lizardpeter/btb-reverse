#pragma once

#include "btb/herding_runtime.hpp"

#include <bit>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>

namespace btb::herding {

// Original retail constants, independently decoded from the verified
// PE32 .rdata section (the literal IEEE-754 float bits):
//   0x0043B384 = 0x3C8EFAB5 = 0.017453530803322792 (radians/degree)
//   0x0043B438 = 0x3F4CCCCD = 0.800000011920929 (minimum roaming speed)
//   0x0043B434 = 0x3C23D70A = 0.009999999776482582 (per-step gain)
//   0x0043B430 = 0x40400000 = 3.0 (special follower movement magnitude)
//
// Uses the source's actual INTEGER heading from 0x415D70. This code
// deliberately does not substitute an ordinary atan2 calculation:
// retail's integer heading helper has its own nonstandard quantization
// and quadrant behaviors that require separate source reconstruction.
inline constexpr float kRetailHerdingRadiansPerDegree =
    0.017453530803322792f;
inline constexpr float kRetailHerdingRoamingSpeedThreshold=0.8f;
inline constexpr float kRetailHerdingRoamingSpeedGain=0.01f;
inline constexpr float kRetailHerdingFollowerStep=3.0f;

static_assert(std::bit_cast<std::uint32_t>(
    kRetailHerdingRadiansPerDegree)==0x3C8EFAB5u);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailHerdingRoamingSpeedThreshold)==0x3F4CCCCDu);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailHerdingRoamingSpeedGain)==0x3C23D70Au);
static_assert(std::bit_cast<std::uint32_t>(
    kRetailHerdingFollowerStep)==0x40400000u);

struct RetailHerdingSteeringStep {
    float x{};
    float y{};
    std::int32_t rounded_x{}; // truncating original 0x4304D0
    std::int32_t rounded_y{};
    std::int32_t facing_index{};
    float next_speed{};
};

// Reproduces the branches common to 0x416C45..0x416CA5 and
// 0x416DCE..0x416E2E. The original x87 operations use FSIN/FCOS
// in 80-bit precision, with FSTP to 32-bit float on each axis,
// before integer conversion. std::sin/std::cos below reproduce the
// same directional math but NOT guaranteed bit-identical x87 pixels.
// Differential replay remains mandatory before exactness can be claimed.
[[nodiscard]] inline std::optional<RetailHerdingSteeringStep>
original_herding_steering_step(
    float source_x,
    float source_y,
    std::int32_t source_integer_heading_degrees,
    float magnitude,
    bool native_roaming_acceleration) noexcept {

    if (!std::isfinite(source_x) ||
        !std::isfinite(source_y) ||
        !std::isfinite(magnitude) ||
        magnitude < 0.0f) {
        return std::nullopt;
    }
    const auto radians=
        static_cast<double>(source_integer_heading_degrees) *
        static_cast<double>(kRetailHerdingRadiansPerDegree);
    const auto new_x=static_cast<float>(
        static_cast<double>(source_x) +
        std::sin(radians)*static_cast<double>(magnitude));
    const auto new_y=static_cast<float>(
        static_cast<double>(source_y) -
        std::cos(radians)*static_cast<double>(magnitude));
    if (!std::isfinite(new_x) || !std::isfinite(new_y) ||
        new_x<=static_cast<float>(
            std::numeric_limits<std::int32_t>::min()) ||
        new_y<=static_cast<float>(
            std::numeric_limits<std::int32_t>::min()) ||
        new_x>=static_cast<float>(
            std::numeric_limits<std::int32_t>::max()) ||
        new_y>=static_cast<float>(
            std::numeric_limits<std::int32_t>::max())) {
        return std::nullopt;
    }

    // Native 0x416C18/0x416DA1: signed integer quotient
    // (angle_degrees+22)/45, then subtract 8 when >=8. No floating
    // rounding and no modulo remap of negative indexes.
    const auto numerator=
        static_cast<std::int64_t>(source_integer_heading_degrees)+22;
    auto facing=static_cast<std::int32_t>(numerator/45);
    if (facing>=8) facing-=8;

    auto next_speed=magnitude;
    if (native_roaming_acceleration &&
        magnitude<kRetailHerdingRoamingSpeedThreshold) {
        next_speed=static_cast<float>(
            magnitude+kRetailHerdingRoamingSpeedGain);
    }
    return RetailHerdingSteeringStep{
        new_x,new_y,
        static_cast<std::int32_t>(new_x),
        static_cast<std::int32_t>(new_y),
        facing,next_speed
    };
}

} // namespace btb::herding
