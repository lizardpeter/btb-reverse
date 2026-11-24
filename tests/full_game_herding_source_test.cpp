#include "btb/full_game_herding.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace {
namespace fs=std::filesystem;
namespace h=btb::herding;
using namespace btb::full_game;

class KnownEntityProvider final : public HerdingSimulationProvider {
public:
    int expected_animals{};
    int undelivered{};
    bool fail_next{};
    int updates{};
    int unloads{};
    h::PicklesKeyboardMotion last_motion{};

    bool initialize(const h::Data& file,int difficulty,
                    std::string& error) override {
        if (file.setup_positions[3] != h::Vec2i{478,471} ||
            file.coordinate_groups.size()!=4) {
            error="original herd.txt layout mismatch";
            return false;
        }
        expected_animals=h::initial_undelivered_animal_count(difficulty);
        undelivered=expected_animals;
        error.clear();
        return true;
    }
    bool advance(const ActivityFrameInput&,
                 const h::PicklesKeyboardMotion& motion,
                 HerdingScene& scene,int& remaining,
                 std::vector<Audio>& source_audio_events,
                 std::string& error) override {
        ++updates;
        last_motion=motion;
        if (fail_next) {
            error="original AI provider reported navigation failure";
            return false;
        }
        scene={};
        h::RetailEntityRecord32 pickles{};
        pickles.entity_id=0;
        pickles.type=static_cast<int>(h::EntityType::FarmerPickles);
        pickles.x=478;pickles.y=471;
        pickles.direction=2;
        pickles.animation_frame=0;
        pickles.source_bottom=128;
        scene.entities.push_back(pickles);
        h::RetailEntityRecord32 sheep{};
        sheep.entity_id=1;
        sheep.type=static_cast<int>(h::EntityType::Sheep);
        sheep.x=550;sheep.y=480;
        sheep.source_bottom=103;
        sheep.direction=2;
        sheep.animation_frame=6;
        scene.entities.push_back(sheep);
        remaining=undelivered;
        if (updates==2) {
            source_audio_events.push_back({
                AudioOperation::ManagedSoundId,{},589,50,1
            }); // original sheep attraction line
        }
        error.clear();
        return true;
    }
    bool unload(std::string& error) override {
        ++unloads;
        error.clear();
        return true;
    }
};

void write_retail_herd(const fs::path& root) {
    fs::create_directories(root);
    std::ofstream file(root/"herd.txt");
    file << R"(719 266
527 114
376 170
478 471
93 324
100 120
44 99 202 99 353 279 461 263 530 299 966 181 966 166 966 235 1086 349 1257 405
1257 900 44 900
-1 -1
450 360 600 179 886 139 1028 304 755 360 -1 -1
30 500 500 500 500 500 1200 500 1200 850 30 850 -1 -1
92 324 -1 -1
)";
}
}

