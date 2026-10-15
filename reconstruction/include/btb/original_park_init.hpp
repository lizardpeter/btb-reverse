#pragma once
// Recovered source model of original 0040B7E0 Park Designer initialization.
// This is NOT a retail x86 ABI replacement until native provider integration
// and original-executable differential testing have been completed.
#include "btb/park_designer_data.hpp"
#include <array>
#include <cstdint>
#include <vector>
namespace btb::original_park_init {
using I32=std::int32_t;
using U32=std::uint32_t;
struct Surface {
 U32 guest_slot{},source_string_va{},native_handle{};
 bool keyed{};
};
struct PaletteCell {
 I32 mode{},category{},variant{},left{},top{},right{},bottom{};
 I32 source_left{},source_top{},source_right{},source_bottom{};
 U32 surface{};
 I32 enabled{-1},source_index{-1};
};
struct State {
 park_designer::SaveData save{};
 park_designer::BoundAreas bounds{};
 std::vector<Surface> surfaces{};
 std::array<PaletteCell,park_designer::kBoundPolygonCount> palette{};
 I32 game_mode{4},selected_pond{-1},selected_object{-1},
     selected_bandstand{-1},cursor{-1},scene_music_flag{},
     stage{},ending_stage{},completion_phase{},current_season{},spawn_counter{};
 bool initialized{},register_unload{},sound_stopped{},source_data_loaded{};
};
class IO {
public:
 virtual ~IO()=default;
 virtual void register_original_unload()=0;
 virtual U32 load_original_bitmap(U32 guest_slot,U32 source_string_va)=0;
 virtual U32 load_indexed_original_bitmap(U32 guest_slot,U32 source_string_va,U32 byte_offset)=0;
 // Original dynamically constructed filenames are assembled on stack;
 // the caller must supply the actual retail path, not a guessed substitute.
 virtual U32 load_original_toolbar_bitmap(U32 guest_slot,I32 group,I32 ordinal)=0;
 virtual void set_magenta_key(U32 guest_slot,U32 handle,U32 rgb)=0;
 virtual void load_original_park_data(park_designer::SaveData&,
                                      park_designer::BoundAreas&)=0;
 virtual std::array<I32,4> original_palette_source_rect(
    I32 mode,I32 category,I32 variant,U32 sprite)=0;
 virtual void stop_all_managed_sounds()=0;
};
// 0040B7E0: registered original surfaces, 60 (mode,category,variant)
// editor palettes, retail data restore, 400-record initialization, and
// completion/music state. Runtime editor tick remains in 0040E440.
void initialize_park_designer(State&,IO&);
} // namespace btb::original_park_init
