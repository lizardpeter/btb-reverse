#include "btb/bink32_video_planes.hpp"
#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

namespace btb::bink32_recovered {
namespace {
constexpr std::size_t kMaxPlaneBytes=64U*1024U*1024U;
[[nodiscard]] bool acceptable_size(std::size_t w,std::size_t h) noexcept {
    return w>0 && h>0 && w<=16384 && h<=16384 &&
           w<=kMaxPlaneBytes/h;
}
} // namespace
std::optional<BinkYuvFrame> compose_bink_yuv_planes(
    const BinkYuvDimensions& d,const BinkYuvCommands& cmd,
    const BinkYuvFrame* previous) {
    if(!acceptable_size(d.width,d.height) ||
       !acceptable_size(d.chroma_width,d.chroma_height))
        return std::nullopt;
    const auto y_len=d.width*d.height;
    const auto uv_len=d.chroma_width*d.chroma_height;
    if(uv_len>(kMaxPlaneBytes-y_len)/2)return std::nullopt;
    // With an existing frame, all three reference-plane dimensions must
    // agree, even if the current command list happens not to reference it.
    if(previous &&
       (previous->y.width!=d.width || previous->y.height!=d.height ||
        previous->u.width!=d.chroma_width ||
        previous->u.height!=d.chroma_height ||
        previous->v.width!=d.chroma_width ||
        previous->v.height!=d.chroma_height ||
        previous->y.pixels.size()!=y_len ||
        previous->u.pixels.size()!=uv_len ||
        previous->v.pixels.size()!=uv_len))return std::nullopt;
    const auto old_y=previous ? std::span<const std::uint8_t>(previous->y.pixels)
                              : std::span<const std::uint8_t>{};
    const auto old_u=previous ? std::span<const std::uint8_t>(previous->u.pixels)
                              : std::span<const std::uint8_t>{};
    const auto old_v=previous ? std::span<const std::uint8_t>(previous->v.pixels)
                              : std::span<const std::uint8_t>{};
    auto y=compose_bink_mode_plane(d.width,d.height,cmd.y,old_y);
    if(!y)return std::nullopt;
    auto u=compose_bink_mode_plane(d.chroma_width,d.chroma_height,cmd.u,old_u);
    if(!u)return std::nullopt;
    auto v=compose_bink_mode_plane(d.chroma_width,d.chroma_height,cmd.v,old_v);
    if(!v)return std::nullopt;
    return BinkYuvFrame{std::move(*y),std::move(*u),std::move(*v)};
}
std::optional<BinkYuvFrame> BinkYuvFrameHistory::advance(
    const BinkYuvCommands& commands,bool key_frame) {
    const BinkYuvFrame* reference=key_frame ? nullptr : previous();
    auto current=compose_bink_yuv_planes(dimensions_,commands,reference);
    if(!current)return std::nullopt;
    previous_=*current;
    return current;
}
} // namespace btb::bink32_recovered
