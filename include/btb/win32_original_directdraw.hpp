#pragma once

// This component is the actual Windows/DirectDraw7 implementation boundary,
// not another browser/HTML preview. It is deliberately a separate source
// unit until the complete original-game executable build is enabled.
//
// Requires the Win32 SDK, ddraw.h, ddraw.lib/dxguid.lib and a real HWND.
// The original software-bitmaps are loaded from installed/CD paths, never
// replaced with generated artwork. All COM releases are owned here.
#if defined(_WIN32)

#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <ddraw.h>

#include "btb/full_game_resources.hpp"
#include "btb/display_manager.hpp"
#include "btb/packed_frame_snapshot.hpp"
#include "btb/win32_frame_output.hpp"
#include "btb/bink32_yuva_frame.hpp"
#include "btb/packed_scene.hpp"
#include "btb/win32_native_bitmap_registry.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace btb::full_game {

class Win32OriginalDirectDraw final {
public:
    Win32OriginalDirectDraw() = default;
    ~Win32OriginalDirectDraw();
    Win32OriginalDirectDraw(const Win32OriginalDirectDraw&) = delete;
    Win32OriginalDirectDraw& operator=(
        const Win32OriginalDirectDraw&) = delete;

    [[nodiscard]] bool initialize(
        HWND hwnd,display::DisplayMode mode,std::string& error);
    [[nodiscard]] bool recreate(HWND hwnd,display::DisplayMode mode,std::string& error);
    [[nodiscard]] bool initialize_packed(HWND,std::string&);
    [[nodiscard]] bool packed_mode()const noexcept{return packed_scene_.valid();}
    [[nodiscard]] bool capture_current_frame(display::PackedFrameSnapshot& p,std::string& error){return capture_surface(p,error);}
    [[nodiscard]] bool write_native_global_movie(const bink32_recovered::BinkNativeYuVaFrame&,std::string&);
    [[nodiscard]] bool restore_last_composed_frame(std::string& error);
    [[nodiscard]] bool render_ordered(
        const std::vector<ResolvedDraw>& draw_stream,
        std::string& error);
    [[nodiscard]] bool render_additional(const std::vector<ResolvedDraw>&,std::string& error);
    [[nodiscard]] bool draw_transient_cursor(const ResolvedDraw&,std::string& error);
    [[nodiscard]] bool capture_underlay(CapturedUnderlay,std::string& error);
    [[nodiscard]] bool capture_presented_underlay(CapturedUnderlay,std::string& error);
    [[nodiscard]] bool render_captured_underlay(CapturedUnderlay,const std::vector<ResolvedDraw>&,std::string& error);
    [[nodiscard]] bool capture_replay_underlay(std::string& error);
    [[nodiscard]] bool render_replay_ordered(
        const std::vector<ResolvedDraw>& draw_stream,std::string& error);
    [[nodiscard]] bool present(std::string& error);
    void set_frame_output(Win32FrameOutput* output) noexcept {frame_output_=output;}
    [[nodiscard]] bool enhanced_output_active() const noexcept{return frame_output_!=nullptr;}
    [[nodiscard]] bool blit_packed_movie_surface(const display::PackedFrameSnapshot&,int,int,std::string&);
    void note_native_global_movie(const bink32_recovered::BinkNativeYuVaFrame& frame);
    void set_dialog_owner(HWND hwnd) noexcept {dialog_owner_=hwnd;}
    // Direct original 00402440 cooperative test and loss recovery.
    [[nodiscard]] HRESULT cooperative_status() const noexcept;
    [[nodiscard]] HRESULT prepare_native_frame(std::string& error);
    [[nodiscard]] bool restore_for_frame(std::string& error);
    [[nodiscard]] HRESULT clear_retail_backbuffer(DWORD color) noexcept;
    // Source bitmaps alone are released. Ordered frame paths and the
    // captured replay underlay stay available for surface-loss recovery.
    void release_activity_bitmaps() noexcept {release_cached_bitmaps();}
    void shutdown() noexcept;

    [[nodiscard]] bool initialized() const noexcept {
        return packed_mode() || (draw_ != nullptr && primary_ != nullptr &&
               backbuffer_ != nullptr);
    }

