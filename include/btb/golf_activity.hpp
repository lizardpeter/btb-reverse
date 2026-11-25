#pragma once

#include "btb/golf_runtime.hpp"
#include "btb/retail_crt_random.hpp"

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
    // Retail 0x00415080 state 0 recalculates aim from mouse X each frame:
    // 0x58 - (pointer_x / 7), then arrow-key adjustments and clamping.
    // Absent preserves direct/source unit-test aim control.
    std::optional<int> source_mouse_aim{};
    // Consume the process RNG only on an original rand branch.
    retail::OriginalRetailRandom* random_stream{};
    bool original_windowed_display{};
};

// 00414C86..00414DB1: two spectators share the process CRT stream.
// The idle limits (20,20) are the two initialized words at 004437CC.
// Reaction starts and exclusive ends are the original stack tables.
// Reaction -1 is unused, -2 is already played for the current shot.
struct SpectatorAnimation {
    int frame{};
    int countdown{5};
    int reaction{-1};
};
inline constexpr std::array<int,2> kSpectatorIdleLimits{20,20};
inline constexpr std::array<std::array<int,2>,2> kSpectatorReactionStart{{{21,21},{45,60}}};
inline constexpr std::array<std::array<int,2>,2> kSpectatorReactionEnd{{{45,41},{90,88}}};
template<class NextRand>
constexpr void advance_spectator_animation(SpectatorAnimation& s,
    std::size_t actor, bool hit, NextRand&& next_rand) noexcept {
    if(--s.countdown>0)return;
    s.countdown=5;
    ++s.frame;
    const int idle=kSpectatorIdleLimits[actor];
    if(!hit) {
        if(s.frame>=idle)s.frame=0;
        return;
    }
    if(s.frame==idle) {
        if(s.reaction==-1) {
            s.reaction=next_rand()&1;
            s.frame=kSpectatorReactionStart[actor][s.reaction];
        } else s.frame=0;
    }
    if(s.reaction>=0 && s.frame>=kSpectatorReactionEnd[actor][s.reaction]) {
        s.frame=0;
        s.reaction=-2;
    }
}
[[nodiscard]] constexpr int bob_aim_row(int aim) noexcept {
    // 00414C16: truncate(44.0 - aim / 2.0), not aim/2.
    return static_cast<int>(44.0-static_cast<double>(aim)/2.0);
}
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
    int bounce_height{}; // retail 0x0050AE50; independent of ground position
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
    explicit Round(Data data, int difficulty,std::uint32_t initial_flight_counter=0);

    [[nodiscard]] RoundFrame advance(const RoundInput& input) noexcept;
    [[nodiscard]] const Data& data() const noexcept { return data_; }
    [[nodiscard]] RoundState state() const noexcept { return state_; }
    [[nodiscard]] BallMotion ball() const noexcept { return ball_; }
    [[nodiscard]] int difficulty() const noexcept { return difficulty_; }
    [[nodiscard]] std::uint32_t flight_counter() const noexcept {return flight_tick_;}
    [[nodiscard]] int attempts_remaining() const noexcept {
        return attempts_remaining_;
    }
    [[nodiscard]] int score() const noexcept { return score_; }
    [[nodiscard]] bool finished() const noexcept { return finished_; }
    [[nodiscard]] RoundFrame snapshot() const noexcept;

private:

    Data data_{};
    int difficulty_{};
    RoundState state_{RoundState::Aim};
    BallMotion ball_{};
    int aim_{60};
    std::uint32_t flight_tick_{};
    int bounce_height_{};
    int bounce_amplitude_{};
    int bounce_remaining_{};
    int bounce_direction_{1};
    int power_{};
    int direction_{1};
    int attempts_remaining_{kRetailInitialAttempts};
    int score_{};
    int target_index_{-1};
    bool finished_{};
    bool completion_emitted_{};
};

} // namespace btb::golf
