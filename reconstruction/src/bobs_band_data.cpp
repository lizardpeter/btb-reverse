#include "btb/bobs_band_data.hpp"

#include <stdexcept>

namespace btb::bobs_band {

MachineData parse_machine_data(std::istream& input) {
    MachineData data;
    for (auto& p : data.machine_positions) {
        if (!(input >> p.x >> p.y)) {
            throw std::runtime_error("incomplete Bob's Band machine positions");
        }
    }
    for (auto& size : data.machine_sizes) {
        if (!(input >> size.width >> size.height) ||
            size.width <= 0 || size.height <= 0) {
            throw std::runtime_error("invalid Bob's Band machine sprite sizes");
        }
    }
    for (auto& p : data.conductor_positions) {
        if (!(input >> p.x >> p.y)) {
            throw std::runtime_error("incomplete Bob's Band conductor positions");
        }
    }
    for (auto& size : data.conductor_sizes) {
        if (!(input >> size.width >> size.height) ||
            size.width <= 0 || size.height <= 0) {
            throw std::runtime_error("invalid Bob's Band conductor sprite sizes");
        }
    }
    for (auto& frames : data.conductor_idle_frame_counts) {
        if (!(input >> frames) || frames <= 0) {
            throw std::runtime_error("invalid Bob's Band conductor frame count");
        }
    }
    for (auto& hit : data.machine_static_animation_rects) {
        if (!(input >> hit.left >> hit.top >> hit.right >> hit.bottom) ||
            hit.right <= hit.left || hit.bottom <= hit.top) {
            throw std::runtime_error("invalid Bob's Band static animation rectangle");
        }
    }

    // machinedata.txt ends with explanatory nonnumeric English comments.
    return data;
}

std::string sample_wav_path(int row, int type) {
    if (!valid_cell(row, 0) || !valid_machine(type)) {
        return {};
    }
    // The native table ranges from roley1_5.wav at pitch row zero down to
    // roley1_1.wav on the bottom row (and similarly for all ten types).
    return "Data\\SubGameOpen\\" +
        std::string(kWavStems[static_cast<std::size_t>(type)]) +
        "_" + std::to_string(kRows - row) + ".wav";
}

std::string sound_brick_bitmap_path(int type, bool hover) {
    if (!valid_machine(type)) {
        return {};
    }
    return std::string{"Data\\SubGameOpen\\"} +
        (hover ? "hsound" : "sound") +
        std::to_string(type+1) + ".bmp";
}

std::string sequence_filename(Conductor conductor, int player_index) {
    const int c = static_cast<int>(conductor);
    if (c < 0 || c >= kConductorCount ||
        player_index < 0 || player_index >= 5) {
        return {};
    }
    return std::string(
        kSequenceFileNames[static_cast<std::size_t>(c)]
                          [static_cast<std::size_t>(player_index)]);
}

} // namespace btb::bobs_band
