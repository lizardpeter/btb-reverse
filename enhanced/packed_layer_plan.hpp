#pragma once
#include "sprite_batch_plan.hpp"
#include "btb/frame_composition_layer.hpp"
namespace btb::enhanced {
inline bool append_packed_layer(const full_game::PackedCompositionLayer& layer,
    std::vector<std::uint32_t>& atlas,std::vector<SpriteJob>& jobs){
    if(!layer.pixels||!layer.pixels->valid()||atlas.size()>batch_word_limit)return false;
    const auto& p=*layer.pixels;
    const auto count=p.width*p.height;
    full_game::Draw draw{};draw.x=layer.x;draw.y=layer.y;
    SpriteJob job{};
    if(!make_sprite_job(job,draw,unsigned(p.width),unsigned(p.height),std::uint32_t(atlas.size())))return false;
    if(!job.width||!job.height)return true;
    if(count>batch_word_limit-atlas.size())return false;
    const auto old=atlas.size();atlas.resize(old+count);
    // Normalize all supported source formats to RGBA. Source magenta is
    // ordinary opaque video/captured content, not a sprite color key.
    const display::PackedRgbFormat rgba{4,0xff,0xff00,0xff0000};
    auto* bytes=reinterpret_cast<std::uint8_t*>(atlas.data()+old);
    if(!display::restore_packed_frame(p,{bytes,count*4},p.width*4,rgba)){
        atlas.resize(old);return false;
    }
    for(std::size_t i=old;i<atlas.size();++i)atlas[i]|=0xff000000U;
    jobs.push_back(job);return true;
}
}
