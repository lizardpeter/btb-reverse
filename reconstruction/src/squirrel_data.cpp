#include "btb/squirrel_data.hpp"

#include <stdexcept>
#include <string>

namespace btb::squirrel {
namespace {

std::int32_t read_int(std::istream& in, const char* what) {
    std::int32_t value{};
    if (!(in >> value)) {
        throw std::runtime_error(std::string("short sqdata.txt while reading ") + what);
    }
    return value;
}

} // namespace

Data parse_retail_data(std::istream& in) {
    Data out;

    for (auto& value : out.horizontal_motion_deltas) {
        value = read_int(in, "7 horizontal motion deltas");
    }

    for (auto& row : out.vertical_motion_deltas) {
        for (auto& value : row) {
            value = read_int(in, "8x7 vertical motion profiles");
        }
    }

    out.animation_step_delay = read_int(in, "animation step delay");

    // Exact retail behavior at 0x00424E20: these are scanned into the same
    // stack temporary and discarded.
    out.discarded_scalars[0] = read_int(in, "discarded scalar 0");
    out.discarded_scalars[1] = read_int(in, "discarded scalar 1");

    for (auto& value : out.loaded_unused_scalars) {
        value = read_int(in, "four loaded-but-unused scalars");
    }

    out.initial_horizontal_alignment_offset =
        read_int(in, "initial horizontal alignment offset");
    return out;
}

std::vector<Vec2i> read_unconsumed_legacy_pairs(std::istream& in) {
    std::vector<Vec2i> out;
    Vec2i point{};
    while (in >> point.x >> point.y) {
        out.push_back(point);
    }
    return out;
}

} // namespace btb::squirrel
