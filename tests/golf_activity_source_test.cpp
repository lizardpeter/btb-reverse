#include "btb/golf_activity.hpp"

#include <cassert>
#include <sstream>
#include <string>

using namespace btb::golf;

namespace {
Data original_golf_data() {
    std::istringstream input(R"(98 195
data\subgamegolf\golfsprite_8bit.bmp
45
159 184
16
91 140
330 178
data\subgamegolf\flag.bmp
99 160
22 143
15
3
230 15
data\subgamegolf\windmill.bmp
129 164
62 151
15
3
429 31
data\subgamegolf\clown.bmp
103 129
33 117
15
3
data\subgamegolf\wendy.bmp
20 105
88 130
100
data\subgamegolf\spud.bmp
92 20
97 100
100
data\subgamegolf\wendy.bmp
53 54
85 118
100
5 4 3
2 4 6
10 10
1
105 247
// Comments)");
    return parse_data(input);
}
}

int main() {
    const auto data = original_golf_data();
    assert(data.attempts_by_difficulty[0] == 5);
    assert(data.attempts_by_difficulty[1] == 4);
    assert(data.attempts_by_difficulty[2] == 3);
    assert(data.power_bar_speed_by_difficulty[2] == 6);
    assert((data.initial_ball_position() == Vec2i{187,330}));

    // Hard difficulty is still five retail attempts, not the 3 in the
    // unused source file tuning table.
    Round round(data,2);
    assert(round.attempts_remaining() == 5);
    assert(round.ball().position.x == 187.0f);
    assert(round.ball().position.y == 330.0f);

    auto frame = round.advance({.aim_delta=999});
    assert(frame.aim == 88);
    frame = round.advance({.aim_delta=-999});
    assert(frame.aim == 0);
    frame = round.advance({.accept_aim=true});
    assert(frame.state == RoundState::PowerMeter);

    frame = round.advance({});
    assert(frame.power == 6);
    frame = round.advance({});
    assert(frame.power == 12);
    frame = round.advance({.accept_power=true});
    assert(frame.state == RoundState::LaunchSetup);
    assert(frame.power == 12);
    frame = round.advance({});
    assert(frame.state == RoundState::SwingAnimationDelay);
    assert(frame.ball.angle_degrees == 0);
    assert(frame.ball.speed == 504);

    // No invented animation countdown: physics remains gated until original
    // host reports its eight-by-eight swing sequence completion.
    for (int i=0; i<128; ++i) {
        frame = round.advance({});
        assert(frame.state == RoundState::SwingAnimationDelay);
    }
    frame = round.advance({.swing_animation_finished=true});
    assert(frame.state == RoundState::BallFlight);

    int motion_steps = 0;
    while (round.state() == RoundState::BallFlight && motion_steps < 200) {
        frame = round.advance({});
        ++motion_steps;
    }
    assert(motion_steps > 0 && motion_steps < 200);
    assert(round.state() == RoundState::ResolveLanding);
    frame = round.advance({});
    assert(frame.state == RoundState::ScoreAndFeedback);
    assert(!frame.feedback_started);

    frame = round.advance({.random_value=9});
    assert(frame.managed_voice);
    assert(frame.feedback_started);
    assert(!frame.finished);
    frame = round.advance({});
    assert(!frame.managed_voice);
    assert(frame.state == RoundState::ScoreAndFeedback);
    frame = round.advance({.feedback_audio_finished=true});
    assert(frame.state == RoundState::ResetNextAttempt);
    frame = round.advance({});
    assert(frame.attempts_remaining == 4);
    assert(frame.state == RoundState::Aim);
    assert(frame.ball.position.x == 187.0f);
    assert(frame.ball.position.y == 330.0f);
    assert(frame.power == 0);

    for (int shot=1; shot<5; ++shot) {
        static_cast<void>(round.advance({.accept_aim=true}));
        static_cast<void>(round.advance({.accept_power=true}));
        static_cast<void>(round.advance({}));
        static_cast<void>(round.advance({.swing_animation_finished=true}));
        int frames = 0;
        while (round.state() == RoundState::BallFlight && frames++ < 200) {
            static_cast<void>(round.advance({}));
        }
        assert(round.state() == RoundState::ResolveLanding);
        static_cast<void>(round.advance({}));
        const auto feedback = round.advance({.random_value=0});
        assert(feedback.managed_voice.has_value());
        if (shot == 4) {
            // Zero-score final attempt goes through exact final grade ID135.
            assert(feedback.managed_voice == 135);
        }
        round.advance({.feedback_audio_finished=true});
        frame = round.advance({});
    }
    assert(round.finished());
    assert(frame.finished);
    assert(frame.progress_golf_completion);
    assert(frame.attempts_remaining == 0);
    assert(round.score() == 0);
    assert(!round.advance({}).progress_golf_completion);
}
