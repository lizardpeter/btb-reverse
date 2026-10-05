#pragma once

#include <array>
#include <cstdint>
#include <istream>
#include <string>

namespace btb::golf {

struct Vec2i {
    std::int32_t x{};
    std::int32_t y{};
    friend bool operator==(const Vec2i&, const Vec2i&) = default;
};

struct AnimatedCourseObject {
    Vec2i position{};
    std::string sprite_path;
    Vec2i hot_area_position{};
    Vec2i frame_size{};
    std::int32_t frame_count{};
    std::int32_t animation_repeats{};
};

struct Spectator {
    std::string sprite_path;
    Vec2i position{};
    Vec2i frame_size{};
    std::int32_t frame_count{};
};

struct Data {
    Vec2i bob_position{};
    std::string bob_sprite_path;
    std::int32_t bob_rotation_count{};
    Vec2i bob_frame_size_from_file{};
    Vec2i bob_frame_size_retail{133, 174};
    std::int32_t bob_frame_count{};
    Vec2i ball_offset_from_bob_from_file{};
    Vec2i ball_offset_from_bob_retail{};

    std::array<AnimatedCourseObject, 3> course_objects{};
    std::array<Spectator, 3> spectators{};

    std::array<std::int32_t, 3> attempts_by_difficulty{};
    std::array<std::int32_t, 3> power_bar_speed_by_difficulty{};

    Vec2i ball_frame_size{};
    std::int32_t ball_frame_count{};

    // The shipped file contains one undocumented pair after the ball data.
    Vec2i trailing_point{};

    // Retail performs one more "%d %d" fscanf after trailing_point. In the
    // shipped file the next token is "//Comments", so that scan fails.
    bool final_optional_pair_was_present{false};
    Vec2i final_optional_pair{};

    [[nodiscard]] Vec2i initial_ball_position() const noexcept {
        return {
            bob_position.x + ball_offset_from_bob_retail.x,
            bob_position.y + ball_offset_from_bob_retail.y,
        };
    }
};

Data parse_data(std::istream& in);

} // namespace btb::golf
