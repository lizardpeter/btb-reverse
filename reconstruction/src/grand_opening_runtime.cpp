#include "btb/grand_opening_runtime.hpp"

namespace btb::grand_opening {

PlaybackSecondStep update_playback_second(
    const Composition& composition,
    std::int32_t elapsed_centiseconds,
    std::int32_t previous_elapsed_centiseconds,
    std::array<std::int32_t,5>& machine_animation_states) {

    PlaybackSecondStep result;
    result.second = playback_step_from_centiseconds(elapsed_centiseconds);
    if (result.second < 0) {
        result.timeline_complete =
            elapsed_centiseconds >=
            static_cast<std::int32_t>(kTimelineStepCount) * 100;
        return result;
    }

    const auto previous =
        playback_step_from_centiseconds(previous_elapsed_centiseconds);
    if (previous == result.second) {
        return result;
    }

    result.second_changed = true;
    result.triggers.reserve(kPitchRowCount);

    const auto second = static_cast<std::size_t>(result.second);

    for (std::size_t row = 0; row < kPitchRowCount; ++row) {
        const auto value = composition.cells[row][second];
        if (!is_machine_type(value)) {
            continue;
        }

        const auto type = static_cast<MachineType>(value);
        const auto machine = machine_for_type(type);
        const auto machine_index = static_cast<std::size_t>(machine);

        std::int32_t animation_write = 0;
        if (machine_animation_states[machine_index] == 0) {
            animation_write =
                static_cast<std::int32_t>(duration_for_type(type)) + 1;
            machine_animation_states[machine_index] = animation_write;
        }

        result.triggers.push_back({
            row,
            second,
            type,
            loaded_sound_slot_index(row, type),
            machine,
            animation_write,
        });
    }

    return result;
}

} // namespace btb::grand_opening
