#pragma once
// PE32 source reconstructions of 004115C0 and 00411C80.
// Real Bink, DirectDraw, CD fallback and save I/O are injected dependencies.
// No original x86 ABI replacement or retail-behavior certification is claimed.
#include "btb/fireworks_sequence.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
namespace btb::original_fireworks_start2 {
using U32=std::uint32_t;
using I32=std::int32_t;
inline constexpr std::size_t kMovieCount=23;
inline constexpr std::size_t kMaxOriginalMovieRead=1'500'000;
struct MovieSlot {
 U32 movie_handle{},bitmap_handle{};
 std::vector<std::uint8_t> original_bytes;
 I32 width{},height{},depth{7},surface_structure_size{124};
};
struct MovieBank {
 std::array<MovieSlot,kMovieCount> slots{};
 U32 minimized_bitmap{},certificate_bitmap{};
 I32 crowd_phase{},movie_count{};
};
class MovieIO {
public:
 virtual ~MovieIO()=default;
 virtual U32 load_bitmap(U32 retail_slot,std::string_view path)=0;
 virtual void color_key(U32 retail_slot,U32 bitmap,U32 rgb)=0;
 virtual std::vector<std::uint8_t> open_game_movie_with_cd_fallback(std::string_view path,std::size_t max_bytes)=0;
 virtual U32 create_video_surface(U32 retail_slot,I32 width,I32 height,I32 depth,I32 structure_size)=0;
 virtual U32 open_bink_movie_from_memory(std::size_t index,
                                  const std::vector<std::uint8_t>& bytes)=0;
};
// Original 004115C0: two bitmaps plus 23 Bink-asset buffers, DD video
// surfaces and movie handles. Exact 23 paths from ghidra/firework_movies.csv.
void load_firework_movie_bank(MovieBank&,MovieIO&);
[[nodiscard]] std::string_view movie_path(std::size_t index);
struct Sprite {
 U32 original_slot{},handle{};bool keyed{};
};
struct EditorState {
 I32 selected_player{},activity_route{4},editor_state{},
    primary_cursor_x{260},primary_cursor_y{168},other_cursor_x{260},
    other_cursor_y{},frame_status{},mouse_active{1},sound_volume{};
 bool completion_marked{},mouse_capture_enabled{};
 std::array<I32,2> dragging{},hover{},anim_frame{},anim_counter{},launch_pending{},timed_launch_pending{};
 std::array<I32,120> selection_grid{};
 std::array<I32,18> saved_grid{};
 std::array<Sprite,27> sprites{};
 fireworks::Sequence authored{};
 fireworks::LayoutData layout{};
};
class EditorIO {
public:
 virtual ~EditorIO()=default;
 virtual void install_unload_callback()=0;
 virtual void mark_player_activity_started(std::size_t zero_based_player)=0;
 virtual void load_editor_resources()=0;
 // Missing save file is a valid new profile. An existing file must be
 // exactly 18 little-endian i32 entries; no synthetic content.
 virtual std::optional<std::vector<std::uint8_t>> player_save(std::size_t zero_based_player)=0;
 virtual std::string placement_layout_data()=0;
 virtual std::string palette_layout_data()=0;
 virtual std::string_view palette_bitmap_name(std::size_t index)=0;
 virtual std::string_view overlay_bitmap_name(std::size_t index)=0;
 virtual U32 load_and_register_bitmap(U32 original_slot,std::string_view file)=0;
 virtual void set_color_key(U32 original_slot,U32 bitmap,U32 rgb)=0;
 virtual void set_global_volume(I32 millibel)=0;
 virtual void activate_input()=0;
};
// Original 00411C80; preserves profile-grid, 18+12 rectangles, surface
// registration and cursor/activity globals, using recovered parser contracts.
void initialize_fireworks_activity(EditorState&,EditorIO&);
}
