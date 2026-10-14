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

    // Source initializer distinguishes absent progress files from zeros.
    const auto absent = path / "absent";
    fs::remove_all(absent);
    fg::GameRoot fresh;
    std::string absent_error;
    assert(fresh.load_profile_files(absent, absent_error));
    for (const auto& record : fresh.globals().player_progress) {
        assert(record.values[0] == -1);
        for (std::size_t i = 1; i < record.values.size(); ++i)
            assert(record.values[i] == 0);
    }

    // The real state-1 coordinator owns entry setup, drawing-before-input,
    // managed audio gating and the transition to the original scene movie.
    // These are source fixtures, not an assertion about retail bitmap sizes.
    std::array<fg::Rect,95> font;
    font.fill({0,0,8,10});
    std::array<fg::Rect,6> badges;
    for(int i=0;i<6;++i)badges[i]={i*50,0,i*50+50,45};
    assert(fresh.begin_profile_name_entry(2,font,badges));
    auto frame=fresh.advance({});
    assert(frame.kind==fg::FrameKind::FrontEndInitialized && frame.effects.draws.empty());
    fg::ActivityFrameInput typed;
    typed.keyboard_state[0x1e]=0x80;typed.translated_character='A';
    frame=fresh.advance(typed);
    assert(frame.kind==fg::FrameKind::FrontEndUpdated);
    for(const auto& draw:frame.effects.draws)
        assert(draw.source_asset!="Data/ui/blue_name_boxed_font.bmp");
    frame=fresh.advance({});
    bool has_glyph=false;
    for(const auto& draw:frame.effects.draws) {
        if(draw.source_asset=="Data/ui/blue_name_boxed_font.bmp") {
            has_glyph=true;
            assert(draw.x==318 && draw.y==200 && draw.source_rectangle->bottom==11);
        }
    }
    assert(has_glyph);
    frame=fresh.advance({.pointer_x=300,.pointer_y=320,.click_pulse=true});
    assert(!frame.state_changed && !fresh.globals().active_profile);
    frame=fresh.advance({.managed_sound_playing=true});
    assert(!frame.state_changed);
    frame=fresh.advance({});
    assert(frame.state_changed && fresh.globals().active_profile==2);
    assert(frame.effects.next_outer_state==0x41);
    assert(fresh.globals().profile_metadata[2].name_length==1);
    assert(fresh.globals().player_progress[2].values[0]==1);

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
    for (const auto& filename : btb::profiles::activity_files_for_profile(3)) {
        std::ofstream out(path / filename);
        out << "previous activity data";
    }
    {
        std::ofstream untouched(path / "firedata1.txt");
        untouched << "keep";
    }

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
    for (const auto& filename : btb::profiles::activity_files_for_profile(3))
        assert(fs::file_size(path / filename) == 0);
    assert(fs::file_size(path / "firedata1.txt") == 4);
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
