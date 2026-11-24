#include "btb/original_game_batch_13.hpp"
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>

namespace btb::original_game13 {
using U32=std::uint32_t;
void construct_display_manager(btb::display::RetailDisplayManager32& d) noexcept {
    d.vtable_ptr32=btb::display::kDisplayManagerVtable32;
    d.direct_draw7_ptr32=0;
    d.primary_surface_ptr32=0;
    d.render_surface_ptr32=0;
    d.unused_legacy_surface_ptr32=0;
}
void construct_wave_file(btb::dsutil::RetailCWaveFile32& w) noexcept {
    w.m_pwfx=0;
    w.m_hmmio=0;
    w.m_dwSize=0;
    w.m_bIsReadingFromMemory=0;
}
void construct_bitmap_printer(btb::printing::RetailBitmapPrinter32& p) noexcept {
    p.dib_allocation_ptr32=0;
    p.dib_bits_ptr32=0;
    p.scale_mode=3;
    p.vtable_ptr32=btb::printing::kBitmapPrinterVtable32;
    p.document_name.fill('\0');
    p.target_rect={};
}
void construct_sound_manager(btb::dsutil::RetailCSoundManager32& s) noexcept {
    s.m_pDS=0;
}
Rect squirrel_frame_rectangle(I32 frame) noexcept {
    const auto left=(frame/9+(frame%9)*4)*89;
    return {left,0,left+89,133};
}
I32 compare_park_sort_layers(
    I32 type_a,I32 layer_a,I32 type_b,I32 layer_b) noexcept {
    if(type_a==-1)return type_b==-1?0:-1;
    if(type_b==-1)return 1;
    return layer_a<=layer_b?-1:1;
}
bool can_place_grand_opening(
    std::span<const I32> grid,I32 row,I32 column,I32 length) noexcept {
    if(length<=0)return true;
    if(row<0||column<0||column>=24)return false;
    const auto index=static_cast<std::int64_t>(row)*24+column;
    if(index<0||index>=static_cast<std::int64_t>(grid.size()))
        return false;
    for(I32 i=0;i<length;++i){
        const auto item=index+i;
        if(column+i>=24||item>=static_cast<std::int64_t>(grid.size())||
           grid[static_cast<std::size_t>(item)]!=-1)return false;
    }
    return true;
}
bool can_place_firework(
    std::span<const I32> grid,I32 row,I32 column,I32 length) noexcept {
    if(length<=0)return true;
    const auto index=static_cast<std::int64_t>(row)*6+column;
    if(index<0||index>=static_cast<std::int64_t>(grid.size()))
        return false;
    for(I32 i=0;i<length;++i){
        const auto item=index+i;
        if(item>=static_cast<std::int64_t>(grid.size())||
           grid[static_cast<std::size_t>(item)]!=-1)return false;
    }
    return true;
}
I32 remove_firework(
    std::span<I32> grid,std::span<const I32> lengths,I32 row,I32 column) noexcept {
    if(lengths.empty()||can_place_firework(grid,row,column,lengths[0]))
        return -1;
    const auto index=static_cast<std::int64_t>(row)*6+column;
    if(index<0||index>=static_cast<std::int64_t>(grid.size()))return -1;
    const auto item=grid[static_cast<std::size_t>(index)];
    if(item<0||static_cast<std::size_t>(item)>=lengths.size())return -1;
    const auto length=lengths[static_cast<std::size_t>(item)];
    if(length<=0||index+length>static_cast<std::int64_t>(grid.size()))
        return -1;
    for(I32 i=0;i<length;++i)
        grid[static_cast<std::size_t>(index+i)]=-1;
    return item;
}
I32 hit_test_park_toolbar(
    std::span<const Rect> table,I32 x,I32 y) noexcept {
    for(std::size_t i=0;i<table.size();++i){
        const auto& b=table[i];
        if(b.left==-1)break;
        if(b.left<x&&x<b.right&&b.top<y&&y<b.bottom)
            return static_cast<I32>(i);
    }
    return -1;
}
void clear_bandstand(std::span<BandstandRecord> records,I32 selector) noexcept {
    for(auto& item:records){
        if((selector==0&&item.category>=0&&item.category<=2)||
           (selector==1&&item.category==2))
            item.active_object=-1;
    }
}
void clear_pond(
    std::span<PondRecord> records,I32 selector,PondGlobals& g) noexcept {
    for(auto& item:records){
        if(selector==0){
            item.primary_type=-1;g.value_00507a28=-1;
        }else if(selector==1&&item.primary_type==7){
            item.primary_type=-1;g.value_005079fc=-1;
        }else if(selector==2&&item.primary_type!=7&&item.primary_type<100){
            item.primary_type=-1;
        }
    }
}
I32 compare_herding_depth(
    const HerdingSortEntry& a,const HerdingSortEntry& b) noexcept {
    if(a.category==15||a.category==16)return -1;
    if(b.category==15||b.category==16)return 1;
    const U32 av=static_cast<U32>(a.bottom)-static_cast<U32>(a.top)+
                 static_cast<U32>(a.origin_y);
    const U32 bv=static_cast<U32>(b.bottom)-static_cast<U32>(b.top)+
                 static_cast<U32>(b.origin_y);
    return std::bit_cast<I32>(bv)<std::bit_cast<I32>(av)?1:-1;
}
bool verify_13_original_game_calls() noexcept {
    btb::display::RetailDisplayManager32 disp{};
    disp.direct_draw7_ptr32=9;construct_display_manager(disp);
    if(disp.vtable_ptr32!=btb::display::kDisplayManagerVtable32||
       disp.direct_draw7_ptr32!=0)return false;
    btb::dsutil::RetailCWaveFile32 wave{};
    wave.m_pwfx=2;wave.m_dwSize=20;construct_wave_file(wave);
    if(wave.m_dwSize!=0||wave.m_pwfx!=0)return false;
    btb::printing::RetailBitmapPrinter32 printer{};
    printer.dib_bits_ptr32=22;construct_bitmap_printer(printer);
    if(printer.dib_bits_ptr32!=0||printer.scale_mode!=3)return false;
    btb::dsutil::RetailCSoundManager32 sound{16};
    construct_sound_manager(sound);if(sound.m_pDS!=0)return false;
    const auto sq=squirrel_frame_rectangle(2);
    if(sq.left!=712||sq.right!=801||sq.bottom!=133)return false;
    if(compare_park_sort_layers(-1,0,-1,0)!=0||
       compare_park_sort_layers(0,4,0,5)!=-1||
       compare_park_sort_layers(0,6,0,5)!=1)return false;
    std::array<I32,48> grand{};
    grand.fill(-1);
    if(!can_place_grand_opening(grand,1,22,2)||
       can_place_grand_opening(grand,1,22,3))return false;
    std::array<I32,12> fireworks{};
    fireworks.fill(-1);
    const std::array<I32,2> lengths{1,3};
    if(!can_place_firework(fireworks,0,5,3))return false;
    fireworks[5]=1;fireworks[6]=15;fireworks[7]=15;
    if(remove_firework(fireworks,lengths,0,5)!=1||
       fireworks[5]!=-1||fireworks[7]!=-1)return false;
    std::array<Rect,2> tool{{{0,0,10,10},{-1,0,0,0}}};
    if(hit_test_park_toolbar(tool,5,5)!=0||
       hit_test_park_toolbar(tool,0,5)!=-1)return false;
    std::array<BandstandRecord,3> stands{{{14,0},{15,2},{16,4}}};
    clear_bandstand(stands,1);
    if(stands[0].active_object!=14||stands[1].active_object!=-1||
       stands[2].active_object!=16)return false;
    std::array<PondRecord,3> pond{{{7},{5},{150}}};
    PondGlobals globals{};
    clear_pond(pond,1,globals);
    if(pond[0].primary_type!=-1||pond[1].primary_type!=5||
       globals.value_005079fc!=-1)return false;
    const HerdingSortEntry a{0,0,20,20}, b{0,0,5,5};
    return btb::original_game13::compare_herding_depth(a,b)==1 &&
           btb::original_game13::compare_herding_depth(b,a)==-1 &&
           btb::original_game13::compare_herding_depth(HerdingSortEntry{15,0,0,0},b)==-1;
}
}
