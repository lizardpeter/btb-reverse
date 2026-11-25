#include "presentation_activity.hpp"
using namespace btb::enhanced;
int main(){
    static_assert(configured_fps(L"")==240&&configured_fps(L"garbage")==240);
    static_assert(configured_fps(L"0")==0&&configured_fps(L"480")==480);
    PresentationActivity p;
    CursorStamp hidden{},a{20,30,1},b{21,30,1},art{21,30,2};
    if(!p.needs_frame(false,false,false,hidden))return 1;
    p.presented(hidden);
    for(unsigned i=0;i<10000;++i)if(p.needs_frame(false,false,false,hidden))return 2;
    if(p.wait_us(250,false)!=16000||p.wait_us(250,true)!=4000)return 3;
    if(!p.needs_frame(false,false,false,a)||p.quiet)return 4;
    // A failed acquire does not clear cursor work.
    if(!p.needs_frame(false,false,false,a))return 5;
    p.presented(a);
    if(p.needs_frame(false,false,false,a)||!p.needs_frame(false,false,false,b))return 6;
    p.presented(b);
    if(!p.needs_frame(false,false,false,art)||!p.needs_frame(false,false,false,hidden))return 7;
    if(!p.needs_frame(false,true,false,b)||!p.needs_frame(false,false,true,b)||!p.needs_frame(true,false,false,b))return 8;
    p.request_repaint();if(!p.needs_frame(false,false,false,b))return 9;
    p.presented(b);if(p.needs_frame(false,false,false,b))return 10;
    btb::display::PackedFrameSnapshot first,second;
    first.width=second.width=640;first.height=second.height=480;
    first.format=second.format={4,0xff0000,0xff00,0xff};
    first.pixels.resize(640*480*4);second.pixels=first.pixels;
    if(!same_frame(first,second))return 11;
    second.pixels.back()=1;if(same_frame(first,second))return 12;
    second.pixels.back()=0;second.format={4,0xff,0xff00,0xff0000};if(same_frame(first,second))return 13;
    second.clear();if(same_frame(first,second)||same_frame(second,second))return 14;
    return 0;
}
