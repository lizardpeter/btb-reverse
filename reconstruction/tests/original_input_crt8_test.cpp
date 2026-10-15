#include "btb/original_input_crt8.hpp"
#include <cassert>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>
using namespace btb::original_input_crt8;
struct Device:InputIO {
 std::vector<std::string> calls;
 std::vector<U32> slots,marked;
 std::vector<std::string> sounds;
 std::array<U8,256> key_input{};
 MouseReading mouse_input{};
 U32 tick{1000},next_bitmap{600},cursor_draws{};
 I32 fail_at{-1},steps{},translated_attempts{},acquires{};
 bool mouse_down{},loading_drawn{},click_playing{};
 I32 gate(std::string what){calls.push_back(what);return ++steps==fail_at?-7:0;}
 I32 create_direct_input(U32& p) override {auto r=gate("create_input");if(r>=0)p=10;return r;}
 I32 create_mouse(U32,U32& p) override {auto r=gate("create_mouse");if(r>=0)p=20;return r;}
 I32 set_mouse_format(U32) override {return gate("mouse_format");}
 I32 set_mouse_cooperation(U32,U32 f) override {assert(f==5);return gate("mouse_coop");}
 U32 create_event() override {calls.push_back("event");return 30;}
 I32 set_mouse_notification(U32,U32 h) override {assert(h==30);return gate("mouse_notify");}
 I32 set_mouse_range(U32,U32 size,U32 property) override {
  assert(size==0x14 && property==1);return gate("mouse_range");}
 void reset_mouse_bounds() override {calls.push_back("reset_mouse_bounds");}
 U32 load_surface(std::string_view name) override {
  calls.push_back(std::string("load:")+std::string(name));return ++next_bitmap;}
 void register_surface(U32 slot,U32 surface,std::string_view) override {
  assert(surface>600);slots.push_back(slot);calls.push_back("register");}
 void mark_keyed(U32 slot) override {marked.push_back(slot);calls.push_back("mark_key");}
 void set_magenta_key(U32) override {calls.push_back("set_key");}
 void initialize_profiles() override {calls.push_back("profiles");}
 std::string_view sound_name(std::size_t i) override {
  static constexpr const char* names[]{"one.wav","","three.wav","","","six.wav"};
  return names[i];}
 void load_sound(U32,std::string_view name) override {
  sounds.emplace_back(name);calls.push_back("sound");}
 void set_cursor_surface(U32 which) override {assert(which==0);calls.push_back("cursor_zero");}
 I32 create_keyboard(U32,U32& p) override {auto r=gate("create_keyboard");if(r>=0)p=40;return r;}
 I32 set_keyboard_format(U32) override {return gate("keyboard_format");}
 I32 set_keyboard_cooperation(U32,U32 h,U32 flags) override {
  assert(h==30&&flags==0x16);return gate("keyboard_coop");}
 void update_acquire_state() override {calls.push_back("acquire_state");}
 U32 time_milliseconds() override {return tick++;}
 I32 poll_mouse(U32) override{return gate("poll_mouse");}
 I32 acquire_mouse(U32) override {++acquires;return 0;}
 I32 read_keyboard(U32,std::array<U8,256>& keys) override {
  keys=key_input;calls.push_back("read_keyboard");return 0;}
 I32 read_mouse(U32,MouseReading& reading) override {
  reading=mouse_input;calls.push_back("read_mouse");return 0;}
 I32 translate_key(U32 scan,std::uint16_t& code) override {
  ++translated_attempts;code=static_cast<std::uint16_t>(scan==0x1e?'A':0xcccc);
  return scan==0x1e?1:0;
 }
 bool click_sound_playing(U32) override {return click_playing;}
 void play_click_sound(U32) override {calls.push_back("click");}
 void draw_loading_cursor() override {loading_drawn=true;}
 void draw_cursor(U32,I32 x,I32 y,const Rectangle& r,bool direct) override {
  assert(direct&&x>=0&&y>=0&&r.right==19&&r.bottom==19);++cursor_draws;
 }
};
struct Start:StartupIO {
 std::vector<std::string> calls;
 U32 show{7};
 void step(std::string s){calls.push_back(std::move(s));}
 U32 get_version() override {step("version");return 0x12340506U;}
 bool initialize_heap() override {step("heap");return true;}
 void fatal_runtime(I32) override {assert(false);}
 void initialize_exceptions() override {step("exceptions");}
 std::string command_line() override {step("cmd");return "full command";}
 std::string prepare_command_line() override {step("prepare");return "game.exe /test";}
 void initialize_multibyte() override {step("multibyte");}
 void initialize_locale() override {step("locale");}
 void initialize_io() override {step("io");}
 bool startup_show_window(U32& s) override {step("show");s=show;return true;}
 U32 instance_handle() override {step("instance");return 123;}
 I32 call_game_winmain(U32 inst,const std::string& str,U32 flag) override {
  assert(inst==123&&str=="game.exe /test"&&flag==7);
  step("WinMain");return 17;}
 void exit_runtime(I32 code) override {assert(code==17);step("exit");}
};
struct Seh:SehIO {U32 push_guest_exception_frame(U32 address) override{
 assert(address==0x70000);return 0x60000;}
};
struct Collation:CompareIO {
 bool wide_supported{true},narrow_supported{true},fail_conversion{};
 int wide_calls{},ansi_calls{},conversions{};
 I32 compare_wide(U32 l,U32 f,std::wstring_view a,std::wstring_view b) override{
  ++wide_calls;
  if(!l&&!f&&a.empty()&&b.empty())return wide_supported?2:0;
  return a==b?2:(a<b?1:3);
 }
 I32 compare_ansi(U32 l,U32 f,std::string_view a,std::string_view b) override {
  ++ansi_calls;
  if(!l&&!f&&a.empty()&&b.empty())return narrow_supported?2:0;
  return a==b?2:(a<b?1:3);
 }
 bool cp_lead_byte(U32,U8 c) override{return c>=0x81;}
 bool multibyte_to_wide(U32 cp,std::string_view in,std::wstring& out) override{
  ++conversions;assert(cp==1252||cp==932);
  if(fail_conversion)return false;
  out.clear();for(unsigned char c:in)out.push_back(c);return true;
 }
};
struct PrintFormat:FormatIO{
 I32 printf_engine(std::span<char> dst,std::string_view fmt,
                   std::span<const U32> args,std::size_t& used) override{
  assert(fmt=="X%u"&&args.size()==1&&args[0]==23);
  const char result[]="X23";
  if(dst.size()<4){used=3;return 3;}
  std::memcpy(dst.data(),result,3);used=3;return 3;
 }
};
struct Float:FloatIO{
 std::string invoked;std::array<U32,3> parts{};
 void extended_parts(double val,U32& lo,U32& hi,U32& se) override{
  assert(val==12.5);lo=1;hi=2;se=0x4000;invoked="parts";}
 U32 convert_80(U32 lo,U32 hi,U32 se,I32 precision,U8 flags,
                std::span<char> scratch,I32& decimal,U8& sign) override{
  assert(lo==1&&hi==2&&se==0x4000&&precision==17&&!flags);
  scratch[0]='1';scratch[1]='2';scratch[2]=0;
  decimal=2;sign=0;invoked="convert";return 1;
 }
 char* format_e(const double*,char* buffer,I32,I32) override{
  invoked="E";std::memcpy(buffer,"e10",4);return buffer;}
 char* format_f(const double*,char* buffer,I32) override{
  invoked="F";std::memcpy(buffer,"f10",4);return buffer;}
 char* format_g(const double*,char* buffer,I32,I32) override{
  invoked="G";std::memcpy(buffer,"g10",4);return buffer;}
};
int main(){
 // High-yield live input initialization: COM/resource ordering and early failure.
 Device d;InputState s;
 assert(initialize_direct_input(s,d)==0 && s.initialized);
 assert(s.mouse==20 && s.keyboard==40 && s.event_handle==30);
 assert(d.slots.size()==15&&d.marked.size()==14&&d.sounds.size()==5);
 assert(d.slots.front()==0x004fbf88&&d.slots.back()==0x00519944);
 const auto pos=[&](std::string name){for(std::size_t i=0;i<d.calls.size();++i)
  if(d.calls[i]==name)return i;return d.calls.size();};
 assert(pos("profiles")<pos("create_keyboard") && pos("cursor_zero")<pos("create_keyboard"));
 Device failure;InputState stopped;failure.fail_at=3;
 assert(initialize_direct_input(stopped,failure)==-7);
 assert(!stopped.initialized&&stopped.keyboard==0&&failure.slots.empty());
 Device e;InputState no_event;e.fail_at=-1;
 // Input polling uses the actual injected mouse and keyboard state.
 s.selected_cursor=s.cursor;s.primary_area={200,-20,450,100};
 s.secondary_area={500,200,600,280};s.mouse_button_counter=0;
 d.key_input[0x1e]=0x80;
 d.mouse_input.dx=10;d.mouse_input.dy=5;d.mouse_input.left=0x80;
 poll_direct_input(s,d);
 assert(s.mouse_down==1&&s.x==330&&s.y==5);
 assert(s.translated_key=='A'&&d.cursor_draws==1);
 assert(d.cursor_draws==1&&s.click_pulse==0);
 d.mouse_input={};
 poll_direct_input(s,d);
 assert(s.mouse_down==0&&s.click_pulse==1&&(s.button_flags&2));
 assert(s.translated_key==0xffffffffU && d.cursor_draws==2);
 assert(s.global_draw_counter==2);
 d.mouse_input.dx=100000;d.mouse_input.dy=100000;
 poll_direct_input(s,d);
 assert(s.x<=621&&s.y<=461);
 // Source early return on poll failure should not advance cursor draw counter.
 d.fail_at=d.steps+1;const auto before=d.cursor_draws;
 poll_direct_input(s,d);
 assert(d.cursor_draws==before);
 // Runtime entry dispatch preserves order and Windows version unpack.
 RuntimeState st;Start boot;original_crt_entry(st,boot);
 assert(st.major==6&&st.minor==5&&st.packed_version==0x605);
 assert(st.build==0x1234&&st.show_window==7&&st.crt_initialized);
 assert(boot.calls.back()=="exit"&&boot.calls[boot.calls.size()-2]=="WinMain");
 // Ghidra 0043A228 installs the new SEH frame, returns old chain head.
 SehFrame frame{0x70000,0};Seh seh;original_register_seh(frame,seh);
 assert(frame.previous==0x60000);
 // Host collation capability is probed once, and ANSI fallback is distinct.
 CompareState col;Collation ci;
 assert(compare_locale(col,ci,1,0,"cat","dog",1252)==1);
 assert(col.mode==CompareMode::Wide&&ci.conversions==2);
 assert(compare_locale(col,ci,1,0,"cat","cat",1252)==2);
 assert(ci.wide_calls==3);
 CompareState ansi;Collation old;old.wide_supported=false;
 assert(compare_locale(ansi,old,1,0,"A","B",1252)==1);
 assert(ansi.mode==CompareMode::Ansi&&old.ansi_calls==2);
 CompareState broken;Collation bad;bad.wide_supported=false;bad.narrow_supported=false;
 assert(compare_locale(broken,bad,1,0,"a","b",1252)==0);
 assert(broken.mode==CompareMode::Unknown);
 CompareState failure_locale;Collation bad_conversion;bad_conversion.fail_conversion=true;
 assert(compare_locale(failure_locale,bad_conversion,1,0,"a","b",1252)==0);
 // sprintf dispatch preserves printf_engine's return and NUL termination.
 PrintFormat printf_io;char buffer[16]{};
 assert(original_sprintf(buffer,"X%u",std::array<U32,1>{23},printf_io)==3);
 assert(std::string(buffer)=="X23");
 bool short_buffer=false;char little[3]{};
 try{(void)original_sprintf(little,"X%u",std::array<U32,1>{23},printf_io);}
 catch(const std::out_of_range&){short_buffer=true;}
 assert(short_buffer);
 // 00437AD7 float context and 004335B9 formatting routing.
 Float fl;const auto context=original_float_context(12.5,fl);
 assert(context.converter_return==1&&context.decimal_point==2&&!context.sign);
 assert(context.scratch[0]=='1'&&fl.invoked=="convert");
 double val=12.5;char value[20]{};
 assert(original_cfltcvt(&val,value,'E',4,1,fl)==value&&fl.invoked=="E");
 assert(original_cfltcvt(&val,value,'f',4,1,fl)==value&&fl.invoked=="F");
 assert(original_cfltcvt(&val,value,'g',4,1,fl)==value&&fl.invoked=="G");
}
