#include "btb/full_game_startup_movies.hpp"

#include <cassert>
#include <sstream>
#include <string>

int main() {
    using namespace btb::full_game;
    using btb::game_flow::State;

    std::istringstream retail(R"(Data\movies\herdstartup.bik
Data\movies\dinostartup.bik
Data\movies\skatestartup.bik
Data\movies\adventstartup.bik
Data\movies\fireworkstartup.bik
Data\movies\squirelstartup.bik
Data\movies\musicstartup.bik
Data\movies\designstartup.bik
)");
    OriginalStartupMovieCatalog movies;
    std::string error;
    assert(movies.read(retail,error));
    assert(movies.loaded());
    assert(error.empty());

    assert(retail_pregame_global_intro_index(
        State::HerdingPregameSetup) == 0);
    assert(retail_pregame_global_intro_index(
        State::FireworksPregameSetup) == 4);
    assert(retail_pregame_global_intro_index(
        State::SquirrelPregameSetup) == 5);
    assert(retail_pregame_global_intro_index(
        State::ParkDesignerPregameSetup) == 7);
    assert(!retail_pregame_global_intro_index(
        State::GolfPregameSetup));
    assert(!retail_pregame_global_intro_index(
        State::BobsBandPregameSetup));

    assert(*movies.movie(0) ==
           "Data\\movies\\herdstartup.bik");
    assert(*movies.movie(4) ==
           "Data\\movies\\fireworkstartup.bik");
    assert(*movies.movie(5) ==
           "Data\\movies\\squirelstartup.bik");
    assert(*movies.movie(7) ==
           "Data\\movies\\designstartup.bik");
    assert(!movies.movie(8));
    assert(!movies.movie(-1));

    std::istringstream malformed("Data\\movies\\herdstartup.bik");
    assert(!movies.read(malformed,error));
    assert(!error.empty());
    assert(*movies.movie(7) ==
           "Data\\movies\\designstartup.bik");
}
