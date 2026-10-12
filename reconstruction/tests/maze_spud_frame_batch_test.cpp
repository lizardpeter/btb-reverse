#include "btb/maze_spud_frame_batch.hpp"
#include "btb/maze_motion.hpp"
#include "btb/maze_transition.hpp"
#include <cassert>
#include <cstdint>
#include <algorithm>

using namespace btb::maze_spud_batch;
namespace {
bool has(const Frame& frame,Op what) {
    return std::any_of(frame.effects.begin(),frame.effects.end(),
        [&](const Effect& e){return e.op==what;});
}
I32 audio(const Frame& frame,Op what) {
    for(const auto& e:frame.effects)if(e.op==what)return e.argument;
    return -999;
}
}

int main() {
    // 0041D4D0: original continuous Maze frame order and startup voice.
    Maze m{};
    auto out=update_maze(m,{});
    assert(out.return_value==0&&m.shared_activity_latch==1);
    assert(out.effects.size()==4);
    assert(out.effects[0].op==Op::EnableInput);
    assert(out.effects[1].op==Op::StartAmbientSound);
    assert(out.effects[2].op==Op::UpdatePlayer);
    assert(out.effects[3].op==Op::DrawMaze);
    // NPC is updated after the draw, but only when startup phase==0.
    assert(has(out,Op::UpdateNpc));
    m.timer_seconds=15;
    out=update_maze(m,{});
    assert(audio(out,Op::PlayManagedSound)==83);
    assert(out.effects[1].op==Op::StopManagedSounds);

    m.timer_seconds=20;m.startup_state=1;m.startup_sound_pending=1;
    m.difficulty=2;
    out=update_maze(m,{});
    assert(has(out,Op::DrawMaze)&&!has(out,Op::UpdatePlayer));
    assert(!has(out,Op::UpdateNpc));
    assert(audio(out,Op::PlayMusic)==5);
    assert(audio(out,Op::PlayManagedSound)==0x4b);
    assert(audio(out,Op::MarkVoiceInterruptible)==0x4b);
    assert(m.startup_sound_pending==0);
    out=update_maze(m,{.startup_voice_playing=true});
    assert(m.startup_state==1&&has(out,Op::CaptureTime));
    out=update_maze(m,{.startup_voice_playing=false});
    assert(m.startup_state==0);
    // A new scroll transition starts in the middle of movement: source code
    // skips the normal DrawMazeActivity but still runs NPC updates.
    out=update_maze(m,{.player_update_began_transition=true});
    assert(m.transition==1);
    assert(has(out,Op::UpdatePlayer)&&has(out,Op::UpdateNpc));
    assert(!has(out,Op::DrawMaze));
    out=update_maze(m,{});
    assert(has(out,Op::DrawMazeTransition)&&has(out,Op::DrawMaze));
    assert(!has(out,Op::UpdatePlayer));

    // Outcome is held until scrolling ends, then emits a voice, waits until
    // no sound plays, prepares Play Again and finally unloads.
    m.transition=0;m.remaining_packages=0;m.timer_seconds=22;
    out=update_maze(m,{});
    assert(out.return_value==1&&m.outro_phase==1);
    assert(audio(out,Op::PlayManagedSound)==84);
    assert(out.effects.back().op==Op::DrawMaze);
    out=update_maze(m,{.any_managed_sound_playing=true});
    assert(m.outro_phase==1&&out.return_value==1);
    out=update_maze(m,{.any_managed_sound_playing=false});
    assert(m.outro_phase==2);
    m.neighboring_progress_complete=true;
    out=update_maze(m,{});
    assert(out.return_value==1&&out.transitioned);
    assert(has(out,Op::PreparePlayAgain)&&has(out,Op::UnloadMaze));
    assert(m.outer_state==0x3c&&m.ui_context==0x20);
    assert(m.timer_seconds==999&&m.progress_won&&m.shared_completion==3);
    assert(m.shared_replay_latch==1);
    Maze m_timeout{};
    m_timeout.timer_seconds=-1;
    out=update_maze(m_timeout,{});
    assert(out.return_value==1&&audio(out,Op::PlayManagedSound)==86);
    Maze m_slow{};
    m_slow.timer_seconds=3;m_slow.remaining_packages=0;
    out=update_maze(m_slow,{});
    assert(audio(out,Op::PlayManagedSound)==85);
    // Forced shutdown: exit 0x1c then shared completion overrides 0x40.
    Maze exiting{};
    exiting.shutdown_request=true;exiting.shared_completion=4;
    out=update_maze(exiting,{});
    assert(out.return_value==1&&out.transitioned);
    assert(exiting.outer_state==0x40&&!exiting.shutdown_request);
    assert(out.effects[0].op==Op::EnableInput);
    assert(out.effects[1].op==Op::UnloadMaze);

    // 00423F40: repair-success outcome waits for Bentley narration and uses
    // Spud Maze's own 120-tick threshold, replay UI context 0x3a.
    SpudMaze spud{};
    out=update_spud_maze(spud,{});
    assert(out.return_value==0);
    assert(has(out,Op::UpdateSpudMazeBob)&&has(out,Op::DrawSpudMaze));
    assert(has(out,Op::UpdatePilchard));
    spud.startup_sound_pending=1;spud.timer_seconds=15;
    out=update_spud_maze(spud,{});
    assert(out.effects[0].op==Op::EnableInput);
    assert(audio(out,Op::PlayManagedSound)==994); // low timer precedes startup
    assert(audio(out,Op::PlayMusic)==7);
    assert(spud.startup_sound_pending==0);
    assert(has(out,Op::MarkVoiceInterruptible));
    spud.timer_seconds=11;spud.repairs_remaining=0;
    out=update_spud_maze(spud,{.narration_finished=true});
    assert(out.return_value==1&&spud.narration_ticks==120);
    assert(out.effects.back().op==Op::DrawSpudMaze);
    spud.adjacent_progress_complete=true;
    out=update_spud_maze(spud,{});
    assert(out.return_value==1&&out.transitioned);
    assert(spud.progress_won&&spud.shared_completion==2);
    assert(spud.outer_state==60&&spud.chosen_game==22);
    assert(spud.ui_context==58&&spud.replay_latch==1);
    assert(out.effects[out.effects.size()-2].op==Op::PreparePlayAgain);
    assert(out.effects.back().op==Op::UnloadSpudMaze);
    SpudMaze spud_timeout{};
    spud_timeout.timer_seconds=-1;
    spud_timeout.narration_ticks=120;
    out=update_spud_maze(spud_timeout,{});
    assert(out.return_value==1&&!spud_timeout.progress_won);
    assert(spud_timeout.shared_completion==-1);
    SpudMaze spud_scrolling{};
    spud_scrolling.transition=1;spud_scrolling.repairs_remaining=0;
    out=update_spud_maze(spud_scrolling,{});
    assert(out.return_value==0&&!has(out,Op::UpdateBentleyNarration));
    assert(has(out,Op::DrawSpudMazeTransition)&&has(out,Op::UpdatePilchard));
    SpudMaze spud_begins{};
    out=update_spud_maze(spud_begins,{.bob_update_began_transition=true});
    assert(has(out,Op::UpdateSpudMazeBob)&&!has(out,Op::DrawSpudMaze));
    assert(has(out,Op::UpdatePilchard));
    SpudMaze spud_exit{};
    spud_exit.shutdown_request=true;spud_exit.shared_completion=3;
    out=update_spud_maze(spud_exit,{});
    assert(out.return_value==1&&spud_exit.outer_state==64);
    assert(!spud_exit.shutdown_request&&has(out,Op::UnloadSpudMaze));

    // 004211A0: keyboard mode takes precedence only for current frame,
    // while a changed mouse position with no input requests mouse mode next.
    SpudDirection dir{};
    dir.previous_mouse_x=20;dir.previous_mouse_y=10;
    SpudDirectionInput input{};
    input.actor_x=100;input.actor_y=100;
    input.mouse_x=20;input.mouse_y=10;input.button_bits=0x0f;
    input.dead_zone=6;input.x_component_threshold=.25f;
    input.y_component_threshold=.25f;
    auto result=compute_spud_input(dir,input);
    assert(result.x==1&&result.y==-1);
    assert(!result.processed_mouse&&result.wrote_node_radius_four);
    input.button_bits=0;
    input.mouse_x=200;
    result=compute_spud_input(dir,input);
    assert(dir.mouse_mode&&result.x==0&&result.y==0);
    // Following frame follows the mouse, with no 3-sample maze debounce.
    result=compute_spud_input(dir,input);
    assert(result.processed_mouse&&result.wrote_node_radius_four);
    assert(result.x==1);
    dir.mouse_mode=true;
    input.button_bits=1;input.mouse_x=100;input.mouse_y=50;
    result=compute_spud_input(dir,input);
    assert(result.dead_zone_return);
    assert(!result.wrote_node_radius_four);
    assert(!dir.mouse_mode); // keyboard input flips future mode on entry

    // 00423470: cooldown decrements exactly once, collision returns 1 when
    // overlapping even if no hammer was held; no drop when cooldown>0.
    CollisionState collision{};
    collision.hammer_held=true;collision.current_screen=3;
    collision.bob_direction=1;collision.bob_animation=3;
    CollisionInput ci{};
    ci.bob_x=100;ci.bob_y=120;ci.pilchard_x=103;ci.pilchard_y=121;
    ci.impact_distance_exclusive=10;ci.max_bob_animation=30;
    ci.random_speech_roll=4;ci.random_pilchard_roll=1;
    collision.cooldown=2;
    out=handle_pilchard_collision(collision,ci);
    assert(out.return_value==0&&collision.cooldown==1);
    out=handle_pilchard_collision(collision,ci);
    assert(out.return_value==1&&collision.cooldown==500);
    assert(!collision.hammer_held&&collision.hammer_dropped);
    assert(collision.hammer.x==110&&collision.hammer.y==161);
    assert(collision.hammer.screen==3);
    assert(audio(out,Op::PlayManagedSound)==700);
    assert(out.effects.size()==3);
    out=handle_pilchard_collision(collision,ci);
    assert(out.return_value==0&&collision.cooldown==499);

    // Independently exercise source-model functions which already existed
    // as coherent subsystems but remained unregistered in source inventory.
    assert(btb::maze::angle_between_integer_points_degrees(
           {100,100},{100,0})==359);
    btb::maze::ScreenTransitionState transition{
        btb::maze::Screen::Middle,btb::maze::Screen::East};
    const auto transition_frame=btb::maze::advance_screen_transition(transition);
    assert(transition_frame.pixel_step==2&&transition.phase==2);
}
