#pragma once
// Native compatibility storage for authentic composed retail pixels.
// No bitmap synthesis: same-format restoration is an exact byte copy;
// different formats convert RGB fields with rounded integer scaling.
#include <algorithm>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <span>
#include <vector>

namespace btb::display {
struct PackedRgbFormat {
    unsigned bytes{};
    std::uint32_t red{},green{},blue{};
    constexpr bool operator==(const PackedRgbFormat&) const=default;
    [[nodiscard]] constexpr bool valid() const noexcept {
        if(bytes<2||bytes>4||!red||!green||!blue||
           (red&green)||(red&blue)||(green&blue))return false;
        if(bytes<4&&((red|green|blue)>>(bytes*8)))return false;
        for(auto mask:{red,green,blue}){
            const auto field=mask>>std::countr_zero(mask);
            if(std::popcount(field)>10||(field&(field+1U)))return false;
        }
        return true;
    }
};
struct PackedFrameSnapshot {
    PackedRgbFormat format{};
    std::size_t width{},height{};
    std::vector<std::uint8_t> pixels{};
    [[nodiscard]] bool valid() const noexcept {
        return format.valid()&&width&&height&&width<=640&&height<=480&&
            pixels.size()==width*height*format.bytes;
    }
    void clear() noexcept {pixels.clear();width=height=0;format={};}
};
[[nodiscard]] inline bool packed_rect_valid(std::size_t width,std::size_t height,
    PackedRgbFormat format,std::size_t pitch,std::size_t size) noexcept {
    return format.valid()&&width&&height&&width<=640&&height<=480&&
        pitch>=width*format.bytes&&pitch<=(std::numeric_limits<std::size_t>::max)()/height&&
        size>=(height-1)*pitch+width*format.bytes;
}
[[nodiscard]] inline bool capture_packed_frame(PackedFrameSnapshot& frame,
    std::span<const std::uint8_t> bytes,std::size_t pitch,std::size_t width,
    std::size_t height,PackedRgbFormat format) {
    if(!packed_rect_valid(width,height,format,pitch,bytes.size()))return false;
    const auto row=width*format.bytes;
    frame.pixels.resize(row*height);
    for(std::size_t y=0;y<height;++y)
        std::memcpy(frame.pixels.data()+y*row,bytes.data()+y*pitch,row);
    frame.width=width;frame.height=height;frame.format=format;
    return true;
}
[[nodiscard]] constexpr std::uint32_t convert_rgb_field(std::uint32_t pixel,
    std::uint32_t from,std::uint32_t to) noexcept {
    const auto shift_from=std::countr_zero(from),shift_to=std::countr_zero(to);
    const auto max_from=from>>shift_from,max_to=to>>shift_to;
    const auto field=(pixel&from)>>shift_from;
    return ((field*max_to+max_from/2U)/max_from)<<shift_to;
}
[[nodiscard]] inline bool restore_packed_frame(const PackedFrameSnapshot& frame,
    std::span<std::uint8_t> bytes,std::size_t pitch,PackedRgbFormat format) noexcept {
    if(!frame.valid()||!packed_rect_valid(frame.width,frame.height,format,pitch,bytes.size()))
        return false;
    const auto source_row=frame.width*frame.format.bytes;
    if(frame.format==format){
        for(std::size_t y=0;y<frame.height;++y)
            std::memcpy(bytes.data()+y*pitch,frame.pixels.data()+y*source_row,source_row);
        return true;
    }
    for(std::size_t y=0;y<frame.height;++y){
        const auto* src=frame.pixels.data()+y*source_row;
        auto* dst=bytes.data()+y*pitch;
        for(std::size_t x=0;x<frame.width;++x){
            std::uint32_t pixel=0;
            for(unsigned b=0;b<frame.format.bytes;++b)pixel|=std::uint32_t(src[b])<<(8*b);
            const auto rgb=convert_rgb_field(pixel,frame.format.red,format.red)|
                convert_rgb_field(pixel,frame.format.green,format.green)|
                convert_rgb_field(pixel,frame.format.blue,format.blue);
            for(unsigned b=0;b<format.bytes;++b)dst[b]=static_cast<std::uint8_t>(rgb>>(8*b));
            src+=frame.format.bytes;dst+=format.bytes;
        }
    }
    return true;
}
}
