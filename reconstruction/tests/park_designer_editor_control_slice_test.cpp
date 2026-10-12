#include "btb/park_designer_editor_control_slice.hpp"
#include <cassert>
using namespace btb::park_designer;
int main() {
    SaveData data{};
    EditorControlState state{};
    data.state(TrailingStateIndex::ActiveEditorMode)=0;
    auto out=apply_park_toolbar_click(data,state,7);
    assert(out.action==ControlAction::SwitchMode&&state.first_mode_voice_pending);
    assert(data.state(TrailingStateIndex::ActiveEditorMode)==0);
    state.first_mode_voice_pending=false;
    out=apply_park_toolbar_click(data,state,8);
    assert(out.action==ControlAction::SwitchMode&&state.first_mode_voice_pending);
    assert(data.state(TrailingStateIndex::ActiveEditorMode)==1);
    out=apply_park_toolbar_click(data,state,11);
    assert(out.action==ControlAction::EnterDelete&&state.cursor_mode==5);
    out=apply_park_toolbar_click(data,state,11);
    assert(out.action==ControlAction::ExitDelete&&state.cursor_mode==0);
    data.state(TrailingStateIndex::BandstandPalettePage)=1;
    state.scroll_up_available=false;
    out=apply_park_toolbar_click(data,state,5);
    assert(out.action==ControlAction::None);
    assert(data.state(TrailingStateIndex::BandstandPalettePage)==1);
    state.scroll_up_available=true;
    out=apply_park_toolbar_click(data,state,5);
    assert(out.action==ControlAction::Scroll&&state.scroll_animation==61);
    assert(data.state(TrailingStateIndex::BandstandPalettePage)==0);
    state.scroll_down_available=true;
    out=apply_park_toolbar_click(data,state,6);
    assert(out.action==ControlAction::Scroll&&state.scroll_animation==-61);
    assert(data.state(TrailingStateIndex::BandstandPalettePage)==1);
    // View control 10 assigns mode directly; Delete index11 launches
    // shared Yes/No with voice 300 rather than toggling delete cursor.
    state.first_mode_voice_pending=false;
    out=apply_park_toolbar_click(data,state,10);
    assert(out.action==ControlAction::SwitchMode&&!state.first_mode_voice_pending);
    assert(data.state(TrailingStateIndex::ActiveEditorMode)==3);
    out=apply_park_toolbar_click(data,state,11);
    assert(out.action==ControlAction::ConfirmDeleteAll);
    assert(out.managed_voice&&*out.managed_voice==300);
    assert(out.use_original_delete_all_bitmap);
    assert(state.delete_all_confirmation_pending&&state.cursor_mode==0);

    // View toolbar source 0040E440: summer/winter use random
    // managed voice alternatives 294/295 and 297/298, print is delegated.
    out=apply_park_view_click(data,ViewToolbarItem::Summer,1);
    assert(out.action==ControlAction::ChangeSeason);
    assert(out.managed_voice&&*out.managed_voice==295);
    assert(out.season==Season::Summer);
    assert(data.state(TrailingStateIndex::Season)==0);
    out=apply_park_view_click(data,ViewToolbarItem::Winter,0);
    assert(out.action==ControlAction::ChangeSeason);
    assert(out.managed_voice&&*out.managed_voice==297);
    assert(data.state(TrailingStateIndex::Season)==1);
    out=apply_park_view_click(data,ViewToolbarItem::Print,0);
    assert(out.action==ControlAction::PrintImage);
    out=apply_park_view_click(data,ViewToolbarItem::InfoHoverOnly,0);
    assert(out.action==ControlAction::None);
    out=apply_park_view_click(data,ViewToolbarItem::Summer,2);
    assert(out.action==ControlAction::None);
    assert(data.state(TrailingStateIndex::Season)==1);
    data.state(TrailingStateIndex::ActiveEditorMode)=2;
    out=apply_park_view_click(data,ViewToolbarItem::Summer,0);
    assert(out.action==ControlAction::None);
    assert(data.state(TrailingStateIndex::Season)==1);
}
