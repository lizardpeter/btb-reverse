#include "btb/original_runtime5.hpp"
#include <cstdio>
int main(){
 if(!btb::original_runtime5::verify_5_original_runtime_functions()){
    std::fputs("Original five CRT functions failed\n",stderr);
    return 1;
 }
 return 0;
}
