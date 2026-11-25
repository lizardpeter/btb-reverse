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

## High-refresh presentation and late-sampled cursor

The enhanced message loop now pumps independent presentation in both FIFO and low-latency modes. A high-resolution Windows waitable timer replaces the coarse 1 ms message-loop timeout when available, with bounded message draining and nonblocking fence/acquire checks. The original 16 ms source-game timer and recovered gameplay rules are unchanged.

- `BTB_MAX_FPS=0` (default): no software FPS cap; `30` through `1000` selects a limit (e.g. 120, 144, 165, 240, 360, 480). A monotonic deadline prevents catch-up bursts.
- `BTB_PRESENT_MODE=fifo` (default): display-synchronized; `mailbox` and `immediate` request supported low-latency modes, with FIFO fallback. `BTB_UNCAPPED_PRESENT=1` remains a shorthand for requesting immediate mode when no explicit mode is set.
- `BTB_SHOW_FPS=1`: window title reports successful queue presents/s separately from source updates/s. This is not a measurement of physical displayed frames.
- Default normal cursor is late-sampled on each presentation. The source frame is retained without its cursor; the existing GPU compositor can compose the cursorless draw stream. A small 19x19 keyed cursor overlay is copied into a scratch image on-device, with disjoint opaque row runs. No full CPU frame upload is needed for cursor-only refreshes. Special activity cursors, dragged pieces, Help, movies and missing/unexpected cursor artwork retain the original source-frame behavior. `BTB_DISABLE_LATE_CURSOR=1` disables the separate overlay.

Portable tests cover pacing at 60, 120, 144, 165, 240, 360, 480 and 1000 FPS, long stalls, exact color-key runs, cursor bounds and channel packing. Physical high-refresh throughput depends on GPU, desktop composition, swapchain mode and display. This change does not invent interpolated original animation frames or accelerate gameplay to the display refresh rate.

## High-refresh sprite motion and resident artwork

Set `BTB_INTERPOLATE_SPRITES=1` to opt into render-only interpolation (also enables the GPU sprite compositor). Only identical painter-order signatures in a continuing activity-run state are eligible; a unique keyed sprite with the same asset, crop and clip can move by at most 32 pixels per source tick. Duplicate assets, changed animation artwork, scene changes, source stalls, teleportation, movies, captured backgrounds, and special cursor frames remain on the exact source-frame path. Interpolation uses the two most recent source positions and intentionally trades one source interval of visual delay for smoother motion. Positions remain on the original 640x480 pixel grid. The independently sampled normal pointer is not interpolated. This is not frame generation or a change to simulation timing. Default remains off for lowest latency.

The GPU compositor now keeps artwork in a reserved tail of its mapped storage buffer for unchanged ordered asset paths. Only tile bins and sprite metadata are refreshed on motion frames; the atlas is rebuilt if the draw-path list changes or another compute operation invalidates it. Capacity checks preserve fallback behavior. Portable regressions cover 16 ms position sampling, final exact samples, teleport/scene/stall rejection, and fixed-atlas layout bounds.

The Windows SDK build and SPIR-V validation for commit 5232cbed succeeded; its hosted runner lacked a compatible Vulkan 1.4 driver (VK_ERROR_INCOMPATIBLE_DRIVER). The presentation smoke now explicitly skips that specific environment failure rather than treating it as proof of device execution. Other initialization and rendering errors remain failures.

### Required headless shader execution

The compute differential is now platform-neutral. The Windows job publishes the exact Slang-compiled sprite and opaque Bink SPIR-V; a dependent Linux job validates that bytecode for Vulkan 1.2/SPIR-V 1.5 and executes it using Mesa Lavapipe. This is the same Vulkan compute implementation, not a separate CPU shader interpreter or substitute shader. It also verifies retained atlas composition across changed positions, channel order, and invalidation/reload around video conversion. Together the tests compare 2,150,400 output pixels. Linux software-driver execution is mandatory (skip is failure); only the actual Windows presentation/driver smoke can skip for a missing compatible device. Production still requires Vulkan 1.4 and real desktop performance remains unmeasured.

## Mixed bitmap, movie and saved-background layers

`BTB_GPU_LAYERS=1` opts into ordered mixed-layer composition. A scene may retain real RGB pixels from a saved replay/Help/Options/global-movie underlay, ordinary bitmap commands, the actual native decoded movie surface at its source viewport, and subsequent foreground sprites and cursor. The same tile-binned Slang shader composes these in painter order. Packed surfaces are normalized to RGBA with checked format conversion; movie magenta is opaque, while keyed BMP magenta is transparent. Normal cursor late sampling remains available. A failed snapshot or oversized batch falls back to the complete reference frame; the original DirectDraw renderer remains responsible for the current reference composition, offscreen video storage, and printing. Alpha from legacy video is already resolved within the movie surface before this layer is captured; this is GPU layer composition, not a claim of GPU alpha decoding or blending. The new portable plan test covers RGB565, negative placement, clipping, ordering and key semantics. The headless GPU test adds 307,200 mixed-surface pixels, for 2,457,600 total configured comparisons.
