#pragma once
#if defined(_WIN32)
#include <windows.h>
#include "btb/packed_frame_snapshot.hpp"
#include <string>
namespace btb::full_game {
// Optional native presentation sink. Faithful host passes nullptr and
// continues to use the original DD7 presentation path.
class Win32FrameOutput {
public:
    virtual ~Win32FrameOutput()=default;
    virtual bool initialize(HWND,std::string&)=0;
    virtual bool present(const display::PackedFrameSnapshot&,std::string&)=0;
    virtual void state_changed(int) noexcept {}
    virtual void map_pointer(int&,int&,int,int) const noexcept=0;
    virtual void toggle_borderless() noexcept=0;
    virtual void shutdown() noexcept=0;
};
}
#endif
