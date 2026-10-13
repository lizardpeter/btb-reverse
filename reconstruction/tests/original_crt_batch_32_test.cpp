#include "btb/original_crt_batch_32.hpp"
#include <cstdio>
int main() {
    if (!btb::original_crt32::verify_32_original_crt_functions()) {
        std::fputs("Original CRT batch 32 failed\n",stderr);
        return 1;
    }
    return 0;
}
