#include "btb/spud_skate_data.hpp"

#include <cassert>
#include <sstream>

using namespace btb::spud_skate;

int main() {
    std::istringstream timing(R"(
8
85 97 107 116
134 146 156 165
222 234 244 253
290 302 312 322
376 388 399 409
461 473 483 492
543 555 566 575
631 643 653 661
134
202
271
360
426
531
592
679
44 660
)");

    const auto data = parse_timing(timing);
    assert(data.stunt_count == 8);
    assert(data.windows[0].contains(85));
    assert(data.windows[0].contains(115));
    assert(!data.windows[0].contains(116));
    assert(data.windows[0].quality_for_press(85) == StuntQuality::Normal);
    assert(data.windows[0].quality_for_press(97) == StuntQuality::Okay);
    assert(data.windows[0].quality_for_press(107) == StuntQuality::Good);
    assert(data.return_to_bad_frames[0] == 134);
    assert(data.return_to_bad_frames[7] == 679);
    assert(data.loop_start_frame == 44);
    assert(data.loop_end_frame == 660);

    assert(stunt_at_frame(data, 90) == 0);
    assert(stunt_at_frame(data, 150) == 1);
    assert(!stunt_at_frame(data, 200).has_value());

    // Minimal synthetic soundinfo corpus with exact retail dimensions.
    std::ostringstream src;
    int value = 1000;
    for (std::size_t stunt = 0; stunt < kStuntCount; ++stunt) {
        for (std::size_t quality = 0; quality < kQualityCount; ++quality) {
            for (std::size_t candidate = 0;
                 candidate < kSoundsPerQuality;
                 ++candidate) {
                src << value++ << ' ';
            }
            src << '\n';
        }
    }
    for (int frame : {124,180,260,330,415,500,580,665}) {
        src << frame << '\n';
    }

    std::istringstream sound_input(src.str());
    const auto sounds = parse_sound_info(sound_input);
    assert(sounds.sound_ids[0][0][0] == 1000);
    assert(sounds.sound_ids[0][3][4] == 1019);
    assert(sounds.sound_ids[7][3][4] == 1159);
    assert(sounds.trigger_frames[0] == 124);
    assert(sounds.trigger_frames[7] == 665);
    assert(sound_id_for_random_slot(
        sounds, 0, StuntQuality::Good, 4) == 1019);
}
