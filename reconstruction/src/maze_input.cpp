#include "btb/maze_input.hpp"

#include <cmath>
#include <cstdint>

namespace btb::maze {
namespace {

constexpr long double kRetailDegreesPerRadian =
    static_cast<long double>(57.2949981689453125f);
constexpr long double kRetailRadiansPerDegree =
    static_cast<long double>(0.017453530803322792f);
constexpr long double kVerticalRatioSentinel = 9999.0L;
constexpr long double kMouseDeadZone = 6.0L;
constexpr float kDirectionalComponentThreshold = 0.25f;

[[nodiscard]] long double integer_distance(
    Vec2i a,
    Vec2i b) noexcept {

    const auto dx =
        static_cast<long double>(a.x) -
        static_cast<long double>(b.x);
    const auto dy =
        static_cast<long double>(a.y) -
        static_cast<long double>(b.y);
    return std::sqrt(dx * dx + dy * dy);
}

[[nodiscard]] std::int32_t trunc_float_to_int(
    float value) noexcept {
    // 0x004304D0 temporarily selects x87 round-toward-zero before FISTP.
    return static_cast<std::int32_t>(value);
}

} // namespace

std::int32_t angle_between_integer_points_degrees(
    Vec2i from,
    Vec2i to) noexcept {

    const auto dx64 =
        static_cast<std::int64_t>(from.x) -
        static_cast<std::int64_t>(to.x);
    const auto dy64 =
        static_cast<std::int64_t>(from.y) -
        static_cast<std::int64_t>(to.y);

    const auto abs_dx =
        dx64 < 0 ? -static_cast<long double>(dx64)
                 :  static_cast<long double>(dx64);
    const auto abs_dy =
        dy64 < 0 ? -static_cast<long double>(dy64)
                 :  static_cast<long double>(dy64);

    const auto ratio =
        abs_dx == 0.0L
            ? kVerticalRatioSentinel
            : abs_dy / abs_dx;

    const auto base =
        std::atan(ratio) * kRetailDegreesPerRadian;

    long double angle{};
    if (dx64 < 0) {
        angle = dy64 < 0
            ? base + 90.0L
            : 90.0L - base;
    } else {
        angle = dy64 < 0
            ? 270.0L - base
            : base + 270.0L;
    }

    return static_cast<std::int32_t>(angle);
}

DirectionalIntent keyboard_direction_from_bits(
    std::uint8_t direction_bits) noexcept {

    DirectionalIntent result{};

    // Retail tests +X before -X, so 0x02 wins if both bits are set.
    if ((direction_bits & 0x02u) != 0) {
        result.x = 1;
    } else if ((direction_bits & 0x01u) != 0) {
        result.x = -1;
    }

    // Retail tests -Y before +Y, so 0x08 wins if both bits are set.
    if ((direction_bits & 0x08u) != 0) {
        result.y = -1;
    } else if ((direction_bits & 0x04u) != 0) {
        result.y = 1;
    }

    return result;
}

PlayerInputStep compute_player_input_direction(
    InputModeState& mode,
    DirectionalDebounce& debounce,
    const PlayerInputSample& sample) noexcept {

    const auto entry_mouse_mode = mode.mouse_mode;

    PlayerInputStep result;
    result.path =
        entry_mouse_mode ? InputPath::Mouse : InputPath::Keyboard;

    if (!entry_mouse_mode) {
        // Mouse movement requests mouse mode for the next call only.
        if (sample.mouse_x != mode.remembered_mouse_x ||
            sample.mouse_y != mode.remembered_mouse_y) {
            mode.mouse_mode = true;
            result.mode_changed_for_next_call = true;
        }

        mode.remembered_mouse_x = sample.mouse_x;
        mode.remembered_mouse_y = sample.mouse_y;

        result.intent =
            keyboard_direction_from_bits(sample.direction_bits);
        result.writes_node_capture_radius_four = true;
        return result;
    }

    // Any keyboard direction requests keyboard mode, again for the next call.
    if (sample.direction_bits != 0) {
        mode.mouse_mode = false;
        result.mode_changed_for_next_call = true;
    }

    const Vec2i player{
        trunc_float_to_int(sample.player_x),
        trunc_float_to_int(sample.player_y),
    };
    const Vec2i mouse{
        sample.mouse_x,
        sample.mouse_y,
    };

    if (integer_distance(player, mouse) <= kMouseDeadZone) {
        result.mouse_dead_zone = true;
        return result;
    }

    const auto angle =
        angle_between_integer_points_degrees(player, mouse);
    const auto radians =
        static_cast<long double>(angle) * kRetailRadiansPerDegree;

    // 0x0041AF60 explicitly stores FSIN/FCOS to 32-bit temporaries before the
    // abs/sign tests, so preserve that precision drop.
    const auto sin_component =
        static_cast<float>(std::sin(radians));
    const auto cos_component =
        static_cast<float>(std::cos(radians));

    DirectionalIntent raw{};

    if (std::fabs(sin_component) >
        kDirectionalComponentThreshold) {
        raw.x = sin_component >= 0.0f ? 1 : -1;
    }

    if (std::fabs(cos_component) >
        kDirectionalComponentThreshold) {
        raw.y = cos_component >= 0.0f ? 1 : -1;
    }

    result.intent = debounce.update(raw);
    result.debounce_applied = true;
    result.writes_node_capture_radius_four = true;
    return result;
}

} // namespace btb::maze
