#include "btb/original_msvc30.hpp"
#include <cstdio>
int main(){
    if(!btb::original_msvc30::verify_30_original_msvc_functions()){
        std::fputs("Original MSVC dependency batch failed\n",stderr);
        return 1;
    }
    return 0;
}
