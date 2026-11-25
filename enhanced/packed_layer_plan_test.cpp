#include "packed_layer_plan.hpp"
using namespace btb;
using namespace btb::enhanced;
int main(){
    auto background=std::make_shared<display::PackedFrameSnapshot>();
    const std::array<uint8_t,8> base{0x00,0xf8,0xe0,0x07,0x1f,0,0xff,0xff};
    if(!display::capture_packed_frame(*background,base,4,2,2,{2,0xf800,0x7e0,0x1f}))return 1;
    auto movie=std::make_shared<display::PackedFrameSnapshot>();
    const std::array<uint8_t,8> video{255,0,255,0,0,255,255,0};
    if(!display::capture_packed_frame(*movie,video,8,2,1,{4,0xff0000,0xff00,0xff}))return 2;
    std::vector<uint32_t> atlas;std::vector<SpriteJob> jobs;std::vector<uint32_t> packed;
    if(!append_packed_layer({background,31,31},atlas,jobs))return 3;
    if(!append_packed_layer({movie,31,32},atlas,jobs))return 4;
    // Opaque video covers the lower row; keyed sprite covers only green.
    full_game::Draw overlay{"key",31,32,{},true};
    SpriteJob top{};
    if(!make_sprite_job(top,overlay,2,1,uint32_t(atlas.size())))return 5;
    atlas.push_back(0xffff00ff);atlas.push_back(0xff00ff00);jobs.push_back(top);
    if(!pack_sprite_batch(jobs,atlas,packed))return 6;
    if(sprite_batch_pixel(packed,31,31)!=0xff0000ff||sprite_batch_pixel(packed,32,31)!=0xff00ff00||
       sprite_batch_pixel(packed,31,32)!=0xffff00ff||sprite_batch_pixel(packed,32,32)!=0xff00ff00)return 7;
    if(sprite_batch_pixel(packed,0,0)!=0xff000000)return 8;
    const auto count=atlas.size();
    if(!append_packed_layer({movie,-2,0},atlas,jobs)||atlas.size()!=count)return 9;
    if(!append_packed_layer({movie,-1,0},atlas,jobs)||jobs.back().src_x!=1||jobs.back().width!=1)return 10;
    if(append_packed_layer({},atlas,jobs))return 11;
    if(!pack_sprite_batch(jobs,atlas,packed)||sprite_batch_pixel(packed,0,0)!=0xff00ffff)return 12;
    return 0;
}
