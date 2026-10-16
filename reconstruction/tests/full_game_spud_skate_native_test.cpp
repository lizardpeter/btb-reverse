#include "btb/full_game_spud_skate.hpp"
#include "btb/full_game_runtime.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace fs=std::filesystem;
namespace fg=btb::full_game;
namespace skate=btb::spud_skate;

struct DecodedFiveBinks final : fg::SpudSkateMovieSource {
    std::vector<fg::SpudSkateMovieFrame> real_decoded_frames{};
    std::vector<skate::StuntQuality> quality_selections{};
    std::size_t clock_cursor{};
    int opens{},closes{},end_movies{},seek_calls{},seek_frame{-1};

    [[nodiscard]] bool open(
        const std::array<fs::path,5>& names,std::string& error) override {
        const std::array<std::string,5> required{
            "bad.bik","normal.bik","ok.bik","good.bik","end.bik"};
        for(std::size_t i=0;i<names.size();++i)
            if(names[i].filename().string()!=required[i]) {
                error="nonoriginal Spud Skate movie order";return false;
            }
        ++opens;
        error.clear();return true;
    }
    [[nodiscard]] bool observe(
        fg::SpudSkateMovieFrame& result,std::string& error) override {
        if(clock_cursor>=real_decoded_frames.size()) {
            error="original Bink decoder has no further decoded frame";
            return false;
        }
        result=real_decoded_frames[clock_cursor++];
        error.clear();return true;
    }
    [[nodiscard]] bool select_quality(
        skate::StuntQuality q,std::string& error) override {
        quality_selections.push_back(q);error.clear();return true;
    }
    [[nodiscard]] bool resynchronize_all(
        int target,std::string& error) override {
        ++seek_calls;seek_frame=target;error.clear();return true;
    }
    [[nodiscard]] bool begin_end_movie(std::string& error) override {
        ++end_movies;error.clear();return true;
    }
    [[nodiscard]] bool close(std::string& error) override {
        ++closes;error.clear();return true;
    }
};

static void write_original_tables(const fs::path& root) {
    const auto folder=root/"Data"/"SubGameSpudSkate";
    fs::create_directories(folder);
    for(int difficulty=1;difficulty<=3;++difficulty) {
        std::ofstream timing(folder/("spuddata"+
            std::to_string(difficulty)+".txt"));
        timing<<8<<"\n";
        constexpr int starts[]{85,157,226,295,379,465,547,620};
        for(int start:starts)
            timing<<start<<" "<<start+12<<" "<<start+22<<" "
                  <<start+31<<"\n";
        constexpr int returns[]{134,202,271,360,426,531,592,679};
        for(int value:returns)timing<<value<<" ";
        timing<<"\n44 660\n";
    }
    std::ofstream snd(folder/"soundinfo.txt");
    for(int stunt=0;stunt<8;++stunt)
      for(int quality=0;quality<4;++quality)
        for(int candidate=0;candidate<5;++candidate)
            snd<<750+stunt*20+quality*5+candidate<<" ";
    snd<<"\n124 180 260 330 415 500 580 665\n";
    for(const auto& name:{
        "bad.bik","normal.bik","ok.bik","good.bik","end.bik"}) {
        std::ofstream movie(folder/name,std::ios::binary);
        movie<<"Original Bink data belongs to the physical decoder";
    }
}

