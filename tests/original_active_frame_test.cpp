#include "btb/original_active_frame.hpp"
#include <cstdio>
int main(){
    if(!btb::original_active_frame::verify_source_frame_control_flow()){
        std::fputs("Original 00402440 control-flow test failed\n",stderr);
        return 1;
    }
    return 0;
}
