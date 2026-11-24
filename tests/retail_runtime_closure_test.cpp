#include "btb/retail_runtime_closure.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

using namespace btb::retail_closure;

namespace {
std::vector<int> events;
std::uint32_t fill(void* dest,const std::array<std::uint32_t,25>& fx,
                   std::uint32_t flags) noexcept {
    assert(dest==reinterpret_cast<void*>(0x1));
    assert(flags==0x400U);
    assert(fx[0]==100U&&fx[20]==0xAABBCCDDU);
    for(std::size_t i=1;i<fx.size();++i)if(i!=20)assert(fx[i]==0U);
    events.push_back(1);
    return 0x887601C2U;
}
std::uint32_t fast(void* dest,std::uint32_t x,std::uint32_t y,
                   void* src,const void* rect,std::uint32_t flags) noexcept {
    assert(dest==reinterpret_cast<void*>(0x1));
    assert(x==300&&y==200&&src==reinterpret_cast<void*>(0x2));
    assert(rect==reinterpret_cast<void*>(0x3));
    assert(flags==0x8000);
    events.push_back(2);
    return 0x8876021CU;
}
std::uint32_t restore(void* dd) noexcept {
    assert(dd==reinterpret_cast<void*>(0x4));
    events.push_back(3);
    return 0x80004005U;
}
bool client(std::uintptr_t hwnd,btb::display::RetailRect32& r) noexcept {
    assert(hwnd==0xBEEF);
    events.push_back(4);
    r={0,0,640,480};
    return true;
}
void client_to_screen(std::uintptr_t hwnd,std::int32_t& x,
                      std::int32_t& y) noexcept {
    assert(hwnd==0xBEEF);
    events.push_back(5);
    x+=10;y+=20;
}
std::int32_t metrics(std::int32_t index) noexcept {
    events.push_back(index==0?6:7);
    return index==0?1920:1080;
}
bool is_order(std::initializer_list<int> expected){
    return events==std::vector<int>(expected);
}
}

int main() {
    // 00411010: write type head, type+14 for all continuation cells.
    std::array<std::int32_t,18> fireworks{};
    fireworks.fill(-1);
    constexpr std::array<std::int32_t,3> fw_lengths{1,2,3};
    assert(place_firework(fireworks,fw_lengths,0,5,1,false));
    assert(fireworks[5]==-1);
    assert(place_firework(fireworks,fw_lengths,0,5,1,true));
    assert(fireworks[5]==1&&fireworks[6]==15);
    assert(!place_firework(fireworks,fw_lengths,0,5,1,true));
    assert(place_firework(fireworks,fw_lengths,0,1,2,true));
    assert(fireworks[1]==2&&fireworks[2]==16&&fireworks[3]==16);
    assert(!place_firework(fireworks,fw_lengths,2,5,1,true));

    // 00412B80: only action 12, only head types [0,12).
    assert(!delete_selected_firework(fireworks,fw_lengths,11,5));
    assert(!delete_selected_firework(fireworks,fw_lengths,12,6));
    assert(fireworks[5]==1&&fireworks[6]==15);
    assert(delete_selected_firework(fireworks,fw_lengths,12,5));
    assert(fireworks[5]==-1&&fireworks[6]==-1);
    assert(!delete_selected_firework(fireworks,fw_lengths,12,5));
    assert(delete_selected_firework(fireworks,fw_lengths,12,1));
    assert(fireworks[1]==-1&&fireworks[2]==-1&&fireworks[3]==-1);

    // 0041F430: Grand Opening tail markers encode >9 and must be walked
    // backwards to the original head before deleting the whole event.
    std::array<std::int32_t,48> grand{};
    grand.fill(-1);
    constexpr std::array<std::int32_t,10> lengths{1,1,3,1,1,1,1,1,1,1};
    grand[3]=2;grand[4]=12;grand[5]=12;
    assert(remove_grand_opening_event(grand,lengths,0,5)==2);
    assert(grand[3]==-1&&grand[4]==-1&&grand[5]==-1);
    assert(remove_grand_opening_event(grand,lengths,0,5)==-1);
    // Do not cross a row boundary when finding a head.
    grand[24]=12;
    assert(remove_grand_opening_event(grand,lengths,1,0)==-1);
    assert(grand[24]==12);

    // 00403540: null surface returns E_POINTER without callback.
    events.clear();
    assert(clear_back_buffer(nullptr,0xAABBCCDDU,fill)==0x80004003U);
    assert(events.empty());
    assert(clear_back_buffer(reinterpret_cast<void*>(0x1),0xAABBCCDDU,fill)==
           0x887601C2U);
    assert(is_order({1}));

    // 00403500: BltFast passes through original flags and HRESULT.
    events.clear();
    assert(blt_fast_to_back_buffer(nullptr,300,200,nullptr,nullptr,0,fast)==
           0x80004003U);
    assert(events.empty());
    assert(blt_fast_to_back_buffer(reinterpret_cast<void*>(0x1),300,200,
           reinterpret_cast<void*>(0x2),reinterpret_cast<void*>(0x3),0x8000,fast)
           ==0x8876021CU);
    assert(is_order({2}));

    // 00402620: RestoreAllSurfaces collapses every successful HRESULT to 0,
    // while preserving negative HRESULTs exactly.
    static_assert(restore_surfaces_result(0U)==0U);
    static_assert(restore_surfaces_result(1U)==0U);
    static_assert(restore_surfaces_result(0x7FFFFFFFU)==0U);
    static_assert(restore_surfaces_result(0x80004005U)==0x80004005U);
    events.clear();
    assert(restore_all_surfaces(reinterpret_cast<void*>(0x4),restore)==
           0x80004005U);
    assert(is_order({3}));

    // 00403590: native Windowed rect = GetClientRect followed by two
    // ClientToScreen calls. Fullscreen invokes metrics index0 then index1.
    btb::display::RetailDisplayManager32 display{};
    display.windowed=1;display.hwnd32=0xBEEF;
    events.clear();
    update_display_destination_rect(display,client,client_to_screen,metrics);
    assert(is_order({4,5,5}));
    assert(display.destination_rect.left==10&&display.destination_rect.top==20);
    assert(display.destination_rect.right==650&&display.destination_rect.bottom==500);
    events.clear();display.windowed=0;
    update_display_destination_rect(display,client,client_to_screen,metrics);
    assert(is_order({6,7}));
    assert(display.destination_rect.left==0&&display.destination_rect.top==0);
    assert(display.destination_rect.right==1920&&display.destination_rect.bottom==1080);

    // 00414A30 trig model: conversion factor comes from original global.
    const auto v0=vector_from_degrees_magnitude(0,20,0.017453292F);
    assert(std::abs(v0.x-20.0F)<0.0001F);
    assert(std::abs(v0.y)<0.0001F);
    const auto v90=vector_from_degrees_magnitude(90,20,0.017453292F);
    assert(std::abs(v90.x)<0.001F&&std::abs(v90.y-20.0F)<0.001F);

    // 0041AE90 comparison threshold is explicitly input, not assumed 0.
    static_assert(float_abs_with_retail_threshold(-3.0F,0.0F)==3.0L);
    static_assert(float_abs_with_retail_threshold(3.0F,0.0F)==3.0L);
    static_assert(float_abs_with_retail_threshold(2.0F,3.0F)==-2.0L);
    static_assert(float_abs_with_retail_threshold(-2.0F,-4.0F)==-2.0L);
}
