#include "btb/bink32_native_movie_player.hpp"
#include "btb/bink32_container.hpp"
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace btb::bink32_recovered {
std::optional<BinkNativeSilentMovie> BinkNativeSilentMovie::open_bytes(
    std::vector<std::uint8_t> original_bik,std::string& error) {
    // Inspect the original header BEFORE moving ownership into the stream.
    const auto index=parse_container_index(original_bik);
    if(!index || index->header.magic!=Magic::SJPi) {
        error="not a supported, complete SJPi Bink movie";
        return std::nullopt;
    }
    // No silent degradation: even a track whose first packet has zero
    // compressed bytes may become audible later. Refuse all track-bearing
    // streams until native Bink audio is implemented and certified.
    if(!index->tracks.empty()) {
        error="native Bink compressed audio is not decoded; "+
              std::to_string(index->tracks.size())+
              " audio track(s) require the original decoder";
        return std::nullopt;
    }
    // BTB original source sizes are 640x480 or 282x179; reject large or
    // misleading geometry before letting the video codec allocate frames.
    if(!index->header.width||!index->header.height||
       index->header.width>640||index->header.height>480) {
        error="native Bink movie exceeds supported BTB frame geometry";
        return std::nullopt;
    }
    auto stream=BinkNativeVideoStream::open(std::move(original_bik));
    if(!stream || !stream->frame_count() ||
       !stream->frame_rate_numerator() ||
       !stream->frame_rate_denominator()) {
        error="invalid native Bink movie frame index or rate";
        return std::nullopt;
    }
    // End-of-show clock = frame_count * source timebase, not the timestamp
    // of frame_count (which would cut the last image's lifetime short).
    const std::uint64_t ticks=std::uint64_t(stream->frame_count())*
                              stream->frame_rate_denominator();
    constexpr std::uint64_t million=1'000'000;
    if(ticks>std::numeric_limits<std::uint64_t>::max()/million) {
        error="native Bink movie duration would overflow clock";
        return std::nullopt;
    }
    const std::uint64_t end_us=
        (ticks*million)/stream->frame_rate_numerator();
    if(end_us==0) {
        error="native Bink movie has invalid zero-length frame timing";
        return std::nullopt;
    }
    auto width=index->header.width,height=index->header.height;
    error.clear();
    return BinkNativeSilentMovie(
        std::move(*stream),width,height,end_us);
}

std::optional<BinkNativeSilentMovie> BinkNativeSilentMovie::open_file(
    const std::filesystem::path& file,std::string& error) {
    std::ifstream input(file,std::ios::binary|std::ios::ate);
    if(!input) {error="cannot open original Bink movie";return std::nullopt;}
    const auto size=input.tellg();
    constexpr std::streamoff max_bytes=100'000'000;
    if(size<=0||size>max_bytes) {
        error="original Bink movie exceeds bounded 100 MB input limit";
        return std::nullopt;
    }
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    input.seekg(0,std::ios::beg);
    if(!input.read(reinterpret_cast<char*>(bytes.data()),
                   static_cast<std::streamsize>(bytes.size()))) {
        error="cannot read complete original Bink movie";
        return std::nullopt;
    }
    return open_bytes(std::move(bytes),error);
}

bool BinkNativeSilentMovie::service(
    std::uint64_t now_us,const Presenter& present,
    bool& finished,std::string& error) {
    finished=false;
    if(!present) {error="native Bink has no real pixel presenter";return false;}
    if(completed_) {finished=true;error.clear();return true;}
    if(!started_) {
        started_=true;
        origin_us_=now_us;
    } else if(now_us<last_now_us_) {
        error="native Bink host clock moved backward";
        return false;
    }
    last_now_us_=now_us;
    const std::uint64_t elapsed=now_us-origin_us_;

    // Present a decoded image that failed surface upload before decoding
    // any later frame. This preserves the original frame on device loss.
    if(!pending_) {
        std::uint32_t decoded{};
        while(!stream_.at_end() && decoded<4) {
            const auto due=stream_.frame_timestamp_us(
                stream_.frame_number()+1);
            if(!due) {
                error="invalid native Bink frame presentation timestamp";
                return false;
            }
            if(elapsed<*due)break;
            if(!stream_.decode_next()) {
                error="native Bink compressed video frame decode failed";
                return false;
            }
            ++decoded;
        }
        pending_=decoded!=0;
    }
    if(pending_) {
        const auto* frame=stream_.current_frame();
        if(!frame||!present(*frame,error)) {
            if(error.empty())error="native Bink DirectDraw pixel upload failed";
            return false;
        }
        published_frame_=stream_.frame_number();
        pending_=false;
    }
    // A prolonged host stall may pass EOS, but never finish while any
    // undecoded frames remain or terminal pixels are unpresented.
    if(stream_.at_end() && published_frame_==stream_.frame_count() &&
       elapsed>=end_us_) {
        completed_=true;
        finished=true;
    }
    error.clear();
    return true;
}

void BinkNativeSilentMovie::rewind() noexcept {
    stream_.rewind();
    published_frame_=0;
    origin_us_=last_now_us_=0;
    started_=pending_=completed_=false;
}
} // namespace btb::bink32_recovered
