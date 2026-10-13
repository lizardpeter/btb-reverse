#include "btb/original_source9.hpp"
#include <cstdio>
int main(){
    if(!btb::original_source9::verify_9_original_source_functions()){
        std::fputs("Nine native Spud, Park Designer and MSVC original functions failed\n",stderr);
        return 1;
    }
    return 0;
}
