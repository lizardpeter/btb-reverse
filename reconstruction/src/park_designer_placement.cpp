#include "btb/park_designer_placement.hpp"
#include "btb/retail_point_in_polygon.hpp"

#include <array>
#include <cmath>
#include <limits>
#include <vector>
namespace btb::park_designer {
namespace {
using State=TrailingStateIndex;
constexpr U32 kNoGeometry=std::numeric_limits<U32>::max();
constexpr std::array<State,4> kLinks{{State::PondLinkedRecord0,State::PondLinkedRecord1,
                                     State::PondLinkedRecord2,State::PondLinkedRecord3}};
bool valid_xy(I32 x,I32 y){return x>=-1000&&x<=2000&&y>=-1000&&y<=2000;}
bool valid_variant(I32 n){return n>=0&&n<5;}
bool valid_slot(I32 n,I32 lo,I32 hi){return n>=lo&&n<hi;}
bool within_rect(I32 x,I32 y,Rect r){return r.left<x&&x<r.right&&r.top<y&&y<r.bottom;}
I32 integer_distance(I32 x,I32 y,I32 a,I32 b) {
    return static_cast<I32>(std::hypot(static_cast<double>(x)-a,static_cast<double>(y)-b));
}
void set_geometry(RetailObjectRecord32& o,I32 slot,I32 code,I32 x,I32 y,Vec2i size,I32 layer) {
    o.left=x;o.top=y;o.right=x+size.x;o.bottom=y+size.y;
    o.object_code=code;o.object_subcode=0;o.enabled_or_active=1;
    o.sprite_width=size.x;o.sprite_height=size.y;o.sort_layer=layer;o.record_index=slot;
}
void reset_animation(RetailObjectRecord32& o) {
    o.visual_frame_or_segment=0;o.fountain_phase=0;o.fountain_frame_countdown=9;
}
void clear_linked_bandstand(SaveData& d) {
    for(auto key:kLinks) {
        auto& link=d.state(key);
        if(valid_slot(link,200,300))d.objects[link].object_code=-1;
        link=-1;
    }
    d.state(State::PondLinkedCount)=0;
    auto& roof=d.state(State::SelectedObjectRecord);
    if(valid_slot(roof,200,300))d.objects[roof].object_code=-1;
    roof=-1;
}
void clear_pond(SaveData& d) {
    clear_pond_primary_objects(d,PondClearSelector::AllPrimary);
    clear_pond_primary_objects(d,PondClearSelector::ObjectCode7);
    clear_pond_primary_objects(d,PondClearSelector::Non7CodeBelow100);
    d.state(State::PondSelectedPrimary)=-1;
    d.state(State::PondSpecialRecord)=-1;
}
void clear_fountain_and_accessories(SaveData& d) {
    clear_pond_primary_objects(d,PondClearSelector::ObjectCode7);
    clear_pond_primary_objects(d,PondClearSelector::Non7CodeBelow100);
    d.state(State::PondSpecialRecord)=-1;
}
ParkPlacement voice(PlacementStatus status,I32 id,I32 context=99) {
    return {status,-1,id,context};
}
}
ParkPlacement commit_retail_pond(SaveData& d,I32 x,I32 y,I32 variant,
    std::optional<I32> moving,retail::OriginalRetailRandom& rng) {
    if(!valid_xy(x,y)||!valid_variant(variant)||d.state(State::ActiveEditorMode)!=0)return {};
    auto& page=d.state(State::PondPalettePage);
    auto& next=d.state(State::NextPondPrimaryRecord);
    if(page<0||page>2||next<0||next>100)return {};
    if(moving&&(!valid_slot(*moving,0,100)||d.objects[*moving].object_code<1||
        d.objects[*moving].object_code>5||page!=2))return {};
    if(page==0) {
        if(next>=99)return voice(PlacementStatus::Full,241);
        // Source performs an ephemeral code-100 geometry write to the
        // next primary slot, sets it back to -1, and checks overlap with
        // flag 1. This can invalidate decor even on a rejected placement.
        const auto dim=kRetailPondCandidateDimensions[variant];
        auto& probe=d.objects[next];
        if(probe.object_code!=-1)return {};
        set_geometry(probe,next,100,x,y,kRetailPrimarySpriteDimensions[0],0);
        probe.right=x+dim.x;probe.bottom=y+dim.y;probe.sort_layer=198;probe.object_code=-1;
        const auto collision=evaluate_placement(d,x,y,dim.x,dim.y,1,next,kRetailStoredCollisionFootprints);
        if(collision.invalid_geometry)return {};
        if(!collision.permitted)return voice(PlacementStatus::Blocked,241);
        clear_pond(d);
        const I32 slot=next++;
        auto& primary=d.objects[slot];
        set_geometry(primary,slot,100,x,y,kRetailPrimarySpriteDimensions[0],0);
        primary.bound_category=0;primary.bound_variant=variant;reset_animation(primary);
        d.state(State::PondSelectedPrimary)=slot;
        d.state(State::PondShapeVariant)=variant;
        const I32 id=275+rng.next_rand()%2;
        auto& auxiliary=d.objects[next];
        set_geometry(auxiliary,next,101,x,y,kRetailPrimarySpriteDimensions[0],200);
        auxiliary.surface_ptr32=kNoGeometry;++next;
        d.state(State::PondDragAnchorMouseX)=x;d.state(State::PondDragAnchorMouseY)=y;
        page=1;d.state(State::PondPaletteMaxPage)=1;
        return {PlacementStatus::Placed,slot,id,1,-61};
    }
    const auto primary=d.state(State::PondSelectedPrimary);
    const auto shape=d.state(State::PondShapeVariant);
    if(!valid_slot(primary,0,100)||!valid_variant(shape)||d.objects[primary].object_code!=100)return {};
    const auto anchor_x=d.state(State::PondDragAnchorMouseX);
    const auto anchor_y=d.state(State::PondDragAnchorMouseY);
    if(!valid_xy(anchor_x,anchor_y))return {};
    if(page==1) {
        const auto dim=kRetailFountainHitDimensions[shape];
        if(!within_rect(x,y,{anchor_x-64,anchor_y-128,anchor_x-64+dim.x,anchor_y-128+dim.y}))
            return voice(PlacementStatus::Blocked,280);
    } else {
        const I32 code=moving?d.objects[*moving].object_code:variant+1;
        if(code<1||code>5)return {};
        std::vector<btb::herding::Vec2i> interior;
        for(std::size_t i=0;i<kRetailPondInteriorCounts[shape];++i) {
            const auto point=kRetailPondInterior[shape][i];
            interior.push_back({anchor_x+point.x,anchor_y+point.y});
        }
        const auto r=kRetailAccessoryCornerOffsets[code];
        for(const auto offset:std::array<Vec2i,4>{{{r.left,r.top},{r.left,r.bottom},{r.right,r.top},{r.right,r.bottom}}}) {
            const auto status=retail_geometry::original_point_in_polygon_status(interior,x+offset.x,y+offset.y);
            if(!status)return {};
            if(*status!=0)return voice(PlacementStatus::Blocked,287);
        }
    }
    if(!moving&&next>=88)return voice(PlacementStatus::Full,241,1);
    const I32 slot=moving?*moving:next++;
    auto& o=d.objects[slot];
    if(page==1) {
        clear_fountain_and_accessories(d);
        const auto& base=d.objects[primary];
        const auto pos=kRetailFountainAnchors[base.bound_variant];
        set_geometry(o,slot,7,base.left+pos.x-62,base.top+pos.y-106,kRetailPrimarySpriteDimensions[7],199);
        o.bound_category=1;o.bound_variant=variant;o.surface_ptr32=kNoGeometry;reset_animation(o);
        d.state(State::PondSpecialRecord)=slot;
        const I32 id=278+rng.next_rand()%2;
        page=2;d.state(State::PondPaletteMaxPage)=2;
        d.objects[primary].visual_frame_or_segment=0;
        return {PlacementStatus::Placed,slot,id,1,-61};
    }
    const I32 code=moving?o.object_code:variant+1;
    if(!moving){o.bound_category=2;o.bound_variant=variant;}
    o.surface_ptr32=kNoGeometry;
    set_geometry(o,slot,code,x,y,kRetailPrimarySpriteDimensions[code],199);
    return {PlacementStatus::Placed,slot,281+rng.next_rand()%3,1};
}

ParkPlacement commit_retail_bandstand(SaveData& d,I32 x,I32 y,I32 variant,
    std::optional<I32> moving,retail::OriginalRetailRandom& rng) {
    if(!valid_xy(x,y)||!valid_variant(variant)||d.state(State::ActiveEditorMode)!=1)return {};
    auto& page=d.state(State::BandstandPalettePage);
    auto& next=d.state(State::NextBandstandRecord);
    if(page<0||page>2||next<200||next>300)return {};
    I32 category=page;
    if(moving) {
        if(!valid_slot(*moving,200,300)||d.objects[*moving].object_code!=300)return {};
        category=d.objects[*moving].bound_category;variant=d.objects[*moving].bound_variant;
    }
    if(category<0||category>2||!valid_variant(variant))return {};
    // Source allocates one record before validating the placement,
    // and allocates a SECOND one on the new-roof path. Limit at the
    // recovered family boundary rather than corrupt adjacent records.
    if(!moving&&next>=300)return voice(PlacementStatus::Full,241,1);
    if(!moving&&category==2&&next>=299)return voice(PlacementStatus::Full,241,1);
    I32 slot=moving?*moving:next++;
    auto& base_slot=d.state(State::BandstandSelectedRecord);
    const auto dim=kRetailBandstandDimensions[category];
    if(category==0) {
        auto& o=d.objects[slot];o.bound_category=0;o.bound_variant=variant;
        const auto overlap=evaluate_placement(d,x,y,dim.x,dim.y,1,-1,kRetailStoredCollisionFootprints);
        if(overlap.invalid_geometry)return {};
        if(!overlap.permitted||base_slot!=-1)return voice(PlacementStatus::Blocked,241);
        set_geometry(o,slot,300,x,y,dim,201);o.surface_ptr32=kNoGeometry;reset_animation(o);
        base_slot=slot;clear_linked_bandstand(d);
        const I32 id=202+rng.next_rand()%2;
        I32 scroll=0;
        if(page==0){page=1;d.state(State::BandstandPaletteMaxPage)=1;scroll=-61;}
        return {PlacementStatus::Placed,slot,id,1,scroll};
    }
    if(!valid_slot(base_slot,200,300)||d.objects[base_slot].object_code!=300||
       d.objects[base_slot].bound_category!=0)return {};
    const auto& base=d.objects[base_slot];
    if(category==1) {
        std::size_t i=0;I32 target_x=0,target_y=0;
        for(;i<kLinks.size();++i) {
            const auto anchor=kRetailBandstandPillarAnchors[i];
            target_x=base.left+anchor.x;target_y=base.top+anchor.y-64;
            if(integer_distance(x,y,target_x,target_y)<30)break;
        }
        if(i==kLinks.size())return voice(PlacementStatus::Blocked,207);
        auto& link=d.state(kLinks[i]);
        if(link==-1)++d.state(State::PondLinkedCount);
        else if(valid_slot(link,200,300))d.objects[link].object_code=-1;
        else return {};
        link=slot;
        auto& o=d.objects[slot];set_geometry(o,slot,300,target_x,target_y,dim,201);
        o.surface_ptr32=kNoGeometry;o.bound_category=1;o.bound_variant=variant;reset_animation(o);
        if(page==1&&d.state(State::PondLinkedCount)>3) {
            page=2;d.state(State::BandstandPaletteMaxPage)=2;
            return {PlacementStatus::Placed,slot,d.state(State::SelectedObjectRecord)==-1?205:-1,1,-61};
        }
        return {PlacementStatus::Placed,slot};
    }
    if(!moving)slot=next++;
    const I32 target_x=base.left-2,target_y=base.top-87;
    if(integer_distance(x,y,target_x,target_y)>49)return voice(PlacementStatus::Blocked,210);
    auto& roof=d.state(State::SelectedObjectRecord);
    if(valid_slot(roof,200,300))d.objects[roof].object_code=-1;
    else if(roof!=-1)return {};
    roof=slot;
    auto& o=d.objects[slot];set_geometry(o,slot,300,target_x,target_y,dim,201);
    o.surface_ptr32=kNoGeometry;o.bound_category=2;o.bound_variant=variant;reset_animation(o);
    return {PlacementStatus::Placed,slot,208+rng.next_rand()%2,1};
}

bool begin_retail_park_drag(SaveData& d,I32 slot) noexcept {
    if(!valid_slot(slot,0,400))return false;
    auto& o=d.objects[slot];
    if(o.object_code==-1||o.enabled_or_active!=1)return false;
    if(slot>=200&&slot<300&&o.bound_category==1){
        d.state(State::BandstandPaletteMaxPage)=1;
        clear_bandstand_objects(d,BandstandClearSelector::Category2Only);
        auto& roof=d.state(State::SelectedObjectRecord);
        if(valid_slot(roof,200,300))d.objects[roof].object_code=-1;
        roof=-1;
        for(auto key:kLinks)if(d.state(key)==slot){
            d.state(key)=-1;--d.state(State::PondLinkedCount);
        }
    }
    o.enabled_or_active=2;
    return true;
}
void finish_retail_park_drag(SaveData& d,I32 slot) noexcept {
    if(!valid_slot(slot,0,400))return;
    auto& o=d.objects[slot];
    // Original 0040E3E0 deliberately excludes a disconnected pillar
    // in slots 201..299. It remains hidden when returned to the toolbar.
    if(o.object_code!=-1&&(slot<201||slot>=300||o.bound_category!=1))
        o.enabled_or_active=1;
}

ParkPlacement delete_retail_park_object(SaveData& d,I32 slot,retail::OriginalRetailRandom& rng) {
    if(!valid_slot(slot,0,400))return {};
    auto& o=d.objects[slot];
    if(o.object_code==-1||(o.object_code>=101&&o.object_code<=199))return {};
    I32 id=-1;
    if(slot<100&&o.object_code==100) {
        clear_pond(d);d.state(State::PondPaletteMaxPage)=0;id=245;
    } else if(slot<100&&o.object_code==7) {
        id=242+rng.next_rand()%2;clear_fountain_and_accessories(d);
        d.state(State::PondPaletteMaxPage)=1;
    } else if(slot>=200&&slot<300) {
        if(o.bound_category==0) {
            clear_bandstand_objects(d,BandstandClearSelector::Categories0To2);
            d.state(State::BandstandSelectedRecord)=-1;clear_linked_bandstand(d);
            d.state(State::BandstandPaletteMaxPage)=0;id=244;
        } else if(o.bound_category==1) {
            id=242+rng.next_rand()%2;
            d.state(State::BandstandPaletteMaxPage)=1;
            clear_bandstand_objects(d,BandstandClearSelector::Category2Only);
            auto& roof=d.state(State::SelectedObjectRecord);
            if(valid_slot(roof,200,300))d.objects[roof].object_code=-1;
            roof=-1;
            for(auto key:kLinks)if(d.state(key)==slot){
                d.state(key)=-1;--d.state(State::PondLinkedCount);
            }
        } else {
            d.state(State::SelectedObjectRecord)=-1;id=242+rng.next_rand()%2;
        }
    } else id=242+rng.next_rand()%2;
    o.object_code=-1;
    return {PlacementStatus::Placed,slot,id,1};
}
}
