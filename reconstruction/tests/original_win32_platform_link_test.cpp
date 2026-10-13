#include "btb/win32_original_platform_link.hpp"
#include "btb/original_recompiled_bitmaps.hpp"
#include "btb/win32_recompiled_graphics.hpp"
#include "btb/win32_native_bitmap_registry.hpp"
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
#endif
    return 0;
}
