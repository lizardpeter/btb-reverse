#include "btb/front_end_ui_data.hpp"

#include <cassert>
#include <sstream>

using namespace btb::front_end;

int main() {
    std::istringstream counts(
        "8\n10\n6\n5\n5\n4\n4\n3\n3\n2\n3\n2\n-1\n");
    const auto parsed_counts = parse_hot_area_counts(counts);
    assert(parsed_counts.size() == 12);
    assert(parsed_counts[0] == 8);
    assert(parsed_counts[1] == 10);
    assert(parsed_counts[11] == 2);

    std::istringstream geometry(R"(
main_ui_screen---start_game
295 416
344 416
344 466
295 466
-1 -1 start_game
120 102
490 102
490 396
120 426
-1 -1 quit_game
-2 -2 game_select_screen---pets_corner
385 170
620 170
620 304
385 304
-1 -1
-2 -2 InstructionScreen_before_subgame_start---back
574 418
625 418
625 469
574 469
-1 -1 back
-99 -99
)");

    const auto screens = parse_hot_areas(geometry);
    assert(screens.size() == 3);
    assert(screens[0].raw_header == "main_ui_screen---start_game");
    assert(screens[0].areas.size() == 2);
    assert(screens[0].areas[0].polygon.size() == 4);
    assert(screens[0].areas[0].polygon[0] == Point{295,416});
    assert(screens[0].areas[0].polygon[3] == Point{295,466});
    assert(screens[0].areas[0].separator_label == "start_game");
    assert(screens[0].areas[1].separator_label == "quit_game");

    assert(screens[1].raw_header == "game_select_screen---pets_corner");
    assert(screens[1].areas.size() == 1);
    assert(screens[1].areas[0].polygon[0] == Point{385,170});
    // Retail source omits the optional action token on the final -1 row.
    assert(screens[1].areas[0].separator_label.empty());

    assert(
        screens[2].raw_header ==
        "InstructionScreen_before_subgame_start---back");
    assert(screens[2].areas.size() == 1);
    assert(screens[2].areas[0].separator_label == "back");

    std::istringstream replacements(R"(
Game_select_screen
385 170 385 170 data//ui//actanims//petsh 7 39 -1 NULL 12 20 NULL 451 10 data//ui//actanims//petsb
574 418 425 317 data//ui//backred 429 456 -1 NULL -1 1 data//ui//backreddep.bmp -1 0 NULL
-1 Information_screen_pre_game
574 418 425 317 data//ui//backred 429 456 -1 NULL -1 1 data//ui//backreddep.bmp -1 0 NULL
404 381 160 317 data//ui//instruction//startred 433 460 -1 NULL -5 1 data//ui//instruction//startdepred.bmp -1 0 NULL
-1 FireworkEditPlayAgain
220 260 220 160 data//ui//editplay//Fireworkeditred 486 -1 -1 NULL -20 1
data//ui//editplay//Fireworkeditdep.bmp -1 0 NULL
-1 End
)");

    const auto replacement_screens = parse_replacement_table(replacements);
    assert(replacement_screens.size() == 3);

    assert(replacement_screens[0].raw_header == "Game_select_screen");
    assert(replacement_screens[0].records.size() == 2);

    const auto& pets = replacement_screens[0].records[0];
    assert(pets.x == 385 && pets.y == 170);
    assert(pets.secondary_x == 385 && pets.secondary_y == 170);
    assert(pets.hover_bitmap_base == "data//ui//actanims//petsh");
    assert((pets.hover_sound_ids == std::array<std::int32_t,3>{7,39,-1}));
    assert(pets.optional_surface.empty());
    assert(pets.target_state_or_action == 12);
    assert(pets.hover_frame_count == 20);
    assert(pets.pressed_bitmap.empty());
    assert(pets.click_sound_id == 451);
    assert(pets.secondary_frame_count == 10);
    assert(pets.secondary_bitmap_base == "data//ui//actanims//petsb");
    assert(available_hover_sound_count(pets) == 2);
    assert(selected_hover_sound_id(pets,0) == 7);
    assert(selected_hover_sound_id(pets,1) == 39);
    assert(selected_hover_sound_id(pets,2) == -1);

    assert(choose_hover_sound_index(pets,0) == 0);
    assert(choose_hover_sound_index(pets,1) == 1);
    assert(choose_hover_sound_index(pets,2) == 0);

    // Exact retail quirk: count non--1 entries, but directly index the first
    // count slots instead of compacting holes. Shipped tables put -1 at the
    // tail, so this malformed synthetic record proves the preserved behavior.
    ReplacementRecord holey;
    holey.hover_sound_ids = {10,-1,30};
    assert(available_hover_sound_count(holey) == 2);
    assert(choose_hover_sound_index(holey,1) == 1);
    assert(selected_hover_sound_id(holey,1) == -1);

    const auto& back = replacement_screens[0].records[1];
    assert(back.target_state_or_action == -1);
    assert(back.hover_frame_count == 1);
    assert(back.pressed_bitmap == "data//ui//backreddep.bmp");
    assert(back.click_sound_id == -1);
    assert(back.secondary_frame_count == 0);
    assert(back.secondary_bitmap_base.empty());

    const auto& start = replacement_screens[1].records[1];
    assert(start.target_state_or_action == -5);
    assert(start.hover_sound_ids[0] == 433);
    assert(start.hover_sound_ids[1] == 460);
    assert(start.pressed_bitmap ==
           "data//ui//instruction//startdepred.bmp");

    // The parser is token-based exactly where retail fscanf is token-based,
    // so a 15-field row may wrap onto the next physical line.
    const auto& edit = replacement_screens[2].records[0];
    assert(edit.target_state_or_action == -20);
    assert(edit.hover_bitmap_base ==
           "data//ui//editplay//Fireworkeditred");
    assert(edit.pressed_bitmap ==
           "data//ui//editplay//Fireworkeditdep.bmp");
    assert(edit.hover_sound_ids[0] == 486);
    assert(available_hover_sound_count(edit) == 1);
}
