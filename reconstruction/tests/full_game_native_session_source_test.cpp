#include "btb/full_game_native_session.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace {
namespace fs=std::filesystem;
using namespace btb::full_game;

void touch(const fs::path& root,const std::string& relative) {
    const auto path=root/fs::path(relative);
    fs::create_directories(path.parent_path());
    std::ofstream(path,std::ios::binary) << "original fixture";
}

OriginalWalkthroughCatalog walkthrough_fixture() {
    std::ostringstream txt;
    for (int i=0;i<10;++i) {
        txt << "data/movies/bink_" << i << ".bik "
            << "data/sound/help_" << i << ".wav\n";
    }
    for (int i=0;i<3;++i) {
        txt << "NULL data/sound/subhelp_" << i << ".wav\n";
    }
    OriginalWalkthroughCatalog table;
    std::istringstream source(txt.str());
    std::string error;
    assert(table.read(source,error));
    return table;
}

OriginalStartupMovieCatalog startup_fixture() {
    std::ostringstream txt;
    for (int i=0;i<8;++i) {
        txt << "Data/movies/start_" << i << ".bik\n";
    }
    OriginalStartupMovieCatalog table;
    std::istringstream source(txt.str());
    std::string error;
    assert(table.read(source,error));
    return table;
}

class Device final : public NativeGameDevice {
public:
    PhysicalFrameObservation sample{};
    int observe_count{};
    int global_open{};
    int walk_open{};
    int walk_close{};
    int sound_count{};
    int draw_count{};
    int present_count{};
    bool fail_draw{};
    std::string last_movie{};
    std::string last_help{};
    std::vector<Draw> received_draws{};

    bool observe(PhysicalFrameObservation& out,
                 std::string& error) override {
        ++observe_count; out=sample; error.clear(); return true;
    }
    bool open_global_movie(
        const fs::path& movie,std::string& error) override {
        ++global_open;
        last_movie=movie.generic_string();
        error.clear(); return true;
    }
    bool open_walkthrough_movie(
        const fs::path& movie,const fs::path& help,
        std::string& error) override {
        ++walk_open;
        last_movie=movie.generic_string();
        last_help=help.generic_string();
        error.clear(); return true;
    }
    bool close_walkthrough_movie(std::string& error) override {
        ++walk_close;error.clear();return true;
    }
    bool apply_sound(const ResolvedSoundEffect&,
                     std::string& error) override {
        ++sound_count;error.clear();return true;
    }
    bool draw_ordered(const std::vector<ResolvedDraw>& draws,
                      std::string& error) override {
        ++draw_count;
        if (fail_draw) {
            error="forced DirectDraw provider failure";
            return false;
        }
        for (const auto& draw : draws) {
            received_draws.push_back(draw.original);
        }
        error.clear();return true;
    }
    bool present(std::string& error) override {
        ++present_count;error.clear();return true;
    }
};

class Instruction final : public FrontEndDriver {
public:
    bool initialize(
        const btb::progress::Record&,
        btb::progress::FinaleGate&,
        std::string& error) override {
        error.clear();return true;
    }
    void synchronize_finale_gate(
        btb::progress::FinaleGate) noexcept override {}
    ActivityFrameOutput advance(const ActivityFrameInput& input) override {
        ActivityFrameOutput result;
        result.draws.push_back({
            "Data/ui/original.bmp",0,0,std::nullopt,false
        });
        if (input.click_pulse) {
            result.negative_ui_action=-5;
        }
        return result;
    }
};
}

int main() {
    using namespace btb::full_game;
    using btb::game_flow::State;
    const auto directory=fs::temp_directory_path()/
        "btb_original_native_session_source";
    fs::remove_all(directory);
    touch(directory,"data/movies/bink_5.bik");
    touch(directory,"data/sound/help_5.wav");
    touch(directory,"data/movies/bink_0.bik");
    touch(directory,"data/sound/help_0.wav");
    touch(directory,"Data/movies/start_0.bik");
    touch(directory,"Data/ui/original.bmp");

    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1; // Adventure Golf

        Device device;
        NativeGameSession session{
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        };
        game.set_outer_state(State::GolfPregameSetup);

        const auto initialized=session.tick({},false,false);
        assert(!initialized.failure);
        assert(initialized.submitted);
        assert(initialized.walkthrough_opened);
        assert(initialized.prepared.game.original_walkthrough_index == 5);
        assert(!initialized.global_movie_opened);
        assert(device.walk_open == 1);
        assert(device.last_movie.find("bink_5.bik") != std::string::npos);
        assert(device.last_help.find("help_5.wav") != std::string::npos);

        const auto instruction=session.tick({},false,false);
        assert(!instruction.failure);
        assert(instruction.submitted);
        assert(instruction.prepared.resources.draws.size() == 1);
        assert(device.received_draws.size() == 1);
        assert(device.received_draws[0].source_asset ==
               "Data/ui/original.bmp");

        // Pre-game Start must close the independent looping Bink before
        // routing to Golf initializer. Never close on a difficulty choice.
        const auto start=session.tick({.click_pulse=true},true,false);
        assert(!start.failure);
        assert(start.submitted);
        assert(start.walkthrough_closed);
        assert(start.prepared.game.pregame_action);
        assert(start.prepared.game.pregame_action->start_activity);
        assert(device.walk_close == 1);
        assert(!session.has_walkthrough());
        assert(game.globals().dispatcher.current_state == 0x36);
    }

    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::HerdingPregameSetup,std::make_unique<Instruction>()));
        Device device;
        NativeGameSession session{
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        };
        game.set_outer_state(State::HerdingPregameSetup);
        const auto initial=session.tick({},false,false);
        assert(initial.submitted);
        assert(initial.global_movie_opened);
        assert(initial.walkthrough_opened);
        assert(device.global_open == 1);
        assert(game.globals().dispatcher.generic_screen_mode == 14);
        device.sample.global_movie_finished = false;
        const auto waiting=session.tick({},false,false);
        assert(waiting.prepared.game.kind == FrameKind::Intercept);
        assert(game.globals().dispatcher.current_state == 0x0C);

        device.sample.global_movie_finished = true;
        const auto resumed=session.tick({.click_pulse=true},true,false);
        assert(resumed.prepared.game.kind == FrameKind::FrontEndUpdated);
        assert(resumed.prepared.game.clear_input_pulse);
        assert(!resumed.prepared.game.pregame_action);
        assert(game.globals().dispatcher.current_state == 0x0D);
        assert(device.walk_close == 0);
    }

    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1;
        Device device;
        device.fail_draw=true;
        NativeGameSession session{
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        };
        game.set_outer_state(State::GolfPregameSetup);
        assert(session.tick({},false,false).submitted);
        const auto broken=session.tick({},false,false);
        assert(broken.failure);
        assert(broken.failure->stage==
               PhysicalPresentationStage::SubmitDraw);
        assert(!broken.submitted);
        const auto stopped=session.tick({},false,false);
        assert(stopped.failure);
        assert(!stopped.submitted);
    }

    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1;
        Device device;
        NativeGameSession session{
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        };
        game.set_outer_state(State::GolfPregameSetup);
        assert(session.tick({},false,false).submitted);
        fs::remove(directory/"Data/ui/original.bmp");
        const auto missing=session.tick({},false,false);
        assert(missing.failure);
        assert(missing.failure->stage==
               PhysicalPresentationStage::AssetResolve);
        assert(device.draw_count == 1);
    }
    fs::remove_all(directory);
}
