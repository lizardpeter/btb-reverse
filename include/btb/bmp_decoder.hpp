#pragma once

// Deterministic, platform-independent BMP decoder for the enhanced renderer.
// Output is top-down BGRA8. Legacy transparency remains an explicit magenta
// color key in PackedScene; BI_RGB's reserved fourth byte is always opaque.
#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace btb::display {

struct DecodedBitmap {
    std::uint32_t width{}, height{};
    std::uint16_t bits_per_pixel{};
    std::uint32_t compression{};
    bool top_down{};
    std::vector<std::uint8_t> bgra;
};

namespace bmp_detail {
inline constexpr std::uint32_t kRgb=0, kRle8=1, kRle4=2,
    kBitfields=3, kAlphaBitfields=6;
inline constexpr std::size_t kMaxPixels=16u*1024u*1024u;
inline constexpr std::size_t kMaxFileBytes=256u*1024u*1024u;

inline bool range(std::size_t offset,std::size_t count,std::size_t limit) noexcept {
    return offset<=limit && count<=limit-offset;
}
inline std::uint16_t u16(std::span<const std::uint8_t> b,std::size_t p) noexcept {
    return std::uint16_t(b[p]) | (std::uint16_t(b[p+1])<<8);
}
inline std::uint32_t u32(std::span<const std::uint8_t> b,std::size_t p) noexcept {
    return std::uint32_t(b[p]) | (std::uint32_t(b[p+1])<<8) |
        (std::uint32_t(b[p+2])<<16) | (std::uint32_t(b[p+3])<<24);
}
inline std::int32_t i32(std::span<const std::uint8_t> b,std::size_t p) noexcept {
    return std::bit_cast<std::int32_t>(u32(b,p));
}
inline bool valid_mask(std::uint32_t mask,unsigned bits) noexcept {
    if(!mask || bits==0 || bits>32 || (bits<32 && (mask>>bits)!=0))return false;
    const unsigned shift=std::countr_zero(mask);
    const std::uint32_t field=mask>>shift;
    return (field & (field+1u))==0;
}
inline std::uint8_t component(std::uint32_t pixel,std::uint32_t mask) noexcept {
    if(!mask)return 255;
    const unsigned shift=std::countr_zero(mask);
    const unsigned bits=std::popcount(mask);
    const std::uint64_t max=bits==32?0xffffffffull:((std::uint64_t{1}<<bits)-1);
    const std::uint64_t value=(pixel&mask)>>shift;
    return static_cast<std::uint8_t>((value*255u+max/2u)/max);
}
} // namespace bmp_detail

