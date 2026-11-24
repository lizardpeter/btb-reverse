#include "btb/original_active_frame.hpp"
namespace btb::original_active_frame {
namespace {
[[nodiscard]] constexpr bool failed(U32 hr) noexcept {
    return (hr&0x80000000U)!=0;
}
}
Result run_active_game_frame(
    State& state,U32 now,const Callbacks& c) noexcept {
    // Retail checks the removable CD every 101 invocations, NOT once
    // per visible animation frame. Preserve this counter when gated by time.
    ++state.media_counter;
    if(state.media_counter>100U){
        state.media_counter=0;
        if(c.check_original_disc){
            if(!c.check_original_disc(c.context))
                return {Outcome::Failed,0x80004005U};
        } else if(!state.media_check_waived){
            // Never silently claim the original CD/volume check succeeded.
            return {Outcome::Failed,0x80004005U};
        }
    }
    if(now==state.last_time_ms)return {};
    if(state.bitmap_reload_requested){
        if(!c.reload_original_bitmaps||
           !c.reload_original_bitmaps(c.context))
            return {Outcome::Failed,0x80004005U};
        state.bitmap_reload_requested=false;
        if(state.contextual_help_active){
            state.contextual_help_active=false;
            if(c.reset_contextual_help_after_reload)
                c.reset_contextual_help_after_reload(c.context);
        }
    }
    state.last_time_ms=now;
    if(c.reap_finished_sounds)c.reap_finished_sounds(c.context);
    if(!state.contextual_help_active){
        if(!c.run_main_game_flow||!c.run_main_game_flow(c.context))
            return {Outcome::Failed,0x80004005U};
    } else if(c.update_contextual_help)
        c.update_contextual_help(c.context);
    if(state.contextual_help_request>1U&&c.enter_contextual_help)
        c.enter_contextual_help(c.context);
    if(c.poll_direct_input)c.poll_direct_input(c.context);
    const U32 cooperative=c.check_directdraw_cooperative
        ? c.check_directdraw_cooperative(c.context):0U;
    if(failed(cooperative)){
        if(c.cancel_pending_frame)c.cancel_pending_frame(c.context);
        if(cooperative==0x887600e1U||cooperative==0x88760245U){
            if(c.sleep_ms)c.sleep_ms(10U,c.context);
            return {Outcome::Recoverable,0U};
        }
        if(cooperative==0x8876024bU){
            const U32 reset=c.reset_display_manager
                ? c.reset_display_manager(c.context):0x80004005U;
            return {failed(reset)?Outcome::Failed:Outcome::Recoverable,reset};
        }
        return {Outcome::Failed,cooperative};
    }
    const U32 presented=c.present_original_game_frame
        ? c.present_original_game_frame(c.context):0x80004005U;
    if(failed(presented)){
        if(presented==0x887601c2U){
            if(c.restore_all_surfaces)
                c.restore_all_surfaces(c.context);
            return {Outcome::Recoverable,0U};
        }
        return {Outcome::Failed,presented};
    }
    return {Outcome::Submitted,0U};
}
} // namespace btb::original_active_frame
