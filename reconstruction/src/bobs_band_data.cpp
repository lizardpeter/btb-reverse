#include "btb/bobs_band_data.hpp"

#include <stdexcept>

namespace btb::bobs_band {

MachineData parse_machine_data(std::istream& in) {
    MachineData out;

    for (auto& v : out.machine_positions) {
        if (!(in >> v.x >> v.y)) throw std::runtime_error("machine position");
    }
    for (auto& v : out.machine_sizes) {
        if (!(in >> v.x >> v.y)) throw std::runtime_error("machine size");
    }
    for (auto& v : out.source_conductor_positions) {
        if (!(in >> v.x >> v.y)) throw std::runtime_error("conductor position");
    }
    for (auto& v : out.source_conductor_sizes) {
        if (!(in >> v.x >> v.y)) throw std::runtime_error("conductor size");
    }
    for (auto& v : out.conductor_frame_counts) {
        if (!(in >> v)) throw std::runtime_error("conductor frames");
    }
    for (auto& v : out.sound_regions) {
        if (!(in >> v.a >> v.b >> v.c >> v.d)) {
            throw std::runtime_error("sound region");
        }
    }

    return out;
}

} // namespace btb::bobs_band
