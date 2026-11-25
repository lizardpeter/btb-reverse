#pragma once
#include "btb/bink32_surface_convert.hpp"
#include "btb/packed_frame_snapshot.hpp"
namespace btb::full_game {
// Portable native movie storage. One packed RGB image per independent
// stream; alpha blends into that stream's previous pixels, just as the
// recovered native Bink surface conversion does. No graphics API handle.
class NativeMoviePixels {
    display::PackedFrameSnapshot pixels_{};
public:
    bool allocate(int width,int height){
        if(width<=0||height<=0||width>640||height>480)return false;
        pixels_.format={4,0x000000ffU,0x0000ff00U,0x00ff0000U};
        pixels_.width=std::size_t(width);pixels_.height=std::size_t(height);
        pixels_.pixels.assign(pixels_.width*pixels_.height*4,0);
        return true;
    }
    bool valid() const noexcept{return pixels_.valid();}
    const display::PackedFrameSnapshot& snapshot() const noexcept{return pixels_;}
    void reset() noexcept{pixels_.clear();}
    bool write(const bink32_recovered::BinkNativeYuVaFrame& frame) noexcept {
        if(!valid()||frame.width>pixels_.width||frame.height>pixels_.height)return false;
        bink32_recovered::BinkPackedSurface target{
            pixels_.pixels,pixels_.width*4,frame.width,frame.height,4,
            pixels_.format.red,pixels_.format.green,pixels_.format.blue};
        return bink32_recovered::blit_bink_yuva_to_surface(frame,target)==bink32_recovered::BinkSurfaceResult::ok;
    }
};
}
