#include "btb/retail_game_lifecycle_15.hpp"

#include <exception>
#include <sstream>

namespace btb::game_batch_15 {

bool save_park_designer(
    const park_designer::SaveData& data,std::ostream* file) {
    // 0040AC90 opens dypdata[1..5].txt; missing fopen() skips writes.
    if(file==nullptr)return false;
    try {
        park_designer::write_save(*file,data);
        return static_cast<bool>(*file);
    } catch(const std::exception&) {
        return false;
    }
}

ParkLoadOutcome load_park_designer(
    std::istream& original_polygons,std::istream* optional_save,
    park_designer::SaveData& data,park_designer::BoundAreas& bounds,
    const SeasonCallback& apply) {
    ParkLoadOutcome outcome{};
    // 0040AEA0 starts with all sixty authored polygon groups, including
    // stored terminators. Parse before touching state to avoid synthetic data.
    try {
        auto parsed=park_designer::parse_bound_areas(original_polygons);
        bounds=std::move(parsed);
        outcome.polygons_loaded=true;
        if(optional_save==nullptr)return outcome;
        auto loaded=park_designer::read_save(*optional_save);
        data=std::move(loaded);
        outcome.save_loaded=true;
        if(data.state(park_designer::TrailingStateIndex::Season)==1) {
            if(!apply) {
                outcome.error="original ApplyParkDesignerSeason provider unavailable";
                return outcome;
            }
            if(!apply(park_designer::Season::Winter)) {
                outcome.error="original winter surface bank rejected";
                return outcome;
            }
            outcome.applied_winter=true;
        }
    } catch(const std::exception& e) {
        outcome.error=e.what();
    }
    return outcome;
}

bool refresh_park_geometry(
    park_designer::SaveData& data,U32 slot,
    std::span<const I32> depth_lookup,const SurfaceQuery& query) {
    if(slot>=data.objects.size()||depth_lookup.empty()||!query)return false;
    auto& obj=data.objects[slot];
    // Original only clamps codes >99. Corrupt negative codes would index
    // outside the retail table: reject them on a safe modern host.
    I32 category=obj.object_code;
    if(category>99)category=0;
    if(category<0||static_cast<std::size_t>(category)>=depth_lookup.size())
        return false;
    // Two native GetSurfaceDesc vtable calls, both before geometry writes.
    if(!query(static_cast<U32>(category)) ||
       !query(static_cast<U32>(category)))return false;
    obj.bottom=obj.top+0x7c;
    obj.right=obj.left+6;
    obj.enabled_or_active=1;
    obj.sprite_width=6;
    obj.sprite_height=0x7c;
    obj.sort_layer=depth_lookup[static_cast<std::size_t>(category)];
    obj.record_index=static_cast<I32>(slot);
    return true;
}
} // namespace btb::game_batch_15
