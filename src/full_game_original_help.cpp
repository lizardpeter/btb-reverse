#include "btb/full_game_runtime.hpp"
namespace btb::full_game {
void GameRoot::begin_original_help(help::State& state,bool popup,bool progress,help::Services& io) {
    help::enter_original_help_mode(state,globals_.dispatcher.current_state,popup,progress,io);
}
}
