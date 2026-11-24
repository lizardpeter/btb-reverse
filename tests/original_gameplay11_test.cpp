#include "btb/original_gameplay11.hpp"
#include <cstdio>
int main(){
 if(!btb::original_gameplay11::verify_11_gameplay_data_functions()){
  std::fputs("Original 11 game/update/data functions failed\n",stderr);
  return 1;
 }
 return 0;
}
