#include "btb/fireworks_movie_bank_manifest.hpp"
#include "btb/fireworks_sequence.hpp"

#include <cassert>
#include <string>

int main() {
    using namespace btb::fireworks;
    static_assert(kOriginalFireworkMovieCount==23);
    static_assert(kOriginalTopMiddleMovie==20);
    static_assert(kOriginalCrowdLoopMovie==21);
    static_assert(kOriginalCrowdEndMovie==22);
    static_assert(kOriginalFireworkBinkNames[0]=="airbombredleft.bik");
    static_assert(kOriginalFireworkBinkNames[5]=="Mediumgreenleft.bik");
    static_assert(kOriginalFireworkBinkNames[12]=="airbombredright.bik");
    static_assert(kOriginalFireworkBinkNames[19]=="airbombblueright.bik");
    static_assert(kOriginalFireworkBinkNames[20]=="topmiddle.bik");
    static_assert(kOriginalFireworkBinkNames[21]=="fireworkcrowdloop.bik");
    static_assert(kOriginalFireworkBinkNames[22]=="fireworkcrowdend.bik");
    const auto paths=original_firework_movie_paths(
        std::filesystem::path("Data")/"SubGameFirework");
    assert(paths.size()==23);
    assert(paths[0]==std::filesystem::path("Data")/
        "SubGameFirework"/"graph"/"airbombredleft.bik");
    assert(paths[8]==std::filesystem::path("Data")/
        "SubGameFirework"/"graph"/"Candlered.bik");
    assert(paths[22]==std::filesystem::path("Data")/
        "SubGameFirework"/"graph"/"fireworkcrowdend.bik");
    for(std::size_t i=0;i<paths.size();++i) {
        assert(paths[i].filename()==kOriginalFireworkBinkNames[i]);
        assert(paths[i].extension()==".bik");
        for(std::size_t j=0;j<i;++j)
            assert(paths[i]!=paths[j]);
    }
    // The authored show applies type+12 in middle row, unlike editor
    // preview. Verify the original source table includes those slots.
    assert(kOriginalFireworkBinkNames[12]=="airbombredright.bik");
    assert(kOriginalFireworkBinkNames[20]=="topmiddle.bik");
}
