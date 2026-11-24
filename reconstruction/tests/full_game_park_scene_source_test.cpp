#include "btb/full_game_park_scene.hpp"
#include <algorithm>
#include <cassert>
#include <numeric>
using namespace btb::full_game;
namespace p=btb::park_designer;
static p::SaveData initial() {
    p::SaveData s{};
    for(auto& o:s.objects)o.object_code=-1;
    s.state(p::TrailingStateIndex::NextDecorateRecord)=303;
    s.state(p::TrailingStateIndex::BandstandSelectedRecord)=-1;
    s.state(p::TrailingStateIndex::PondSpecialRecord)=-1;
    s.state(p::TrailingStateIndex::PondSelectedPrimary)=-1;
    return s;
}
static void set(p::SaveData& s,int i,int code,int cat,int variant,int x,int y,int layer) {
    auto& o=s.objects[i];
    o.object_code=code;o.enabled_or_active=1;o.record_index=i;o.bound_category=cat;
    o.bound_variant=variant;o.left=x;o.top=y;o.right=x+100;o.bottom=y+100;o.sort_layer=layer;
}
int main() {
    auto s=initial();
    RetailParkScenePlan plan;std::string error;
    assert(plan_retail_park_scene(s,plan,error)&&plan.draws.size()==2);
    assert(plan.draws[0].source_asset==p::kRetailParkBackground.summer);
    assert(!plan.draws[0].color_keyed&&plan.draws[1].color_keyed);
    set(s,300,400,1,0,100,100,201); // tree feet depth=257
    set(s,301,400,0,1,200,200,201); // bush feet depth=234
    set(s,302,400,2,4,10,50,201); // bench depth=120
    assert(plan_retail_park_scene(s,plan,error));
    assert(plan.draws.size()==5);
    assert(plan.draws[1].source_asset==p::kRetailDecorationSheets[2].summer);
    assert(plan.draws[2].source_asset==p::kRetailDecorationSheets[0].summer);
    assert(plan.draws[3].source_asset==p::kRetailDecorationSheets[1].summer);
    assert((plan.draws[2].source_rectangle==Rect{124,0,248,65}));
    auto sorted=plan.sorted_slots;std::sort(sorted.begin(),sorted.end());
    for(std::size_t i=0;i<sorted.size();++i)assert(sorted[i]==i);
    s.state(p::TrailingStateIndex::Season)=1;
    assert(plan_retail_park_scene(s,plan,error));
    assert(plan.draws[0].source_asset==p::kRetailParkBackground.winter);
    assert(plan.draws[3].source_asset==p::kRetailDecorationSheets[1].winter);
    set(s,0,100,0,0,20,20,0);
    set(s,1,101,0,0,20,20,198);
    set(s,2,7,1,0,25,25,199);
    s.state(p::TrailingStateIndex::PondSelectedPrimary)=0;
    s.state(p::TrailingStateIndex::PondSpecialRecord)=2;
    s.objects[2].fountain_frame_countdown=1;s.objects[2].visual_frame_or_segment=4;
    assert(plan_retail_park_scene(s,plan,error));
    assert(plan.draws.size()==11);
    assert(s.objects[2].visual_frame_or_segment==5&&s.objects[0].visual_frame_or_segment==1);
    assert((plan.draws[1].source_rectangle==Rect{0,0,198,109}));
    assert((plan.draws[6].source_rectangle==Rect{760,0,912,152}));
    // The fountain is advanced at its sorted draw position, after pond.
    assert(plan_retail_park_scene(s,plan,error));
    assert((plan.draws[1].source_rectangle==Rect{990,0,1188,109}));
    const auto old=plan.draws.size();
    s.objects[301].bound_variant=5;
    const auto frame=s.objects[2].fountain_frame_countdown;
    assert(!plan_retail_park_scene(s,plan,error));
    assert(!error.empty()&&plan.draws.size()==old&&s.objects[2].fountain_frame_countdown==frame);
}
