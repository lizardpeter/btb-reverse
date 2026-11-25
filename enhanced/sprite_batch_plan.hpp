#pragma once
#include "btb/full_game_blt_clip.hpp"
#include <array>
#include <cstdint>
#include <span>
#include <vector>
#include <limits>
namespace btb::enhanced {
// Word-addressed storage layout shared with sprite_batch.slang.
// Colors are 0xAABBGGRR (the little-endian byte stream is RGBA).
inline constexpr unsigned tile_size=32,tiles_x=20,tiles_y=15,tile_count=300;
inline constexpr std::size_t batch_word_limit=8*1024*1024;
inline constexpr unsigned job_words=8,header_words=4,tile_words=2*tile_count;
struct SpriteJob {
    std::uint32_t atlas{},stride{},src_x{},src_y{},dst_x{},dst_y{},width{},height{};
    bool keyed{};
};
inline bool make_sprite_job(SpriteJob& job,const full_game::Draw& draw,
    unsigned width,unsigned height,std::uint32_t atlas) noexcept {
    using full_game::Rect;
    if(!width||!height||width>32768||height>32768)return false;
    const auto src=draw.source_rectangle.value_or(Rect{0,0,int(width),int(height)});
    if(src.left<0||src.top<0||src.right<=src.left||src.bottom<=src.top||
       src.right>int(width)||src.bottom>int(height))return false;
    const auto clip=full_game::retail_clip_blt_to_rect(src,draw.x,draw.y,
        draw.destination_clip.value_or(Rect{0,0,640,480}));
    job={};
    if(!clip.visible)return true;
    job={atlas,width,unsigned(clip.source.left),unsigned(clip.source.top),
        unsigned(clip.destination.left),unsigned(clip.destination.top),
        unsigned(clip.destination.right-clip.destination.left),
        unsigned(clip.destination.bottom-clip.destination.top),draw.color_keyed};
    return true;
}
// Reverse-order lists allow the shader to stop at the first opaque pixel.
// Painter order is exact, including multiple overlapping keyed sprites.
inline bool pack_sprite_metadata(std::span<const SpriteJob> jobs,
    std::size_t atlas_size,std::size_t reserved_atlas_base,std::vector<std::uint32_t>& words) {
    if(jobs.size()>8192||atlas_size>batch_word_limit)return false;
    std::array<std::vector<std::uint32_t>,tile_count> bins;
    std::size_t total_indices=0;
    for(std::size_t n=jobs.size();n>0;--n){
        const auto& j=jobs[n-1];
        if(!j.width||!j.height)continue;
        if(!j.stride||j.dst_x>=640||j.dst_y>=480||j.width>640-j.dst_x||
           j.height>480-j.dst_y||j.src_x>=j.stride||j.width>j.stride-j.src_x)return false;
        const std::uint64_t last=std::uint64_t(j.atlas)+(std::uint64_t(j.src_y)+j.height-1)*j.stride+j.src_x+j.width-1;
        if(last>=atlas_size)return false;
        for(unsigned y=j.dst_y/tile_size;y<=(j.dst_y+j.height-1)/tile_size;++y)
            for(unsigned x=j.dst_x/tile_size;x<=(j.dst_x+j.width-1)/tile_size;++x){
                bins[y*tiles_x+x].push_back(std::uint32_t(n-1));++total_indices;
            }
    }
    const auto job_base=header_words+tile_words;
    const auto index_base=job_base+jobs.size()*job_words;
    const auto metadata_size=index_base+total_indices;
    const auto atlas_base=reserved_atlas_base?reserved_atlas_base:metadata_size;
    if(metadata_size>atlas_base||atlas_base>batch_word_limit||atlas_size>batch_word_limit-atlas_base)return false;
    words.resize(metadata_size);
    words[0]=std::uint32_t(job_base);words[1]=std::uint32_t(index_base);
    words[2]=std::uint32_t(atlas_base);words[3]=std::uint32_t(jobs.size());
    std::size_t cursor=0;
    for(unsigned i=0;i<tile_count;++i){
        words[header_words+2*i]=std::uint32_t(cursor);
        words[header_words+2*i+1]=std::uint32_t(bins[i].size());
        for(auto idx:bins[i])words[index_base+cursor++]=idx;
    }
    for(std::size_t i=0;i<jobs.size();++i){
        const auto& j=jobs[i];auto* p=words.data()+job_base+i*job_words;
        p[0]=j.atlas;p[1]=j.stride;p[2]=j.src_x;p[3]=j.src_y;
        p[4]=j.dst_x;p[5]=j.dst_y;p[6]=j.width;
        p[7]=j.height|(j.keyed?0x80000000U:0);
    }
    return true;
}
inline bool pack_sprite_batch(std::span<const SpriteJob> jobs,
    std::span<const std::uint32_t> atlas,std::vector<std::uint32_t>& words){
    if(!pack_sprite_metadata(jobs,atlas.size(),0,words))return false;
    const auto base=words[2];words.resize(base+atlas.size());
    std::copy(atlas.begin(),atlas.end(),words.begin()+base);return true;
}
// Worst-case tile coverage reserves stable atlas offsets for every possible
// visible subset of the current draw stream (including offscreen sprites).
constexpr std::size_t reserve_atlas_base(std::size_t draws) noexcept {
    if(draws>8192)return 0;
    return header_words+tile_words+draws*(job_words+tile_count);
}
// Portable oracle for shader test vectors; not used by the renderer.
inline std::uint32_t sprite_batch_pixel(std::span<const std::uint32_t> w,unsigned x,unsigned y,bool bgra=false) {
    const auto tile=(y/32)*20+x/32;
    auto color=0xff000000U;
    const auto first=w[4+2*tile],count=w[5+2*tile];
    for(unsigned n=0;n<count;++n){
        const auto p=w[0]+8*w[w[1]+first+n];
        const auto dx=w[p+4],dy=w[p+5],h=w[p+7]&0x7fffffffU;
        if(x<dx||y<dy||x-dx>=w[p+6]||y-dy>=h)continue;
        const auto c=w[w[2]+w[p]+(w[p+3]+y-dy)*w[p+1]+w[p+2]+x-dx]&0xffffffU;
        if((w[p+7]&0x80000000U)&&c==0xff00ffU)continue;
        color=c|0xff000000U;break;
    }
    if(bgra)color=(color&0xff00ff00U)|((color&0xffU)<<16)|((color>>16)&0xffU);
    return color;
}
}
