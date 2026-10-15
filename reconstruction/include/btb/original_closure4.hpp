#pragma once
// Original PE32 source-model entrypoints:
// 00402700 WinMain, 0042A2C0 RunMainGameFlow,
// 00438968 CRT numeric text->x87, 00438E39 CRT x87->decimal.
// Source-derived orchestration; NOT retail binary/behavior equivalence.
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>
namespace btb::original_closure4 {
using I32=std::int32_t;using U32=std::uint32_t;
struct Message {I32 kind{},wparam{},lparam{};};
struct WindowState {
 U32 instance{},window{},accelerators{},sound_manager{},display_manager{},
     fullscreen_hook{},last_tick{},initial_seed{};
 I32 credits_state{},display_active{},destroy_requested{},time_slice{},pump_iterations{};
 bool initialized{},hook_installed{},startup_failed{},shutdown_complete{},quit_message{};
};
class WindowIO {
public:
 virtual ~WindowIO()=default;
 virtual std::string registry_install_path(std::string_view const_subkeys)=0;
 virtual void change_original_working_directory(std::string_view)=0;
 virtual void setup_runtime_and_help()=0;
 virtual void disable_window_accessibility_keys()=0;
 virtual U32 original_first_seed()=0;
 virtual U32 tick_count()=0;
 virtual void seed_original_prng(U32)=0;
 virtual void load_original_global_tables()=0;
 virtual I32 create_game_window(U32 instance,I32 show,U32& window,U32& accelerators)=0;
 virtual I32 initialize_original_display(U32 window)=0;
 virtual U32 create_original_sound_manager(U32 original_bytes)=0;
 virtual I32 initialize_directinput()=0;
 virtual void initialize_original_bink()=0;
 virtual U32 install_original_fullscreen_key_hook(U32 instance)=0;
 virtual bool peek_windows_message()=0;
 virtual bool get_windows_message(Message&)=0;
 virtual bool translate_accelerator(U32 window,U32 accelerators,const Message&)=0;
 virtual void translate_and_dispatch(const Message&)=0;
 virtual void load_original_credits()=0;
 virtual void update_original_credits()=0;
 virtual void run_active_game_frame(U32 window)=0;
 virtual void run_original_game_flow(U32 window)=0;
 virtual void destroy_display_manager()=0;
 virtual void release_original_input()=0;
 virtual void shutdown_original_directsound()=0;
 virtual void destroy_original_sound(U32 handle)=0;
 virtual void destroy_original_display(U32 handle)=0;
 virtual void uninstall_original_hook(U32 hook)=0;
 virtual void post_quit_message()=0;
 virtual void wait_windows_message()=0;
 virtual void original_message_cd_removed()=0;
 virtual void destroy_game_window(U32 window)=0;
};
// Original WinMain startup/real message dispatch/credits/teardown, with
// bounded frame-pump budget for deterministic validation without a live GUI.
I32 run_winmain_00402700(WindowState&,WindowIO&,U32 instance,I32 show,
                          U32 maximum_pump_iterations);
struct GameFlowState {
 I32 scene{},movie_phase{},last_movie_scene{},movie_result{},overlay_options{},
     overlay_progress{},overlay_yes_no{},overlay_quit{},overlay_leave{},
     overlay_again{},legacy_delay{},unload_requested{},credits_pending{},
     active_difficulty{},active_activity_id{-1},global_activity_flags{},
     saved_activity{},generic_ui_state{};
 bool intro_loaded{},ui_registered{},activity_unload_installed{},
      stop_requested{},initialized{};
};
enum class Activity : I32 {
 Herding=0x0f,Dino=0x15,SpudSkate=0x1b,Maze=0x21,Fireworks=0x25,
 Squirrel=0x29,BobsBand=0x2f,ParkDesigner=0x33,Golf=0x37,SpudMaze=0x3b
};
class FlowIO {
public:
 virtual ~FlowIO()=default;
 virtual void unload_original_generic_ui()=0;
 virtual void release_original_ui_surface()=0;
 virtual void unload_original_activity()=0;
 virtual void pause_original_bink()=0;
 virtual void update_options()=0;
 virtual void update_progress()=0;
 virtual void update_yes_no()=0;
 virtual void update_quit()=0;
 virtual void update_leave()=0;
 virtual void update_play_again()=0;
 virtual void original_noop_hook()=0;
 virtual I32 update_bink_movie()=0;
 virtual void restore_original_movie_scene(I32 phase)=0;
 virtual I32 update_startup_video()=0;
 virtual I32 run_original_activity(Activity kind)=0;
 virtual void update_generic_help_hover_voice()=0;
 virtual void update_help_button()=0;
 virtual void initialize_activity(Activity kind)=0;
 virtual void begin_loading_cursor_animation()=0;
 virtual void close_original_walkthrough_movie()=0;
 // Explicitly retain the original shared-game branches that are not yet
 // decoded into this source model; callbacks must call real native providers.
 virtual void process_original_remaining_scene(I32 scene,GameFlowState&)=0;
};
enum class FlowResult {None,Overlay,Unloaded,Movie,Start,Activity,Initialized,Delegated};
FlowResult run_main_game_flow_0042a2c0(GameFlowState&,FlowIO&);

struct Ext80 {U32 mantissa_lo{},mantissa_hi{};std::uint16_t sign_and_exponent{};};
struct ParsedFloat {
 Ext80 value{};
 std::size_t consumed{};
 I32 decimal_exponent{},significant_digits{};
 I32 status{}; // source 0=ordinary,1=underflow,2=overflow,4=invalid
 bool negative{},valid{},had_exponent{};
 std::string mantissa_digits{};
};
class FloatIO {
public:
 virtual ~FloatIO()=default;
 virtual Ext80 pack_original_x87_decimal(std::string_view digits,
                                           I32 decimal_exp,bool negative)=0;
 virtual std::string original_finite_decimal_digits(const Ext80&,I32 requested_precision,
                                                     bool count_fractional,
                                                     I32& decimal_position)=0;
};
// Original FUN_00438968 parses signed floating literals including original
// D/d/E/e and locale separators, returning packed 80-bit x87 via provider.
ParsedFloat parse_float_00438968(std::string_view,char decimal_separator,
                                  I32 original_scale_a,I32 original_scale_b,
                                  bool exponent_signs_enabled,FloatIO&);
// Original FUN_00438E39 recognizes ±INF, QNAN, SNAN, IND and zero, then
// uses recovered native 80-bit converter for finite digits.
struct FloatDigits {I32 decimal_position{};char sign{' '};std::string digits{"0"};I32 status{1};};
FloatDigits format_x87_00438e39(const Ext80&,I32 requested_precision,
                                 bool count_fractional,FloatIO&);
} // namespace btb::original_closure4
