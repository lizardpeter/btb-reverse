#include "btb/win32_original_display_lifecycle.hpp"
#if defined(_WIN32)
#include <cstdint>
#include <string>
namespace btb::original_win32 {
// PE32 00401FA0: dispose the prior manager, construct a fresh instance,
// initialize mode-specific primary/render surfaces and clipper, install the
// windowed system-menu style/resource 102, then clear the original backbuffer.
// Owns real DirectDraw7/GDI objects rather than returning a synthetic plan.
// The original allocator and HWND storage use native Win32 ownership in
// Win32OriginalDirectDraw, not raw host-dependent 32-bit pointer casts.
HRESULT create_or_reset_display_manager(
    HWND window,display::DisplayMode mode,
    full_game::Win32OriginalDirectDraw& graphics,std::string& error) {
    // The original dereferences a null display/window in some paths;
    // modern host must report an error instead of invoking undefined behavior.
    if(!window || !IsWindow(window)){
        graphics.shutdown();
        error="00401FA0 requires an existing native game HWND";
        return E_POINTER;
    }
    graphics.shutdown(); // 00402FE0 then native object release
    if(!graphics.initialize(window,mode,error)) {
        // The host initialization interface currently returns bool, not the
        // exact failing DD7 HRESULT. Never claim the original failure code.
        return E_FAIL;
    }
    if(mode==display::DisplayMode::Windowed){
        const auto style=static_cast<DWORD>(GetWindowLongA(window,GWL_STYLE));
        (void)SetWindowLongA(window,GWL_STYLE,
                             static_cast<LONG>(style|WS_SYSMENU));
        const auto instance=reinterpret_cast<HINSTANCE>(
            GetWindowLongPtrA(window,GWLP_HINSTANCE));
        const HMENU menu=LoadMenuA(instance,MAKEINTRESOURCEA(0x66));
        (void)SetMenu(window,menu);
    }
    const HRESULT filled=graphics.clear_retail_backbuffer(0);
    if(FAILED(filled)){
        error="00401FA0 retail ClearBackBuffer failed (HRESULT "+
              std::to_string(static_cast<std::uint32_t>(filled))+")";
        return filled;
    }
    error.clear();
    return S_OK;
}

// PE32 00401F80: original wrapper forwards CreateOrResetDisplayManager,
// normalizing all SUCCEEDED HRESULTs to zero and preserving negative failures.
HRESULT initialize_display_for_game_window(
    HWND window,display::DisplayMode mode,
    full_game::Win32OriginalDirectDraw& graphics,std::string& error) {
    const HRESULT hr=create_or_reset_display_manager(
        window,mode,graphics,error);
    return SUCCEEDED(hr)?S_OK:hr;
}
}
#endif
