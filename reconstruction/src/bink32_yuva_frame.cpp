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
    if(!width || !height || width>16384 || height>16384 ||
       width>kMaxPlanePixels/height || video_packet.size()<4)
        return std::nullopt;

    // Plane-visibility geometry follows YUV 4:2:0: the decoder
    // rounds odd chroma extents UP before 8x8 block alignment.
    // These padded margins must persist across interframes.
    const std::size_t cw=(width+1)/2,ch=(height+1)/2;
    const std::array<std::size_t,4> visible_w{
        width,cw,cw,width};
    const std::array<std::size_t,4> visible_h{
        height,ch,ch,height};
    std::array<std::size_t,4> padded_w{},padded_h{};
    for(std::size_t p=0;p<4;++p) {
        padded_w[p]=((visible_w[p]+7)/8)*8;
        padded_h[p]=((visible_h[p]+7)/8)*8;
        if(padded_w[p]>16384 || padded_h[p]>16384 ||
           padded_w[p]>kMaxPlanePixels/padded_h[p])
            return std::nullopt;
    }
    if(previous) {
        if(previous->width!=width || previous->height!=height ||
           previous->source_has_alpha!=has_alpha)
            return std::nullopt;
        for(std::size_t p=0;p<4;++p) {
            if(previous->planes[p].size()!=visible_w[p]*visible_h[p] ||
               previous->codec_plane_widths[p]!=padded_w[p] ||
               previous->codec_plane_heights[p]!=padded_h[p] ||
               previous->codec_planes[p].size()!=padded_w[p]*padded_h[p])
                return std::nullopt;
        }
    }
    const auto outer=has_alpha
        ?parse_alpha_picture_entropy_prologue(video_packet)
        :std::nullopt;
    if(has_alpha && !outer)return std::nullopt;
    const auto reference=[&](BinkYuvaPlane p)
        -> std::span<const std::uint8_t> {
        if(!previous)return {};
        return previous->codec_planes[static_cast<std::size_t>(p)];
    };
    BinkNativeYuVaFrame output{};
    output.width=width;
    output.height=height;
    output.source_has_alpha=has_alpha;
    output.codec_plane_widths=padded_w;
    output.codec_plane_heights=padded_h;

    const auto store=[&](BinkYuvaPlane index,
                         BinkSimplePictureLuma& decoded) -> bool {
        const auto p=static_cast<std::size_t>(index);
        if(decoded.image.pixels.size()!=padded_w[p]*padded_h[p])
            return false;
        output.codec_planes[p]=std::move(decoded.image.pixels);
        auto& cropped=output.planes[p];
        cropped.resize(visible_w[p]*visible_h[p]);
        for(std::size_t y=0;y<visible_h[p];++y)
            std::copy_n(
                output.codec_planes[p].begin()+
                    static_cast<std::ptrdiff_t>(y*padded_w[p]),
                visible_w[p],
                cropped.begin()+static_cast<std::ptrdiff_t>(
                    y*visible_w[p]));
        output.plane_end_byte_offsets[p]=decoded.next_plane_byte_offset;
        output.mode_counts[p]=decoded.mode_counts;
        return true;
    };

    std::size_t first_y_tree_byte=4;
    if(has_alpha) {
        auto alpha=decode_bink_picture_plane_at(
            video_packet,4,padded_w[3],padded_h[3],
            reference(BinkYuvaPlane::A));
        if(!alpha ||
           alpha->next_plane_byte_offset!=outer->alpha_section_bytes ||
           !store(BinkYuvaPlane::A,*alpha))
            return std::nullopt;
        first_y_tree_byte=outer->picture.initial_byte_offset;
    } else {
        // Missing alpha is an opaque presentation convention, not
        // a decoded fourth plane in the BIK. Keep padded alpha
        // opaque for consistency across all previous-frame copies.
        output.codec_planes[3].assign(padded_w[3]*padded_h[3],255);
        output.planes[3].assign(width*height,255);
        output.plane_end_byte_offsets[3]=0;
    }

    auto y=decode_bink_picture_plane_at(
        video_packet,first_y_tree_byte,padded_w[0],padded_h[0],
        reference(BinkYuvaPlane::Y));
    if(!y || !store(BinkYuvaPlane::Y,*y))
        return std::nullopt;
    // SJPi compressed physical order is Y,V,U, but output always
    // exposes Y,U,V,A and preserves the exact padded codec-domain
    // references needed by motion prediction for future frames.
    auto v=decode_bink_picture_plane_at(
        video_packet,y->next_plane_byte_offset,
        padded_w[2],padded_h[2],reference(BinkYuvaPlane::V));
    if(!v || !store(BinkYuvaPlane::V,*v))
        return std::nullopt;
    auto u=decode_bink_picture_plane_at(
        video_packet,v->next_plane_byte_offset,
        padded_w[1],padded_h[1],reference(BinkYuvaPlane::U));
    if(!u || u->next_plane_byte_offset>video_packet.size() ||
       !store(BinkYuvaPlane::U,*u))
        return std::nullopt;
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
