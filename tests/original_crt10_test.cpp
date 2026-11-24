#include "btb/original_crt10.hpp"
#include <cstdio>
int main(){
 if(!btb::original_crt10::verify_10_original_crt_io_functions()){
  std::fputs("Ten native original CRT I/O and heap functions failed\n",stderr);
  return 1;
 }
 return 0;
}
