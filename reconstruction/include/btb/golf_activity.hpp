#pragma once

#include "btb/golf_runtime.hpp"

#include <cstdint>
#include <optional>

namespace btb::golf {

// Exact core gameplay is split from platform input / sprite timing.
// The retail keyboard/mouse adapter must translate its action to these
// explicit events; guessed mouse-to-aim formulae are not embedded here.
struct RoundInput {
    int aim_delta{};
    bool accept_aim{};
    bool accept_power{};
    bool swing_animation_finished{};
    bool feedback_audio_finished{};
    std::uint32_t random_value{};
};

struct RoundFrame {
    RoundState state{RoundState::Aim};
    BallMotion ball{};
    int aim{};
    int power{};
    int score{};
    int attempts_remaining{};
    int matched_target{-1};
    std::optional<int> managed_voice{};
    bool feedback_started{};
    bool finished{};
    bool progress_golf_completion{};
};

// Sources:
//   0x004145F0 InitializeGolfActivity — five attempts, all difficulties
//   0x00415080 UpdateGolfGameplayState — inner states 0,1,2,99,3,4,5,6
//   0x00415830 UpdateGolfActivity — final audio / Play Again handoff
//
// The inner simulation uses the recovered exact helper equations from
// golf_runtime. Physical key transitions, eight-by-eight swing sprite timing,
// and sound completion remain explicit events from the native host.
// Advance never forges those events merely to move the model forward.
class Round {
public:
    explicit Round(Data data, int difficulty);

    [[nodiscard]] RoundFrame advance(const RoundInput& input) noexcept;
    [[nodiscard]] const Data& data() const noexcept { return data_; }
    [[nodiscard]] RoundState state() const noexcept { return state_; }
    [[nodiscard]] BallMotion ball() const noexcept { return ball_; }
    [[nodiscard]] int difficulty() const noexcept { return difficulty_; }
    [[nodiscard]] int attempts_remaining() const noexcept {
        return attempts_remaining_;
    }
    [[nodiscard]] int score() const noexcept { return score_; }
    [[nodiscard]] bool finished() const noexcept { return finished_; }

private:
    [[nodiscard]] RoundFrame snapshot() const noexcept;

    Data data_{};
    int difficulty_{};
    RoundState state_{RoundState::Aim};
    BallMotion ball_{};
    int aim_{};
    int power_{};
    int direction_{1};
    int attempts_remaining_{kRetailInitialAttempts};
    int score_{};
    int target_index_{-1};
    bool feedback_has_started_{};
    bool finished_{};
};

} // namespace btb::golf
