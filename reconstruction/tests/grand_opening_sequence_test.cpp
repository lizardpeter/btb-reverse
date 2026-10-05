#include "btb/grand_opening_sequence.hpp"

#include <cassert>
#include <sstream>

using namespace btb::grand_opening;

int main() {
    static_assert(kCompositionCellCount == 120);
    static_assert(kCompositionSaveBytes == 480);
    static_assert(kTimelineStepMilliseconds == 1000);
    static_assert(kTimelineDurationSeconds == 24);
    static_assert(static_cast<int>(ToolbarControl::Play) == 0);
    static_assert(static_cast<int>(ToolbarControl::Stop) == 1);
    static_assert(static_cast<int>(ToolbarControl::ClearAll) == 2);
    static_assert(static_cast<int>(ToolbarControl::Delete) == 3);
    static_assert(static_cast<int>(ActivityState::PreparePlayback) == 8);
    static_assert(static_cast<int>(ActivityState::Playing) == 9);
    static_assert(static_cast<int>(ActivityState::ExitToPlayAgain) == 10);

    static_assert(static_cast<int>(Conductor::Bob) == 0);
    static_assert(static_cast<int>(Conductor::Wendy) == 1);
    static_assert(static_cast<int>(Conductor::FarmerPickles) == 2);
    static_assert(pitch_for_row(0) == Pitch::CSharp);
    static_assert(pitch_for_row(1) == Pitch::B);
    static_assert(pitch_for_row(2) == Pitch::A);
    static_assert(pitch_for_row(3) == Pitch::GSharp);
    static_assert(pitch_for_row(4) == Pitch::FSharp);
    static_assert(pitch_name(Pitch::CSharp) == "C#");
    static_assert(pitch_name(Pitch::FSharp) == "F#");
    static_assert(backing_track_filename(Conductor::Bob) == "bobmt.wav");
    static_assert(backing_track_filename(Conductor::Wendy) == "Wendymt.wav");
    static_assert(backing_track_filename(Conductor::FarmerPickles) == "fpmt.wav");

    static_assert(machine_type(Machine::Roley, Duration::Short)
        == MachineType::Roley1Second);
    static_assert(machine_type(Machine::Scoop, Duration::Long)
        == MachineType::Scoop2Second);
    static_assert(machine_for_type(MachineType::Muck2Second) == Machine::Muck);
    static_assert(duration_for_type(MachineType::Lofty1Second) == Duration::Short);
    static_assert(duration_for_type(MachineType::Dizzy2Second) == Duration::Long);

    static_assert(machine_name(MachineType::Roley1Second) == "Roley 1 second");
    static_assert(machine_name(MachineType::Scoop2Second) == "Scoop 2 second");

    static_assert(machine_span(MachineType::Roley1Second) == 1);
    static_assert(machine_span(MachineType::Roley2Second) == 2);
    static_assert(machine_span(MachineType::Muck1Second) == 1);
    static_assert(machine_span(MachineType::Muck2Second) == 2);
    static_assert(machine_span(MachineType::Lofty1Second) == 1);
    static_assert(machine_span(MachineType::Lofty2Second) == 2);
    static_assert(machine_span(MachineType::Dizzy1Second) == 1);
    static_assert(machine_span(MachineType::Dizzy2Second) == 2);
    static_assert(machine_span(MachineType::Scoop1Second) == 1);
    static_assert(machine_span(MachineType::Scoop2Second) == 2);

    static_assert(wav_suffix_for_pitch_row(0) == 5);
    static_assert(wav_suffix_for_pitch_row(1) == 4);
    static_assert(wav_suffix_for_pitch_row(2) == 3);
    static_assert(wav_suffix_for_pitch_row(3) == 2);
    static_assert(wav_suffix_for_pitch_row(4) == 1);

    static_assert(
        loaded_sound_slot_index(0, MachineType::Roley1Second) == 40);
    static_assert(
        loaded_sound_slot_index(0, MachineType::Scoop2Second) == 49);
    static_assert(
        loaded_sound_slot_index(4, MachineType::Roley1Second) == 0);
    static_assert(
        loaded_sound_slot_index(4, MachineType::Scoop2Second) == 9);

    constexpr auto first_cell = decode_grid_region(0);
    static_assert(first_cell.has_value());
    static_assert(first_cell->pitch_row == 0 && first_cell->second == 0);
    constexpr auto last_cell = decode_grid_region(119);
    static_assert(last_cell.has_value());
    static_assert(last_cell->pitch_row == 4 && last_cell->second == 23);
    static_assert(!decode_grid_region(120).has_value());

    static_assert(playback_step_from_centiseconds(0) == 0);
    static_assert(playback_step_from_centiseconds(99) == 0);
    static_assert(playback_step_from_centiseconds(100) == 1);
    static_assert(playback_step_from_centiseconds(2399) == 23);
    static_assert(playback_step_from_centiseconds(2400) == -1);

    Composition composition;
    for (const auto& row : composition.cells) {
        for (const auto cell : row) {
            assert(cell == -1);
        }
    }

    assert(place_event(
        composition, 0, 0, MachineType::Roley1Second));
    assert(composition.cells[0][0] == 0);

    assert(place_event(
        composition, 1, 5, MachineType::Muck2Second));
    assert(composition.cells[1][5] == 3);
    assert(composition.cells[1][6] == 10);
    assert(!can_place_event(
        composition, 1, 6, MachineType::Scoop1Second));
    assert(!place_event(
        composition, 1, 23, MachineType::Scoop2Second));

    assert(remove_event_at(composition, 1, 6) == 3);
    assert(composition.cells[1][5] == -1);
    assert(composition.cells[1][6] == -1);
    assert(remove_event_at(composition, 1, 6) == -1);

    assert(place_event(
        composition, 4, 23, MachineType::Scoop1Second));

    std::ostringstream encoded(std::ios::binary);
    write_composition(encoded, composition);
    assert(encoded.str().size() == kCompositionSaveBytes);

    std::istringstream decoded(encoded.str(), std::ios::binary);
    const auto roundtrip = read_composition(decoded);
    assert(roundtrip.cells[0][0] == 0);
    assert(roundtrip.cells[4][23] == 8);

    assert(composition_file_index(Conductor::Bob, 0) == 0);
    assert(composition_file_index(Conductor::Wendy, 0) == 5);
    assert(composition_file_index(Conductor::FarmerPickles, 4) == 14);

    assert(std::string_view(composition_filename(Conductor::Bob, 0))
        == "musicbob1.txt");
    assert(std::string_view(composition_filename(Conductor::Wendy, 4))
        == "musicwendy5.txt");
    assert(std::string_view(composition_filename(
        Conductor::FarmerPickles, 2)) == "musicfarmer3.txt");

    constexpr auto legacy = legacy_last_payload(Conductor::Wendy);
    static_assert(legacy[0] == 1 && legacy[4] == 1);

    static_assert(conductor_progress_index(0, Conductor::Bob) == 0);
    static_assert(conductor_progress_index(0, Conductor::FarmerPickles) == 2);
    static_assert(conductor_progress_index(4, Conductor::Wendy) == 401);
    static_assert(!all_conductors_complete({1,1,0}));
    static_assert(all_conductors_complete({1,1,1}));
}
