#pragma once
// Original function entry points 00409E00 and 00428600.
// Ghidra evidence: analysis/ghidra_main_exe/functions/{00409E00,00428600}.c
// Source PE SHA256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// The video, bitmap, audio and input services are REQUIRED native providers;
// these C++ source-level reconstructions do not establish retail x86 parity.
#include <array>
#include <cstdint>
#include <utility>
#include <vector>
namespace btb::original_coordinators2 {
using I32=std::int32_t; using U32=std::uint32_t;
struct Rect {I32 left{},top{},right{},bottom{};};
struct DinoPiece {
 I32 x{},y{},width{},height{},target_x{},target_y{};
 I32 state{},visible{},original_index{};
 U32 cursor_surface{};
};
struct DinoState {
 std::vector<DinoPiece> pieces{};
 I32 phase{},selected{-1},completed{},score_index{},level_variant{},
     radius{9},cursor_x{},cursor_y{},cursor_reset_countdown{},
     animation_state{4},feedback_state{},completion_state{},
     success_variant{},failure_variant{},next_game_state{},
     marked_profile{-1},profile_index{},last_selected{-1},stage_progress{};
 I32 board_x{},board_y{},end_x{},end_y{};
 std::array<I32,2> feedback_frames{},feedback_gates{};
 std::vector<I32> profile_completed{};
 bool quit_requested{},menu_requested{},music_pending{true},input_down{},
      actor_moving{},completion_sprite_loaded{};
};
class DinoIO {
public:
 virtual ~DinoIO()=default;
 virtual void enable_input_processing()=0;
 virtual bool any_managed_sound_playing()=0;
 virtual void stop_all_managed_sounds()=0;
 virtual void sound(I32 sound_id,I32 priority,I32 group)=0;
 virtual U32 random32()=0;
 virtual bool cursor_in_rectangle(Rect)=0;
 virtual bool cursor_near_point(I32 x,I32 y,I32 radius)=0;
 virtual void set_cursor_surface(U32 surface)=0;
 virtual void load_completion_sprite(I32 level_variant)=0;
 virtual void draw_dino_activity()=0;
 virtual void update_print_button()=0;
 virtual void unload_dino_activity()=0;
 virtual void prepare_play_again()=0;
 virtual void stop_activity_music()=0;
 virtual void play_activity_music(I32 index)=0;
 virtual void mark_activity_intro_sound(I32 stage)=0;
};
// 00409E00. Returns original nonzero keep-updating result.
I32 update_dino_activity(DinoState&,DinoIO&);

struct UiOption {
 std::vector<std::pair<I32,I32>> polygon{};
 I32 next_state{},voice{-1},hover_delay{8},hover_frame{},
     hover_frame_count{},passive_frame{},passive_frame_count{},
     passive_delay{8},passive_tick{},sound_cursor{},
     pressed_voice_count{},normal_x{},normal_y{},hover_x{},hover_y{};
 std::vector<I32> pressed_voices{};
 std::vector<U32> hover_surfaces{},passive_surfaces{};
 U32 static_surface{},pressed_surface{};
 I32 static_x{},static_y{},pressed_x{},pressed_y{};
};
struct UiState {
 I32 screen{},active_option{},selected_option{-1},pending_voice{},
     mode{},next_mode{},profile{},mouse_x{},mouse_y{},
     previous_hover{-1},passive_clock{5},unlocked_mode{},
     intro_complete{},render_enabled{},transition_screen{};
 bool mouse_down{},mouse_clicked{},progress_screen_loaded{},
      progress_button_enabled{true},sound_pending{},activate_progress{},
      requires_display_reset{};
 std::array<I32,100> awards{};
 std::array<std::vector<UiOption>,4> options{};
};
class UiIO {
public:
 virtual ~UiIO()=default;
 virtual void enable_input_processing()=0;
 virtual bool any_managed_sound_playing()=0;
 virtual void stop_all_managed_sounds()=0;
 virtual void play_sound(I32 id,I32 priority,I32 group)=0;
 virtual bool original_point_in_polygon(const std::vector<std::pair<I32,I32>>&,I32 x,I32 y)=0;
 virtual void blit(U32 surface,I32 x,I32 y,bool color_keyed)=0;
 virtual void invalidate_or_reset_primary_display()=0;
 virtual void no_op_legacy_hook()=0;
 virtual void load_progress_screen_assets()=0;
};
// 00428600. The original PointInPolygon helper uses an inverted hit
// convention: return false from original_point_in_polygon for a hit.
I32 update_generic_ui_screen(UiState&,UiIO&);
} // namespace btb::original_coordinators2
