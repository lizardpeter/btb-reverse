#pragma once

#if defined(_WIN32)
#include "btb/fireworks_movie_bank_manifest.hpp"
#include "btb/win32_original_bink_exports.hpp"
#include "btb/win32_original_directdraw.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

namespace btb::full_game {

// Retail 0x004115C0: ALL 23 original files are backed by retained
// source memory and independent Bink1 decoder handles / DD7 surfaces.
// This is the physical decoder, not the show scheduler or a movie timer.
class Win32RetailFireworksMovieBank final {
public:
    Win32RetailFireworksMovieBank(
        Win32OriginalDirectDraw& draw,
        std::filesystem::path installed_game_root)
        :draw_(draw),root_(std::move(installed_game_root)) {}
    ~Win32RetailFireworksMovieBank();
    Win32RetailFireworksMovieBank(const Win32RetailFireworksMovieBank&)=delete;
    Win32RetailFireworksMovieBank& operator=(
        const Win32RetailFireworksMovieBank&)=delete;

    [[nodiscard]] bool open(const std::filesystem::path& source_subgame,
        std::string& error);
    [[nodiscard]] bool decode_frame(
        std::size_t index,bool& finished,std::string& error);
    [[nodiscard]] bool blit_frame(
        std::size_t index,int x,int y,std::string& error);
    [[nodiscard]] bool restart(std::size_t index,std::string& error);
    [[nodiscard]] bool close(std::string& error);
    // Win32 GameRoot decodes real Bink surfaces during state-9 dispatch;
    // these source overlays are presented only AFTER the DD7 backing frame.
    struct ShowBlit {
        std::size_t index{};
        int x{},y{};
        // A stream may be referenced by multiple active effect records in
        // one source frame. Freeze earlier DD7 pixels before the next decode
        // overwrites the only per-index offscreen source surface.
        IDirectDrawSurface7* frozen_pixels{};
    };
    void begin_show_frame() noexcept;
    [[nodiscard]] bool decode_and_queue(
        std::size_t index,int x,int y,bool& finished,
        std::string& error);
    [[nodiscard]] bool blit_queued_show(std::string& error);

    [[nodiscard]] bool ready() const noexcept {return ready_;}
    [[nodiscard]] bool has_frame(std::size_t index) const noexcept {
        return index<has_frame_.size() && has_frame_[index];
    }

private:
    void release_queued_show() noexcept;
    [[nodiscard]] bool freeze_prior_uses(
        std::size_t index,std::string& error);
    [[nodiscard]] bool load_original_bink_dll(std::string& error);
    [[nodiscard]] bool reject_and_close(
        std::string reason,std::string& error);
    Win32OriginalDirectDraw& draw_;
    std::filesystem::path root_;
    Win32OriginalBinkExports dll_{};
    std::array<std::vector<std::uint8_t>,23> bytes_{};
    std::array<void*,23> handles_{};
    std::array<IDirectDrawSurface7*,23> surfaces_{};
    std::array<std::uint32_t,23> width_{};
    std::array<std::uint32_t,23> height_{};
    std::array<bool,23> has_frame_{};
    std::array<bool,23> final_frame_copied_{};
    std::vector<ShowBlit> queued_show_blits_{};
    bool ready_{};
};

} // namespace btb::full_game
#endif
