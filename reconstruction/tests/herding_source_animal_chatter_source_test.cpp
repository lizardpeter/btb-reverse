#include "btb/herding_source_animal_chatter.hpp"

#include <cassert>

namespace h=btb::herding;
namespace r=btb::retail;

constexpr bool verified_idle_sequence() {
    r::OriginalRetailRandom rng{1};
    // srand(1): first result 41, so 41%8=1. All three source voices
    // idle -> second result 18467, selects voice index 2.
    const auto step=h::original_herding_animal_chatter(
        rng,1,{false,false,false});
    return step && step->requested_sound_id==822 &&
        step->random_calls==2 &&
        step->consulted_sound_group_status &&
        !step->used_random_eighth_override;
}
static_assert(verified_idle_sequence());

constexpr bool busy_group_preserves_next_random() {
    r::OriginalRetailRandom rng{1};
    const auto busy=h::original_herding_animal_chatter(
        rng,0,{false,true,false});
    return busy && !busy->requested_sound_id &&
        busy->random_calls==1 &&
        busy->consulted_sound_group_status &&
        rng.next_rand()==18467;
}
static_assert(busy_group_preserves_next_random());

constexpr bool rare_eighth_overrides_active_status() {
    r::OriginalRetailRandom rng{3};
    // Seed 3 yields first result 48 (48%8==0) and next result
    // 7196 (7196%3==2), so request group2's highest voice ID.
    const auto step=h::original_herding_animal_chatter(
        rng,2,{true,true,true});
    return step && step->requested_sound_id==825 &&
        step->used_random_eighth_override &&
        !step->consulted_sound_group_status &&
        step->random_calls==2 &&
        step->source_priority==10 && step->source_class==0;
}
static_assert(rare_eighth_overrides_active_status());

int main() {
    r::OriginalRetailRandom rng{1};
    const auto old=rng.state();
    assert(!h::original_herding_animal_chatter(
        rng,-1,{false,false,false}));
    assert(rng.state()==old);
    assert(!h::original_herding_animal_chatter(
        rng,3,{false,false,false}));
    assert(rng.state()==old);

    const auto played=h::original_herding_animal_chatter(
        rng,1,{false,false,false});
    assert(played && played->requested_sound_id==822);
    const auto next=rng.next_rand();
    assert(next==6334);
}
