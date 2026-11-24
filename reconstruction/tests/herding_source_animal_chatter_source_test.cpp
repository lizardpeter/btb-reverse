#include "btb/herding_source_animal_chatter.hpp"
#include "btb/herding_source_near_retreat.hpp"
#include "btb/herding_source_ordinary_following.hpp"
#include <cmath>

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
    // Pure binary-derived near-retreat kernel, independent of the audio
    // device. Actor anchor is (-half-width,+half-height).
    h::RetailEntityRecord32 hero{}, sheep{};
    hero.type=static_cast<int>(h::EntityType::FarmerPickles);
    hero.x=500;hero.y=300;hero.source_bottom=100;
    sheep.type=static_cast<int>(h::EntityType::Sheep);
    sheep.x=580;sheep.y=360;sheep.x_float=580;sheep.y_float=360;
    sheep.source_right=40;sheep.source_bottom=40;
    sheep.movement_speed=0.9f;sheep.animation_frame=9;sheep.animation_frame_countdown=20;
    const auto retreat=h::original_herding_near_retreat(sheep,hero);
    assert(retreat && retreat->heading==90 && retreat->distance==60.f);
    assert(std::fabs(retreat->moved.x_float-582.f)<0.001f);
    assert(std::fabs(retreat->moved.y_float-360.f)<0.001f);
    assert(retreat->moved.movement_active==1 && retreat->moved.direction==2);
    assert(std::fabs(retreat->moved.movement_speed-0.5f)<0.000001f);
    assert(retreat->moved.animation_frame==6 && retreat->moved.animation_frame_countdown==100);
    sheep.x=600;sheep.x_float=600;
    assert(!h::original_herding_near_retreat(sheep,hero)); // exact 80 excluded
    sheep.x=580;sheep.x_float=580;sheep.temporary_target_timer=1;
    assert(!h::original_herding_near_retreat(sheep,hero)); // exclusive higher priority
    sheep.temporary_target_timer=0;
    // First formation slot's speed test uses POST-increment ordinal.
    hero.x=500;hero.y=300;hero.direction=2;
    sheep.x=530;sheep.y=340;sheep.x_float=530;sheep.y_float=340;
    sheep.movement_speed=0.5f;
    const auto following=h::original_herding_ordinary_follower_step(sheep,hero,0);
    assert(following && !following->holds_formation_position);
    assert(following->moved.movement_speed<0.5f);

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
