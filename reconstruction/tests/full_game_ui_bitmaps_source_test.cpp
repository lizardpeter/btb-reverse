#include "btb/full_game_ui_bitmaps.hpp"

#include <cassert>
#include <sstream>
#include <string>

int main() {
    using namespace btb::full_game;

    std::istringstream original(R"(Data\ui\loading\loading1.bmp
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

    OriginalUiBitmapCatalog table;
    std::string error;
    assert(table.read(original,error));
    assert(error.empty());
    assert(table.loaded());
    assert(table.filename(22) != nullptr);
    assert(*table.filename(22) ==
           "Data\\ui\\subact\\music.bmp");
    assert(*table.filename(2) ==
           "Data\\ui\\actanims\\newactselect.bmp");
    assert(!table.filename(23));

    const auto draw=table.backdrop(22);
    assert(draw);
    assert(draw->source_asset ==
           "Data\\ui\\subact\\music.bmp");
    assert(draw->x == 0 && draw->y == 0);
    assert(!draw->color_keyed);

    std::istringstream truncated("Data\\ui\\loading\\loading1.bmp\nEND.bmp\n");
    assert(!table.read(truncated,error));
    assert(!error.empty());
    // A damaged late resource-table load cannot replace good source data.
    assert(*table.filename(22) ==
           "Data\\ui\\subact\\music.bmp");
}
