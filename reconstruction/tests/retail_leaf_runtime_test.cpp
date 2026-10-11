#include "btb/retail_leaf_runtime.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

using namespace btb::retail_leaf;

namespace {
int polled=0;
int blitted=0;
int goto_calls=0;
int close_calls=0;
void* last_movie=nullptr;
std::int32_t last_frame=-1;
std::int32_t last_flags=-1;
Rect32 last_rect{};
void poll() { ++polled; }
void blit(void* destination,std::int32_t x,std::int32_t y,void* source,
          const Rect32& rect,std::int32_t flags) {
    ++blitted;
    assert(destination==reinterpret_cast<void*>(0xAA));
    assert(source==reinterpret_cast<void*>(0xBB));
    assert(x==280&&y==190&&flags==1);
    last_rect=rect;
}
void goto_frame(void* movie,std::int32_t frame,std::int32_t flags) {
    ++goto_calls;last_movie=movie;last_frame=frame;last_flags=flags;
}
void close_movie(void* movie) { ++close_calls;last_movie=movie; }
}

int main() {
    // 00402AF0/00408430/00408440/00408450: exact global mutations.
    no_op_legacy_hook();
    std::int32_t gate=-1;
    enable_input_processing(gate);
    assert(gate==1);
    disable_input_processing(gate);
    assert(gate==0);
    std::int32_t unknown_flag=23;
    clear_original_flag_004fbe54(unknown_flag);
    assert(unknown_flag==0);

    // 00407AC0 and 00407A70/00407A90: original cursor state and clamps.
    btb::input::MouseBounds bounds{-1,-1,-1,-1};
    reset_mouse_bounds(bounds);
    assert(bounds.min_x==2 && bounds.min_y==2);
    assert(bounds.max_x==637 && bounds.max_y==477);

    btb::input::CursorState cursor{-10,-20,11,12};
    set_cursor_hotspot(cursor,2,3);
    assert(cursor.hotspot_x==2 && cursor.hotspot_y==3);
    clear_cursor_hotspot_and_clamp_nonnegative(cursor);
    assert(cursor.x==0&&cursor.y==0);
    assert(cursor.hotspot_x==0&&cursor.hotspot_y==0);
    set_cursor_hotspot(cursor,8,9);
    assert(cursor.hotspot_x==8&&cursor.hotspot_y==9);

    // 004079E0: no polling while cursor surface is not the loading sprite.
    LoadingCursorState loading{};
    loading.default_surface=reinterpret_cast<void*>(0xCC);
    loading.loading_surface=reinterpret_cast<void*>(0xBB);
    loading.current_surface=reinterpret_cast<void*>(0xDD);
    loading.current_frame=3;
    update_loading_cursor_animation(loading,poll);
    assert(polled==0 && loading.current_frame==3);

    loading.current_surface=loading.loading_surface;
    loading.current_frame=5;
    update_loading_cursor_animation(loading,poll);
    assert(polled==1 && loading.current_frame==0);
    assert(loading.source_rectangle.left==0);
    assert(loading.source_rectangle.top==0);
    assert(loading.source_rectangle.right==100);
    assert(loading.source_rectangle.bottom==100);
    for(int i=0;i<5;++i)update_loading_cursor_animation(loading,poll);
    assert(polled==6&&loading.current_frame==5);
    assert(loading.source_rectangle.left==500);
    assert(loading.source_rectangle.right==600);
    // 00407A40 has fixed 280,190 blit destination; source rect follows frame.
    draw_loading_cursor_animation(
        reinterpret_cast<void*>(0xAA),loading,blit);
    assert(blitted==1&&last_rect.left==500&&last_rect.bottom==100);
    // 004079C0 does not reset source frame, only the 2 latches and surface.
    loading.completion_latch_004fbfc4=23;
    loading.completion_latch_0051c33c=19;
    end_loading_cursor_animation(loading);
    assert(loading.current_surface==loading.default_surface);
    assert(loading.completion_latch_004fbfc4==0);
    assert(loading.completion_latch_0051c33c==0);
    assert(loading.current_frame==5);

    // 00409CA0 inclusive Dino hitbox; 00409C60 axis-aligned tolerance.
    cursor={10,20,0,0};
    const Rect32 box{10,20,40,50};
    assert(dino_cursor_inside_rect(cursor,box));
    assert(dino_cursor_near_point(cursor,12,17,3));
    assert(!dino_cursor_near_point(cursor,12,16,3));
    assert(!dino_cursor_near_point(cursor,12,17,-1));
    assert(dino_cursor_inside_rect({40,50,0,0},box));
    assert(!dino_cursor_inside_rect({41,50,0,0},box));

    // 0040DD20 excludes edges; 004294D0 includes them.
    assert(!point_inside_rect_exclusive(10,20,box));
    assert(point_inside_rect_exclusive(11,21,box));
    assert(!point_inside_rect_exclusive(40,49,box));
    assert(point_in_rect_inclusive(10,20,box));
    assert(point_in_rect_inclusive(40,50,box));
    assert(!point_in_rect_inclusive(10,51,box));

    // 00428520 returns 0=inside and 1=outside.
    std::vector<btb::herding::Vec2i> poly{
        {0,0},{100,0},{100,100},{0,100}
    };
    const auto inside=btb::retail_geometry::original_point_in_polygon_status(
        poly,50,50);
    const auto outside=btb::retail_geometry::original_point_in_polygon_status(
        poly,150,50);
    assert(inside && *inside==0);
    assert(outside && *outside==1);

    // 00413420 and 0041FC60 use signed low DWORD only.
    static_assert(filetime_delta_milliseconds(0,100000)==10);
    static_assert(filetime_delta_centiseconds(0,100000)==1);
    static_assert(filetime_delta_milliseconds(100000,0)==-10);
    constexpr auto intmax=std::numeric_limits<std::int32_t>::max();
    constexpr auto intmin=std::numeric_limits<std::int32_t>::min();
    static_assert(filetime_delta_milliseconds(intmax,intmin)==0);
    static_assert(filetime_delta_milliseconds(intmin,intmax)==0);

    // 0041A700: full 64-bit unsigned FILETIME difference, 10M ticks/second.
    static_assert(filetime_delta_seconds({0,0},{10000000,0})==1);
    static_assert(filetime_delta_seconds({0xFFFFFFFFU,0},
                                         {0xFFFFFFFFU,1})==429);
    // 00412B50 and 0041F3F0 use signed IDIV, including negative remainder.
    static_assert(decode_fireworks_cell(13).row==2);
    static_assert(decode_fireworks_cell(13).column==1);
    static_assert(decode_bobs_band_cell(49).row==2);
    static_assert(decode_bobs_band_cell(49).column==1);
    static_assert(decode_fireworks_cell(-7).row==-1);
    static_assert(decode_fireworks_cell(-7).column==-1);

    // 00423610 resets the two original delay globals (not random).
    PilchardSpawnState pilchard{87,12};
    reset_pilchard_spawn_delay(pilchard);
    assert(pilchard.value_005109cc==0);
    assert(pilchard.delay_005107e8==500);

    // 004048E0 returns CWaveFile+0x30 without recomputing the sample length.
    btb::dsutil::RetailCWaveFile32 wave{};
    wave.m_dwSize=0xDEADBEEFU;
    assert(wave_file_get_size(wave)==0xDEADBEEFU);

    // 00405330/00405350 and 00406970/00406980/004069B0.
    btb::printing::RetailBitmapPrinter32 printer{};
    printer.initialize_retail_defaults();
    printer_set_document_name(printer,"Bob Builds a Park");
    assert(std::string(printer_get_document_name(printer))=="Bob Builds a Park");
    const btb::display::RetailRect32 requested{12,34,56,78};
    printer_set_target_rect(printer,requested);
    const auto r=printer_get_target_rect(printer);
    assert(r.left==12&&r.top==34&&r.right==56&&r.bottom==78);
    assert(printer_get_scale_mode(printer)==3);

    // 00408D80 and 00408EA0 dispatch actual original API parameters.
    void* movie=reinterpret_cast<void*>(0xB10C);
    restart_bink_movie(movie,goto_frame);
    assert(goto_calls==1&&last_movie==movie);
    assert(last_frame==1&&last_flags==0);
    close_bink_movie(movie,close);
    assert(close_calls==1&&last_movie==movie);

    // 00415D30 x87-length semantic model (host rounding may differ).
    assert(distance_between_integer_points(0,0,3,4)==5.0L);
    assert(distance_between_integer_points(-3,4,0,0)==5.0L);
    assert(distance_between_integer_points(12,13,12,13)==0.0L);
}
