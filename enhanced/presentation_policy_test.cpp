#include "presentation_policy.hpp"
using namespace btb::enhanced;
int main(){
    constexpr auto a=viewport(3840,2160,true);
    static_assert(a.width==2560&&a.height==1920&&a.x==640&&a.y==120);
    constexpr auto b=viewport(2560,1440,true);
    static_assert(b.width==1920&&b.height==1440&&b.x==320);
    constexpr auto c=viewport(3840,2160,false);
    static_assert(c.width==2880&&c.height==2160&&c.x==480);
    constexpr auto d=viewport(320,200,true);
    static_assert(d.width==266&&d.height==200);
    int x=a.x+a.width-1,y=a.y+a.height-1;map_pointer(x,y,a);
    if(x!=639||y!=479)return 1;
    x=a.x-1;y=a.y;map_pointer(x,y,a);if(x!=-1||y!=0)return 2;
    x=a.x+a.width;y=a.y+a.height;map_pointer(x,y,a);if(x!=640||y!=480)return 3;
    if(!valid_discord_id("123456789012345678")||valid_discord_id("12345678901234567x"))return 4;
    if(presence_for_state(0x33)!="Designing a park"||presence_for_state(0x3b)!="Spud Maze")return 5;
    return 0;
}