// Decodes Windows BITMAPINFOHEADER-family and OS/2 CORE BMP files:
// indexed 1/4/8-bit, 16-bit RGB555 or BITFIELDS, 24/32-bit RGB,
// 16/32-bit BITFIELDS/ALPHABITFIELDS, and RLE4/RLE8. Unsupported
// compression, malformed masks, out-of-range palette references, and
// truncated rows/commands are rejected rather than partially rendered.
inline bool decode_bmp(std::span<const std::uint8_t> bytes,
                       DecodedBitmap& output,std::string& error) {
    using namespace bmp_detail;
    output={};
    const auto fail=[&](const char* why){error=why;output={};return false;};
    if(bytes.size()<26 || bytes[0]!='B' || bytes[1]!='M')
        return fail("not a complete BMP file");
    const std::uint32_t declared_size=u32(bytes,2);
    const std::size_t limit=declared_size?declared_size:bytes.size();
    if(limit>bytes.size() || limit>kMaxFileBytes || limit<26)
        return fail("invalid BMP file size");
    const std::uint32_t pixel_offset=u32(bytes,10);
    const std::uint32_t dib_size=u32(bytes,14);
    if(dib_size==12) {
        if(!range(14,12,limit))return fail("truncated OS/2 CORE header");
    } else if(dib_size<40 || !range(14,dib_size,limit)) {
        return fail("unsupported or truncated BMP DIB header");
    }

    std::int64_t width{},signed_height{};
    std::uint16_t planes{},bpp{};
    std::uint32_t compression{},image_size{},colors_used{};
    const bool core=dib_size==12;
    if(core) {
        width=u16(bytes,18);signed_height=u16(bytes,20);
        planes=u16(bytes,22);bpp=u16(bytes,24);
    } else {
        width=i32(bytes,18);signed_height=i32(bytes,22);
        planes=u16(bytes,26);bpp=u16(bytes,28);
        compression=u32(bytes,30);image_size=u32(bytes,34);
        colors_used=u32(bytes,46);
    }
    if(width<=0 || signed_height==0 || signed_height==std::numeric_limits<std::int32_t>::min() ||
       planes!=1 || width>8192 || signed_height>8192 || signed_height< -8192)
        return fail("invalid BMP dimensions or planes");
    const bool top_down=signed_height<0;
    const std::uint32_t w=static_cast<std::uint32_t>(width);
    const std::uint32_t h=static_cast<std::uint32_t>(top_down?-signed_height:signed_height);
    if(std::uint64_t(w)*h>kMaxPixels)return fail("BMP exceeds the decoded pixel budget");
    if(bpp!=1 && bpp!=4 && bpp!=8 && bpp!=16 && bpp!=24 && bpp!=32)
        return fail("unsupported BMP bit depth");
    if(compression!=kRgb && compression!=kRle8 && compression!=kRle4 &&
       compression!=kBitfields && compression!=kAlphaBitfields)
        return fail("unsupported BMP compression");
    if((compression==kRle8 && (bpp!=8 || top_down)) ||
       (compression==kRle4 && (bpp!=4 || top_down)) ||
       ((compression==kBitfields || compression==kAlphaBitfields) && bpp!=16 && bpp!=32))
        return fail("BMP compression does not match bit depth or orientation");

    std::uint32_t red_mask=0,green_mask=0,blue_mask=0,alpha_mask=0;
    std::size_t palette_offset=14+dib_size;
    if(compression==kBitfields || compression==kAlphaBitfields) {
        const bool embedded=dib_size>=52;
        if(embedded) {
            red_mask=u32(bytes,54);green_mask=u32(bytes,58);blue_mask=u32(bytes,62);
            if(dib_size>=56)alpha_mask=u32(bytes,66);
        } else {
            const std::size_t count=compression==kAlphaBitfields?4:3;
            if(!range(palette_offset,count*4,limit))return fail("truncated BMP channel masks");
            red_mask=u32(bytes,palette_offset);green_mask=u32(bytes,palette_offset+4);
            blue_mask=u32(bytes,palette_offset+8);
            if(count==4)alpha_mask=u32(bytes,palette_offset+12);
            palette_offset+=count*4;
        }
        if(compression==kAlphaBitfields && !alpha_mask)
            return fail("missing BMP alpha channel mask");
    } else if(bpp==16) {
        red_mask=0x7c00;green_mask=0x03e0;blue_mask=0x001f;
    } else if(bpp==32) {
        red_mask=0x00ff0000;green_mask=0x0000ff00;blue_mask=0x000000ff;
    }
    if(bpp==16 || bpp==32) {
        const unsigned mask_bits=bpp;
        if(!valid_mask(red_mask,mask_bits)||!valid_mask(green_mask,mask_bits)||
           !valid_mask(blue_mask,mask_bits)||
           (alpha_mask&&!valid_mask(alpha_mask,mask_bits))||
           (red_mask&green_mask)||(red_mask&blue_mask)||(green_mask&blue_mask)||
           (alpha_mask&(red_mask|green_mask|blue_mask)))
            return fail("invalid or overlapping BMP channel masks");
    }

    std::array<std::array<std::uint8_t,4>,256> palette{};
    std::size_t palette_count=0;
    if(bpp<=8)palette_count=colors_used?colors_used:(std::size_t{1}<<bpp);
    else if(colors_used)palette_count=colors_used;
    if(palette_count>256)return fail("BMP palette exceeds 256 entries");
    const std::size_t palette_entry_bytes=core?3:4;
    if(palette_count) {
        if(!range(palette_offset,palette_count*palette_entry_bytes,limit) ||
           palette_offset+palette_count*palette_entry_bytes>pixel_offset)
            return fail("truncated BMP color table");
        for(std::size_t i=0;i<palette_count;++i) {
            const auto p=palette_offset+i*palette_entry_bytes;
            palette[i]={bytes[p],bytes[p+1],bytes[p+2],255};
        }
    }
    if(bpp<=8 && palette_count==0)return fail("indexed BMP has no color table");
    if(pixel_offset>limit || pixel_offset<palette_offset+palette_count*palette_entry_bytes)
        return fail("invalid BMP pixel-data offset");

    DecodedBitmap decoded;
    decoded.width=w;decoded.height=h;decoded.bits_per_pixel=bpp;
    decoded.compression=compression;decoded.top_down=top_down;
    decoded.bgra.resize(std::size_t(w)*h*4);
    const auto put=[&](std::uint32_t x,std::uint32_t y,std::uint8_t b,
                       std::uint8_t g,std::uint8_t r,std::uint8_t a) {
        auto* p=decoded.bgra.data()+(std::size_t(y)*w+x)*4;
        p[0]=b;p[1]=g;p[2]=r;p[3]=a;
    };
    const auto put_index=[&](std::uint32_t x,std::uint32_t file_y,std::uint32_t index)->bool {
        if(x>=w || file_y>=h || index>=palette_count)return false;
        const auto& c=palette[index];
        put(x,h-1-file_y,c[0],c[1],c[2],c[3]);return true;
    };

    if(compression==kRle8 || compression==kRle4) {
        const std::size_t end=image_size?std::size_t(pixel_offset)+image_size:limit;
        if(end>limit || end<pixel_offset)return fail("invalid compressed BMP data size");
        if(palette_count)for(std::uint32_t y=0;y<h;++y)for(std::uint32_t x=0;x<w;++x)
            (void)put_index(x,y,0);
        std::size_t pos=pixel_offset;
        std::uint32_t x=0,y=0;
        bool ended=false;
        while(pos<end && !ended) {
            if(!range(pos,2,end))return fail("truncated BMP RLE command");
            const std::uint8_t count=bytes[pos++],value=bytes[pos++];
            if(count) {
                if(y>=h || count>w-x)return fail("BMP RLE run exceeds image bounds");
                for(std::uint32_t i=0;i<count;++i) {
                    const auto index=compression==kRle8?value:
                        ((i&1)?(value&15):(value>>4));
                    if(!put_index(x++,y,index))return fail("BMP RLE palette index out of range");
                }
                continue;
            }
            if(value==0) { // end of line
                x=0;
                if(++y>h)return fail("BMP RLE line exceeds image bounds");
            } else if(value==1) { // end of bitmap
                ended=true;
            } else if(value==2) { // delta
                if(!range(pos,2,end))return fail("truncated BMP RLE delta");
                const auto dx=bytes[pos++],dy=bytes[pos++];
                if(x+dx>w || y+dy>=h)return fail("BMP RLE delta exceeds image bounds");
                x+=dx;y+=dy;
            } else { // absolute mode; value is pixel count
                const std::uint32_t n=value;
                if(y>=h || n>w-x)return fail("BMP RLE absolute run exceeds image bounds");
                const std::size_t packed=compression==kRle8?n:(n+1)/2;
                const std::size_t padded=packed+(packed&1);
                if(!range(pos,padded,end))return fail("truncated BMP RLE absolute pixels");
                for(std::uint32_t i=0;i<n;++i) {
                    const auto byte=bytes[pos+(compression==kRle8?i:i/2)];
                    const auto index=compression==kRle8?byte:
                        ((i&1)?(byte&15):(byte>>4));
                    if(!put_index(x++,y,index))return fail("BMP RLE palette index out of range");
                }
                pos+=padded;
            }
        }
        if(!ended)return fail("BMP RLE stream has no end-of-bitmap marker");
    } else {
        const std::uint64_t row_bits=std::uint64_t(w)*bpp;
        const std::uint64_t stride64=((row_bits+31)/32)*4;
        const std::uint64_t total64=stride64*h;
        if(stride64>std::numeric_limits<std::size_t>::max() ||
           total64>std::numeric_limits<std::size_t>::max() ||
           total64>limit-pixel_offset)
            return fail("truncated BMP pixel rows");
        const auto stride=static_cast<std::size_t>(stride64);
        const auto total=static_cast<std::size_t>(total64);
        if(image_size && (image_size<total || image_size>limit-pixel_offset))
            return fail("BMP image-size field does not cover its pixel rows");
        for(std::uint32_t fy=0;fy<h;++fy) {
            const auto* row=bytes.data()+pixel_offset+std::size_t(fy)*stride;
            const auto y=top_down?fy:(h-1-fy);
            for(std::uint32_t x=0;x<w;++x) {
                if(bpp<=8) {
                    std::uint32_t index{};
                    if(bpp==8)index=row[x];
                    else if(bpp==4)index=(x&1)?(row[x/2]&15):(row[x/2]>>4);
                    else index=(row[x/8]>>(7-(x&7)))&1;
                    if(index>=palette_count)return fail("BMP pixel references a missing palette entry");
                    const auto& c=palette[index];put(x,y,c[0],c[1],c[2],255);
                } else if(bpp==24) {
                    const auto* p=row+std::size_t(x)*3;
                    put(x,y,p[0],p[1],p[2],255);
                } else {
                    const std::uint32_t pixel=bpp==16?u16(bytes,pixel_offset+std::size_t(fy)*stride+std::size_t(x)*2):
                        u32(bytes,pixel_offset+std::size_t(fy)*stride+std::size_t(x)*4);
                    const auto a=alpha_mask?component(pixel,alpha_mask):std::uint8_t{255};
                    put(x,y,component(pixel,blue_mask),component(pixel,green_mask),
                        component(pixel,red_mask),a);
                }
            }
        }
    }
    output=std::move(decoded);
    error.clear();
    return true;
}

inline bool decode_bmp_file(const std::filesystem::path& path,
                            DecodedBitmap& output,std::string& error) {
    output={};
    std::error_code ec;
    const auto size=std::filesystem::file_size(path,ec);
    if(ec || size<26 || size>bmp_detail::kMaxFileBytes) {
        error="cannot read BMP or file exceeds size limit: "+path.string();
        return false;
    }
    std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
    std::ifstream input(path,std::ios::binary);
    if(!input || !input.read(reinterpret_cast<char*>(bytes.data()),
                            static_cast<std::streamsize>(bytes.size()))) {
        error="cannot read complete BMP file: "+path.string();
        return false;
    }
    if(!decode_bmp(bytes,output,error)) {
        error=path.string()+": "+error;
        return false;
    }
    return true;
}

} // namespace btb::display
