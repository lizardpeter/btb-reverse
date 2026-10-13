#include "btb/original_lifecycle12.hpp"
namespace btb::original_lifecycle12 {
bool verify_12_original_gameplay_lifecycle() noexcept {
 // 004110E0: exactly 16 unregisters and a conditional native Release.
 ActivityResources editor{};
 editor.slot[0x0050ab18U]=99;
 original_lifecycle12::unload_fireworks_editor(editor);
 if(editor.actions.size()!=17||
    editor.actions[0].kind!=Op::UnregisterBitmap||
    editor.actions[0].address!=0x0050ab18U||
    editor.actions[1].kind!=Op::ReleaseBitmap||
    editor.handle(0x0050ab18U)!=0)return false;
 // 004111C0: 23 real movie/surface/buffer triples plus four bitmaps.
 ActivityResources bank{};
 bank.slot[0x0050aaacU]=88;
 original_lifecycle12::unload_fireworks_movie_bank(bank);
 if(bank.actions.size()!=73||
    bank.actions[0].kind!=Op::CloseBink||
    bank.actions[0].argument!=88||
    bank.actions[1].kind!=Op::UnregisterBitmap||
    bank.actions[2].kind!=Op::FreeMovieBuffer||
    bank.handle(0x0050aaacU)!=0)return false;
 // 0040A530: three fixed, N dynamic, 3 tail, sound array, cursor and music.
 ActivityResources dino{};
 dino.audio_latch=7;dino.activity_music_playing=true;
 original_lifecycle12::unload_dino(dino,2);
 if(dino.actions.size()!=10||
    dino.actions[dino.actions.size()-2].kind!=Op::ResetCursor||
    dino.actions.back().kind!=Op::StopMusic||
    dino.audio_latch!=0||dino.activity_music_playing)return false;
 // 00419390 Herding: original non-monotonic 0x510738 in released slots.
 ActivityResources herd{};
 herd.herding_character_latch_a=4;herd.herding_character_latch_b=5;
 original_lifecycle12::unload_herding(herd);
 if(herd.actions.size()!=19||
    herd.actions[8].address!=0x00510738U||
    herd.herding_character_latch_a||herd.herding_character_latch_b)
    return false;
 // 0041A390 Maze resets shared UI after 10 surfaces and 20 sound slots.
 ActivityResources maze{};maze.shared_ui_latch=17;
 original_lifecycle12::unload_maze(maze);
 if(maze.actions.size()!=11||maze.shared_ui_latch||
    maze.actions.back().kind!=Op::StopMusic)return false;
 // 00414410 Golf stores input block and releases 18 authored surfaces.
 ActivityResources golf{};
 original_lifecycle12::unload_golf(golf);
 if(golf.actions.size()!=20||golf.skate_input_blocked!=1||
    golf.actions[0].address!=0x0050aedcU||
    golf.actions[1].address!=0x0050ab88U||
    golf.actions.back().kind!=Op::StopMusic)return false;
 // 00424100 Spud Skate unloads Bink resources but does not stop music.
 ActivityResources skate{};
 skate.audio_latch=4;
 original_lifecycle12::unload_spud_skate(skate);
 if(skate.actions.size()!=20||skate.skate_input_blocked!=1||
    skate.audio_latch!=0||skate.actions[1].kind!=Op::CloseBink||
    skate.actions.back().kind==Op::StopMusic)return false;
 // 00424F00 Squirrel 16 authored surface pointers, cursor and music.
 ActivityResources squirrel{};
 original_lifecycle12::unload_squirrel(squirrel);
 if(squirrel.actions.size()!=18||
    squirrel.actions[0].address!=0x00515068U||
    squirrel.actions[16].kind!=Op::ResetCursor)return false;
 // 00420360 Spud Maze validates five variable-length source groups.
 ActivityResources spud{};
 if(original_lifecycle12::unload_spud_maze(spud,{-1,0,0,0,0})||
    !spud.actions.empty())return false;
 if(!original_lifecycle12::unload_spud_maze(spud,{1,2,0,0,0})||
    spud.actions.size()!=19||
    spud.actions.back().kind!=Op::StopMusic)return false;
 // 00412BD0 original palette selection, grid remove, delete-all confirm.
 FireState fire{};
 int removed=0;
 const RemoveCell remove=[&](I32 row,I32 col){
  if(row==0&&col==1)++removed;
 };
 original_lifecycle12::begin_fireworks_action(
     fire,4,0,0,7,remove);
 if(fire.selected_type!=4||fire.bob_actor_state!=1||
    fire.events.size()!=2||
    fire.events[1].kind!=FireAction::Sound||
    fire.events[1].a!=864)return false;
 fire.cells[1]=4;
 original_lifecycle12::begin_fireworks_action(
     fire,12,0,1,1,remove);
 if(removed!=1||fire.events.back().kind!=FireAction::RemoveCell)
    return false;
 original_lifecycle12::begin_fireworks_action(
     fire,25,0,0,1,remove);
 if(!fire.open_delete_all||
    fire.events.back().kind!=FireAction::DeleteAllConfirm)
    return false;
 // 00412D50 original placement notifies actor voice before placement.
 fire.cells[1]=-1;
 int placed=0;
 const PlaceCell place=[&](I32 row,I32 col,I32 item){
  if(row==0&&col==1&&item==4)++placed;
  return true;
 };
 original_lifecycle12::complete_fireworks_action(
     fire,12,0,1,4,place);
 if(placed!=1||fire.bob_actor_state!=10||
    !fire.cursor_cleared||fire.actor_follow_bias!=0||
    fire.events.back().kind!=FireAction::ClearPlacementTool)return false;
 // 00424D10 actual Spud Skate update/exit: original shutdown unloads
 // MAZE, not Skate, and completion movie supersedes default outer state.
 SkateLoop state{};std::vector<Action> actions{};
 state.startup_sound_pending=1;
 if(original_lifecycle12::advance_spud_skate_activity(state,actions)!=0||
    state.startup_sound_pending!=0||actions.size()!=5||
    actions[4].kind!=Op::ManagedSound||
    actions[4].argument!=0x2f6)return false;
 state.phase=2;actions.clear();
 if(original_lifecycle12::advance_spud_skate_activity(state,actions)!=0||
    state.outer_state!=60||state.game_selection!=22||
    state.replay_class!=26||actions[2].kind!=Op::UnloadSkate)
    return false;
 state.global_shutdown=1;state.shared_completion=3;actions.clear();
 if(original_lifecycle12::advance_spud_skate_activity(state,actions)!=1||
    state.outer_state!=64||state.global_shutdown!=0||
    actions[1].kind!=Op::UnloadMaze)return false;
 return true;
}
}
