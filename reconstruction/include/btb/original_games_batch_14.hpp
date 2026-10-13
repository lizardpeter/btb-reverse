#pragma once
// Fourteen independently compiled source-derived game-system procedures.
// Evidence: analysis/ghidra_main_exe/functions/<ADDRESS>.c, original PE32.
// Real native code, not just inlined documentation models. Retail globals
// and platform effects remain typed host operands/callbacks, not rebased x86
// module pointers or verified retail differential behavior.
#include "btb/display_manager.hpp"
#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
namespace btb::original_games14 {
using I32=std::int32_t;using U32=std::uint32_t;
using Rect=btb::display::RetailRect32;
using Callback=void(*)(I32,void*);
// 00402640 original Win32 hook input gate.
[[nodiscard]] bool suppress_fullscreen_key(
    I32 hook_code,U32 message,I32 key,bool alt,bool ctrl) noexcept;
// 00407920 all eleven original dependency calls in source order.
[[nodiscard]] I32 load_global_data_tables(Callback,void*);
// 00409CD0 Dinosaur print button.
enum class PrintEvent {Base,Highlight,HoverVoice,StopAll,PrintImage,Print};
using PrintSink=void(*)(PrintEvent,bool,void*);
struct DinoPrintState {I32 x{},y{};bool click{},alternate{},hover_latched{};};
void update_dino_print(DinoPrintState&,PrintSink,void*);
// 0040D870 source rectangle, no GPU fake provider.
[[nodiscard]] Rect pond_source_rect(
    I32 segment,I32 leading,I32 stride,I32 height) noexcept;
// 0040E3E0
struct ParkDrag {
    I32 mode{},selected{};
    std::span<I32> visibility{};
    std::span<const I32> valid_object_flags{};
};
void finish_park_drag(ParkDrag&,void(*)(void*),void*);
// 0041F4B0. Sound callback is original (type-row*10) lookup.
[[nodiscard]] bool place_grand_opening(
    std::span<I32>,std::span<const I32> lengths,std::span<I32> pair_modes,
    I32 row,I32 column,I32 type,Callback sound,void*);
// 00422760 actual MSVC rand 15-bit supplied by originating service.
using Random=U32(*)(void*);
using SoundActive=bool(*)(I32,void*);
using PlayVoice=I32(*)(I32,U32,I32,void*);
[[nodiscard]] I32 maybe_play_spud_voice(
    I32 cooldown,I32 last,Random,SoundActive,PlayVoice,void*);
// 00427270
[[nodiscard]] bool move_squirrel_towards(
    I32& x,I32& y,I32 target_x,I32 target_y) noexcept;
// 00428120 original hover voice and Bink restart path.
using MovieOp=void(*)(void*);
void update_walkthrough(
    I32 x,I32 y,bool any_managed_sound,I32 help_id,PlayVoice,
    MovieOp decode,U32 frame,U32 total_frames,MovieOp restart,void*);
// 00428E30 real DirectDraw Blt invoked by host callback, HRESULT retry.
using UiBlit=U32(*)(I32,bool,void*);
[[nodiscard]] I32 draw_generic_ui(
    bool play_again,UiBlit,void*,std::size_t max_attempts=0);
// 00407770 max ten original video-sequence entries.
using ReadI32=bool(*)(I32*,void*);
[[nodiscard]] std::size_t load_video_sequence(
    std::span<I32>,ReadI32,void*);
// 00415CB0 sentinel terminated Herding XY data.
using ReadPair=bool(*)(I32&,I32&,void*);
[[nodiscard]] std::size_t load_herding_coordinates(
    std::span<std::array<I32,2>>,ReadPair,void*);
// 00407390 / 00407430 one read each; host handles original fopen/fclose.
using ReadRecord=bool(*)(std::span<I32>,void*);
[[nodiscard]] bool load_options(std::span<I32>,ReadRecord,void*);
[[nodiscard]] bool load_play_again(std::span<I32>,ReadRecord,void*);
[[nodiscard]] bool verify_14_original_game_systems() noexcept;
}
