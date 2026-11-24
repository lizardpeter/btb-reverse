#pragma once
// Last two original source-model entrypoints of the 615-function inventory:
// 0040E440 UpdateParkDesignerEditorInteraction
// 0041DB20 InitializeBobsBandActivity
// Native renderer, original file IO, hit testing and audio are REQUIRED.
#include "btb/park_designer_data.hpp"
#include "btb/grand_opening_data.hpp"
#include <array>
#include <cstdint>
#include <vector>
namespace btb::original_last2 {
using I32=std::int32_t;using U32=std::uint32_t;
struct Surface{U32 slot{},path_va{},handle{};bool keyed{};};
struct ParkEditor {
 park_designer::SaveData* save{};
 I32 mouse_x{},mouse_y{},click{},key_down{},selected_palette{-1},
     selected_object{-1},old_control{-1},last_hover{-1},
     active_cursor_slot{0x00507a00},scroll_offset{},season{},scene_phase{},
     mouse_latch{},voice_latch{},last_voice{-1},delete_requested{};
 bool previous_arrow{},next_arrow{},confirmation_requested{},mode_changed{};
};
class ParkEditorIO{
public:
 virtual ~ParkEditorIO()=default;
 virtual void draw_original_scene(const park_designer::SaveData&)=0;
 virtual void draw_original_toolbar(I32 mode,I32 page)=0;
 virtual I32 original_toolbar_hit_test(I32 x,I32 y)=0;
 virtual void set_original_cursor(U32 slot)=0;
 virtual void play_original_editor_voice(I32 id,I32 priority)=0;
 virtual void process_original_palette_and_placement(ParkEditor&)=0;
 virtual void process_original_delete(ParkEditor&)=0;
 virtual void process_original_view_controls(ParkEditor&)=0;
 virtual void draw_original_palette(I32 mode,I32 page,I32 offset)=0;
 virtual void draw_original_page_arrow(bool previous,bool next)=0;
};
void update_park_editor_0040e440(ParkEditor&,ParkEditorIO&);

struct BandGridCell {
 I32 machine_code{-1},x{},y{},frame{},kind{};
};
struct BandEditorSlot{
 I32 machine_id{},sprite_x{},sprite_y{},timer{100};
 U32 surface{};
};
struct BandInit {
 grand_opening::MachineData machine_data{};
 std::vector<Surface> resources{};
 std::array<I32,120> retail_sequence{};
 std::array<BandGridCell,120> grid{};
 std::array<BandEditorSlot,10> editor_slots{};
 std::array<I32,5> machine_states{};
 std::array<I32,6> conductor_states{};
 I32 conductor{},player_profile{},global_volume{100},loaded_sound_count{},
     active_music_id{},original_grid_total{},animation_tick{},
     runtime_state{},timeline_step{},completion_code{},
     active_game_music_flag{},wave_count{};
 bool initialized{},unload_registered{},sequence_loaded{};
};
class BandInitIO{
public:
 virtual ~BandInitIO()=default;
 virtual void initialize_original_crt()=0;
 virtual grand_opening::MachineData load_original_machine_data()=0;
 virtual void register_original_band_unload()=0;
 virtual U32 load_original_band_bitmap(U32 guest_slot,I32 group,I32 item,U32 retail_va)=0;
 virtual void key_original_bitmap(U32 guest_slot,U32 handle,U32 rgb)=0;
 virtual std::array<I32,120> load_original_music_sequence(I32 conductor,I32 player)=0;
 virtual I32 original_machine_grid_x()=0; // DAT_00444B04
 virtual I32 original_machine_grid_dx()=0; // DAT_00444B10
 virtual I32 original_machine_grid_y()=0; // DAT_00444B08
 virtual I32 original_machine_grid_dy()=0; // DAT_00444B0C
 virtual U32 original_machine_editor_sprite(I32 index)=0; // DAT_00444BE8
 virtual U32 original_machine_sequence_sprite(I32 index)=0; // DAT_00444C38
 virtual void register_original_active_machine(BandEditorSlot&,I32 index)=0;
 virtual void apply_original_global_volume(I32 attenuation)=0;
};
void initialize_bobs_band_0041db20(BandInit&,BandInitIO&);
} // namespace btb::original_last2
