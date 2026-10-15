#include "btb/bink32_recovered.hpp"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <string_view>
using namespace btb::bink32_recovered;
// Release-safe checks (MSVC /DNDEBUG must not suppress tests).
#define CHECK(p) do{if(!(p)){std::fprintf(stderr,"Bink RE regression line %d: %s\n",__LINE__,#p);std::abort();}}while(0)
int main() {
    Globals g{};
    rad_set_memory(g,0x1234,0x9876);
    CHECK(g.allocation_callback==0x1234&&g.free_callback==0x9876);
    buffer_set_resolution(g,640,480,16);
    CHECK(g.buffer_width==640&&g.buffer_height==480&&g.buffer_bpp==16);
    g.buffer_error="buffer error";
    CHECK(std::string_view(buffer_get_error(g))=="buffer error");
    set_error(g,"Bink open failed");
    CHECK(std::string_view(get_error(g))=="Bink open failed");
    set_frame_rate(g,30000,1001);
    CHECK(g.frame_rate==30000&&g.frame_rate_divisor==1001);
    set_io_size(g,65536);set_io(g,0x123456);set_simulate(g,200);
    CHECK(g.io_buffer_size==65536&&g.io_callback==0x123456&&g.simulation==200);
    const std::array<std::uint32_t,9> ids{1,2,3,4,5,6,7,8,9};
    set_sound_track(g,9,ids);
    CHECK(g.sound_track_count==8&&g.sound_track_ids[0]==1&&g.sound_track_ids[7]==8);
    set_sound_track(g,2,ids);
    CHECK(g.sound_track_count==2&&g.sound_track_ids[7]==8);
    set_sound_track(g,0,ids);CHECK(g.sound_track_count==0);
    const std::array<std::uint32_t,2> small{4,5};
    set_sound_track(g,8,small);CHECK(g.sound_track_count==0);
    const std::array<std::uint32_t,5> types{2,10,7,4,3};
    const std::array<std::uint32_t,5> sizes{100,200,300,400,500};
    const std::array<std::uint32_t,5> track_ids{99,100,101,102,103};
    Movie m{};
    m.track_types=types;m.track_max_sizes=sizes;m.track_ids=track_ids;
    CHECK(get_track_type(&m,2)==7&&get_track_type(nullptr,0)==0);
    CHECK(get_track_max_size(&m,4)==500&&get_track_max_size(&m,5)==0);
    CHECK(get_track_id(&m,0)==99&&get_track_id(nullptr,0)==0);
    set_video_on_off(&m,0);CHECK(m.video_on_off==0);
    set_video_on_off(&m,1);CHECK(m.video_on_off==1);
    const std::array<std::uint32_t,8> flags{1,0,0,1,0,1,0,0};
    m.frame_count=8;m.frame_offset_flags=flags;
    CHECK(get_key_frame(&m,1,0)==1);
    CHECK(get_key_frame(&m,4,1)==4);
    CHECK(get_key_frame(&m,6,2)==6);
    CHECK(get_key_frame(&m,5,0)==4);
    CHECK(get_key_frame(&m,6,0x80)==4);
    CHECK(get_key_frame(&m,8,0)==6);
    CHECK(get_key_frame(&m,2,1)==4);
    CHECK(get_key_frame(&m,4,0x81)==6);
    CHECK(get_key_frame(&m,8,1)==0);
    CHECK(get_key_frame(&m,2,2)==1);
    CHECK(get_key_frame(nullptr,3,0)==0);
    CHECK(get_key_frame(&m,0,0)==0);
    CHECK(get_key_frame(&m,5,31)==0);
    std::puts("Bink retail DLL reconstructed source: PASS");
}
