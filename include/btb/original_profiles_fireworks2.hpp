#pragma once
// Source reconstructions of BTB PE32 originals 0042D270 and 00412E50.
// Original native asset, DirectDraw, sound, and file interfaces are required
// provider boundaries; not an executable-level x86 ABI replacement.
#include "btb/player_profiles.hpp"
#include "btb/player_progress.hpp"
#include "btb/fireworks_sequence.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
namespace btb::original_profiles_fireworks2 {
using I32=std::int32_t;using U32=std::uint32_t;
struct Surface {U32 address{},handle{};bool keyed{};};
inline constexpr std::size_t kFontGlyphs=255;
using FontTable=std::array<std::array<I32,4>,kFontGlyphs>;
struct ProfilesState {
 std::vector<Surface> surfaces;
 std::array<FontTable,3> fonts{};
 std::array<I32,141> ascii{};
 std::size_t ascii_count{};
 profiles::ProfileTable profiles{};
 std::array<progress::Record,profiles::kProfileCount> progress{};
 I32 selected{-1},control_state{},delete_mode{},initialized{};
};
class ProfilesIO {
public:
 virtual ~ProfilesIO()=default;
 virtual U32 load_registered_surface(U32 original_address,std::string_view filename)=0;
 virtual void color_key(U32 address,U32 handle,U32 rgb)=0;
 // The source file is loaded through the original CD fallback if needed.
 virtual std::optional<std::string> read_game_data(std::string_view filename)=0;
 // Player writable saves use normal local fopen, not the CD fallback.
 virtual std::optional<std::string> read_local_save(std::string_view filename)=0;
 virtual std::string_view button_path(std::size_t index)=0;
};
void initialize_player_profiles(ProfilesState&,ProfilesIO&);

struct SpriteFrame {I32 x{},y{},width{},height{};};
struct FlyingActor {
 I32 x{200},y{140},animation{13},frame{},counter{},
     phase{},pending_type{-1},target_x{},target_y{},direction{4};
};
struct FireworksEditorState {
 fireworks::RetailGrid cells{};
 std::array<fireworks::Recti,18> layout{};
 std::array<FlyingActor,2> actors{};
 I32 pointer_x{320},pointer_y{160},proximity_count{},animation_counter{};
 bool draw_foreground{true};
 I32 played_count{};
};
class FireworksEditorIO {
public:
 virtual ~FireworksEditorIO()=default;
 virtual void draw_backdrop(U32 source_guest_slot)=0;
 virtual void blit_keyed(U32 guest_slot,I32 x,I32 y,
                         std::optional<SpriteFrame> clip)=0;
 virtual void draw_overlay(U32 guest_slot,I32 x,I32 y)=0;
 virtual void place_firework(I32 x,I32 y,I32 type,bool from_actor)=0;
 virtual void play_sound(I32 id,I32 priority,I32 arbitration_class)=0;
 virtual I32 angle_degrees(I32 ax,I32 ay,I32 bx,I32 by)=0;
 virtual I32 step_sine(I32 degrees)=0;
 virtual I32 step_cosine(I32 degrees)=0;
 virtual I32 distance(I32 ax,I32 ay,I32 bx,I32 by)=0;
};
void draw_fireworks_editor(FireworksEditorState&,FireworksEditorIO&);
}
