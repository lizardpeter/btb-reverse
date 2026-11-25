# Enhanced edition: first Vulkan migration stage

The faithful executable still uses the original DirectDraw 7 presentation.
Enhanced is a distinct target and selects Vulkan 1.4 rather than silently
falling back to a legacy presenter.

## Windows build

Install a Vulkan 1.4 SDK and a GPU driver exposing Vulkan 1.4. Slang is
recommended (slangc in PATH or the SDK Bin directory).

```powershell
cmake -S . -B build/enhanced -A x64 -DBTB_BUILD_ENHANCED=ON -DBTB_REQUIRE_VULKAN=ON -DBTB_REQUIRE_SLANG=ON
cmake --build build/enhanced --config Release --target btb_game_enhanced
build/enhanced/enhanced/Release/btb_game_enhanced.exe --native-original "C:\Games\Bob Builds a Park"
```

The enhanced executable opens a resizable 4:3 letterboxed Vulkan window.
F11 toggles monitor-sized borderless fullscreen. Default nearest-neighbor
integer scaling preserves the 640x480 original pixels, at a native output
swapchain extent including 1440p and 4K displays. Environment settings:
- BTB_FRACTIONAL_SCALE=1: use the largest aspect-correct picture.
- BTB_LINEAR_FILTER=1: optional bilinear scaling.
- BTB_UNCAPPED_PRESENT=1: request immediate or mailbox instead of FIFO when
  supported (source game ticks are still on the existing 16 ms host timer).
- BTB_DISCORD_APP_ID: optional numeric Discord developer application ID.
  The local desktop client receives the currently active game activity and
  session start time. Missing ID/client never prevents gameplay. No player
  names, save contents, tokens or external network requests are used.

## Implementation status

Vulkan 1.4 instance/device selection, Win32 swapchain, resize/out-of-date
handling, staging upload, GPU scaled blit and per-image present semaphores
are implemented. The DirectDraw composition is deliberately isolated on a
hidden HWND; native Bink/movie/audio and original state logic are unchanged.
This is Vulkan presentation, not yet a fully DirectDraw-free renderer.
The rendered bitmap/video content remains 640x480, not newly authored 4K art.
GPU sprite and Bink YUV shaders are initial Slang source with build rules,
not yet dispatched by the presentation path. Independent uncapped render
scheduling, texture atlas/batching, GPU movie plane upload, and fully native
render targets remain subsequent milestones. No interactive parity or
performance measurement is implied by source compilation.

The platform-neutral policy test covers integer/fractional scaling and
letterbox hit testing at 1440p/4K, and Discord state mapping.
Vulkan synchronization and local RPC references:
- https://docs.vulkan.org/guide/latest/swapchain_semaphore_reuse.html
- https://docs.vulkan.org/refpages/latest/refpages/source/VkSwapchainCreateInfoKHR.html
- https://docs.shader-slang.org/en/stable/first-slang-shader.html
- https://github.com/discord/discord-api-docs/blob/main/developers/topics/rpc.mdx
