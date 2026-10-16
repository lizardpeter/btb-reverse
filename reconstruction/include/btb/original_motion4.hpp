#pragma once
// Source-level C++26 originals: 0041B140, 00421360, 0041CCB0, 00416A30.
// Original PE SHA-256: c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Calls to geometry, physics, sound and movement providers are required.
// Compiled-source reconstruction is NOT an original PE32 ABI equivalence claim.
#include "btb/maze_data.hpp"
#include "btb/maze_motion.hpp"
#include "btb/maze_player_navigation.hpp"
#include "btb/maze_spud_controller.hpp"
#include "btb/herding_runtime.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
namespace btb::original_motion4 {
using I32=std::int32_t;using U32=std::uint32_t;
struct Input {I32 dx{},dy{};bool mouse_pressed{},action_pressed{};};
struct Repair {I32 x{},y{},height{40},counter{1};};
struct Player {
 float x{},y{},previous_x{},previous_y{};
 I32 screen{},node{},last_captured_node{-1},last_direction{-1},
     facing{},movement_mask{15},movement_latch{},transition{},animation{},
     input_ticks{},repair_hit_count{},repair_sound{-1},repair_delay{};
 bool frozen{},mouse_mode{},can_move_x{true},occupied{},hazard_lock{},repair_tool_engaged{};
 float speed{1.0f},node_radius{30.0f},capture_radius{4.0f};
 std::array<maze::ScreenGraph,5> graphs{};
 std::array<std::vector<Repair>,5> repairs{};
};
struct Npc {
 maze::SpudNpcPhase phase{maze::SpudNpcPhase::SpawnWait};
 I32 screen{},player_screen{},difficulty{},current_node{},
     current_direction{},path_index{},selected_package{-1},
     spawn_wait{},spawn_interval{60},offscreen_wait{},animation_countdown{},
     animation_frame{},nearest_node_distance{999999},carry_active{},
     collision_latch{},score{},timer_cooldown{};
 maze::ActorPosition position{};
 std::array<maze::ScreenGraph,3> graphs{};
 std::array<std::array<maze::SpudPackageSlot,4>,3> packages{};
 // Source post-END 12-reference table maps package location index 0..3
 // to graph node IDs, which are NOT the same indices.
 std::array<std::array<I32,4>,3> box_target_nodes{};
 bool use_box_target_nodes{};
 I32 player_node{-1};
 std::array<I32,4> player_links{{-1,-1,-1,-1}};
 std::vector<I32> path{};
};
struct HerdAnimal {
 herding::RetailEntityRecord32 source{};
 I32 countdown{},undelivered{},roam_countdown{},follower_index{-1},
     difficulty{},home_counter{},last_sound{-1},direction_countdown{};
 bool collision_latch{},target_confirmed{},source_collision_confirmed{};
 float max_speed{2.0f},acceleration{0.15f},arrival_radius{10.0f};
};
class IO {
public:
 virtual ~IO()=default;
 virtual Input player_input(bool spud_maze)=0;
 virtual U32 random32()=0;
 virtual void sound(I32 id,I32 priority,I32 sound_class)=0;
 virtual void spud_spawn_delay_reset()=0;
 virtual float npc_distance_to_player(const Npc& actor)=0;
 virtual void spud_repair_completed(I32 screen,I32 index)=0;
 virtual float actor_distance(float x,float y,float tx,float ty)=0;
 virtual I32 integer_angle(I32 ax,I32 ay,I32 bx,I32 by)=0;
 virtual std::array<float,2> movement_delta(I32 degrees,float speed)=0;
 virtual std::vector<I32> shortest_path(const maze::ScreenGraph& graph,I32 start,I32 target)=0;
 virtual void animal_collision_recovery(herding::RetailEntityRecord32& animal)=0;
 virtual bool original_herding_boundary_collision(const herding::RetailEntityRecord32& animal)=0;
};
void update_maze_player(Player&,IO&);
void update_spud_maze_bob(Player&,IO&);
void update_maze_spud_npc(Npc&,IO&);
I32 update_herding_animal(HerdAnimal&,IO&);
}
