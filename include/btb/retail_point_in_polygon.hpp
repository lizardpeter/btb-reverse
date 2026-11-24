#pragma once

#include "btb/herding_data.hpp"

#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace btb::retail_geometry {

// Literal translation of 0x00428520, the original shared PE32 x86
// PointInPolygon function (checked against the 311296-byte retail exe).
//
// Return convention is NONSTANDARD:
//   0 = odd crossing count = inside
//   1 = even crossing count = outside
// The original game tests eax after the call; callers must not assume
// nonzero means inside. The source function also preserves boundary
// asymmetry (pointY>minY && pointY<=maxY, pointX<=intersection),
// signed integer division and 32-bit intermediate multiplication.
//
// std::nullopt represents an x86 IDIV fault / unsupported operand size
// rather than fabricating a deterministic inside/outside result.
[[nodiscard]] inline std::optional<std::int32_t>
original_point_in_polygon_status(
    const std::vector<herding::Vec2i>& points,
    std::int32_t point_x,
    std::int32_t point_y) noexcept {

    if (points.empty()) {
        return 1;
    }
    auto current=points.front();
    std::uint32_t intersections=0;

    // Retail starts at point 0, iterates indices 1..N inclusive and
    // wraps the endpoint by signed IDIV modulo the vertex count.
    for (std::size_t i=1;i<=points.size();++i) {
        const auto next=points[i % points.size()];
        const auto min_y=std::min(current.y,next.y);
        const auto max_y=std::max(current.y,next.y);
        const auto max_x=std::max(current.x,next.x);

        if (point_y>min_y && point_y<=max_y &&
            point_x<=max_x && current.y!=next.y) {

            bool crossing=false;
            if (current.x==next.x) {
                crossing=true;
            } else {
                // x86's SUB and IMUL wrap at 32 bits; IDIV uses signed
                // division truncating toward zero (not float rounding).
                const auto wrap_sub=[](
                    std::int32_t a,std::int32_t b) -> std::int32_t {
                    return std::bit_cast<std::int32_t>(
                        static_cast<std::uint32_t>(a) -
                        static_cast<std::uint32_t>(b));
                };
                const auto dx=wrap_sub(next.x,current.x);
                const auto dy=wrap_sub(point_y,current.y);
                const auto denom=wrap_sub(next.y,current.y);
                const auto product=std::bit_cast<std::int32_t>(
                    static_cast<std::uint32_t>(dx) *
                    static_cast<std::uint32_t>(dy));
                if (denom==0 ||
                    (product==std::numeric_limits<std::int32_t>::min() &&
                     denom==-1)) {
                    return std::nullopt;
                }
                const auto offset=product/denom;
                const auto crossing_x=std::bit_cast<std::int32_t>(
                    static_cast<std::uint32_t>(current.x) +
                    static_cast<std::uint32_t>(offset));
                crossing = point_x <= crossing_x;
            }
            if (crossing) ++intersections;
        }
        current=next;
    }

    return (intersections & 1U)==0U ? 1 : 0;
}

[[nodiscard]] inline std::optional<bool> original_polygon_contains(
    const std::vector<herding::Vec2i>& points,
    std::int32_t point_x,
    std::int32_t point_y) noexcept {
    const auto original=original_point_in_polygon_status(
        points,point_x,point_y);
    return original ? std::optional<bool>{*original==0}
                    : std::nullopt;
}

} // namespace btb::retail_geometry
