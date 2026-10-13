#include "btb/original_lifecycle12.hpp"
#include <cstdio>
int main(){
 if(!btb::original_lifecycle12::verify_12_original_gameplay_lifecycle()){
  std::fputs("Original twelve gameplay lifecycle functions failed\n",stderr);
  return 1;
 }
 return 0;
}
