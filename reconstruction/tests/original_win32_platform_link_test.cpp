#include "btb/win32_original_platform_link.hpp"
#include "btb/original_recompiled_bitmaps.hpp"
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
#endif
    return 0;
}
