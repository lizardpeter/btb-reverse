#include "btb/original_closure4.hpp"
#include <cassert>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_closure4;
struct WindowHost final:WindowIO{
 std::vector<std::string> events;
 bool pending{},quit_msg{},input_failure{};
 I32 active_frames{},credit_updates{},game_flows{};
 std::string registry_install_path(std::string_view name) override{
  assert(name.find("Install Path")!=std::string_view::npos);
  events.push_back("registry");return "C:\\retail\\Bob";
 }
 void change_original_working_directory(std::string_view p) override{
  assert(p=="C:\\retail\\Bob");events.push_back("chdir");
 }
 void setup_runtime_and_help() override{events.push_back("help");}
 void disable_window_accessibility_keys() override{events.push_back("accessibility");}
 U32 original_first_seed() override{events.push_back("firstseed");return 123;}
 U32 tick_count() override{return 456;}
 void seed_original_prng(U32 seed) override{
  assert(seed==123||seed==456);events.push_back("seed");
 }
 void load_original_global_tables() override{events.push_back("globaltables");}
 I32 create_game_window(U32,U32,U32&,U32&) = delete;
 I32 create_game_window(U32 inst,I32 show,U32& win,U32& acc) override{
  assert(inst==99&&show==1);win=10;acc=11;
  events.push_back("window");return 0;
 }
 I32 initialize_original_display(U32 window) override{
  assert(window==10);events.push_back("display");return 0;
 }
 U32 create_original_sound_manager(U32 bytes) override{
  assert(bytes==0x7738U);events.push_back("sound");return 8;
 }
 I32 initialize_directinput() override{return input_failure?-1:0;}
 void initialize_original_bink() override{events.push_back("bink");}
 U32 install_original_fullscreen_key_hook(U32) override{
  events.push_back("hook");return 5;
 }
 bool peek_windows_message() override{return pending;}
 bool get_windows_message(Message& msg) override{
  pending=false;msg={1,12,0};
  return !quit_msg;
 }
 bool translate_accelerator(U32 win,U32 acc,const Message&) override{
  assert(win==10&&acc==11);events.push_back("accelerator");return false;
 }
 void translate_and_dispatch(const Message&) override{events.push_back("dispatch");}
 void load_original_credits() override{events.push_back("credits");}
 void update_original_credits() override{++credit_updates;}
 void run_active_game_frame(U32) override{++active_frames;}
 void run_original_game_flow(U32) override{++game_flows;}
 void destroy_display_manager() override{events.push_back("destroy-display-manager");}
 void release_original_input() override{events.push_back("release-input");}
 void shutdown_original_directsound() override{events.push_back("shutdown-sound");}
 void destroy_original_sound(U32 handle) override{
  assert(handle==8);events.push_back("destroy-sound");
 }
 void destroy_original_display(U32 handle) override{
  assert(handle!=0);events.push_back("destroy-display");
 }
 void uninstall_original_hook(U32 handle) override{
  assert(handle==5);events.push_back("unhook");
 }
 void post_quit_message() override{events.push_back("postquit");}
 void wait_windows_message() override{events.push_back("wait");}
 void original_message_cd_removed() override{events.push_back("cdremoved");}
 void destroy_game_window(U32 w) override{assert(w==10);events.push_back("destroy-window");}
};
struct FlowHost final:FlowIO{
 std::vector<std::string> events{};
 I32 bink{},startup{},activity_result{},delegated{},initialized{};
 void unload_original_generic_ui() override{events.push_back("unloadui");}
 void release_original_ui_surface() override{events.push_back("releaseui");}
 void unload_original_activity() override{events.push_back("unloadactivity");}
 void pause_original_bink() override{events.push_back("pause");}
 void update_options() override{events.push_back("options");}
 void update_progress() override{events.push_back("progress");}
 void update_yes_no() override{events.push_back("yesno");}
 void update_quit() override{events.push_back("quit");}
 void update_leave() override{events.push_back("leave");}
 void update_play_again() override{events.push_back("again");}
 void original_noop_hook() override{events.push_back("noop");}
 I32 update_bink_movie() override{events.push_back("movie");return bink;}
 void restore_original_movie_scene(I32 x) override{
  assert(x==2||x==7);events.push_back("restore");
 }
 I32 update_startup_video() override{return startup;}
 I32 run_original_activity(Activity a) override{
  assert(static_cast<I32>(a)>=0xf&&static_cast<I32>(a)<=0x3b);
  events.push_back("activity");return activity_result;
 }
 void update_generic_help_hover_voice() override{events.push_back("hover");}
 void update_help_button() override{events.push_back("help");}
 void initialize_activity(Activity a) override{
  ++initialized;events.push_back("init");
  assert(a==Activity::Dino||a==Activity::Squirrel||a==Activity::ParkDesigner);
 }
 void begin_loading_cursor_animation() override{events.push_back("loadcursor");}
 void close_original_walkthrough_movie() override{events.push_back("closemovie");}
 void process_original_remaining_scene(I32 scene,GameFlowState&) override{
  ++delegated;assert(scene>=0);events.push_back("delegated");
 }
};
struct FloatHost final:FloatIO{
 I32 packs{},finite{};
 std::string text{};I32 exponent{};bool neg{};
 Ext80 pack_original_x87_decimal(std::string_view digits,I32 exp,bool negative) override{
  ++packs;text=std::string(digits);exponent=exp;neg=negative;
  return {0x123456U,0x80000000U,static_cast<std::uint16_t>(
    negative?0xc000U:0x4000U)};
 }
 std::string original_finite_decimal_digits(const Ext80& x,I32 precision,
                                             bool fraction,I32& dp) override{
  ++finite;assert(x.mantissa_hi==0x80000000U);
  assert(precision==5&&fraction);dp=1;return "12345";
 }
};
int main(){
 {
  WindowHost io;WindowState s;s.display_active=1;
  assert(run_winmain_00402700(s,io,99,1,2)==-2);
  assert(s.initialized&&s.sound_manager==8&&s.hook_installed);
  assert(io.active_frames==2&&s.pump_iterations==2);
  assert(io.events[0]=="registry"&&io.events[1]=="chdir");
  assert(io.events[2]=="help"&&io.events[3]=="accessibility");
  io.pending=true;
  assert(run_winmain_00402700(s,io,99,1,1)==-2);
  assert(io.events.back()!="dispatch"||io.active_frames==3);
  s.destroy_requested=1;
  assert(run_winmain_00402700(s,io,99,1,1)==0);
  assert(s.shutdown_complete&&s.quit_message&&s.sound_manager==0);
  assert(io.events.back()=="cdremoved");
 }
 {
  WindowHost io;WindowState s;s.display_active=1;
  io.input_failure=true;
  assert(run_winmain_00402700(s,io,99,1,3)==0);
  assert(s.startup_failed&&!s.initialized&&io.events.back()=="destroy-window");
 }
 {
  WindowHost io;WindowState s;s.display_active=1;
  assert(run_winmain_00402700(s,io,99,1,0)==-2);
  io.pending=true;io.quit_msg=true;
  assert(run_winmain_00402700(s,io,99,1,2)==12);
  assert(!s.shutdown_complete); // retail returns immediately on WM_QUIT
 }
 {
  FlowHost io;GameFlowState s;s.scene=0x15;
  s.unload_requested=1;s.ui_registered=true;s.activity_unload_installed=true;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Unloaded);
  assert(io.events.size()==3&&io.events[0]=="unloadui");
  s.unload_requested=0;s.overlay_options=1;s.overlay_progress=1;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Overlay);
  assert(io.events.back()=="options");
  s.overlay_options=0;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Overlay);
  assert(io.events.back()=="progress");
  s.overlay_progress=0;s.movie_phase=13;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Movie);
  io.bink=1;s.last_movie_scene=0x15;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Activity);
  assert(s.movie_phase==2&&s.scene==0x15);
  io.activity_result=-1;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Activity);
  assert(s.scene==4);
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Delegated);
 }
 {
  FlowHost io;GameFlowState s;s.scene=0x28;s.ui_registered=true;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Initialized);
  assert(s.scene==0x29&&s.activity_unload_installed);
  assert(io.events[0]=="unloadui"&&io.events[1]=="releaseui");
  assert(io.events[2]=="closemovie"&&io.events[3]=="loadcursor");
  assert(io.events[4]=="init");
  s.scene=0x2f;s.global_activity_flags=1;
  assert(run_main_game_flow_0042a2c0(s,io)==FlowResult::Activity);
  assert(io.events.back()!="help"&&s.overlay_leave==1);
 }
 {
  FloatHost io;
  auto a=parse_float_00438968("  -0.001230e+2!",'.',0,0,true,io);
  assert(a.valid&&a.status==0&&a.negative&&a.had_exponent);
  assert(a.consumed==13&&io.text=="123"&&io.exponent==-3);
  auto b=parse_float_00438968("123400",'.',0,0,true,io);
  assert(b.mantissa_digits=="1234"&&b.decimal_exponent==2);
  auto c=parse_float_00438968("+7,50D-2",',',0,0,true,io);
  assert(c.valid&&c.had_exponent&&c.decimal_exponent==-3);
  auto d=parse_float_00438968("notnumber",'.',0,0,true,io);
  assert(!d.valid&&d.status==4&&d.consumed==0);
  auto e=parse_float_00438968("6E6000",'.',0,0,true,io);
  assert(e.status==2&&e.value.sign_and_exponent==0x7fffU);
  auto f=parse_float_00438968("-9E-6000",'.',0,0,true,io);
  assert(f.status==1&&f.value.sign_and_exponent==0x8000U);
  auto g=parse_float_00438968("0.000",'.',0,0,true,io);
  assert(g.valid&&g.status==0&&g.value.mantissa_hi==0);
 }
 {
  FloatHost io;Ext80 zero{};
  auto a=format_x87_00438e39(zero,5,true,io);
  assert(a.digits=="0"&&a.decimal_position==0&&a.sign==' ');
  auto inf=format_x87_00438e39({0,0x80000000U,0x7fffU},5,true,io);
  assert(inf.digits=="1#INF"&&inf.status==0);
  auto ind=format_x87_00438e39({0,0xc0000000U,0xffffU},5,true,io);
  assert(ind.digits=="1#IND");
  auto qnan=format_x87_00438e39({1,0xc0000000U,0x7fffU},5,true,io);
  assert(qnan.digits=="1#QNAN");
  auto snan=format_x87_00438e39({1,0x80000000U,0x7fffU},5,true,io);
  assert(snan.digits=="1#SNAN");
  auto finite=format_x87_00438e39({0,0x80000000U,0x3fffU},5,true,io);
  assert(finite.digits=="12345"&&io.finite==1);
 }
}
