#include "frame_pacing.hpp"
#include "cursor_runs.hpp"
using namespace btb::enhanced;
int main(){
    static_assert(parse_fps(L"240")==240&&parse_fps(L"480")==480&&parse_fps(L"1000")==1000);
    static_assert(parse_fps(L"0")==0&&parse_fps(L"29")==0&&parse_fps(L"1001")==0&&parse_fps(L"240Hz")==0);
    for(unsigned fps:{60U,120U,144U,165U,240U,360U,480U,1000U}){
        FramePacer p(fps);
        std::uint64_t now=1000000;
        if(!p.due(now))return 1;
        for(unsigned i=0;i<fps;++i){
            if(!p.due(now))return 2;
            p.submitted(now);
            if(p.due(now)||!p.due(p.next_ns)||p.wait_us(now)>2000)return 3;
            now=p.next_ns;
        }
        if(now<1001000000ULL||now>1001001000ULL)return 4;
        now+=10000000000ULL;
        p.submitted(now);
        if(p.next_ns!=now+p.period_ns)return 5;
    }
    FramePacer unlimited;
    if(!unlimited.due(0)||unlimited.wait_us(1)!=250)return 6;
    CursorPixels pixels{};pixels.fill(0xffff00ffU);
    pixels[0]=0xff0000ff;pixels[1]=0xff00ff00;pixels[3]=0xffff0000;
    pixels[18]=0xffffffff;pixels[19]=0xff000000;pixels.back()=0x00000000;
    auto runs=cursor_runs(pixels);
    if(runs.size()!=5||runs[0].x!=0||runs[0].length!=2||runs[1].x!=3||runs.back().x!=18||runs.back().y!=18)return 7;
    unsigned covered=0;
    for(const auto& run:runs)for(unsigned i=0;i<run.length;++i){
        if((pixels[run.y*19+run.x+i]&0xffffffU)==0xff00ffU)return 8;
        ++covered;
    }
    if(covered!=6||cursor_format(0x000000ff,true)!=0xffff0000U||cursor_format(0x000000ff,false)!=0xff0000ffU)return 9;
    int x=639,y=479;if(!cursor_position(x,y)||x!=621||y!=461)return 10;
    x=-1;if(cursor_position(x,y))return 11;
    pixels.fill(0xffff00ffU);if(!cursor_runs(pixels).empty())return 12;
    pixels.fill(0xffffffffU);if(cursor_runs(pixels).size()!=19)return 13;
    return 0;
}
