#include "native_cursor_plan.hpp"
using namespace btb::enhanced;
int main(){
    constexpr auto a=native_cursor_dimensions(viewport(3840,2160,true));
    static_assert(a.width==76&&a.height==76);
    constexpr auto b=native_cursor_dimensions(viewport(1280,960,true));
    static_assert(b.width==38&&b.height==38);
    constexpr auto c=native_cursor_dimensions(viewport(320,240,false));
    static_assert(c.width==10&&c.height==10);
    CursorPixels p{};p.fill(0xffff00ffU);p[0]=0xff0000ffU;p[18]=0xff00ff00U;p.back()=0xffff0000U;
    CursorBitmap out;
    if(!make_cursor_bitmap(p,38,38,out)||out.bgra.size()!=1444||out.mask.size()!=6*38)return 1;
    if(out.bgra[0]!=0xffff0000U||out.bgra[39]!=0xffff0000U||out.bgra[2]!=0||out.bgra[37]!=0xff00ff00U||
       out.bgra.back()!=0xff0000ffU)return 2;
    if((out.mask[37*6]&0xc0)!=0||(out.mask[37*6]&0x20)==0)return 3;
    if((out.mask[4]&0x0c)!=0)return 4;
    if(make_cursor_bitmap(p,0,19,out)||make_cursor_bitmap(p,257,19,out))return 5;
    return 0;
}
