#pragma once
// BTB executable c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Original-address draw routines: 00409990, 0041C110, 00414A60, 00416370.
// Source-level control-flow reconstructions; unresolved x87, DirectDraw,
// timing and untyped guest global operations stay behind required providers.
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>
namespace btb::original_draw4 {
using I32=std::int32_t;using U32=std::uint32_t;
struct Clip {I32 l{},t{},r{},b{};};
struct Draw {
 U32 surface{}; I32 x{},y{};
 std::optional<Clip> clip{};
 bool keyed{};
};
class IO {
public:
 virtual ~IO()=default;
 virtual void draw(const Draw&)=0;
 virtual void restore_surface(U32 guest_slot)=0;
 virtual void sound(I32 id,I32 priority,I32 sound_class)=0;
 virtual U32 random32()=0;
 // Source's x87 floating point projection uses original integer
 // truncation; provider receives the signed angle and source speed.
 virtual std::array<I32,2> polar(I32 degrees,I32 speed)=0;
 virtual I32 maze_clock_digits()=0;
};
struct DinoTrack {
 I32 phase{},animation{},requested{-1};
};
struct DinoEntity {
 I32 mode{},kind{},x{},y{},surface_slot{},picture_x{},picture_y{};
 Clip source{};
};
struct DinoState {
 U32 backdrop{0x004fc9e8U},character{0x004fc2acU},interface_surface{0x004fc434U};
 I32 frame_variant{},track_phase{},score{};
 std::array<DinoTrack,2> tracks{};
 std::vector<DinoEntity> entities{};
 I32 max_entities{2048};
};
void draw_dino(DinoState&,IO&);
struct MazeActor {
 I32 node{-1},state{},x{},y{};
};
struct MazeState {
 bool foreground_active{true},sprite_moving{},secondary_animation{};
 I32 screen{},view_offset{},view_x{},view_y{},other_screen{},other_enabled{},
     bob_x{},bob_y{},bob_previous_x{},bob_previous_y{},
     bob_pose{},bob_frame{},bob_timer{10},bob_animation_length{8},
     frame_interval{10},bob_sound_count{},timer_digits{},
     spud_pose{},spud_frame{},spud_timer{10},spud_animation_mode{},
     spud_x{},spud_y{},spud_previous_x{},spud_previous_y{},
     progress{},timer_left{},right_edge{};
 std::array<MazeActor,4> actors{};
 std::array<std::array<I32,2>,20> original_node_positions{};
 U32 box_surface{0x00512114U},player_surface{0x0051210cU},spud_surface{0x00512110U};
 std::array<U32,3> backdrops{0x00512100U,0x00512104U,0x00512108U};
};
void draw_maze(MazeState&,IO&);
struct GolfSprite {
 U32 slot{};
 I32 x{},y{},frame{},frame_width{},frame_height{},counter{5},length{8},
     source_y{},phase{},mode{};
};
struct GolfState {
 U32 backdrop{0x0050aedcU},digits{0x0050adecU},marker{0x0050aee8U},
     ball{0x0050aef8U},trail{0x0050aef0U},
     trail_secondary{0x0050aee4U},club{0x0050aee0U},
     celebration{0x0050aef4U};
 I32 score{},markers{},swing_angle{},swing_state{},selected_actor{-1},
     club_x{},club_y{},ball_x{},ball_y{},trail_elapsed{},
     status_level{},animation_tick{},wind_index{},wind_speed{},
     meter_value{},background_phase{};
 float trail_gate{},trail_threshold{1.0f};
 std::array<GolfSprite,2> actors{};
 std::array<GolfSprite,3> extras{};
};
void draw_golf(GolfState&,IO&);
struct HerdEntity {
 I32 id{-1},type{},frame{},counter{100},mode{},direction{},
     x{},y{},depth{},pause{},source_x{},source_y{},
     width{},height{};
 U32 surface{};
 bool animation_event{};
};
struct HerdState {
 I32 camera_x{},camera_y{},clip_x{},clip_y{},view_x{},view_y{},
     skin_group{},skin_frame_width{39},skin_frame_height{39},
     overlay_index{-1};
 U32 landscape{0x00510724U},foreground{0x00510748U};
 std::vector<HerdEntity> entities{};
 struct Prop {bool enabled{};U32 surface{};I32 x{},y{};};
 std::array<Prop,3> props{};
};
void draw_herding(HerdState&,IO&);
}
