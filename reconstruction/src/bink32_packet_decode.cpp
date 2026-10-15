#include "btb/bink32_packet_decode.hpp"
#include <cstdint>

namespace btb::bink32_recovered {
namespace {
std::uint32_t little32(std::span<const std::uint8_t> bytes,
                       std::size_t p) noexcept {
    return std::uint32_t(bytes[p])|
           (std::uint32_t(bytes[p+1])<<8)|
           (std::uint32_t(bytes[p+2])<<16)|
           (std::uint32_t(bytes[p+3])<<24);
}
}
std::optional<EncodedFrame> split_encoded_frame(
    const ContainerIndex& index, std::span<const std::uint8_t> source,
    std::uint32_t number) noexcept {
    const auto packet=index.frame(number);
    if(!packet || packet->offset>source.size() ||
       packet->compressed_bytes>source.size()-packet->offset)
        return std::nullopt;
    auto whole=source.subspan(packet->offset,packet->compressed_bytes);
    EncodedFrame result{};
    result.frame=*packet;
    result.audio.reserve(index.tracks.size());
    std::size_t p=0;
    for(std::size_t i=0;i<index.tracks.size();++i) {
        if(whole.size()-p<4)return std::nullopt;
        const auto payload_size=little32(whole,p);
        p+=4;
        if(payload_size>whole.size()-p)return std::nullopt;
        result.audio.push_back({static_cast<std::uint32_t>(i),
                                whole.subspan(p,payload_size)});
        p+=payload_size;
    }
    result.video=whole.subspan(p);
    if(result.video.empty())return std::nullopt;
    return result;
}
std::optional<std::uint32_t> LsbBitReader::peek(unsigned width) const noexcept {
    if(width>32 || width>bits_remaining())return std::nullopt;
    std::uint32_t result=0;
    for(unsigned i=0;i<width;++i) {
        const auto bit=pos_+i;
        const auto value=(bytes_[bit/8]>>(bit%8))&1U;
        result|=std::uint32_t(value)<<i;
    }
    return result;
}
std::optional<std::uint32_t> LsbBitReader::read(unsigned width) noexcept {
    const auto v=peek(width);
    if(v)pos_+=width;
    return v;
}
bool LsbBitReader::skip(std::size_t count) noexcept {
    if(count>bits_remaining())return false;
    pos_+=count;
    return true;
}
bool LsbBitReader::align(unsigned bytes) noexcept {
    if(!bytes||(bytes&(bytes-1)))return false;
    const auto multiple=std::size_t(bytes)*8;
    const auto remainder=pos_%multiple;
    return remainder==0 || skip(multiple-remainder);
}
} // namespace btb::bink32_recovered
