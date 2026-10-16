#pragma once
#if defined(_WIN32)
#include "btb/win32_original_game_device.hpp"
#include "btb/win32_original_bink_exports.hpp"
#include "btb/win32_original_directsound.hpp"
#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>
namespace btb::full_game {
// Real 32-bit retail Bink decode/presentation for both global cutscenes and
// the independent, looping instructional walkthrough Bink surface.
// Uses the actual retail x86 binkw32.dll. No media is synthesized.
class Win32OriginalBinkMovie final : public OriginalBinkProvider {
public:
 explicit Win32OriginalBinkMovie(
   std::filesystem::path root,Win32OriginalDirectDraw& graphics,
   Win32OriginalDirectSound& audio)
     : root_(std::move(root)),graphics_(graphics),audio_(audio) {}
 ~Win32OriginalBinkMovie() override;
 [[nodiscard]] bool service(bool& global_finished,std::string& error) override;
 [[nodiscard]] bool open_global(
   const std::filesystem::path& movie,std::string& error) override;
 [[nodiscard]] bool skip_global(std::string& error) override;
 [[nodiscard]] bool open_walkthrough(
   const std::filesystem::path& movie,
   const std::filesystem::path& help_audio,std::string& error) override;
 [[nodiscard]] bool close_walkthrough(std::string& error) override;
 [[nodiscard]] bool render_walkthrough_overlay(std::string& error) override;
private:
 // Only the seven documented public BINK struct prefix fields are inspected.
 struct BinkPrefix {
  std::uint32_t width{},height{},frames{},frame_num{},last_frame_num{};
  std::uint32_t frame_rate{},frame_rate_div{};
 };
 using BinkOpenFn=void* (__stdcall*)(const char*,std::uint32_t);
 using BinkCloseFn=void (__stdcall*)(void*);
 using BinkWaitFn=std::int32_t (__stdcall*)(void*);
 using BinkDoFrameFn=std::int32_t (__stdcall*)(void*);
 using BinkCopyFn=std::int32_t (__stdcall*)(
   void*,void*,std::int32_t,std::uint32_t,std::uint32_t,std::uint32_t,std::uint32_t);
 using BinkNextFn=void (__stdcall*)(void*);
 [[nodiscard]] bool ensure_decoder(std::string& error);
 [[nodiscard]] bool open_original_file(
    const std::filesystem::path& file,void*& handle,std::string& error);
 void close_global() noexcept;
 void close_walkthrough_handle() noexcept;
 std::filesystem::path root_{};
 Win32OriginalDirectDraw& graphics_;
 Win32OriginalDirectSound& audio_;
 Win32OriginalBinkExports exports_{};
 bool sound_system_ready_{};
 void* current_{};
 std::uint32_t width_{},height_{},total_frames_{},decoded_frames_{};
 bool last_frame_shown_{},have_pixels_{};
 std::vector<std::uint8_t> bgra_{};
 void* walkthrough_{};
 std::uint32_t walk_width_{},walk_height_{},walk_total_frames_{},walk_decoded_frames_{};
 bool walk_have_pixels_{},walk_restart_on_next_frame_{};
 std::vector<std::uint8_t> walkthrough_bgra_{};
};
} // namespace btb::full_game
#endif
