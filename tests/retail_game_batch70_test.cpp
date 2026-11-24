#include "btb/retail_game_batch70.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <string_view>
#include <vector>
using namespace btb::retail_game70;
namespace {
std::vector<I32> calls;
I32 random_count=0;
bool cursor_reset=false;
void release(void*) noexcept {calls.push_back(1);}
void* load(std::string_view name) noexcept {assert(name=="a.bmp");calls.push_back(2);return reinterpret_cast<void*>(0x20);}
void key(void* s,U32 c) noexcept {assert(s==reinterpret_cast<void*>(0x20)&&c==0xff00ff);calls.push_back(3);}
void table(I32 n) noexcept {calls.push_back(n);}
void dino(DinoPrintEvent event,bool alt) noexcept {calls.push_back(static_cast<I32>(event));if(event==DinoPrintEvent::DrawHighlight)assert(alt);}
void clear() {cursor_reset=true;}
void grand(I32 id) noexcept {calls.push_back(id);}
U32 random15() noexcept {return (++random_count)==1 ? 3000U : 3U;}
bool not_playing(I32) noexcept {return false;}
I32 voice(I32 id,U32 pri,I32 kind) noexcept {calls.push_back(id);assert((pri==50&&kind==1)||(pri==90&&kind==1));return id;}
void decode() noexcept {calls.push_back(80);}
void restart() noexcept {calls.push_back(81);}
int blit_counter=0;
U32 blit(I32 layer,bool last) noexcept {
    calls.push_back(layer+90);
    if(layer==1)assert(last);
    if(layer==2&&++blit_counter==1)
        return static_cast<U32>(btb::display::kDdErrWasStillDrawing);
    return 0;
}
int input_counter=0;
bool next_video(I32* out) noexcept {if(input_counter>=3)return false;*out=++input_counter;return true;}
bool next_coord(I32& x,I32& y) noexcept {if(input_counter>=2)return false;x=++input_counter;y=x+10;return true;}
bool record(std::span<I32> data) noexcept {for(std::size_t i=0;i<data.size();++i)data[i]=static_cast<I32>(i+1);return true;}
}
int main() {
    static_assert(suppress_fullscreen_system_key(0,0x100,13,true,false));
    static_assert(suppress_fullscreen_system_key(0,0x104,0x73,true,false));
    static_assert(suppress_fullscreen_system_key(0,0x105,9,true,false));
    static_assert(!suppress_fullscreen_system_key(-1,0x100,13,true,false));
    static_assert(!suppress_fullscreen_system_key(0,0x102,13,true,false));
    static_assert(!suppress_fullscreen_system_key(0,0x100,13,false,false));
    static_assert(suppress_fullscreen_system_key(0,0x100,0x1b,false,true));
    static_assert(suppress_fullscreen_system_key(0,0x100,0x2e,true,true));
    // 00403830 source lifecycle: only live entries are reloaded and keyed.
    std::array<RegisteredBitmap,2> images{
        RegisteredBitmap{reinterpret_cast<void*>(0x10),"a.bmp",true},
        RegisteredBitmap{nullptr,"a.bmp",true}
    };
    calls.clear();bool reloading=false;
    reload_registered_bitmap_surfaces(images,reloading,release,load,key);
    assert((calls==std::vector<I32>{1,2,3})&&!reloading);
    assert(images[0].surface==reinterpret_cast<void*>(0x20)&&!images[1].surface);
    // 00407920 all eleven source loaders, exact ordering, returns 1.
    calls.clear();assert(load_global_data_tables(table)==1);
    assert(calls.size()==11);
    for(int i=0;i<11;++i)assert(calls[static_cast<std::size_t>(i)]==i);
    // 00409CD0 dino hover and click gates.
    calls.clear();DinoPrintState print{299,422,false,true,false};
    update_dino_print_button(print,dino);
    assert((calls==std::vector<I32>{0,1,2})&&print.hover_latched);
    calls.clear();update_dino_print_button(print,dino);
    assert((calls==std::vector<I32>{0,1}));
    print.click=true;calls.clear();update_dino_print_button(print,dino);
    assert((calls==std::vector<I32>{0,3,4,5}));
    print.x=350;print.click=false;update_dino_print_button(print,dino);
    assert(!print.hover_latched);
    // 0040D870 clipped source rectangle (retail has no lower-left clamp).
    constexpr auto pond=pond_segment_source(2,4,350,75);
    static_assert(pond.left==2100 && pond.right==2053 && pond.bottom==75);
    // 0040E3E0 reset dragging, some eligible slots do not set visible=1.
    std::array<I32,301> flags{};
    std::array<I32,301> valid{};
    valid[210]=1;
    ParkDrag drag{2,210,flags,valid};
    cursor_reset=false;finish_park_designer_drag(drag,clear);
    assert(drag.mode==0&&!cursor_reset==false); // cursor callback executes
    assert(flags[210]==0);
    drag.mode=2;drag.selected=15;
    finish_park_designer_drag(drag,clear);
    assert(flags[15]==1);
    // 0041F4B0 genuine 24-column head and tail marker 10.
    std::array<I32,48> grid{};grid.fill(-1);
    const std::array<I32,3> lens{1,3,2};
    std::array<I32,2> pairs{};
    calls.clear();
    assert(place_grand_opening_event(grid,lens,pairs,0,4,1,grand));
    assert(grid[4]==1&&grid[5]==10&&grid[6]==10);
    assert(pairs[0]==2&&calls[0]==1);
    assert(!place_grand_opening_event(grid,lens,pairs,0,4,1,grand));
    // 00422760 triggers only on rare draw and avoids playing duplicates.
    random_count=0;calls.clear();
    // first random15=3000 means %3000==0; second draw=3 -> ID 725.
    assert(maybe_play_random_spud_voice(0,-1,random15,not_playing,voice)==0x2d5);
    assert(calls.size()==1&&calls[0]==0x2d5);
    // 00427270 squirrel movement by +/-2 with snap threshold4.
    I32 x=0,y=0;assert(!step_squirrel_actor(x,y,100,100));
    assert(x==2&&y==2);
    assert(step_squirrel_actor(x,y,4,4)&&x==4&&y==4);
    // 00428120 voice/no-overlap, decoding followed by terminal restart.
    calls.clear();
    update_walkthrough_movie(50,141,false,220,voice,decode,10,10,restart);
    assert((calls==std::vector<I32>{220,80,81}));
    calls.clear();
    update_walkthrough_movie(50,141,true,220,voice,decode,3,10,restart);
    assert((calls==std::vector<I32>{80}));
    // 00428E30 draw path: retry DDERR_STILLDRAWING.
    calls.clear();blit_counter=0;
    assert(draw_generic_ui(false,blit,4)==1);
    assert((calls==std::vector<I32>{92,92}));
    calls.clear();assert(draw_generic_ui(true,blit)==1);
    assert((calls==std::vector<I32>{90,91}));
    // 00407770 video sequence EOF and max ten records.
    std::array<I32,10> clips{};
    input_counter=0;
    assert(load_video_sequence(clips,next_video)==3);
    assert(clips[0]==1&&clips[1]==2&&clips[2]==3);
    // 00415CB0 terminate coordinate pairs using (-1,-1).
    std::array<std::array<I32,2>,5> points{};
    input_counter=0;
    assert(load_herding_coordinate_pairs(points,next_coord)==2);
    assert(points[0][0]==1&&points[0][1]==11);
    assert(points[1][0]==2&&points[1][1]==12);
    assert(points[2][0]==-1&&points[2][1]==-1);
    // 00407390/00407430 each parse one host-provided record.
    std::array<I32,11> values{};
    assert(load_options_config(values,record));
    assert(values[0]==1&&values[10]==11);
    assert(load_play_again_overlay(values,record));
}
