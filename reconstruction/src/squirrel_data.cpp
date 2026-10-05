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

    for (auto& value : out.phase_header) {
        value = read_int(in, "7-int phase header");
    }

    for (auto& row : out.phase_table) {
        for (auto& value : row) {
            value = read_int(in, "8x7 phase table");
        }
    }

    out.animation_step_delay = read_int(in, "animation step delay");

    // Exact retail behavior at 0x00424E20: these are scanned into the same
    // stack temporary and discarded.
    out.discarded_scalars[0] = read_int(in, "discarded scalar 0");
    out.discarded_scalars[1] = read_int(in, "discarded scalar 1");

    for (auto& value : out.tuning_tuple) {
        value = read_int(in, "four-value tuning tuple");
    }

    out.alignment_offset = read_int(in, "alignment offset");
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
