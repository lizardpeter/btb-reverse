#include "btb/original_activity9.hpp"
#include <cstdio>
int main(){
 if(!btb::original_activity9::verify_9_original_activity_functions()){
   std::fputs("Nine original source gameplay and media routines failed\n",stderr);
   return 1;
 }
 return 0;
}
