#include "btb/original_input_crt8.hpp"
#include <algorithm>
#include <bit>
#include <limits>
#include <stdexcept>
#include <utility>

namespace btb::original_input_crt8 {
namespace {
constexpr U32 kMagenta=0xff00ffU;
constexpr I32 kFailure=std::bit_cast<I32>(0x80004005U);
struct Resource {U32 slot;const char* name;bool keyed;};
constexpr std::array<Resource,15> kBitmaps{{
 {0x004fbf88,R"(data\ui\cursor.bmp)",true},
 {0x0051c288,R"(data\ui\options\optionsscreen.bmp)",true},
 {0x0051c28c,R"(data\ui\options\pointer.bmp)",true},
 {0x0051c290,R"(data\ui\main\quitmain.bmp)",true},
 {0x0051c2a0,R"(data\ui\playagain\payessel.bmp)",true},
 {0x0051c2a4,R"(data\ui\playagain\panosel.bmp)",true},
 {0x0051c2a8,R"(data\ui\playagain\payesdep.bmp)",true},
 {0x0051c2ac,R"(data\ui\playagain\panodep.bmp)",true},
 {0x0051c298,R"(data\ui\main\quitmain.bmp)",false},
 {0x00519954,R"(data\ui\textcursor.bmp)",true},
 {0x0051be40,R"(data\ui\textcursor.bmp)",true},
 {0x0051be44,R"(data\ui\textcursor.bmp)",true},
 {0x0051be34,R"(data\ui\textcursor.bmp)",true},
 {0x0051be38,R"(data\ui\textcursor.bmp)",true},
 {0x00519944,R"(data\ui\help_cursor.bmp)",true},
}};
void load_bitmap(InputIO& io,InputState& s,const Resource& desc){
 const U32 handle=io.load_surface(desc.name);
 io.register_surface(desc.slot,handle,desc.name);
 if(desc.keyed){io.mark_keyed(desc.slot);io.set_magenta_key(handle);}
 if(desc.slot==0x004fbf88)s.cursor=handle;
 if(desc.slot==0x00519944)s.help_cursor=handle;
 if(desc.slot==0x0051c288)s.options_screen=handle;
 if(desc.slot==0x0051c28c)s.options_pointer=handle;
 if(desc.slot==0x0051c290)s.quit_screen=handle;
}
bool in_rect(Rectangle r,I32 x,I32 y) {
 return x>r.left&&x<r.right&&y>r.top&&y<r.bottom;
}
void update_modifier_flags(InputState& s){
 // Ghidra references byte indices 0xcb,0xcd,0xc8,0xd0 and 0x39
 // relative to 004FBE70; these are extended keyboard scancodes.
 s.modifier_flags=((s.keys[0xcb]&0x80)?1U:0U)
  |((s.keys[0xcd]&0x80)?2U:0U)
  |((s.keys[0xc8]&0x80)?4U:0U)
  |((s.keys[0xd0]&0x80)?8U:0U)
  |((s.keys[0x39]&0x80)?16U:0U);
}
void clamp_cursor(InputState& s){
 const auto min_x=static_cast<std::int64_t>(s.min_x)-s.bounds_offset_x;
 const auto min_y=static_cast<std::int64_t>(s.min_y)-s.bounds_offset_y;
 const auto max_x=static_cast<std::int64_t>(s.max_x)-s.bounds_offset_x;
 const auto max_y=static_cast<std::int64_t>(s.max_y)-s.bounds_offset_y;
 if(min_x>max_x||min_y>max_y)throw std::invalid_argument("invalid original input bounds");
 s.x=static_cast<I32>(std::clamp<std::int64_t>(s.x,min_x,max_x));
 s.y=static_cast<I32>(std::clamp<std::int64_t>(s.y,min_y,max_y));
}
}
I32 initialize_direct_input(InputState& s,InputIO& io){
 I32 hr=io.create_direct_input(s.input_interface);
 if(hr<0)return hr;
 hr=io.create_mouse(s.input_interface,s.mouse);
 if(hr<0)return hr;
 hr=io.set_mouse_format(s.mouse);
 if(hr<0)return hr;
 hr=io.set_mouse_cooperation(s.mouse,5);
 if(hr<0)return hr;
 s.event_handle=io.create_event();
 if(!s.event_handle)return kFailure;
 hr=io.set_mouse_notification(s.mouse,s.event_handle);
 if(hr<0)return hr;
 hr=io.set_mouse_range(s.mouse,0x14,1);
 if(hr<0)return hr;
 io.reset_mouse_bounds();
 s.x=0x140;s.y=0;s.armed_press=false;s.mouse_down=0;
 load_bitmap(io,s,kBitmaps[0]);
 io.initialize_profiles();
 // Six 128-byte sound table rows at 0051624C..0051654B.
 for(U32 i=0;i<6;++i){
  const auto name=io.sound_name(i);
  if(!name.empty())io.load_sound(0x005168c4U+i*4U,name);
 }
 for(std::size_t i=1;i<kBitmaps.size();++i)load_bitmap(io,s,kBitmaps[i]);
 io.load_sound(0x004fbf8c,R"(Data\ui\click.wav)");
 io.load_sound(0x0051c2b8,R"(data\sound\sting_1_out.wav)");
 s.click_sound=0x004fbf8c;s.click_sound_available=true;
 io.set_cursor_surface(0);
 hr=io.create_keyboard(s.input_interface,s.keyboard);
 if(hr<0)return hr;
 hr=io.set_keyboard_format(s.keyboard);
 if(hr<0)return hr;
 hr=io.set_keyboard_cooperation(s.keyboard,s.event_handle,0x16);
 if(hr<0)return hr;
 io.update_acquire_state();
 s.initialized=true;return 0;
}
void poll_direct_input(InputState& s,InputIO& io){
 const U32 tick=io.time_milliseconds();
 s.seconds=static_cast<float>(tick)*s.seconds_per_ms;
 bool has_mouse=s.mouse!=0;
 InputIO::MouseReading mouse{};
 if(has_mouse){
  if(io.poll_mouse(s.mouse)<0)return;
  (void)io.read_keyboard(s.keyboard,s.keys);
  const I32 result=io.read_mouse(s.mouse,mouse);
  if(result==std::bit_cast<I32>(0x8007001eU)){
   if(io.acquire_mouse(s.mouse)>=0)return;
  }
  if(s.synthetic_move==0&&s.armed_press){
   s.synthetic_move=1;
  }
  if(s.synthetic_move!=0){
   ++s.repeat_counter;
   // Retail increments every other frame while input latch is set.
   if((s.repeat_counter&1)==0)mouse.left|=0x80;
  }
  std::uint16_t translated=0xcccc;
  U32 selected=0xffffffffU;
  for(U32 code=0;code<255;++code){
   if(code==0x2a||code==0x36||!(s.keys[code]&0x80))continue;
   if(io.translate_key(code,translated)>0){selected=code;break;}
  }
  const auto word=static_cast<std::int16_t>(translated);
  if(word==static_cast<std::int16_t>(s.translated_scan)||word==static_cast<std::int16_t>(0xcccc)){
   s.translated_scan=-1;s.translated_key=0xffffffffU;
  }else {
   s.translated_scan=word;s.translated_key=static_cast<U32>(translated&0xff);
  }
  s.last_selected_scancode=selected;
  update_modifier_flags(s);
  if(mouse.wheel<0){s.down_deadline=s.seconds+s.reference_delta;s.up_deadline=0.0f;}
  if(mouse.wheel>0){s.up_deadline=s.seconds+s.reference_delta;s.down_deadline=0.0f;}
  if(s.loading_cursor){io.draw_loading_cursor();return;}
  s.button_flags=0;
  const bool first=in_rect(s.primary_area,s.x,s.y)&&s.selected_cursor==s.cursor;
  const bool second=in_rect(s.secondary_area,s.x,s.y)&&s.selected_cursor==s.cursor;
  // Hover flag 8 for the profile-screen variant, otherwise for generic rect.
  if(first)s.button_flags|=8;
  if(second)s.button_flags|=4;
  if(s.click_pulse){s.click_pulse=0;s.armed_press=false;}
  const bool down=(mouse.left&0x80)!=0||(mouse.right&0x80)!=0;
  if(!down){
   s.mouse_down=0;s.armed_press=false;
   if(s.mouse_press_latch){
    s.click_pulse=1;
    if(first)s.button_flags|=2;
    if(second)s.button_flags|=1;
   }
   s.mouse_press_latch=0;
  }else{
   s.mouse_down=1;s.mouse_press_latch=1;
   if(!s.armed_press){
    if((first||second)&&s.capture_phase==12){
     s.button_flags|=first?2U:1U;
    }
    s.armed_press=true;
    if(s.click_sound_available&&!io.click_sound_playing(s.click_sound))
     io.play_click_sound(s.click_sound);
   }
  }
  s.synthetic_move=0;
  s.x+=mouse.dx;s.y+=mouse.dy;
 }
 if(s.voice_help_pending){
  const I32 step=s.intro_capture?4:1;
  if(s.modifier_flags&1)s.x-=step;
  if(s.modifier_flags&2)s.x+=step;
  if(s.modifier_flags&4)s.y-=step;
  if(s.modifier_flags&8)s.y+=step;
 }
 if(s.modifier_flags&16){
  if(!s.keyboard_enabled){
   s.keyboard_enabled=true;s.click_pulse=1;s.armed_press=true;
  }
 }else s.keyboard_enabled=false;
 clamp_cursor(s);
 if(s.show_cursor&&s.initialized){
  const bool default_cursor=s.selected_cursor==s.cursor;
  if(default_cursor){
   s.x=std::min(s.x,621);s.y=std::min(s.y,461);
  }else{
   s.y=std::min(s.y,461);
  }
  const Rectangle source{0,0,default_cursor?19:s.sprite_width,
    default_cursor?19:s.sprite_height};
  io.draw_cursor(s.selected_cursor,s.x,s.y,source,
                 default_cursor||(!s.bounds_offset_x&&!s.bounds_offset_y));
 }
 s.global_draw_counter=(s.global_draw_counter+1)%32;
}
void original_crt_entry(RuntimeState& s,StartupIO& io){
 const U32 value=io.get_version();
 s.os_version=value;
 s.major=value&0xff;s.minor=(value>>8)&0xff;
 s.packed_version=(s.major<<8)|s.minor;
 s.build=value>>16;
 if(!io.initialize_heap())io.fatal_runtime(0x1c);
 io.initialize_exceptions();s.seh_installed=true;
 s.command_line=io.command_line();
 s.runtime_command=io.prepare_command_line();
 io.initialize_multibyte();io.initialize_locale();io.initialize_io();
 U32 requested=10;
 if(io.startup_show_window(requested))s.show_window=requested;
 else s.show_window=10;
 s.instance=io.instance_handle();s.crt_initialized=true;
 const I32 result=io.call_game_winmain(s.instance,s.runtime_command,s.show_window);
 io.exit_runtime(result);
}
void original_register_seh(SehFrame& s,SehIO& io){
 s.previous=io.push_guest_exception_frame(s.guest_frame);
}
I32 compare_locale(CompareState& s,CompareIO& io,U32 locale,U32 flags,
                   std::string_view a,std::string_view b,U32 cp){
 if(s.mode==CompareMode::Unknown){
  const auto wide_probe=io.compare_wide(0,0,L"",L"");
  if(wide_probe!=0)s.mode=CompareMode::Wide;
  else{
   const auto ansi_probe=io.compare_ansi(0,0,"","");
   if(ansi_probe==0)return 0;
   s.mode=CompareMode::Ansi;
  }
 }
 // Retail strips a trailing NUL (via 0043991A) for explicit-length calls.
 if(!a.empty()&&a.back()==0)a.remove_suffix(1);
 if(!b.empty()&&b.back()==0)b.remove_suffix(1);
 if(s.mode==CompareMode::Ansi)
  return io.compare_ansi(locale,flags,a,b);
 if(cp==0)cp=s.active_code_page;
 if(a.empty()||b.empty()){
  if(a.size()==b.size())return 2;
  if(b.size()>1)return 1;
  if(a.size()>1)return 3;
  const bool a_lead=!a.empty()&&io.cp_lead_byte(cp,static_cast<U8>(a.front()));
  const bool b_lead=!b.empty()&&io.cp_lead_byte(cp,static_cast<U8>(b.front()));
  if(a_lead||b_lead)return 2;
  return a.empty()?1:3;
 }
 std::wstring aw,bw;
 if(!io.multibyte_to_wide(cp,a,aw))return 0;
 if(!io.multibyte_to_wide(cp,b,bw))return 0;
 return io.compare_wide(locale,flags,aw,bw);
}
I32 original_sprintf(std::span<char> dst,std::string_view format,
                     std::span<const U32> args,FormatIO& io){
 // MSVC2003 supplies an unbounded count 0x7fffffff and a 0x42 output flag.
 // This port requires a bounded host span instead of corrupting host memory.
 std::size_t position=0;
 const auto result=io.printf_engine(dst,format,args,position);
 if(position>=dst.size())throw std::out_of_range("original sprintf destination");
 dst[position]=0;
 return result;
}
FloatContext original_float_context(double value,FloatIO& io){
 FloatContext state{};
 U32 low=0,high=0,se=0;
 io.extended_parts(value,low,high,se);
 state.converter_return=io.convert_80(low,high,se,0x11,0,
       std::span<char>(state.scratch),state.decimal_point,state.sign);
 return state;
}
char* original_cfltcvt(const double* input,char* buffer,char format,
                       I32 digits,I32 flags,FloatIO& io){
 if(!input||!buffer)throw std::invalid_argument("null cfltcvt argument");
 if(format=='e'||format=='E')return io.format_e(input,buffer,digits,flags);
 if(format=='f')return io.format_f(input,buffer,digits);
 return io.format_g(input,buffer,digits,flags);
}
} // namespace btb::original_input_crt8
