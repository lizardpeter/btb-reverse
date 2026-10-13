#include "btb/original_shared_ui8.hpp"
#include <cstdio>
int main(){
 if(!btb::original_shared_ui8::verify_8_original_shared_ui()){
   std::fputs("8 compiled original UI functions failed\n",stderr);
   return 1;
 }
 return 0;
}
