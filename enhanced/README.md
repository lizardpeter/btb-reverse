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
build/enhanced/Release/btb_game_enhanced.exe --native-original "C:\Games\Bob Builds a Park"
```

The enhanced executable opens a resizable 4:3 letterboxed Vulkan window.
F11 toggles monitor-sized borderless fullscreen. Default nearest-neighbor
integer scaling preserves the 640x480 original pixels, at a native output
swapchain extent including 1440p and 4K displays. Environment settings:
- BTB_FRACTIONAL_SCALE=1: use the largest aspect-correct picture.
- BTB_LINEAR_FILTER=1: optional bilinear scaling.
- BTB_UNCAPPED_PRESENT=1: request immediate or mailbox instead of FIFO when
  supported (the independent idle presenter reuses the latest frame without running
  gameplay again; source ticks stay on their existing 16 ms timer).
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
not yet dispatched by the presentation path. Texture atlas/batching, GPU movie plane upload, and fully native
render targets remain subsequent milestones. No interactive parity or
performance measurement is implied by source compilation.

The platform-neutral policy test covers integer/fractional scaling and
letterbox hit testing at 1440p/4K, and Discord state mapping.
Vulkan synchronization and local RPC references:
- https://docs.vulkan.org/guide/latest/swapchain_semaphore_reuse.html
- https://docs.vulkan.org/refpages/latest/refpages/source/VkSwapchainCreateInfoKHR.html
- https://docs.shader-slang.org/en/stable/first-slang-shader.html
- https://github.com/discord/discord-api-docs/blob/main/developers/topics/rpc.mdx

## Experimental GPU bitmap composition

Set `BTB_GPU_SPRITES=1` to enable the Slang compute sprite compositor. Ordered bitmap-only frame commands are retained by the native renderer and supplied to the Vulkan presenter. The compositor batches clipped BMP sprites into one GPU dispatch using 32x32 tile lists in reverse painter order; color-keyed magenta is discarded and each output pixel has a single writer. A bounded CPU bitmap cache and per-frame storage-buffer atlas supply original artwork. Movie frames, captured-underlay frames, unusual packed pixel formats, and over-budget batches use the fully composed reference frame instead. DirectDraw composition still runs offscreen to support snapshots, printing and the legacy movie providers; this is an experimental second rendering path, not yet removal of DirectDraw or a measured performance improvement. The portable test verifies crop geometry, transparency, painter order, tile boundaries, channel order and malformed inputs. GPU execution requires the compiled shader next to the executable in its shaders directory.

The enhanced Vulkan compute differential uses 192 deterministic overlapping and clipped sprite commands with mixed keyed/opaque colors and checks all 307,200 output pixels in both RGBA and BGRA modes (614,400 comparisons). It runs without a desktop surface; a missing Vulkan 1.4 compute device explicitly skips. This tests the actual compiled Slang pipeline, not merely a shader source or CPU oracle.

Optional `BTB_DISCORD_LARGE_IMAGE` selects an uploaded Discord application asset by key (lowercase letters, numbers, underscores, hyphens). The enhanced viewport rejects letterbox-only click starts and hides the software cursor outside the active picture; releases for drags started within the image still reach the source input model.

## Experimental native Bink GPU conversion

Set `BTB_GPU_BINK=1` to enable the opaque full-screen native Bink conversion. The original native decoder still supplies authentic Y/U/V/A planes. When a decoded 640x480 frame has all alpha bytes equal to 255 and no additional draw overwrites it, Vulkan uploads the three packed YUV420 planes and executes the Slang integer BT.601 conversion to the swapchain's RGBA/BGRA order. Native CPU conversion and the hidden composition surface remain the reference and fallback; subsequent unchanged frames, alpha-bearing images, movie overlays and captured backgrounds stay on the reference path. This does not replace Bink entropy decoding or audio timing. The headless GPU differential also compares 614,400 video pixels against `blit_bink_yuva_to_surface`, for 1,228,800 total sprite/video pixel comparisons per execution. The earlier alpha-aware texture-based shader remains a candidate for mixed movie overlays, not the path being dispatched here.
