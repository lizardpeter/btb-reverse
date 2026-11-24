#pragma once
// Verified control slices of the large 0040E440 Park Designer editor.
// This is NOT a full source reconstruction of the 10,433-byte original
// function. Placement/drag, scrolling presentation and DirectDraw ownership
// still require recovered native integration; do not count it as completed.
#include "btb/park_designer_data.hpp"
#include "btb/park_designer_runtime.hpp"
#include <cstdint>
#include <optional>

namespace btb::park_designer {

struct EditorControlState {
    std::int32_t cursor_mode{};          // DAT_00507B14: 0 idle, 5 delete
    std::int32_t scroll_animation{};     // DAT_00509348: +/-0x3D
    bool scroll_up_available{};          // DAT_00507A20
    bool scroll_down_available{};        // DAT_00507A24
    bool first_mode_voice_pending{};     // DAT_00441F18
    bool delete_all_confirmation_pending{};
};

enum class ControlAction {
    None, SelectPlacement, SwitchMode, Scroll, EnterDelete,
    ExitDelete, ConfirmDeleteAll, ChangeSeason, PrintImage,
};
struct ControlResult {
    ControlAction action{ControlAction::None};
    std::optional<int> managed_voice{};
    bool use_original_delete_all_bitmap{};
    std::optional<Season> season{};
};

// A toolbar hit index is from HitTestParkDesignerToolbar (0x0040D420):
// [0,4]=palette, 5/6=up/down, 7..10=modes, 11=delete/clear.
// Here we only close the editor's control-state transitions, not palette
// cursor synthesis. Empty/missing indices are not fabricated selections.
[[nodiscard]] inline ControlResult apply_park_toolbar_click(
    SaveData& data,EditorControlState& state,int index) noexcept {

    const int mode=data.state(TrailingStateIndex::ActiveEditorMode);
    if(mode<0||mode>3)return {};
    if(index>=0&&index<5) {
        if(mode==3)return {};
        return {ControlAction::SelectPlacement};
    }
    if(index>=7&&index<=10) {
        data.state(TrailingStateIndex::ActiveEditorMode)=index-7;
        if(index!=10)state.first_mode_voice_pending=true;
        return {ControlAction::SwitchMode};
    }
    if(index==11) {
        if(mode==static_cast<int>(EditorMode::View)) {
            state.delete_all_confirmation_pending=true;
            return {ControlAction::ConfirmDeleteAll,300,true};
        }
        if(state.cursor_mode==5) {
            state.cursor_mode=0;
            return {ControlAction::ExitDelete};
        }
        state.cursor_mode=5;
        return {ControlAction::EnterDelete};
    }
    if(index==5&&state.scroll_up_available) {
        state.scroll_animation=0x3d;
        --data.state(kCurrentPageStateByEditorMode[static_cast<std::size_t>(mode)]);
        return {ControlAction::Scroll};
    }
    if(index==6&&state.scroll_down_available) {
        state.scroll_animation=-0x3d;
        ++data.state(kCurrentPageStateByEditorMode[static_cast<std::size_t>(mode)]);
        return {ControlAction::Scroll};
    }
    return {};
}

[[nodiscard]] inline ControlResult apply_park_view_click(
    SaveData& data,ViewToolbarItem item,std::int32_t random_mod2) noexcept {
    if(data.state(TrailingStateIndex::ActiveEditorMode)!=3)return {};
    const auto original=view_toolbar_click(item,random_mod2);
    if(original.season) {
        data.state(TrailingStateIndex::Season)=
            static_cast<std::int32_t>(*original.season);
        return {ControlAction::ChangeSeason,original.sound_id,false,
                original.season};
    }
    if(original.action==ViewToolbarAction::PrintCurrentFrame)
        return {ControlAction::PrintImage};
    return {};
}
} // namespace btb::park_designer
