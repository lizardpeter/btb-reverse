#include "btb/herding_source_pair_separation.hpp"
#include <array>
#include <cassert>
#include <cmath>
using namespace btb::herding;

static RetailEntityRecord32 animal(int x,int y,EntityType type=EntityType::Sheep) {
    RetailEntityRecord32 a{};
    a.type=static_cast<int>(type);
    a.x=x;a.y=y;a.x_float=static_cast<float>(x);a.y_float=static_cast<float>(y);
    a.source_right=40;a.source_bottom=40;a.movement_speed=2.0f;
    a.previous_x=77;a.previous_y=88;
    return a;
}
int main() {
    auto a=animal(110,500);
    auto b=animal(100,500,EntityType::Duck);
    b.behavior_state=99;b.movement_active=1;
    const auto pair=original_herding_pair_separation(a,b);
    assert(pair&&pair->separated);
    assert(std::abs(pair->current.x_float-110.9f)<0.0001f);
    assert(std::abs(pair->later.x_float-99.1f)<0.0001f);
    assert(pair->current.direction==2&&pair->later.direction==6);
    assert(pair->current.movement_speed==0&&pair->later.movement_speed==0);
    assert(pair->current.movement_active==1&&pair->later.movement_active==1);
    assert(pair->later.behavior_state==99&&pair->later.previous_x==77);
    const auto boundary=original_herding_pair_separation(animal(123,500),b);
    assert(boundary&&!boundary->separated);
    // Signed sprite anchor, not center or top-left.
    auto wide=animal(140,500);wide.source_right=100;
    assert(original_herding_pair_separation(wide,b)->separated);
    // A scan beginning at index 1 never mutates index 0. It
    // continues past the first pair even after the latch is set.
    std::array records{animal(110,500),a,b,animal(100,500),animal(110,500,EntityType::Scruffty)};
    assert(original_herding_pair_scan(records,1,false)==2);
    assert(records[0].x_float==110&&records[4].x_float==110);
    assert(records[2].x_float<100&&records[3].x_float<100);
    assert(original_herding_pair_scan(records,1,false)==0);
    records[1].movement_active=0;
    assert(original_herding_pair_scan(records,1,true)==0);
}
