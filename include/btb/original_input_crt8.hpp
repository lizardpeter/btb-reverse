#pragma once
// Eight PE32 original addresses: DirectInput, CRT startup, formatting,
// collation, SEH install and floating-point dispatch.
// Injected OS callbacks are required dependencies; this is not an x86 ABI shim.
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace btb::original_input_crt8 {
using U8=std::uint8_t;
using U32=std::uint32_t;
using I32=std::int32_t;
struct Rectangle { I32 left{},top{},right{},bottom{}; };
struct BitmapResource { U32 slot{},registration_slot{};const char* filename{};bool magenta_key{}; };
struct InputState {
 U32 input_interface{},mouse{},keyboard{},event_handle{},cursor{};
 U32 selected_cursor{},help_cursor{},click_sound{};
 U32 options_screen{},options_pointer{},quit_screen{};
 std::array<U8,256> keys{};
 U32 mouse_button_counter{},mouse_press_latch{},mouse_down{},click_pulse{};
 U32 translated_key{0xffffffffU};
 I32 translated_scan{-1};
 I32 x{320},y{0},min_x{},min_y{},max_x{640},max_y{480};
 I32 bounds_offset_x{},bounds_offset_y{};
 I32 sprite_width{19},sprite_height{19};
 U32 button_flags{},modifier_flags{},last_selected_scancode{0xffffffffU};
 bool initialized{},show_cursor{true},loading_cursor{},help_enabled{};
 bool armed_press{},keyboard_enabled{true},intro_capture{},click_sound_available{};
 bool use_mouse_events{true},voice_help_pending{},should_clip_cursor{};
 I32 synthetic_move{},repeat_counter{},capture_phase{},animation_frame{};
 float seconds{},seconds_per_ms{0.001f},reference_delta{},down_deadline{},up_deadline{};
 U32 global_draw_counter{};
 Rectangle primary_area{},secondary_area{};
};
class InputIO {
public:
 virtual ~InputIO()=default;
 // Original DirectInput8Create and IDirectInput8/IDirectInputDevice8 interfaces.
 virtual I32 create_direct_input(U32& interface_handle)=0;
 virtual I32 create_mouse(U32 input,U32& mouse)=0;
 virtual I32 set_mouse_format(U32 mouse)=0;
 virtual I32 set_mouse_cooperation(U32 mouse,U32 flags)=0;
 virtual U32 create_event()=0;
 virtual I32 set_mouse_notification(U32 mouse,U32 event_handle)=0;
 virtual I32 set_mouse_range(U32 mouse,U32 size,U32 property)=0;
 virtual void reset_mouse_bounds()=0;
 virtual U32 load_surface(std::string_view filename)=0;
 virtual void register_surface(U32 slot,U32 resource,std::string_view filename)=0;
 virtual void mark_keyed(U32 registration_slot)=0;
 virtual void set_magenta_key(U32 resource)=0;
 virtual void initialize_profiles()=0;
 virtual std::string_view sound_name(std::size_t ordinal)=0;
 virtual void load_sound(U32 destination,std::string_view path)=0;
 virtual void set_cursor_surface(U32 resource)=0;
 virtual I32 create_keyboard(U32 input,U32& keyboard)=0;
 virtual I32 set_keyboard_format(U32 keyboard)=0;
 virtual I32 set_keyboard_cooperation(U32 keyboard,U32 event_handle,U32 flags)=0;
 virtual void update_acquire_state()=0;
 // Original keyboard and mouse polling boundaries
 virtual U32 time_milliseconds()=0;
 virtual I32 poll_mouse(U32 mouse)=0;
 virtual I32 acquire_mouse(U32 mouse)=0;
 virtual I32 read_keyboard(U32 keyboard,std::array<U8,256>& keys)=0;
 struct MouseReading { I32 dx{},dy{},wheel{};U8 left{},right{}; };
 virtual I32 read_mouse(U32 mouse,MouseReading& reading)=0;
 virtual I32 translate_key(U32 scan_code,std::uint16_t& translated)=0;
 virtual bool click_sound_playing(U32 resource)=0;
 virtual void play_click_sound(U32 resource)=0;
 virtual void draw_loading_cursor()=0;
 virtual void draw_cursor(U32 bitmap,I32 x,I32 y,const Rectangle& source,bool direct)=0;
};
// 00407E50: exact resource order, COM failure short circuits, final keyboard.
[[nodiscard]] I32 initialize_direct_input(InputState&,InputIO&);
// 004084B0: input polling, keyboard event translation, button edges,
// cursor bounds, region state and present. No synthetic OS input.
void poll_direct_input(InputState&,InputIO&);

struct RuntimeState {
 U32 os_version{},major{},minor{},build{},packed_version{};
 std::string command_line;
 std::string runtime_command;
 U32 show_window{10},instance{};
 bool crt_initialized{},seh_installed{};
};
class StartupIO {
public:
 virtual ~StartupIO()=default;
 virtual U32 get_version()=0;
 virtual bool initialize_heap()=0;
 virtual void fatal_runtime(I32 code)=0;
 virtual void initialize_exceptions()=0;
 virtual std::string command_line()=0;
 virtual std::string prepare_command_line()=0;
 virtual void initialize_multibyte()=0;
 virtual void initialize_locale()=0;
 virtual void initialize_io()=0;
 virtual bool startup_show_window(U32& show)=0;
 virtual U32 instance_handle()=0;
 virtual I32 call_game_winmain(U32 instance,const std::string&,U32)=0;
 virtual void exit_runtime(I32)=0;
};
// 00430FEA: CRT startup before WinMain, not an actual platform entrypoint.
void original_crt_entry(RuntimeState&,StartupIO&);
struct SehFrame {U32 guest_frame{},previous{};};
class SehIO {
public:
 virtual ~SehIO()=default;
 virtual U32 push_guest_exception_frame(U32 guest_frame)=0;
};
// 0043A228: source-level FS:0 registration boundary.
void original_register_seh(SehFrame&,SehIO&);

enum class CompareMode : U8 { Unknown=0,Wide=1,Ansi=2 };
struct CompareState {CompareMode mode{CompareMode::Unknown};U32 active_code_page{1252};};
class CompareIO {
public:
 virtual ~CompareIO()=default;
 virtual I32 compare_wide(U32 locale,U32 flags,std::wstring_view a,std::wstring_view b)=0;
 virtual I32 compare_ansi(U32 locale,U32 flags,std::string_view a,std::string_view b)=0;
 virtual bool cp_lead_byte(U32 code_page,U8 byte)=0;
 virtual bool multibyte_to_wide(U32 code_page,std::string_view a,std::wstring& out)=0;
};
// 0043969D: CompareStringA/W capability probe and length/lead-byte handling.
[[nodiscard]] I32 compare_locale(CompareState&,CompareIO&,U32 locale,U32 flags,
                      std::string_view a,std::string_view b,U32 cp);

class FormatIO {
public:
 virtual ~FormatIO()=default;
 // Required recovered 00432971 printf engine. Writes to dest, returns the
 // number of characters and points 'end' to first unwritten char.
 virtual I32 printf_engine(std::span<char> dest,std::string_view format,
                           std::span<const U32> x86_arguments,std::size_t& end)=0;
};
// 00430144: CRT vformat wrapper, terminating NUL and return value.
[[nodiscard]] I32 original_sprintf(std::span<char>,std::string_view,
                                   std::span<const U32>,FormatIO&);

class FloatIO {
public:
 virtual ~FloatIO()=default;
 // Original provider 00437B3B + 00438E39; no host floating format guesswork.
 virtual void extended_parts(double value,U32& lo,U32& hi,U32& sign_exponent)=0;
 virtual U32 convert_80(U32 lo,U32 hi,U32 sign_exponent,I32 precision,
                         U8 flags,std::span<char> scratch,I32& decimal_point,
                         U8& sign)=0;
 virtual char* format_e(const double*,char*,I32,I32)=0;
 virtual char* format_f(const double*,char*,I32)=0;
 virtual char* format_g(const double*,char*,I32,I32)=0;
};
struct FloatContext {
 U32 converter_return{};
 I32 decimal_point{};
 U8 sign{};
 std::array<char,128> scratch{};
};
// 00437AD7: extended-precision decimal conversion and global context fill.
[[nodiscard]] FloatContext original_float_context(double value,FloatIO&);
// 004335B9: MSVC 2003 __cfltcvt E/e F/f and G/g dispatch. The Ghidra
// source incorrectly types the format selector as sizeInBytes.
[[nodiscard]] char* original_cfltcvt(const double*,char*,char format,
                                     I32 digits,I32 flags,FloatIO&);
} // namespace btb::original_input_crt8
