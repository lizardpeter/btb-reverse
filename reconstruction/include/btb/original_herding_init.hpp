#pragma once
// 00419600 InitializeHerdingActivity — reconstruction of the original
// executable's activity initialization, not retail x86 ABI equivalence.
// Uses the shipped parsed herd.txt coordinate groups and original PE
// resource-string VAs. DirectDraw and volume backends are REQUIRED services.
#include "btb/herding_data.hpp"
#include <array>
#include <cstdint>
#include <vector>
namespace btb::original_herding_init {
using I32=std::int32_t; using U32=std::uint32_t;
struct Bitmap {
 U32 guest_slot{},source_string_va{},handle{};
 bool color_keyed{};
};
struct Entity {
 I32 id{},x{},y{},facing{},type{},behavior{17},
     frame_counter{},width{},height{},timer{},render_flags{};
 float precise_x{},precise_y{},heading_radians{};
 U32 bitmap{};
};
struct State {
 herding::Data data{};
 I32 difficulty{},player_mode{},volume{100},leader_count{},
     herd_remaining{},selected_pen{-1},return_state{},waypoint_index{},
     guide_count{},collision_index{},source_entity_stride{0x19},
     music_start_pending{},startup_flag{},gate_left_index{-1},
     gate_right_index{-1},initial_anim_frame{};
 bool initialized{};
 std::array<I32,20> followers{};
 std::array<I32,20> retained_indices{};
 std::array<I32,20> active_target_indices{};
 std::array<I32,20> common_overlay{};
 std::array<I32,3> pen_status{{1,1,1}};
 std::vector<Bitmap> bitmaps{};
 std::vector<Entity> entities{};
 std::vector<herding::Vec2i> original_polygon{};
 std::vector<herding::Vec2i> guide_route{};
 std::vector<herding::Vec2i> roaming_path{};
 herding::Vec2i finish_marker{};
};
class IO {
public:
 virtual ~IO()=default;
 virtual herding::Data load_original_coordinate_data()=0;
 virtual void register_original_unload()=0;
 virtual void set_global_game_volume(I32 attenuation)=0;
 virtual U32 load_registered_bitmap(U32 guest_slot,U32 string_va)=0;
 virtual void key_bitmap(U32 guest_slot,U32 handle,U32 color)=0;
 virtual U32 original_random32()=0;
 virtual float original_angle_to_radians(I32 facing)=0;
};
// 00419600: startup arrays and volume, 18 DirectDraw images,
// difficulty-indexed 3 x (3 + profile) animal populations, fixed
// background entities, and herd.txt's source delimiter groups.
void initialize_herding(State&,IO&);
} // namespace btb::original_herding_init
