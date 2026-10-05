#include "btb/golf_data.hpp"

#include <stdexcept>

namespace btb::golf {
namespace {

Vec2i read_vec2(std::istream& in, const char* what) {
    Vec2i out{};
    if (!(in >> out.x >> out.y)) {
        throw std::runtime_error(std::string("could not read Golf ") + what);
    }
    return out;
}

std::string read_path(std::istream& in, const char* what) {
    std::string out;
    if (!(in >> out)) {
        throw std::runtime_error(std::string("could not read Golf ") + what);
    }
    return out;
}

std::int32_t read_int(std::istream& in, const char* what) {
    std::int32_t out{};
    if (!(in >> out)) {
        throw std::runtime_error(std::string("could not read Golf ") + what);
    }
    return out;
}

} // namespace

Data parse_data(std::istream& in) {
    Data out;

    out.bob_position = read_vec2(in, "Bob position");
    out.bob_sprite_path = read_path(in, "Bob sprite path");
    out.bob_rotation_count = read_int(in, "Bob rotation count");
    out.bob_frame_size_from_file = read_vec2(in, "Bob frame size");
    out.bob_frame_count = read_int(in, "Bob frame count");
    out.ball_offset_from_bob_from_file = read_vec2(in, "ball offset");

    // Exact post-parse correction in LoadGolfData @ 0x004159F0.
    out.ball_offset_from_bob_retail = {
        out.ball_offset_from_bob_from_file.x - 2,
        out.ball_offset_from_bob_from_file.y - 5,
    };

    for (auto& object : out.course_objects) {
        object.position = read_vec2(in, "course object position");
        object.sprite_path = read_path(in, "course object sprite");
        object.hot_area_position = read_vec2(in, "course object hot area");
        object.frame_size = read_vec2(in, "course object frame size");
        object.frame_count = read_int(in, "course object frame count");
        object.animation_repeats = read_int(in, "course object repeat count");
    }

    for (auto& spectator : out.spectators) {
        spectator.sprite_path = read_path(in, "spectator sprite");
        spectator.position = read_vec2(in, "spectator position");
        spectator.frame_size = read_vec2(in, "spectator frame size");
        spectator.frame_count = read_int(in, "spectator frame count");
    }

    for (auto& value : out.attempts_by_difficulty) {
        value = read_int(in, "difficulty attempt count");
    }
    for (auto& value : out.power_bar_speed_by_difficulty) {
        value = read_int(in, "difficulty power-bar speed");
    }

    out.ball_frame_size = read_vec2(in, "ball frame size");
    out.ball_frame_count = read_int(in, "ball frame count");
    out.trailing_point = read_vec2(in, "trailing point");

    // Exact retail behavior: one final fscanf("%d %d") is attempted.
    // The shipped golfdata.txt reaches its //Comments section here, so this
    // normally fails and leaves the destination globals unchanged.
    Vec2i optional{};
    if (in >> optional.x >> optional.y) {
        out.final_optional_pair_was_present = true;
        out.final_optional_pair = optional;
    } else {
        in.clear();
    }

    return out;
}

} // namespace btb::golf
