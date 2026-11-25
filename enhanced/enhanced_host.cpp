#include "enhanced_host.hpp"
#include "btb/win32_recompiled_host.hpp"
#if defined(_WIN32)
#if defined(BTB_HAS_VULKAN)
#include "vulkan_presenter.hpp"
#endif
namespace btb::enhanced {
int run_enhanced(HINSTANCE instance,const std::filesystem::path& root,int show,bool skip,int profile){
#if defined(BTB_HAS_VULKAN)
    // Keep Win32 client coordinates and Vulkan swapchain pixels in the same
    // coordinate system on high-DPI monitors (before any game HWND exists).
    (void)SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    auto presenter=make_vulkan_presenter();
    return full_game::run_recompiled_native_game(instance,root,show,skip,profile,presenter.get());
#else
    MessageBoxW(nullptr,L"This enhanced executable was built without the Vulkan 1.4 SDK. Reconfigure with BTB_REQUIRE_VULKAN=ON.",L"Vulkan backend unavailable",MB_OK|MB_ICONERROR);
    return 7;
#endif
}
}
#endif
