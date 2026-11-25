#pragma once
#if defined(_WIN32)
#include "btb/win32_original_directdraw.hpp"
#include "btb/win32_native_bink_surface.hpp"
#include "btb/native_movie_pixels.hpp"
#include <utility>
namespace btb::full_game {
// The enhanced movie banks no longer own DD7 surfaces. The faithful path
// retains its existing surface format, conversion, and clone operations.
// This wrapper is intentionally separate from the legacy RAD Bink ABI.
class Win32NativeMovieSurface {
    IDirectDrawSurface7* legacy_{};
    NativeMoviePixels packed_{};
    int width_{},height_{};
public:
    Win32NativeMovieSurface()=default;
    ~Win32NativeMovieSurface(){reset();}
    Win32NativeMovieSurface(const Win32NativeMovieSurface&)=delete;
    Win32NativeMovieSurface& operator=(const Win32NativeMovieSurface&)=delete;
    Win32NativeMovieSurface(Win32NativeMovieSurface&& other) noexcept
        :legacy_(std::exchange(other.legacy_,nullptr)),packed_(std::move(other.packed_)),
         width_(std::exchange(other.width_,0)),height_(std::exchange(other.height_,0)){}
    Win32NativeMovieSurface& operator=(Win32NativeMovieSurface&& other) noexcept{
        if(this!=&other){reset();legacy_=std::exchange(other.legacy_,nullptr);
            packed_=std::move(other.packed_);width_=std::exchange(other.width_,0);height_=std::exchange(other.height_,0);}
        return *this;
    }
    explicit operator bool() const noexcept{return legacy_||packed_.valid();}
    void reset() noexcept{if(legacy_)legacy_->Release();legacy_=nullptr;packed_.reset();width_=height_=0;}
    bool allocate(Win32OriginalDirectDraw& renderer,int width,int height,std::string& error){
        reset();
        if(renderer.enhanced_output_active()){
            if(!packed_.allocate(width,height)){error="invalid enhanced movie surface size";return false;}
        }else if(!renderer.create_bink_offscreen_surface(width,height,legacy_,error))return false;
        width_=width;height_=height;error.clear();return true;
    }
    bool write(const bink32_recovered::BinkNativeYuVaFrame& frame,std::string& error){
        if(legacy_)return copy_native_bink_frame_to_directdraw(legacy_,frame,error);
        if(!packed_.write(frame)){error="native packed movie frame conversion failed";return false;}
        error.clear();return true;
    }
    bool blit(Win32OriginalDirectDraw& renderer,int x,int y,std::string& error) const{
        if(legacy_)return renderer.blit_bink_offscreen_surface(legacy_,width_,height_,x,y,error);
        if(!packed_.valid()){error="native packed movie surface unavailable";return false;}
        return renderer.blit_packed_movie_surface(packed_.snapshot(),x,y,error);
    }
    bool clone_from(Win32OriginalDirectDraw& renderer,const Win32NativeMovieSurface& source,std::string& error){
        if(this==&source||!source){error="invalid native movie snapshot source";return false;}
        reset();
        if(source.packed_.valid()){
            packed_=source.packed_;width_=source.width_;height_=source.height_;error.clear();return true;
        }
        if(!renderer.create_bink_offscreen_surface(source.width_,source.height_,legacy_,error))return false;
        RECT rect{0,0,source.width_,source.height_};
        const auto hr=legacy_->BltFast(0,0,source.legacy_,&rect,DDBLTFAST_WAIT);
        if(FAILED(hr)){reset();error="native movie duplicate overlay snapshot failed";return false;}
        width_=source.width_;height_=source.height_;error.clear();return true;
    }
};
}
#endif
