#pragma once

#if defined(_WIN32)
#include "btb/full_game_spud_skate.hpp"
#include "btb/win32_original_bink_exports.hpp"
#include "btb/win32_original_directdraw.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <string>
#include <utility>

namespace btb::full_game {

// Native game MovieSource for source 0x424690. The retail x86 DLL
// does actual decode of the 4 quality streams and the end.bik.
// Four original movie surface buffers are kept separately and copied
// into the 600x380 game viewport AFTER the original bitmap frame.
// Missing, unsynchronized or undecodable frames NEVER produce points.
class Win32RetailSpudSkateMovies final : public SpudSkateMovieSource {
public:
    Win32RetailSpudSkateMovies(
        Win32OriginalDirectDraw& draw,
        std::filesystem::path installed_original_root)
        :draw_(draw),root_(std::move(installed_original_root)) {}
    ~Win32RetailSpudSkateMovies() override;
    Win32RetailSpudSkateMovies(const Win32RetailSpudSkateMovies&)=delete;
    Win32RetailSpudSkateMovies& operator=(
        const Win32RetailSpudSkateMovies&)=delete;

    [[nodiscard]] bool open(
        const std::array<std::filesystem::path,5>& original_binks,
        std::string& error) override;
    [[nodiscard]] bool observe(
        SpudSkateMovieFrame& frame,std::string& error) override;
    [[nodiscard]] bool select_quality(
        spud_skate::StuntQuality quality,std::string& error) override;
    [[nodiscard]] bool resynchronize_all(
        int decoded_frame,std::string& error) override;
    [[nodiscard]] bool begin_end_movie(
        std::string& error) override;
    [[nodiscard]] bool close(std::string& error) override;

    // Called by the real GameDevice after submitting the original
    // game-owned BMP layer, before DirectDraw presents the backbuffer.
    [[nodiscard]] bool blit_selected(std::string& error);
    [[nodiscard]] bool active() const noexcept {return active_;}

private:
    [[nodiscard]] bool load_dll(std::string& error);
    [[nodiscard]] bool decode_to_surface(
        std::size_t stream,std::string& error);
    [[nodiscard]] bool seek_to_exact_frame(
        std::size_t stream,int frame,std::string& error);

    Win32OriginalDirectDraw& draw_;
    std::filesystem::path root_;
    Win32OriginalBinkExports exports_{};
    std::array<void*,5> handles_{};
    std::array<IDirectDrawSurface7*,5> surfaces_{};
    std::size_t selected_{};
    bool active_{};
    bool in_end_movie_{};
    bool has_decoded_frame_{};
    std::uint32_t last_observed_frame_{};
};

} // namespace btb::full_game
#endif
