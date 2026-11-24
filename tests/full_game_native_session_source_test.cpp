#include "btb/full_game_native_session.hpp"

#include <cassert>
#include <cstdio>
#include <cstdlib>
// The original test file was not registered with CMake. Native Release
// builds define NDEBUG, which makes assert(...) vanish, including calls
// with state-changing expressions. Keep every check executable in Release.
#undef assert
#define assert(condition) do { \
    if(!(condition)){ \
        std::fprintf(stderr,"native session assertion failed: %s at %s:%d\\n", \
                     #condition,__FILE__,__LINE__); \
        std::abort(); \
    } \
} while(false)
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace {
int report_failure(int code) {
    std::fprintf(stderr,"native two-phase session regression failed: %d\n",code);
    return code;
}
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
    int global_close{};
    int walk_open{};
    int walk_close{};
    int sound_count{};
    int draw_count{};
    int present_count{};
    int print_count{};
    int replay_captures{};
    int replay_draws{};
    bool fail_print{};
    std::vector<std::string> operation_order{};
    bool fail_draw{};
    bool fail_present{};
    std::string last_movie{};
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
    bool close_global_movie(std::string& error) override {
        ++global_close;error.clear();return true;
    }
    bool open_walkthrough_movie(
        const fs::path& movie,
        std::string& error) override {
        ++walk_open;
        last_movie=movie.generic_string();
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
        operation_order.push_back("draw");
        if (fail_draw) {
            error="forced DirectDraw provider failure";
            return false;
        }
        for (const auto& draw : draws) {
            received_draws.push_back(draw.original);
        }
        error.clear();return true;
    }
    bool capture_replay_underlay(std::string& error) override {
        ++replay_captures;operation_order.push_back("capture");
        error.clear();return true;
    }
    bool draw_replay_ordered(const std::vector<ResolvedDraw>& draws,
                            std::string& error) override {
        if(!replay_captures){error="no captured source frame";return false;}
        ++replay_draws;operation_order.push_back("replay");
        for(const auto& d:draws)received_draws.push_back(d.original);
        error.clear();return true;
    }
    bool print_current_frame(std::string& error) override {
        ++print_count;
        operation_order.push_back("print");
        if(fail_print) {
            error="the original Windows printer rejected this print";
            return false;
        }
        error.clear();return true;
    }
    bool present(std::string& error) override {
        ++present_count;
        operation_order.push_back("present");
        if(fail_present){error="forced native presentation failure";return false;}
        error.clear();return true;
    }
};

// Frame fixture models Fireworks state-15 preserving an already-decoded
// original Bink surface. No fake Bink bytes or timers are presented.
class SourceMovieActivity final : public ActivityDriver {
public:
    int frames{};
    [[nodiscard]] bool initialize(int,std::string& error) override {
        error.clear();return true;
    }
    [[nodiscard]] ActivityFrameOutput advance(
        const ActivityFrameInput& input) override {
        ++frames;
        ActivityFrameOutput output{};
        if(!input.original_global_movie_finished)
            output.retain_original_global_movie_pixels=true;
        else
            output.draws.push_back({
                "Data/ui/original.bmp",0,0,std::nullopt,false
            });
        return output;
    }
    [[nodiscard]] bool unload(std::string& error) override {
        error.clear();return true;
    }
};

class SourcePrintActivity final : public ActivityDriver {
public:
    bool initialize(int,std::string& error) override {
        error.clear();return true;
    }
    ActivityFrameOutput advance(const ActivityFrameInput& input) override {
        ActivityFrameOutput out;
        out.draws.push_back({
            "Data/ui/original.bmp",0,0,std::nullopt,false
        });
        out.request_original_frame_print=input.click_pulse;
        return out;
    }
    bool unload(std::string& error) override {
        error.clear();return true;
    }
};

class SourceReplayActivity final : public ActivityDriver {
public:
    bool initialize(int,std::string& error) override {error.clear();return true;}
    ActivityFrameOutput advance(const ActivityFrameInput& input) override {
        ActivityFrameOutput out;
        out.draws.push_back({"Data/ui/original.bmp",0,0});
        if(input.click_pulse) {
            out.save_and_unload=true;
            out.next_saved_state=static_cast<int>(btb::game_flow::State::GolfInit);
            out.next_replay_class=1;
            out.next_outer_state=static_cast<int>(btb::game_flow::State::PlayAgainYesNoSetup);
        }
        return out;
    }
    bool unload(std::string& error) override {error.clear();return true;}
};

