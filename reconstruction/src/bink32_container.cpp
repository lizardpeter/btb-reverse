#include "btb/bink32_container.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <utility>

namespace btb::bink32_recovered {
namespace {
std::uint32_t read32(std::span<const std::uint8_t> b,std::size_t i) noexcept {
    return std::uint32_t(b[i])|
        std::uint32_t(b[i+1])<<8|
        std::uint32_t(b[i+2])<<16|
        std::uint32_t(b[i+3])<<24;
}
constexpr std::size_t kHeaderBytes=44;
constexpr std::size_t kTrackBytes=12;
constexpr std::uint32_t kMaximumTracks=64;
constexpr std::uint32_t kMaximumFrames=1000000;
}

std::optional<FramePacket> ContainerIndex::frame(
    std::uint32_t one_based) const noexcept {
    if(one_based==0||one_based>header.frame_count||
       frame_offsets_flags.size()!=std::size_t(header.frame_count)+1)
        return std::nullopt;
    const std::size_t i=one_based-1;
    // Bink's frame-index low bit is the key-frame flag. Packet offsets
    // are aligned to four bytes in the verified retail video files.
    const auto start=frame_offsets_flags[i]&~3U;
    const auto end=frame_offsets_flags[i+1]&~3U;
    if(end<start)return std::nullopt;
    return FramePacket{one_based,start,end-start,
                       (frame_offsets_flags[i]&1U)!=0};
}

std::optional<ContainerIndex> parse_container_index(
    std::span<const std::uint8_t> b) noexcept {
    if(b.size()<kHeaderBytes||b.size()>std::numeric_limits<std::uint32_t>::max())
        return std::nullopt;
    const auto prefix=parse_header_prefix(b.first(kHeaderBytes));
    if(!prefix||prefix->frame_count>kMaximumFrames||
       prefix->width==0||prefix->height==0||
       prefix->frame_rate==0||prefix->divisor==0)
        return std::nullopt;

    const std::uint32_t reported_length=read32(b,4);
    if(reported_length!=b.size()-8)return std::nullopt;
    const auto track_count=read32(b,40);
    if(track_count>kMaximumTracks)return std::nullopt;
    const auto index_start=kHeaderBytes+std::size_t(track_count)*kTrackBytes;
    const auto count=std::size_t(prefix->frame_count)+1;
    if(index_start>b.size()||count>(b.size()-index_start)/4)
        return std::nullopt;
    const auto frame_data_start=index_start+count*4;

    ContainerIndex out;
    out.header=*prefix;
    out.declared_file_size_minus_8=reported_length;
    out.maximum_frame_bytes=read32(b,12);
    out.video_flags=read32(b,36);
    out.tracks.reserve(track_count);
    for(std::size_t i=0;i<track_count;++i) {
        const auto off=kHeaderBytes+i*kTrackBytes;
        out.tracks.push_back({read32(b,off),read32(b,off+4),read32(b,off+8)});
    }
    out.frame_offsets_flags.reserve(count);
    std::uint32_t prev=0;
    for(std::size_t i=0;i<count;++i) {
        const auto entry=read32(b,index_start+i*4);
        const auto offset=entry&~3U;
        if((entry&2U)!=0||offset>b.size()||
           (i==0&&offset<frame_data_start)||
           (i>0&&offset<prev))
            return std::nullopt;
        if(i==count-1 && (entry!=b.size()))
            return std::nullopt;
        out.frame_offsets_flags.push_back(entry);
        prev=offset;
    }
    return out;
}
} // namespace btb::bink32_recovered
