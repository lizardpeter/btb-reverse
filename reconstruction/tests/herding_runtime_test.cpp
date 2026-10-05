#include "btb/herding_runtime.hpp"

#include <cassert>
#include <cstddef>
#include <sstream>

using namespace btb::herding;

int main() {
    static_assert(sizeof(RetailEntityRecord32) == 0x64);
    static_assert(offsetof(RetailEntityRecord32, entity_id) == 0x00);
    static_assert(offsetof(RetailEntityRecord32, direction) == 0x08);
    static_assert(offsetof(RetailEntityRecord32, x_float) == 0x1C);
    static_assert(offsetof(RetailEntityRecord32, type) == 0x30);
    static_assert(offsetof(RetailEntityRecord32, source_left) == 0x34);
    static_assert(offsetof(RetailEntityRecord32, surface_ptr32) == 0x44);
    static_assert(offsetof(RetailEntityRecord32, movement_speed) == 0x48);
    static_assert(offsetof(RetailEntityRecord32, behavior_state) == 0x50);
    static_assert(offsetof(RetailEntityRecord32, target_y) == 0x60);

    assert(is_herd_animal(EntityType::Sheep));
    assert(is_herd_animal(EntityType::Rabbit));
    assert(is_herd_animal(EntityType::Duck));
    assert(!is_herd_animal(EntityType::Scruffty));

    assert(required_food_for(EntityType::Duck) == FoodType::DuckFood);
    assert(required_food_for(EntityType::Rabbit) == FoodType::RabbitFood);
    assert(required_food_for(EntityType::Sheep) == FoodType::SheepFood);
    assert(required_food_for(EntityType::FarmerPickles) == FoodType::None);

    assert(classify_behavior_state(0) == BehaviorClass::FreeRoamFollowAndCollision);
    assert(classify_behavior_state(1) == BehaviorClass::BeginHomeRoute);
    for (int state = 10; state <= 15; ++state) {
        assert(classify_behavior_state(state) == BehaviorClass::ApproachHomeEntrance);
    }
    for (int state = 20; state <= 25; ++state) {
        assert(classify_behavior_state(state) == BehaviorClass::EnterHome);
    }
    assert(classify_behavior_state(99) == BehaviorClass::Delivered);
    assert(classify_behavior_state(2) == BehaviorClass::RetailNoOp);
    assert(classify_behavior_state(98) == BehaviorClass::RetailNoOp);

    FollowerList followers;
    assert(followers.count() == 0);
    assert(followers.add(3));
    assert(followers.add(7));
    assert(!followers.add(3));
    assert(followers.contains(7));
    assert(followers.count() == 2);
    assert(followers.remove(3));
    assert(!followers.contains(3));
    assert(followers.count() == 1);

    std::istringstream in(R"(
719 266
527 114
376 170
478 471
93 324
100 120
44 99 202 99 353 279 461 263 530 299 966 181 966 166 966 235 1086 349 1257 405
1257 900 44 900
-1 -1
450 360 600 179 886 139 1028 304 755 360 -1 -1
30 500 500 500 500 500 1200 500 1200 850 30 850 -1 -1
92 324 -1 -1
)");

    const auto data = parse_data(in);
    const auto& patrol = scruffty_patrol_path(data);
    assert(patrol.size() == 5);
    assert((patrol.front() == Vec2i{450, 360}));
    assert((patrol.back() == Vec2i{755, 360}));
}
