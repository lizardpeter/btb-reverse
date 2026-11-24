#pragma once
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace btb::graphics {
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
