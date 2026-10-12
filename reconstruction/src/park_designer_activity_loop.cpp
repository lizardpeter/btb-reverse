#include "btb/park_designer_activity_loop.hpp"

namespace btb::park_designer {
namespace {
[[nodiscard]] bool call_draw(
    LoopFrame& out,const LoopState& state,const SceneDraw& callback) {
    if(!callback){out.error="original Park Designer DrawParkDesignerActivity provider unavailable";return false;}
    std::string error;
    if(!callback(state.save,error)) {
        out.error=error.empty()?"Park Designer native scene draw failed":error;
        return false;
    }
    out.requested_draw=true;
    return true;
}
[[nodiscard]] bool call_editor(
    LoopFrame& out,LoopState& state,const EditorUpdate& callback) {
    if(!callback){out.error="original Park Designer UpdateParkDesignerEditorInteraction provider unavailable";return false;}
    std::string error;
    if(!callback(state.save,error)) {
        out.error=error.empty()?"Park Designer native editor update failed":error;
        return false;
    }
    out.requested_editor=true;
    return true;
}
}
void reset_park_designer_runtime(LoopState& state) noexcept {
    // Source 00410D10 loops all 400 +0x1C object_code entries.
    for(auto& obj:state.save.objects)obj.object_code=-1;
    state.save.state(TrailingStateIndex::NextDecorateRecord)=300;
    state.save.state(TrailingStateIndex::NextBandstandRecord)=200;
    state.save.state(TrailingStateIndex::NextPondPrimaryRecord)=0;
    state.save.state(TrailingStateIndex::PondLinkedRecord0)=-1;
    state.save.state(TrailingStateIndex::PondLinkedRecord1)=-1;
    state.save.state(TrailingStateIndex::PondLinkedRecord2)=-1;
    state.save.state(TrailingStateIndex::PondLinkedRecord3)=-1;
    state.save.state(TrailingStateIndex::PondLinkedRecord4)=0;
    state.save.state(TrailingStateIndex::PondLinkedCount)=0;
    state.save.state(TrailingStateIndex::SelectedObjectRecord)=-1;
    state.save.state(TrailingStateIndex::PondSelectedPrimary)=-1;
    state.save.state(TrailingStateIndex::PondSpecialRecord)=-1;
    state.save.state(TrailingStateIndex::BandstandSelectedRecord)=-1;
    state.save.state(TrailingStateIndex::ActiveEditorMode)=0;
    for(auto idx:kCurrentPageStateByEditorMode)state.save.state(idx)=0;
    state.save.state(TrailingStateIndex::LegacyPage4)=0;
    state.save.state(TrailingStateIndex::PondPaletteMaxPage)=0;
    state.save.state(TrailingStateIndex::BandstandPaletteMaxPage)=0;
    // Native reset selectively initializes the two first max-page entries.
    state.editor_cursor_mode=0;
    state.queued_voice=-1;
    state.reset_all_pending=false;
}

LoopFrame advance_park_designer_loop(
    LoopState& state,const LoopInput& input,
    const SceneDraw& draw,const EditorUpdate& edit) {

    LoopFrame out{};
    const bool blocked=state.exit_requested||input.global_input_blocked;
    const bool shutdown=state.shutdown_requested||input.global_shutdown;
    if(!blocked&&!shutdown) {
        if(state.reset_all_pending) {
            reset_park_designer_runtime(state);
            out.performed_reset=true;
        }
        // NoOpLegacyHook executes before every ordinary draw.
        if(!call_draw(out,state,draw))return out;
        if(!state.intro_music_pending) {
            if(!call_editor(out,state,edit))return out;
        } else {
            out.sound.push_back({LoopSoundKind::StopIntroSound});
            out.sound.push_back({LoopSoundKind::StartParkBackingTrack,2});
            if(state.intro_managed_voice_pending) {
                out.sound.push_back({LoopSoundKind::ManagedVoice,0x123,1});
                state.intro_managed_voice_pending=false;
            } else if(!input.managed_sound_playing) {
                state.intro_music_pending=false;
            }
        }
        out.compaction=compact_record_families(state.save);
        if(state.queued_voice>=0&&!input.managed_sound_playing) {
            out.sound.push_back({LoopSoundKind::InputInterruptibleVoice,
                                 state.queued_voice,state.queued_voice_context});
            state.queued_voice=-1;
        }
        return out; // native returns 0 while activity remains active
    }

    // Original closure 0x00509368: 0 speak, 1 await managed audio, 2 unload.
    if(state.outro_stage==0) {
        out.sound.push_back({LoopSoundKind::InputInterruptibleVoice,
                             static_cast<std::int32_t>(
                                 0xf6U+(input.random_value%3U)),1});
        state.outro_stage=1;
        (void)call_draw(out,state,draw);
        return out;
    }
    if(state.outro_stage==1) {
        if(!call_draw(out,state,draw))return out;
        if(!input.managed_sound_playing)state.outro_stage=2;
        return out;
    }
    out.performed_unload=true;
    out.completed=true;
    out.mark_park_progress=has_any_placed_object(state.save);
    if(shutdown) {
        state.shutdown_requested=false;
        state.outer_state=4; // original DAT_0044DE14 := 4 (Activity Select)
        out.return_to_activity_select=true;
    }
    return out; // native returns 1
}
} // namespace btb::park_designer
