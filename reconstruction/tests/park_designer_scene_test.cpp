#include "btb/park_designer_scene.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <limits>
#include <vector>

using namespace btb::park_designer;

namespace {
void clear(SaveData& state) {
    for(auto& o:state.objects) {
        o={};
        o.object_code=-1;
        o.surface_ptr32=std::numeric_limits<U32>::max();
    }
}
}
int main() {
    // 0040E2B0: compact both disjoint families; neither pond auxiliary
    // [100,200) nor bandstands [200,300) may be relocated.
    SaveData state{};
    clear(state);
    state.state(TrailingStateIndex::PondSelectedPrimary)=17;
    state.state(TrailingStateIndex::PondSpecialRecord)=7;
    state.state(TrailingStateIndex::NextPondPrimaryRecord)=99;
    state.state(TrailingStateIndex::NextDecorateRecord)=399;
    state.objects[1].object_code=100;
    state.objects[1].record_index=1;
    state.objects[3].object_code=7;
    state.objects[3].record_index=3;
    state.objects[6].object_code=42;
    state.objects[6].record_index=6;
    state.objects[100].object_code=100;
    state.objects[200].object_code=200;
    state.objects[301].object_code=300;
    state.objects[301].record_index=301;
    state.objects[303].object_code=401;
    state.objects[303].record_index=303;
    const auto compacted=compact_record_families(state);
    assert(compacted.moved_primary==3&&compacted.moved_decoration==2);
    assert(state.objects[0].object_code==100&&state.objects[0].record_index==0);
    assert(state.objects[1].object_code==7&&state.objects[1].record_index==1);
    assert(state.objects[2].object_code==42&&state.objects[2].record_index==2);
    assert(state.objects[3].object_code==-1&&state.objects[6].object_code==-1);
    assert(state.objects[100].object_code==100);
    assert(state.objects[200].object_code==200);
    assert(state.objects[300].object_code==300);
    assert(state.objects[301].object_code==401);
    assert(state.objects[301].record_index==301);
    assert(state.objects[303].object_code==-1);
    assert(state.state(TrailingStateIndex::PondSelectedPrimary)==0);
    assert(state.state(TrailingStateIndex::PondSpecialRecord)==1);
    assert(state.state(TrailingStateIndex::NextPondPrimaryRecord)==3);
    assert(state.state(TrailingStateIndex::NextDecorateRecord)==302);
    const auto again=compact_record_families(state);
    assert(again.moved_primary==0&&again.moved_decoration==0);
    assert(state.state(TrailingStateIndex::NextPondPrimaryRecord)==3);

    // 0040D2E0: strict outer rectangle then original PointInPolygon
    // 0==inside predicate; highest sorted entry wins, not lowest slot.
    clear(state);
    BoundAreas polygons{};
    polygons[0][0][0].vertices={
        {0,0},{20,0},{20,20},{0,20}
    };
    for(int i=0;i<2;++i) {
        auto& o=state.objects[static_cast<std::size_t>(i)];
        o.object_code=23;
        o.record_index=i;
        o.bound_category=0;
        o.bound_variant=0;
        o.left=10;o.top=10;o.right=50;o.bottom=50;
    }
    const std::array<std::size_t,2> sorted{0,1};
    auto hit=find_topmost_object(state,polygons,sorted,15,15,0,-1,1,0);
    assert(hit&&(hit->original_sorted_position==1&&hit->record_slot==1));
    hit=find_topmost_object(state,polygons,sorted,15,15,0,1,1,0);
    assert(hit&&hit->record_slot==0);
    hit=find_topmost_object(state,polygons,sorted,15,15,0,-1,0,0);
    assert(!hit);
    hit=find_topmost_object(state,polygons,sorted,15,15,1,-1,1,0);
    assert(!hit); // mode 1 only accepts object codes 101..199.
    hit=find_topmost_object(state,polygons,sorted,48,48,0,-1,1,0);
    assert(!hit); // within coarse rectangle, outside authored polygon.
    state.objects[1].object_code=7;
    hit=find_topmost_object(state,polygons,sorted,15,15,0,-1,1,0);
    assert(hit&&hit->record_slot==0);
    state.objects[1].object_code=105;
    state.objects[1].record_index=150;
    polygons[1][0][0].vertices=polygons[0][0][0].vertices;
    hit=find_topmost_object(state,polygons,sorted,15,15,1,-1,1,0);
    assert(hit&&hit->record_slot==1);

    // 0040DFF0: real placement logic, original nine-sample collision
    // and mandatory code-100 pond footprint.
    clear(state);
    state.objects[100].object_code=100;
    state.objects[100].left=300;
    state.objects[100].top=300;
    state.objects[100].bound_variant=0;
    auto placed=evaluate_placement(state,350,350,10,10,0,-1,{});
    assert(!placed.permitted&&!placed.invalid_geometry);
    placed=evaluate_placement(state,0,0,20,20,0,-1,{});
    assert(placed.permitted);
    placed=evaluate_placement(state,350,350,10,10,0,100,{});
    assert(placed.permitted); // exclude current editing slot
    state.objects[100].bound_variant=5;
    placed=evaluate_placement(state,350,350,10,10,0,-1,{});
    assert(!placed.permitted&&placed.invalid_geometry);

    // Code 300 in the 200..299 family uses +0x14/+0x18 alternate
    // origin for its surface rectangle rather than saved x/y.
    clear(state);
    auto& band=state.objects[200];
    band.object_code=300;
    band.bound_category=0;
    band.surface_ptr32=0;
    band.left=1;band.top=1;
    band.unused_persisted_14=50;band.unused_persisted_18=50;
    const std::array<Rect,1> offsets{{{0,0,20,20}}};
    placed=evaluate_placement(state,55,55,5,5,0,-1,offsets);
    assert(!placed.permitted&&!placed.invalid_geometry);
    placed=evaluate_placement(state,10,10,5,5,0,-1,offsets);
    assert(placed.permitted);
    placed=evaluate_placement(state,55,55,5,5,0,200,offsets);
    assert(placed.permitted);

    // Overlapping code>399 is removed during first conditional scan.
    // That original side effect is NOT itself placement rejection.
    clear(state);
    auto& large=state.objects[310];
    large.object_code=400;
    large.left=0;large.top=0;large.right=40;large.bottom=40;
    placed=evaluate_placement(state,15,15,5,5,0,-1,{});
    assert(placed.permitted&&large.object_code==400);
    placed=evaluate_placement(state,15,15,5,5,1,-1,{});
    assert(placed.permitted&&large.object_code==-1);
    assert((placed.invalidated_large_object_slots==std::vector<I32>{310}));

    // 0040D470: sentinel-bound hover, strict edge rules, one voice on
    // transition (7=>236, 8=>235, 9=>237, 10=>238, 11=>239).
    std::array<Rect,13> toolbar{};
    for(auto& r:toolbar)r={500,500,550,550};
    toolbar[7]={10,10,20,20};
    toolbar[11]={30,30,40,40};
    toolbar[12].left=-1;
    auto hover=update_toolbar_hover(toolbar,15,15,-1,EditorMode::Pond);
    assert(hover.next_hover_index==7);
    assert(hover.new_voice_id&&*hover.new_voice_id==236);
    assert(hover.highlighted_region&&*hover.highlighted_region==7);
    hover=update_toolbar_hover(toolbar,15,15,7,EditorMode::Pond);
    assert(!hover.new_voice_id);
    hover=update_toolbar_hover(toolbar,35,35,7,EditorMode::View);
    assert(hover.next_hover_index==11&&!hover.new_voice_id);
    assert(!hover.highlighted_region);
    hover=update_toolbar_hover(toolbar,35,35,7,EditorMode::Decorate);
    assert(hover.new_voice_id&&*hover.new_voice_id==239);
    hover=update_toolbar_hover(toolbar,10,15,7,EditorMode::Pond);
    assert(hover.next_hover_index==-1&&!hover.new_voice_id);

    // 0040D1D0: 15 resource assignments, 5 palette slots, native
    // season-specific background and 9 resource-family decisions.
    SeasonResources seasons{};
    for(std::size_t i=0;i<15;++i) {
        seasons.seasonal_triplets[0][i]=static_cast<U32>(100+i);
        seasons.seasonal_triplets[1][i]=static_cast<U32>(200+i);
    }
    for(std::size_t i=0;i<5;++i) {
        seasons.seasonal_palette[0][i]=static_cast<U32>(10+i);
        seasons.seasonal_palette[1][i]=static_cast<U32>(20+i);
    }
    seasons.background_surfaces={701,702};
    seasons.seasonal_scene_background={801,802};
    seasons.placements[0]={1,2,-1};
    seasons.seasonal_objects[0]={201,202};
    seasons.placements[1]={0,0,101};
    seasons.seasonal_objects[1]={301,302};
    assert(apply_season(seasons,Season::Winter));
    assert(seasons.current_triplets[4]==204);
    assert(seasons.current_palette[4]==24);
    assert(seasons.current_background==702);
    assert(seasons.current_scene_background==802);
    assert(seasons.sub_sprite_surfaces[6][4]==202);
    assert(seasons.current_object_surfaces[1]==302);
    assert(apply_season(seasons,Season::Summer));
    assert(seasons.current_triplets[4]==104);
    assert(seasons.sub_sprite_surfaces[6][4]==201);
    assert(seasons.current_object_surfaces[1]==301);
    const auto last_background=seasons.current_background;
    assert(!apply_season(seasons,static_cast<Season>(6)));
    assert(seasons.current_background==last_background);
    seasons.placements[0].shape=6;
    assert(!apply_season(seasons,Season::Winter));
    assert(seasons.current_background==last_background);

    // 0040D900: source-backed fountain tick + source rectangle plan.
    clear(state);
    state.state(TrailingStateIndex::PondSpecialRecord)=4;
    state.state(TrailingStateIndex::PondSelectedPrimary)=2;
    state.objects[4].object_code=7;
    state.objects[4].left=123;state.objects[4].top=234;
    state.objects[4].bound_variant=1;
    state.objects[4].visual_frame_or_segment=4;
    state.objects[4].fountain_phase=0;
    state.objects[4].fountain_frame_countdown=1;
    state.objects[2].visual_frame_or_segment=1;
    auto fountain=advance_fountain(state);
    assert(fountain&&fountain->slot==4);
    assert(fountain->animation.frame_advanced);
    assert(state.objects[4].visual_frame_or_segment==5);
    assert(state.objects[2].visual_frame_or_segment==2);
    assert(fountain->source.left==18*152);
    assert(fountain->source.right==19*152);
    state.state(TrailingStateIndex::PondSpecialRecord)=-1;
    assert(!advance_fountain(state));

    // 0040DA60 and 0040DB50: draw plan preserves incoming native sort order,
    // original special pond surface callbacks, and background/overlay framing.
    clear(state);
    state.objects[0].object_code=42;
    state.objects[0].enabled_or_active=1;
    state.objects[0].record_index=0;
    state.objects[1].object_code=100;
    state.objects[1].enabled_or_active=1;
    state.objects[1].record_index=101;
    state.objects[2].object_code=105;
    state.objects[2].enabled_or_active=1;
    state.objects[2].record_index=102;
    state.objects[3].object_code=300;
    state.objects[3].enabled_or_active=1;
    state.objects[3].record_index=300;
    state.objects[3].bound_category=2;
    state.objects[3].bound_variant=3;
    state.objects[4].object_code=7;
    state.objects[4].enabled_or_active=1;
    const std::array<std::size_t,6> order{3,2,0,4,1,5};
    const auto draws=plan_sorted_object_draws(state,order);
    assert(draws.size()==5);
    assert(draws[0].kind==DrawKind::Object&&draws[0].sprite_index==13);
    assert(draws[1].kind==DrawKind::SegmentedPondSurface);
    assert(draws[1].sprite_index==1); // sorted index, not record slot
    assert(draws[2].kind==DrawKind::Object&&draws[2].sprite_index==42);
    assert(draws[3].kind==DrawKind::Fountain);
    assert(draws[4].kind==DrawKind::PondSurface&&draws[4].sprite_index==4);
    const auto frame=plan_activity_draw(state,order,801,901);
    assert(frame.size()==7);
    assert(frame.front().kind==DrawKind::Background&&frame.front().handle32==801);
    assert(frame.back().kind==DrawKind::Overlay&&frame.back().handle32==901);
    assert(frame[1].slot==3);
}
