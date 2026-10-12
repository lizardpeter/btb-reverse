#include "btb/recompiled_core_smoke.hpp"
#include "btb/full_game_bobs_band.hpp"
#include "btb/full_game_fireworks.hpp"
#include "btb/full_game_golf.hpp"
#include "btb/full_game_park_designer.hpp"
#include "btb/full_game_runtime.hpp"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <system_error>

namespace btb::full_game {
namespace {
namespace fs=std::filesystem;
struct FixtureDirectory {
    fs::path root;
    FixtureDirectory():root(fs::temp_directory_path() /
        ("btb_linked_game_"+std::to_string(
          std::chrono::steady_clock::now().time_since_epoch().count()))) {
        fs::create_directories(root);
    }
    ~FixtureDirectory() {
        std::error_code ignored;fs::remove_all(root,ignored);
    }
    FixtureDirectory(const FixtureDirectory&)=delete;
    FixtureDirectory& operator=(const FixtureDirectory&)=delete;
};
bool make_golf_data(const fs::path& p){
    std::ofstream f(p/"golfdata.txt");
    if(!f)return false;
    f << R"(98 195
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
    return static_cast<bool>(f);
}
bool rectangles(const fs::path& p,int total,bool grid){
    std::ofstream f(p);if(!f)return false;
    for(int i=0;i<total;++i){
        const int x=grid?20+(i%6)*80:40+i*45;
        const int y=grid?100+(i/6)*90:20;
        const int rx=x+(grid?48:30),by=y+30;
        f<<x<<' '<<y<<' '<<rx<<' '<<y<<' '<<rx<<' '<<by<<' '<<x<<' '<<by<<'\n';
    }
    return static_cast<bool>(f);
}
bool make_band_data(const fs::path& p){
    std::ofstream f(p/"machinedata.txt");
    if(!f)return false;
    f << R"(0 138
0 140
98 117
100 120
219 1
228 25
349 113
352 123
410 70
406 81
168 157
216 166
174 128
194 144
142 216
169 191
73 95
63 78
105 136
121 139
525 141
519 145
524 142
56 71
56 68
52 72
59 59 64
259 264 319 290
206 246 258 289
323 248 378 276
264 224 318 262
353 219 397 247
320 196 351 245
413 193 445 232
354 183 409 217
493 200 525 248
448 193 491 252
static machine animation rectangles
)";
    return static_cast<bool>(f);
}
bool make_park_bounds(const fs::path& p){
    std::ofstream f(p/"boundareas.txt");
    if(!f)return false;
    for(int i=0;i<60;++i)f<<"0 0 40 0 40 40 0 40 -1 -1\n";
    return static_cast<bool>(f);
}
ActivityFrameInput click(int x,int y){
    ActivityFrameInput out{};
    out.pointer_x=x;out.pointer_y=y;out.click_pulse=true;
    return out;
}
}

bool verify_integrated_activity_fixtures() {
    // Fixtures emulate original text-file grammar ONLY; they do not verify
    // rendering, retail assets, voice playback, Win32 COM or gameplay parity.
    FixtureDirectory directory;
    const auto golf=directory.root/"Data"/"SubGameGolf";
    const auto fireworks=directory.root/"Data"/"SubGameFirework";
    const auto band=directory.root/"Data"/"SubGameOpen";
    const auto park=directory.root/"Data"/"SubGameDYP";
    const auto saves=directory.root/"profiles";
    fs::create_directories(golf);
    fs::create_directories(fireworks/"graph");
    fs::create_directories(band);
    fs::create_directories(park);
    fs::create_directories(saves);
    if(!make_golf_data(golf)||
       !rectangles(fireworks/"graph"/"fireworks.txt",18,true)||
       !rectangles(fireworks/"graph"/"fireworkboxes.txt",12,false)||
       !make_band_data(band)||!make_park_bounds(park))return false;

    GameRoot game;
    if(!game.select_profile(0))return false;
    auto golf_driver=std::make_unique<GolfDriver>(golf,1);
    auto* golf_live=golf_driver.get();
    auto fireworks_driver=std::make_unique<FireworksDriver>(fireworks,saves);
    auto* fireworks_live=fireworks_driver.get();
    auto band_driver=std::make_unique<BobsBandDriver>(
        band,saves,btb::bobs_band::Conductor::Bob);
    auto* band_live=band_driver.get();
    // No fabricated Park Designer sprite/editor/audio provider.
    auto park_driver=std::make_unique<ParkDesignerDriver>(park,saves);
    auto* park_live=park_driver.get();
    game.install(ActivityId::Golf,std::move(golf_driver));
    game.install(ActivityId::Fireworks,std::move(fireworks_driver));
    game.install(ActivityId::BobsBand,std::move(band_driver));
    game.install(ActivityId::ParkDesigner,std::move(park_driver));
    std::string error;

    game.set_outer_state(game_flow::State::GolfInit);
    if(game.advance({}).kind!=FrameKind::ActivityInitialized ||
       !golf_live->round()||golf_live->round()->attempts_remaining()!=5)
        return false;
    if(game.advance({}).kind!=FrameKind::ActivityUpdated)return false;
    auto golf_click=game.advance({.click_pulse=true});
    if(golf_click.kind!=FrameKind::ActivityUpdated ||
       golf_live->round()->state()!=btb::golf::RoundState::PowerMeter)return false;
    if(!game.unload_current(error))return false;

    game.set_outer_state(game_flow::State::FireworksInit);
    if(game.advance({}).kind!=FrameKind::ActivityInitialized ||
       !fireworks_live->initialized())return false;
    if(game.advance({}).kind!=FrameKind::ActivityUpdated)return false;
    if(game.advance(click(230,30)).kind!=FrameKind::ActivityUpdated)return false;
    if(game.advance(click(210,185)).kind!=FrameKind::ActivityUpdated)return false;
    if(game.advance({}).kind!=FrameKind::ActivityUpdated ||
       fireworks_live->sequence().occupied_count()!=1)return false;
    if(!game.unload_current(error)||fs::file_size(saves/"firedata1.txt")!=72)
        return false;
    game.set_outer_state(game_flow::State::FireworksInit);
    if(game.advance({}).kind!=FrameKind::ActivityInitialized ||
       fireworks_live->sequence().occupied_count()!=1)return false;
    if(!game.unload_current(error))return false;

    game.set_outer_state(game_flow::State::BobsBandInit);
    if(game.advance({}).kind!=FrameKind::ActivityInitialized||
       !band_live->activity())return false;
    if(game.advance(click(245,295)).kind!=FrameKind::ActivityUpdated)return false;
    if(game.advance(click(45,311)).kind!=FrameKind::ActivityUpdated ||
       band_live->activity()->editor().composition().at({0,0})!=0)
        return false;
    if(!game.unload_current(error)||fs::file_size(saves/"musicbob1.txt")!=480)
        return false;

    game.set_outer_state(game_flow::State::ParkDesignerInit);
    if(game.advance({}).kind!=FrameKind::ActivityInitialized ||
       !park_live->initialized())return false;
    const auto no_renderer=game.advance({});
    // This is a deliberate block: central 0x0040E440 and retail sprite
    // host are not complete. Do NOT convert it into a fake successful frame.
    if(no_renderer.kind!=FrameKind::ActivityFailed ||
       no_renderer.error.empty())return false;
    return game.unload_current(error);
}
} // namespace btb::full_game
