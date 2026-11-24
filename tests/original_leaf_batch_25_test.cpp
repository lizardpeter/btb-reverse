#include "btb/original_leaf_batch_25.hpp"
#include <cstdio>
int main() {
    if(!btb::original_leaf::verify_25_original_leaf_calls()){
        std::fputs("A source-identified original leaf function failed\n",stderr);
        return 1;
    }
    return 0;
}