int main() {
    using namespace btb::full_game;
    using btb::game_flow::State;
    const auto root=fs::temp_directory_path()/
        "btb_full_game_herding_source";
    fs::remove_all(root);
    write_retail_herd(root);

    GameRoot game;
    assert(game.select_profile(0));
    auto source=std::make_unique<KnownEntityProvider>();
    auto* provider=source.get();
    auto adapter=std::make_unique<HerdingDriver>(
        root,std::move(source),0);
    auto* handle=adapter.get();
    // Native -4 on original 0x0D instruction screen means Hard.
    RetailMenuState menu{};
    menu.variant_selection_origin=State::HerdingPregameUpdate;
    menu.source_variant=2;
    adapter->configure_menu_state(menu);
    game.install(ActivityId::Herding,std::move(adapter));

    game.set_outer_state(State::HerdingInit);
    auto frame=game.advance({});
    assert(frame.kind==FrameKind::ActivityInitialized);
    assert(handle->initialized());
    assert(handle->difficulty()==2);
    assert(provider->expected_animals==15);
    assert(game.globals().dispatcher.current_state==0x0F);

    frame=game.advance({.directional_input_bits=0x06});
    assert(frame.kind==FrameKind::ActivityUpdated);
    assert(provider->updates==1);
    assert(provider->last_motion.movement_mask==
           (h::PicklesMoveRight|h::PicklesMoveUp));
    assert(provider->last_motion.delta_x==1.5f);
    assert(provider->last_motion.delta_y==-1.5f);
    assert(frame.effects.draws.size()==7); // bg, 2 entities, 3 bags, surround
    assert(frame.effects.draws[0].source_asset ==
           "Data\\SubGame1\\bk)1_revised_01.bmp");
    assert(frame.effects.draws[0].destination_clip);
    assert(frame.effects.audio.size()==2);
    assert(frame.effects.audio[0].operation ==
           AudioOperation::StartBackingTrack);
    assert(frame.effects.audio[0].source_asset ==
           "data\\music\\petscorner.wav");
    assert(frame.effects.audio[1].sound_id==581);
    assert(frame.effects.progress_writes.empty());

    frame=game.advance({});
    assert(frame.kind==FrameKind::ActivityUpdated);
    assert(frame.effects.audio.size()==1);
    assert(frame.effects.audio[0].sound_id==589); // AI owns this event

    provider->undelivered=0;
    frame=game.advance({.managed_sound_playing=true});
    assert(frame.effects.audio.empty());
    assert(handle->completion_stage()==0);

    frame=game.advance({.random_value=1});
    assert(handle->completion_stage()==1);
    assert(frame.effects.audio.size()==1);
    assert(frame.effects.audio[0].sound_id==600);
    assert(frame.effects.audio[0].priority==0x5a);
    assert(frame.effects.progress_writes.empty());

    frame=game.advance({.managed_sound_playing=true});
    assert(frame.effects.progress_writes.empty());
    assert(handle->initialized());

    frame=game.advance({});
    assert(frame.kind==FrameKind::ActivityUpdated);
    assert(frame.effects.save_and_unload);
    assert(frame.effects.progress_writes.size()==1);
    assert(frame.effects.progress_writes[0].slot ==
           btb::progress::Slot::PetsCorner);
    assert(game.globals().player_progress[0].get(
               btb::progress::Slot::PetsCorner)==1);
    assert(frame.effects.next_saved_state==0x0E);
    assert(game.globals().dispatcher.saved_state==0x0E);
    assert(game.globals().dispatcher.current_state==0x3C);
    assert(frame.replay_preparation);
    assert(frame.effects.audio.back().sound_id==573);
    assert(provider->unloads==1);
    assert(!handle->initialized());

    // Provider errors must not advance the original 68-state dispatcher.
    game.set_outer_state(State::HerdingInit);
    assert(game.advance({}).kind==FrameKind::ActivityInitialized);
    provider->fail_next=true;
    frame=game.advance({});
    assert(frame.kind==FrameKind::ActivityFailed);
    assert(frame.error=="original AI provider reported navigation failure");
    assert(game.globals().dispatcher.current_state==0x0F);
    assert(frame.effects.draws.empty());
    assert(game.unload_current(frame.error));

    // The original final-speech sound uses ONE process-wide CRT
    // draw, not ActivityFrameInput's synthetic default zero. Before
    // the final real completion gate, no extra random is consumed.
    {
        btb::retail::OriginalRetailRandom original_rng{0x12345678u};
        auto expected_rng=original_rng;
        GameRoot terminal_game{};
        assert(terminal_game.select_profile(0));
        auto completion_source=std::make_unique<KnownEntityProvider>();
        auto* completion_provider=completion_source.get();
        auto terminal_driver=std::make_unique<HerdingDriver>(
            root,std::move(completion_source),0,&original_rng);
        auto* observed_driver=terminal_driver.get();
        terminal_game.install(
            ActivityId::Herding,std::move(terminal_driver));
        terminal_game.set_outer_state(State::HerdingInit);
        assert(terminal_game.advance({}).kind==
               FrameKind::ActivityInitialized);
        const auto init_rng=original_rng.state();
        assert(init_rng==expected_rng.state());
        auto waiting=terminal_game.advance({.random_value=1});
        assert(waiting.kind==FrameKind::ActivityUpdated);
        assert(original_rng.state()==init_rng);
        completion_provider->undelivered=0;
        waiting=terminal_game.advance({
            .managed_sound_playing=true,.random_value=1});
        assert(waiting.kind==FrameKind::ActivityUpdated);
        assert(original_rng.state()==init_rng);
        assert(observed_driver->completion_stage()==0);
        const int expected_sound=599+(expected_rng.next_rand()&1);
        auto final_line=terminal_game.advance({.random_value=1});
        assert(final_line.kind==FrameKind::ActivityUpdated);
        assert(original_rng.state()==expected_rng.state());
        assert(observed_driver->completion_stage()==1);
        assert(final_line.effects.audio.size()==1);
        assert(final_line.effects.audio[0].sound_id==expected_sound);
        assert(final_line.effects.audio[0].priority==0x5a);
        assert(final_line.effects.audio[0].arbitration_class==1);
        assert(final_line.effects.progress_writes.empty());
        // The follow-up exit waits for speech, but does not call
        // rand() a second time even after the voice stops.
        waiting=terminal_game.advance({
            .managed_sound_playing=true,.random_value=1});
        assert(waiting.effects.progress_writes.empty());
        assert(original_rng.state()==expected_rng.state());
        auto finished=terminal_game.advance({.random_value=1});
        assert(finished.effects.save_and_unload);
        assert(original_rng.state()==expected_rng.state());
        assert(terminal_game.globals().player_progress[0].get(
            btb::progress::Slot::PetsCorner)==1);
    }

    fs::remove_all(root);
}
