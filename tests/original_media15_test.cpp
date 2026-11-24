#include "btb/original_media15.hpp"
#include <cstdio>
int main(){
 if(!btb::original_media15::verify_15_original_media_functions()){
   std::fputs("Source-compiled media and movie functions failed\n",stderr);
   return 1;
 }
 return 0;
}
