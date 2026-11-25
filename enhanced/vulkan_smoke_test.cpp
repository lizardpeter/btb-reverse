#include "vulkan_presenter.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main(){
    using namespace btb;
    const auto hwnd=CreateWindowExW(0,L"STATIC",L"BTB Vulkan 1.4 smoke",WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,800,600,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    if(!hwnd){std::cerr<<"No interactive Windows desktop\n";return 77;}
    ShowWindow(hwnd,SW_SHOWNA);
    auto presenter=enhanced::make_vulkan_presenter();
    std::string error;
    if(!presenter->initialize(hwnd,error)){
        presenter->shutdown();DestroyWindow(hwnd);
        std::cerr<<error<<'\n';
        if(error.starts_with("No Vulkan 1.4 graphics/present device")||error.starts_with("Vulkan 1.4 loader unavailable"))return 77;
        return 1;
    }
    display::PackedFrameSnapshot frame;
    std::vector<uint8_t> pixels(640*480*4);
    for(size_t y=0;y<480;++y)for(size_t x=0;x<640;++x){
        auto i=4*(y*640+x);pixels[i]=uint8_t(x%256);pixels[i+1]=uint8_t(y%256);pixels[i+2]=127;pixels[i+3]=255;
    }
    if(!display::capture_packed_frame(frame,pixels,640*4,640,480,{4,0xff,0xff00,0xff0000}))return 2;
    auto run=[&](unsigned long long target){
        const auto until=std::chrono::steady_clock::now()+std::chrono::seconds(5);
        while(std::chrono::steady_clock::now()<until){
            MSG msg{};while(PeekMessageW(&msg,nullptr,0,0,PM_REMOVE)){TranslateMessage(&msg);DispatchMessageW(&msg);}
            if(!presenter->present(frame,error))return false;
            if(presenter->presented_frames()>=target)return true;
            std::this_thread::sleep_for(std::chrono::milliseconds(8));
        }
        error="Timed out without a successful Vulkan queue present";return false;
    };
    bool ok=run(2);
    if(ok){SetWindowPos(hwnd,nullptr,0,0,1024,768,SWP_NOMOVE|SWP_NOZORDER);ok=run(4);}
    presenter->shutdown();DestroyWindow(hwnd);
    if(!ok){std::cerr<<error<<'\n';return 3;}
    std::cout<<"Vulkan 1.4 queue presentation and resized swapchain passed\n";
    return 0;
}
