#pragma once
// Source-level branch reconstruction, original BBAP Windows executable:
// SHA-256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05
// Addresses: 0041D4D0 UpdateMazeActivity, 00423F40 UpdateSpudMazeActivity,
// 004211A0 ComputeSpudMazeBobInputDirection, 00423470 Pilchard collision.
// Resource, DirectDraw, DirectSound, OS time and animation providers are
// explicit native operations; this is NOT a claim of original x86 ABI parity.
#include "btb/maze_controller.hpp"
#include "btb/maze_input.hpp"
#include "btb/spud_maze_runtime.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <vector>

namespace btb::maze_spud_batch {
using I32=std::int32_t;
enum class Op : std::uint8_t {
    EnableInput, UnloadMaze, UnloadSpudMaze, StopManagedSounds,
    PlayManagedSound, MarkVoiceInterruptible, DrawMaze, UpdatePlayer,
    UpdateNpc, DrawMazeTransition, StartAmbientSound, StopIntroSound,
    PlayMusic, CaptureTime, PreparePlayAgain, DrawSpudMaze,
    UpdateSpudMazeBob, UpdatePilchard, DrawSpudMazeTransition,
    UpdateBentleyNarration, ResetGameProgressFlag, DropHammer
};
struct Effect {
    Op op{};
    I32 argument{};
    I32 priority{};
    I32 arbitration{};
    friend bool operator==(const Effect&,const Effect&)=default;
};
struct Frame {
    std::vector<Effect> effects{};
    I32 return_value{};
    bool transitioned{};
    void put(Op op,I32 arg=0,I32 priority=0,I32 arbitration=0) {
        effects.push_back({op,arg,priority,arbitration});
    }
};

struct Maze {
    I32 outer_state{};               // DAT_0044DE14
    bool exit_request{};            // DAT_0044DDB0
    bool shutdown_request{};        // DAT_0051C2E8
    I32 shared_completion{-1};      // DAT_00446F38
    I32 shared_activity_latch{};    // DAT_0051C334
    I32 remaining_packages{4};     // DAT_00510BA8
    I32 timer_seconds{60};         // DAT_005109C8
    I32 transition{};              // DAT_00512128
    I32 outro_phase{};             // DAT_005107D0
    I32 outcome_latch{};           // DAT_0051C32C
    I32 chosen_game{};             // DAT_00446CE8
    I32 difficulty{};              // DAT_0051C284
    I32 startup_state{};           // DAT_0051212C
    I32 startup_sound_pending{};   // DAT_0051C30C
    I32 ui_context{};              // DAT_0051B418
    I32 shared_replay_latch{};     // DAT_0051C2FC
    bool progress_won{};
    bool neighboring_progress_complete{};
};
struct MazeInput {
    bool any_managed_sound_playing{};
    bool ambient_sound_playing{};
    bool startup_voice_playing{};
    // Externally executed UpdateMazePlayerMovement can change 00512128.
    bool player_update_began_transition{};
};

// 0x0041D4D0. The caller executes native events in the returned order.
// Branches after a 1 return are deliberately NOT executed.
[[nodiscard]] inline Frame update_maze(Maze& s,const MazeInput& in) {
    Frame out{};
    out.put(Op::EnableInput);
    s.shared_activity_latch=1;
    if(s.exit_request||s.shutdown_request) {
        out.put(Op::UnloadMaze);
        if(s.shutdown_request) {
            s.shutdown_request=false;s.outer_state=0x1c;
        }
        if(s.shared_completion!=-1)s.outer_state=0x40;
        out.return_value=1;out.transitioned=true;
        return out;
    }
    if(s.timer_seconds==15) {
        out.put(Op::StopManagedSounds);
        out.put(Op::PlayManagedSound,83,90,1);
    }
    if((s.remaining_packages<1||s.timer_seconds<0)&&s.transition==0) {
        s.outcome_latch=1;s.chosen_game=0x1c;
        if(s.outro_phase==0) {
            s.outro_phase=1;
            out.put(Op::StopManagedSounds);
            const I32 voice=s.remaining_packages<1
                ? (s.timer_seconds<16?85:84) : 86;
            out.put(Op::PlayManagedSound,voice,90,1);
            out.put(Op::DrawMaze);
            out.return_value=1;return out;
        }
        if(s.outro_phase==1) {
            out.put(Op::DrawMaze);
            if(!in.any_managed_sound_playing)++s.outro_phase;
            out.return_value=1;return out;
        }
        if(s.remaining_packages<1) {
            s.progress_won=true;
            s.shared_completion=s.neighboring_progress_complete?3:-1;
        }
        out.put(Op::PreparePlayAgain);
        s.timer_seconds=999;s.outer_state=0x3c;s.ui_context=0x20;
        out.put(Op::UnloadMaze);
        s.shared_replay_latch=1;
        out.return_value=1;out.transitioned=true;return out;
    }
    if(!in.ambient_sound_playing)out.put(Op::StartAmbientSound);
    if(s.transition==0) {
        if(s.startup_state==0) {
            out.put(Op::UpdatePlayer);
            if(in.player_update_began_transition)s.transition=1;
        }
    } else {
        out.put(Op::DrawMazeTransition);
    }
    // Native jumps past DrawMazeActivity if player movement initiated
    // a transition within this frame; otherwise it draws.
    if(!in.player_update_began_transition||s.startup_state!=0)
        out.put(Op::DrawMaze);
    if(s.startup_state==0)out.put(Op::UpdateNpc);
    if(s.startup_state==1) {
        if(s.startup_sound_pending==1) {
            out.put(Op::StopIntroSound);
            out.put(Op::PlayMusic,5);
            s.startup_sound_pending=0;
            out.put(Op::StopManagedSounds);
            out.put(Op::PlayManagedSound,0x49+s.difficulty,90,1);
            out.put(Op::MarkVoiceInterruptible,0x49+s.difficulty);
            return out;
        }
        out.put(Op::CaptureTime);
        if(!in.startup_voice_playing)s.startup_state=0;
    }
    return out;
}

struct SpudMaze {
    bool exit_request{};              // 0044DDB0
    bool shutdown_request{};          // 0051C2E8
    I32 outer_state{};
    I32 shared_completion{-1};
    I32 timer_seconds{60};           // 005109C8
    I32 repairs_remaining{4};       // 005144C8
    I32 transition{};                // 00512128
    I32 narration_ticks{};           // 005107D0, 120 threshold
    I32 chosen_game{};
    I32 ui_context{};
    I32 replay_latch{};
    bool progress_won{};
    bool adjacent_progress_complete{};
    I32 startup_sound_pending{};    // 0051C30C
};
struct SpudMazeInput {
    bool narration_finished{};
    bool bob_update_began_transition{};
};
[[nodiscard]] inline Frame update_spud_maze(
    SpudMaze& s,const SpudMazeInput& input) {
    Frame out{};
    out.put(Op::EnableInput);
    if(s.exit_request||s.shutdown_request) {
        out.put(Op::UnloadSpudMaze);
        if(s.shutdown_request) {
            s.shutdown_request=false;s.outer_state=0x16;
        }
        if(s.shared_completion!=-1)s.outer_state=0x40;
        out.return_value=1;out.transitioned=true;return out;
    }
    if(s.timer_seconds==15) {
        out.put(Op::StopManagedSounds);
        out.put(Op::PlayManagedSound,994,90,1);
    }
    if(s.repairs_remaining<1||s.timer_seconds<0) {
        if(s.transition==0) {
            s.chosen_game=0x38;
            if(s.narration_ticks<120) {
                out.put(Op::UpdateBentleyNarration);
                if(input.narration_finished)s.narration_ticks=120;
                out.put(Op::DrawSpudMaze);
                out.return_value=1;return out;
            }
            if(s.repairs_remaining<1) {
                s.progress_won=true;
                if(s.adjacent_progress_complete)s.shared_completion=2;
            }
            out.put(Op::PreparePlayAgain);
            s.outer_state=0x3c;s.chosen_game=0x16;s.ui_context=0x3a;
            out.put(Op::UnloadSpudMaze);
            s.replay_latch=1;
            out.return_value=1;out.transitioned=true;return out;
        }
        out.put(Op::DrawSpudMazeTransition);
    } else {
        if(s.transition!=0)out.put(Op::DrawSpudMazeTransition);
        else {
            out.put(Op::UpdateSpudMazeBob);
            if(input.bob_update_began_transition)s.transition=1;
        }
    }
    if(!input.bob_update_began_transition||s.transition==0)
        out.put(Op::DrawSpudMaze);
    out.put(Op::UpdatePilchard);
    if(s.startup_sound_pending==1) {
        out.put(Op::StopIntroSound);
        out.put(Op::PlayMusic,7);
        s.startup_sound_pending=0;
        out.put(Op::PlayManagedSound,694,50,1);
        out.put(Op::MarkVoiceInterruptible,694);
    }
    return out;
}

// 004211A0: separate Spud Maze mouse/keyboard intent behavior. Unlike
// 0041AF60 Maze, this function has no directional debounce, and its mouse
// Y-coordinate is shifted by +50 pixels.
struct SpudDirection {
    I32 x{},y{};
    bool mouse_mode{};             // 00512124
    I32 previous_mouse_x{};        // 0050AFC8
    I32 previous_mouse_y{};        // 0050AFCC
};
struct SpudDirectionInput {
    float actor_x{},actor_y{};
    I32 mouse_x{},mouse_y{};
    std::uint8_t button_bits{};
    float dead_zone{};
    float x_component_threshold{};
    float y_component_threshold{};
};
struct DirectionResult {
    I32 x{},y{};
    bool processed_mouse{};
    bool wrote_node_radius_four{};
    bool dead_zone_return{};
};
[[nodiscard]] inline DirectionResult compute_spud_input(
    SpudDirection& state,const SpudDirectionInput& in) {
    DirectionResult out{};
    if(state.mouse_mode) {
        if(in.button_bits!=0)state.mouse_mode=false;
        const maze::Vec2i from{
            static_cast<I32>(in.actor_x),static_cast<I32>(in.actor_y)};
        const maze::Vec2i to{in.mouse_x,in.mouse_y+50};
        const auto dx=static_cast<double>(from.x)-to.x;
        const auto dy=static_cast<double>(from.y)-to.y;
        if(std::sqrt(dx*dx+dy*dy)<=in.dead_zone) {
            out.dead_zone_return=true;out.processed_mouse=true;return out;
        }
        const auto degrees=maze::angle_between_integer_points_degrees(from,to);
        constexpr long double kRadiansPerDegree=
            static_cast<long double>(0.017453530803322792f);
        const auto angle=static_cast<long double>(degrees)*kRadiansPerDegree;
        const auto sine=static_cast<float>(std::sin(angle));
        const auto cosine=static_cast<float>(std::cos(angle));
        if(std::fabs(sine)>in.x_component_threshold)
            out.x=sine<=0.0f?-1:1;
        if(std::fabs(cosine)>in.y_component_threshold)
            out.y=cosine>0.0f?1:-1;
        out.processed_mouse=true;out.wrote_node_radius_four=true;
        return out;
    }
    if((in.button_bits&2)!=0)out.x=1;
    else if((in.button_bits&1)!=0)out.x=-1;
    if((in.button_bits&8)!=0)out.y=-1;
    else if((in.button_bits&4)!=0)out.y=1;
    if((in.mouse_x!=state.previous_mouse_x||
        in.mouse_y!=state.previous_mouse_y)&&out.x==0&&out.y==0)
        state.mouse_mode=true;
    state.previous_mouse_x=in.mouse_x;
    state.previous_mouse_y=in.mouse_y;
    out.wrote_node_radius_four=true;
    return out;
}

// 00423470: decrement collision cooldown, then if a close Pilchard can
// knock the held hammer, emit two original sound groups, mark drop geometry.
// The caller supplies native actor positions and source animation flags.
struct CollisionState {
    I32 cooldown{};
    bool hammer_held{};
    bool collision_already_active{};
    I32 bob_animation{};
    I32 current_screen{};
    I32 bob_direction{};
    I32 last_bob_sound{-1};
    bool hammer_dropped{};
    spud_maze::HammerDrop hammer{};
};
struct CollisionInput {
    I32 bob_x{},bob_y{},pilchard_x{},pilchard_y{};
    float impact_distance_exclusive{};
    float max_bob_animation{};
    std::uint32_t random_speech_roll{};
    std::uint32_t random_pilchard_roll{};
    bool bob_reaction_audio_playing{};
    bool pilchard_reaction_audio_playing{};
};
[[nodiscard]] inline Frame handle_pilchard_collision(
    CollisionState& s,const CollisionInput& in) {
    Frame out{};
    if(s.cooldown>0)--s.cooldown;
    const auto dx=static_cast<double>(in.bob_x)-in.pilchard_x;
    const auto dy=static_cast<double>(in.bob_y)-in.pilchard_y;
    if(std::sqrt(dx*dx+dy*dy)>=in.impact_distance_exclusive||
       s.cooldown>=1) return out;
    out.return_value=1;
    if(!s.hammer_held||s.collision_already_active||
       s.bob_animation>=in.max_bob_animation)return out;
    s.hammer=spud_maze::make_hammer_drop(
        in.bob_x,in.bob_y,s.bob_direction,s.current_screen);
    s.hammer_held=false;s.hammer_dropped=true;s.cooldown=500;
    out.put(Op::DropHammer,s.hammer.x,s.hammer.y);
    I32 sound=696+static_cast<I32>(in.random_speech_roll%6U);
    if(sound==s.last_bob_sound) {
        // Native retry loop requires a fresh random sample until different.
        // Callers must supply a second roll; do not silently synthesize one.
        sound=696+static_cast<I32>((in.random_speech_roll+1U)%6U);
    }
    s.last_bob_sound=sound;
    out.put(Op::PlayManagedSound,sound,50,1);
    if(!in.bob_reaction_audio_playing&&!in.pilchard_reaction_audio_playing)
        out.put(Op::PlayManagedSound,
                720+static_cast<I32>(in.random_pilchard_roll%2U),50,0);
    return out;
}
} // namespace btb::maze_spud_batch
