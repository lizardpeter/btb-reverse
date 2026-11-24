#include "btb/original_services8.hpp"
#include <cstdio>
int main(){
 if(!btb::original_services8::verify_8_original_services()){
   std::fputs("Original services regression failed\n",stderr);
   return 1;
 }
 return 0;
}