class ReplayMenu final : public FrontEndDriver {
public:
    bool initialize(const btb::progress::Record&,btb::progress::FinaleGate&,
                    std::string& error) override {error.clear();return true;}
    void synchronize_finale_gate(btb::progress::FinaleGate) noexcept override {}
    ActivityFrameOutput advance(const ActivityFrameInput& input) override {
        ActivityFrameOutput out;
        out.draws.push_back({"Data/ui/replay.bmp",0,0,std::nullopt,true});
        if(input.click_pulse)out.negative_ui_action=-20;
        return out;
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
    // NativeGameSession owns the 1100-entry sound catalog and retail sound
    // manager (tens of KiB each). MSVC can reserve distinct stack slots for
    // all twenty scoped fixtures in this function, exceeding the default
    // Windows executable stack. Allocate each session separately, keeping
    // its original scope, destruction order, and every assertion intact.
    const auto directory=fs::temp_directory_path()/
        "btb_original_native_session_source";
    fs::remove_all(directory);
    touch(directory,"data/movies/bink_5.bik");
    touch(directory,"data/movies/bink_0.bik");
    touch(directory,"Data/movies/start_0.bik");
    touch(directory,"Data/ui/original.bmp");
    touch(directory,"Data/ui/replay.bmp");
    touch(directory,"Data/sound/replay.wav");

    {
        auto game_storage=std::make_unique<GameRoot>();auto& game=*game_storage;
        assert(game.select_profile(0));
        game.install(ActivityId::Golf,std::make_unique<SourceReplayActivity>());
        assert(game.install_generic_front_end_pair(
            State::PlayAgainYesNoSetup,std::make_unique<ReplayMenu>()));
        assert(game.install_generic_front_end_pair(
            State::PlayAgainDifficultySetup,std::make_unique<ReplayMenu>()));
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},startup_fixture(),walkthrough_fixture());
        auto& session=*session_storage;
        std::istringstream sounds("replay.wav 573\n");
        assert(session.load_sound_catalog(sounds).success);
        game.set_outer_state(State::GolfInit);
        assert(session.tick({},false,false).submitted);
        assert(session.tick({},false,false).submitted);
        device.operation_order.clear();
        const auto exit=session.tick({.click_pulse=true},true,false);
        assert(exit.submitted&&exit.prepared.game.replay_preparation);
        assert(device.operation_order==std::vector<std::string>({"draw","capture","present"}));
        assert(device.replay_captures==1);
        assert(session.tick({},false,false).submitted); // setup draws underlay
        assert(device.replay_draws==1);
        const auto replay=session.tick({.click_pulse=true},true,false);
        assert(replay.submitted&&device.replay_draws==2);
        assert(device.received_draws.back().color_keyed);
        assert(game.globals().dispatcher.current_state==static_cast<int>(State::PlayAgainDifficultySetup));
        assert(session.tick({},false,false).submitted);
        assert(device.replay_draws==3&&device.replay_captures==1);
    }

    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1; // Adventure Golf

        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::GolfPregameSetup);

        const auto initialized=session.tick({},false,false);
        assert(!initialized.failure);
        assert(initialized.submitted);
        assert(initialized.walkthrough_opened);
        assert(initialized.prepared.game.original_walkthrough_index == 5);
        assert(!initialized.global_movie_opened);
        assert(device.walk_open == 1);
        assert(device.last_movie.find("bink_5.bik") != std::string::npos);
        // The retail table names obsolete help WAVs, but 004281D0
        // never opens them. This fixture deliberately has no such WAVs.
        assert(!fs::exists(directory/"data/sound/help_5.wav"));

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
        // A missing Golf adapter cannot masquerade as a successful
        // gameplay frame. The native host stops with a clear error.
        const auto unhosted=session.tick({},false,false);
        assert(unhosted.failure);
        assert(unhosted.failure->stage ==
               PhysicalPresentationStage::GameAdvance);
        assert(!unhosted.submitted);
        assert(session.failed());
    }

    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::HerdingPregameSetup,std::make_unique<Instruction>()));
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::HerdingPregameSetup);
        const auto initial=session.tick({},false,false);
        assert(initial.submitted);
        assert(initial.global_movie_opened);
        assert(initial.walkthrough_opened);
        assert(device.global_open == 1);
        assert(game.globals().dispatcher.generic_screen_mode == 14);
        device.sample.global_movie_finished = false;
        const auto prior_draws=device.draw_count;
        const auto waiting=session.tick({},false,false);
        assert(waiting.prepared.game.kind == FrameKind::Intercept);
        assert(waiting.submitted);
        assert(game.globals().dispatcher.current_state == 0x0C);
        // Original Bink decoder has already written its pixels during
        // observe(); an empty normal draw would erase those movie pixels.
        assert(device.draw_count == prior_draws);
        assert(device.present_count == 2);

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
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::GolfPregameSetup);
        assert(session.tick({},false,false).submitted);
        // The first setup frame legitimately draws. Force failure only
        // for the following instruction frame so the sticky-failure
        // behavior is tested rather than failing the setup frame.
        device.fail_draw=true;
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
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::GolfPregameSetup);
        assert(session.tick({},false,false).submitted);
        fs::remove(directory/"Data/ui/original.bmp");
        const auto missing=session.tick({},false,false);
        assert(missing.failure);
        assert(missing.failure->stage==
               PhysicalPresentationStage::AssetResolve);
        assert(missing.failure->message.find("Data/ui/original.bmp")!=
               std::string::npos);
        assert(missing.failure->message.find("Unresolved bitmap/WAV counts: 1/0")!=
               std::string::npos);
        assert(device.draw_count == 1);
    }
    // Fireworks state 15: the Bink service has already decoded a frame
    // into the shared DD7 render target before GameRoot::advance. Do not
    // invoke the ordinary activity draw path, which would clear that movie.
    // The original true decoder EOS releases that protection on a later
    // frame and restores regular source bitmap presentation.
    touch(directory,"Data/ui/original.bmp");
    {
        GameRoot game;
        assert(game.select_profile(0));
        auto movie=std::make_unique<SourceMovieActivity>();
        auto* raw_movie=movie.get();
        game.install(ActivityId::Fireworks,std::move(movie));
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::FireworksInit);
        const auto setup=session.tick({},false,false,true);
        assert(!setup.failure && setup.ready_for_present);
        const int before_movie_draws=device.draw_count;
        std::string present_error;
        assert(session.present_prepared(present_error));
        const int before_movie_presents=device.present_count;

        device.sample.global_movie_finished=false;
        const auto movie_frame=session.tick({},false,false,true);
        assert(!movie_frame.failure && movie_frame.ready_for_present);
        assert(movie_frame.prepared.game.effects
                   .retain_original_global_movie_pixels);
        assert(raw_movie->frames==1);
        assert(device.draw_count==before_movie_draws);
        assert(session.present_prepared(present_error));
        assert(device.present_count==before_movie_presents+1);

        device.sample.global_movie_finished=true;
        const auto finished=session.tick({},false,false,true);
        assert(!finished.failure && finished.ready_for_present);
        assert(!finished.prepared.game.effects
                    .retain_original_global_movie_pixels);
        assert(raw_movie->frames==2);
        assert(device.draw_count==before_movie_draws+1);
        assert(session.present_prepared(present_error));
        assert(device.present_count==before_movie_presents+2);
    }

    // A click requests the actual Windows print service after original
    // DD7 draw and BEFORE Flip/Blt. The core never invents a print result.
    {
        GameRoot game;
        assert(game.select_profile(0));
        game.install(ActivityId::Fireworks,
            std::make_unique<SourcePrintActivity>());
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::FireworksInit);
        const auto initialized=session.tick({},false,false,true);
        assert(initialized.ready_for_present && !initialized.failure);
        std::string error;
        assert(session.present_prepared(error));
        device.operation_order.clear();
        const auto click=session.tick({.click_pulse=true},true,false,true);
        assert(!click.failure && click.ready_for_present);
        assert(click.prepared.game.effects.request_original_frame_print);
        assert(device.print_count==1);
        assert((device.operation_order==std::vector<std::string>{
            "draw","print"}));
        assert(session.present_prepared(error));
        assert((device.operation_order==std::vector<std::string>{
            "draw","print","present"}));
        const auto next=session.tick({},false,false,true);
        assert(!next.failure && next.ready_for_present);
        assert(device.print_count==1); // one print per requested click
        assert(session.present_prepared(error));
    }
    {
        GameRoot game;
        assert(game.select_profile(0));
        game.install(ActivityId::Fireworks,
            std::make_unique<SourcePrintActivity>());
        Device device;
        device.fail_print=true;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::FireworksInit);
        const auto initialized=session.tick({},false,false,true);
        assert(initialized.ready_for_present);
        std::string error;
        assert(session.present_prepared(error));
        const auto rejected=session.tick(
            {.click_pulse=true},true,false,true);
        assert(rejected.failure);
        assert(rejected.failure->stage==PhysicalPresentationStage::Print);
        assert(rejected.failure->message.find("printer")!=std::string::npos);
        assert(!rejected.ready_for_present);
        assert(device.print_count==1);
        assert(device.present_count==1);
    }

    // Exercise the REAL deferred two-phase NativeGameSession path used
    // by original RunActiveGameFrame (00402440) before DD7 presentation.
    touch(directory,"Data/ui/original.bmp");
    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1;
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::GolfPregameSetup);
        const auto prepared=session.tick({},false,false,true);
        if(prepared.failure || prepared.submitted ||
           !prepared.ready_for_present || device.present_count!=0)
            return report_failure(61);
        std::string present_error;
        if(!session.present_prepared(present_error) ||
           device.present_count!=1)return report_failure(62);
        // No double-present without another prepared game frame.
        if(session.present_prepared(present_error) ||
           device.present_count!=1)return report_failure(63);
        const auto second=session.tick({},false,false,true);
        if(second.failure || !second.ready_for_present ||
           second.submitted || device.present_count!=1)return report_failure(64);
        session.discard_prepared();
        // A cooperative-level retry drops only the prepared presentation,
        // not all subsequent game states or the entire game session.
        const auto third=session.tick({},false,false,true);
        if(third.failure||!third.ready_for_present||
           !session.present_prepared(present_error) ||
           device.present_count!=2)return report_failure(65);
    }
    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1;
        Device device;
        device.fail_present=true;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::GolfPregameSetup);
        const auto prepared=session.tick({},false,false,true);
        if(prepared.failure || !prepared.ready_for_present)return report_failure(66);
        std::string present_error;
        if(session.present_prepared(present_error)||
           !session.failed()||present_error.empty())return report_failure(67);
    }
    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1;
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::GolfPregameSetup);
        const auto prepared=session.tick({},false,false,true);
        if(prepared.failure || !prepared.ready_for_present)return report_failure(68);
        const auto illegally_overlapping=session.tick({},false,false,true);
        if(!illegally_overlapping.failure ||
           illegally_overlapping.failure->stage!=
               PhysicalPresentationStage::Present ||
           !session.failed())return report_failure(69);
    }
    // Explicit video-free mode is a development/gameplay escape hatch only.
    // It must not call decoder APIs, invent audio, or claim movie playback.
    // The normal mode above continues to exercise genuine movie contracts.
    fs::remove(directory/"data/movies/bink_5.bik");
    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1;
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture(),true
        );
        auto& session=*session_storage;
        assert(session.skipping_video());
        game.set_outer_state(State::GolfPregameSetup);
        const auto setup=session.tick({},false,false);
        assert(setup.submitted && !setup.failure);
        assert(setup.prepared.game.original_walkthrough_index==5);
        assert(!setup.walkthrough_opened);
        assert(!session.has_walkthrough());
        assert(device.walk_open==0 && device.global_open==0);
        const auto start=session.tick({.click_pulse=true},true,false);
        assert(start.submitted && !start.failure);
        assert(start.prepared.game.pregame_action);
        assert(start.prepared.game.pregame_action->start_activity);
        assert(!start.walkthrough_closed);
        assert(device.walk_close==0);
        assert(game.globals().dispatcher.current_state ==
               static_cast<int>(State::GolfInit));
    }
    {
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::SquirrelPregameSetup,std::make_unique<Instruction>()));
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture(),true
        );
        auto& session=*session_storage;
        game.set_outer_state(State::SquirrelPregameSetup);
        const auto setup=session.tick({},false,false);
        assert(setup.submitted && !setup.failure);
        assert(setup.prepared.game.original_global_intro_movie_index);
        assert(device.global_open==0 && device.walk_open==0);
        assert(game.globals().dispatcher.generic_screen_mode==14);
        device.sample.global_movie_finished=false;
        const auto resumed=session.tick({},false,false);
        assert(resumed.submitted && !resumed.failure);
        assert(resumed.prepared.game.kind==FrameKind::FrontEndUpdated);
        assert(game.globals().dispatcher.current_state ==
               static_cast<int>(State::SquirrelPregameUpdate));
        assert(device.global_open==0 && device.walk_open==0);
    }
    {
        // Without --skip-bink, absent retail media is still a hard error.
        GameRoot game;
        assert(game.select_profile(0));
        assert(game.install_pregame_front_end_pair(
            State::GolfPregameSetup,std::make_unique<Instruction>()));
        game.globals().menu.selected_subgame=1;
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::GolfPregameSetup);
        const auto blocked=session.tick({},false,false);
        assert(blocked.failure);
        assert(blocked.failure->stage==PhysicalPresentationStage::MovieResolve);
        assert(blocked.failure->message.find("bink_5.bik")!=std::string::npos);
        assert(device.walk_open==0);
        assert(!blocked.submitted);
    }
    // Retail state 0 runs each original videoseq.txt Bink in order, keeps
    // the decoded surface visible while waiting for EOS, and starts the
    // original enter-name backing WAV only after the final clip completes.
    {
        touch(directory,"Data/movies/thqlogo.bik");
        touch(directory,"Data/movies/bbclogo.bik");
        touch(directory,"Data/music/entername.wav");
        std::istringstream sequence_text(
            "Data/movies/thqlogo.bik 1\n"
            "Data/movies/bbclogo.bik 0\n");
        OriginalVideoSequenceCatalog sequence;
        std::string error;
        assert(sequence.read(sequence_text,error));

        GameRoot game;
        assert(game.configure_startup_video_sequence(
            sequence.entries(),error));
        game.set_outer_state(State::StartupMoviesThenProfileSetup);
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        auto first=session.tick({},false,false);
        assert(first.submitted && !first.failure);
        assert(first.global_movie_opened);
        assert(first.prepared.game.original_startup_sequence_movie_path);
        assert(device.global_open==1);
        assert(device.last_movie.find("thqlogo.bik")!=std::string::npos);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::StartupMoviesThenProfileSetup));
        const int draws_before=device.draw_count;

        device.sample.global_movie_finished=false;
        auto waiting=session.tick({.click_pulse=true},true,false);
        assert(waiting.submitted && !waiting.failure);
        assert(!waiting.global_movie_opened);
        assert(device.global_open==1);
        assert(device.draw_count==draws_before);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::StartupMoviesThenProfileSetup));

        device.sample.global_movie_finished=true;
        auto first_done=session.tick({},false,false);
        assert(first_done.submitted && !first_done.failure);
        assert(!first_done.global_movie_opened);
        assert(device.global_open==1);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::StartupMoviesThenProfileSetup));

        device.sample.global_movie_finished=false;
        auto second=session.tick({},false,false);
        assert(second.submitted && !second.failure);
        assert(second.global_movie_opened);
        assert(device.global_open==2);
        assert(device.last_movie.find("bbclogo.bik")!=std::string::npos);
        assert(device.draw_count==draws_before);

        device.sample.global_movie_finished=true;
        const int sounds_before=device.sound_count;
        auto final_movie_done=session.tick({},false,false);
        assert(final_movie_done.submitted && !final_movie_done.failure);
        assert(!final_movie_done.global_movie_opened);
        assert(device.global_open==2);
        assert(device.sound_count==sounds_before+1);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::PlayerProfileAndNameEntry));
        assert(final_movie_done.prepared.game.state_changed);
        assert(final_movie_done.prepared.game.effects.retain_original_global_movie_pixels);
        assert(final_movie_done.prepared.game.effects.audio.size()==1);
        assert(final_movie_done.prepared.game.effects.audio[0].operation==
               AudioOperation::StartBackingTrack);
        assert(final_movie_done.prepared.game.effects.audio[0].source_asset==
               "Data/music/entername.wav");
        assert(device.draw_count==draws_before);
    }

    // After 0x0042DCC0 accepts a name, retail enters the dedicated 0x41
    // scene-set movie and then Activity Select. The optional non-retail
    // video-free session may skip this particular source-known transition.
    for(const auto skipped_movie_state:{State::SceneSetMovieTransition}) {
        GameRoot game;
        assert(game.select_profile(2));
        game.install_activity_select(std::make_unique<Instruction>());
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture(),true
        );
        auto& session=*session_storage;
        game.set_outer_state(skipped_movie_state);
        const auto reached=session.tick({.click_pulse=true},true,false);
        assert(reached.submitted && !reached.failure);
        assert(reached.prepared.game.kind==FrameKind::FrontEndInitialized);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::ActivitySelectUpdate));
        assert(device.global_open==0 && device.walk_open==0);
        assert(!reached.global_movie_opened);
    }
    {
        // Real 0x41: open sceneset.bik exactly once, preserve its decoded
        // DD7 pixels until physical Bink EOS, then reach Activity Select.
        // A fake Device only simulates native EOS; the production path
        // still calls the installed decoder and never fabricates it.
        touch(directory,"Data/movies/sceneset.bik");
        GameRoot game;
        assert(game.select_profile(2));
        game.install_activity_select(std::make_unique<Instruction>());
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::SceneSetMovieTransition);
        const int prior_draws=device.draw_count;
        const auto opened=session.tick({},false,false);
        assert(!opened.failure && opened.submitted);
        assert(opened.global_movie_opened);
        assert(device.global_open==1);
        assert(device.last_movie.find("sceneset.bik")!=std::string::npos);
        assert(device.draw_count==prior_draws);
        for(int i=0;i<3;++i) {
            device.sample.global_movie_finished=false;
            const auto waiting=session.tick({},false,false);
            assert(!waiting.failure && waiting.submitted);
            assert(!waiting.global_movie_opened);
            assert(!waiting.prepared.game.state_changed);
            assert(game.globals().dispatcher.current_state==
                   static_cast<int>(State::SceneSetMovieTransition));
            assert(device.global_open==1);
            assert(device.global_close==0);
            assert(device.draw_count==prior_draws);
        }
        device.sample.global_movie_finished=true;
        const auto completed=session.tick({},false,false);
        assert(!completed.failure && completed.submitted);
        assert(completed.prepared.game.state_changed);
        assert(device.global_open==1);
        assert(device.draw_count==prior_draws);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::ActivitySelectSetup));
        device.sample.global_movie_finished=false;
        const auto selected=session.tick({},false,false);
        assert(!selected.failure && selected.submitted);
        assert(selected.prepared.game.kind==FrameKind::FrontEndInitialized);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::ActivitySelectUpdate));
        assert(device.draw_count==prior_draws+1);
    }
    {
        // Mode 0 permits an input skip of the final intro clip. The real
        // global movie is explicitly closed before the profile screen begins.
        touch(directory,"Data/movies/introbig1.bik");
        touch(directory,"Data/music/entername.wav");
        std::istringstream sequence_text("Data/movies/introbig1.bik 0\n");
        OriginalVideoSequenceCatalog sequence;
        std::string error;
        assert(sequence.read(sequence_text,error));
        GameRoot game;
        assert(game.configure_startup_video_sequence(sequence.entries(),error));
        game.set_outer_state(State::StartupMoviesThenProfileSetup);
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        const auto opened=session.tick({},false,false);
        assert(opened.submitted && !opened.failure && opened.global_movie_opened);
        device.sample.global_movie_finished=false;
        const int sounds_before=device.sound_count;
        const auto skipped=session.tick({.click_pulse=true},true,false);
        assert(skipped.submitted && !skipped.failure);
        assert(skipped.prepared.game.close_original_global_movie);
        assert(device.global_close==1);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::PlayerProfileAndNameEntry));
        assert(device.sound_count==sounds_before+1);
        assert(skipped.prepared.game.effects.audio.size()==1);
        assert(skipped.prepared.game.effects.audio[0].source_asset==
               "Data/music/entername.wav");
    }
    {
        // State 0x41 also accepts the original click-to-skip pulse and closes
        // the Bink player before Activity Select starts drawing next frame.
        touch(directory,"Data/movies/sceneset.bik");
        GameRoot game;
        assert(game.select_profile(2));
        game.install_activity_select(std::make_unique<Instruction>());
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::SceneSetMovieTransition);
        const auto opened=session.tick({},false,false);
        assert(opened.submitted && opened.global_movie_opened);
        device.sample.global_movie_finished=false;
        const auto skipped=session.tick({.click_pulse=true},true,false);
        assert(skipped.submitted && !skipped.failure);
        assert(skipped.prepared.game.close_original_global_movie);
        assert(device.global_close==1);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::ActivitySelectSetup));
        assert(device.draw_count==0);
        const auto selected=session.tick({},false,false);
        assert(selected.submitted && !selected.failure);
        assert(selected.prepared.game.kind==FrameKind::FrontEndInitialized);
        assert(game.globals().dispatcher.current_state==
               static_cast<int>(State::ActivitySelectUpdate));
        assert(device.draw_count==1);
    }
    {
        // Missing sceneset.bik is not silently skipped in retail mode.
        fs::remove(directory/"Data/movies/sceneset.bik");
        GameRoot game;
        assert(game.select_profile(2));
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture()
        );
        auto& session=*session_storage;
        game.set_outer_state(State::SceneSetMovieTransition);
        const auto missing=session.tick({},false,false);
        assert(missing.failure);
        assert(missing.failure->stage==PhysicalPresentationStage::MovieResolve);
        assert(device.global_open==0);
    }
    // Original case 0x40: no queued movie, independent return state.
    for(int destination:{-1,0x1c}) {
        GameRoot game;
        assert(game.select_profile(2));
        game.globals().shared_movie_return_state=destination;
        game.globals().dispatcher.saved_state=0x36; // replay, NOT return
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture());
        auto& session=*session_storage;
        game.set_outer_state(State::SharedMovieTransition);
        auto frame=session.tick({},false,false);
        assert(frame.submitted && !frame.failure);
        assert(game.globals().dispatcher.current_state==
               (destination==-1?4:destination));
        assert(game.globals().shared_movie_return_state==-1);
        assert(game.globals().dispatcher.saved_state==0x36);
        assert(device.global_open==0);
    }
    // Source catalog index, physical movie EOS, one open, one transition.
    for(bool skip:{false,true}) {
        touch(directory,"Data/movies/completion_0.bik");
        touch(directory,"Data/movies/completion_1.bik");
        GameRoot game;
        assert(game.select_profile(2));
        std::string error;
        std::istringstream source(
            "Data/movies/completion_0.bik\nData/movies/completion_1.bik\n");
        assert(game.configure_completion_movies(source,error));
        game.globals().shared_completion_code=1;
        game.globals().shared_movie_return_state=0x10;
        game.set_outer_state(State::SharedMovieTransition);
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture(),skip);
        auto& session=*session_storage;
        auto first=session.tick({},false,false);
        assert(first.submitted && !first.failure);
        if(!skip) {
            assert(first.global_movie_opened && device.global_open==1);
            assert(device.last_movie.find("completion_1.bik")!=std::string::npos);
            assert(game.globals().shared_completion_code==-2);
            assert(device.draw_count==0);
            auto waiting=session.tick({},false,false);
            assert(waiting.submitted && !waiting.failure);
            assert(device.global_open==1 && device.draw_count==0);
            assert(game.globals().dispatcher.current_state==0x40);
            device.sample.global_movie_finished=true;
            auto finished=session.tick({},false,false);
            assert(finished.submitted && !finished.failure);
            assert(device.draw_count==0);
        } else {
            assert(device.global_open==0 && !first.global_movie_opened);
        }
        assert(game.globals().dispatcher.current_state==0x10);
        assert(game.globals().shared_movie_return_state==-1);
        assert(game.globals().shared_completion_code==-1);
    }
    {
        GameRoot game;
        assert(game.select_profile(2));
        game.globals().shared_completion_code=9;
        game.set_outer_state(State::SharedMovieTransition);
        Device device;
        auto session_storage=std::make_unique<NativeGameSession>(
            game,device,OriginalAssetResolver{directory},
            startup_fixture(),walkthrough_fixture());
        auto& session=*session_storage;
        const auto blocked=session.tick({},false,false);
        assert(blocked.failure);
        assert(blocked.failure->stage==PhysicalPresentationStage::GameAdvance);
        assert(device.global_open==0);
    }
    fs::remove_all(directory);
}
