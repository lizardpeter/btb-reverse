#include "btb/full_game_fireworks.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs=std::filesystem;
namespace fg=btb::full_game;
namespace fw=btb::fireworks;

static void write_source_geometry(const fs::path& original) {
    fs::create_directories(original/"graph");
    std::ofstream cells(original/"graph"/"fireworks.txt");
    std::ofstream palette(original/"graph"/"fireworkboxes.txt");
    for(int i=0;i<18;++i) {
        const int x=100+i*10,y=20+i;
        cells<<x<<' '<<y<<'\n'
             <<x+30<<' '<<y<<'\n'
             <<x+30<<' '<<y+60<<'\n'
             <<x<<' '<<y+60<<"\n\n";
    }
    for(int i=0;i<12;++i) {
        const int x=40+i*5,y=300;
        palette<<x<<' '<<y<<'\n'
               <<x+50<<' '<<y<<'\n'
               <<x+50<<' '<<y+90<<'\n'
               <<x<<' '<<y+90<<"\n\n";
    }
}

int main() {
    const auto root=fs::temp_directory_path()/
        "btb_native_fireworks_real_movie_transition_test";
    fs::remove_all(root);
    const auto original=root/"Data"/"SubGameFirework";
    write_source_geometry(original);
    const auto save=root/"BTB-Saves";
    int opens=0;
    int closes=0;
    fs::path observed_movie;
    fg::FireworksDriver activity(original,save,
        [&](const fs::path& movie,std::string& error) {
            ++opens;
            observed_movie=movie;
            error.clear();
            return true;
        },
        [&]() {++closes;});
    std::string error;
    assert(activity.initialize(2,error));
    assert(activity.editor().internal_state==fw::InternalState::Editor);

    // Authored Play control 27 enters state 14 with managed sound 880.
    const auto clicked=activity.advance({
        .pointer_x=320,.pointer_y=440,.click_pulse=true});
    assert(!clicked.fatal_error);
    assert(activity.editor().internal_state==
           fw::InternalState::PreShowMovieSetup);
    bool played_voice=false;
    for(const auto& sound:clicked.audio)
        if(sound.operation==fg::AudioOperation::ManagedSoundId &&
           sound.sound_id==880)played_voice=true;
    assert(played_voice);

    // Retail waits for the *actual* managed sound system. Silence cannot
    // bypass state 14 or request movie playback.
    const auto waiting=activity.advance({
        .pointer_x=320,.pointer_y=440,.managed_sound_playing=false});
    assert(!waiting.fatal_error);
    assert(!waiting.draws.empty());
    assert(opens==0);

    const auto started=activity.advance({
        .pointer_x=320,.pointer_y=440,.managed_sound_playing=true});
    assert(!started.fatal_error);
    assert(opens==1);
    assert(observed_movie==root/"Data"/"movies"/"fireworkcomplete.bik");
    assert(activity.editor().internal_state==
           fw::InternalState::PreShowMoviePlayback);

    // Original decoded pixel surface is preserved for presentation while
    // Bink reports no real EOS, not erased by empty editor drawing.
    const auto playback=activity.advance({});
    assert(!playback.fatal_error);
    assert(playback.retain_original_global_movie_pixels);
    assert(playback.draws.empty());
    assert(opens==1);

    const auto finished=activity.advance({
        .original_global_movie_finished=true});
    assert(!finished.fatal_error);
    assert(!finished.retain_original_global_movie_pixels);
    assert(activity.editor().internal_state==fw::InternalState::ShowSetup);
    assert(finished.audio.size()==2);
    assert(finished.audio[0].operation==
           fg::AudioOperation::StopManagedSounds);
    assert(finished.audio[1].sound_id==142);
    assert(finished.audio[1].priority==90);
    assert(finished.audio[1].arbitration_class==1);
    assert(finished.audio[1].input_interruptible);

    // The unhosted 23-movie bank still fails closed; never claim that the
    // authored show or certificate actually finished.
    const auto missing_bank=activity.advance({});
    assert(missing_bank.fatal_error);
    assert(!missing_bank.save_and_unload);
    assert(missing_bank.progress_writes.empty());
    assert(activity.unload(error));
    assert(closes==0); // already BinkClosed by real decoder EOS
    assert(fs::is_regular_file(save/"firedata3.txt"));
    assert(fs::file_size(save/"firedata3.txt")==72);

    // Mid-movie exit must cancel the original handle, rather than leak
    // its frame service into the next activity or menus.
    fg::FireworksDriver cancelled(original,save,
        [&](const fs::path&,std::string& e) {
            e.clear();return true;
        },
        [&]() {++closes;});
    assert(cancelled.initialize(1,error));
    assert(!cancelled.advance({
        .pointer_x=320,.pointer_y=440,.click_pulse=true}).fatal_error);
    assert(!cancelled.advance({.managed_sound_playing=true}).fatal_error);
    assert(cancelled.editor().internal_state==
           fw::InternalState::PreShowMoviePlayback);
    assert(cancelled.unload(error));
    assert(closes==1);
    assert(cancelled.unload(error)); // safe idempotent teardown
    assert(closes==1);

    // Missing Bink provider cannot silently complete or advance state 14.
    fg::FireworksDriver absent(original,save);
    assert(absent.initialize(1,error));
    const auto play=absent.advance({
        .pointer_x=320,.pointer_y=440,.click_pulse=true});
    assert(!play.fatal_error);
    const auto rejected=absent.advance({.managed_sound_playing=true});
    assert(rejected.fatal_error);
    assert(absent.editor().internal_state==
           fw::InternalState::PreShowMovieSetup);
    assert(absent.unload(error));
    fs::remove_all(root);
}