    // Borrowed original DirectDraw7 video target. The retail Bink
    // provider locks and writes this surface only during its frame
    // decode callback; it does not own or release the COM object.
    [[nodiscard]] IDirectDrawSurface7* bink_render_target() const noexcept {
        return backbuffer_;
    }
    [[nodiscard]] HWND window_handle() const noexcept {return dialog_owner_?dialog_owner_:hwnd_;}

    // Bink movie frames decode into source-sized offscreen DD7 surfaces.
    // Caller owns returned COM reference, and presents it only after
    // source bitmap draws (the 600x380 Spud Skate viewport is at 20,20).
    [[nodiscard]] bool create_bink_offscreen_surface(
        int width,int height,IDirectDrawSurface7*& output,
        std::string& error);
    [[nodiscard]] bool blit_bink_offscreen_surface(
        IDirectDrawSurface7* source,int width,int height,
        int x,int y,std::string& error);

private:
    struct OriginalSurface {
        IDirectDrawSurface7* surface{};
        int width{};
        int height{};
        bool color_keyed{};
    };
    [[nodiscard]] bool load_bitmap(
        const std::filesystem::path& file,
        OriginalSurface& out,std::string& error);
    [[nodiscard]] bool blit(
        const ResolvedDraw& draw,std::string& error);
    enum class UnderlayKind { None, Replay, Help, GlobalMovie, Modal };
    [[nodiscard]] bool capture_surface(display::PackedFrameSnapshot&,std::string&,IDirectDrawSurface7* source=nullptr);
    [[nodiscard]] display::PackedFrameSnapshot& underlay_target(CapturedUnderlay) noexcept;
    [[nodiscard]] bool render_ordered_with_underlay(
        const std::vector<ResolvedDraw>& stream,std::string& error,
        UnderlayKind kind);
    [[nodiscard]] bool restore_lost_surfaces(std::string& error);
    void release_cached_bitmaps() noexcept;

    struct PackedBitmap{unsigned width{},height{};std::vector<std::uint8_t> pixels;std::uint64_t last_used{};};
    PackedScene packed_scene_{};
    std::unordered_map<std::filesystem::path,PackedBitmap> packed_bitmaps_;
    std::size_t packed_bitmap_bytes_{};
    std::uint64_t packed_bitmap_clock_{};
    bool blit_packed_bitmap(const ResolvedDraw&,std::string&);
    HWND hwnd_{};
    HWND dialog_owner_{};
    Win32FrameOutput* frame_output_{};
    bool bitmap_frame_valid_{};
    std::vector<ResolvedDraw> bitmap_frame_{};
    OpaqueGlobalVideo opaque_video_{};
    bool layer_frame_valid_{},has_pixel_layer_{};
    std::vector<FrameCompositionLayer> layer_frame_{};
    void reset_layers() noexcept{layer_frame_valid_=false;has_pixel_layer_=false;layer_frame_.clear();}
    void append_layers(const std::vector<ResolvedDraw>&);
    void append_pixel_layer(const display::PackedFrameSnapshot&,int,int);
    std::optional<ResolvedDraw> late_default_cursor_{};
    display::DisplayMode mode_{display::DisplayMode::Windowed};
    IDirectDraw7* draw_{};
    IDirectDrawSurface7* primary_{};
    IDirectDrawSurface7* backbuffer_{};
    IDirectDrawClipper* clipper_{};
    bool extra_back_reference_{};
    std::unordered_map<std::wstring,OriginalSurface> images_{};
    std::unique_ptr<original_win32::NativeBitmapRegistry> registry_{
        std::make_unique<original_win32::NativeBitmapRegistry>()};
    // Independent CPU-owned snapshots survive COM surface/device loss.
    display::PackedFrameSnapshot replay_underlay_{},help_underlay_{},
        global_underlay_{},modal_underlay_{},composed_frame_{};
    display::PackedFrameSnapshot pending_cursorless_frame_{},presented_cursorless_frame_{};
    bool cursor_pending_{};
    bool surface_lost_during_draw_{};
};

} // namespace btb::full_game
#endif
