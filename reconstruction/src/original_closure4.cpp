#include "btb/original_closure4.hpp"
#include <algorithm>
#include <array>
#include <cctype>
#include <limits>
#include <stdexcept>
#include <utility>
namespace btb::original_closure4 {
namespace {
void release_window(WindowState& s,WindowIO& io,bool destroy_display){
 if(destroy_display)io.destroy_display_manager();
 io.post_quit_message();s.quit_message=true;
 io.release_original_input();
 io.shutdown_original_directsound();
 if(s.sound_manager){io.destroy_original_sound(s.sound_manager);s.sound_manager=0;}
 if(s.display_manager){io.destroy_original_display(s.display_manager);s.display_manager=0;}
 if(s.hook_installed){io.uninstall_original_hook(s.fullscreen_hook);s.hook_installed=false;}
 s.display_active=0;s.shutdown_complete=true;
}
bool game_activity(I32 scene,Activity& result){
 switch(scene){
 case 0x0f:result=Activity::Herding;return true;
 case 0x15:result=Activity::Dino;return true;
 case 0x1b:result=Activity::SpudSkate;return true;
 case 0x21:result=Activity::Maze;return true;
 case 0x25:result=Activity::Fireworks;return true;
 case 0x29:result=Activity::Squirrel;return true;
 case 0x2f:result=Activity::BobsBand;return true;
 case 0x33:result=Activity::ParkDesigner;return true;
 case 0x37:result=Activity::Golf;return true;
 case 0x3b:result=Activity::SpudMaze;return true;
 default:return false;
 }
}
bool init_activity(I32 scene,Activity& result){
 switch(scene){
 case 0x0e:result=Activity::Herding;return true;
 case 0x14:result=Activity::Dino;return true;
 case 0x1a:result=Activity::SpudSkate;return true;
 case 0x20:result=Activity::Maze;return true;
 case 0x24:result=Activity::Fireworks;return true;
 case 0x28:result=Activity::Squirrel;return true;
 case 0x2e:result=Activity::BobsBand;return true;
 case 0x32:result=Activity::ParkDesigner;return true;
 case 0x36:result=Activity::Golf;return true;
 case 0x3a:result=Activity::SpudMaze;return true;
 default:return false;
 }
}
bool ascii_space(char ch) {return ch==' '||ch=='\t'||ch=='\n'||ch=='\r';}
bool ascii_digit(char ch) {return ch>='0'&&ch<='9';}
}
I32 run_winmain_00402700(WindowState& s,WindowIO& io,U32 instance,I32 show,
                          U32 maximum_pump_iterations){
 if(!s.initialized&&!s.startup_failed){
  s.instance=instance;
  // WinMain 00402700 walks four registry subkeys and reads Install Path
  // into a 100-byte local before the original directory setup.
  const std::string path=io.registry_install_path(
   R"(Software\BBC Multimedia\Bob the Builder - Bob Builds a Park\1.0.0\Install Path)");
  if(path.size()>99)throw std::length_error("retail Install Path buffer is 100 bytes");
  io.change_original_working_directory(path);
  io.setup_runtime_and_help();
  io.disable_window_accessibility_keys();
  s.credits_state=0;
  s.initial_seed=io.original_first_seed();
  io.seed_original_prng(s.initial_seed);
  io.load_original_global_tables();
  io.seed_original_prng(io.tick_count());
  if(io.create_game_window(instance,show,s.window,s.accelerators)<0){
   s.startup_failed=true;return 0;
  }
  if(io.initialize_original_display(s.window)<0){
   s.startup_failed=true;return 0;
  }
  s.last_tick=io.tick_count();
  // Original operator_new(0x7738), SoundManagerConstructor and
  // DirectInput initialization. Keep allocation on the native boundary.
  s.sound_manager=io.create_original_sound_manager(0x7738U);
  if(io.initialize_directinput()<0){
   io.destroy_game_window(s.window);s.startup_failed=true;return 0;
  }
  io.initialize_original_bink();
  if(s.fullscreen_mode==0){
   s.fullscreen_hook=io.install_original_fullscreen_key_hook(instance);
   s.hook_installed=s.fullscreen_hook!=0;
  }
  s.initialized=true;
 }
 if(!s.initialized)return 0;
 for(U32 i=0;i<maximum_pump_iterations;++i){
  ++s.pump_iterations;
  // PeekMessage + GetMessage (vs unconditional GetMessage) is
  // significant; original runs game frames when message queue empties.
  while(io.peek_windows_message()){
   Message msg{};
   if(!io.get_windows_message(msg))return msg.wparam;
   if(!io.translate_accelerator(s.window,s.accelerators,msg))
    io.translate_and_dispatch(msg);
  }
  if(s.destroy_requested==1){
   release_window(s,io,false);
   io.original_message_cd_removed();
   return 0;
  }
  if(s.credits_state!=0){
   if(s.credits_state==1){
    io.load_original_credits();++s.credits_state;
   }
   if(s.credits_state==2)io.update_original_credits();
   if(s.credits_state>=3){
    io.run_original_game_flow(s.window);
    release_window(s,io,true);
    if(s.destroy_requested==1)io.original_message_cd_removed();
    io.wait_windows_message();s.last_tick=io.tick_count();
    continue;
   }
  }
  if(!s.display_active){
   io.wait_windows_message();
   s.last_tick=io.tick_count();
  }else {
   io.run_active_game_frame(s.window);
  }
 }
 return -2; // deterministic harness yield; not part of retail ABI
}
FlowResult run_main_game_flow_0042a2c0(GameFlowState& s,FlowIO& io){
 // Original DAT_0044DDB0 unload precedes the modal overlays.
 if(s.unload_requested){
  io.unload_original_generic_ui();
  if(s.ui_registered){io.release_original_ui_surface();s.ui_registered=false;}
  if(s.activity_unload_installed)io.unload_original_activity();
  return FlowResult::Unloaded;
 }
 if(s.overlay_options){io.pause_original_bink();io.update_options();return FlowResult::Overlay;}
 if(s.overlay_progress){io.update_progress();return FlowResult::Overlay;}
 if(s.overlay_yes_no){io.pause_original_bink();io.update_yes_no();return FlowResult::Overlay;}
 if(s.overlay_quit){io.pause_original_bink();io.update_quit();return FlowResult::Overlay;}
 if(s.overlay_leave){io.pause_original_bink();io.update_leave();return FlowResult::Overlay;}
 if(s.overlay_again){io.pause_original_bink();io.update_play_again();return FlowResult::Overlay;}
 if(s.legacy_delay>0){io.original_noop_hook();return FlowResult::Overlay;}
 // 0xD/0xE are the original global Bink hand-off phases (NOT menu states).
 if(s.movie_phase==0x0d||s.movie_phase==0x0e){
  if(!io.update_bink_movie())return FlowResult::Movie;
  if(s.movie_phase==0x0d){
   s.scene=s.last_movie_scene;
   s.movie_phase=2;
  }else{
   s.movie_phase=s.scene==0x22?7:2;
   ++s.scene;
  }
  io.restore_original_movie_scene(s.movie_phase);
 }
 if(s.scene==0){
  if(io.update_startup_video()){
   ++s.scene;s.intro_loaded=true;
  }
  return FlowResult::Start;
 }
 // The source calls Initialize{Activity} on the *preceding* scene.
 // Generic UI unload / cursor animation services are ordered before init
 // in source 0x20,24,28,2E,32,36,3A. 0x0E,14,1A also have movie steps.
 Activity kind{};
 if(init_activity(s.scene,kind)){
  s.active_activity_id=static_cast<I32>(kind);
  s.saved_activity=s.scene;
  if(s.scene!=0x0e&&s.scene!=0x14&&s.scene!=0x1a){
   io.unload_original_generic_ui();
   if(s.ui_registered){io.release_original_ui_surface();s.ui_registered=false;}
   io.close_original_walkthrough_movie();
   io.begin_loading_cursor_animation();
  }
  io.initialize_activity(kind);
  s.activity_unload_installed=true;
  ++s.scene;
  return FlowResult::Initialized;
 }
 if(game_activity(s.scene,kind)){
  s.saved_activity=s.scene;
  s.active_activity_id=static_cast<I32>(kind);
  const I32 activity_result=io.run_original_activity(kind);
  if(activity_result==-1)s.scene=4;
  io.update_generic_help_hover_voice();
  // Bob's Band original 0x2F only calls the help hover conditionally;
  // the remaining activity cases include standard help controller.
  if(kind!=Activity::BobsBand)io.update_help_button();
  if(s.global_activity_flags&1){
   s.overlay_leave=1;
   if(kind==Activity::ParkDesigner||kind==Activity::SpudMaze||
      kind==Activity::Maze||kind==Activity::Golf||kind==Activity::Squirrel)
    s.overlay_progress=0;
  }
  return FlowResult::Activity;
 }
 // Other screens/movies/asset reloads call concrete native providers
 // (source has 0x01..0x43). No synthetic branch equivalence is claimed.
 io.process_original_remaining_scene(s.scene,s);
 return FlowResult::Delegated;
}
ParsedFloat parse_float_00438968(std::string_view input,char sep,
                                  I32 scale_a,I32 scale_b,
                                  bool exponent_signs_enabled,FloatIO& io){
 ParsedFloat r{};
 std::size_t p=0;
 while(p<input.size()&&ascii_space(input[p]))++p;
 const auto original_start=p;
 if(p<input.size()&&(input[p]=='-'||input[p]=='+')){
  r.negative=input[p]=='-';++p;
 }
 bool had_digit=false,after_decimal=false,found_nonzero=false;
 I32 decimal_exp=0;
 std::string digits;digits.reserve(25);
 I32 dropped=0;
 while(p<input.size()){
  const char c=input[p];
  if(c==sep&&!after_decimal){after_decimal=true;++p;continue;}
  if(!ascii_digit(c))break;
  had_digit=true;
  if(c!='0')found_nonzero=true;
  if(found_nonzero){
   if(digits.size()<25)digits.push_back(c);
   else ++dropped;
  }else if(after_decimal)--decimal_exp;
  if(after_decimal&&found_nonzero)--decimal_exp;
  ++p;
 }
 if(!had_digit){r.consumed=original_start;r.status=4;return r;}
 // Original accepts D/d/E/e exponent tokens; incomplete exponents
 // rewind to before the E and leave the mantissa valid.
 const std::size_t exponent_start=p;
 I32 explicit_exp=0;
 if(p<input.size()&&(input[p]=='e'||input[p]=='E'||input[p]=='d'||input[p]=='D')){
  ++p;r.had_exponent=true;
  bool negexp=false;
  if(p<input.size()&&(input[p]=='-'||input[p]=='+')){
   if(!exponent_signs_enabled){
    p=exponent_start;r.had_exponent=false;
   }else{
    negexp=input[p]=='-';++p;
   }
  }
  if(r.had_exponent){
   if(p==input.size()||!ascii_digit(input[p])){
    p=exponent_start;r.had_exponent=false;
   }else{
    while(p<input.size()&&ascii_digit(input[p])){
     explicit_exp=std::min(0x1451,explicit_exp*10+(input[p]-'0'));++p;
    }
    if(negexp)explicit_exp=-explicit_exp;
   }
  }
 }
 r.consumed=p;
 if(!found_nonzero) {
  r.mantissa_digits="0";
  r.significant_digits=1;
  r.decimal_exponent=0;
  r.value.sign_and_exponent=r.negative?0x8000U:0;
  r.valid=true;
  return r;
 }
 // Retail stores 25 significant digits, rounds to 24, then trims
 // trailing zeros to reduce conversion work.
 if(digits.size()==25){
  if(digits[24]>'4'){
   I32 k=23;while(k>=0&&digits[static_cast<std::size_t>(k)]=='9')
    digits[static_cast<std::size_t>(k--)]='0';
   if(k>=0)++digits[static_cast<std::size_t>(k)];
   else {digits[0]='1';++decimal_exp;}
  }
  digits.resize(24);++dropped;
 }
 // Extra integer digits beyond the 25-digit source buffer raise
 // the decimal exponent; fractional drops do not.
 if(!after_decimal)decimal_exp+=dropped;
 while(digits.size()>1&&digits.back()=='0'){digits.pop_back();++decimal_exp;}
 const std::int64_t total=static_cast<std::int64_t>(decimal_exp)+explicit_exp+scale_a-scale_b;
 r.decimal_exponent=static_cast<I32>(std::clamp<std::int64_t>(total,-100000,100000));
 r.mantissa_digits=digits;r.significant_digits=static_cast<I32>(digits.size());
 r.valid=true;
 if(total>0x1450){
  r.status=2;r.value.sign_and_exponent=(r.negative?0x8000U:0)|0x7fffU;
  r.value.mantissa_hi=0x80000000U;return r;
 }
 if(total< -0x1450){
  r.status=1;r.value.sign_and_exponent=r.negative?0x8000U:0;return r;
 }
 r.value=io.pack_original_x87_decimal(digits,r.decimal_exponent,r.negative);
 return r;
}
FloatDigits format_x87_00438e39(const Ext80& x,I32 precision,
                                 bool count_fractional,FloatIO& io){
 if(precision<0||precision>512)throw std::out_of_range("original x87 digit precision");
 FloatDigits out{};
 out.sign=(x.sign_and_exponent&0x8000U)?'-':' ';
 const U32 exp=x.sign_and_exponent&0x7fffU;
 if(exp==0&&x.mantissa_hi==0&&x.mantissa_lo==0){
  out.decimal_position=0;out.sign=' ';out.digits="0";return out;
 }
 if(exp==0x7fffU){
  out.decimal_position=1;out.status=0;
  if(x.mantissa_hi==0x80000000U&&x.mantissa_lo==0){
   out.digits="1#INF";return out;
  }
  if(out.sign=='-'&&x.mantissa_hi==0xc0000000U&&x.mantissa_lo==0){
   out.digits="1#IND";return out;
  }
  if((x.mantissa_hi&0x40000000U)!=0 ){
   out.digits="1#QNAN";return out;
  }
  out.digits="1#SNAN";return out;
 }
 out.digits=io.original_finite_decimal_digits(x,precision,count_fractional,out.decimal_position);
 if(out.digits.empty())throw std::runtime_error("original x87 finite conversion returned empty");
 for(const char c:out.digits)if(!ascii_digit(c))
  throw std::runtime_error("original x87 finite digit provider not decimal");
 return out;
}
} // namespace btb::original_closure4
