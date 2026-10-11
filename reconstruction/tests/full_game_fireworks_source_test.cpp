#include "btb/full_game_fireworks.hpp"

#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace fs = std::filesystem;
using namespace btb::full_game;
using btb::fireworks::FireworkType;

namespace {

void write_original_rectangles(const fs::path& file,
                                int count, bool placement) {
    std::ofstream out(file);
    assert(out);
    for (int i = 0; i < count; ++i) {
        const int x = placement ? 20 + (i % 6) * 80 : 40 + i * 45;
        const int y = placement ? 100 + (i / 6) * 90 : 20;
        const int right = x + (placement ? 48 : 30);
        const int bottom = y + 30;
        // Original text has four (x,y) corners. Parser retains only
        // vertices 0 and 2; other coordinates are deliberately different.
        out << x << ' ' << y << ' '
            << right << ' ' << y << ' '
            << right << ' ' << bottom << ' '
            << x << ' ' << bottom << '\n';
    }
}

btb::full_game::ActivityFrameInput click(int x,int y,int random=0) {
    btb::full_game::ActivityFrameInput input;
    input.pointer_x=x;
    input.pointer_y=y;
    input.click_pulse=true;
    input.random_value=random;
    return input;
}

} // namespace

int main() {
    const auto dir = fs::temp_directory_path() /
                     "btb_full_game_fireworks_driver_fixture";
    fs::remove_all(dir);
    const auto source = dir / "Data" / "SubGameFirework";
    const auto saves = dir / "profiles";
    fs::create_directories(source / "graph");
    write_original_rectangles(source/"graph"/"fireworks.txt",18,true);
    write_original_rectangles(source/"graph"/"fireworkboxes.txt",12,false);

    GameRoot root;
    assert(root.select_profile(0));
    auto fireworks = std::make_unique<FireworksDriver>(source,saves);
    auto* live = fireworks.get();
    root.install(ActivityId::Fireworks,std::move(fireworks));
    root.set_outer_state(btb::game_flow::State::FireworksInit);

    assert(root.advance({}).kind == FrameKind::ActivityInitialized);
    assert(live->initialized());
    assert(live->sequence().occupied_count() == 0);
    auto editor = root.advance({});
    assert(editor.kind == FrameKind::ActivityUpdated);
    assert(!editor.effects.draws.empty());
    assert(editor.effects.draws[0].source_asset ==
           "Data\\SubGameFirework\\graph\\Bk_01e.bmp");
    assert(!editor.effects.draws[0].color_keyed);

    // Type 4 palette, then the authored center row (row 1/column 2).
    auto selected = root.advance(click(230,30,2));
    assert(selected.kind == FrameKind::ActivityUpdated);
    assert(live->editor().selected_type == FireworkType::LargeRed);
    assert(!selected.effects.audio.empty());
    const auto place = root.advance(click(210,185,1));
    assert(place.kind == FrameKind::ActivityUpdated);
    // Source actor state 10 -> 11 -> commit. The first frame queues,
    // and the following draw performs the actual single-cell write.
    assert(live->sequence().occupied_count() == 0);
    const auto committed = root.advance({});
    assert(committed.kind == FrameKind::ActivityUpdated);
    assert(live->sequence().at(1,2) == FireworkType::LargeRed);
    bool found_bitmap=false;
    for (const auto& draw : committed.effects.draws) {
        if (draw.source_asset ==
            "Data\\SubGameFirework\\graph\\red_large_01.bmp") {
            found_bitmap=true;
            assert(draw.color_keyed);
        }
    }
    assert(found_bitmap);

    // Retail delete-selected control enters state 13; its grid click
    // erases a cell without scheduling an actor placement.
    auto delete_mode = root.advance(click(500,440));
    assert(delete_mode.kind == FrameKind::ActivityUpdated);
    assert(live->editor().internal_state ==
           btb::fireworks::InternalState::DeleteSelected);
    root.advance(click(210,185));
    assert(live->sequence().occupied_count() == 0);
    root.advance(click(500,440));
    assert(live->editor().internal_state ==
           btb::fireworks::InternalState::Editor);

    // Delete All is a shared Yes/No context 3, not an unconditional erase.
    root.advance(click(230,30));
    root.advance(click(210,185));
    root.advance({});
    assert(live->sequence().occupied_count() == 1);
    auto delete_all = root.advance(click(130,440));
    assert(delete_all.effects.open_yes_no_confirmation);
    assert(delete_all.effects.yes_no_context == 3);
    auto deny = ActivityFrameInput{};
    deny.confirmation_resolved = true;
    root.advance(deny);
    assert(live->sequence().occupied_count() == 1);
    delete_all = root.advance(click(130,440));
    assert(delete_all.effects.open_yes_no_confirmation);
    auto confirm = ActivityFrameInput{};
    confirm.confirmation_resolved = true;
    confirm.confirmation_yes = true;
    root.advance(confirm);
    assert(live->sequence().occupied_count() == 0);

    // Save and reload the actual 18 signed little-endian 32-bit cells.
    root.advance(click(230,30));
    root.advance(click(50,90)); // row 0 / column 0, after x+10,y-20 shift
    root.advance({});
    assert(live->sequence().at(0,0) == FireworkType::LargeRed);
    std::string error;
    assert(root.unload_current(error));
    assert(!live->initialized());
    const auto file = saves / "firedata1.txt";
    assert(fs::file_size(file) == 72);
    {
        std::ifstream saved(file,std::ios::binary);
        std::array<unsigned char,4> first{};
        saved.read(reinterpret_cast<char*>(first.data()),first.size());
        assert(first[0] == 4);
        assert(first[1] == 0 && first[2] == 0 && first[3] == 0);
    }

    root.set_outer_state(btb::game_flow::State::FireworksInit);
    assert(root.advance({}).kind == FrameKind::ActivityInitialized);
    assert(live->sequence().at(0,0) == FireworkType::LargeRed);

    // Source Play sets internal state 14. Without the authenticated Bink
    // provider the NEXT frame must fail at the GameRoot boundary, not
    // return a successful activity frame or silently skip to results.
    const auto play = root.advance(click(320,440));
    assert(play.kind == FrameKind::ActivityUpdated);
    assert(live->editor().internal_state ==
           btb::fireworks::InternalState::PreShowMovieSetup);
    const auto unsupported = root.advance({});
    assert(unsupported.kind == FrameKind::ActivityFailed);
    assert(unsupported.error.find("Bink") != std::string::npos);
    assert(!unsupported.effects.save_and_unload);
    assert(root.unload_current(error));

    // Corrupt saves fail initialization rather than being repaired or
    // converted into a synthetic gameplay layout.
    {
        std::fstream bad(file,std::ios::binary|std::ios::in|std::ios::out);
        const unsigned char invalid=99;
        bad.write(reinterpret_cast<const char*>(&invalid),1);
    }
    root.set_outer_state(btb::game_flow::State::FireworksInit);
    const auto corrupt = root.advance({});
    assert(corrupt.kind == FrameKind::ActivityFailed);
    assert(!live->initialized());
    assert(!corrupt.error.empty());

    fs::remove_all(dir);
}
