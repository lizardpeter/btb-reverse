#include "btb/original_leaf_batch_25.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <string_view>

namespace btb::original_leaf {

void no_op_legacy_hook() noexcept {} // 00402AF0

void reset_mouse_bounds(btb::input::MouseBounds& b) noexcept {
    // 00407AC0 writes MAX-X, MIN-Y, MIN-X, MAX-Y, in this order.
    b.max_x=637;b.min_y=2;b.min_x=2;b.max_y=477;
}
void enable_input_processing(I32& gate) noexcept {gate=1;} // 00408440
void disable_input_processing(I32& gate) noexcept {gate=0;} // 00408450
void clear_004fbe54(I32& flag) noexcept {flag=0;} // 00408430

void set_cursor_hotspot(btb::input::CursorState& s,I32 x,I32 y) noexcept {
    s.hotspot_x=x;s.hotspot_y=y; // 00407A70
}
void clear_cursor_hotspot(btb::input::CursorState& s) noexcept {
    // 00407A90 only clamps negative coordinates.
    s.hotspot_x=0;s.hotspot_y=0;
    if(s.x<0)s.x=0;
    if(s.y<0)s.y=0;
}
void end_loading_cursor_animation(LoadingCursorGlobals& g) noexcept {
    // 004079C0 restores the default surface and clears two separate latches.
    g.current_surface=g.default_surface;
    g.completion_004fbfc4=0;
    g.completion_0051c33c=0;
}

bool is_dino_cursor_inside_rect(
    const btb::input::CursorState& c,const Rect& r) noexcept {
    return r.left<=c.x&&c.x<=r.right&&r.top<=c.y&&c.y<=r.bottom;
}
namespace {
[[nodiscard]] I32 x86_abs_diff(I32 a,I32 b) noexcept {
    const U32 delta=static_cast<U32>(a)-static_cast<U32>(b);
    const U32 mask=(delta&0x80000000U)?0xffffffffU:0U;
    return std::bit_cast<I32>((delta^mask)-mask);
}
}
bool is_dino_cursor_near_point(
    const btb::input::CursorState& c,I32 x,I32 y,I32 tolerance) noexcept {
    return x86_abs_diff(c.x,x)<=tolerance &&
           x86_abs_diff(c.y,y)<=tolerance;
}
bool point_inside_rect_exclusive(I32 x,I32 y,const Rect& r) noexcept {
    return r.left<x&&x<r.right&&r.top<y&&y<r.bottom;
}
bool point_inside_rect_inclusive(I32 x,I32 y,const Rect& r) noexcept {
    return r.left<=x&&x<=r.right&&r.top<=y&&y<=r.bottom;
}
I32 filetime_delta_milliseconds(I32 old_low,I32 new_low) noexcept {
    const auto delta=std::bit_cast<I32>(
        static_cast<U32>(new_low)-static_cast<U32>(old_low));
    return delta/10000;
}
I32 filetime_delta_centiseconds(I32 old_low,I32 new_low) noexcept {
    const auto delta=std::bit_cast<I32>(
        static_cast<U32>(new_low)-static_cast<U32>(old_low));
    return delta/100000;
}
U64 unsigned_filetime_delta_seconds(
    std::array<U32,2> before,std::array<U32,2> after) noexcept {
    const U64 start=(static_cast<U64>(before[1])<<32U)|before[0];
    const U64 end=(static_cast<U64>(after[1])<<32U)|after[0];
    // 0041A700 uses unsigned subtraction including carry/borrow.
    return (end-start)/10000000ULL;
}
Cell decode_fireworks_cell(I32 selected) noexcept {
    return {selected/6,selected%6};
}
Cell decode_bobs_band_cell(I32 selected) noexcept {
    return {selected/24,selected%24};
}
void reset_pilchard_spawn_delay(PilchardGlobals& s) noexcept {
    s.value_005109cc=0;s.delay_005107e8=500;
}
U32 wave_file_get_size(const btb::dsutil::RetailCWaveFile32& w) noexcept {
    return w.m_dwSize;
}
void printer_set_document_name(
    btb::printing::RetailBitmapPrinter32& p,std::string_view name) noexcept {
    // 00405330 uses lstrcpynA with a 260-byte destination; copying
    // at most 259 bytes and terminating matches its normal string path.
    const auto nul=name.find('\0');
    const auto length=std::min<std::size_t>(
        nul==std::string_view::npos?name.size():nul,
        p.document_name.size()-1U);
    std::memcpy(p.document_name.data(),name.data(),length);
    p.document_name[length]='\0';
}
void printer_set_target_rect(
    btb::printing::RetailBitmapPrinter32& p,const Rect& r) noexcept {
    p.target_rect=r;
    p.scale_mode=3;
}
const char* printer_get_document_name(
    const btb::printing::RetailBitmapPrinter32& p) noexcept {
    return p.document_name.data();
}
Rect printer_get_target_rect(
    const btb::printing::RetailBitmapPrinter32& p) noexcept {
    return p.target_rect;
}
I32 printer_get_scale_mode(
    const btb::printing::RetailBitmapPrinter32& p) noexcept {
    return p.scale_mode;
}
long double distance_between_integer_points(
    I32 x1,I32 y1,I32 x2,I32 y2) noexcept {
    const auto dx=static_cast<long double>(x86_abs_diff(x1,x2));
    const auto dy=static_cast<long double>(x86_abs_diff(y1,y2));
    return std::sqrt(dx*dx+dy*dy);
}

bool verify_25_original_leaf_calls() noexcept {
    no_op_legacy_hook();
    btb::input::MouseBounds bounds{8,9,10,11};
    reset_mouse_bounds(bounds);
    if(bounds.min_x!=2||bounds.max_x!=637||
       bounds.min_y!=2||bounds.max_y!=477)return false;
    I32 flag=9;
    enable_input_processing(flag);if(flag!=1)return false;
    disable_input_processing(flag);if(flag!=0)return false;
    flag=9;clear_004fbe54(flag);if(flag!=0)return false;
    btb::input::CursorState cursor{-8,6,0,0};
    set_cursor_hotspot(cursor,7,4);
    if(cursor.hotspot_x!=7||cursor.hotspot_y!=4)return false;
    clear_cursor_hotspot(cursor);
    if(cursor.x!=0||cursor.y!=6||
       cursor.hotspot_x!=0||cursor.hotspot_y!=0)return false;
    LoadingCursorGlobals animation{reinterpret_cast<void*>(1),
                                   reinterpret_cast<void*>(2),1,2};
    end_loading_cursor_animation(animation);
    if(animation.current_surface!=animation.default_surface ||
       animation.completion_004fbfc4!=0 ||
       animation.completion_0051c33c!=0)return false;
    const Rect box{0,0,10,10};
    if(!is_dino_cursor_inside_rect(cursor,box) ||
       !is_dino_cursor_near_point(cursor,0,6,0) ||
       is_dino_cursor_near_point(cursor,20,20,2) ||
       point_inside_rect_exclusive(0,0,box) ||
       !point_inside_rect_exclusive(1,1,box) ||
       !point_inside_rect_inclusive(0,0,box))return false;
    if(filetime_delta_milliseconds(0,100000)!=10 ||
       filetime_delta_centiseconds(0,100000)!=1 ||
       unsigned_filetime_delta_seconds({0,0},{10000000,0})!=1)return false;
    const auto fire=decode_fireworks_cell(13);
    const auto band=decode_bobs_band_cell(25);
    if(fire.row!=2||fire.column!=1||band.row!=1||band.column!=1)return false;
    PilchardGlobals pilchard{12,19};
    reset_pilchard_spawn_delay(pilchard);
    if(pilchard.value_005109cc!=0||pilchard.delay_005107e8!=500)return false;
    btb::dsutil::RetailCWaveFile32 wave{};
    wave.m_dwSize=0x12345678U;
    if(wave_file_get_size(wave)!=0x12345678U)return false;
    btb::printing::RetailBitmapPrinter32 printer{};
    printer_set_document_name(printer,"Original Game Printer");
    const Rect print_rect{10,20,30,40};
    printer_set_target_rect(printer,print_rect);
    const auto got=printer_get_target_rect(printer);
    if(std::strcmp(printer_get_document_name(printer),
                   "Original Game Printer")!=0 ||
       got.left!=10||got.top!=20||got.right!=30||got.bottom!=40 ||
       printer_get_scale_mode(printer)!=3)return false;
    return distance_between_integer_points(0,0,3,4)==5.0L;
}

} // namespace btb::original_leaf
