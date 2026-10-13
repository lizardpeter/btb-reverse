#include "btb/original_control6.hpp"
#include <cstdio>
int main(){
 if(!btb::original_control6::verify_6_original_gameplay_controls()){
  std::fputs("Six original gameplay control functions failed\n",stderr);
  return 1;
 }
 return 0;
}
