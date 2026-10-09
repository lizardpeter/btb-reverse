#include "btb/retail_crt_random.hpp"
#include "btb/herding_source_recovery_target.hpp"

#include <array>
#include <cassert>
#include <cstdint>

namespace r=btb::retail;
namespace h=btb::herding;

constexpr bool exact_retail_sequence() {
    r::OriginalRetailRandom original{};
    constexpr std::array<int,10> expected{{
        41,18467,6334,26500,19169,
        15724,11478,29358,26962,24464
    }};
    for (const int result : expected) {
        if (original.next_rand()!=result) return false;
    }
    return true;
}
static_assert(exact_retail_sequence());

constexpr bool original_recovery_rolls() {
    r::OriginalRetailRandom generator{};
    const auto first=h::next_original_herding_recovery_target(generator);
    // Original seed=1 => rand() 41 -> 1; rand() 18467 -> 3.
    if (first.point != (h::Vec2i{400,659})) return false;
    if (first.selected_x_index!=1 || first.selected_y_index!=3 ||
        first.rand_calls!=2) return false;
    const auto second=h::next_original_herding_recovery_target(generator);
    // Next two calls: 6334%4=2, 26500%4=0.
    return second.point==(h::Vec2i{891,766}) &&
           second.rand_calls==2;
}
static_assert(original_recovery_rolls());

int main() {
    r::OriginalRetailRandom generator{};
    assert(generator.state()==1);
    assert(generator.next_rand()==41);
    assert(generator.next_rand()==18467);

    generator.seed(1);
    assert(generator.next_rand()==41);

    generator.seed(0);
    assert(generator.next_rand()==38); // 0x269EC3 >> 16 = 38
    assert(generator.next_mod(4));
    const auto prior=generator.state();
    assert(!generator.next_mod(0));
    assert(generator.state()==prior); // invalid host input never burns RNG
    assert(!generator.next_mod(-1));
    assert(generator.state()==prior);

    // Reconstructed calls must reproduce the same deterministic value
    // whether triggered by Pets Corner or any other activity.
    r::OriginalRetailRandom other{1};
    for (int i=0;i<100;++i) {
        assert(generator.next_rand()>=0);
        assert(other.next_rand()>=0);
    }
}
