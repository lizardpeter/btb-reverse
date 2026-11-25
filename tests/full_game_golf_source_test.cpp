#include "btb/full_game_golf.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#undef assert
#define assert(expr) do { if(!(expr)) { \
    std::fprintf(stderr,"native Golf test failed: %s at %s:%d\\n", \
        #expr,__FILE__,__LINE__);std::abort();} } while(false)
#include <filesystem>
#include <fstream>
#include <iterator>
#include <memory>
#include <string>

int main() {
    using namespace btb::full_game;
    namespace fs = std::filesystem;

    // Exact executable animation tables: every actor/reaction pair
    // consumes precisely one random draw at the idle boundary.
    for(std::size_t actor=0;actor<2;++actor)for(int variant=0;variant<2;++variant) {
        btb::golf::SpectatorAnimation a{19,1,-1};
        int calls=0;
        auto next=[&]{++calls;return variant;};
        btb::golf::advance_spectator_animation(a,actor,true,next);
        assert(a.frame==btb::golf::kSpectatorReactionStart[actor][variant]);
        assert(a.reaction==variant && calls==1);
        const int ticks=(btb::golf::kSpectatorReactionEnd[actor][variant]-a.frame)*5;
        for(int i=0;i<ticks;++i)
            btb::golf::advance_spectator_animation(a,actor,true,next);
        assert(a.frame==0 && a.reaction==-2 && calls==1);
        for(int i=0;i<100;++i)
            btb::golf::advance_spectator_animation(a,actor,true,next);
        assert(a.frame==0 && calls==1);
    }
    static_assert(btb::golf::bob_aim_row(0)==44);
    static_assert(btb::golf::bob_aim_row(60)==14);
    static_assert(btb::golf::bob_aim_row(87)==0);
    static_assert(btb::golf::bob_aim_row(88)==0);

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
    // Retail DrawGolfActivity draws five toolbar balls, physical ball
    // and shadow BMPs, then the authored course/characters, not merely
    // eight neutral backdrop/actor sprites.
    assert(step.effects.draws.size()==16);
    assert(step.effects.draws[0].source_asset=="Data\\SubGameGolf\\golfbg.bmp");
    const auto& digit=step.effects.draws[1];
    assert(digit.source_asset=="Data\\SubGameGolf\\golfnumbers.bmp");
    assert(digit.color_keyed && digit.x==535 && digit.y==426);
    assert(digit.source_rectangle->left==135 && digit.source_rectangle->right==150);
    assert(digit.source_rectangle->bottom==32);
    assert(step.effects.draws[2].source_asset=="Data\\SubGameGolf\\toolbar ball.bmp");
    assert(step.effects.draws[2].color_keyed && step.effects.draws[2].x==373);
    assert(step.effects.draws[6].x==453);
    assert(step.effects.draws[7].source_asset=="Data\\SubGameGolf\\marker.bmp");
    assert(step.effects.draws[8].source_asset=="Data\\SubGameGolf\\shadow.bmp");
    assert(step.effects.draws[8].x==191 && step.effects.draws[8].y==338);
    assert(step.effects.draws[9].source_asset=="Data\\SubGameGolf\\golfballstop.bmp");
    assert(step.effects.draws[9].x==187 && step.effects.draws[9].y==330);
    assert(step.effects.draws[10].source_asset==handle->round()->data().bob_sprite_path);
    assert(step.effects.draws[10].source_rectangle->top==14*174); // initial aim is 60
    assert(step.effects.draws[11].source_rectangle->left==1);
    assert(step.effects.draws[13].source_asset=="data\\subgamegolf\\flag.bmp");
    assert(step.effects.draws[13].source_rectangle->left==0);
    assert(step.effects.draws[15].source_asset=="data\\subgamegolf\\clown.bmp");
    assert(step.effects.audio.size()==2);
    assert(step.effects.audio[0].source_asset=="Data/music/golf.wav");
    assert(step.effects.audio[1].sound_id==123 && step.effects.audio[1].input_interruptible);
    for(int i=0;i<3;++i)step=game.advance({.managed_sound_playing=true});
    assert(step.effects.draws[13].source_rectangle->left==22);
    assert(step.effects.audio.empty());
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
            // Medium speed 4 x 30 ticks = power 120. Original shaderless
            // DirectDraw crop is floor(120*257/1000)=30px at x83 y416.
            for(int charge=0;charge<31;++charge)
                step=game.advance({});
            assert(handle->round()->state()==btb::golf::RoundState::PowerMeter);
            const auto& bar=step.effects.draws.back();
            assert(bar.source_asset=="Data\\SubGameGolf\\powerbar.bmp");
            assert(bar.x==83 && bar.y==416);
            assert(bar.source_rectangle);
            assert(bar.source_rectangle->right==30);
            assert(bar.source_rectangle->bottom==53);
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

        assert(handle->round()->state()==btb::golf::RoundState::ResetNextAttempt);
        step=game.advance({.managed_sound_playing=true});
        if(shot==4) {
            assert(handle->round()->finished());
            step=game.advance({.managed_sound_playing=true});
            assert(!step.effects.save_and_unload);
            step=game.advance({});
        }
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
    assert(step.effects.draws[10].source_rectangle->top==14*174);
    assert(live->round()->state()==btb::golf::RoundState::Aim);
    step=native.advance({.pointer_x=0});
    assert(step.effects.draws[10].source_rectangle->top==44*174);
    assert(step.effects.audio.size()==1 && step.effects.audio[0].sound_id==101);
    assert(step.kind==FrameKind::ActivityUpdated);
    step=native.advance({.pointer_x=0,.directional_input_bits=0x02});
    assert(step.effects.draws[10].source_rectangle->top==0);
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
            for(const auto& draw:step.effects.draws)if(draw.source_asset==live->round()->data().bob_sprite_path){
                assert(draw.source_rectangle->left==ticks/9*133);
                assert(draw.source_rectangle->right<=2128);
            }
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
        assert(live->round()->state()==btb::golf::RoundState::ResetNextAttempt);
        step=native.advance({.managed_sound_playing=true});
        if(shot<4) {
            assert(live->round()->state()==btb::golf::RoundState::Aim);
            assert(!step.effects.save_and_unload);
        } else {
            assert(live->round()->finished());
            for(int tick=0;tick<3;++tick) {
                step=native.advance({.managed_sound_playing=true});
                assert(!step.effects.save_and_unload);
                assert(!step.effects.draws.empty());
            }
            step=native.advance({.managed_sound_playing=false});
        }
    }
    assert(step.kind==FrameKind::ActivityUpdated);
    assert(step.effects.progress_writes.size()==1);
    assert(step.effects.progress_writes[0].slot==
           btb::progress::Slot::Golf);
    assert(native.globals().player_progress[4].get(
        btb::progress::Slot::Golf)==1);
    assert(native.globals().dispatcher.current_state==0x3c);
    assert(live->round()==nullptr);

    // Controlled target location: first ground contact overlaps the flag.
    // This fixture changes geometry only, not the source-backed mechanics.
    std::string fixture;
    {
        std::ifstream in(root/"golfdata.txt");
        fixture.assign(std::istreambuf_iterator<char>(in),{});
    }
    const auto pos=fixture.find("330 178");
    assert(pos!=std::string::npos);
    fixture.replace(pos,7,"154 170");
    {std::ofstream file(root/"golfdata.txt");file<<fixture;}
    GolfDriver hit_driver(root,0);
    assert(hit_driver.initialize(0,error));
    auto effects=hit_driver.advance({.pointer_x=616,.click_pulse=true});
    effects=hit_driver.advance({.pointer_x=616,.click_pulse=true});
    effects=hit_driver.advance({});
    hit_driver.queue_control({.swing_animation_finished=true});
    effects=hit_driver.advance({});
    for(int i=0;i<200 && hit_driver.round()->state()==btb::golf::RoundState::BallFlight;++i)
        effects=hit_driver.advance({});
    assert(hit_driver.round()->state()==btb::golf::RoundState::ResolveLanding);
    effects=hit_driver.advance({});
    const auto flag_hit=[](const ActivityFrameOutput& out)->const decltype(out.draws)::value_type*{
        for(const auto& draw:out.draws)
            if(draw.source_asset=="Data\\SubGameGolf\\flag01.bmp")return &draw;
        return nullptr;
    };
    assert(!flag_hit(effects)); // resolution happens after this draw
    effects=hit_driver.advance({.managed_sound_playing=true});
    const auto* first_hit=flag_hit(effects);
    assert(first_hit && first_hit->x==119 && first_hit->y==172);
    assert(first_hit->source_rectangle->left==0 && first_hit->source_rectangle->right==198);
    for(int i=0;i<4;++i)effects=hit_driver.advance({.managed_sound_playing=true});
    const auto* next_hit=flag_hit(effects);
    assert(next_hit && next_hit->source_rectangle->left==198);
    effects=hit_driver.advance({});
    effects=hit_driver.advance({});
    assert(hit_driver.round()->state()==btb::golf::RoundState::Aim);
    assert(flag_hit(effects)); // persists through attempt reset
    effects=hit_driver.advance({.pointer_x=616,.click_pulse=true});
    assert(flag_hit(effects)); // confirmation frame still draws old state
    effects=hit_driver.advance({});
    assert(!flag_hit(effects)); // next frame sees cleared previous hit
    assert(hit_driver.unload(error));

    // Live host windowed cadence is passed all the way to the round,
    // and DAT_0050AF08 persists across native activity replay.
    GolfDriver windowed(root,0);
    assert(windowed.initialize(0,error));
    effects=windowed.advance({.pointer_x=616,.click_pulse=true,.original_windowed_display=true});
    effects=windowed.advance({.click_pulse=true,.original_windowed_display=true});
    effects=windowed.advance({.original_windowed_display=true});
    for(int i=0;i<72;++i)
        effects=windowed.advance({.original_windowed_display=true});
    assert(windowed.round()->state()==btb::golf::RoundState::BallFlight);
    const auto before=windowed.round()->ball();
    for(int i=0;i<10;++i) {
        const auto old=windowed.round()->ball();
        effects=windowed.advance({.original_windowed_display=true});
        assert(windowed.round()->flight_counter()==static_cast<unsigned>(i+1));
        if(i==0)assert(windowed.round()->ball().speed==old.speed);
        else assert(windowed.round()->ball().speed<old.speed);
    }
    assert(windowed.round()->ball().position.x>before.position.x);
    assert(windowed.unload(error)&&windowed.initialize(0,error));
    assert(windowed.round()->flight_counter()==10);
    assert(windowed.unload(error));
    fs::remove_all(root);
}
