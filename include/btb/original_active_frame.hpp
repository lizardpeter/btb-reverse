#pragma once
// PE32 00402440 original RunActiveGameFrame source-derived state flow.
// The original x86 disk, DirectDraw, DirectInput, help/audio and game-flow
// services are explicit callbacks. Native host binds actual game session,
// DirectDraw7 cooperative-level and present; CD authorization unavailable
// in source-only developer mode is reported via media_check_waived.
#include <cstdint>
namespace btb::original_active_frame {
using U32=std::uint32_t;
struct State {
    U32 media_counter{};
    U32 last_time_ms{};
    bool bitmap_reload_requested{};
    bool contextual_help_active{};
    U32 contextual_help_request{};
    bool media_check_waived{};
};
using Action=void(*)(void*);
using Predicate=bool(*)(void*);
using Device=U32(*)(void*);
using Sleep=void(*)(U32,void*);
struct Callbacks {
    void* context{};
    Predicate check_original_disc{};
    Predicate reload_original_bitmaps{};
    Action reset_contextual_help_after_reload{};
    Action reap_finished_sounds{};
    Predicate run_main_game_flow{};
    Action update_contextual_help{};
    Action enter_contextual_help{};
    Action poll_direct_input{};
    Device check_directdraw_cooperative{};
    Device reset_display_manager{};
    Device present_original_game_frame{};
    Action restore_all_surfaces{};
    Action cancel_pending_frame{};
    Sleep sleep_ms{};
};
enum class Outcome:std::uint8_t {Skipped,Submitted,Recoverable,Failed};
struct Result {Outcome outcome{Outcome::Skipped};U32 original_status{};};
// now_ms comes from real timeGetTime(), so a deterministic test can
// simulate original DAT_0044DDD4 one-millisecond frame suppression.
[[nodiscard]] Result run_active_game_frame(
    State&,U32 now_ms,const Callbacks&) noexcept;
[[nodiscard]] bool verify_source_frame_control_flow() noexcept;
}
