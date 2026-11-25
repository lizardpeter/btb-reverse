#pragma once
#include "btb/packed_frame_snapshot.hpp"
#include "frame_pacing.hpp"
#include <cstring>
namespace btb::enhanced {
constexpr unsigned configured_fps(std::wstring_view value) noexcept {
    if(value==L"0")return 0;
    const auto parsed=parse_fps(value);
    return parsed?parsed:240;
}
struct CursorStamp {
    int x{-1},y{-1};
    std::uint64_t revision{};
    constexpr bool visible()const noexcept{return x>=0&&y>=0;}
    constexpr bool operator==(const CursorStamp&)const=default;
};
inline bool same_frame(const display::PackedFrameSnapshot& a,const display::PackedFrameSnapshot& b) noexcept {
    return a.valid()&&b.valid()&&a.width==b.width&&a.height==b.height&&a.format==b.format&&
        a.pixels.size()==b.pixels.size()&&std::memcmp(a.pixels.data(),b.pixels.data(),a.pixels.size())==0;
}
// Actual work, not source tick cadence or swapchain refresh rate, triggers
// rendering. A failed acquire/present must not acknowledge pending work.
struct PresentationActivity {
    CursorStamp presented_cursor{};
    bool repaint{true};
    bool quiet{};
    constexpr bool needs_frame(bool continuous,bool upload,bool resize,CursorStamp cursor) noexcept {
        const bool needed=continuous||upload||resize||repaint||cursor!=presented_cursor;
        quiet=!needed;return needed;
    }
    constexpr void presented(CursorStamp cursor) noexcept {presented_cursor=cursor;repaint=false;}
    constexpr void request_repaint() noexcept {repaint=true;quiet=false;}
    constexpr unsigned wait_us(unsigned active_wait,bool live_cursor)const noexcept {
        // Windows mouse/paint/size messages also wake the waitable timer.
        return quiet?(live_cursor?4000U:16000U):active_wait;
    }
};
}
