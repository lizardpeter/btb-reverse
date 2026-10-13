#include "btb/original_gameplay9.hpp"
#include <array>
#include <cmath>
#include <cstdint>
namespace btb::original_gameplay9 {
namespace {
int polls{},blits{},seeks{},closes{};
std::int32_t last_keyframe{},last_mode{},last_x{},last_y{};
void poll(){++polls;}
void blit(void*,std::int32_t x,std::int32_t y,void*,
          const btb::retail_leaf::Rect32& rect,std::int32_t flags){
 if(flags==1&&rect.right-rect.left==100){
  ++blits;last_x=x;last_y=y;
 }
}
void goto_frame(void*,std::int32_t frame,std::int32_t mode){
 ++seeks;last_keyframe=frame;last_mode=mode;
}
void close(void*){++closes;}
}
bool verify_9_original_gameplay_math_functions() noexcept {
 // Original 004079E0 and 00407A40, loading cursor poll-before-advance,
 // six authored frames of 100x100 and DirectDraw blit x280/y190.
 polls=blits=seeks=closes=0;
 LoadingCursorState cursor{};
 cursor.loading_surface=reinterpret_cast<void*>(0x10);
 cursor.current_surface=cursor.loading_surface;
 cursor.current_frame=5;
 original_gameplay9::update_loading_cursor_animation(cursor,poll);
 if(polls!=1||cursor.current_frame!=0||
    cursor.source_rectangle.left!=0||
    cursor.source_rectangle.right!=100)return false;
 original_gameplay9::update_loading_cursor_animation(cursor,poll);
 if(polls!=2||cursor.current_frame!=1||
    cursor.source_rectangle.left!=100||
    cursor.source_rectangle.right!=200)return false;
 original_gameplay9::draw_loading_cursor_animation(
     reinterpret_cast<void*>(0x11),cursor,blit);
 if(blits!=1||last_x!=280||last_y!=190)return false;
 cursor.current_surface=nullptr;
 original_gameplay9::update_loading_cursor_animation(cursor,poll);
 if(polls!=2)return false;
 // Original 00408D80 always BinkGoto(movie,1,0); 00408EA0 close.
 original_gameplay9::restart_bink_movie(
     reinterpret_cast<void*>(0x123),goto_frame);
 original_gameplay9::close_bink_movie(
     reinterpret_cast<void*>(0x123),close);
 if(seeks!=1||last_keyframe!=1||last_mode!=0||closes!=1)return false;
 // Original 00411010 tests flat 6-wide Fireworks contiguous placements,
 // commits head type and continuation type+14, even across row boundary.
 std::array<std::int32_t,18> cells{};
 cells.fill(-1);
 std::array<std::int32_t,12> type_lengths{};type_lengths.fill(1);
 type_lengths[0]=2;type_lengths[2]=3;
 if(!original_gameplay9::place_firework(
     cells,type_lengths,0,5,2,true) ||
    cells[5]!=2||cells[6]!=16||cells[7]!=16)return false;
 if(original_gameplay9::place_firework(
     cells,type_lengths,0,5,2,false))return false;
 // Original 00412B80 refuses removing a continuation cell (>=12);
 // selecting the head invokes the original removal routine.
 if(original_gameplay9::delete_selected_firework(
     cells,type_lengths,12,6))return false;
 if(!original_gameplay9::delete_selected_firework(
     cells,type_lengths,12,5))return false;
 if(cells[5]!=-1||cells[6]!=-1||cells[7]!=-1)return false;
 // Original 0041F430 scans backwards from Grand Opening continuation.
 std::array<std::int32_t,48> band{};
 band.fill(-1);band[0]=0;band[1]=10;
 const std::array<std::int32_t,2> lengths{2,3};
 if(original_gameplay9::remove_grand_opening_event(
     band,lengths,0,1)!=0||
    band[0]!=-1||band[1]!=-1)return false;
 if(original_gameplay9::remove_grand_opening_event(
     band,lengths,0,1)!=-1)return false;
 // Original x87 angle and FloatAbs models: model-level representative
 // numerical assertions; no bit-for-bit x87 parity is claimed.
 constexpr float pi_over_180=0.017453292519943295769f;
 const auto v=original_gameplay9::vector_from_degrees_magnitude(
     90,10,pi_over_180);
 if(std::abs(v.x)>0.01f||std::abs(v.y-10.0f)>0.01f)
    return false;
 if(original_gameplay9::float_abs_with_retail_threshold(-3.0f,0.f)!=3.0L||
    original_gameplay9::float_abs_with_retail_threshold(-3.f,-4.f)!=-3.0L)
    return false;
 return true;
}
}
