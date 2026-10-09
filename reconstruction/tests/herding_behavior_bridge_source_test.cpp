#include "btb/herding_behavior_bridge.hpp"

#include <cassert>
#include <vector>

namespace h=btb::herding;

std::vector<h::RetailEntityRecord32> native_population(
    int difficulty,int behavior_state) {

    std::vector<h::RetailEntityRecord32> entities;
    h::RetailEntityRecord32 hero{};
    hero.entity_id=0;
    hero.type=static_cast<int>(h::EntityType::FarmerPickles);
    entities.push_back(hero);
    for (const auto species : {
        h::EntityType::Sheep,h::EntityType::Rabbit,h::EntityType::Duck}) {
        for (int i=0;i<difficulty+3;++i) {
            h::RetailEntityRecord32 animal{};
            animal.entity_id=static_cast<int>(entities.size());
            animal.type=static_cast<int>(species);
            animal.behavior_state=behavior_state;
            animal.x=500;
            animal.y=300;
            entities.push_back(animal);
        }
    }
    for (const auto type : {
        h::EntityType::GateLeft,h::EntityType::GateRight}) {
        h::RetailEntityRecord32 gate{};
        gate.entity_id=static_cast<int>(entities.size());
        gate.type=static_cast<int>(type);
        gate.animation_frame_countdown=-1;
        entities.push_back(gate);
    }
    return entities;
}

int main() {
    h::HerdingRecoveredBehavior invalid(-1,native_population(0,0));
    assert(!invalid.valid());

    h::HerdingRecoveredBehavior easy(0,native_population(0,0));
    assert(easy.valid());
    assert(easy.undelivered()==9);
    assert(easy.food().selected==h::FoodType::None);

    const std::vector<h::WanderRandom> random(
        easy.entities().size(),{11,22});
    const auto sheep_bag=easy.select_food(
        h::FoodType::SheepFood,240,414,1,random);
    assert(sheep_bag.accepted);
    assert(sheep_bag.sound_id==584); // PC_PIC_04
    assert(easy.food().selected==h::FoodType::SheepFood);
    assert(easy.food().bag_states[2]==-1);
    assert(!easy.select_food(
        h::FoodType::SheepFood,240,414,1,random).accepted);

    assert(!easy.join_follower(1,0,false).accepted); // no proximity
    assert(!easy.join_follower(4,0,true).accepted);  // wrong food
    auto joined=easy.join_follower(1,0,true);
    assert(joined.accepted);
    assert(joined.sound_id==589); // PC_PIC_09
    assert(easy.followers().contains(1));
    assert(!easy.join_follower(1,1,true).accepted); // no duplication

    // A corrupt RNG entry cannot release only SOME followers before
    // failing, nor can it change selected food.
    auto invalid_rng=random;
    invalid_rng[1]={400,10};
    assert(!easy.select_food(
        h::FoodType::RabbitFood,365,354,0,invalid_rng).accepted);
    assert(easy.followers().contains(1));
    assert(easy.food().selected==h::FoodType::SheepFood);

    auto changed=easy.select_food(
        h::FoodType::RabbitFood,365,354,0,random);
    assert(changed.accepted);
    assert(changed.sound_id==610); // PC_TR_03
    assert(changed.released_followers==1);
    assert(easy.followers().count()==0);
    assert(easy.entities()[1].temporary_target_timer==200);
    assert(easy.entities()[1].target_x==297);
    assert(easy.entities()[1].target_y==472);

    joined=easy.join_follower(4,1,true);
    assert(joined.accepted);
    assert(joined.sound_id==588); // PC_PIC_08
    const auto scruffty=easy.scruffty_distraction(4,0,true);
    assert(scruffty.accepted);
    assert(easy.followers().count()==0);
    assert(easy.entities()[4].target_x==286);
    assert(easy.entities()[4].temporary_target_timer==200);
    assert(!easy.scruffty_distraction(4,399,true).accepted);

    // Home route counters are per species. All 3 sheep allocate
    // 10/11/12 and the last triggers the left gate; rabbits trigger
    // the right gate, while ducks do not trigger gates.
    h::HerdingRecoveredBehavior home(
        0,native_population(0,1));
    assert(home.valid());
    int deliveries=0;
    for (int species=0;species<3;++species) {
        for (int n=0;n<3;++n) {
            const std::size_t id=1+species*3+n;
            const auto begun=home.begin_home_route(id);
            assert(begun.accepted);
            assert(home.entities()[id].behavior_state==10+n);
            if (n==2) {
                assert(begun.sound_id==(596+species));
                assert(begun.gate_trigger==
                       (species==0 ? h::HomeRouteGateTrigger::LeftGate
                       : species==1 ? h::HomeRouteGateTrigger::RightGate
                                    : h::HomeRouteGateTrigger::None));
            } else {
                assert(begun.sound_id==-1);
            }
            assert(!home.begin_home_route(id).accepted);
            assert(!home.arrive_at_home_waypoint(id,false).accepted);
            auto approaching=home.arrive_at_home_waypoint(id,true);
            assert(approaching.accepted && !approaching.animal_delivered);
            assert(home.entities()[id].behavior_state==20+n);
            auto delivered=home.arrive_at_home_waypoint(id,true);
            assert(delivered.accepted && delivered.animal_delivered);
            ++deliveries;
            assert(home.undelivered()==9-deliveries);
            assert(home.entities()[id].behavior_state==99);
            assert(!home.arrive_at_home_waypoint(id,true).accepted);
        }
    }
    assert(home.undelivered()==0);
    assert(home.entities()[10].animation_frame_countdown==50);
    assert(home.entities()[11].animation_frame_countdown==50);
}
