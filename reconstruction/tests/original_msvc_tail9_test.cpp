#include "btb/original_msvc_tail9.hpp"
#include <cstdio>
int main(){
 if(!btb::original_msvc_tail9::verify_9_original_msvc_tail()){
  std::fputs("Nine original MSVC2003 runtime functions failed\n",stderr);
  return 1;
 }
 return 0;
}