int main() {
    const auto path=fs::temp_directory_path()/
        "btb_native_spud_skate_original_clock_test";
    fs::remove_all(path);
    write_original_tables(path);

    // Without a real movie service, neither an activity session nor
    // a fake win can initialize: the native host uses this fail-closed
    // source path until the physical five-stream decoder is wired.
    {
        fg::SpudSkateDriver absent(path,nullptr);
        std::string error;
        assert(!absent.initialize(0,error));
        assert(error.find("Bink provider")!=std::string::npos);
        assert(!absent.initialized());
    }

    btb::retail::OriginalRetailRandom shared_rng{77};
    auto movies=std::make_unique<DecodedFiveBinks>();
    auto* source=movies.get();
    source->real_decoded_frames={
        {85,false,false,true},   // original first stunt click
        {124,false,false,true},  // frame-124 sound + score
        {660,true,false,true},   // 1st 660 = special final-stunt score
        {44,false,false,true},   // actual four-stream seek to frame 44
        {660,false,false,true},  // 2nd 660 = end.bik
        {-1,false,false,false}, // first real end-movie decode
        {-1,false,true,false},  // decoded end, managed voice still playing
        {-1,false,true,false}   // after sound stops, replay transition
    };
    fg::SpudSkateDriver game(path,std::move(movies),&shared_rng);
    fg::RetailMenuState menu{};
    menu.variant_selection_origin=
        btb::game_flow::State::SpudSkatePregameUpdate;
    menu.source_variant=0;
    game.configure_menu_state(menu);
    std::string error{};
    assert(game.initialize(0,error));
    assert(source->opens==1);
    auto frame=game.advance({.click_pulse=true});
    assert(!frame.fatal_error);
    assert(game.score()==0);
    assert(!source->quality_selections.empty());
    assert(source->quality_selections.back()==skate::StuntQuality::Normal);
    assert(frame.audio.size()==2); // original music and 758
    assert(frame.audio[1].sound_id==758);
    assert(frame.progress_writes.empty());

    frame=game.advance({});
    assert(!frame.fatal_error);
    assert(game.score()==1);
    assert(frame.audio.size()==1);
    assert(frame.progress_writes.empty());

    frame=game.advance({});
    assert(!frame.fatal_error);
    assert(game.pass_count()==1);
    assert(game.score()==2);
    assert(source->seek_calls==1 && source->seek_frame==44);
    assert(source->end_movies==0);

    frame=game.advance({});
    assert(!frame.fatal_error);
    frame=game.advance({});
    assert(!frame.fatal_error);
    assert(game.pass_count()==2);
    assert(source->end_movies==1);
    assert(game.phase()==skate::PlaybackPhase::EndMovie);
    assert(frame.progress_writes.empty());

    const auto before_result_rng=shared_rng.state();
    auto expected_rng=shared_rng;
    const auto expected_low1=773+(expected_rng.next_rand()%2);
    const auto expected_low2=775+(expected_rng.next_rand()%3);
    frame=game.advance({});
    assert(!frame.fatal_error);
    assert(frame.progress_writes.size()==1);
    assert(frame.progress_writes[0].slot==btb::progress::Slot::SpudSkate);
    assert(frame.audio.size()==3); // stop previous sounds, two final voices
    assert(frame.audio[1].sound_id==expected_low1);
    assert(frame.audio[2].sound_id==expected_low2);
    assert(shared_rng.state()!=before_result_rng);
    assert(shared_rng.state()==expected_rng.state());
    assert(!frame.save_and_unload);

    frame=game.advance({.managed_sound_playing=true});
    assert(!frame.fatal_error);
    assert(!frame.save_and_unload);
    assert(frame.progress_writes.empty());
    assert(shared_rng.state()==expected_rng.state());

    frame=game.advance({});
    assert(!frame.fatal_error);
    assert(frame.save_and_unload);
    assert(frame.next_outer_state==0x3C);
    assert(frame.next_saved_state==0x16);
    assert(frame.next_ui_context==0x1A);
    assert(frame.progress_writes.empty());
    assert(shared_rng.state()==expected_rng.state());
    assert(game.unload(error));
    assert(source->closes==1);

    // Original frame observation is authoritative. A missing or
    // unsynchronized Bink frame must fail, not advance source score.
    auto invalid=std::make_unique<DecodedFiveBinks>();
    invalid->real_decoded_frames={{-1,false,false,false}};
    fg::SpudSkateDriver blocked(path,std::move(invalid));
    assert(blocked.initialize(0,error));
    const auto rejected=blocked.advance({.click_pulse=true});
    assert(rejected.fatal_error &&
           rejected.fatal_error->find("synchronized original")!=
               std::string::npos);
    assert(blocked.score()==0);
    assert(blocked.unload(error));

    fs::remove_all(path);
}
