#include "sprite_batch_plan.hpp"
using namespace btb;
using namespace btb::enhanced;
int main(){
    const std::vector<std::uint32_t> atlas{0xff0000ff,0xff00ff00,0xffff0000,0xffffffff,
        0xffff00ff,0xff0000ff,0xff00ff00,0xffff00ff};
    full_game::Draw back{"base",31,31,{},false};
    full_game::Draw front{"keyed",31,31,{},true};
    SpriteJob b{},f{};
    if(!make_sprite_job(b,back,2,2,0)||!make_sprite_job(f,front,2,2,4))return 1;
    std::vector<std::uint32_t> words;
    const std::array jobs{b,f};
    if(!pack_sprite_batch(jobs,atlas,words))return 2;
    if(sprite_batch_pixel(words,31,31)!=0xff0000ff||sprite_batch_pixel(words,32,31)!=0xff0000ff||
       sprite_batch_pixel(words,31,32)!=0xff00ff00||sprite_batch_pixel(words,32,32)!=0xffffffff)return 3;
    if(sprite_batch_pixel(words,30,31)!=0xff000000||sprite_batch_pixel(words,31,31,true)!=0xffff0000)return 4;
    front.color_keyed=false;
    if(!make_sprite_job(f,front,2,2,4)||!pack_sprite_batch(std::array{b,f},atlas,words)||
       sprite_batch_pixel(words,31,31)!=0xffff00ff)return 5;
    front.x=-1;front.y=-1;
    if(!make_sprite_job(f,front,2,2,4)||f.src_x!=1||f.src_y!=1||f.width!=1||f.height!=1)return 6;
    front.source_rectangle=full_game::Rect{0,0,3,2};
    if(make_sprite_job(f,front,2,2,4))return 7;
    back.destination_clip=full_game::Rect{32,32,34,34};
    if(!make_sprite_job(b,back,2,2,0)||b.src_x!=1||b.src_y!=1||b.width!=1||b.height!=1)return 8;
    b.atlas=100;
    if(pack_sprite_batch(std::array{b},atlas,words))return 9;
    if(!pack_sprite_batch({}, {},words)||sprite_batch_pixel(words,639,479)!=0xff000000)return 10;
    return 0;
}
