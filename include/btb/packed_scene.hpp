#pragma once
#include "btb/full_game_blt_clip.hpp"
#include "btb/packed_frame_snapshot.hpp"
namespace btb::full_game {
// Enhanced reference scene. Canonical top-down BGRX at original logical
// coordinates; no DirectDraw device, primary surface or surface locks.
class PackedScene {
    display::PackedFrameSnapshot frame_{};
public:
    static constexpr display::PackedRgbFormat format{4,0xff0000U,0xff00U,0xffU};
    bool initialize(){frame_.width=640;frame_.height=480;frame_.format=format;
        frame_.pixels.assign(640*480*4,0);return true;}
    bool valid()const noexcept{return frame_.valid();}
    void reset()noexcept{frame_.clear();}
    const display::PackedFrameSnapshot& frame()const noexcept{return frame_;}
    display::PackedFrameSnapshot& frame()noexcept{return frame_;}
    void clear(std::uint32_t color=0)noexcept{
        if(!valid())return;
        for(std::size_t i=0;i<frame_.pixels.size();i+=4)std::memcpy(frame_.pixels.data()+i,&color,4);
    }
    bool restore(const display::PackedFrameSnapshot& p,int x=0,int y=0)noexcept{
        if(!valid()||!p.valid()||x<0||y<0||x>640||y>480||
            p.width>std::size_t(640-x)||p.height>std::size_t(480-y))return false;
        auto dst=std::span<std::uint8_t>(frame_.pixels).subspan((std::size_t(y)*640+x)*4);
        return display::restore_packed_frame(p,dst,640*4,format);
    }
    bool blit(std::span<const std::uint8_t> source,unsigned width,unsigned height,const Draw& draw)noexcept{
        if(!valid()||!width||!height||width>8192||height>8192||
           source.size()!=std::size_t(width)*height*4)return false;
        const auto src=draw.source_rectangle.value_or(Rect{0,0,int(width),int(height)});
        if(src.left<0||src.top<0||src.right>int(width)||src.bottom>int(height)||
           src.right<=src.left||src.bottom<=src.top)return false;
        const auto c=retail_clip_blt_to_rect(src,draw.x,draw.y,
            draw.destination_clip.value_or(Rect{0,0,640,480}));
        if(!c.visible)return true;
        const auto n=std::size_t(c.destination.right-c.destination.left);
        for(int y=0;y<c.destination.bottom-c.destination.top;++y){
            const auto* in=source.data()+(std::size_t(c.source.top+y)*width+c.source.left)*4;
            auto* out=frame_.pixels.data()+(std::size_t(c.destination.top+y)*640+c.destination.left)*4;
            if(!draw.color_keyed){std::memcpy(out,in,n*4);continue;}
            for(std::size_t x=0;x<n;++x){
                const auto* p=in+4*x;
                if(p[0]==255&&p[1]==0&&p[2]==255)continue;
                std::memcpy(out+4*x,p,4);
            }
        }
        return true;
    }
};
}
