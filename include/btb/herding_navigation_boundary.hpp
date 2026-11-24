#pragma once

#include "btb/retail_point_in_polygon.hpp"

#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace btb::herding {

// Retail outer UpdateHerdingActivity calls PointInPolygon (0x428520)
// at 0x418C54 / 0x418C87 / 0x418CC0. A new position outside the
// source navigation polygon is corrected first by attempting
// (previousX,currentY), then (currentX,previousY), finally restoring
// both axes. A separate original runtime condition controls whether
// to attempt axis-specific recovery; the caller supplies that condition
// rather than this function inventing its meaning.
//
// Float->integer tests are performed by runtime helper 0x4304D0:
// x87 FISTP with RC=11 (truncate toward zero).
// Verified at 0x418C65..0x418C74:
// [esp+0x10] counts accepted directional input axes, and 0x443A9C
// preserves the native mouse-navigation mode. On leaving the polygon,
// the retail branch attempts independent axis recovery only when
// direction_count>1 OR the mouse mode value is nonzero.
[[nodiscard]] constexpr bool retail_herding_axis_recovery_enabled(
    std::int32_t directional_axes,
    std::int32_t mouse_navigation_mode_443a9c) noexcept {
    return directional_axes>1 || mouse_navigation_mode_443a9c!=0;
}

enum class RetailHerdingBoundaryChoice {
    ValidNewPosition,
    RestorePreviousX,
    RestorePreviousY,
    RestoreBothAxes
};

struct RetailHerdingBoundaryStep {
    float x{};
    float y{};
    RetailHerdingBoundaryChoice choice{};
    std::int32_t polygon_tests{};
};

[[nodiscard]] inline std::optional<RetailHerdingBoundaryStep>
retail_herding_navigation_boundary_step(
    const std::vector<Vec2i>& original_nav_vertices,
    float attempted_x,
    float attempted_y,
    std::int32_t previous_x,
    std::int32_t previous_y,
    bool source_allows_partial_axis_recovery) noexcept {

    if (!std::isfinite(attempted_x) ||
        !std::isfinite(attempted_y) ||
        attempted_x <= static_cast<float>(
            std::numeric_limits<std::int32_t>::min()) ||
        attempted_x >= static_cast<float>(
            std::numeric_limits<std::int32_t>::max()) ||
        attempted_y <= static_cast<float>(
            std::numeric_limits<std::int32_t>::min()) ||
        attempted_y >= static_cast<float>(
            std::numeric_limits<std::int32_t>::max())) {
        return std::nullopt;
    }
    const auto tested_x=static_cast<std::int32_t>(attempted_x);
    const auto tested_y=static_cast<std::int32_t>(attempted_y);
    std::int32_t tests=0;
    const auto inside=[&](std::int32_t x,std::int32_t y)
        -> std::optional<bool> {
        ++tests;
        return retail_geometry::original_polygon_contains(
            original_nav_vertices,x,y);
    };

    const auto new_position=inside(tested_x,tested_y);
    if (!new_position) return std::nullopt;
    if (*new_position) {
        return RetailHerdingBoundaryStep{
            attempted_x,attempted_y,
            RetailHerdingBoundaryChoice::ValidNewPosition,tests};
    }

    if (source_allows_partial_axis_recovery) {
        // Native tests (previousX,currentY) first, with the first
        // coordinate reconstructed using FILD from the prior integer.
        const auto x_restored=inside(previous_x,tested_y);
        if (!x_restored) return std::nullopt;
        if (*x_restored) {
            return RetailHerdingBoundaryStep{
                static_cast<float>(previous_x),attempted_y,
                RetailHerdingBoundaryChoice::RestorePreviousX,tests};
        }

        const auto y_restored=inside(tested_x,previous_y);
        if (!y_restored) return std::nullopt;
        if (*y_restored) {
            return RetailHerdingBoundaryStep{
                attempted_x,static_cast<float>(previous_y),
                RetailHerdingBoundaryChoice::RestorePreviousY,tests};
        }
    }
    return RetailHerdingBoundaryStep{
        static_cast<float>(previous_x),
        static_cast<float>(previous_y),
        RetailHerdingBoundaryChoice::RestoreBothAxes,tests};
}

} // namespace btb::herding
