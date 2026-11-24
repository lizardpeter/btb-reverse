#include "btb/original_system14.hpp"
#include <cstdio>
int main(){
 if(!btb::original_system14::verify_14_original_system_functions()){
   std::fputs("14 original COM and MSVC destructors failed\n",stderr);
   return 1;
 }
 return 0;
}
