#include "btb/herding_source_wrong_home.hpp"
#include <cassert>

int main() {
    using namespace btb::herding;
    RetailWrongHomeLatches fresh{};
    assert(!original_herding_wrong_home_step(EntityType::FarmerPickles,{0,0},fresh));
    assert(!original_herding_wrong_home_step(EntityType::Sheep,{736,183},fresh));
    auto sheep=original_herding_wrong_home_step(EntityType::Sheep,{1023,337},fresh);
    assert(sheep&&sheep->count==1&&sheep->requested_sound_ids[0]==0x3ab);
    assert(sheep->next.latched[2]);
    auto duplicate=original_herding_wrong_home_step(EntityType::Sheep,{1023,337},sheep->next);
    assert(duplicate&&duplicate->count==0);
    auto clear=original_herding_wrong_home_step(EntityType::Sheep,{100,700},duplicate->next);
    assert(clear&&!clear->next.latched[2]);
    auto duck=original_herding_wrong_home_step(EntityType::Duck,{736,183},fresh);
    assert(duck&&duck->count==2&&duck->requested_sound_ids[0]==0x3aa&&duck->requested_sound_ids[1]==0x3a9);
    assert(duck->next.latched[1]&&duck->next.latched[3]);
    auto duck_clear=original_herding_wrong_home_step(EntityType::Duck,{100,700},duck->next);
    assert(duck_clear&&duck_clear->next.latched[1]&&!duck_clear->next.latched[3]);
    // Exact boundary is outside the speech gate.
    auto boundary=original_herding_wrong_home_step(EntityType::Rabbit,{1223,337},fresh);
    assert(boundary&&boundary->count==0);
}
