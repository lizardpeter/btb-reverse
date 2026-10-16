#include "btb/win32_original_platform_link.hpp"
#include "btb/original_recompiled_bitmaps.hpp"
#include "btb/win32_recompiled_graphics.hpp"
#include "btb/win32_native_bitmap_registry.hpp"
#include "btb/win32_original_display_lifecycle.hpp"
#include "btb/win32_original_main_window_proc.hpp"
#include "btb/win32_retail_bink_provider.hpp"
#include <cstdio>

int main() {
#if defined(_WIN32)
    // These checks must actually execute in Release/x86/x64, where NDEBUG
    // removes assert expressions and could otherwise yield false passes.
    if(!btb::full_game::verify_linked_win32_providers()){
        std::fputs("Win32 game device provider failed preinitialization\n",stderr);
        return 1;
    }
    // Calls cross the final native-library linkage boundary and enforce
    // the retail null-parameter error for the genuine 004035F0 and 00415E50
    // implementations, not merely a behavioral plan or function registry.
    if(btb::original_win32::copy_bitmap_to_surface(nullptr,nullptr)
           !=DDERR_INVALIDPARAMS)return 2;
    btb::original_win32::LoadedBitmap loaded{};
    if(btb::original_win32::load_bitmap_to_directdraw_surface(
           nullptr,L"unused-original-bitmap.bmp",loaded)
           !=DDERR_INVALIDPARAMS)return 3;
    if(loaded.surface!=nullptr)return 4;
    if(btb::original_win32::blit_color_keyed_surface_clipped(
           nullptr,nullptr,0,0,nullptr,0,0)
           !=DDERR_INVALIDPARAMS)return 5;
    if(btb::original_win32::blt_fast_to_backbuffer(
           nullptr,0,0,nullptr,nullptr,0)!=E_POINTER)return 6;
    if(btb::original_win32::clear_backbuffer(nullptr,0)!=E_POINTER)return 7;
    RECT destination{};
    if(btb::original_win32::update_display_destination_rect(
           nullptr,true,destination))return 8;
    if(btb::original_win32::colorref_to_surface_pixel(
           nullptr,RGB(255,0,255))!=0xffffffffU)return 9;
    if(btb::original_win32::set_surface_transparency_color_key(
           nullptr,RGB(255,0,255))!=DDERR_INVALIDPARAMS)return 10;
    btb::original_win32::NativeBitmapRegistry registry;
    IDirectDrawSurface7* original_surface=nullptr;
    if(!registry.register_surface_reference(
           &original_surface,"image.bmp"))return 11;
    if(registry.high_water_count()!=1)return 12;
    if(!registry.mark_registered_surface_color_keyed(
           &original_surface))return 13;
    if(!registry.unregister_surface_reference(&original_surface))return 14;
    if(registry.high_water_count()!=1)return 15;
    if(!registry.register_surface_reference(
           &original_surface,"replacement.bmp"))return 16;
    if(registry.high_water_count()!=1)return 17;
    if(registry.reload_registered_surfaces(nullptr))return 18;
    if(registry.reload_in_progress())return 19;
    if(!registry.unregister_surface_reference(&original_surface))return 20;
    bool bitmap_reload=false;
    if(btb::original_win32::present_display_retail(
           nullptr,nullptr,nullptr,nullptr,false,bitmap_reload)!=E_POINTER)
        return 21;
    if(bitmap_reload)return 22;
    if(btb::original_win32::present_game_frame_retail(
           nullptr,nullptr,nullptr,nullptr,false,bitmap_reload)!=E_POINTER)
        return 23;
    if(btb::original_win32::restore_all_directdraw_surfaces(nullptr)!=E_POINTER)
        return 24;
    // Two original entry points linked to the ACTUAL Win32 game; neither
    // may attempt COM creation or menu mutation with a missing HWND.
    btb::full_game::Win32OriginalDirectDraw invalid_display{};
    std::string display_error;
    if(btb::original_win32::create_or_reset_display_manager(
           nullptr,btb::display::DisplayMode::Windowed,
           invalid_display,display_error)!=E_POINTER)return 25;
    if(display_error.empty() || invalid_display.initialized())return 26;
    display_error.clear();
    if(btb::original_win32::initialize_display_for_game_window(
           nullptr,btb::display::DisplayMode::Fullscreen,
           invalid_display,display_error)!=E_POINTER)return 27;
    if(display_error.empty() || invalid_display.initialized())return 28;
    btb::original_win32::RetailWindowState state{};
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_SYSKEYDOWN,VK_MENU,0,state,invalid_display,
           display_error)!=std::optional<LRESULT>{0})return 29;
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_SYSCOMMAND,SC_SCREENSAVE,0,state,invalid_display,
           display_error)!=std::optional<LRESULT>{0})return 30;
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_MOVE,0,0,state,invalid_display,
           display_error)!=std::optional<LRESULT>{0})return 31;
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_SIZE,SIZE_MINIMIZED,0,state,invalid_display,
           display_error).has_value()||state.accepting_input)return 32;
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_SIZE,SIZE_RESTORED,0,state,invalid_display,
           display_error).has_value()||!state.accepting_input)return 33;
    MINMAXINFO limits{};
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_GETMINMAXINFO,0,reinterpret_cast<LPARAM>(&limits),
           state,invalid_display,display_error)!=std::optional<LRESULT>{0}||
           limits.ptMaxTrackSize.x<640||limits.ptMinTrackSize.y<480)
        return 34;
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_COMMAND,0x3E9,0,state,invalid_display,
           display_error)!=std::optional<LRESULT>{0}||
           state.outer_state!=1)return 35;
    state.windowed=false;
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_SETCURSOR,0,0,state,invalid_display,
           display_error)!=std::optional<LRESULT>{1})return 36;
    if(btb::original_win32::main_window_proc_recompiled(
           nullptr,WM_DESTROY,0,0,state,invalid_display,
           display_error).has_value()||state.outer_state!=3)return 37;
    // Exact video position is from retail PE32 0x428195/0x42819C,
    // NOT estimated from the larger walkthrough mouse hover region.
    if(btb::full_game::kOriginalWalkthroughMovieX!=63 ||
       btb::full_game::kOriginalWalkthroughMovieY!=157 ||
       btb::full_game::kOriginalWalkthroughMovieWidth!=282 ||
       btb::full_game::kOriginalWalkthroughMovieHeight!=179)return 38;
    // The real video decoder must never allocate a DD7 movie surface
    // from an uninitialized display or pretend the frame was shown.
    IDirectDrawSurface7* invalid_movie=nullptr;
    if(invalid_display.create_bink_offscreen_surface(
           282,179,invalid_movie,display_error) ||
       invalid_movie!=nullptr || display_error.empty())return 39;
    display_error.clear();
    if(invalid_display.blit_bink_offscreen_surface(
           nullptr,282,179,63,157,display_error) ||
       display_error.empty())return 40;
#endif
    return 0;
}
