#include "btb/original_games_batch_14.hpp"
#include <cstdio>
int main(){
  if(!btb::original_games14::verify_14_original_game_systems()){
    std::fputs("14 original game procedures failed\n",stderr);
    return 1;
  }
  return 0;
}
