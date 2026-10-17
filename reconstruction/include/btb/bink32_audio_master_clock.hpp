#pragma once
#include <algorithm>
#include <cstdint>
#include <limits>
#include <optional>

namespace btb::bink32_recovered {
// Portable audio-device-master video timebase. DirectSound reports positions
// in sample frames; this adapter maintains monotonic timestamps even if a
// driver briefly reports an older cursor or resets it on audio completion.
// Once audio finishes, extend the movie clock with host monotonic time so
// terminal video frames still complete their original presentation interval.
class Bink1AudioMasterClock final {
public:
    explicit Bink1AudioMasterClock(std::uint32_t rate,
                                   std::uint64_t total_frames) noexcept
        :rate_(rate),total_frames_(total_frames) {}
    [[nodiscard]] std::optional<std::uint64_t> observe(
        std::uint64_t now_us,std::uint64_t played_frames,
        bool completed) noexcept {
        if(rate_<1000 || rate_>192000 || !total_frames_ ||
           (observed_ && now_us<last_wall_us_))
            return std::nullopt;
        const auto bounded=std::min(played_frames,total_frames_);
        const std::uint64_t sample_us=(bounded/rate_)*1'000'000ULL+
                         ((bounded%rate_)*1'000'000ULL)/rate_;
        if(!observed_) {
            observed_=true;
            first_sample_us_=sample_us;
        }
        if(!audio_completed_) {
            const auto progressed=sample_us>=first_sample_us_
                ? sample_us-first_sample_us_ : std::uint64_t{0};
            position_us_=std::max(position_us_,progressed);
            if(completed) {
                audio_completed_=true;
                audio_finished_at_wall_us_=now_us;
                audio_end_position_us_=position_us_;
            }
        } else {
            const auto elapsed=now_us-audio_finished_at_wall_us_;
            if(elapsed>std::numeric_limits<std::uint64_t>::max()-
                        audio_end_position_us_)
                return std::nullopt;
            position_us_=audio_end_position_us_+elapsed;
        }
        last_wall_us_=now_us;
        return position_us_;
    }
    void rewind() noexcept {
        observed_=audio_completed_=false;
        first_sample_us_=position_us_=last_wall_us_=0;
        audio_finished_at_wall_us_=audio_end_position_us_=0;
    }
private:
    std::uint32_t rate_{};
    std::uint64_t total_frames_{};
    bool observed_{},audio_completed_{};
    std::uint64_t first_sample_us_{},position_us_{},last_wall_us_{};
    std::uint64_t audio_finished_at_wall_us_{},audio_end_position_us_{};
};
} // namespace btb::bink32_recovered
