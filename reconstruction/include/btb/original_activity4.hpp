#pragma once
// PE32 original source models at 004227F0, 00424690, 004267B0, 00401700.
// Explicit backend contracts keep DirectDraw, Bink, host input/sound and
// original guest data dependencies visible. Not binary ABI replacements.
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
namespace btb::original_activity4 {
using I32=std::int32_t; using U32=std::uint32_t;
struct Rect {I32 l{},t{},r{},b{};};
struct Sprite {U32 surface{}; I32 x{},y{};bool keyed{};std::optional<Rect> source{};};
class IO {
public:
 virtual ~IO()=default;
 virtual void draw(const Sprite& sprite)=0;
 virtual void sound(I32 id,I32 priority,I32 klass)=0;
 virtual void stop_sounds()=0;
 virtual bool sound_active()=0;
 virtual bool sound_id_active(I32 id)=0;
 virtual U32 random32()=0;
 // Bink calls are original runtime/asset-dependent, not synthetic decoders.
 virtual bool bink_end(std::size_t index)=0;
 virtual void bink_decode(std::size_t index)=0;
 virtual void bink_next(std::size_t index)=0;
 virtual void bink_wait(std::size_t index)=0;
 virtual void bink_seek(std::size_t index,I32 frame)=0;
 virtual I32 bink_position(std::size_t index)=0;
 virtual void update_pilchard_animation()=0;
 virtual void update_bob_animation()=0;
 virtual void maybe_spud_voice()=0;
 virtual std::uint64_t filetime_100ns()=0;
 virtual I32 spud_maze_timer_digits(I32 remaining_seconds)=0;
 virtual void squirrel_run_assembly()=0;
 virtual void voice_help(I32 sound,I32 kind)=0;
 virtual void leave_help_mode()=0;
};
struct RepairSprite {I32 x{},y{},counter{};U32 surface{};};
struct SpudMazeScene {
 bool background_mode{},pilchard_present{},allow_bob_animation{true},
      bob_locked{},raise_hammer{},pilchard_talking{};
 I32 screen{},spud_screen{},bob_screen{},screen_offset{},old_screen_offset{},
     bob_facing{},bob_frame{},bob_x{},bob_y{},
     pilchard_facing{},pilchard_frame{},pilchard_x{},pilchard_y{},
     hammer_x{},hammer_y{},hammer_velocity{},hammer_present{},
     clock_seconds{300},time_bias_seconds{},last_seconds{},
     remaining_hammers{},used_hammers{},blink_frame{},retail_pose{};
 std::uint64_t baseline_filetime{};
 std::array<U32,5> backgrounds{};
 U32 pilchard{0x005144fcU},bob{0x005144f8U},hammer{0x00514494U},
     border{0x005144c4U},digits{0x005144d0U},
     hammer_on{0x00514508U},hammer_off{0x00514504U},
     helper{0x005144b8U};
 std::array<std::vector<RepairSprite>,5> repairs{};
};
void draw_spud_maze_scene(SpudMazeScene&,IO&);

struct SkateSegment {I32 first{},last{};std::array<I32,4> thresholds{};};
struct SpudSkateScene {
 U32 background{},digits{},end_surface{},bonus_surface{};
 std::array<U32,4> frame_surfaces{};
 std::array<U32,4> link_arrows{};
 I32 score{},phase{},last_sound_gate{},active_channel{},score_step{},
     frame_counter{},ending_counter{},ending_profile{},ending_profile_flag{},
     pause_index{-1},terminal_time{},transition_frame{},
     current_skate_time{},selected_sequence{-1},current_path{0};
 bool input_select{},button_press{},ending_bink_prepared{};
 std::array<SkateSegment,32> segments{};
 I32 segment_count{};
 std::array<std::array<I32,5>,16> event_sounds{};
 std::array<I32,5> terminal_sounds{};
 I32 rewind_frame{1};
};
void draw_spud_skate_playback(SpudSkateScene&,IO&);

struct SquirrelSprite {I32 x{},y{},frame{-1};};
struct SquirrelScene {
 bool replay{},player_selected{},background_animation{},sound_owned{};
 I32 screen{},scroll{},secondary_scroll{},sprite_layer{},animation_counter{},
     background_counter{},background_phase{},overlay_phase{},overlay_frame{21},
     progress{},speech_sound{-1},sound_delay{},last_audio_variant{-1},
     squirrel_actor_state{},moving_actor_index{-1},actor_x{},actor_y{};
 U32 backdrop{},foreground{},foreground_alt{},character{},overlay{},
     panel{},frame_surface{},squirrel_sheet{},mouse_pointer{};
 std::array<U32,4> borders{};
 std::array<I32,4> drawable_per_screen{};
 std::array<std::array<I32,4>,4> sprite_frame_ids{};
 std::array<std::array<I32,4>,4> second_frame_ids{};
 std::array<SquirrelSprite,4> positions{};
 std::array<SquirrelSprite,4> secondary_positions{};
 std::vector<SquirrelSprite> active_sprites{};
 std::vector<I32> active_frame_ids{};
 I32 panel_x{},panel_y{};
};
void draw_squirrel_scene(SquirrelScene&,IO&);

struct HelpHotspot {Rect area{};I32 sound{},id{};bool enabled{true};};
struct ContextHelp {
 I32 activity{},previous{-1},mouse_x{},mouse_y{},progress{},mode_index{},
     camera_x{},camera_y{},dino_entities{},dino_alternate{};
 bool click{},hover_active{};
 std::array<std::vector<HelpHotspot>,32> generic_hotspots{};
 std::vector<HelpHotspot> special_hotspots{};
 std::array<Rect,2> maze_help{};
 std::array<Rect,4> herding_hotspots{};
 std::array<Rect,10> dino_hotspots{};
};
void update_contextual_help(ContextHelp&,IO&);
}
