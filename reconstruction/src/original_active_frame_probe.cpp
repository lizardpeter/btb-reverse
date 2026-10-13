#include "btb/original_active_frame.hpp"
namespace btb::original_active_frame {
namespace {
struct Host {
 int disc{},reload{},reset_help{},reap{},game{},help{},enter{},
     poll{},coop{},reset{},present{},restore{},cancel{},sleep{};
 bool disc_valid{true},reload_valid{true},game_valid{true};
 U32 cooperative{},reset_hr{},present_hr{},slept_ms{};
};
Host& host(void* p){return *static_cast<Host*>(p);}
bool disc(void* p){++host(p).disc;return host(p).disc_valid;}
bool reload(void* p){++host(p).reload;return host(p).reload_valid;}
void clear_help(void* p){++host(p).reset_help;}
void reap(void* p){++host(p).reap;}
bool game(void* p){++host(p).game;return host(p).game_valid;}
void help(void* p){++host(p).help;}
void enter(void* p){++host(p).enter;}
void poll(void* p){++host(p).poll;}
U32 coop(void* p){++host(p).coop;return host(p).cooperative;}
U32 reset(void* p){++host(p).reset;return host(p).reset_hr;}
U32 present(void* p){++host(p).present;return host(p).present_hr;}
void restore(void* p){++host(p).restore;}
void cancel(void* p){++host(p).cancel;}
void sleep(U32 ms,void* p){++host(p).sleep;host(p).slept_ms=ms;}
Callbacks hooks(Host& h){
 return {&h,disc,reload,clear_help,reap,game,help,enter,poll,coop,
         reset,present,restore,cancel,sleep};
}
}
bool verify_source_frame_control_flow() noexcept {
 Host h{};
 State state{};
 auto fn=hooks(h);
 const auto first=run_active_game_frame(state,0,fn);
 if(first.outcome!=Outcome::Skipped||state.media_counter!=1||
    h.game!=0||h.present!=0)return false;
 const auto ready=run_active_game_frame(state,1,fn);
 if(ready.outcome!=Outcome::Submitted||h.reap!=1||h.game!=1||
    h.poll!=1||h.coop!=1||h.present!=1||state.last_time_ms!=1)
    return false;
 state.media_counter=100;
 h.disc_valid=false;
 const auto denied=run_active_game_frame(state,2,fn);
 if(denied.outcome!=Outcome::Failed||h.disc!=1||
    state.media_counter!=0||h.game!=1)return false;
 h.disc_valid=true;
 state.bitmap_reload_requested=true;
 state.contextual_help_active=true;
 state.contextual_help_request=2;
 const auto reloaded=run_active_game_frame(state,3,fn);
 if(reloaded.outcome!=Outcome::Submitted||h.reload!=1||
    h.reset_help!=1||h.enter!=1||h.game!=2||
    state.contextual_help_active||state.bitmap_reload_requested)
    return false;
 state.contextual_help_active=true;
 const auto contextual=run_active_game_frame(state,4,fn);
 if(contextual.outcome!=Outcome::Submitted||
    h.help!=1||h.game!=2)return false;
 state.contextual_help_active=false;
 h.cooperative=0x887600e1U;
 const auto busy=run_active_game_frame(state,5,fn);
 if(busy.outcome!=Outcome::Recoverable||h.sleep!=1||
    h.slept_ms!=10||h.cancel!=1||h.present!=3)return false;
 h.cooperative=0x88760245U;
 const auto busy2=run_active_game_frame(state,6,fn);
 if(busy2.outcome!=Outcome::Recoverable||h.sleep!=2||
    h.cancel!=2)return false;
 h.cooperative=0x8876024bU;
 const auto wrong_mode=run_active_game_frame(state,7,fn);
 if(wrong_mode.outcome!=Outcome::Recoverable||h.reset!=1||
    h.cancel!=3||h.present!=3)return false;
 h.cooperative=0x887601c2U;
 const auto fatal=run_active_game_frame(state,8,fn);
 if(fatal.outcome!=Outcome::Failed||
    fatal.original_status!=0x887601c2U)return false;
 h.cooperative=0;
 h.present_hr=0x887601c2U;
 const auto lost=run_active_game_frame(state,9,fn);
 if(lost.outcome!=Outcome::Recoverable||h.restore!=1)return false;
 h.present_hr=0;
 state.media_counter=100;fn.check_original_disc=nullptr;
 state.media_check_waived=true;
 const auto waived=run_active_game_frame(state,10,fn);
 if(waived.outcome!=Outcome::Submitted||state.media_counter!=0)return false;
 state.media_counter=100;state.media_check_waived=false;
 const auto blocked=run_active_game_frame(state,11,fn);
 return blocked.outcome==Outcome::Failed;
}
}
