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
    assert(step.replay_preparation);
    assert(step.replay_preparation->voice_sound_id == 573);
    assert(game.globals().menu.replay_active_latch);
    assert(step.effects.audio.size() >= 2);
    assert(step.effects.audio[step.effects.audio.size()-2].operation ==
           AudioOperation::StopManagedSounds);
    assert(step.effects.audio.back().sound_id == 573);
    assert(step.effects.audio.back().input_interruptible);
    assert(step.effects.progress_writes.size() == 1);
    assert(step.effects.progress_writes[0].slot ==
           btb::progress::Slot::Golf);
    assert(game.globals().player_progress[0].get(btb::progress::Slot::Golf) == 1);
    assert(game.globals().dispatcher.current_state == 0x3C);
    assert(game.globals().dispatcher.saved_state == 0x36);
    assert(game.globals().menu.replay_class == 1);
    assert(!handle->round()); // released once, never duplicated

    // Original 0x3F replay difficulty action -32 writes value 2 into
    // global 0x51C284, then re-enters saved Golf init state 0x36.
    game.globals().menu.variant_selection_origin =
        btb::game_flow::State::PlayAgainDifficultyUpdate;
    game.globals().menu.source_variant = 2;
    game.set_outer_state(btb::game_flow::State::GolfInit);
    auto again = game.advance({});
    assert(again.kind == FrameKind::ActivityInitialized);
    assert(handle->round());
    assert(handle->round()->difficulty() == 2);
    assert(handle->round()->attempts_remaining() == 5);
    std::string error;
    assert(game.unload_current(error));

    // Host-integrated real Golf path: no test-only queue_control calls.
    // Original input confirms angle and power, state 99 completes after
    // 8 * 9 ticks, and finished DirectSound feedback unlocks the next shot.
    GameRoot native;
    assert(native.select_profile(4));
    auto source_driver=std::make_unique<GolfDriver>(root,0);
    auto* live=source_driver.get();
    native.install(ActivityId::Golf,std::move(source_driver));
    native.set_outer_state(btb::game_flow::State::GolfInit);
    step=native.advance({});
    assert(step.kind==FrameKind::ActivityInitialized);
    // Mouse X maps through the retail 88 - (x/7) aiming equation.
    step=native.advance({.pointer_x=616});
    assert(live->round()->state()==btb::golf::RoundState::Aim);
    step=native.advance({.pointer_x=0});
    assert(step.kind==FrameKind::ActivityUpdated);
    step=native.advance({.pointer_x=0,.directional_input_bits=0x02});
    // Aim is now 87, then the UI returns to mouse-originated aim at 616.
    assert(live->round()->state()==btb::golf::RoundState::Aim);

    for(int shot=0;shot<5;++shot) {
        step=native.advance({.pointer_x=616,.click_pulse=true});
        assert(live->round()->state()==btb::golf::RoundState::PowerMeter);
        step=native.advance({.pointer_x=616,.click_pulse=true});
        assert(live->round()->state()==btb::golf::RoundState::LaunchSetup);
        step=native.advance({});
        assert(live->round()->state()==btb::golf::RoundState::SwingAnimationDelay);
        for(int ticks=0;ticks<71;++ticks) {
            step=native.advance({});
            assert(live->round()->state()==btb::golf::RoundState::SwingAnimationDelay);
        }
        step=native.advance({});
        assert(live->round()->state()==btb::golf::RoundState::BallFlight);
        int physics_frames=0;
        while(live->round()->state()==btb::golf::RoundState::BallFlight &&
              physics_frames++<300)
            step=native.advance({});
        assert(physics_frames<300);
        step=native.advance({}); // ResolveLanding -> ScoreAndFeedback
        assert(live->round()->state()==btb::golf::RoundState::ScoreAndFeedback);
        step=native.advance({}); // managed voice submitted
        assert(!step.effects.audio.empty());
        assert(step.effects.audio[0].operation==AudioOperation::ManagedSoundId);
        for(int tick=0;tick<3;++tick) {
            step=native.advance({.managed_sound_playing=true});
            assert(live->round()->state()==btb::golf::RoundState::ScoreAndFeedback);
        }
        step=native.advance({.managed_sound_playing=false});
        assert(live->round()->state()==btb::golf::RoundState::ResetNextAttempt);
        step=native.advance({});
        if(shot<4)
            assert(live->round()->state()==btb::golf::RoundState::Aim);
    }
    assert(step.kind==FrameKind::ActivityUpdated);
    assert(step.effects.progress_writes.size()==1);
    assert(step.effects.progress_writes[0].slot==
           btb::progress::Slot::Golf);
    assert(native.globals().player_progress[4].get(
        btb::progress::Slot::Golf)==1);
    assert(native.globals().dispatcher.current_state==0x3c);
    assert(live->round()==nullptr);

    fs::remove_all(root);
}
