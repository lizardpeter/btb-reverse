#include "btb/bink32_yuva_frame.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <utility>
namespace btb::bink32_recovered {
std::optional<BinkNativeYuVaFrame> decode_bink_sjpi_video_frame(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,bool has_alpha,
    const BinkNativeYuVaFrame* previous) {
    constexpr std::size_t kMaxPlanePixels=64U*1024U*1024U;
    // Chroma in the original SJPi stream is 4:2:0. This exact
    // decoder currently requires integral 8x8 chroma tile dimensions.
    if(!width || !height || width>16384 || height>16384 ||
       (width%16)!=0 || (height%16)!=0 ||
       width>kMaxPlanePixels/height)return std::nullopt;
    const auto full_pixels=width*height;
    const auto chroma_pixels=full_pixels/4;
    if(previous) {
        if(previous->width!=width || previous->height!=height ||
           previous->source_has_alpha!=has_alpha)
            return std::nullopt;
        for(std::size_t plane=0;plane<4;++plane) {
            const auto expected=(plane==1||plane==2)
                ?chroma_pixels:full_pixels;
            if(previous->planes[plane].size()!=expected)
                return std::nullopt;
        }
    }
    if(video_packet.size()<4)return std::nullopt;
    const auto outer=has_alpha
        ?parse_alpha_picture_entropy_prologue(video_packet)
        :std::nullopt;
    if(has_alpha && !outer)return std::nullopt;
    const auto reference=[&](BinkYuvaPlane p)
        -> std::span<const std::uint8_t> {
        if(!previous)return {};
        return previous->planes[static_cast<std::size_t>(p)];
    };
    BinkNativeYuVaFrame output{};
    output.width=width;
    output.height=height;
    output.source_has_alpha=has_alpha;
    // SJPi with alpha: four-byte section length, encoded A plane,
    // another four-byte picture control, then Y/V/U. Without alpha:
    // one four-byte picture control, then Y/V/U. All offsets here
    // are within the unmodified compressed video packet.
    std::size_t first_y_tree_byte=4;
    if(has_alpha) {
        auto alpha=decode_bink_picture_plane_at(
            video_packet,4,width,height,reference(BinkYuvaPlane::A));
        if(!alpha ||
           alpha->next_plane_byte_offset!=outer->alpha_section_bytes)
            return std::nullopt;
        first_y_tree_byte=outer->picture.initial_byte_offset;
        output.planes[3]=std::move(alpha->image.pixels);
        output.plane_end_byte_offsets[3]=alpha->next_plane_byte_offset;
        output.mode_counts[3]=alpha->mode_counts;
    } else {
        // The absent A channel is explicitly opaque. It is not
        // evidence of an encoded alpha plane or alpha parity.
        output.planes[3].assign(full_pixels,255);
        output.plane_end_byte_offsets[3]=0;
    }
    auto y=decode_bink_picture_plane_at(
        video_packet,first_y_tree_byte,
        width,height,reference(BinkYuvaPlane::Y));
    if(!y)return std::nullopt;
    // Original SJPi versions h/i swap physical U/V stream order.
    // Plane order in the bitstream is A,Y,V,U, although the decoded
    // output must preserve canonical Y,U,V,A channel order.
    auto v=decode_bink_picture_plane_at(
        video_packet,y->next_plane_byte_offset,
        width/2,height/2,reference(BinkYuvaPlane::V));
    if(!v)return std::nullopt;
    auto u=decode_bink_picture_plane_at(
        video_packet,v->next_plane_byte_offset,
        width/2,height/2,reference(BinkYuvaPlane::U));
    if(!u || u->next_plane_byte_offset>video_packet.size())
        return std::nullopt;
    auto store=[&](BinkYuvaPlane index,BinkSimplePictureLuma& plane) {
        const auto n=static_cast<std::size_t>(index);
        output.planes[n]=std::move(plane.image.pixels);
        output.plane_end_byte_offsets[n]=plane.next_plane_byte_offset;
        output.mode_counts[n]=plane.mode_counts;
    };
    store(BinkYuvaPlane::Y,*y);
    store(BinkYuvaPlane::V,*v);
    store(BinkYuvaPlane::U,*u);
    output.video_bytes_consumed=u->next_plane_byte_offset;
    return output;
}

std::optional<BinkNativeYuVaFrame> decode_bink_sjpi_yuva_frame(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,
    const BinkNativeYuVaFrame* previous) {
    return decode_bink_sjpi_video_frame(
        video_packet,width,height,true,previous);
}

std::optional<BinkFrameParity> compare_bink_yuva_reference(
    const BinkNativeYuVaFrame& native,
    std::span<const std::uint8_t> packed_yuva420p) noexcept {
    constexpr std::size_t limit=64U*1024U*1024U;
    if(!native.width || !native.height ||
       native.width>limit/native.height ||
       (native.width%2) || (native.height%2))
        return std::nullopt;
    const auto full=native.width*native.height;
    const auto quarter=full/4;
    if(packed_yuva420p.size()!=2*full+2*quarter)
        return std::nullopt;
    BinkFrameParity result{};
    std::size_t start=0;
    for(std::size_t plane=0;plane<4;++plane) {
        const auto expected=(plane==1 || plane==2)?quarter:full;
        const auto& actual=native.planes[plane];
        if(actual.size()!=expected)
            return std::nullopt;
        auto& stats=result.planes[plane];
        stats.compared_bytes=expected;
        stats.first_different_index=expected;
        for(std::size_t i=0;i<expected;++i) {
            const auto reference=packed_yuva420p[start+i];
            const auto value=actual[i];
            if(value==reference)continue;
            if(stats.different_bytes==0) {
                stats.first_different_index=i;
                stats.first_native=value;
                stats.first_reference=reference;
            }
            ++stats.different_bytes;
            const auto distance=std::max(value,reference)-
                std::min(value,reference);
            stats.maximum_absolute_difference=
                std::max<std::uint8_t>(
                    stats.maximum_absolute_difference,distance);
            stats.sum_absolute_differences+=distance;
        }
        result.total_different_bytes+=stats.different_bytes;
        start+=expected;
    }
    result.identical=result.total_different_bytes==0;
    return result;
}
} // namespace btb::bink32_recovered
