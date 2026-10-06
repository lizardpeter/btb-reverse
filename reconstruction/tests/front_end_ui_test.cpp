#include "btb/front_end_ui.hpp"

#include <cassert>

using namespace btb::front_end;

int main() {
    static_assert(kScreenCount == 12);
    static_assert(kRetailHotAreaCounts[0] == 8);
    static_assert(kRetailHotAreaCounts[1] == 10);
    static_assert(kRetailHotAreaCounts[2] == 6);
    static_assert(kRetailHotAreaCounts[3] == 5);
    static_assert(kRetailHotAreaCounts[4] == 5);
    static_assert(kRetailHotAreaCounts[5] == 4);
    static_assert(kRetailHotAreaCounts[6] == 4);
    static_assert(kRetailHotAreaCounts[7] == 3);
    static_assert(kRetailHotAreaCounts[8] == 3);
    static_assert(kRetailHotAreaCounts[9] == 2);
    static_assert(kRetailHotAreaCounts[10] == 3);
    static_assert(kRetailHotAreaCounts[11] == 2);

    for (std::size_t i = 0; i < kScreens.size(); ++i) {
        assert(static_cast<std::size_t>(kScreens[i].screen) == i);
        assert(kScreens[i].hot_area_count == kRetailHotAreaCounts[i]);
        assert(!kScreens[i].semantic_name.empty());
    }

    constexpr auto* activity = descriptor(1);
    static_assert(activity);
    static_assert(activity->screen == Screen::ActivitySelect);
    static_assert(activity->hot_area_count == 10);
    static_assert(activity->replacement_table_label == "Game_select_screen");

    constexpr auto* music = descriptor(4);
    static_assert(music);
    static_assert(music->screen == Screen::MusicChooser);
    static_assert(music->hot_area_count == 5);
    static_assert(music->raw_hot_area_label == "Dino_sub_chooser----back");
    static_assert(music->replacement_table_label == "MUSIC_sub_chooser");
    static_assert(has_raw_label_mismatch(Screen::MusicChooser));
    static_assert(!has_raw_label_mismatch(Screen::DinoChooser));

    constexpr auto* fireworks_replay = descriptor(11);
    static_assert(fireworks_replay);
    static_assert(
        fireworks_replay->screen == Screen::FireworksEditViewReplay);
    static_assert(fireworks_replay->hot_area_count == 2);

    static_assert(descriptor(-1) == nullptr);
    static_assert(descriptor(12) == nullptr);

    static_assert(static_cast<int>(CommonAction::Back) == -1);
    static_assert(static_cast<int>(CommonAction::Help) == -6);
    static_assert(
        static_cast<int>(CommonAction::PlayAgainYesOrFireworksEdit) == -20);
    static_assert(
        static_cast<int>(CommonAction::PlayAgainNoOrFireworksView) == -21);
    static_assert(static_cast<int>(CommonAction::ReplayEasy) == -30);
    static_assert(static_cast<int>(CommonAction::ReplayHard) == -32);

    static_assert(is_negative_ui_action(-1));
    static_assert(!is_negative_ui_action(7));
    static_assert(is_common_navigation_action(-1));
    static_assert(is_common_navigation_action(-32));
    static_assert(!is_common_navigation_action(-7));
    static_assert(!is_common_navigation_action(24));

    static_assert(kUiBitmapNameFile == "loaddata\\uiBitmapName.txt");
    static_assert(kNumUiHotAreaFile == "loaddata\\NumUiHotArea.txt");
    static_assert(kUiHotAreaFile == "loaddata\\uiHotArea.txt");
    static_assert(kUiHotAreaReplaceFile == "loaddata\\uiHotAreaReplace.txt");
}
