#include "btb/fireworks_editor.hpp"

#include <algorithm>
#include <cmath>

namespace btb::fireworks {
namespace {

[[nodiscard]] constexpr bool strict_hit(
    const Recti& rect,
    std::int32_t x,
    std::int32_t y) noexcept {
    return x > rect.left && x < rect.right &&
           y > rect.top && y < rect.bottom;
}

[[nodiscard]] constexpr Recti control_rect(EditorAction action) noexcept {
    switch (action) {
    case EditorAction::Play:
        return {292, 416, 346, 472};
    case EditorAction::DeleteAll:
        return {102, 416, 156, 472};
    case EditorAction::DeleteSelected:
        return {483, 416, 537, 472};
    default:
        return {};
    }
}

[[nodiscard]] constexpr std::int32_t action_id(EditorAction action) noexcept {
    return static_cast<std::int32_t>(action);
}

[[nodiscard]] const std::array<ActorFollowPoint, 4>& actor_follow_path(
    PlacementActorChannel channel) noexcept {
    return channel == PlacementActorChannel::Bob
        ? kBobFollowPath
        : kWendyFollowPath;
}

struct ActorPathEvaluation {
    std::int32_t top_left_y{};
    std::int32_t vertical_direction_code{};
};

[[nodiscard]] ActorPathEvaluation evaluate_actor_follow_path(
    PlacementActorChannel channel,
    std::int32_t actor_x) noexcept {

    const auto& path = actor_follow_path(channel);
    const auto center_x = actor_x + kActorSpriteHalfSize;

    // Retail scans the upper X breakpoints using strict center_x < breakpoint.
    std::size_t upper = 1;
    while (upper + 1 < path.size() && center_x >= path[upper].x) {
        ++upper;
    }
    const auto lower = upper - 1;

    const auto& p0 = path[lower];
    const auto& p1 = path[upper];
    const auto span_x = p1.x - p0.x;
    const auto retained_numerator = p0.y - p1.y;

    // The original keeps this interpolation on the x87 stack and converts with
    // truncate-toward-zero. long double preserves that old x87-style precision
    // closely on the reconstruction host.
    const long double retained_slope =
        static_cast<long double>(retained_numerator) /
        static_cast<long double>(span_x);
    const long double path_y =
        static_cast<long double>(p0.y) -
        static_cast<long double>(center_x - p0.x) * retained_slope;

    ActorPathEvaluation result;
    result.top_left_y =
        static_cast<std::int32_t>(path_y) - kActorSpriteSize;
    result.vertical_direction_code =
        retained_numerator < 0 ? 1 :
        retained_numerator > 0 ? -1 : 0;
    return result;
}

constexpr void tick_walking_actor_animation(
    PlacementActorVisual& visual) noexcept {

    ++visual.frame_tick;
    if (visual.frame_tick <= 5) {
        return;
    }

    visual.frame_tick = 0;
    ++visual.source_row;

    if (visual.source_row >= kDormantMotionRowEndExclusive) {
        visual.source_row = kDormantMotionFirstRow;
    }
    if (visual.source_row <= kDormantMotionFirstRow) {
        visual.source_row = kDormantMotionFirstRow;
    }
}

[[nodiscard]] std::int32_t retail_actor_angle_degrees(
    std::int32_t x,
    std::int32_t y,
    std::int32_t target_x,
    std::int32_t target_y) noexcept {

    const auto xdiff = x - target_x;
    const auto ydiff = y - target_y;

    const auto abs_x = std::abs(xdiff);
    const auto abs_y = std::abs(ydiff);

    // 0x00415D70 substitutes 9999.0 when abs(xdiff)==0, then uses atan.
    const float ratio =
        abs_x == 0
            ? 9999.0F
            : static_cast<float>(abs_y) / static_cast<float>(abs_x);

    float angle = std::atan(ratio) * 57.29499816894531F;

    if (xdiff < 0) {
        angle = ydiff < 0 ? angle + 90.0F : 90.0F - angle;
    } else {
        angle = ydiff < 0 ? 270.0F - angle : angle + 270.0F;
    }

    // The retail helper at 0x004304D0 temporarily changes the x87 rounding
    // mode to truncate toward zero before returning the integer angle.
    return static_cast<std::int32_t>(angle);
}

} // namespace

ActorMouseTrackingStep update_actor_mouse_tracking(
    EditorRuntimeState& state,
    std::int32_t mouse_x,
    std::int32_t mouse_y) noexcept {

    ActorMouseTrackingStep result;

    const bool palette_latched =
        state.actor_states[0] == PlacementActorState::PaletteLatched ||
        state.actor_states[1] == PlacementActorState::PaletteLatched;

    result.split_y = palette_latched
        ? kActorMouseSplitYWhenPaletteLatched
        : kActorMouseSplitY;

    // Retail chooses the idle actor first. Mouse below the split means Wendy
    // idles and Bob tracks; mouse above the split means Bob idles and Wendy
    // tracks.
    if (mouse_y > result.split_y) {
        result.idle_actor = PlacementActorChannel::Wendy;
        result.tracking_actor = PlacementActorChannel::Bob;
    } else {
        result.idle_actor = PlacementActorChannel::Bob;
        result.tracking_actor = PlacementActorChannel::Wendy;
    }

    tick_idle_actor_animation(
        state.actor_visuals[placement_actor_index(result.idle_actor)]);

    result.clamped_mouse_x =
        std::clamp(mouse_x, kActorMouseMinX, kActorMouseMaxX);

    auto& tracking =
        state.actor_visuals[placement_actor_index(result.tracking_actor)];

    const auto path =
        evaluate_actor_follow_path(result.tracking_actor, tracking.x);
    tracking.y = path.top_left_y;
    result.interpolated_path_y = tracking.y;
    result.vertical_direction_code = path.vertical_direction_code;

    const auto center_delta =
        tracking.x - result.clamped_mouse_x + kActorSpriteHalfSize;
    const auto absolute_delta = std::abs(center_delta);
    const auto stop_threshold =
        kActorFollowBaseThreshold - state.actor_follow_threshold_bias;

    if (absolute_delta < stop_threshold) {
        state.actor_follow_threshold_bias = 0;
        tick_idle_actor_animation(tracking);
        result.standing = true;
        return result;
    }

    state.actor_follow_threshold_bias = kActorFollowTightThresholdBias;
    const auto target_x =
        result.clamped_mouse_x - kActorSpriteHalfSize;

    if (tracking.x < target_x) {
        ++tracking.x;
        tracking.source_column = path.vertical_direction_code + 2;
        result.moved_right = true;
    } else if (tracking.x > target_x) {
        --tracking.x;
        tracking.source_column = path.vertical_direction_code + 6;
        result.moved_left = true;
    }

    tick_walking_actor_animation(tracking);
    return result;
}

DormantMotionStep tick_dormant_legacy_motion(
    EditorRuntimeState& state,
    PlacementActorChannel channel) noexcept {

    DormantMotionStep result;
    const auto index = placement_actor_index(channel);
    if (state.actor_states[index] != PlacementActorState::DormantLegacyMotion) {
        return result;
    }

    result.active = true;
    auto& visual = state.actor_visuals[index];

    ++visual.frame_tick;
    if (visual.frame_tick > 5) {
        visual.frame_tick = 0;
        ++visual.source_row;
        if (visual.source_row >= kDormantMotionRowEndExclusive) {
            visual.source_row = kDormantMotionFirstRow;
        }
    }

    const auto [target_x, target_y] = kDormantPlacementActorTargets[index];
    result.angle_degrees = retail_actor_angle_degrees(
        visual.x, visual.y, target_x, target_y);

    auto direction = (result.angle_degrees + 22) / 45;
    if (direction >= 8) {
        direction -= 8;
    }
    visual.source_column = direction;

    constexpr float kDegreesToRadians = 0.017453530803322792F;
    const float radians =
        static_cast<float>(result.angle_degrees) * kDegreesToRadians;

    result.delta_x = static_cast<std::int32_t>(
        std::sin(radians) * static_cast<float>(kDormantMotionSpeed));
    result.delta_y = static_cast<std::int32_t>(
        std::cos(radians) * static_cast<float>(-kDormantMotionSpeed));

    visual.x += result.delta_x;
    visual.y += result.delta_y;

    const auto dx = visual.x - target_x;
    const auto dy = visual.y - target_y;
    result.distance_after_move = std::sqrt(
        static_cast<float>(dx * dx + dy * dy));

    if (result.distance_after_move < kDormantMotionArrivalDistance) {
        state.actor_states[index] = PlacementActorState::Idle;
        visual.source_column = 4;
        result.arrived = true;
    }

    return result;
}

PlacementBeginResult begin_placement_region_action(
    Sequence& sequence,
    std::size_t row,
    std::size_t column) noexcept {

    PlacementBeginResult result;
    const auto existing = sequence.at(row, column);
    if (!existing) {
        return result;
    }

    result.had_existing_start = true;
    result.removed_type = sequence.remove(row, column);
    return result;
}

PlacementReleaseResult complete_placement_release(
    EditorRuntimeState& state,
    const Sequence& sequence,
    std::size_t row,
    std::size_t column,
    std::int32_t random_mod_4) noexcept {

    PlacementReleaseResult result;
    state.cursor = {};

    if (!sequence.in_bounds(row, column)) {
        return result;
    }

    const auto existing = sequence.at(row, column);
    if (existing) {
        // 0x00412D9C..0x00412DA3 rejects a cell containing a start type 0..11.
        return result;
    }

    result.attempted = true;
    result.channel = placement_actor_for_row(row);

    const auto index = placement_actor_index(result.channel);
    state.pending[index] = {row, column};
    state.actor_states[index] = PlacementActorState::PlacementQueued;
    result.sound_id = placement_voice_sound_id(result.channel, random_mod_4);

    result.valid = sequence.validate_placement(
        row, column, state.selected_type);
    if (result.valid) {
        state.internal_state = InternalState::Editor;
        state.cursor = {};
    }

    return result;
}

PlacementActorTickResult tick_placement_actor(
    EditorRuntimeState& state,
    Sequence& sequence,
    PlacementActorChannel channel) {

    PlacementActorTickResult result;
    const auto index = placement_actor_index(channel);
    auto& actor_state = state.actor_states[index];

    if (actor_state == PlacementActorState::PlacementQueued) {
        actor_state = PlacementActorState::PlacementCommit;
        result.queued_to_commit = true;
        return result;
    }

    if (actor_state != PlacementActorState::PlacementCommit) {
        return result;
    }

    result.commit_attempted = true;
    const auto target = state.pending[index];
    result.committed = sequence.commit_placement(
        target.row,
        target.column,
        state.selected_type);

    // Retail clears the channel unconditionally after the commit call.
    actor_state = PlacementActorState::Idle;

    result.grid_full = sequence.full();
    if (result.grid_full) {
        result.sound_id = full_grid_voice_sound_id(channel);
    }

    return result;
}

EditorControlOutput update_editor_controls(
    EditorRuntimeState& state,
    const EditorControlInput& input) noexcept {

    EditorControlOutput output;

    if (state.control_cooldown > 0) {
        --state.control_cooldown;
        return output;
    }

    const auto [x, y] =
        retail_control_hit_point(state, input.mouse_x, input.mouse_y);

    // The table order is Play, Delete All, Delete Selected. Their rectangles
    // do not overlap, so this preserves retail selection without ambiguity.
    constexpr std::array controls{
        EditorAction::Play,
        EditorAction::DeleteAll,
        EditorAction::DeleteSelected,
    };

    std::optional<EditorAction> hit;
    for (const auto control : controls) {
        if (strict_hit(control_rect(control), x, y)) {
            hit = control;
            break;
        }
    }

    if (!hit) {
        // Retail does not reset 0x00442A30 when the pointer leaves all controls.
        return output;
    }

    output.control = *hit;
    const auto previous = state.previous_control_action;

    if (*hit == EditorAction::Play) {
        // Play is completely ignored while either actor channel is nonzero.
        if (state.actor_states[0] != PlacementActorState::Idle ||
            state.actor_states[1] != PlacementActorState::Idle) {
            return output;
        }

        if (input.click_active &&
            state.internal_state == InternalState::Editor) {
            state.internal_state = InternalState::PreShowMovieSetup;
            state.previous_control_action = action_id(*hit);
            output.action = EditorControlActionKind::EnterPreShow;
            output.sound_id = kPlayClickSoundId;
            return output;
        }

        if (input.pressed_visual) {
            state.previous_control_action = action_id(*hit);
            output.visual = EditorControlVisual::Pressed;
            return output;
        }

        state.previous_control_action = action_id(*hit);
        output.visual = EditorControlVisual::Hover;
        if (previous != action_id(*hit)) {
            output.sound_id = kPlayHoverSoundId;
        }
        return output;
    }

    if (*hit == EditorAction::DeleteSelected) {
        // 0x00413CDA checks specifically for actor state == 1 when the shared
        // interaction gate is active, not for all positive actor states.
        if (input.delete_interaction_gate &&
            (state.actor_states[0] == PlacementActorState::PaletteLatched ||
             state.actor_states[1] == PlacementActorState::PaletteLatched)) {
            state.actor_states[0] = PlacementActorState::Idle;
            state.actor_states[1] = PlacementActorState::Idle;
            state.internal_state = InternalState::Editor;
            state.cursor = {};
            state.control_cooldown = kConflictCooldownFrames;
            output.action =
                EditorControlActionKind::CancelConflictingInteraction;
            return output;
        }

        if (input.click_active) {
            if (state.internal_state == InternalState::DeleteSelected) {
                state.internal_state = InternalState::Editor;
                state.cursor = {};
                state.previous_control_action = action_id(*hit);
                output.action = EditorControlActionKind::LeaveDeleteMode;
                return output;
            }

            state.internal_state = InternalState::DeleteSelected;
            state.cursor = {EditorCursorKind::Delete, std::nullopt};
            state.previous_control_action = action_id(*hit);
            output.action = EditorControlActionKind::EnterDeleteMode;
            output.sound_id = kDeleteClickSoundId;
            return output;
        }

        if (input.pressed_visual) {
            state.previous_control_action = action_id(*hit);
            output.visual = EditorControlVisual::Pressed;
            return output;
        }

        state.previous_control_action = action_id(*hit);
        output.visual = EditorControlVisual::Hover;
        if (previous != action_id(*hit)) {
            output.sound_id = kDeleteHoverSoundId;
        }
        return output;
    }

    // Delete All.
    if (input.click_active &&
        state.internal_state == InternalState::Editor) {
        state.previous_control_action = action_id(*hit);
        output.action = EditorControlActionKind::OpenDeleteAllConfirmation;
        output.sound_id = kDeleteAllClickSoundId;
        output.open_yes_no_confirmation = true;
        output.confirmation_context = kDeleteAllConfirmationContext;
        return output;
    }

    if (input.pressed_visual) {
        state.previous_control_action = action_id(*hit);
        output.visual = EditorControlVisual::Pressed;
        return output;
    }

    state.previous_control_action = action_id(*hit);
    output.visual = EditorControlVisual::Hover;
    if (previous != action_id(*hit)) {
        output.sound_id = kDeleteAllHoverSoundId;
    }
    return output;
}

} // namespace btb::fireworks
