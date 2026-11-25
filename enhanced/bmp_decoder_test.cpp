#include "btb/bmp_decoder.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
using namespace btb::display;
namespace {
void put16(std::vector<std::uint8_t>& b,std::size_t p,std::uint16_t v){
    b[p]=std::uint8_t(v);b[p+1]=std::uint8_t(v>>8);
}
void put32(std::vector<std::uint8_t>& b,std::size_t p,std::uint32_t v){
    for(unsigned i=0;i<4;++i)b[p+i]=std::uint8_t(v>>(8*i));
}
std::vector<std::uint8_t> make(std::uint32_t w,std::int32_t h,std::uint16_t bpp,
    std::uint32_t compression,std::vector<std::uint8_t> palette,
    std::vector<std::uint8_t> pixels,std::vector<std::uint32_t> masks={},
    std::uint32_t colors=0){
    const auto offset=14+40+masks.size()*4+palette.size();
    std::vector<std::uint8_t> b(offset+pixels.size());
    b[0]='B';b[1]='M';put32(b,2,std::uint32_t(b.size()));put32(b,10,std::uint32_t(offset));
    put32(b,14,40);put32(b,18,w);put32(b,22,std::uint32_t(h));
    put16(b,26,1);put16(b,28,bpp);put32(b,30,compression);
    put32(b,34,std::uint32_t(pixels.size()));put32(b,46,colors);
    std::size_t p=54;for(auto m:masks){put32(b,p,m);p+=4;}
    std::copy(palette.begin(),palette.end(),b.begin()+p);
    std::copy(pixels.begin(),pixels.end(),b.begin()+offset);
    return b;
}
std::vector<std::uint8_t> pal(std::initializer_list<std::array<std::uint8_t,3>> rgb){
    std::vector<std::uint8_t> out;
    for(auto c:rgb){out.push_back(c[2]);out.push_back(c[1]);out.push_back(c[0]);out.push_back(0);}
    return out;
}
bool pixel(const DecodedBitmap& b,unsigned x,unsigned y,
           unsigned r,unsigned g,unsigned bl,unsigned a=255){
    if(x>=b.width||y>=b.height)return false;
    const auto* p=b.bgra.data()+4*(y*b.width+x);
    return p[2]==r&&p[1]==g&&p[0]==bl&&p[3]==a;
}
int fail(const char* name,const std::string& detail={}) {
    std::cerr<<"FAIL "<<name<<": "<<detail<<"\n";return 1;
}
}
int main(){
    DecodedBitmap out;std::string error;
    // 24-bit bottom-up rows, including DWORD padding and vertical reversal.
    auto b24=make(2,2,24,0,{},{
        255,0,0, 0,255,0, 0,0,       // bottom: blue, green
        0,0,255, 255,255,255, 0,0    // top: red, white
    });
    if(!decode_bmp(b24,out,error)||out.width!=2||out.height!=2||
       !pixel(out,0,0,255,0,0)||!pixel(out,1,0,255,255,255)||
       !pixel(out,0,1,0,0,255)||!pixel(out,1,1,0,255,0))return fail("24-bit bottom-up",error);
    // Negative-height top-down BMP.
    auto top=make(1,-2,24,0,{},{
        0,0,255,0, // top red
        255,0,0,0  // bottom blue
    });
    if(!decode_bmp(top,out,error)||!pixel(out,0,0,255,0,0)||!pixel(out,0,1,0,0,255))
        return fail("24-bit top-down",error);
    // Indexed 8-bit palette.
    const auto p4=pal({{0,0,0},{255,0,0},{0,255,0},{0,0,255}});
    auto b8=make(3,1,8,0,p4,{1,2,3,0});
    if(!decode_bmp(b8,out,error)||!pixel(out,0,0,255,0,0)||
       !pixel(out,1,0,0,255,0)||!pixel(out,2,0,0,0,255))return fail("8-bit palette",error);
    // Packed high/low nibbles in 4-bit indexed data.
    auto b4=make(3,1,4,0,p4,{0x12,0x30,0,0});
    if(!decode_bmp(b4,out,error)||!pixel(out,0,0,255,0,0)||
       !pixel(out,1,0,0,255,0)||!pixel(out,2,0,0,0,255))return fail("4-bit palette",error);
    // MSB-first 1-bit palette data.
    const auto p2=pal({{0,0,0},{255,0,255}});
    auto b1=make(3,1,1,0,p2,{0xa0,0,0,0});
    if(!decode_bmp(b1,out,error)||!pixel(out,0,0,255,0,255)||
       !pixel(out,1,0,0,0,0)||!pixel(out,2,0,255,0,255))return fail("1-bit palette",error);
    // Default 16-bit RGB555 and explicit 16-bit RGB565 masks.
    auto b555=make(1,1,16,0,{}, {0x00,0x7c,0,0});
    if(!decode_bmp(b555,out,error)||!pixel(out,0,0,255,0,0))return fail("16-bit RGB555",error);
    auto b565=make(1,1,16,3,{}, {0xe0,0x07,0,0},{0xf800,0x07e0,0x001f});
    if(!decode_bmp(b565,out,error)||!pixel(out,0,0,0,255,0))return fail("16-bit RGB565",error);
    // BI_RGB's fourth byte is reserved and must not make UI sprites transparent.
    auto b32=make(1,1,32,0,{}, {0x33,0x22,0x11,0x00});
    if(!decode_bmp(b32,out,error)||!pixel(out,0,0,0x11,0x22,0x33,255))return fail("32-bit BI_RGB opaque",error);
    auto ba=make(1,1,32,6,{}, {0x33,0x22,0x11,0x80},
        {0x00ff0000,0x0000ff00,0x000000ff,0xff000000});
    if(!decode_bmp(ba,out,error)||!pixel(out,0,0,0x11,0x22,0x33,0x80))return fail("32-bit alpha bitfields",error);
    // RLE8 encoded run, EOL, then absolute palette indices.
    auto rle8=make(4,2,8,1,p4,{4,1,0,0, 0,4,0,1,2,3,0,1});
    if(!decode_bmp(rle8,out,error)||!pixel(out,0,0,0,0,0)||
       !pixel(out,0,1,255,0,0)||!pixel(out,3,1,255,0,0))return fail("RLE8",error);
    // RLE4 encoded alternating nibbles, including an odd final pixel.
    auto rle4=make(5,1,4,2,p4,{5,0x12,0,1});
    if(!decode_bmp(rle4,out,error)||!pixel(out,0,0,255,0,0)||
       !pixel(out,1,0,0,255,0)||!pixel(out,2,0,255,0,0)||
       !pixel(out,3,0,0,255,0)||!pixel(out,4,0,255,0,0))return fail("RLE4",error);
    // RLE delta leaves untouched pixels at palette index zero.
    auto delta=make(4,2,8,1,p4,{1,1,0,2,2,0,1,2,0,1});
    if(!decode_bmp(delta,out,error)||!pixel(out,0,1,255,0,0)||
       !pixel(out,1,1,0,0,0)||!pixel(out,2,1,0,255,0))return fail("RLE8 delta",error);
    // OS/2 CORE header, 24-bit pixel row.
    std::vector<std::uint8_t> core(14+12+4);
    core[0]='B';core[1]='M';put32(core,2,std::uint32_t(core.size()));put32(core,10,26);
    put32(core,14,12);put16(core,18,1);put16(core,20,1);put16(core,22,1);put16(core,24,24);
    core[26]=0;core[27]=0;core[28]=255;core[29]=0;
    if(!decode_bmp(core,out,error)||!pixel(out,0,0,255,0,0))return fail("OS/2 CORE",error);
    // Reject truncated data, bad palettes, malformed masks and unsupported compression.
    auto truncated=b24;truncated.pop_back();
    if(decode_bmp(truncated,out,error)||out.width)return fail("truncated BMP accepted");
    auto bad_palette=make(1,1,8,0,p4,{9,0,0,0});
    if(decode_bmp(bad_palette,out,error))return fail("out-of-range palette accepted");
    auto bad_masks=make(1,1,16,3,{}, {0,0,0,0},{0xf800,0xf800,0x001f});
    if(decode_bmp(bad_masks,out,error))return fail("overlapping masks accepted");
    auto bad_compression=make(1,1,24,4,{}, {0,0,0,0});
    if(decode_bmp(bad_compression,out,error))return fail("unsupported compression accepted");
    auto bad_offset=b24;put32(bad_offset,10,0xfffffff0u);
    if(decode_bmp(bad_offset,out,error))return fail("invalid pixel offset accepted");
    std::cout<<"PASS: BMP formats 1/4/8/16/24/32-bit, top-down, palettes, RGB555/565, alpha masks, RLE4/RLE8, OS/2 CORE, malformed-input guards\n";
    return 0;
}
