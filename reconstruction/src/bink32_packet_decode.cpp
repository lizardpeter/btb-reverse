#include "btb/bink32_packet_decode.hpp"
#include <cstdint>
#include <bit>
#include <algorithm>

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

 // Real original 0x3000FC04..0x3000FC68 computes the entropy lookup width
 // from ((plane_width >> 3) * multiplier + base - 1), clamped at 15 bits.
 // Original 0x3000FC76..0x3000FCA6 resets state and optional prefix mask.
 EntropyPlaneSetup init_entropy_plane(
     std::uintptr_t table_address, std::uint32_t symbol_base,
     std::uint32_t plane_width, std::uint32_t symbol_bits,
     std::uint32_t plane_multiplier, bool install_prefix_mask) noexcept {
     const auto count=std::uint64_t(symbol_base)+
         std::uint64_t(plane_width>>3)*plane_multiplier;
     const auto width=std::bit_width(std::min<std::uint64_t>(
         count?count-1:0,0x7fffU));
     EntropyPlaneSetup out{};
     out.symbol_bits=symbol_bits;
     out.table_bit_width=width;
     out.lookup_table_address=table_address;
     if(install_prefix_mask && symbol_bits>=1 && symbol_bits<=32)
         out.prefix_mask=std::uint32_t(1)<<(symbol_bits-1);
     return out;
 }


std::optional<EntropyTreePrefix> read_entropy_tree_prefix(
    LsbBitReader& source) noexcept {
    // Temporary bit cursor ensures malformed headers do not partially
    // consume a compressed packet.
    auto probe=source;
    const auto selector=probe.read(4);
    if(!selector)return std::nullopt;
    EntropyTreePrefix result{};
    result.selector=static_cast<std::uint8_t>(*selector);
    result.bits_consumed=4;
    if(!*selector) {
        source=probe;
        return result; // 0x3000FD32: all-zero 16-symbol table
    }
    const auto branch=probe.read(1);
    if(!branch)return std::nullopt;
    result.bits_consumed=5;
    if(*branch) {
        result.branch_mode=1;
        source=probe;
        return result; // branch to 0x3000FFE7
    }
    const auto mode=probe.read(2);
    if(!mode)return std::nullopt;
    result.branch_mode=2;
    result.submode=static_cast<std::uint8_t>(*mode);
    result.bits_consumed=7;
    source=probe;
    return result;
}

} // namespace btb::bink32_recovered
