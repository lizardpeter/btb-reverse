#include "btb/full_game_runtime.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

int main() {
    namespace fs = std::filesystem;
    namespace fg = btb::full_game;
    namespace progress = btb::progress;
    const auto path = fs::temp_directory_path() /
                      "btb_full_game_native_profile_fixture";
    fs::create_directories(path);

    fg::GameRoot first;
    auto& root = first.globals();
    root.profile_metadata[3].badge_index = 2;
    root.profile_metadata[3].name_length = 3;
    root.profile_metadata[3].name_glyph_codes[0] = 5;
    root.profile_metadata[3].name_glyph_codes[1] = 7;
    root.profile_metadata[3].name_glyph_codes[2] = 9;
    root.player_progress[3].set(progress::Slot::BobsBandBob, 1);
    root.player_progress[3].set(progress::Slot::BobsBandWendy, 1);
    root.player_progress[3].values[17] = 53; // nonvisible native progress
    std::string error;
    assert(first.save_profile_files(path, error));

    assert(fs::exists(path / "playerinfo.txt"));
    for (int i=1; i<=5; ++i) {
        assert(fs::exists(path / ("player"+std::to_string(i)+".txt")));
    }

    fg::GameRoot second;
    assert(second.load_profile_files(path,error));
    assert(second.globals().profile_metadata[3].name_length == 3);
    assert(second.globals().profile_metadata[3].badge_index == 2);
    assert(second.globals().profile_metadata[3].name_glyph_codes[2] == 9);
    assert(second.globals().player_progress[3].get(
        progress::Slot::BobsBandBob) == 1);
    assert(second.globals().player_progress[3].values[17] == 53);
    assert(second.select_profile(3));

    // Native profile deletion clears ONLY visible progress slots 50..64
    // and resets occupied header metadata without overwriting old glyphs.
    assert(second.delete_profile(3));
    assert(!second.globals().active_profile);
    assert(second.globals().profile_metadata[3].badge_index == -1);
    assert(second.globals().profile_metadata[3].name_length == 0);
    assert(second.globals().profile_metadata[3].name_glyph_codes[2] == 9);
    assert(second.globals().player_progress[3].get(
        progress::Slot::BobsBandBob) == 0);
    assert(second.globals().player_progress[3].values[17] == 53);
    assert(second.save_profile_files(path,error));

    fg::GameRoot third;
    assert(third.load_profile_files(path,error));
    assert(third.globals().player_progress[3].get(
        progress::Slot::BobsBandBob) == 0);
    assert(third.globals().player_progress[3].values[17] == 53);

    // Corrupt progress files must not partially replace a loaded session.
    {
        std::ofstream out(path / "player2.txt",std::ios::trunc);
        out << "1 2 3";
    }
    assert(!third.load_profile_files(path,error));
    assert(!error.empty());
    assert(third.globals().player_progress[3].values[17] == 53);

    fs::remove_all(path);
}
