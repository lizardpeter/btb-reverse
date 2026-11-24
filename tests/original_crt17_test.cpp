#include "btb/original_crt17.hpp"
#include <cstdio>
int main(){
  if(!btb::original_crt17::verify_17_original_crt_functions()){
    std::fputs("17 original MSVC runtime functions failed\n",stderr);
    return 1;
  }
  return 0;
}
