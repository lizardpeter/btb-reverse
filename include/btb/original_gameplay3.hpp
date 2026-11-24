#pragma once
// Original PE32 gameplay entry-point source reconstructions.
// Ghidra evidence: 00413450 / 00423630 / 004273F0, executable SHA-256
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05.
// The animation, movie, audio and movement backends are required interfaces,
// not stubs replacing original asset decoding / DirectDraw / DirectSound.
#include "btb/fireworks_sequence.hpp"
#include <array>
#include <cstdint>
#include <utility>
#include <vector>
#include <stdexcept>
namespace btb::original_gameplay3 {
using I32=std::int32_t;
using U32=std::uint32_t;
struct XY {I32 x{},y{};};
struct SquirrelState {
 I32 phase{},selected{-1},carrying_value{-1},placement_round{};
 I32 selectable_count{},pieces_placed{},placed_capacity{4},points{};
 I32 target_index{-1},visible_level{},feedback_index{-1},voice_state{},voice_delay{};
 I32 placement_finished{},feedback_finished{},intro_active{},celebration{};
 I32 actor_animation{},sound_group{};
 XY home{},placement_target{};
 std::array<XY,32> selection_positions{},round_positions{};
 std::array<I32,32> selection_enabled{},selection_values{};
 std::array<I32,32> round_counts{};
 std::array<I32,128> written_values{},expected_values{};
};
class SquirrelIO {
public:
 virtual ~SquirrelIO()=default;
 virtual bool step_actor_toward(I32 x,I32 y)=0;
 virtual U32 random32()=0;
 virtual void stop_all_sounds()=0;
 virtual void sound(I32 id,I32 priority,I32 kind)=0;
};
// 004273F0: authored mouse hit targets and 0..8 squirrel placement states.
void update_squirrel_placement(SquirrelState&,I32 mouse_x,I32 mouse_y,
                               bool click,SquirrelIO&);

struct PilchardState {
 I32 phase{},screen{},previous_screen{-1},difficulty{},wait_frames{},
     screen_return_frames{},current_node{},destination_node{-1},
     edge_index{},path_cursor{},wander_direction{},traversal_speed{},
     reset_countdown{},collision_flag{},last_distance{999999};
 bool animation_enabled{};
 XY position{},previous_position{};
 std::vector<I32> shortest_path{},neighbors{};
};
class PilchardIO {
public:
 virtual ~PilchardIO()=default;
 virtual U32 random32()=0;
 virtual std::vector<I32> shortest_path(I32 start,I32 target)=0;
 virtual std::vector<I32> neighbors(I32 screen,I32 node)=0;
 virtual XY node_position(I32 screen,I32 node)=0;
 virtual bool advance_toward_node(I32 node,I32 speed,XY& pos)=0;
 virtual I32 distance_to_node(I32 node,const XY& position)=0;
 virtual bool collision_and_hammer_drop()=0;
 virtual void sound(I32 id,I32 priority,I32 kind)=0;
 virtual I32 pursuit_speed(I32 difficulty)=0;
 virtual I32 wander_speed(I32 difficulty)=0;
};
// 00423630: original 0..7 Spud Maze Pilchard pursuit and wander state loop.
void update_spud_maze_pilchard(PilchardState&,PilchardIO&);

struct FireworksState {
 fireworks::RetailShowRuntime runtime{};
 fireworks::Sequence authored_grid{};
 I32 current_internal_state{9}; // retail ShowPlayback
 I32 frame_count{};
};
class FireworksIO {
public:
 virtual ~FireworksIO()=default;
 // Each value is obtained from the actual original movie/service.
 virtual I32 elapsed_centiseconds()=0;
 virtual bool decode_crowd_movie(I32 movie_index,I32 column)=0;
 virtual bool decode_topmiddle_movie(I32 column)=0;
 virtual bool decode_active_movie(std::size_t slot,I32 type,I32 row,I32 column)=0;
 virtual void restart_crowd_movie(I32 movie_index)=0;
 virtual void restart_topmiddle_movie()=0;
 virtual void restart_active_movie(std::size_t slot)=0;
 virtual bool any_managed_sound_playing()=0;
 virtual bool sound_id_playing(I32 id)=0;
 virtual U32 random32()=0;
 virtual void stop_all_managed_sounds()=0;
 virtual void play_managed_sound(I32 id,I32 priority,I32 kind)=0;
 virtual void draw_background()=0;
 virtual void draw_end_overlay()=0;
 virtual void reset_end_sound()=0;
 virtual void stop_end_sound()=0;
 virtual void launch_movie(std::size_t slot,I32 movie_index,I32 x,I32 y)=0;
};
// 00413450: old completed movies processed before authored grid launches.
// Native Bink decoder and SoundManager required. No synthetic finished flags.
void update_fireworks_show(FireworksState&,FireworksIO&);
}
