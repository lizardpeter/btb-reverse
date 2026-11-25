#include "btb/win32_original_directdraw.hpp"
#include "btb/win32_native_movie_surface.hpp"
#include <iostream>
#include <fstream>
#include <array>
namespace {
using namespace btb;
using namespace btb::full_game;
struct Window {
    HWND hwnd=CreateWindowExW(0,L"STATIC",L"BTB packed scene test",WS_POPUP,0,0,640,480,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    ~Window(){if(hwnd)DestroyWindow(hwnd);}
};
struct Fixture {
    std::filesystem::path path;
    ~Fixture(){if(!path.empty()){std::error_code ignored;std::filesystem::remove(path,ignored);}}
    bool create(){
        wchar_t dir[MAX_PATH]{},name[MAX_PATH]{};
        const auto n=GetTempPathW(MAX_PATH,dir);
        if(!n||n>=MAX_PATH||!GetTempFileNameW(dir,L"btb",0,name))return false;
        path=name;
        BITMAPFILEHEADER file{};BITMAPINFOHEADER info{};
        file.bfType=0x4d42;file.bfOffBits=sizeof(file)+sizeof(info);file.bfSize=file.bfOffBits+16;
        info.biSize=sizeof(info);info.biWidth=2;info.biHeight=2;info.biPlanes=1;info.biBitCount=24;info.biCompression=BI_RGB;info.biSizeImage=16;
        // Bottom-up 24-bit BGR, two bytes of padding per row.
        const std::array<unsigned char,16> bytes{255,0,0,0,255,0,0,0,0,0,255,255,0,255,0,0};
        std::ofstream out(path,std::ios::binary|std::ios::trunc);
        out.write(reinterpret_cast<const char*>(&file),sizeof(file));out.write(reinterpret_cast<const char*>(&info),sizeof(info));
        out.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());return bool(out);
    }
};
struct Sink final:Win32FrameOutput {
    display::PackedFrameSnapshot latest;
    unsigned presents{},layers{};
    bool initialize(HWND,std::string&)override{return true;}
    bool present(const display::PackedFrameSnapshot& p,std::string& error)override{latest=p;++presents;error.clear();return true;}
    bool supports_layer_frames()const noexcept override{return true;}
    bool present_layer_frame(const display::PackedFrameSnapshot& p,const std::vector<FrameCompositionLayer>& stream,
        const display::PackedFrameSnapshot*,const ResolvedDraw*,std::string& error)override{layers=static_cast<unsigned>(stream.size());return present(p,error);}
    void map_pointer(int&,int&,int,int)const noexcept override{}
    void toggle_borderless()noexcept override{}
    void shutdown()noexcept override{}
};
int fail(const char* why,const std::string& error={}){std::cerr<<why<<": "<<error<<'\n';return 1;}
std::uint32_t pixel(const display::PackedFrameSnapshot& p,unsigned x,unsigned y){
    const auto* b=p.pixels.data()+4*(y*p.width+x);
    return std::uint32_t(b[0])|(std::uint32_t(b[1])<<8)|(std::uint32_t(b[2])<<16);
}
bink32_recovered::BinkNativeYuVaFrame white(unsigned w,unsigned h){
    bink32_recovered::BinkNativeYuVaFrame f;f.width=w;f.height=h;
    f.planes[0].assign(w*h,235);f.planes[1].assign(((w+1)/2)*((h+1)/2),128);
    f.planes[2]=f.planes[1];f.planes[3].assign(w*h,255);return f;
}
}
int main(){
    Window window;if(!window.hwnd){std::cout<<"SKIP: no native window station\n";return 77;}
    Fixture fixture;if(!fixture.create())return fail("temporary BMP creation");
    Sink sink;Win32OriginalDirectDraw renderer;std::string error;
    if(renderer.initialize(window.hwnd,display::DisplayMode::Windowed,error)||renderer.initialized()||error.find("disabled")==std::string::npos)return fail("legacy factory must be disabled",error);
    if(!renderer.initialize_packed(window.hwnd,error)||!renderer.packed_mode()||renderer.bink_render_target())return fail("packed initialize",error);
    renderer.set_frame_output(&sink);
    ResolvedDraw draw{{"fixture",30,40,{},true},{AssetResolutionStatus::FoundInstalled,fixture.path,{}}};
    if(!renderer.render_ordered({draw},error)||!renderer.present(error))return fail("packed sprite",error);
    if(pixel(sink.latest,30,40)!=0xff0000||pixel(sink.latest,31,40)!=0||
       pixel(sink.latest,30,41)!=0xff||pixel(sink.latest,31,41)!=0xff00)return fail("GDI source pixels / key");
    if(!renderer.capture_presented_underlay(CapturedUnderlay::Modal,error))return fail("capture modal",error);
    if(FAILED(renderer.prepare_native_frame(error))||!renderer.write_native_global_movie(white(640,480),error)||
       !renderer.present(error)||pixel(sink.latest,30,40)!=0xffffff)return fail("global video without DD7",error);
    if(!renderer.render_captured_underlay(CapturedUnderlay::Modal,{},error))return fail("restore modal",error);
    Win32NativeMovieSurface movie,frozen;
    if(!movie.allocate(renderer,3,3,error)||!movie.write(white(3,3),error)||!frozen.clone_from(renderer,movie,error)||
       !frozen.blit(renderer,100,100,error)||!renderer.present(error))return fail("native movie storage / mixed frame",error);
    if(sink.layers!=2||pixel(sink.latest,100,100)!=0xffffff||pixel(sink.latest,30,40)!=0xff0000)return fail("mixed layer output");
    if(!renderer.recreate(window.hwnd,display::DisplayMode::Windowed,error)||!renderer.restore_last_composed_frame(error)||
       renderer.bink_render_target()||!renderer.present(error)||pixel(sink.latest,100,100)!=0xffffff)return fail("recreate without DD7",error);
    IDirectDrawSurface7* forbidden=nullptr;
    if(renderer.create_bink_offscreen_surface(2,2,forbidden,error)||forbidden)return fail("unexpected legacy surface allocation");
    if(GetModuleHandleW(L"ddraw.dll"))return fail("DirectDraw unexpectedly loaded during packed scene test");
    std::cout<<"PASS: Win32 packed scene, bitmap, native global/overlay movie, captured modal and recreation without DD7\n";
    return 0;
}
