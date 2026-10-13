#include "btb/original_gameplay9.hpp"
#include <cstdio>
int main(){
 if(!btb::original_gameplay9::verify_9_original_gameplay_math_functions()){
   std::fputs("Nine original cursor/fireworks/Bink/maths routines failed\n",stderr);
   return 1;
 }
 return 0;
}
