#include "btb/herding_source_heading.hpp"
#include "btb/herding_source_steering.hpp"

#include <cassert>
#include <cstdint>
#include <limits>

namespace h=btb::herding;

int main() {
    // The executable's 9999 fallback resolves perfectly vertical UP
    // to 359 degrees; using atan2 would produce 0 and incorrectly
    // change the sprite cell and tiny horizontal movement.
    assert(h::original_herding_integer_heading(0,0,0,-10)==359);
    assert(h::original_herding_integer_heading(0,0,0,10)==180);
    assert(h::original_herding_integer_heading(0,0,10,0)==90);
    assert(h::original_herding_integer_heading(0,0,-10,0)==270);

    // Retail uses 57.294998 instead of the more accurate
    // 180/pi = 57.2957795. Truncation introduces asymmetric exact
    // degree values even at 45-degree diagonals.
    assert(h::original_herding_integer_heading(0,0,10,-10)==45);
    assert(h::original_herding_integer_heading(0,0,10,10)==134);
    assert(h::original_herding_integer_heading(0,0,-10,-10)==314);
    assert(h::original_herding_integer_heading(0,0,-10,10)==225);
    assert(h::original_herding_integer_heading(0,0,0,0)==359);

    // Source heading is derived from integer actor and target.
    // Its value must flow into the same source-based sprite index
    // and FSIN/FCOS per-frame movement code.
    const auto north=h::original_herding_integer_heading(
        478,471,478,400);
    assert(north==359);
    const auto step=h::original_herding_steering_step(
        478.0f,471.0f,*north,0.5f,true);
    assert(step);
    assert(step->facing_index==0);
    assert(step->x<478.0f); // original 359-degree leftward artifact
    assert(step->y<471.0f);
    assert(step->next_speed>0.5f);

    // Signed 32-bit coordinate subtraction outside native gameplay
    // domain is refused rather than generating unsafe modern overflow.
    assert(!h::original_herding_integer_heading(
        std::numeric_limits<std::int32_t>::max(),0,
        std::numeric_limits<std::int32_t>::min(),0));
}
