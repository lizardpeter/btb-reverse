#include "btb/original_gameplay9.hpp"
namespace btb::original_gameplay9 {
using namespace btb::retail_leaf;
using namespace btb::retail_closure;
// Original 0x004079E0
void update_loading_cursor_animation(
    LoadingCursorState& s, void (*poll_direct_input)()) {
    if (s.current_surface != s.loading_surface) return;
    // 004079E0 polls the input system BEFORE advancing the sprite frame.
    if (poll_direct_input) poll_direct_input();
    ++s.current_frame;
    if (s.current_frame > 5) s.current_frame = 0;
    s.source_rectangle = {s.current_frame * 100, 0,
                          s.current_frame * 100 + 100, 100};
}

// Original 0x00407A40
void draw_loading_cursor_animation(
    void* destination, const LoadingCursorState& s,
    LoadingCursorBlit blit) {
    if (blit) blit(destination, 0x118, 0xBE,
                   s.current_surface, s.source_rectangle, 1);
}

// Original 0x00408D80
void restart_bink_movie(void* movie,BinkGoto goto_frame) {
    if (goto_frame) goto_frame(movie,1,0);
}

// Original 0x00408EA0
void close_bink_movie(void* movie,BinkClose close) {
    if (close) close(movie);
}

// Original 0x00411010
[[nodiscard]] bool place_firework(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t row,std::int32_t col,std::int32_t type,
    bool commit) noexcept {
    if(type<0 || type>=static_cast<std::int32_t>(type_lengths.size()))
        return false; // host safety
    const auto count=type_lengths[static_cast<std::size_t>(type)];
    if(!btb::retail_shared::can_place_firework(grid,row,col,count))
        return false;
    if(!commit) return true;
    const std::int64_t start=static_cast<std::int64_t>(row)*6+col;
    if(start<0 || start>=static_cast<std::int64_t>(grid.size()))
        return false;
    grid[static_cast<std::size_t>(start)]=type;
    for(std::int32_t i=1;i<count;++i)
        grid[static_cast<std::size_t>(start+i)]=type+14;
    return true;
}

// Original 0x00412B80
[[nodiscard]] bool delete_selected_firework(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t action,std::int32_t selected_cell) noexcept {
    if(action!=12 || selected_cell<0 ||
       selected_cell>=static_cast<std::int32_t>(grid.size()))
        return false;
    const auto type=grid[static_cast<std::size_t>(selected_cell)];
    if(type<0 || type>=12) return false;
    return btb::retail_shared::remove_firework(
        grid,type_lengths,selected_cell/6,selected_cell%6)>=0;
}

// Original 0x0041F430
[[nodiscard]] std::int32_t remove_grand_opening_event(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t row,std::int32_t col) noexcept {
    if(!btb::retail_shared::can_place_grand_opening(
           grid,row,col,type_lengths.empty()?0:type_lengths[0]))
        {
            const auto begin=static_cast<std::int64_t>(row)*24;
            std::int64_t idx=begin+col;
            if(col<0 || col>=24 || begin<0 ||
               idx>=static_cast<std::int64_t>(grid.size())) return -1;
            while(idx>begin && grid[static_cast<std::size_t>(idx)]>9) --idx;
            const auto type=grid[static_cast<std::size_t>(idx)];
            if(type<0 || type>=static_cast<std::int32_t>(type_lengths.size()))
                return -1;
            const auto length=type_lengths[static_cast<std::size_t>(type)];
            if(length<1 || idx+length>static_cast<std::int64_t>(grid.size()))
                return -1;
            for(std::int32_t i=0;i<length;++i)
                grid[static_cast<std::size_t>(idx+i)]=-1;
            return type;
        }
    return -1;
}

// Original 0x00414A30
[[nodiscard]] Vec2f vector_from_degrees_magnitude(
    std::int32_t angle,std::int32_t magnitude,float radians_per_degree) noexcept {
    const long double radians=static_cast<long double>(angle)*
        static_cast<long double>(radians_per_degree);
    return {static_cast<float>(std::cos(radians) * magnitude),
            static_cast<float>(std::sin(radians) * magnitude)};
}

// Original 0x0041AE90
[[nodiscard]] long double float_abs_with_retail_threshold(
    float value,float threshold) noexcept {
    return value<threshold ? -static_cast<long double>(value)
                           : static_cast<long double>(value);
}

}
