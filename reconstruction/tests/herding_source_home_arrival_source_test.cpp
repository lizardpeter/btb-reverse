#include "btb/herding_source_home_arrival.hpp"

#include <cassert>
#include <cstdint>
#include <limits>

namespace h=btb::herding;

static_assert(h::original_herding_within_home_radius(
    {0,0},{0,0}));
static_assert(h::original_herding_within_home_radius(
    {6,7},{0,0})); // sqrt(85)<10
static_assert(!h::original_herding_within_home_radius(
    {6,8},{0,0})); // sqrt(100)==10 is NOT arrival
static_assert(!h::original_herding_within_home_radius(
    {10,0},{0,0}));
static_assert(h::original_herding_within_home_radius(
    {-9,0},{0,0}));
static_assert(!h::original_herding_within_home_radius(
    {-10,0},{0,0}));

int main() {
    for (const auto type : {
        h::EntityType::Sheep,h::EntityType::Rabbit,
        h::EntityType::Duck}) {
        h::RetailEntityRecord32 animal{};
        animal.type=static_cast<int>(type);
        animal.behavior_state=10;
        const auto entrance=h::home_entrance_target(type);
        assert(entrance.x>=0);
        animal.x=entrance.x;animal.y=entrance.y;
        assert(h::original_herding_home_arrival(animal));

        animal.x=entrance.x+6;
        animal.y=entrance.y+8;
        assert(!h::original_herding_home_arrival(animal));
        animal.x=entrance.x+6;
        animal.y=entrance.y+7;
        assert(h::original_herding_home_arrival(animal));

        for (int i=0;i<5;++i) {
            animal.behavior_state=20+i;
            const auto final_target=h::home_entry_target(
                type,animal.behavior_state);
            assert(final_target);
            animal.x=final_target->x;
            animal.y=final_target->y;
            assert(h::original_herding_home_arrival(animal));
        }
        animal.behavior_state=99;
        assert(!h::original_herding_home_arrival(animal));
    }
    assert(!h::original_herding_within_home_radius(
        {std::numeric_limits<std::int32_t>::max(),
         std::numeric_limits<std::int32_t>::max()},
        {std::numeric_limits<std::int32_t>::min(),
         std::numeric_limits<std::int32_t>::min()}));
}
