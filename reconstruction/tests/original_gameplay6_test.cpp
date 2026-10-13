#include "btb/original_gameplay6.hpp"
#include <cstdio>
int main(){
 if(!btb::original_gameplay6::verify_6_original_gameplay_functions()){
     std::fputs("Six source-derived native gameplay functions failed\n",stderr);
     return 1;
 }
 return 0;
}
