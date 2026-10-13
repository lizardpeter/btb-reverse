#include "btb/original_shared11.hpp"
#include <cstdio>
int main(){
 if(!btb::original_shared11::verify_11_shared_native_functions()){
  std::fputs("original shared runtime 11 failed\n",stderr);
  return 1;
 }
 return 0;
}
