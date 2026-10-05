#include "btb/herding_runtime.hpp"

#include <cassert>
#include <cstddef>
#include <sstream>

using namespace btb::herding;

int main() {
    static_assert(sizeof(RetailEntityRecord32) == 0x64);
    static_assert(offsetof(RetailEntityRecord32, entity_id) == 0x00);
    static_assert(offsetof(RetailEntityRecord32, direction) == 0x08);
    static_assert(offsetof(RetailEntityRecord32, direction_degrees) == 0x1C);
    static_assert(offsetof(RetailEntityRecord32, x_float) == 0x20);
    static_assert(offsetof(RetailEntityRecord32, y_float) == 0x24);
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
    static_assert(entity_type_has_retail_constructor(EntityType::FarmerPickles));
    static_assert(entity_type_has_retail_constructor(EntityType::Sheep));
    static_assert(entity_type_has_retail_constructor(EntityType::Scruffty));
    static_assert(!entity_type_has_retail_constructor(EntityType::DormantLegacy4));
    static_assert(!entity_type_has_retail_constructor(EntityType::DormantLegacy5));
    static_assert(!entity_type_has_retail_constructor(EntityType::DormantLegacy6));

    assert(required_food_for(EntityType::Duck) == FoodType::DuckFood);
    assert(required_food_for(EntityType::Rabbit) == FoodType::RabbitFood);
    assert(required_food_for(EntityType::Sheep) == FoodType::SheepFood);
    assert(required_food_for(EntityType::FarmerPickles) == FoodType::None);

    assert(home_for(EntityType::Sheep) == HomeKind::Pen);
    assert(home_for(EntityType::Rabbit) == HomeKind::RabbitHutches);
    assert(home_for(EntityType::Duck) == HomeKind::Pond);
    assert((home_entrance_target(EntityType::Sheep) == Vec2i{639, 135}));
    assert((home_entrance_target(EntityType::Rabbit) == Vec2i{815, 91}));
    assert((home_entrance_target(EntityType::Duck) == Vec2i{1040, 264}));

    static_assert(
        home_entry_target(EntityType::Sheep, 20) &&
        *home_entry_target(EntityType::Sheep, 20) == Vec2i{503, 142});
    static_assert(
        home_entry_target(EntityType::Sheep, 24) &&
        *home_entry_target(EntityType::Sheep, 24) == Vec2i{656, 110});
    static_assert(
        home_entry_target(EntityType::Rabbit, 20) &&
        *home_entry_target(EntityType::Rabbit, 20) == Vec2i{738, 106});
    static_assert(
        home_entry_target(EntityType::Rabbit, 24) &&
        *home_entry_target(EntityType::Rabbit, 24) == Vec2i{807, 107});
    static_assert(
        home_entry_target(EntityType::Duck, 20) &&
        *home_entry_target(EntityType::Duck, 20) == Vec2i{933, 255});
    static_assert(
        home_entry_target(EntityType::Duck, 24) &&
        *home_entry_target(EntityType::Duck, 24) == Vec2i{1022, 286});
    static_assert(!home_entry_target(EntityType::Duck, 25));
    static_assert(!home_entry_target(EntityType::Scruffty, 20));
    static_assert(kHomeRouteArrivalDistance == 10.0F);

    constexpr auto entrance_arrival = home_route_arrival_step(10, 9);
    static_assert(entrance_arrival);
    static_assert(entrance_arrival->next_behavior_state == 20);
    static_assert(entrance_arrival->next_undelivered_count == 9);
    static_assert(!entrance_arrival->delivered);

    constexpr auto entrance_arrival_last_slot = home_route_arrival_step(14, 9);
    static_assert(entrance_arrival_last_slot);
    static_assert(entrance_arrival_last_slot->next_behavior_state == 24);

    constexpr auto delivered_arrival = home_route_arrival_step(24, 1);
    static_assert(delivered_arrival);
    static_assert(delivered_arrival->next_behavior_state == 99);
    static_assert(delivered_arrival->next_undelivered_count == 0);
    static_assert(delivered_arrival->delivered);

    static_assert(!home_route_arrival_step(15, 9));
    static_assert(!home_route_arrival_step(25, 9));
    static_assert(!home_route_arrival_step(99, 0));

    assert(animals_per_species(0) == 3);
    assert(animals_per_species(1) == 4);
    assert(animals_per_species(2) == 5);
    assert(!scruffty_enabled(0));
    assert(scruffty_enabled(1));
    assert(scruffty_enabled(2));
    assert(initial_undelivered_animal_count(0) == 9);
    assert(initial_undelivered_animal_count(1) == 12);
    assert(initial_undelivered_animal_count(2) == 15);

    assert(food_pickup_sound_ids(FoodType::DuckFood).pickles_line == 582);
    assert(food_pickup_sound_ids(FoodType::DuckFood).travis_line == 609);
    assert(food_pickup_sound_ids(FoodType::RabbitFood).pickles_line == 583);
    assert(food_pickup_sound_ids(FoodType::SheepFood).travis_line == 611);

    assert(attraction_sound_ids(FoodType::DuckFood).a == 585);
    assert(attraction_sound_ids(FoodType::DuckFood).b == 586);
    assert(attraction_sound_ids(FoodType::RabbitFood).a == 587);
    assert(attraction_sound_ids(FoodType::SheepFood).b == 590);
    assert(species_home_route_sound_id(EntityType::Sheep) == 596);
    assert(species_home_route_sound_id(EntityType::Rabbit) == 597);
    assert(species_home_route_sound_id(EntityType::Duck) == 598);

    constexpr auto sheep_easy_first =
        begin_home_route_step(EntityType::Sheep, 0, 0);
    static_assert(sheep_easy_first);
    static_assert(sheep_easy_first->assigned_behavior_state == 10);
    static_assert(sheep_easy_first->next_species_route_counter == 1);
    static_assert(!sheep_easy_first->final_species_animal);
    static_assert(sheep_easy_first->sound_id == -1);
    static_assert(
        sheep_easy_first->gate_trigger == HomeRouteGateTrigger::None);
    static_assert(sheep_easy_first->gate_animation_timer == 0);

    constexpr auto sheep_easy_last =
        begin_home_route_step(EntityType::Sheep, 2, 0);
    static_assert(sheep_easy_last);
    static_assert(sheep_easy_last->assigned_behavior_state == 12);
    static_assert(sheep_easy_last->next_species_route_counter == 3);
    static_assert(sheep_easy_last->final_species_animal);
    static_assert(sheep_easy_last->sound_id == 596);
    static_assert(
        sheep_easy_last->gate_trigger == HomeRouteGateTrigger::LeftGate);
    static_assert(
        sheep_easy_last->gate_animation_timer ==
        kHomeRouteGateAnimationTimer);

    constexpr auto rabbit_medium_last =
        begin_home_route_step(EntityType::Rabbit, 3, 1);
    static_assert(rabbit_medium_last);
    static_assert(rabbit_medium_last->assigned_behavior_state == 13);
    static_assert(rabbit_medium_last->sound_id == 597);
    static_assert(
        rabbit_medium_last->gate_trigger ==
        HomeRouteGateTrigger::RightGate);
    static_assert(rabbit_medium_last->gate_animation_timer == 50);

    constexpr auto duck_hard_last =
        begin_home_route_step(EntityType::Duck, 4, 2);
    static_assert(duck_hard_last);
    static_assert(duck_hard_last->assigned_behavior_state == 14);
    static_assert(duck_hard_last->next_species_route_counter == 5);
    static_assert(duck_hard_last->final_species_animal);
    static_assert(duck_hard_last->sound_id == 598);
    static_assert(
        duck_hard_last->gate_trigger == HomeRouteGateTrigger::None);
    static_assert(duck_hard_last->gate_animation_timer == 0);

    static_assert(!begin_home_route_step(EntityType::Scruffty, 0, 0));
    static_assert(!begin_home_route_step(EntityType::Sheep, 3, 0));
    static_assert(!begin_home_route_step(EntityType::Sheep, 5, 2));
    static_assert(!begin_home_route_step(EntityType::Sheep, 0, 3));

    auto completion = herding_completion_step(1, 0, false, 0);
    assert(completion.action == CompletionAction::None);

    completion = herding_completion_step(0, 0, true, 0);
    assert(completion.action == CompletionAction::None);

    completion = herding_completion_step(0, 0, false, 0);
    assert(completion.stage == 1);
    assert(completion.action == CompletionAction::PlayFinalLine);
    assert(completion.sound_id == 599);

    completion = herding_completion_step(0, 0, false, 1);
    assert(completion.sound_id == 600);

    completion = herding_completion_step(0, 1, false, 0);
    assert(completion.action == CompletionAction::ExitToPlayAgain);

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

    RetailEntityRecord32 distracted{};
    distracted.y = 300;
    distracted.source_top = 10;
    distracted.source_bottom = 70;
    assert(apply_scruffty_distraction(distracted, followers, 7, 123));
    assert(!followers.contains(7));
    assert(distracted.target_flag_or_timer == 200);
    assert(distracted.target_x == 409);
    assert(distracted.target_y == 330);
    assert(!apply_scruffty_distraction(distracted, followers, 7, 123));

    assert(followers.add(9));
    RetailEntityRecord32 released{};
    assert(release_follower_for_food_change(released, followers, 9, 10, 20));
    assert(!followers.contains(9));
    assert(released.target_flag_or_timer == 200);
    assert(released.target_x == 296);
    assert(released.target_y == 470);
    assert((food_change_wander_target(399, 399) == Vec2i{685, 849}));

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

    const auto& exclusion = animal_exclusion_polygon(data);
    assert(exclusion.size() == 6);
    assert((exclusion.front() == Vec2i{30, 500}));
    assert((exclusion.back() == Vec2i{30, 850}));
    static_assert(kAnimalExclusionEscapeTarget == Vec2i{650, 486});

    const auto recovery = animal_navigation_recovery_target(data);
    assert(recovery && (*recovery == Vec2i{92, 324}));

    assert((farmer_pickles_start(data) == Vec2i{478, 471}));
}
