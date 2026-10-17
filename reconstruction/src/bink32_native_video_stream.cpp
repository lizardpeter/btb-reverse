#include "btb/bink32_native_video_stream.hpp"
#include <cstdint>
#include <limits>
#include <optional>
#include <utility>
#include <vector>

namespace btb::bink32_recovered {
std::optional<BinkNativeVideoStream> BinkNativeVideoStream::open(
    std::vector<std::uint8_t> original_bik) {
    const auto index=parse_container_index(original_bik);
    if(!index || index->header.magic!=Magic::SJPi)
        return std::nullopt;
    return BinkNativeVideoStream(
        std::move(original_bik),std::move(*index));
}

bool BinkNativeVideoStream::decode_next() {
    if(at_end())return false;
    const auto number=frame_number_+1;
    const auto packet=encoded_frame(number);
    if(!packet)return false;
    // Do not advance the stream or publish partial video on a
    // malformed bitstream. The previous frame stays usable.
    const auto next=decode_bink_sjpi_video_frame(
        packet->video,index_.header.width,index_.header.height,
        has_encoded_alpha(),current_?&*current_:nullptr);
    if(!next)return false;
    current_=std::move(*next);
    frame_number_=number;
    return true;
}

bool BinkNativeVideoStream::seek(std::uint32_t one_based_frame) {
    if(!one_based_frame ||
       one_based_frame>index_.header.frame_count)
        return false;
    if(one_based_frame==frame_number_)
        return true;
    // Bink interframes depend on previous reconstructed Y/U/V/A
    // and invisible padded pixels. Seeking is strictly decoded from
    // the beginning (or an already verified earlier frame), never
    // done by blindly interpreting a delta frame as a key frame.
    const bool forward=one_based_frame>frame_number_;
    std::optional<BinkNativeYuVaFrame> probe=
        forward?current_:std::nullopt;
    const auto start=forward?frame_number_+1:1;
    for(auto number=start;number<=one_based_frame;++number) {
        const auto packet=encoded_frame(number);
        if(!packet)return false;
        auto frame=decode_bink_sjpi_video_frame(
            packet->video,index_.header.width,index_.header.height,
            has_encoded_alpha(),probe?&*probe:nullptr);
        if(!frame)return false;
        probe=std::move(*frame);
    }
    // Commit only when the complete seek chain decodes.
    current_=std::move(probe);
    frame_number_=one_based_frame;
    return true;
}

void BinkNativeVideoStream::rewind() noexcept {
    current_.reset();
    frame_number_=0;
}

std::optional<std::uint64_t>
BinkNativeVideoStream::frame_timestamp_us(
    std::uint32_t one_based_frame) const noexcept {
    if(!one_based_frame ||
       one_based_frame>index_.header.frame_count ||
       !index_.header.frame_rate)
        return std::nullopt;
    const auto ticks=std::uint64_t(one_based_frame-1)*
                     index_.header.divisor;
    constexpr auto one_million=std::uint64_t(1000000);
    if(ticks>std::numeric_limits<std::uint64_t>::max()/
             one_million)return std::nullopt;
    return (ticks*one_million)/index_.header.frame_rate;
}
} // namespace btb::bink32_recovered
