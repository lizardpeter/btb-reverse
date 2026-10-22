#pragma once
#if defined(_WIN32)
#include "btb/bink32_native_video_stream.hpp"
#include "btb/win32_retail_fireworks_movie_bank.hpp"
#include <array>
#include <chrono>
#include <optional>
#include <vector>
namespace btb::full_game {
// Source-file-backed independent Bink1 streams. Never synthesizes frames.
class Win32NativeFireworksMovieBank final {
public:
 explicit Win32NativeFireworksMovieBank(Win32OriginalDirectDraw& d):draw_(d){}
 ~Win32NativeFireworksMovieBank();
 bool open(const std::filesystem::path&,std::string&);
 bool decode_frame(std::size_t,bool&,std::string&);
 bool restart(std::size_t,std::string&);
 bool close(std::string&);
 void begin_show_frame() noexcept;
 bool decode_and_queue(std::size_t,int,int,bool&,std::string&);
 bool blit_queued_show(std::string&);
 bool ready() const noexcept{return ready_;}
private:
 struct Cue {std::size_t index;int x,y;IDirectDrawSurface7* frozen{};};
 void release_cues() noexcept;
 bool freeze_earlier(std::size_t,std::string&);
 Win32OriginalDirectDraw& draw_;
 std::array<std::optional<btb::bink32_recovered::BinkNativeVideoStream>,23> streams_{};
 std::array<IDirectDrawSurface7*,23> surfaces_{};
 std::array<std::uint32_t,23> width_{},height_{};
 std::array<bool,23> has_frame_{},final_copied_{},clock_started_{};
 std::array<std::chrono::steady_clock::time_point,23> origins_{};
 std::vector<Cue> cues_{};
 bool ready_{};
};
class Win32HybridFireworksMovieBank final {
public:
 Win32HybridFireworksMovieBank(Win32OriginalDirectDraw& d,
                              std::filesystem::path root,bool enabled)
  :native_(d),retail_(d,std::move(root)),enabled_(enabled){}
 bool open(const std::filesystem::path&,std::string&);
 bool restart(std::size_t,std::string&);
 bool close(std::string&);
 void begin_show_frame() noexcept;
 bool decode_and_queue(std::size_t,int,int,bool&,std::string&);
 bool blit_queued_show(std::string&);
 bool ready() const noexcept;
private:
 Win32NativeFireworksMovieBank native_;
 Win32RetailFireworksMovieBank retail_;
 bool enabled_{},using_native_{},active_{};
};
}
#endif
