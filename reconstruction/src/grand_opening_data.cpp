#include "btb/grand_opening_data.hpp"

#include <stdexcept>
#include <string>

namespace btb::grand_opening {
namespace {

std::int32_t read_int(std::istream& in, const char* what) {
    std::int32_t value{};
    if (!(in >> value)) {
        throw std::runtime_error(
            std::string("short machinedata.txt while reading ") + what);
    }
    return value;
}

Vec2i read_vec2(std::istream& in, const char* what) {
    return {read_int(in, what), read_int(in, what)};
}

RectI read_rect(std::istream& in, const char* what) {
    return {
        read_int(in, what),
        read_int(in, what),
        read_int(in, what),
        read_int(in, what),
    };
}

} // namespace

MachineData parse_machine_data(std::istream& in) {
    MachineData out;

    for (auto& value : out.machine_positions) {
        value = read_vec2(in, "machine position");
    }
    for (auto& value : out.machine_sizes) {
        value = read_vec2(in, "machine size");
    }

    for (auto& value : out.parsed_but_unused_conductor_positions) {
        value = read_vec2(in, "conductor position");
    }
    for (auto& value : out.parsed_but_unused_conductor_sizes) {
        value = read_vec2(in, "conductor size");
    }

    for (auto& value : out.conductor_frame_counts) {
        value = read_int(in, "conductor frame count");
    }

    for (auto& value : out.static_animation_rects) {
        value = read_rect(in, "static animation rectangle");
    }

    return out;
}

} // namespace btb::grand_opening
