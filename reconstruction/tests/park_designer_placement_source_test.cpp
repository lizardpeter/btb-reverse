#include "btb/park_designer_placement.hpp"
#include "btb/park_designer_activity_loop.hpp"
#include <cassert>

namespace p=btb::park_designer;
using S=p::TrailingStateIndex;
int main(){
    p::LoopState loop{};
    p::reset_park_designer_runtime(loop);
    auto& d=loop.save;
    btb::retail::OriginalRetailRandom rng{7};
    auto pond=p::commit_retail_pond(d,100,200,0,{},rng);
    assert(pond.status==p::PlacementStatus::Placed&&pond.slot==0&&pond.scroll_animation==-61);
    assert(d.objects[0].object_code==100&&d.objects[0].sprite_width==640&&d.objects[0].sprite_height==480);
    assert(d.objects[1].object_code==101&&d.objects[1].sort_layer==200);
    assert(d.state(S::PondPalettePage)==1&&d.state(S::NextPondPrimaryRecord)==2);
    const auto before=rng.state();
    auto wrong=p::commit_retail_pond(d,500,200,1,{},rng);
    assert(wrong.status==p::PlacementStatus::Blocked&&wrong.voice==280&&rng.state()==before);
    auto fountain=p::commit_retail_pond(d,100,100,1,{},rng);
    assert(fountain.status==p::PlacementStatus::Placed&&fountain.slot==2);
    assert(d.objects[2].left==160&&d.objects[2].top==145&&d.objects[2].object_code==7);
    assert(d.state(S::PondPalettePage)==2&&d.state(S::PondSpecialRecord)==2);
    auto rejected=p::commit_retail_pond(d,500,200,1,{},rng);
    assert(rejected.status==p::PlacementStatus::Blocked&&rejected.voice==287);
    auto accessory=p::commit_retail_pond(d,140,265,0,{},rng);
    assert(accessory.status==p::PlacementStatus::Placed&&accessory.slot==3);
    assert(d.objects[3].object_code==1&&d.objects[3].bound_category==2);
    auto deletion=p::delete_retail_park_object(d,2,rng);
    assert(deletion.status==p::PlacementStatus::Placed);
    assert(d.objects[2].object_code==-1&&d.objects[3].object_code==-1);
    assert(d.objects[0].object_code==100&&d.state(S::PondPaletteMaxPage)==1);
    assert(p::delete_retail_park_object(d,1,rng).status==p::PlacementStatus::Invalid);
    assert(p::delete_retail_park_object(d,0,rng).voice==245);
    assert(d.objects[0].object_code==-1&&d.objects[1].object_code==-1);
    p::reset_park_designer_runtime(loop);
    d.state(S::ActiveEditorMode)=1;
    auto base=p::commit_retail_bandstand(d,200,200,2,{},rng);
    assert(base.status==p::PlacementStatus::Placed&&base.slot==200&&d.state(S::BandstandPalettePage)==1);
    assert(d.objects[200].sprite_width==127&&d.objects[200].sprite_height==72);
    for(const auto& anchor:p::kRetailBandstandPillarAnchors){
        auto pillar=p::commit_retail_bandstand(d,200+anchor.x,200+anchor.y-64,0,{},rng);
        assert(pillar.status==p::PlacementStatus::Placed);
    }
    assert(d.state(S::PondLinkedCount)==4&&d.state(S::BandstandPalettePage)==2);
    auto roof=p::commit_retail_bandstand(d,198,113,3,{},rng);
    assert(roof.status==p::PlacementStatus::Placed&&roof.slot==206); // source skips slot 205
    assert(d.state(S::NextBandstandRecord)==207&&d.objects[205].object_code==-1);
    assert(d.objects[206].sprite_width==127&&d.objects[206].sprite_height==81);
    assert(p::delete_retail_park_object(d,201,rng).status==p::PlacementStatus::Placed);
    assert(d.objects[206].object_code==-1&&d.state(S::PondLinkedCount)==3&&d.state(S::BandstandPaletteMaxPage)==1);
    assert(p::delete_retail_park_object(d,200,rng).voice==244);
    for(int i=200;i<207;++i)assert(d.objects[i].object_code==-1);
}
