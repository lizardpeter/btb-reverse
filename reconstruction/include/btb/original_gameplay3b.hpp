#pragma once
// Recovery targets 00415080 / 00425F40 / 004182B0 (original 2002 PE32).
// Ghidra evidence is source pseudocode, NOT retail behavioral equivalence.
// External movement, sound, collision and DirectDraw are required native services.
#include <array>
#include <cstdint>
#include <utility>
#include <vector>
namespace btb::original_gameplay3b {
using I32=std::int32_t; using U32=std::uint32_t;
struct Vec2 {float x{},y{};};
struct Point {I32 x{},y{};};
struct Box {I32 l{},t{},r{},b{};};
struct GolfHole {I32 x{},y{},offset_x{},offset_y{};};
struct Golf {
 I32 phase{},shot_angle{},angle_position{},power{0},power_step{1},remaining_shots{3},
     shot_count{},delay_count{},ball_anim{},ball_anim_max{8},bump_delay{},
     velocity{},bounce{1},bounce_power{},remaining_energy{},bounce_amount{},
     frame_counter{},frame_rate{},hits{},hole{-1},voice_quality{},
     target_band{},feedback_ticks{},score{},last_voice{-1},difficulty{},
     cursor_x{},input_mask{},start_x{},start_y{},mode{},landed_phase{};
 I32 radius_a{20},radius_b{52};float close_threshold{22},goal_threshold{15};
 float ball_x{},ball_y{},gravity_scale{1.0f};
 bool mouse_confirm{},mouse_down{},environment_enabled{};
 std::array<I32,3> band_angle{0,60,34};
 std::array<GolfHole,3> holes{};
 std::array<Point,16> landing_marks{};
};
class GolfIO {
public:
 virtual ~GolfIO()=default;
 virtual U32 random32()=0;
 virtual void play_sound(I32 id,I32 priority,I32 kind)=0;
 virtual Vec2 original_vector_from_degrees(I32 degrees,I32 magnitude)=0;
 virtual I32 original_launch_angle(const Golf& game)=0; // __ftol source dependency
 virtual I32 original_energy_loss(const Golf& game)=0; // retail trajectory friction
 virtual float original_distance(Point a,Point b)=0;
};
I32 update_golf_gameplay(Golf&,GolfIO&);

struct SquirrelRun {
 I32 phase{},profile{},sequence{},frame{},countdown{1},cadence{1},direction{},x{},y{},
     offset_x{},offset_y{},sprite_width{64},sprite_height{64},scroll{},scroll_basis{},
     clip_frame{},sheet_width{},sheet_index{},overlay_state{},visible_level{},
     completed_cycles{},motion_variant{},current_sound{},round{},branch{},initial_floor{},
     min_run{},max_run{10},min_elevated{},max_elevated{10},reset_run{},
     stage_select{},profile_mark{},last_motion_tick{-1};
 bool finish_flag{},show_overlay{},special_animation{},hide_actor{},use_frame_table{};
 U32 sprite_surface{};
 std::array<I32,4> source_profile{0,1,2,3};
 std::array<I32,32> frame_start{};
 std::array<I32,32> step_x{},step_y{},fall_x{},fall_y{};
 std::array<I32,8> direction_mask{};
};
class SquirrelRunIO {
public:
 virtual ~SquirrelRunIO()=default;
 virtual U32 random32()=0;
 virtual void select_original_motion_profile(SquirrelRun&)=0;
 virtual void no_op_legacy_hook()=0;
 virtual I32 original_frame_offset(I32 profile,I32 frame,bool descending)=0;
 virtual I32 original_sequence_base(I32 profile)=0;
 virtual bool original_visibility_gate(I32 round,I32 branch,bool even)=0;
 virtual void original_blt(I32 dest_x,I32 dest_y,U32 surface,Box source,bool keyed)=0;
};
void update_and_draw_squirrel_run(SquirrelRun&,SquirrelRunIO&);

struct HerdingAnimal {
 I32 id{-1},type{},behavior{},direction{},frame{},animation_delay{},size_x{},size_y{},
     x{},y{},previous_x{},previous_y{},speed{},phase_counter{},
     previous_sprite_x{},previous_sprite_y{};
 float x_float{},y_float{},velocity_x{},velocity_y{},acceleration{};
 bool locked{},tracked{};
};
struct HerdingPen {Point center{};I32 kind{},status{};};
struct Herding {
 std::vector<HerdingAnimal> animals{};
 std::array<HerdingPen,3> pens{};
 std::array<I32,20> followers{};
 std::vector<Point> guide_path{};
 I32 current_x{},current_y{},last_x{},last_y{},facing{},frame{},anim_tick{50},
     direction_mask{},selected_pen{-1},last_cursor_x{},last_cursor_y{},
     entity_count{},remaining{},finish_state{},game_state{},activity_state{},
     saved_progress{},active_profile{},active_path{},stage_index{},
     last_sound{-1},saved_frame{},control_mode{},cursor_x{},cursor_y{},
     input_mask{},collision_count{},frames_rendered{};
 float position_x{},position_y{},prev_position_x{},prev_position_y{},
       pointer_offset_x{},pointer_offset_y{},walk_speed{1.0f},
       max_walk_speed{5.0f},velocity_x{},velocity_y{},
       pointer_tolerance{20.f},acceleration{1.f};
 bool quit_requested{},menu_requested{},pointer_controls{},
      motion_enabled{},music_pending{true},all_animals_home{};
};
class HerdingIO {
public:
 virtual ~HerdingIO()=default;
 virtual void enable_input_processing()=0;
 virtual U32 random32()=0;
 virtual float distance(Point a,Point b)=0;
 virtual I32 angle_degrees(Point a,Point b)=0;
 virtual bool polygon_excludes(Point a)=0; // source PointInPolygon nonzero outside
 virtual bool rectangles_intersect(Box a,Box b)=0;
 virtual bool original_pen_or_animal_collision(const HerdingAnimal&,I32 index)=0;
 virtual void update_original_animal(HerdingAnimal&,I32 index)=0;
 virtual void draw_herding_activity()=0;
 virtual bool any_managed_sound_playing()=0;
 virtual void play_sound(I32 sound_id,I32 priority,I32 group)=0;
 virtual void unload_herding_activity()=0;
 virtual void prepare_play_again()=0;
 virtual void stop_activity_music()=0;
 virtual void play_activity_music(I32 music)=0;
 virtual void mark_intro_sound(I32 id)=0;
};
I32 update_herding_activity(Herding&,HerdingIO&);
} // namespace btb::original_gameplay3b
