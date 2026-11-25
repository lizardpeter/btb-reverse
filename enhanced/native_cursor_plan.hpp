#pragma once
#include "cursor_runs.hpp"
#include "presentation_policy.hpp"
#include <vector>
namespace btb::enhanced {
struct CursorBitmap {
    std::vector<std::uint32_t> bgra;
    std::vector<std::uint8_t> mask;
};
struct CursorDimensions {unsigned width{},height{};};
constexpr CursorDimensions native_cursor_dimensions(Viewport v) noexcept {
    if(v.width<=0||v.height<=0)return {};
    return {unsigned(std::clamp<std::int64_t>((std::int64_t(v.width)*19+320)/640,1,256)),
            unsigned(std::clamp<std::int64_t>((std::int64_t(v.height)*19+240)/480,1,256))};
}
// Native alpha cursor, nearest-neighbor original artwork, top-down BGRA.
// Monochrome AND mask is bottom-up, WORD aligned, 1 = transparent.
inline bool make_cursor_bitmap(const CursorPixels& src,unsigned w,unsigned h,CursorBitmap& out){
    if(!w||!h||w>256||h>256)return false;
    const unsigned stride=((w+15)/16)*2;
    out.bgra.resize(std::size_t(w)*h);
    out.mask.assign(std::size_t(stride)*h,0);
    for(unsigned y=0;y<h;++y)for(unsigned x=0;x<w;++x){
        const auto c=src[(y*19/h)*19+x*19/w];
        const bool transparent=(c&0xffffffU)==0xff00ffU;
        out.bgra[y*w+x]=transparent?0:cursor_format(c,true);
        if(transparent)out.mask[(h-1-y)*stride+x/8]|=std::uint8_t(0x80U>>(x%8));
    }
    return true;
}
}
