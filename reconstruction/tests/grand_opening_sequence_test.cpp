#include "btb/grand_opening_sequence.hpp"

#include <cassert>
#include <sstream>

using namespace btb::grand_opening;

int main() {
    static_assert(kCompositionCellCount == 120);
    static_assert(kCompositionSaveBytes == 480);

    static_assert(static_cast<int>(Conductor::Bob) == 0);
    static_assert(static_cast<int>(Conductor::Wendy) == 1);
    static_assert(static_cast<int>(Conductor::FarmerPickles) == 2);

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

    static_assert(clip_variation_for_row(0) == 5);
    static_assert(clip_variation_for_row(1) == 4);
    static_assert(clip_variation_for_row(2) == 3);
    static_assert(clip_variation_for_row(3) == 2);
    static_assert(clip_variation_for_row(4) == 1);

    static_assert(
        loaded_sound_slot_index(0, MachineType::Roley1Second) == 40);
    static_assert(
        loaded_sound_slot_index(0, MachineType::Scoop2Second) == 49);
    static_assert(
        loaded_sound_slot_index(4, MachineType::Roley1Second) == 0);
    static_assert(
        loaded_sound_slot_index(4, MachineType::Scoop2Second) == 9);

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
    static_assert(conductor_progress_index(4, Conductor::Wendy) == 101);
}
