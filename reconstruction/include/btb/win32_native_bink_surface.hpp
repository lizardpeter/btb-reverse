#pragma once
// Real DirectDraw7 upload adapter for the native (DLL-free) Bink video frames.
#if defined(_WIN32)
#include "btb/bink32_yuva_frame.hpp"
#include <ddraw.h>
#include <string>
namespace btb::full_game {
[[nodiscard]] bool copy_native_bink_frame_to_directdraw(
    IDirectDrawSurface7* surface,
    const btb::bink32_recovered::BinkNativeYuVaFrame& decoded,
    std::string& error);
}
#endif
