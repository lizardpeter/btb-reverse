#include "btb/bink32_yuva_frame.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <utility>
namespace btb::bink32_recovered {
std::optional<BinkNativeYuVaFrame> decode_bink_sjpi_yuva_frame(
    std::span<const std::uint8_t> video_packet,
    std::size_t width,std::size_t height,
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
        if(previous->width!=width || previous->height!=height)
            return std::nullopt;
        for(std::size_t plane=0;plane<4;++plane) {
            const auto expected=(plane==1||plane==2)
                ?chroma_pixels:full_pixels;
            if(previous->planes[plane].size()!=expected)
                return std::nullopt;
        }
    }
    const auto outer=parse_alpha_picture_entropy_prologue(video_packet);
    if(!outer)return std::nullopt;
    const auto reference=[&](BinkYuvaPlane p)
        -> std::span<const std::uint8_t> {
        if(!previous)return {};
        return previous->planes[static_cast<std::size_t>(p)];
    };
    BinkNativeYuVaFrame output{};
    output.width=width;
    output.height=height;
    // SJPi 'i' video packets: first DWORD is alpha-section length,
    // 23 alpha trees begin at byte 4. Reject spills across the
    // independently recorded alpha/picture section boundary.
    auto alpha=decode_bink_picture_plane_at(
        video_packet,4,width,height,reference(BinkYuvaPlane::A));
    if(!alpha ||
       alpha->next_plane_byte_offset!=outer->alpha_section_bytes)
        return std::nullopt;
    // The second DWORD starts the color-picture section. Its first
    // entropy trees begin immediately AFTER the control DWORD.
    auto y=decode_bink_picture_plane_at(
        video_packet,outer->picture.initial_byte_offset,
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
    store(BinkYuvaPlane::A,*alpha);
    store(BinkYuvaPlane::Y,*y);
    store(BinkYuvaPlane::V,*v);
    store(BinkYuvaPlane::U,*u);
    output.video_bytes_consumed=u->next_plane_byte_offset;
    return output;
}
} // namespace btb::bink32_recovered
