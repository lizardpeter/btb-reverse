#include "btb/native_input_pulse.hpp"
#include <cstdio>
int main() {
    bool queued=true;
    const auto first=btb::full_game::consume_native_click(queued);
    if(!first.activity_click||!first.primary_input_click||queued){
        std::fputs("The original click was lost or consumed twice\n",stderr);
        return 1;
    }
    const auto second=btb::full_game::consume_native_click(queued);
    if(second.activity_click||second.primary_input_click||queued)return 2;
    queued=true;
    const auto third=btb::full_game::consume_native_click(queued);
    if(!third.activity_click||!third.primary_input_click||queued)return 3;
    return 0;
}
