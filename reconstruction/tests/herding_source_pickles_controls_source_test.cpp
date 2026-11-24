#include "btb/herding_source_pickles_controls.hpp"
#include <array>
#include <cassert>
#include <cmath>
using namespace btb::herding;
int main() {
    RetailEntityRecord32 hero{};
    hero.type=static_cast<int>(EntityType::FarmerPickles);
    hero.x=100;hero.y=200;hero.x_float=100.f;hero.y_float=200.f;
    hero.source_right=100;hero.source_bottom=100;
    const std::vector<Vec2i> nav{{0,0},{1200,0},{1200,900},{0,900}};
    const std::array entities{hero};
    RetailPicklesControlState control{};
    auto first=original_pickles_control_proposal(hero,control,{200,250},0,entities,nav);
    assert(first&&first->hero.x_float==100.f&&first->hero.direction==2);
    assert(first->state.mouse_mode&&first->state.movement_latch);
    assert(first->food_probe==(Vec2i{100,200}));
    assert(finish_original_pickles_control(*first,FoodType::SheepFood));
    assert(first->hero.animation_frame==20&&first->hero.animation_frame_countdown==35);
    // Explicitly use the recorded camera from last draw for the next pointer.
    auto state=first->state;state.last_camera={0,0};
    auto second=original_pickles_control_proposal(first->hero,state,{200,250},0,entities,nav);
    assert(second&&std::abs(second->hero.x_float-101.5f)<0.001f);
    assert(second->food_probe.x==101);
    auto dead=original_pickles_control_proposal(hero,state,{180,250},0,entities,nav);
    assert(dead&&dead->hero.movement_direction_mask==0); // <=30 stops
    auto switched=original_pickles_control_proposal(hero,state,{50,250},2,entities,nav);
    assert(switched&&!switched->state.mouse_mode&&switched->hero.x_float<100.f);
    assert(finish_original_pickles_control(*switched,FoodType::None));
    assert(switched->hero.direction==2); // final mode is keyboard
    auto keys=switched->state;keys.previous_pointer={50,250};keys.last_camera={0,0};
    auto keyboard=original_pickles_control_proposal(hero,keys,{50,250},2,entities,nav);
    assert(keyboard&&keyboard->hero.x_float==101.5f);
    assert(keyboard->food_probe==(Vec2i{100,200})); // raw integer before keyboard FTOL
    auto changed=original_pickles_control_proposal(hero,keys,{51,250},2,entities,nav);
    assert(changed&&changed->state.mouse_mode&&changed->hero.x_float==101.5f);

    hero.y=100;hero.y_float=100;
    RetailEntityRecord32 trailer{};
    trailer.type=static_cast<int>(EntityType::Trailer2);
    trailer.x=213;trailer.y=160;trailer.source_right=45;trailer.source_bottom=60;
    const auto feet=original_pickles_feet_rect(hero,101.5f,100.f);
    assert(feet&&*feet==(RetailCornerRect{119,180,183,200}));
    const auto rect=original_pickles_obstacle_rect(trailer);
    assert(rect&&*rect==(RetailCornerRect{183,190,238,250}));
    const std::array blocked{hero,trailer};
    const auto collision=original_pickles_obstacle_pass(hero,101.5f,100.f,blocked);
    assert(collision&&collision->contacts==1&&collision->x==100.f&&collision->y==100.f);
    const auto clear=original_pickles_obstacle_pass(hero,99.f,100.f,blocked);
    assert(clear&&clear->contacts==0&&clear->x==99.f);
    // Scruffty and live animals do not belong to the static collision list.
    trailer.type=static_cast<int>(EntityType::Scruffty);
    const std::array dog{hero,trailer};
    const auto ignore=original_pickles_obstacle_pass(hero,101.5f,100.f,dog);
    assert(ignore&&ignore->contacts==0&&ignore->x==101.5f);
}
