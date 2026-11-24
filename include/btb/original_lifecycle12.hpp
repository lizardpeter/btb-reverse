#pragma once
// Twelve original executable functions reconstructed out-of-line as C++26.
// Ghidra original source per address in analysis/ghidra_main_exe/functions.
// Native release/COM/Bink operations are ordered host effects, not invented.
#include "btb/retail_game_lifecycle_15.hpp"
namespace btb::original_lifecycle12 {
using namespace btb::game_batch_15;
void unload_fireworks_editor(ActivityResources&); // 004110E0
void unload_fireworks_movie_bank(ActivityResources&); // 004111C0
void unload_dino(ActivityResources&,I32); // 0040A530
void unload_herding(ActivityResources&); // 00419390
void unload_maze(ActivityResources&); // 0041A390
void unload_golf(ActivityResources&); // 00414410
void unload_spud_skate(ActivityResources&); // 00424100
void unload_squirrel(ActivityResources&); // 00424F00
[[nodiscard]] bool unload_spud_maze(ActivityResources&,const std::array<I32,5>&); // 00420360
void begin_fireworks_action(FireState&,I32,I32,I32,U32,const RemoveCell&); // 00412BD0
void complete_fireworks_action(FireState&,I32,I32,I32,U32,const PlaceCell&); // 00412D50
[[nodiscard]] I32 advance_spud_skate_activity(SkateLoop&,std::vector<Action>&); // 00424D10
[[nodiscard]] bool verify_12_original_gameplay_lifecycle() noexcept;
}
