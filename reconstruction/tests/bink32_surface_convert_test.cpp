#include "btb/bink32_surface_convert.hpp"
#include <algorithm>
#include <array>
#include <span>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace btb::bink32_recovered;
#define CHECK(e) do {if(!(e)){std::fprintf(stderr,"native Bink surface test %d: %s\n",__LINE__,#e);std::abort();}}while(0)
static BinkNativeYuVaFrame make_frame(std::size_t w,std::size_t h,
                                    std::uint8_t y,std::uint8_t u,
                                    std::uint8_t v,std::uint8_t a) {
    BinkNativeYuVaFrame f{};
    f.width=w;f.height=h;
    f.planes[0].assign(w*h,y);
    f.planes[1].assign(((w+1)/2)*((h+1)/2),u);
    f.planes[2].assign(((w+1)/2)*((h+1)/2),v);
    f.planes[3].assign(w*h,a);
    return f;
}
int main() {
    auto black=make_frame(3,3,16,128,128,255);
    std::vector<std::uint8_t> pixels(3*16,0xaa);
    BinkPackedSurface dst{pixels,16,3,3,4,0x00ff0000,0x0000ff00,0x000000ff};
    CHECK(blit_bink_yuva_to_surface(black,dst)==BinkSurfaceResult::ok);
    for(int row=0;row<3;++row)for(int x=0;x<3;++x) {
        auto* p=pixels.data()+row*16+x*4;
        CHECK(p[0]==0&&p[1]==0&&p[2]==0&&p[3]==0xaa);
    }
    for(int row=0;row<3;++row)for(int i=12;i<16;++i)
        CHECK(pixels[row*16+i]==0xaa); // never write pitch padding
    auto white=make_frame(3,3,235,128,128,255);
    CHECK(blit_bink_yuva_to_surface(white,dst)==BinkSurfaceResult::ok);
    for(int row=0;row<3;++row)for(int x=0;x<3;++x) {
        auto* p=pixels.data()+row*16+x*4;
        CHECK(p[0]==255&&p[1]==255&&p[2]==255&&p[3]==0xaa);
    }
    auto transparent=white;
    transparent.planes[3].assign(9,0);
    const auto saved=pixels;
    CHECK(blit_bink_yuva_to_surface(transparent,dst)==BinkSurfaceResult::ok);
    CHECK(pixels==saved);
    auto blend=white;
    blend.planes[3].assign(9,128);
    std::fill(pixels.begin(),pixels.end(),0);
    CHECK(blit_bink_yuva_to_surface(blend,dst)==BinkSurfaceResult::ok);
    CHECK(pixels[0]==128&&pixels[1]==128&&pixels[2]==128);
    CHECK(pixels[12]==0); // padding
    std::array<std::uint8_t,18> rgb565{};
    BinkPackedSurface dst16{rgb565,6,3,3,2,0xf800,0x07e0,0x001f};
    CHECK(blit_bink_yuva_to_surface(white,dst16)==BinkSurfaceResult::ok);
    for(auto p:rgb565) CHECK(p==0xff);
    // Explicit RGB555, reversed channel masks, and preservation of high bit.
    std::array<std::uint8_t,18> rgb555{};
    rgb555.fill(0x80);
    BinkPackedSurface dst555{rgb555,6,3,3,2,0x001f,0x03e0,0x7c00};
    CHECK(blit_bink_yuva_to_surface(black,dst555)==BinkSurfaceResult::ok);
    CHECK(rgb555[0]==0&&rgb555[1]==0x80);
    // On every invalid case, the destination remains unchanged.
    auto before=pixels;
    auto invalid=white;
    invalid.planes[1].pop_back();
    CHECK(blit_bink_yuva_to_surface(invalid,dst)==BinkSurfaceResult::invalid_source);
    CHECK(pixels==before);
    auto short_buffer=dst;
    short_buffer.bytes=std::span(pixels).first(42);
    CHECK(blit_bink_yuva_to_surface(white,short_buffer)==BinkSurfaceResult::invalid_surface);
    CHECK(pixels==before);
    auto overlapping=dst;
    overlapping.green_mask=overlapping.red_mask;
    CHECK(blit_bink_yuva_to_surface(white,overlapping)==BinkSurfaceResult::unsupported_pixel_format);
    CHECK(pixels==before);
    auto non_contiguous=dst;
    non_contiguous.red_mask=0x00a80000;
    CHECK(blit_bink_yuva_to_surface(white,non_contiguous)==BinkSurfaceResult::unsupported_pixel_format);
    CHECK(pixels==before);
    // Chroma subsampling indexes up and right correctly on odd size.
    auto colorful=white;
    colorful.planes[1][3]=255;
    std::fill(pixels.begin(),pixels.end(),0);
    CHECK(blit_bink_yuva_to_surface(colorful,dst)==BinkSurfaceResult::ok);
    CHECK(pixels[0]==255); // first cell neutral
    CHECK(pixels[2*16+2*4]==255); // blue-rich bottom right
    CHECK(pixels[2*16+2*4+1]<255);
    return 0;
}
