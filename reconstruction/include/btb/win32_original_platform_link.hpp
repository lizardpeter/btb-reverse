#pragma once
#if defined(_WIN32)
namespace btb::full_game {
// Forces the real DirectDraw7, DirectSound8, Bink exports, and game-device
// implementations into the final Windows executable. This performs safe
// pre-initialization checks only, not simulated hardware rendering.
[[nodiscard]] bool verify_linked_win32_providers();
}
#endif
