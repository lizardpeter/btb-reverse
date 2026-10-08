#include "btb/bobs_band_animation.hpp"

namespace btb::bobs_band {

bool MachineAnimations::trigger(int sound_type) noexcept {
    if (!valid_machine(sound_type)) {
        return false;
    }
    auto& machine = channels_[
        static_cast<std::size_t>(machine_owner(sound_type))];
    if (machine.state != MachineAnimationState::Idle) {
        return false;
    }
    machine.state = sound_type % 2 == 0
        ? MachineAnimationState::OneSecond
        : MachineAnimationState::TwoSeconds;
    machine.tick = 0;
    machine.frame = 0;
    return true;
}

void MachineAnimations::advance(int frame_delta) noexcept {
    for (auto& machine : channels_) {
        if (machine.state == MachineAnimationState::Idle) {
            continue;
        }
        machine.tick += frame_delta;
        if (machine.tick <= 3) {
            continue;
        }
        machine.tick = 0;
        ++machine.frame;
        const auto max_frames =
            machine.state == MachineAnimationState::OneSecond ? 15 : 30;
        if (machine.frame >= max_frames) {
            machine.frame = 0;
            machine.state = MachineAnimationState::Idle;
        }
    }
}

MachineSprite MachineAnimations::sprite(
    const MachineData& data, int machine_index) const noexcept {

    if (machine_index < 0 || machine_index >= 5) {
        return {};
    }
    const auto& channel_data = channels_[
        static_cast<std::size_t>(machine_index)];
    const bool long_sheet =
        channel_data.state == MachineAnimationState::TwoSeconds;
    const int type = machine_index * 2 + (long_sheet ? 1 : 0);
    return {
        machine_index,
        type,
        data.machine_positions[static_cast<std::size_t>(type)],
        {
            data.machine_sizes[static_cast<std::size_t>(type)].width,
            data.machine_sizes[static_cast<std::size_t>(type)].height,
        },
        channel_data.frame,
        channel_data.state != MachineAnimationState::Idle,
    };
}

bool ConductorEditorAnimation::advance(int random_value) noexcept {
    ++tick;
    if (tick <= 5) {
        return false;
    }
    tick = 0;
    ++frame;
    if (frame == 20 && random_value % 3 != 0) {
        frame = 0;
    }
    if (frame >= 50) {
        frame = 0;
    }
    return true;
}

} // namespace btb::bobs_band
