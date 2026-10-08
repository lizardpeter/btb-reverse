#include "btb/full_game_golf.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

int main() {
    using namespace btb::full_game;
    namespace fs = std::filesystem;

    const auto root = fs::temp_directory_path() /
                      "btb_golf_full_game_source_test";
    fs::remove_all(root);
    fs::create_directories(root);
    {
        std::ofstream file(root / "golfdata.txt");
        file << R"(98 195
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
// Comments)";
    }

    GameRoot game;
    assert(game.select_profile(0));
    auto golf = std::make_unique<GolfDriver>(root,1);
    GolfDriver* handle = golf.get();
    game.install(ActivityId::Golf, std::move(golf));
    game.set_outer_state(btb::game_flow::State::GolfInit);

    auto step = game.advance({});
    assert(step.kind == FrameKind::ActivityInitialized);
    assert(game.globals().dispatcher.current_state == 0x37);
    assert(handle->round());
    assert(handle->round()->attempts_remaining() == 5);

    step = game.advance({});
    assert(step.kind == FrameKind::ActivityUpdated);
    assert(step.effects.draws.size() == 8); // background + 3+3 + Bob
    assert(step.effects.draws[0].source_asset ==
           "Data\\SubGameGolf\\golfbg.bmp");
    assert(step.effects.draws[1].source_asset ==
           "data\\subgamegolf\\flag.bmp");
    assert(step.effects.progress_writes.empty());

    for (int shot=0; shot<5; ++shot) {
        if (shot == 0) {
            // Original printed manual: click/Space selects angle, second
            // click/Space locks power. No invented pointer-to-angle mapping.
            step = game.advance({.click_pulse=true});
        } else {
            handle->queue_control({.accept_aim=true});
            step = game.advance({});
        }
        assert(handle->round()->state() == btb::golf::RoundState::PowerMeter);
        if (shot == 0) {
            step = game.advance({.click_pulse=true});
        } else {
            handle->queue_control({.accept_power=true});
            step = game.advance({});
        }
        step = game.advance({}); // launch -> swing delay
        assert(handle->round()->state() ==
               btb::golf::RoundState::SwingAnimationDelay);

        // The native input/animation layer explicitly reports completion.
        handle->queue_control({.swing_animation_finished=true});
        step = game.advance({});
        assert(handle->round()->state() == btb::golf::RoundState::BallFlight);

        int flight_frames=0;
        while (handle->round()->state() == btb::golf::RoundState::BallFlight &&
               flight_frames++ < 200) {
            step = game.advance({});
        }
        assert(flight_frames < 200);
        step = game.advance({}); // landing -> feedback
        assert(handle->round()->state() ==
               btb::golf::RoundState::ScoreAndFeedback);
        step = game.advance({}); // once-per-shot managed voice
        assert(step.effects.audio.size() == 1);
        assert(step.effects.audio[0].operation ==
               AudioOperation::ManagedSoundId);

        handle->queue_control({.feedback_audio_finished=true});
        step = game.advance({});
        assert(handle->round()->state() ==
               btb::golf::RoundState::ResetNextAttempt);
        step = game.advance({});
        if (shot < 4) {
            assert(step.effects.progress_writes.empty());
            assert(handle->round());
        }
    }

    // Fifth completed shot writes retail progress slot 62, persists through
    // the owning GameRoot record and changes outer state to Play Again.
    assert(step.kind == FrameKind::ActivityUpdated);
    assert(step.effects.save_and_unload);
    assert(step.effects.progress_writes.size() == 1);
    assert(step.effects.progress_writes[0].slot ==
           btb::progress::Slot::Golf);
    assert(game.globals().player_progress[0].get(btb::progress::Slot::Golf) == 1);
    assert(game.globals().dispatcher.current_state == 0x3C);
    assert(!handle->round()); // released once, never duplicated

    fs::remove_all(root);
}
