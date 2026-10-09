#pragma once

#include "btb/herding_source_roaming_recovery.hpp"

#include <algorithm>
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
// For each rejected attempt the binary jumps BACK to 0x417FB2,
// copies current integer x/y into previous_x/y, RECOMPUTES the
// signed-half-width/height anchor, consumes two NEW shared rand()%4
// calls, and applies 3-unit movement to the CURRENT FLOAT x/y.
// There is no saved list of targets and no reset to the initial float
// position between retries.
//
// With a malicious/invalid polygon the retail loop could run forever.
// A bounded chunk is a host execution-safety facility, not a retail
// decision threshold. The caller MUST resume before advancing any
// other game subsystem or touching the process-global RNG.
[[nodiscard]] inline OriginalRoamingLoopStatus
resume_original_herding_roaming_loop(
    OriginalRoamingLoop& state,
    retail::OriginalRetailRandom& shared_rng,
    const std::vector<Vec2i>& transformed_navigation_polygon,
    std::size_t max_attempts_this_call) noexcept {

    if (state.finished) return state.status;
    if (transformed_navigation_polygon.size()<3 ||
        max_attempts_this_call==0) {
        state.status=OriginalRoamingLoopStatus::InvalidInput;
        return state.status;
    }

    if (!state.started) {
        const auto inside=retail_geometry::original_polygon_contains(
            transformed_navigation_polygon,
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
        candidate.previous_x=candidate.x;
        candidate.previous_y=candidate.y;
        const auto anchor=original_herding_recovery_anchor(candidate);
        if (!anchor) {
            state.status=OriginalRoamingLoopStatus::InvalidInput;
            return state.status;
        }

        auto moved=original_herding_roaming_recovery_attempt(
            candidate,*anchor,shared_rng,transformed_navigation_polygon);
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
