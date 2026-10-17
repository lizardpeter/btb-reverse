#pragma once
// DLL-free Bink1 VIDEO playback lifecycle. Refuses compressed audio rather than
// playing a cinematic silently or claiming game playback is fully native.
// The platform invokes service on real host ticks and supplies a real
// presentation callback that writes actual pixels (e.g. the DD7 adapter).
#include "btb/bink32_native_video_stream.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace btb::bink32_recovered {
class BinkNativeSilentMovie final {
public:
    using Presenter=std::function<bool(
        const BinkNativeYuVaFrame&,std::string&)>;

    // Original assets are read-only; impose a ceiling before allocating.
    [[nodiscard]] static std::optional<BinkNativeSilentMovie> open_file(
        const std::filesystem::path& file,std::string& error,
        bool experimental_audio_tracks=false);
    [[nodiscard]] static std::optional<BinkNativeSilentMovie> open_bytes(
        std::vector<std::uint8_t> original_bik,std::string& error,
        bool experimental_audio_tracks=false);

    // now_us is a monotonic HOST timestamp. The first service establishes
    // origin at frame one. Cap catch-up decoding so an enormous pause cannot
    // block one game tick indefinitely; skipped frames are still decoded
    // in sequence to preserve delta-frame history.
    // At most four new decoded frames per call, with only the last presented.
    // The presenter may fail. On a later service the SAME decoded frame is
    // retried rather than skipped. Never report EOS until terminal pixels
    // have been physically accepted and their display interval elapsed.
    [[nodiscard]] bool service(
        std::uint64_t now_us,const Presenter& present,
        bool& finished,std::string& error);

    // Restart all source frame history and the host clock (walkthrough loop).
    void rewind() noexcept;
    [[nodiscard]] std::uint32_t frame_number() const noexcept {
        return stream_.frame_number();
    }
    [[nodiscard]] std::uint32_t frame_count() const noexcept {
        return stream_.frame_count();
    }
    [[nodiscard]] std::uint32_t width() const noexcept {
        return width_;
    }
    [[nodiscard]] std::uint32_t height() const noexcept {
        return height_;
    }
    [[nodiscard]] bool has_presented_frame() const noexcept {
        return published_frame_!=0;
    }
    [[nodiscard]] const BinkNativeYuVaFrame* current_frame() const noexcept {
        return has_presented_frame()?stream_.current_frame():nullptr;
    }
private:
    explicit BinkNativeSilentMovie(
        BinkNativeVideoStream stream,std::uint32_t width,
        std::uint32_t height,std::uint64_t end_us)
        : stream_(std::move(stream)),width_(width),height_(height),
          end_us_(end_us) {}

    BinkNativeVideoStream stream_;
    std::uint32_t width_{},height_{},published_frame_{};
    std::uint64_t end_us_{},origin_us_{},last_now_us_{};
    bool started_{},pending_{},completed_{};
};
} // namespace btb::bink32_recovered
