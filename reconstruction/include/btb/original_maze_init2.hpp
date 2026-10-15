#pragma once
// Source-level recoveries of original PE32 0041A730/004207E0.
// Depends on the already reconstructed original Maze/Spud Maze data parsers.
// Native DirectDraw, clock, sound and file services are REQUIRED callbacks.
#include "btb/maze_data.hpp"
#include "btb/spud_maze_data.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace btb::original_maze_init2 {
using I32=std::int32_t;using U32=std::uint32_t;
struct ActorSlot {I32 node{-1};I32 enabled{1};};
struct Surface {U32 address{},handle{};bool keyed{};U32 key{};};
struct InitState {
 I32 difficulty{},timer{300},mode{},actor_stride{4},tracked_actors{},
    player_count{},target_count{},startup_flag{},music_volume{100},
    active_sound{},initialized{};
 float player_x{},player_y{};
 std::array<ActorSlot,20> actors{};
 std::vector<Surface> surfaces;
 U32 start_clock_100ns_low{},start_clock_100ns_high{};
 std::uint64_t start_clock_100ns{},last_clock_100ns{};
 maze::Data maze_data{};
 spud_maze::Data spud_data{};
 std::array<bool,spud_maze::kRepairPointCount> damaged{};
 I32 repairs_remaining{},repair_seed{};
};
class IO {
public:
 virtual ~IO()=default;
 virtual void install_unload(bool spud_maze)=0;
 virtual maze::Data read_original_maze_data()=0;
 virtual spud_maze::Data read_original_spud_maze_data()=0;
 virtual void reset_sound_manager()=0;
 virtual void create_maze_engine_sound(std::string_view path)=0;
 virtual void set_volume(I32 native_volume)=0;
 virtual U32 load_registered_surface(U32 slot,std::string_view file)=0;
 virtual void color_key_surface(U32 slot,U32 handle,U32 rgb)=0;
 virtual std::string_view maze_background_path(std::size_t i)=0;
 virtual std::string_view spud_background_path(std::size_t i)=0;
 virtual std::string_view repair_sprite_path(std::size_t screen,std::size_t ordinal)=0;
 virtual U32 next_original_rand()=0;
 virtual std::uint64_t system_filetime_100ns()=0;
};
// 0041A730: Maze data, sprite surfaces, sound manager and difficulty-
// dependent groups of distinct initial actor nodes.
void initialize_maze(InitState&,IO&);
// 004207E0: five-screen Spud Maze, 32 repair sprites, original divisor
// 8/5/3 seeded damage, actor start nodes, clock, sprites and callbacks.
void initialize_spud_maze(InitState&,IO&);
}
