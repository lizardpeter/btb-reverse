#include "btb/win32_original_main_window_proc.hpp"
#if defined(_WIN32)
#include "btb/win32_recompiled_graphics.hpp"
#include <mmsystem.h>
namespace btb::original_win32 {
std::optional<LRESULT> main_window_proc_recompiled(
 HWND hwnd,UINT msg,WPARAM wp,LPARAM lp,RetailWindowState& state,
 full_game::Win32OriginalDirectDraw& graphics,std::string& error,
 WindowServices services){
 if(msg==WM_SYSKEYDOWN)return LRESULT{0};
 switch(msg){
 case WM_DESTROY:
  state.outer_state=3;return std::nullopt;
 case WM_MOVE:
  if(graphics.initialized())(void)update_display_destination_rect(
     hwnd,state.windowed,state.destination);
  return LRESULT{0};
 case WM_SIZE:
  state.accepting_input=(wp!=SIZE_MINIMIZED&&wp!=SIZE_MAXHIDE);
  if(graphics.initialized())(void)update_display_destination_rect(
     hwnd,state.windowed,state.destination);
  return std::nullopt; // original falls through to DefWindowProc
 case WM_ACTIVATE:
  if(wp!=0&&services.acquire_input)services.acquire_input(services.context);
  return std::nullopt;
 case WM_SETCURSOR:
  if(!state.windowed){(void)SetCursor(nullptr);return LRESULT{1};}
  return std::nullopt;
 case WM_GETMINMAXINFO:{
  if(!lp)return LRESULT{0}; // host-safe missing MINMAXINFO pointer
  auto& limits=*reinterpret_cast<MINMAXINFO*>(lp);
  const LONG width=640+GetSystemMetrics(SM_CXFRAME)*2;
  const LONG height=480+GetSystemMetrics(SM_CYFRAME)*2+
                    GetSystemMetrics(SM_CYMENU)+GetSystemMetrics(SM_CYCAPTION);
  limits.ptMinTrackSize={width,height};
  limits.ptMaxTrackSize={width,height};
  return LRESULT{0};
 }
 case WM_TIMER:
  if(wp==99&&services.advance_loading_cursor&&services.draw_loading_cursor){
   services.advance_loading_cursor(services.context);
   services.draw_loading_cursor(services.context);
   return LRESULT{0};
  }
  return std::nullopt; // no invented loading cursor provider
 case WM_SYSCOMMAND:
  if(wp==SC_SCREENSAVE||wp==SC_MONITORPOWER)return LRESULT{0};
  if(!state.windowed&&(wp==SC_MAXIMIZE||wp==SC_SIZE||wp==SC_MOVE))
   return LRESULT{1};
  return std::nullopt;
 case WM_COMMAND:{
  const UINT command=LOWORD(wp);
  if(command==0x3E9U){state.outer_state=1;return LRESULT{0};}
  if(command==0x3EAU){
   if(state.windowed)(void)GetWindowRect(hwnd,&state.previous_window_rect);
   state.windowed=!state.windowed;
   const HRESULT hr=create_or_reset_display_manager(
       hwnd,state.windowed?display::DisplayMode::Windowed:
                            display::DisplayMode::Fullscreen,graphics,error);
   if(FAILED(hr))(void)PostMessageA(hwnd,WM_CLOSE,0,0);
   return LRESULT{0};
  }
  if(command==0x9C41U&&state.help_active==0&&
     state.help_pending==0&&state.help_phase==0&&services.contextual_help){
   services.contextual_help(services.context);
   ++state.help_phase;
  }
  return std::nullopt;
 }
 case 0x212: case 0x232:
  state.last_input_ms=timeGetTime();
  return std::nullopt;
 default:return std::nullopt;
 }
}
}
#endif
