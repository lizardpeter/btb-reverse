#pragma once
#include <algorithm>
#include <cstdint>
#include <string_view>
namespace btb::enhanced {
struct Viewport {int x{},y{},width{},height{};};
constexpr Viewport viewport(int w,int h,bool integer) noexcept {
    if(w<=0||h<=0)return {};
    int dw=w,dh=h;
    if(integer&&w>=640&&h>=480){
        const int scale=std::min(w/640,h/480);dw=640*scale;dh=480*scale;
    }else if(std::int64_t(w)*480>std::int64_t(h)*640)dw=int(std::int64_t(h)*640/480);
    else dh=int(std::int64_t(w)*480/640);
    return {(w-dw)/2,(h-dh)/2,std::max(1,dw),std::max(1,dh)};
}
constexpr void map_pointer(int& x,int& y,Viewport v) noexcept {
    if(!v.width||!v.height){x=y=0;return;}
    // Outside the active picture remains outside the game's hitboxes.
    x=x<v.x?-1:x>=v.x+v.width?640:int(std::int64_t(x-v.x)*640/v.width);
    y=y<v.y?-1:y>=v.y+v.height?480:int(std::int64_t(y-v.y)*480/v.height);
}
constexpr std::string_view presence_for_state(int state) noexcept {
    if(state>=0x0c&&state<=0x0f)return "Herding animals";
    if(state>=0x10&&state<=0x15)return "Dinosaur puzzles";
    if(state>=0x18&&state<=0x1b)return "Spud Skate";
    if(state>=0x1e&&state<=0x21)return "Maze";
    if(state>=0x22&&state<=0x25)return "Fireworks";
    if(state>=0x26&&state<=0x29)return "Squirrel activity";
    if(state>=0x2a&&state<=0x2f)return "Bob's Band";
    if(state>=0x30&&state<=0x33)return "Designing a park";
    if(state>=0x34&&state<=0x37)return "Adventure Golf";
    if(state>=0x38&&state<=0x3b)return "Spud Maze";
    if(state==0)return "Starting the game";
    return "Choosing an activity";
}
constexpr bool valid_discord_id(std::string_view id) noexcept {
    if(id.size()<17||id.size()>20)return false;
    for(char c:id)if(c<'0'||c>'9')return false;
    return true;
}
}
