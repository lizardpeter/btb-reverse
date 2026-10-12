#include "btb/win32_original_platform_link.hpp"
#if defined(_WIN32)
#include "btb/win32_original_bink_exports.hpp"
#include "btb/win32_original_directdraw.hpp"
#include "btb/win32_original_directsound.hpp"
#include "btb/win32_original_game_device.hpp"
#include <filesystem>
#include <string>

namespace btb::full_game {
namespace {
// No fabricated video decoder. A probe that cannot play a source file
// must reject it explicitly; the true original Bink bridge remains pending.
class RejectUnavailableBink final : public OriginalBinkProvider {
public:
    bool service(bool&,std::string& err) override {
        err="original compatible Bink decoder not initialized";return false;
    }
    bool open_global(const std::filesystem::path&,std::string& err) override {
        err="original compatible Bink decoder not initialized";return false;
    }
    bool open_walkthrough(const std::filesystem::path&,
        const std::filesystem::path&,std::string& err) override {
        err="original compatible Bink decoder not initialized";return false;
    }
    bool close_walkthrough(std::string& err) override {
        err="original compatible Bink decoder not initialized";return false;
    }
    bool render_walkthrough_overlay(std::string& err) override {
        err="original compatible Bink decoder not initialized";return false;
    }
};
}
bool verify_linked_win32_providers() {
    Win32OriginalDirectDraw graphics;
    Win32OriginalDirectSound audio;
    Win32OriginalBinkExports imports;
    RejectUnavailableBink no_decoder;
    Win32OriginalGameDevice device(graphics,audio,no_decoder);
    std::string error;
    // Failing when there is no HWND is a real native implementation
    // precondition and never allocates a synthetic DirectDraw7 device.
    if(graphics.initialize(nullptr,display::DisplayMode::Windowed,error) ||
       error.empty() || graphics.initialized())return false;
    error.clear();
    if(audio.initialize(nullptr,error)||error.empty())return false;
    error.clear();
    // Only an absolute retail binkw32.dll may be passed to LoadLibraryExW.
    if(imports.load_exact_original(std::filesystem::path(L"binkw32.dll"),error) ||
       error.empty() || imports.loaded())return false;
    error.clear();
    PhysicalFrameObservation observation{};
    if(device.observe(observation,error)||error.empty())return false;
    return true;
}
}
#endif
