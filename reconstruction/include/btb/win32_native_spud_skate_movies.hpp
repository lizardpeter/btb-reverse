#pragma once
#if defined(_WIN32)
#include "btb/full_game_spud_skate.hpp"
#include "btb/win32_original_directdraw.hpp"
#include "btb/bink32_native_video_stream.hpp"
#include "btb/win32_retail_spud_skate_movies.hpp"
#include <array>
#include <chrono>
#include <optional>
namespace btb::full_game {
class Win32NativeSpudSkateMovies final : public SpudSkateMovieSource {
public:
 explicit Win32NativeSpudSkateMovies(Win32OriginalDirectDraw& d):draw_(d){}
 ~Win32NativeSpudSkateMovies() override;
 bool open(const std::array<std::filesystem::path,5>&,std::string&) override;
 bool observe(SpudSkateMovieFrame&,std::string&) override;
 bool select_quality(spud_skate::StuntQuality,std::string&) override;
 bool resynchronize_all(int,std::string&) override;
 bool begin_end_movie(std::string&) override;
 bool close(std::string&) override;
 bool blit_selected(std::string&);
private:
 bool decode_to_surface(std::size_t,std::string&);
 bool pace(std::size_t,std::uint32_t,std::string&);
 Win32OriginalDirectDraw& draw_;
 std::array<std::optional<bink32_recovered::BinkNativeVideoStream>,5> streams_{};
 std::array<IDirectDrawSurface7*,5> surfaces_{};
 std::chrono::steady_clock::time_point origin_{};
 std::size_t selected_{};
 bool active_{},end_movie_{},end_decoded_{},has_image_{},clock_started_{};
};
class Win32HybridSpudSkateMovies final : public SpudSkateMovieSource {
public:
 Win32HybridSpudSkateMovies(Win32OriginalDirectDraw& d,std::filesystem::path root,bool enabled)
 :native_(d),retail_(d,std::move(root)),enabled_(enabled){}
 bool open(const std::array<std::filesystem::path,5>&,std::string&) override;
 bool observe(SpudSkateMovieFrame&,std::string&) override;
 bool select_quality(spud_skate::StuntQuality,std::string&) override;
 bool resynchronize_all(int,std::string&) override;
 bool begin_end_movie(std::string&) override;
 bool close(std::string&) override;
 bool blit_selected(std::string&);
private:
 SpudSkateMovieSource* source() noexcept;
 Win32NativeSpudSkateMovies native_;
 Win32RetailSpudSkateMovies retail_;
 bool enabled_{},using_native_{},active_{};
};
}
#endif
