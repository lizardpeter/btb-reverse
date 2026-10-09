#pragma once

#include "btb/herding_source_roaming_recovery.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <optional>
#include <vector>

namespace btb::herding {

// Retail 0x417FCA..0x417FFC computes the SOURCE anchor from the
// current integer sprite position and half its signed cell extents:
// x + trunc((source_left - source_right)/2)
// y + trunc((source_top  - source_bottom)/2).
// This is NOT the ordinary (x+width/2,y+height/2) center.
// The original CDQ/SUB/SAR sequence implements truncation toward zero
// for the negative differences. Use wide signed intermediates here
// to avoid undefined C++ overflow on malformed external records.
[[nodiscard]] constexpr std::optional<Vec2i>
original_herding_recovery_anchor(
    const RetailEntityRecord32& e) noexcept {

    const std::int64_t shifted_x =
        static_cast<std::int64_t>(e.x) +
        (static_cast<std::int64_t>(e.source_left)-e.source_right)/2;
    const std::int64_t shifted_y =
        static_cast<std::int64_t>(e.y) +
        (static_cast<std::int64_t>(e.source_top)-e.source_bottom)/2;
    if (shifted_x<std::numeric_limits<std::int32_t>::min() ||
        shifted_x>std::numeric_limits<std::int32_t>::max() ||
        shifted_y<std::numeric_limits<std::int32_t>::min() ||
        shifted_y>std::numeric_limits<std::int32_t>::max()) {
        return std::nullopt;
    }
    return Vec2i{
        static_cast<std::int32_t>(shifted_x),
        static_cast<std::int32_t>(shifted_y)
    };
}

enum class OriginalRoamingLoopStatus {
    AlreadyInside,
    Accepted,
    // The original binary loops in the same update until inside.
    // This source adapter exposes a resumable work budget, but does
    // NOT treat budget exhaustion as successful completed movement.
    Pending,
    InvalidInput
};

struct OriginalRoamingLoop {
    RetailEntityRecord32 entity{};
    std::uint64_t attempts{};
    std::uint64_t consumed_random_calls{};
    bool started{};
    bool finished{};
    OriginalRoamingLoopStatus status{
        OriginalRoamingLoopStatus::Pending};
};

// Reconstructs 0x417F7C..0x418101 as a same-frame state machine.
// 0x417F7C/0x4180F1 test herd.txt GROUP TWO (the six-point
// exclusion polygon at 0x5104E8), NOT the group-zero navigation
// polygon. If inside (native PointInPolygon returns 0), exit.
// Otherwise 0x4180FB jumps BACK to 0x417FB2: restore current
// INTEGER X/Y from the SAVED previous_x/previous_y, derive the
// signed negative-half-sprite anchor using those restored integers,
// consume two NEW shared rand()%4 values, and apply 3-unit movement
// to the ACCUMULATING FLOAT X/Y. Each candidate is tested against
// group two until its integer position lies inside. The previous
// X/Y are not overwritten by any retry branch.
//
// With a malicious/invalid polygon the retail loop could run forever.
// A bounded chunk is a host execution-safety facility, not a retail
// decision threshold. The caller MUST resume before advancing any
// other game subsystem or touching the process-global RNG.
[[nodiscard]] inline OriginalRoamingLoopStatus
resume_original_herding_roaming_loop(
    OriginalRoamingLoop& state,
    retail::OriginalRetailRandom& shared_rng,
    const std::vector<Vec2i>& original_group2_polygon,
    std::size_t max_attempts_this_call) noexcept {

    if (state.finished) return state.status;
    if (original_group2_polygon.size()<3 ||
        max_attempts_this_call==0) {
        state.status=OriginalRoamingLoopStatus::InvalidInput;
        return state.status;
    }

    if (!state.started) {
        // C++ float-to-int overflow is undefined, unlike the original
        // x87 converter's exceptional result. Reject bad host state
        // before calling the native 32-bit polygon predicate.
        const auto in_int32_domain=[](float value) noexcept {
            return std::isfinite(value) &&
                static_cast<double>(value) >=
                    static_cast<double>(
                        std::numeric_limits<std::int32_t>::min()) &&
                static_cast<double>(value) <
                    static_cast<double>(
                        std::numeric_limits<std::int32_t>::max());
        };
        if (!in_int32_domain(state.entity.x_float) ||
            !in_int32_domain(state.entity.y_float)) {
            state.status=OriginalRoamingLoopStatus::InvalidInput;
            return state.status;
        }
        const auto inside=retail_geometry::original_polygon_contains(
            original_group2_polygon,
            static_cast<std::int32_t>(state.entity.x_float),
            static_cast<std::int32_t>(state.entity.y_float));
        if (!inside) {
            state.status=OriginalRoamingLoopStatus::InvalidInput;
            return state.status;
        }
        state.started=true;
        if (*inside) {
            state.status=OriginalRoamingLoopStatus::AlreadyInside;
            state.finished=true;
            return state.status;
        }
    }

    for (std::size_t i=0;i<max_attempts_this_call;++i) {
        auto candidate=state.entity;
        // Original 0x417FB2/0x417FBE copies saved previous INTO
        // current integer position, not current INTO previous.
        candidate.x=candidate.previous_x;
        candidate.y=candidate.previous_y;
        const auto anchor=original_herding_recovery_anchor(candidate);
        if (!anchor) {
            state.status=OriginalRoamingLoopStatus::InvalidInput;
            return state.status;
        }

        auto moved=original_herding_roaming_recovery_attempt(
            candidate,*anchor,shared_rng,original_group2_polygon);
        if (!moved) {
            state.status=OriginalRoamingLoopStatus::InvalidInput;
            return state.status;
        }

        state.entity=moved->moved;
        ++state.attempts;
        state.consumed_random_calls += moved->consumed_rand_calls;

        if (moved->accepted_inside_polygon) {
            state.status=OriginalRoamingLoopStatus::Accepted;
            state.finished=true;
            return state.status;
        }
    }
    state.status=OriginalRoamingLoopStatus::Pending;
    return state.status;
}

} // namespace btb::herding
