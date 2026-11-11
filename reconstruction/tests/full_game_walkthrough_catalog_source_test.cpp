#include "btb/full_game_walkthrough_catalog.hpp"

#include <cassert>
#include <sstream>
#include <string>

int main() {
    using namespace btb::full_game;
    // Each constant comes from the corresponding original PE32
    // pregame dispatcher write to DAT_00446FD8 (0x42AAED..0x42C84E).
    constexpr std::array<int,10> original_voices{{
        0x1f5,0x1f0,0x1fa,0x1f9,0x1e8,
        0x1e9,0x1f3,0x1f7,0x1f4,0x1f1
    }};
    for(int i=0;i<10;++i) {
        assert(retail_walkthrough_hover_voice(
            i,63,157,false)==original_voices[i]);
        assert(!retail_walkthrough_hover_voice(
            i,63,157,true));
        assert(!retail_walkthrough_hover_voice(
            i,49,157,false));
        assert(!retail_walkthrough_hover_voice(
            i,365,157,false));
        assert(!retail_walkthrough_hover_voice(
            i,63,140,false));
        assert(!retail_walkthrough_hover_voice(
            i,63,350,false));
        assert(retail_walkthrough_hover_voice(
            i,50,141,false)==original_voices[i]);
        assert(retail_walkthrough_hover_voice(
            i,364,349,false)==original_voices[i]);
    }
    assert(!retail_walkthrough_hover_voice(-1,63,157,false));
    assert(!retail_walkthrough_hover_voice(10,63,157,false));
    const std::string retail = R"(data//movies//herdwalthrough.bik data//sound//herdwalthroughhelp.wav
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
)";
    OriginalWalkthroughCatalog catalog;
    std::string error;
    std::istringstream installed(retail);
    assert(catalog.read(installed,error));
    assert(catalog.loaded());
    assert(error.empty());
    assert(catalog.entry(0)->movie ==
           "data//movies//herdwalthrough.bik");
    assert(catalog.entry(3)->spoken_help ==
           "data//sound//skate2walthroughhelp.wav");
    assert(catalog.entry(5)->movie ==
           "data//movies//advent2walthrough.bik");
    assert(catalog.entry(9)->movie ==
           "data//movies//designwalthrough.bik");
    assert(catalog.extra_help(0) &&
           *catalog.extra_help(0) ==
           "data//sound//dinowalthroughsubhelp.wav");
    assert(*catalog.extra_help(2) ==
           "data//sound//advent1walthroughsubhelp.wav");
    assert(!catalog.entry(10));
    assert(!catalog.entry(-1));
    assert(!catalog.extra_help(3));

    std::istringstream too_short(
        "data//movies//herdwalthrough.bik data//sound//herdwalthroughhelp.wav");
    assert(!catalog.read(too_short,error));
    assert(!error.empty());
    assert(catalog.entry(9)->movie ==
           "data//movies//designwalthrough.bik");

    auto damaged = retail;
    const auto offset=damaged.find("NULL data//sound//skate1");
    assert(offset != std::string::npos);
    damaged.replace(offset,4,"WRONG");
    std::istringstream wrong_sentinel(damaged);
    assert(!catalog.read(wrong_sentinel,error));
    assert(catalog.loaded());
    assert(catalog.entry(9)->movie ==
           "data//movies//designwalthrough.bik");

    std::istringstream trailing(retail+"EXTRA_BINK\n");
    assert(!catalog.read(trailing,error));
    assert(catalog.entry(0)->movie ==
           "data//movies//herdwalthrough.bik");
}
