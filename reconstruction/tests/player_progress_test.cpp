#include "btb/player_progress.hpp"

#include <cassert>
#include <sstream>

using namespace btb::progress;

int main() {
    static_assert(kPlayerCount == 5);
    static_assert(kValuesPerPlayer == 100);
    static_assert(kRecordBytes == 400);
    static_assert(kPrerequisiteCount == 13);
    static_assert(kRetailFinaleThreshold == 13);

    static_assert(static_cast<std::size_t>(Slot::PetsCorner) == 50);
    static_assert(static_cast<std::size_t>(Slot::SquirrelRun) == 51);
    static_assert(static_cast<std::size_t>(Slot::BobsBandBob) == 52);
    static_assert(static_cast<std::size_t>(Slot::BobsBandFarmerPickles) == 54);
    static_assert(static_cast<std::size_t>(Slot::ParkDesigner) == 55);
    static_assert(static_cast<std::size_t>(Slot::DinoRaptor) == 56);
    static_assert(static_cast<std::size_t>(Slot::DinoTriceratops) == 57);
    static_assert(static_cast<std::size_t>(Slot::DinoTyrannosaurus) == 58);
    static_assert(static_cast<std::size_t>(Slot::SpudMaze) == 59);
    static_assert(static_cast<std::size_t>(Slot::SpudSkate) == 60);
    static_assert(static_cast<std::size_t>(Slot::Maze) == 61);
    static_assert(static_cast<std::size_t>(Slot::Golf) == 62);
    static_assert(static_cast<std::size_t>(Slot::FireworkFinaleEntered) == 63);
    static_assert(static_cast<std::size_t>(Slot::Unused64) == 64);

    Record progress;
    assert(retail_progress_sum(progress) == 0);
    assert(!retail_finale_available(progress));
    assert(!intended_prerequisites_complete(progress));

    for (std::size_t i = kPrerequisiteFirst; i <= kPrerequisiteLast; ++i) {
        progress.values[i] = 1;
    }

    assert(intended_prerequisites_complete(progress));
    assert(retail_progress_sum(progress) == 13);
    assert(retail_finale_available(progress));

    progress.set(Slot::FireworkFinaleEntered, 1);
    assert(retail_progress_sum(progress) == 14);

    // Preserve exact executable behavior for edited/corrupt saves: the retail
    // test sums positive slots 50..64 rather than explicitly checking 50..62.
    Record altered;
    for (std::size_t i = 50; i < 61; ++i) {
        altered.values[i] = 1;
    }
    altered.set(Slot::FireworkFinaleEntered, 1);
    altered.set(Slot::Unused64, 1);
    assert(!intended_prerequisites_complete(altered));
    assert(retail_progress_sum(altered) == 13);
    assert(retail_finale_available(altered));

    std::ostringstream encoded;
    write_record(encoded, progress);

    std::istringstream decoded(encoded.str());
    const auto roundtrip = read_record(decoded);
    assert(roundtrip.values == progress.values);

    constexpr auto dino = dino_species_slots();
    static_assert(dino[0] == Slot::DinoRaptor);
    static_assert(dino[2] == Slot::DinoTyrannosaurus);

    constexpr auto band = bobs_band_conductor_slots();
    static_assert(band[0] == Slot::BobsBandBob);
    static_assert(band[2] == Slot::BobsBandFarmerPickles);

    constexpr auto initial_gate = update_finale_gate_from_progress(Record{});
    static_assert(initial_gate.locked);
    static_assert(!initial_gate.unlocked);
    static_assert(kFireworkFinaleAction == 0x22);
    static_assert(should_open_progress_screen(
        initial_gate, kFireworkFinaleAction));

    static_assert(progress_feedback_sound(0, 3, 0) == 959);
    static_assert(progress_feedback_sound(1, 3, 0) == 958);
    static_assert(progress_feedback_sound(2, 3, 0) == 957);
    static_assert(progress_feedback_sound(3, 3, 0) == 955);
    static_assert(progress_feedback_sound(3, 3, 1) == 956);

    static_assert(progress_feedback_sound(0, 2, 0) == 958);
    static_assert(progress_feedback_sound(1, 2, 0) == 957);
    static_assert(progress_feedback_sound(2, 2, 1) == 956);

    Record grouped;
    grouped.set(Slot::BobsBandBob, 1);
    assert(bobs_band_feedback_sound(grouped, 0) == 958);

    grouped.set(Slot::BobsBandWendy, 1);
    assert(bobs_band_feedback_sound(grouped, 0) == 957);

    grouped.set(Slot::BobsBandFarmerPickles, 1);
    assert(bobs_band_feedback_sound(grouped, 1) == 956);

    grouped.set(Slot::DinoRaptor, 1);
    grouped.set(Slot::DinoTriceratops, 1);
    grouped.set(Slot::DinoTyrannosaurus, 1);
    assert(dino_feedback_sound(grouped, 0) == 955);

    grouped.set(Slot::SpudMaze, 1);
    assert(spud_pair_feedback_sound(grouped, 0) == 957);

    grouped.set(Slot::Maze, 1);
    grouped.set(Slot::Golf, 1);
    assert(adventure_pair_feedback_sound(grouped, 1) == 956);
}
