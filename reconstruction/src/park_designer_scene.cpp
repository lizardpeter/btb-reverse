#include "btb/park_designer_scene.hpp"
#include "btb/original_source9.hpp"
#include "btb/retail_point_in_polygon.hpp"

#include <algorithm>
#include <limits>
#include <vector>

namespace btb::park_designer {
namespace {

[[nodiscard]] constexpr bool is_on_screen(I32 x,I32 y,Rect r) noexcept {
    return r.left<x&&x<r.right&&r.top<y&&y<r.bottom;
}
[[nodiscard]] constexpr bool is_living(const RetailObjectRecord32& o) noexcept {
    return o.object_code!=-1;
}
[[nodiscard]] constexpr Rect record_rectangle(
    const RetailObjectRecord32& o) noexcept {
    return {o.left,o.top,o.right,o.bottom};
}

[[nodiscard]] std::optional<Rect> collision_rectangle(
    const RetailObjectRecord32& o, std::span<const Rect> geometry,
    bool secondary_origin) noexcept {

    if(o.surface_ptr32==std::numeric_limits<U32>::max())
        return record_rectangle(o);
    // 0040DFF0 uses the value at +0x10 as an index in the 16-byte
    // rect-offset table; division/modulo by 5 reconstructs the same index.
    if(o.surface_ptr32>=geometry.size())return std::nullopt;
    const auto offset=geometry[o.surface_ptr32];
    const auto origin_x=secondary_origin?o.special_collision_origin_x:o.left;
    const auto origin_y=secondary_origin?o.special_collision_origin_y:o.top;
    return Rect{origin_x+offset.left,origin_y+offset.top,
                origin_x+offset.right,origin_y+offset.bottom};
}

} // namespace

CompactionResult compact_record_families(SaveData& data) noexcept {
    CompactionResult result{};
    // The two loops in 0040E2B0 only operate in 0..99 and 300..399.
    // The 100..299 pond/bandstand families are NOT compacted.
    auto compact=[&](std::size_t first,std::size_t limit,
                     TrailingStateIndex next_slot,
                     bool primary) noexcept -> std::size_t {
        std::size_t moved=0;
        I32 last_destination=-1;
        for(std::size_t i=first;i<limit;++i) {
            if(is_living(data.objects[i]))continue;
            std::size_t next=i+1;
            while(next<limit&&!is_living(data.objects[next]))++next;
            if(next==limit)break;
            data.objects[i]=data.objects[next];
            data.objects[next].object_code=-1;
            data.objects[i].record_index=static_cast<I32>(i);
            last_destination=static_cast<I32>(i);
            ++moved;
            if(primary) {
                // The source special-case updates DAT_00507A28 when
                // relocating object code 100, and DAT_005079FC for code 7.
                if(data.objects[i].object_code==100)
                    data.state(TrailingStateIndex::PondSelectedPrimary)=static_cast<I32>(i);
                if(data.objects[i].object_code==7)
                    data.state(TrailingStateIndex::PondSpecialRecord)=static_cast<I32>(i);
            }
        }
        // Original next-record globals change only when a record moved.
        if(last_destination!=-1)
            data.state(next_slot)=last_destination+1;
        return moved;
    };
    result.moved_primary=compact(
        0,100,TrailingStateIndex::NextPondPrimaryRecord,true);
    result.moved_decoration=compact(
        300,400,TrailingStateIndex::NextDecorateRecord,false);
    return result;
}

std::optional<PickHit> find_topmost_object(
    const SaveData& data,const BoundAreas& bounds,
    std::span<const std::size_t> sorted_slots,
    I32 mouse_x,I32 mouse_y,I32 picking_mode,
    I32 selected_bandstand,I32 pointer_pressed,I32 cursor_enabled) {

    if(pointer_pressed!=1&&cursor_enabled!=1)return std::nullopt;
    // Ghidra 0040D2E0 starts at the highest qsort pointer (399) and walks
    // backward. The returned integer identifies this sorted position.
    for(std::size_t reverse=sorted_slots.size();reverse>0;--reverse) {
        const std::size_t order=reverse-1,slot=sorted_slots[order];
        if(slot>=data.objects.size())continue;
        const auto& object=data.objects[slot];
        if(!is_living(object))continue;
        const auto type=object.object_code;
        if(picking_mode==0) {
            if(type==7||(type>=100&&type<=199)||
               object.record_index==selected_bandstand)continue;
        } else if(picking_mode==1) {
            if(type<101||type>199)continue;
        }
        if(!is_on_screen(mouse_x,mouse_y,record_rectangle(object)))continue;

        // Native selection maps record_index to 3 x 4 x 5 polygon slots
        // and passes local pointer coordinates to PointInPolygon.
        if(object.record_index<0||
           object.record_index>=static_cast<I32>(kObjectRecordCount)||
           object.bound_category<0||
           object.bound_category>=static_cast<I32>(kBoundCategories)||
           object.bound_variant<0||
           object.bound_variant>=static_cast<I32>(kBoundVariants))continue;
        const std::size_t family=bound_mode_for_record_index(object.record_index);
        const auto& polygon=bounds[family]
            [static_cast<std::size_t>(object.bound_category)]
            [static_cast<std::size_t>(object.bound_variant)];
        std::vector<btb::herding::Vec2i> points;
        points.reserve(polygon.vertices.size());
        for(const auto& p:polygon.vertices)
            points.push_back({p.x,p.y});
        const auto status=btb::retail_geometry::original_point_in_polygon_status(
            points,mouse_x-object.left,mouse_y-object.top);
        if(status&&*status==0) {
            return PickHit{static_cast<I32>(order),static_cast<I32>(slot)};
        }
    }
    return std::nullopt;
}

PlacementResult evaluate_placement(
    SaveData& data,I32 x,I32 y,I32 width,I32 height,
    I32 flag,I32 excluded_slot,std::span<const Rect> geometry) {

    PlacementResult result{};
    constexpr std::array<I32,10> kSpecialOffsets{{
        199,109,156,104,224,139,251,138,173,117
    }};
    for(I32 i=0;i<static_cast<I32>(data.objects.size());++i) {
        if(i==excluded_slot)continue;
        auto& record=data.objects[static_cast<std::size_t>(i)];
        if(!is_living(record))continue;
        I32 type=record.object_code;
        const bool special_decor=(type==300&&record.bound_category==0&&
                                  i>=200&&i<300);
        // First conditional collision check does not directly reject
        // placement, but can invalidate an overlapping object code>399.
        if(flag==1&&!special_decor&&(type<100||type>199)) {
            const auto rect=collision_rectangle(record,geometry,false);
            if(!rect){result.invalid_geometry=true;result.permitted=false;return result;}
            if(!btb::original_source9::park_no_sample_overlap(
                   x,y,width,height,*rect)&&type>399) {
                record.object_code=-1;
                result.invalidated_large_object_slots.push_back(i);
            }
        }
        type=record.object_code;
        // A category-0 object-code 300 in slots 200..299 is tested
        // unconditionally, using its alternate source placement origin.
        if(type==300&&record.bound_category==0&&i>=200&&i<300) {
            const auto rect=collision_rectangle(record,geometry,true);
            if(!rect){result.invalid_geometry=true;result.permitted=false;return result;}
            result.permitted=btb::original_source9::park_no_sample_overlap(
                    x,y,width,height,*rect);
        } else if(type==100) {
            const I32 variant=record.bound_variant;
            if(variant<0||variant>4){
                result.invalid_geometry=true;result.permitted=false;return result;
            }
            const auto idx=static_cast<std::size_t>(variant)*2;
            const Rect pond{record.left,record.top,
                            record.left+kSpecialOffsets[idx],
                            record.top+kSpecialOffsets[idx+1]};
            result.permitted=btb::original_source9::park_no_sample_overlap(
                    x,y,width,height,pond);
        } else {
            continue;
        }
        if(!result.permitted)return result;
    }
    return result;
}

ToolbarHover update_toolbar_hover(
    std::span<const Rect> regions,I32 x,I32 y,
    I32 previous_hover,EditorMode mode) noexcept {

    ToolbarHover result{};
    bool any=false;
    I32 last=previous_hover;
    // A native -1 left-coordinate is a sentinel; all bounds strict.
    for(std::size_t i=0;i<regions.size();++i) {
        if(regions[i].left==-1)break;
        if(!is_on_screen(x,y,regions[i]))continue;
        any=true;
        const I32 current=static_cast<I32>(i);
        if(i>=7&&i<=11&&(i!=11||mode!=EditorMode::View))
            result.highlighted_region=i;
        if(last!=current) {
            last=current;
            switch(i) {
            case 7:result.new_voice_id=236;break; // 0xEC
            case 8:result.new_voice_id=235;break; // 0xEB
            case 9:result.new_voice_id=237;break; // 0xED
            case 10:result.new_voice_id=238;break;// 0xEE
            case 11:
                if(mode!=EditorMode::View)result.new_voice_id=239; // 0xEF
                break;
            default:break;
            }
        }
    }
    result.next_hover_index=any?last:-1;
    return result;
}

bool apply_season(SeasonResources& assets,Season season) noexcept {
    const auto s=static_cast<I32>(season);
    if(s!=0&&s!=1)return false;
    for(const auto& item:assets.placements) {
        if(item.object_code==-1&&
           (item.shape<0||item.shape>4||item.variant<0||item.variant>3))
            return false;
    }
    const auto index=static_cast<std::size_t>(s);
    assets.current_triplets=assets.seasonal_triplets[index];
    assets.current_palette=assets.seasonal_palette[index];
    assets.current_background=assets.background_surfaces[index];
    assets.current_scene_background=assets.seasonal_scene_background[index];
    for(std::size_t i=0;i<assets.placements.size();++i) {
        const auto& p=assets.placements[i];
        const U32 selected=assets.seasonal_objects[i][index];
        if(p.object_code==-1) {
            const auto group=static_cast<std::size_t>(p.variant+p.shape*4);
            assets.sub_sprite_surfaces[group].fill(selected);
        } else {
            assets.current_object_surfaces[i]=selected;
        }
    }
    return true;
}

std::optional<FountainRender> advance_fountain(SaveData& data) noexcept {
    const auto fountain_slot=data.state(TrailingStateIndex::PondSpecialRecord);
    const auto selected_slot=data.state(TrailingStateIndex::PondSelectedPrimary);
    if(fountain_slot<0||fountain_slot>=static_cast<I32>(data.objects.size())||
       selected_slot<0||selected_slot>=static_cast<I32>(data.objects.size()))
        return std::nullopt;
    auto& fountain=data.objects[static_cast<std::size_t>(fountain_slot)];
    if(fountain.object_code==-1)return std::nullopt;
    auto& linked=data.objects[static_cast<std::size_t>(selected_slot)];
    auto step=update_fountain_animation(fountain,linked.visual_frame_or_segment);
    return FountainRender{fountain_slot,fountain.left,fountain.top,
                          fountain_source_rect(fountain),step};
}

std::vector<DrawEntry> plan_sorted_object_draws(
    const SaveData& data,std::span<const std::size_t> sorted_slots) {

    std::vector<DrawEntry> result;
    result.reserve(sorted_slots.size());
    for(std::size_t i=0;i<sorted_slots.size();++i) {
        const auto slot=sorted_slots[i];
        if(slot>=data.objects.size())continue;
        const auto& object=data.objects[slot];
        const I32 type=object.object_code;
        if(type==-1||object.enabled_or_active!=1)continue;
        DrawKind kind=DrawKind::Object;
        I32 sprite=type;
        if(type>=100&&type<=199) {
            kind=type==100?DrawKind::PondSurface:
                              DrawKind::SegmentedPondSurface;
            sprite=static_cast<I32>(i); // original passes SORTED index
        } else if(type==7) {
            kind=DrawKind::Fountain;
            sprite=object.bound_variant;
        } else if(object.record_index>=200) {
            sprite=object.bound_category*5+object.bound_variant;
        }
        result.push_back({kind,static_cast<I32>(slot),
                          static_cast<I32>(i),sprite,
                          object.left,object.top,object.surface_ptr32});
    }
    return result;
}

std::vector<DrawEntry> plan_activity_draw(
    const SaveData& data,std::span<const std::size_t> sorted_slots,
    U32 background,U32 overlay) {

    std::vector<DrawEntry> result;
    result.push_back({DrawKind::Background,-1,-1,-1,0,0,background});
    auto objects=plan_sorted_object_draws(data,sorted_slots);
    result.insert(result.end(),objects.begin(),objects.end());
    result.push_back({DrawKind::Overlay,-1,-1,-1,0,0,overlay});
    return result;
}

} // namespace btb::park_designer
