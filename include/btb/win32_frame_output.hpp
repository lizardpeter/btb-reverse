#pragma once
#if defined(_WIN32)
#include <windows.h>
#include "btb/packed_frame_snapshot.hpp"
#include <string>
#include <vector>
namespace btb::full_game {
// Optional native presentation sink. Faithful host passes nullptr and
// continues to use the original DD7 presentation path.
struct ResolvedDraw;
class Win32FrameOutput {
public:
    virtual ~Win32FrameOutput()=default;
    virtual bool initialize(HWND,std::string&)=0;
    virtual bool present(const display::PackedFrameSnapshot&,std::string&)=0;
    virtual bool supports_bitmap_frames() const noexcept {return false;}
    virtual bool present_bitmap_frame(const display::PackedFrameSnapshot& frame,
        const std::vector<ResolvedDraw>&,std::string& error){return present(frame,error);}
    virtual void state_changed(int) noexcept {}
    virtual bool wants_idle_present() const noexcept {return false;}
    virtual bool idle_present(std::string&) {return true;}
    virtual unsigned long long presented_frames() const noexcept {return 0;}
    virtual void map_pointer(int&,int&,int,int) const noexcept=0;
    virtual void toggle_borderless() noexcept=0;
    virtual void shutdown() noexcept=0;
};
}
#endif
