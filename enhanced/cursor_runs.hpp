#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include <algorithm>
namespace btb::enhanced {
inline constexpr unsigned cursor_size=19,cursor_pixels=cursor_size*cursor_size;
struct CursorRun {unsigned x{},y{},length{};};
using CursorPixels=std::array<std::uint32_t,cursor_pixels>;
// Packed RGB is 0xAABBGGRR. Magenta is the original transparent key.
// All regions are disjoint, as required by vkCmdCopyBufferToImage.
inline std::vector<CursorRun> cursor_runs(const CursorPixels& pixels){
    std::vector<CursorRun> runs;
    for(unsigned y=0;y<cursor_size;++y){
        unsigned x=0;
        while(x<cursor_size){
            while(x<cursor_size&&(pixels[y*cursor_size+x]&0xffffffU)==0xff00ffU)++x;
            const auto first=x;
            while(x<cursor_size&&(pixels[y*cursor_size+x]&0xffffffU)!=0xff00ffU)++x;
            if(x>first)runs.push_back({first,y,x-first});
        }
    }
    return runs;
}
constexpr std::uint32_t cursor_format(std::uint32_t c,bool bgra) noexcept {
    c|=0xff000000U;
    return bgra?(c&0xff00ff00U)|((c&255U)<<16)|((c>>16)&255U):c;
}
constexpr bool cursor_position(int& x,int& y) noexcept {
    if(x<0||y<0||x>=640||y>=480)return false;
    x=std::clamp(x,0,621);y=std::clamp(y,0,461);return true;
}
}
