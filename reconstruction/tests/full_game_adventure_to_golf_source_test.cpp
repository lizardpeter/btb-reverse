#include "btb/full_game_generic_ui.hpp"
#include "btb/full_game_golf.hpp"
#include "btb/full_game_pregame_resources.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace {
using namespace btb;
using namespace btb::full_game;
using btb::game_flow::State;

GenericUiCatalog installed_shaped_ui() {
    GenericUiCatalog data;
    for (std::size_t group=0; group<front_end::kScreenCount; ++group) {
        const int count=front_end::kRetailHotAreaCounts[group];
        data.counts.push_back(count);
        front_end::HotAreaScreen screen;
        front_end::ReplacementScreen replacements;
        for (int i=0; i<count; ++i) {
            const int x=10+i*70;
            screen.areas.push_back({
                {{{x,50},{x+40,50},{x+40,90},{x,90}}},{}
            });
            front_end::ReplacementRecord record{};
            record.target_state_or_action=-6;
            record.click_sound_id=-1;
            record.hover_sound_ids={-1,-1,-1};
            replacements.records.push_back(std::move(record));
        }
        if (group==6) {
            // Original Advent_playground: Back, Help, Maze, Golf.
            for (int i=0;i<count;++i) {
                replacements.records[i].target_state_or_action =
                    std::array<int,4>{-1,-6,-2,-3}[i];
            }
        }
        if (group==2) {
            // Original Information_screen_pre_game:
            // Back, Easy, Medium, Hard, Start, Help.
            for (int i=0;i<count;++i) {
                replacements.records[i].target_state_or_action =
                    std::array<int,6>{-1,-2,-3,-4,-5,-6}[i];
            }
        }
        data.hot_areas.push_back(std::move(screen));
        data.replacements.push_back(std::move(replacements));
    }
    return data;
}

GameFrame select_area(GameRoot& root,int index) {
    ActivityFrameInput click{};
    click.pointer_x=20+70*index;
    click.pointer_y=65;
    click.click_pulse=true;
    const auto pressed=root.advance(click);
    assert(pressed.kind==FrameKind::FrontEndUpdated);
    click.click_pulse=false;
    return root.advance(click);
}
}

int main() {
    using namespace btb::full_game;
    using btb::game_flow::State;
    namespace fs=std::filesystem;

    OriginalUiBitmapCatalog bitmaps;
    std::string error;
    std::istringstream file(R"(Data\ui\loading\loading1.bmp
Data\ui\main\main.bmp
Data\ui\actanims\newactselect.bmp
Data\ui\loading\loadingsub1.bmp
Data\ui\instruction\petscorner.bmp
Data\ui\instruction\vel.bmp
Data\ui\instruction\tri.bmp
Data\ui\instruction\trex.bmp
Data\ui\subact\ddsubsel.bmp
Data\ui\subact\skate.bmp
Data\ui\instruction\repairskate.bmp
Data\ui\instruction\rideskate.bmp
Data\ui\subact\golf.bmp
Data\ui\instruction\golfcollect.bmp
Data\ui\instruction\playgolf.bmp
Data\ui\instruction\go.bmp
Data\ui\Instruction\sqrinstbg.bmp
Data\ui\instruction\mp.bmp
Data\ui\instruction\instdyp.bmp
Data\ui\main\playagain.bmp
Data\ui\main\playagainnodiff.bmp
Data\ui\main\playagaingrandopen.bmp
Data\ui\subact\music.bmp
END.bmp
)");
    assert(bitmaps.read(file,error));

    GameRoot root;
    assert(root.select_profile(0));
    auto catalog=installed_shaped_ui();
    assert(catalog.validate(error));

    auto adventure=std::make_unique<GenericUiScreenDriver>(
        catalog,front_end::Screen::AdventurePlaygroundChooser,
        std::vector<int>(4,0));
    assert(root.install_generic_front_end_pair(
        State::AdventureChooserSetup,std::move(adventure)));

    // Bind the pregame screen while its source-selected subgame is still 0.
    // After Golf is picked it must dynamically switch to backdrop slot 14.
    auto pregame=make_original_pregame_screen_driver(
        catalog,bitmaps,State::GolfPregameSetup,0,
        std::vector<int>(6,0));
    assert(pregame);
    assert(root.install_pregame_front_end_pair(
        State::GolfPregameSetup,std::move(pregame)));

    const auto temp=fs::temp_directory_path()/
        "btb_full_game_golf_chooser_source";
    fs::remove_all(temp);
    fs::create_directories(temp);
    {
        std::ofstream original(temp/"golfdata.txt");
        original << R"(98 195
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
    auto golf=std::make_unique<GolfDriver>(temp,1);
    GolfDriver* golf_handle=golf.get();
    root.install(ActivityId::Golf,std::move(golf));

    root.set_outer_state(State::AdventureChooserSetup);
    assert(root.advance({}).kind==FrameKind::FrontEndInitialized);
    const auto choice=select_area(root,3); // retail -3 -> Golf
    assert(choice.menu_action);
    assert(choice.menu_action->selected_subgame==1);
    assert(root.globals().menu.selected_subgame==1);
    assert(root.globals().dispatcher.current_state==0x34);

    const auto setup=root.advance({});
    assert(setup.kind==FrameKind::FrontEndInitialized);
    assert(setup.original_pregame_backdrop_slot==14);
    assert(setup.original_walkthrough_index==5);
    assert(!setup.original_global_intro_movie_index);
    assert(root.globals().dispatcher.current_state==0x35);

    const auto first_draw=root.advance({});
    assert(first_draw.kind==FrameKind::FrontEndUpdated);
    assert(first_draw.effects.draws.size()==1);
    assert(first_draw.effects.draws[0].source_asset==
           "Data\\ui\\instruction\\playgolf.bmp");

    const auto easy=select_area(root,1);
    assert(easy.pregame_action);
    assert(easy.pregame_action->difficulty==0);
    assert(root.globals().dispatcher.current_state==0x35);
    assert(root.globals().menu.variant_selection_origin==
           State::GolfPregameUpdate);

    const auto start=select_area(root,4);
    assert(start.pregame_action);
    assert(start.pregame_action->start_activity);
    assert(root.globals().dispatcher.current_state==0x36);

    const auto initialized=root.advance({});
    assert(initialized.kind==FrameKind::ActivityInitialized);
    assert(golf_handle->round());
    assert(golf_handle->round()->difficulty()==0); // pregame overrides ctor 1
    assert(golf_handle->round()->attempts_remaining()==5);
    assert(root.globals().dispatcher.current_state==0x37);

    const auto running=root.advance({});
    assert(running.kind==FrameKind::ActivityUpdated);
    assert(!running.effects.draws.empty());
    assert(running.effects.draws[0].source_asset==
           "Data\\SubGameGolf\\golfbg.bmp");

    assert(root.unload_current(error));
    fs::remove_all(temp);
}
