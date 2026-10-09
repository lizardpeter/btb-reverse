#include "btb/full_game_pregame_resources.hpp"

#include <cassert>
#include <sstream>
#include <string>

int main() {
    using namespace btb::full_game;
    using btb::game_flow::State;

    OriginalUiBitmapCatalog images;
    std::istringstream ui(R"(Data\ui\loading\loading1.bmp
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

    OriginalWalkthroughCatalog walkthroughs;
    std::istringstream videos(R"(data//movies//herdwalthrough.bik data//sound//herdwalthroughhelp.wav
data//movies//dinowalthrough.bik data//sound//dinowalthroughhelp.wav
data//movies//skate1walthrough.bik data//sound//skate1walthroughhelp.wav
data//movies//skate2walthrough.bik data//sound//skate2walthroughhelp.wav
data//movies//advent1walthrough.bik data//sound//advent1walthroughhelp.wav
data//movies//advent2walthrough.bik data//sound//advent2walthroughhelp.wav
data//movies//fireworkwalthrough.bik data//sound//fireworkwalthroughhelp.wav
data//movies//squirelwalthrough.bik data//sound//squirelwalthroughhelp.wav
data//movies//musicwalthrough.bik data//sound//musicwalthroughhelp.wav
data//movies//designwalthrough.bik data//sound//designwalthroughhelp.wav
NULL data//sound//dinowalthroughsubhelp.wav
NULL data//sound//skate1walthroughsubhelp.wav
NULL data//sound//advent1walthroughsubhelp.wav
)");

    std::string error;
    assert(images.read(ui,error));
    assert(walkthroughs.read(videos,error));

    const auto golf = resolve_original_pregame_resources(
        State::GolfPregameSetup,1,images,walkthroughs);
    assert(golf);
    assert(golf->backdrop_slot == 14);
    assert(golf->backdrop.source_asset ==
           "Data\\ui\\instruction\\playgolf.bmp");
    assert(golf->walkthrough_index == 5);
    assert(golf->walkthrough_movie ==
           "data//movies//advent2walthrough.bik");
    assert(golf->spoken_help_wav ==
           "data//sound//advent2walthroughhelp.wav");
    assert(!golf->global_intro_mode14);

    const auto band = resolve_original_pregame_resources(
        State::BobsBandPregameSetup,2,images,walkthroughs);
    assert(band);
    assert(band->backdrop.source_asset ==
           "Data\\ui\\instruction\\mp.bmp");
    assert(band->walkthrough_index == 8);

    const auto fireworks = resolve_original_pregame_resources(
        State::FireworksPregameSetup,0,images,walkthroughs);
    assert(fireworks);
    assert(fireworks->walkthrough_index == 6);
    assert(fireworks->backdrop_slot == 15);
    assert(fireworks->global_intro_mode14);

    const auto skate = resolve_original_pregame_resources(
        State::SpudSkatePregameSetup,1,images,walkthroughs);
    assert(skate);
    assert(skate->walkthrough_index == 3);
    assert(skate->backdrop_slot == 11);
    assert(skate->backdrop.source_asset ==
           "Data\\ui\\instruction\\rideskate.bmp");

    assert(!resolve_original_pregame_resources(
        State::GolfPregameSetup,2,images,walkthroughs));
    assert(!resolve_original_pregame_resources(
        State::GolfPregameUpdate,1,images,walkthroughs));
    assert(!resolve_original_pregame_resources(
        State::MusicChooserSetup,0,images,walkthroughs));
}
