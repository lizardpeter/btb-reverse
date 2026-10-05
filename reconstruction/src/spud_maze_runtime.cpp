#include "btb/spud_maze_runtime.hpp"

#include <stdexcept>

namespace btb::spud_maze {

RepairSelection select_repair_damage(
    Difficulty difficulty,
    std::int32_t phase_seed_1_to_4) {

    if (phase_seed_1_to_4 < 1 || phase_seed_1_to_4 > 4) {
        throw std::runtime_error(
            "Spud Maze repair phase seed must match retail rand()%4 + 1");
    }

    RepairSelection result;
    const auto divisor = repair_damage_divisor(difficulty);

    std::int32_t counter = phase_seed_1_to_4;
    for (std::size_t i = 0; i < kRepairPointCount; ++i) {
        const bool damaged = (counter % divisor) == 0;
        result.damaged[i] = damaged;
        if (damaged) {
            ++result.repairs_remaining;
        }
        ++counter;
    }

    return result;
}

} // namespace btb::spud_maze
