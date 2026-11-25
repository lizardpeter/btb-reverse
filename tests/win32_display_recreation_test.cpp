#include "btb/win32_original_directdraw.hpp"
#include "btb/win32_original_display_lifecycle.hpp"
#include <array>
#include <cstdio>
#include <cstring>
#include <string>
#include <string_view>
#include <utility>

#if defined(_WIN32)
namespace {
using btb::display::DisplayMode;
using btb::full_game::CapturedUnderlay;
using btb::full_game::Win32OriginalDirectDraw;
constexpr int kSkip=77;
LRESULT CALLBACK window_proc(HWND h,UINT m,WPARAM w,LPARAM l){return DefWindowProcW(h,m,w,l);}
struct TestWindow {
    HINSTANCE instance{GetModuleHandleW(nullptr)};
    HWND hwnd{};
    ~TestWindow(){if(hwnd)DestroyWindow(hwnd);UnregisterClassW(L"BTBDisplayRecreationTest",instance);}
    bool open(){
        WNDCLASSW cls{};cls.hInstance=instance;cls.lpfnWndProc=window_proc;
        cls.lpszClassName=L"BTBDisplayRecreationTest";
        if(!RegisterClassW(&cls))return false;
        RECT r{0,0,640,480};
        if(!AdjustWindowRect(&r,WS_OVERLAPPEDWINDOW,FALSE))return false;
        hwnd=CreateWindowExW(0,cls.lpszClassName,L"BTB display recovery regression",
            WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,r.right-r.left,r.bottom-r.top,
            nullptr,nullptr,instance,nullptr);
        if(!hwnd)return false;
        ShowWindow(hwnd,SW_SHOWNORMAL);UpdateWindow(hwnd);return true;
    }
};
struct SurfaceRef {
    IDirectDrawSurface7* ptr{};
    ~SurfaceRef(){if(ptr)ptr->Release();}
};
// Real primary-color marker pixels are compared with the actual DD7 masks;
// no BMP fixture, shader, desktop screenshot or original asset is required.
constexpr std::array<unsigned,5> kMarkers{{1,2,4,3,5}};
DWORD marker(const DDPIXELFORMAT& f,unsigned bits){
    return ((bits&1)?f.dwRBitMask:0)|((bits&2)?f.dwGBitMask:0)|((bits&4)?f.dwBBitMask:0);
}
bool access_pixel(IDirectDrawSurface7* s,int x,int y,unsigned bits,bool write){
    if(!s||x<0||y<0)return false;
    DDSURFACEDESC2 d{};d.dwSize=sizeof(d);
    if(FAILED(s->Lock(nullptr,&d,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,nullptr)))return false;
    const auto n=d.ddpfPixelFormat.dwRGBBitCount/8;
    bool ok=false;
    if(d.lpSurface&&d.lPitch>0&&x<static_cast<int>(d.dwWidth)&&y<static_cast<int>(d.dwHeight)&&
       n>=2&&n<=4&&(x+1)*n<=static_cast<unsigned>(d.lPitch)&&
       (d.ddpfPixelFormat.dwFlags&DDPF_RGB)){
        auto* p=static_cast<unsigned char*>(d.lpSurface)+y*d.lPitch+x*n;
        const DWORD expected=marker(d.ddpfPixelFormat,bits);
        if(write){for(unsigned i=0;i<n;++i)p[i]=static_cast<unsigned char>(expected>>(8*i));ok=true;}
        else{
            DWORD actual=0;
            for(unsigned i=0;i<n;++i)actual|=static_cast<DWORD>(p[i])<<(8*i);
            const auto mask=d.ddpfPixelFormat.dwRBitMask|d.ddpfPixelFormat.dwGBitMask|d.ddpfPixelFormat.dwBBitMask;
            ok=(actual&mask)==expected;
        }
    }
    return SUCCEEDED(s->Unlock(nullptr))&&ok;
}
int fail(const char* stage,const std::string& error={}){
    std::fprintf(stderr,"DD7 display recreation: %s: %s\n",stage,error.c_str());return 1;
}
}
int main(int argc,char** argv){
    // A CI service can run outside a visible window station. This is a
    // reported skip, not evidence of device initialization or presentation.
    USEROBJECTFLAGS flags{};DWORD size=0;
    const auto station=GetProcessWindowStation();
    if(!station||!GetUserObjectInformationW(station,UOI_FLAGS,&flags,sizeof(flags),&size)||!(flags.dwFlags&WSF_VISIBLE)){
        std::puts("SKIP: no visible Windows window station for DD7 regression");return kSkip;
    }
    const auto desktop=OpenInputDesktop(0,FALSE,DESKTOP_READOBJECTS);
    if(!desktop){std::puts("SKIP: no accessible Windows input desktop");return kSkip;}
    CloseDesktop(desktop);
    const bool fullscreen=argc==2&&std::string_view(argv[1])=="--fullscreen";
    if(argc>1&&!fullscreen)return fail("usage: --fullscreen is optional");
    TestWindow window;
    if(!window.open())return fail("CreateWindowExW");
    Win32OriginalDirectDraw graphics;
    std::string error;
    const auto reset=[&](DisplayMode mode){return SUCCEEDED(btb::original_win32::create_or_reset_display_manager(window.hwnd,mode,graphics,error));};
    if(!reset(DisplayMode::Windowed))return fail("initialize real windowed DirectDraw",error);
    const auto target=[&](){return graphics.bink_render_target();};
    const auto mark=[&](unsigned color){return access_pixel(target(),17,31,color,true);};
    const auto verify=[&](unsigned color){return access_pixel(target(),17,31,color,false);};
    // Four separate captured backgrounds, with different real pixel values.
    if(!mark(kMarkers[0])||!graphics.capture_replay_underlay(error))return fail("capture Replay",error);
    if(!mark(kMarkers[1])||!graphics.capture_underlay(CapturedUnderlay::Help,error))return fail("capture Help",error);
    if(!mark(kMarkers[2])||!graphics.capture_underlay(CapturedUnderlay::GlobalMovie,error))return fail("capture GlobalMovie",error);
    if(!mark(kMarkers[3])||!graphics.capture_underlay(CapturedUnderlay::Modal,error))return fail("capture Modal",error);
    // Keep an actual movie-sized DD7 offscreen surface on the old device.
    SurfaceRef movie;
    if(!graphics.create_bink_offscreen_surface(8,8,movie.ptr,error)||
       !access_pixel(movie.ptr,3,4,6,true))return fail("old-device movie surface",error);
    // Present a separate fully composed marker, not one of the underlays.
    if(!mark(kMarkers[4])||!graphics.blit_bink_offscreen_surface(movie.ptr,8,8,40,50,error)||
       !graphics.present(error))return fail("actual composed windowed frame present",error);
    // Recreate the DirectDraw object; fullscreen is an explicit local option.
    if(!reset(fullscreen?DisplayMode::Fullscreen:DisplayMode::Windowed))return fail("recreate display",error);
    if(!verify(kMarkers[4])||!access_pixel(target(),43,54,6,false))return fail("restore complete composed frame across recreation");
    if(!graphics.render_replay_ordered({},error)||!verify(kMarkers[0]))return fail("restore independent Replay",error);
    for(const auto [kind,index]:std::array<std::pair<CapturedUnderlay,int>,3>{{
        {CapturedUnderlay::Help,1},{CapturedUnderlay::GlobalMovie,2},{CapturedUnderlay::Modal,3}}}){
        if(!graphics.render_captured_underlay(kind,{},error)||!verify(kMarkers[index]))return fail("restore independent captured underlay",error);
    }
    // The previous-device system-memory movie surface remains valid.
    if(!graphics.blit_bink_offscreen_surface(movie.ptr,8,8,60,70,error)||
       !access_pixel(target(),63,74,6,false))return fail("previous-device movie transfer",error);
    if(!mark(7)||!graphics.present(error))return fail("present recreated frame",error);
    // Capture the presented pixels, even if the next backbuffer was flipped.
    if(!mark(0)||!graphics.capture_presented_underlay(CapturedUnderlay::Modal,error)||
       !graphics.render_captured_underlay(CapturedUnderlay::Modal,{},error)||!verify(7))return fail("capture actual previous present",error);
    if(!reset(DisplayMode::Windowed)||!verify(7))return fail("return to windowed mode",error);
    if(!graphics.render_captured_underlay(CapturedUnderlay::Help,{},error)||!verify(kMarkers[1]))return fail("second recreation preserves Help",error);
    if(FAILED(graphics.prepare_native_frame(error)))return fail("native preflight after recreation",error);
    std::puts("PASS: real DirectDraw7 windows, present, independent underlays, old-device movie pixels, two recreations");
    return 0;
}
#else
int main(){return 77;}
#endif
