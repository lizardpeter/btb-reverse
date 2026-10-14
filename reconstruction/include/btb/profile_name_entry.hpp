#pragma once

#include "btb/player_profiles.hpp"
#include <span>
#include <vector>
#include <optional>

namespace btb::profiles {

// 0x0042DCC0, UpdateEnterNamePopup. These are the six original strict
// rectangles, including two voice-only regions following the four buttons.
inline constexpr std::array<ProfileHitRect, 6> kNameEntryHitRects{{
    {252,111,281,149}, {359,111,390,149},
    {494,305,545,357}, {276,303,363,361},
    {194,189,449,235}, {242,99,397,147},
}};
inline constexpr std::array<std::array<int,2>,4> kNameEntryButtonPositions{{
    {258,112}, {367,112}, {494,305}, {276,303},
}};

struct NameEntryState {
    std::size_t profile_index{}; // DAT_0051B3A0
    int length{};              // DAT_0051BE48
    int badge{-1};             // DAT_0051C248
    std::array<std::uint8_t,kMaxNameGlyphs> glyphs{};
    int previous_scan_code{-1}; // DAT_00446FF4
    int previous_hover{-1};     // DAT_00446FFC
    int hovered{-1};            // DAT_00447000 (unchanged on pointer exit)
    bool awaiting_accept_voice{}; // DAT_0051C39C
    bool popup_active{true};      // DAT_0051C2F4
};
struct NameEntryInput {
    std::span<const std::uint8_t> keyboard; // first 0xED DirectInput bytes
    int translated_character{-1};          // DAT_004FBD44
    int pointer_x{},pointer_y{};
    bool button_held{}; // DAT_004FBFB8: pressed button presentation
    bool click_pulse{}; // DAT_004FBFB4: committed action
    bool managed_sound_playing{};
};
struct NameEntryVoice { int id{},priority{50},arbitration_class{2}; };
struct NameEntryButtonDraw { int button{},x{},y{}; bool pressed{}; };
struct NameEntryEffects {
    std::vector<NameEntryVoice> voices;
    std::vector<NameEntryButtonDraw> buttons;
    bool clear_translated_character{};
    // Source cancel replaces registered slot 0x51C268 with name_signs.bmp.
    bool reload_name_signs{};
    std::optional<int> selected_profile;
    std::optional<int> next_outer_state;
};

// Real state/progress mutations, with typed effects for the shared native
// bitmap and sound services. Invalid host state is rejected before mutation.
NameEntryEffects update_enter_name_popup(
    NameEntryState&,const NameEntryInput&,ProfileTable&,
    std::array<btb::progress::Record,kProfileCount>&);

} // namespace btb::profiles
