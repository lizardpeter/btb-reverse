#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace btb::graphics {
// The source game selects 16-bit fullscreen. Its key comparison occurs
// after RGB565 quantization: e.g. 255,1,255 and 255,0,255 are identical.
// Preserve that behavior on an RGB32 desktop without quantizing opaque
// output pixels or modifying any of the original bitmaps.
inline constexpr std::uint32_t most_significant_channel_bits(
    std::uint32_t channel,unsigned count) noexcept {
    std::uint32_t result=0;
    for(int bit=31;bit>=0&&count;--bit) {
        const auto mask=std::uint32_t{1}<<bit;
        if(channel&mask) {result|=mask;--count;}
    }
    return result;
}
inline constexpr std::uint32_t retail_rgb565_key_mask(
    std::uint32_t red,std::uint32_t green,std::uint32_t blue) noexcept {
    return most_significant_channel_bits(red,5)|
           most_significant_channel_bits(green,6)|
           most_significant_channel_bits(blue,5);
}

// Only actual RGB bits participate in the retail transparency key.
// Rows are byte-addressed to avoid alignment UB.
inline void copy_rgb32_keyed_row(const void* source,void* destination,
    std::size_t pixels,std::uint32_t rgb_mask,std::uint32_t key) noexcept {
    auto* src=static_cast<const unsigned char*>(source);
    auto* dst=static_cast<unsigned char*>(destination);
    const auto rgb_key=key&rgb_mask;
    for(std::size_t x=0;x<pixels;++x,src+=4,dst+=4) {
        std::uint32_t pixel{};
        std::memcpy(&pixel,src,sizeof(pixel));
        if((pixel&rgb_mask)!=rgb_key)
            std::memcpy(dst,&pixel,sizeof(pixel));
    }
}
}
