#include "btb/spud_skate_data.hpp"

#include <stdexcept>

namespace btb::spud_skate {

TimingData parse_timing(std::istream& in) {
    TimingData out;
    if (!(in >> out.stunt_count)) {
        throw std::runtime_error("missing Spud Skate stunt count");
    }
    if (out.stunt_count < 0 ||
        out.stunt_count > static_cast<std::int32_t>(kStuntCount)) {
        throw std::runtime_error("unsupported Spud Skate stunt count");
    }

    for (std::int32_t i = 0; i < out.stunt_count; ++i) {
        auto& w = out.windows[static_cast<std::size_t>(i)];
        if (!(in >> w.normal_start >> w.okay_start
                 >> w.good_start >> w.end_exclusive)) {
            throw std::runtime_error("short Spud Skate stunt-window table");
        }
        if (!(w.normal_start <= w.okay_start &&
              w.okay_start <= w.good_start &&
              w.good_start < w.end_exclusive)) {
            throw std::runtime_error("invalid Spud Skate stunt-window ordering");
        }
    }

    for (std::int32_t i = 0; i < out.stunt_count; ++i) {
        if (!(in >> out.return_to_bad_frames[static_cast<std::size_t>(i)])) {
            throw std::runtime_error("short Spud Skate return-frame table");
        }
    }

    if (!(in >> out.loop_start_frame >> out.loop_end_frame)) {
        throw std::runtime_error("missing Spud Skate loop frame pair");
    }

    return out;
}

SoundData parse_sound_info(std::istream& in) {
    SoundData out;

    for (std::size_t stunt = 0; stunt < kStuntCount; ++stunt) {
        for (std::size_t quality = 0; quality < kQualityCount; ++quality) {
            for (std::size_t candidate = 0;
                 candidate < kSoundsPerQuality;
                 ++candidate) {
                if (!(in >> out.sound_ids[stunt][quality][candidate])) {
                    throw std::runtime_error("short Spud Skate sound matrix");
                }
            }
        }
    }

    for (auto& frame : out.trigger_frames) {
        if (!(in >> frame)) {
            throw std::runtime_error("short Spud Skate sound-trigger frame table");
        }
    }

    return out;
}

} // namespace btb::spud_skate
