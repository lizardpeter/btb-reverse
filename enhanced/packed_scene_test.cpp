#include "btb/packed_scene.hpp"
using namespace btb;
using namespace btb::full_game;
int main(){
    PackedScene scene;
    if(scene.valid()||!scene.initialize()||!scene.valid())return 1;
    scene.clear(0x00010203);
    const std::array<std::uint8_t,16> pixels{0,0,255,0,255,0,255,0,255,0,0,0,0,255,0,0};
    Draw d{"fixture",31,31,{},true};
    if(!scene.blit(pixels,2,2,d))return 2;
    auto pixel=[&](unsigned x,unsigned y){std::uint32_t p;std::memcpy(&p,scene.frame().pixels.data()+4*(y*640+x),4);return p;};
    if(pixel(31,31)!=0xff0000||pixel(32,31)!=0x010203||pixel(31,32)!=0xff||pixel(32,32)!=0xff00)return 3;
    d.color_keyed=false;if(!scene.blit(pixels,2,2,d)||pixel(32,31)!=0xff00ff)return 4;
    d.x=-1;d.y=-1;if(!scene.blit(pixels,2,2,d)||pixel(0,0)!=0xff00)return 5;
    d.source_rectangle=Rect{0,0,3,2};if(scene.blit(pixels,2,2,d))return 6;
    const auto snapshot=scene.frame();
    scene.clear();if(!scene.restore(snapshot)||pixel(0,0)!=0xff00)return 7;
    display::PackedFrameSnapshot movie;
    const std::array<std::uint8_t,4> rgb565{0,0xf8,0x1f,0};
    if(!display::capture_packed_frame(movie,rgb565,4,2,1,{2,0xf800,0x7e0,0x1f})||
       !scene.restore(movie,638,479)||pixel(638,479)!=0xff0000||pixel(639,479)!=0xff)return 8;
    if(scene.restore(movie,639,479)||scene.restore(movie,-1,0))return 9;
    scene.reset();if(scene.valid()||scene.restore(snapshot))return 10;
    return 0;
}
