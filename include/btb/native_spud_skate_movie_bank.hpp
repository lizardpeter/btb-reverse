#pragma once
#include "btb/full_game_spud_skate.hpp"
#include "btb/bink32_native_video_stream.hpp"
#include <functional>

namespace btb::full_game {
// DLL-free five-stream decoder shared by Windows and installed-data probes.
class NativeSpudSkateMovieBank final : public SpudSkateMovieSource {
public:
    using Frame=btb::bink32_recovered::BinkNativeYuVaFrame;
    using Pace=std::function<bool(std::uint64_t,std::string&)>;
    using Present=std::function<bool(std::size_t,const Frame&,std::string&)>;
    explicit NativeSpudSkateMovieBank(Pace pace={},Present present={})
        : pace_(std::move(pace)),present_(std::move(present)) {}
    bool open(const std::array<std::filesystem::path,5>&,std::string&) override;
    bool observe(SpudSkateMovieFrame&,std::string&) override;
    bool select_quality(spud_skate::StuntQuality,std::string&) override;
    bool resynchronize_all(int,std::string&) override;
    bool begin_end_movie(std::string&) override;
    bool close(std::string&) override;
    [[nodiscard]] std::optional<std::size_t> displayed_stream() const noexcept {return displayed_;}
    [[nodiscard]] std::uint32_t decoded_frame(std::size_t stream) const noexcept {
        return stream<streams_.size()&&streams_[stream]?streams_[stream]->frame_number():0;
    }
private:
    bool pace(std::size_t,std::uint32_t,std::string&);
    bool publish(std::size_t,std::string&);
    bool decode(std::size_t,std::string&);
    bool fail(std::string&,const char*);
    std::array<std::optional<bink32_recovered::BinkNativeVideoStream>,5> streams_{};
    Pace pace_;
    Present present_;
    std::optional<std::size_t> displayed_{};
    std::size_t selected_{};
    bool active_{},end_movie_{},faulted_{};
};
}
