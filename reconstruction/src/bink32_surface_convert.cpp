#include "btb/bink32_surface_convert.hpp"
#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace btb::bink32_recovered {
namespace {
struct Channel {
    std::uint32_t mask{};
    unsigned shift{},bits{};
};
[[nodiscard]] bool channel(std::uint32_t mask,Channel& out) noexcept {
    if(!mask)return false;
    const unsigned shift=std::countr_zero(mask);
    const std::uint32_t field=mask>>shift;
    const unsigned bits=std::popcount(field);
    if(bits==0||bits>8||field!=((1U<<bits)-1U))return false;
    out={mask,shift,bits};
    return true;
}
[[nodiscard]] int clip(int value) noexcept {
    return std::clamp(value,0,255);
}
[[nodiscard]] int unpack(std::uint32_t px,const Channel& c) noexcept {
    const auto value=(px&c.mask)>>c.shift;
    const unsigned max=(1U<<c.bits)-1U;
    return static_cast<int>((value*255U+max/2U)/max);
}
[[nodiscard]] std::uint32_t pack(int value,const Channel& c) noexcept {
    const unsigned max=(1U<<c.bits)-1U;
    return ((static_cast<std::uint32_t>(value)*max+127U)/255U)<<c.shift;
}
[[nodiscard]] std::uint32_t load(const std::uint8_t* px,unsigned n) noexcept {
    std::uint32_t value{};
    for(unsigned i=0;i<n;++i)value|=std::uint32_t(px[i])<<(8*i);
    return value;
}
void store(std::uint8_t* px,unsigned n,std::uint32_t value) noexcept {
    for(unsigned i=0;i<n;++i)px[i]=static_cast<std::uint8_t>(value>>(8*i));
}
} // namespace

BinkSurfaceResult blit_bink_yuva_to_surface(
    const BinkNativeYuVaFrame& src,BinkPackedSurface dst) noexcept {
    if(!src.width||!src.height || src.width>16384||src.height>16384 ||
       src.width>std::numeric_limits<std::size_t>::max()/src.height)
        return BinkSurfaceResult::invalid_source;
    const std::size_t full=src.width*src.height;
    const std::size_t cw=(src.width+1)/2,ch=(src.height+1)/2;
    const std::size_t chroma=cw*ch;
    if(src.planes[0].size()!=full || src.planes[1].size()!=chroma ||
       src.planes[2].size()!=chroma || src.planes[3].size()!=full)
        return BinkSurfaceResult::invalid_source;
    if(dst.width!=src.width||dst.height!=src.height ||
       (dst.bytes_per_pixel!=2&&dst.bytes_per_pixel!=4) ||
       dst.row_pitch<dst.width*dst.bytes_per_pixel ||
       dst.row_pitch>std::numeric_limits<std::size_t>::max()/dst.height ||
       dst.bytes.size()<((dst.height-1)*dst.row_pitch+
                         dst.width*dst.bytes_per_pixel))
        return BinkSurfaceResult::invalid_surface;
    const auto rgb=dst.red_mask|dst.green_mask|dst.blue_mask;
    if((dst.red_mask&dst.green_mask)!=0 ||
       (dst.red_mask&dst.blue_mask)!=0 ||
       (dst.green_mask&dst.blue_mask)!=0 ||
       (dst.bytes_per_pixel==2 && (rgb&0xffff0000U)))
        return BinkSurfaceResult::unsupported_pixel_format;
    Channel r{},g{},b{};
    if(!channel(dst.red_mask,r)||!channel(dst.green_mask,g)||
       !channel(dst.blue_mask,b))
        return BinkSurfaceResult::unsupported_pixel_format;

    for(std::size_t y=0;y<src.height;++y) {
        auto* row=dst.bytes.data()+y*dst.row_pitch;
        for(std::size_t x=0;x<src.width;++x) {
            const std::size_t luma=y*src.width+x;
            const std::size_t chroma_index=(y/2)*cw+x/2;
            const int alpha=src.planes[3][luma];
            if(alpha==0)continue;
            const int yy=std::max(0,int(src.planes[0][luma])-16);
            const int uu=int(src.planes[1][chroma_index])-128;
            const int vv=int(src.planes[2][chroma_index])-128;
            int red=clip((298*yy+409*vv+128)>>8);
            int green=clip((298*yy-100*uu-208*vv+128)>>8);
            int blue=clip((298*yy+516*uu+128)>>8);
            auto* pixel=row+x*dst.bytes_per_pixel;
            const auto old=load(pixel,dst.bytes_per_pixel);
            if(alpha<255) {
                red=(alpha*red+(255-alpha)*unpack(old,r)+127)/255;
                green=(alpha*green+(255-alpha)*unpack(old,g)+127)/255;
                blue=(alpha*blue+(255-alpha)*unpack(old,b)+127)/255;
            }
            const std::uint32_t packed=(old&~rgb)|pack(red,r)|pack(green,g)|pack(blue,b);
            store(pixel,dst.bytes_per_pixel,packed);
        }
    }
    return BinkSurfaceResult::ok;
}
} // namespace btb::bink32_recovered
