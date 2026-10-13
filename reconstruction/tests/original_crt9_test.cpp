#include "btb/original_crt9.hpp"
#include <cstdio>
int main(){
 if(!btb::original_crt9::verify_9_original_crt_functions()){
  std::fputs("Nine original CRT runtime functions failed\n",stderr);
  return 1;
 }
 return 0;
}
