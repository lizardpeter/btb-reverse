#pragma once

#include "btb/bobs_band.hpp"
#include "btb/bobs_band_data.hpp"

#include <array>

namespace btb::bobs_band {

enum class MachineAnimationState : int {
    Idle = 0,       // draw frame zero from the 1-second sprite
    OneSecond = 1,  // 15 short-sheet frames
    TwoSeconds = 2, // 30 long-sheet frames
};

struct MachineChannel {
    MachineAnimationState state{MachineAnimationState::Idle};
    int frame{};
    int tick{};
};

struct MachineSprite {
    int machine{}; // 0=Roley, 1=Muck, 2=Lofty, 3=Dizzy, 4=Scoop
    int type{};    // machine*2 + (state == TwoSeconds)
    Point destination{};
    Size frame_size{}; // authoritative source-rect stride from machinedata.txt
    int source_frame{};
    bool animated{};
};

class MachineAnimations {
public:
    // The original editor only starts another machine animation when its
    // per-machine state is zero; selecting a new brick does not interrupt an
    // already-running machine animation.
    [[nodiscard]] bool trigger(int sound_type) noexcept;

    // Native DrawBobsBandScene adds the shared frame delta at 0x00446FDC
    // to each active machine timer. Any value >3 advances ONE frame and
    // resets the timer rather than running a catch-up while loop.
    void advance(int frame_delta) noexcept;

    [[nodiscard]] const MachineChannel& channel(int machine) const noexcept {
        return channels_[static_cast<std::size_t>(machine)];
    }

    [[nodiscard]] MachineSprite sprite(
        const MachineData& data, int machine) const noexcept;

private:
    std::array<MachineChannel,5> channels_{};
};

struct ConductorEditorAnimation {
    int frame{}; // idle animation starts at 0
    int tick{};

    // 0x41F2A7: editor increments tick, advances a frame only when tick >5.
    // At frame 20, rand()%3 !=0 cancels the long idle variant, otherwise
    // it continues through 20..49. Frame 50 always wraps to frame zero.
    [[nodiscard]] bool advance(int random_value) noexcept;
};

} // namespace btb::bobs_band
