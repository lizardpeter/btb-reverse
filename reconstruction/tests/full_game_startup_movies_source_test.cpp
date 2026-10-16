#include "btb/full_game_startup_movies.hpp"
#include "btb/original_asset_resolver.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <fstream>

int main(int argc,char** argv) {
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

    OriginalBootMovieCatalog boot;
    std::istringstream boot_text(R"(Data\movies\THQ.bik 1
Data\movies\BBC.bik 1
Data\movies\hitsting.bik 1
Data\movies\hotsting.bik 1
Data\movies\introbig1.bik 0)");
    assert(boot.read(boot_text,error));
    assert(boot.entries().size()==5);
    assert(!boot.entries()[0].skippable());
    assert(boot.entries()[4].skippable());
    for(const std::string text : {"", "NULL 1", "movie.bik", "movie.bik x"}) {
        std::istringstream bad(text);
        assert(!boot.read(bad,error));
        assert(boot.entries().size()==5); // failed read is transactional
    }
    std::ostringstream too_many;
    for(int i=0;i<11;++i)too_many<<"movie.bik 1\n";
    std::istringstream overflow(too_many.str());
    assert(!boot.read(overflow,error));
    if(argc==2) {
        const std::filesystem::path root=argv[1];
        std::ifstream sequence(root/"loaddata"/"videoseq.txt");
        assert(sequence && boot.read(sequence,error));
        assert(boot.entries().size()==5);
        OriginalAssetResolver assets{root};
        for(std::size_t i=0;i<boot.entries().size();++i) {
            const auto& entry=boot.entries()[i];
            assert(entry.skippable()==(i==4));
            assert(assets.resolve(entry.file).found());
        }
    }
}
