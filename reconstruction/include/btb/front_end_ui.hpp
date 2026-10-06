#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace btb::front_end {

enum class Screen : std::int32_t {
    MainStart = 0,
    ActivitySelect = 1,
    InstructionWithDifficulty = 2,
    DinoChooser = 3,
    MusicChooser = 4,
    SpudChooser = 5,
    AdventurePlaygroundChooser = 6,
    FireworksInstruction = 7,
    InstructionNoDifficulty = 8,
    PlayAgainYesNo = 9,
    PlayAgainDifficulty = 10,
    FireworksEditViewReplay = 11,
};

inline constexpr std::size_t kScreenCount = 12;

struct ScreenDescriptor {
    Screen screen{};
    std::string_view semantic_name{};
    std::int32_t hot_area_count{};
    std::string_view raw_hot_area_label{};
    std::string_view replacement_table_label{};
};

inline constexpr std::array<ScreenDescriptor,kScreenCount> kScreens{{
    {Screen::MainStart,"main_start",8,"main_ui_screen---start_game","Main_start_screen"},
    {Screen::ActivitySelect,"activity_select",10,"game_select_screen---pets_corner","Game_select_screen"},
    {Screen::InstructionWithDifficulty,"instruction_with_difficulty",6,"InstructionScreen_before_subgame_start---back","Information_screen_pre_game"},
    {Screen::DinoChooser,"dino_chooser",5,"Dino_sub_chooser----back","Dino_sub_chooser"},
    // Retail source-data typo: uiHotArea.txt repeats Dino_sub_chooser for index
    // 4, while uiHotAreaReplace.txt correctly identifies MUSIC_sub_chooser.
    {Screen::MusicChooser,"music_chooser",5,"Dino_sub_chooser----back","MUSIC_sub_chooser"},
    {Screen::SpudChooser,"spud_chooser",4,"Skatespud_chooser","skate_spud_chooser"},
    {Screen::AdventurePlaygroundChooser,"adventure_playground_chooser",4,"advent_chooser","Advent_playground"},
    {Screen::FireworksInstruction,"fireworks_instruction",3,"InstructionScreen_before_subgame_startFirework---back","Information_screen_pre_game_for_Firework"},
    {Screen::InstructionNoDifficulty,"instruction_no_difficulty",3,"InstructionScreen_before_subgame_start_NoDiff_level---back","Information_screen_pre_game_for_NO_diff_levels"},
    {Screen::PlayAgainYesNo,"play_again_yes_no",2,"PLAY_AGAIN_YES_NO","Play_again_yes_no"},
    {Screen::PlayAgainDifficulty,"play_again_with_difficulty",3,"PLAY_AGAIN_WITH_DIFF_SELECT--easy","PLAY_AGAIN_WITH_DIFF"},
    {Screen::FireworksEditViewReplay,"fireworks_edit_view_replay",2,"FireworkdEditPlayagain","FireworkEditPlayAgain"},
}};

inline constexpr std::array<std::int32_t,kScreenCount>
kRetailHotAreaCounts{{8,10,6,5,5,4,4,3,3,2,3,2}};

[[nodiscard]] constexpr const ScreenDescriptor* descriptor(
    std::int32_t index) noexcept {
    if (index < 0 || index >= static_cast<std::int32_t>(kScreenCount)) {
        return nullptr;
    }
    return &kScreens[static_cast<std::size_t>(index)];
}

[[nodiscard]] constexpr bool has_raw_label_mismatch(Screen screen) noexcept {
    return screen == Screen::MusicChooser;
}

inline constexpr std::string_view kUiBitmapNameFile =
    "loaddata\\uiBitmapName.txt";
inline constexpr std::string_view kNumUiHotAreaFile =
    "loaddata\\NumUiHotArea.txt";
inline constexpr std::string_view kUiHotAreaFile =
    "loaddata\\uiHotArea.txt";
inline constexpr std::string_view kUiHotAreaReplaceFile =
    "loaddata\\uiHotAreaReplace.txt";

enum class CommonAction : std::int32_t {
    Back = -1,
    Easy = -2,
    Medium = -3,
    Hard = -4,
    StartOrWalkthrough = -5,
    Help = -6,

    PlayAgainYesOrFireworksEdit = -20,
    PlayAgainNoOrFireworksView = -21,

    ReplayEasy = -30,
    ReplayMedium = -31,
    ReplayHard = -32,
};

[[nodiscard]] constexpr bool is_negative_ui_action(
    std::int32_t value) noexcept {
    return value < 0;
}

[[nodiscard]] constexpr bool is_common_navigation_action(
    std::int32_t value) noexcept {
    switch (value) {
    case -1:
    case -2:
    case -3:
    case -4:
    case -5:
    case -6:
    case -20:
    case -21:
    case -30:
    case -31:
    case -32:
        return true;
    default:
        return false;
    }
}

} // namespace btb::front_end
