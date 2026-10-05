#include "btb/bobs_band_runtime.hpp"

#include <cassert>

using namespace btb::bobs_band;

int main() {
    static_assert(sound_type(Machine::Roley, Duration::Short) == 0);
    static_assert(sound_type(Machine::Roley, Duration::Long) == 1);
    static_assert(sound_type(Machine::Muck, Duration::Short) == 2);
    static_assert(sound_type(Machine::Scoop, Duration::Long) == 9);

    static_assert(machine_for_sound_type(0) == Machine::Roley);
    static_assert(machine_for_sound_type(9) == Machine::Scoop);
    static_assert(duration_for_sound_type(8) == Duration::Short);
    static_assert(duration_for_sound_type(9) == Duration::Long);

    static_assert(sound_span(0) == 1);
    static_assert(sound_span(1) == 2);
    static_assert(sound_span(8) == 1);
    static_assert(sound_span(9) == 2);

    static_assert(pitch_wav_suffix(0) == 5);
    static_assert(pitch_wav_suffix(1) == 4);
    static_assert(pitch_wav_suffix(4) == 1);

    constexpr auto first = decode_grid_region(0);
    static_assert(first.has_value());
    static_assert(first->row == 0 && first->column == 0);

    constexpr auto last = decode_grid_region(119);
    static_assert(last.has_value());
    static_assert(last->row == 4 && last->column == 23);
    static_assert(!decode_grid_region(120).has_value());

    static_assert(playback_column_from_centiseconds(0) == 0);
    static_assert(playback_column_from_centiseconds(99) == 0);
    static_assert(playback_column_from_centiseconds(100) == 1);
    static_assert(playback_column_from_centiseconds(2399) == 23);
    static_assert(playback_column_from_centiseconds(2400) == -1);

    static_assert(playback_buffer_index(0, 0) == 40);
    static_assert(playback_buffer_index(0, 9) == 49);
    static_assert(playback_buffer_index(1, 0) == 30);
    static_assert(playback_buffer_index(4, 0) == 0);
    static_assert(playback_buffer_index(4, 9) == 9);
    static_assert(kTimelineSeconds == 24);
    static_assert(kSoundFileStems[0] == "roley1");
    static_assert(kSoundFileStems[1] == "roley2");
    static_assert(kSoundFileStems[8] == "scoop1");
    static_assert(kSoundFileStems[9] == "scoop2");
    static_assert(kConductorBackingTracks[0] == "bobmt.wav");
    static_assert(kConductorBackingTracks[1] == "Wendymt.wav");
    static_assert(kConductorBackingTracks[2] == "fpmt.wav");
    static_assert(static_cast<int>(ToolbarControl::Play) == 0);
    static_assert(static_cast<int>(ToolbarControl::Delete) == 3);

    SequencerGrid grid;
    assert(grid.at(0,0) == kEmptyCell);

    assert(grid.place(0,0,0));
    assert(grid.at(0,0) == 0);
    assert(!grid.place(0,0,2));

    assert(grid.place(2,5,1));
    assert(grid.at(2,5) == 1);
    assert(grid.at(2,6) == kContinuationCell);
    assert(!grid.can_place(2,6,4));

    // Retail lets deletion begin on the continuation cell and walks left to
    // the base item before clearing the full long-sound span.
    assert(grid.remove_at(2,6) == 1);
    assert(grid.at(2,5) == kEmptyCell);
    assert(grid.at(2,6) == kEmptyCell);

    assert(grid.can_place(4,23,8));
    assert(grid.place(4,23,8));
    assert(!grid.can_place(3,23,9));

    grid.clear();
    for (std::size_t row = 0; row < kPitchRows; ++row) {
        for (std::size_t col = 0; col < kTimelineColumns; ++col) {
            assert(grid.at(row,col) == kEmptyCell);
        }
    }
}
