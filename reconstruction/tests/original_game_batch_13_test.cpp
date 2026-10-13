#include "btb/original_game_batch_13.hpp"
#include <cstdio>
int main(){
  if(!btb::original_game13::verify_13_original_game_calls()){
    std::fputs("13-function original game C++ batch failed\n",stderr);
    return 1;
  }
  return 0;
}
