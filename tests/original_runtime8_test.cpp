#include "btb/original_runtime8.hpp"
#include <cstdio>
int main(){
 if(!btb::original_runtime8::verify_8_original_algorithms()){
     std::fputs("Eight original runtime functions failed\n",stderr);
     return 1;
 }
 return 0;
}
