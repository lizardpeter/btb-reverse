#pragma once
#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <filesystem>
namespace btb::full_game {
// Opt-in original-source game host. Uses the real GameRoot,
// NativeGameSession, DirectDraw7 and DirectSound8; incomplete movie/provider
// paths fail visibly instead of substituting the Dinosaur-only GDI preview.
[[nodiscard]] int run_recompiled_native_game(
    HINSTANCE instance,const std::filesystem::path& retail_root,int show,
    bool skip_video = false);
}
#endif
