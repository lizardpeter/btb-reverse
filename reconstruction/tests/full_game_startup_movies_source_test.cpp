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

    std::istringstream sequence_source(
        "Data\\movies\\THQ.bik 1\n"
        "Data\\movies\\BBC.bik 1\n"
        "Data\\movies\\hitsting.bik 1\n"
        "Data\\movies\\hotsting.bik 1\n"
        "Data\\movies\\introbig1.bik 0\n");
    OriginalVideoSequenceCatalog sequence;
    assert(sequence.read(sequence_source,error));
    assert(sequence.loaded());
    assert(sequence.entries().size()==5);
    assert(sequence.entries()[0].path=="Data\\movies\\THQ.bik");
    assert(sequence.entries()[0].input_mode==1);
    assert(sequence.entries()[1].path=="Data\\movies\\BBC.bik");
    assert(sequence.entries()[1].input_mode==1);
    assert(sequence.entries()[2].path=="Data\\movies\\hitsting.bik");
    assert(sequence.entries()[2].input_mode==1);
    assert(sequence.entries()[3].path=="Data\\movies\\hotsting.bik");
    assert(sequence.entries()[3].input_mode==1);
    assert(sequence.entries()[4].path=="Data\\movies\\introbig1.bik");
    assert(sequence.entries()[4].input_mode==0);

    std::istringstream missing_mode("Data\\movies\\introbig1.bik");
    assert(!sequence.read(missing_mode,error));
    assert(!error.empty());
    assert(sequence.entries().size()==5); // failed parse is transactional

    std::ostringstream too_many;
    for(std::size_t i=0;i<kRetailVideoSequenceCapacity+1;++i)
        too_many << "Data\\movies\\clip" << i << ".bik 0\n";
    std::istringstream oversized(too_many.str());
    assert(!sequence.read(oversized,error));
    assert(!error.empty());

    std::istringstream empty("");
    assert(!sequence.read(empty,error));
    assert(!error.empty());
}
