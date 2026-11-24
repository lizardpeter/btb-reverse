#pragma once
// Nine original PE32 loading cursor, Bink, Fireworks, Grand Opening and
// math functions, compiled out of line. x87 precision requires retail
// differential tests; native input/COM/video callbacks are injected.
#include "btb/retail_leaf_runtime.hpp"
#include "btb/retail_runtime_closure.hpp"
namespace btb::original_gameplay9 {
using namespace btb::retail_leaf;
using namespace btb::retail_closure;
// 0x004079E0
void update_loading_cursor_animation(
    LoadingCursorState& s, void (*poll_direct_input)());
// 0x00407A40
void draw_loading_cursor_animation(
    void* destination, const LoadingCursorState& s,
    LoadingCursorBlit blit);
// 0x00408D80
void restart_bink_movie(void* movie,BinkGoto goto_frame);
// 0x00408EA0
void close_bink_movie(void* movie,BinkClose close);
// 0x00411010
[[nodiscard]] bool place_firework(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t row,std::int32_t col,std::int32_t type,
    bool commit) noexcept;
// 0x00412B80
[[nodiscard]] bool delete_selected_firework(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t action,std::int32_t selected_cell) noexcept;
// 0x0041F430
[[nodiscard]] std::int32_t remove_grand_opening_event(
    std::span<std::int32_t> grid,
    std::span<const std::int32_t> type_lengths,
    std::int32_t row,std::int32_t col) noexcept;
// 0x00414A30
[[nodiscard]] Vec2f vector_from_degrees_magnitude(
    std::int32_t angle,std::int32_t magnitude,float radians_per_degree) noexcept;
// 0x0041AE90
[[nodiscard]] long double float_abs_with_retail_threshold(
    float value,float threshold) noexcept;
[[nodiscard]] bool verify_9_original_gameplay_math_functions() noexcept;
}
