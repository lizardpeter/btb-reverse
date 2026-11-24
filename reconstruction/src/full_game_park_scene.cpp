#include "btb/full_game_park_scene.hpp"
#include "btb/original_source9.hpp"
#include <algorithm>
#include <numeric>
#include <span>
#include <utility>

namespace btb::full_game {
namespace {
namespace p=park_designer;
using Index=std::ptrdiff_t;

template<class Compare>
void retail_qsort(std::span<std::size_t> records,Index lo,Index hi,Compare compare) {
    // Direct index translation of the old 32-bit CRT. The source orders
    // larger partitions with a stack; recursion on both partitions is
    // equivalent because neither comparison has side effects. Max 400.
    while(lo<hi) {
        if(hi-lo+1<=8) {
            for(Index end=hi;end>lo;--end) {
                Index max=lo;
                for(Index i=lo+1;i<=end;++i)
                    if(compare(records[i],records[max])>0)max=i;
                std::swap(records[max],records[end]);
            }
            return;
        }
        std::swap(records[lo],records[lo+(hi-lo+1)/2]);
        Index left=lo,right=hi+1;
        do {
            do {++left;} while(left<=hi&&compare(records[left],records[lo])<=0);
            do {--right;} while(right>lo&&compare(records[right],records[lo])>=0);
            if(left<=right)std::swap(records[left],records[right]);
        } while(left<=right);
        std::swap(records[lo],records[right]);
        // Smaller partition first, as in the original; no comparator
        // invocation can read out of bounds even when ties are asymmetric.
        if(right-lo<hi-left+1) {
            retail_qsort(records,lo,right-1,compare);
            lo=left;
        } else {
            retail_qsort(records,left,hi,compare);
            hi=right-1;
        }
    }
}

[[nodiscard]] bool valid_record(const p::RetailObjectRecord32& o,std::size_t i) {
    if(o.object_code==-1)return true;
    // A save is external data: prevent overflow in retail arithmetic.
    auto coord=[](p::I32 v){return v>=-10000&&v<=10000;};
    if(o.record_index!=static_cast<p::I32>(i)||!coord(o.left)||!coord(o.top)||
       !coord(o.right)||!coord(o.bottom)||o.object_code<0||
       o.bound_category<0||o.bound_category>=4||o.bound_variant<0||o.bound_variant>=5)
        return false;
    if(o.object_code==7)
        return o.visual_frame_or_segment>=0&&o.visual_frame_or_segment<13&&
            (o.fountain_phase==0||o.fountain_phase==1)&&
            o.fountain_frame_countdown>=0&&o.fountain_frame_countdown<=9;
    return true;
}

void draw(std::vector<Draw>& out,std::string_view asset,int x,int y,
          std::optional<p::Rect> crop={},bool key=true) {
    Draw d{std::string(asset),x,y,{},key};
    if(crop)d.source_rectangle=Rect{crop->left,crop->top,crop->right,crop->bottom};
    out.push_back(std::move(d));
}
}

bool plan_retail_park_scene(p::SaveData& save,RetailParkScenePlan& out,std::string& error) {
    const int next=save.state(p::TrailingStateIndex::NextDecorateRecord);
    const int season_value=save.state(p::TrailingStateIndex::Season);
    if(next<300||next>400||season_value<0||season_value>1) {
        error="Park Designer invalid retail scene counters/season";return false;
    }
    for(std::size_t i=0;i<save.objects.size();++i)
        if(!valid_record(save.objects[i],i)) {
            error="Park Designer invalid retail object record "+std::to_string(i);return false;
        }
    // Stage animation and output until the entire scene can be resolved.
    auto candidate=save;
    RetailParkScenePlan plan{};
    std::iota(plan.sorted_slots.begin(),plan.sorted_slots.end(),std::size_t{0});
    btb::retail_source_batch::ParkSortContext ctx{};
    std::copy_n(p::kRetailStoredCollisionFootprints.begin(),ctx.category_offsets.size(),ctx.category_offsets.begin());
    const auto band=save.state(p::TrailingStateIndex::BandstandSelectedRecord);
    if(band>=0&&band<400)ctx.selected_bandstand_bottom=save.objects[static_cast<std::size_t>(band)].bottom;
    else if(band!=-1){error="Park Designer invalid selected bandstand";return false;}
    if(band==-1&&std::any_of(save.objects.begin(),save.objects.end(),[](const auto& o){
        return o.object_code==300&&o.bound_category>=1;
    })) {error="Park Designer bandstand component lacks selected base";return false;}

    auto first=std::span(plan.sorted_slots).first(static_cast<std::size_t>(next));
    retail_qsort(first,0,next-1,[&](auto a,auto b){
        const auto& x=save.objects[a];const auto& y=save.objects[b];
        if(x.object_code==-1)return y.object_code==-1?0:-1;
        if(y.object_code==-1)return 1;
        return x.sort_layer<=y.sort_layer?-1:1;
    });
    retail_qsort(first,0,next-1,[&](auto a,auto b){
        return original_source9::compare_park_draw_objects(save.objects[a],save.objects[b],ctx);
    });
    const auto season=static_cast<p::Season>(season_value);
    draw(plan.draws,p::kRetailParkBackground.for_season(season),0,0,{},false);
    for(const auto slot:plan.sorted_slots) {
        const auto& o=candidate.objects[slot];
        if(o.object_code==-1||o.enabled_or_active!=1)continue;
        if(o.object_code==100) {
            auto crop=p::retail_primary_pond_rect(o.bound_variant,o.visual_frame_or_segment);
            if(!crop){error="Park Designer invalid primary pond crop";return false;}
            draw(plan.draws,p::kRetailPondSheets[static_cast<std::size_t>(o.bound_variant)].for_season(season),o.left,o.top,crop);
        } else if(o.object_code>=101&&o.object_code<=199) {
            const auto variant=candidate.state(p::TrailingStateIndex::PondShapeVariant);
            if(variant<0||variant>=5){error="Park Designer invalid segmented pond variant";return false;}
            const auto i=static_cast<std::size_t>(variant);
            for(int j=1;j<=p::kRetailPondSegmentCounts[i];++j){
                auto crop=p::retail_pond_segment_rect(variant,j);
                if(!crop){error="Park Designer invalid segmented pond crop";return false;}
                draw(plan.draws,p::kRetailPondSheets[i].for_season(season),o.left,o.top,crop);
            }
        } else if(o.object_code==7) {
            // Retail draws via the selected fountain global, not the
            // current sorted slot. The animation happens at this draw point.
            const auto f=candidate.state(p::TrailingStateIndex::PondSpecialRecord);
            if(f==-1)continue;
            const auto primary=candidate.state(p::TrailingStateIndex::PondSelectedPrimary);
            if(f<0||f>=100||primary<0||primary>=100||
               candidate.objects[f].object_code!=7||candidate.objects[primary].object_code!=100) {
                error="Park Designer fountain link invalid";return false;
            }
            const auto animated=p::advance_fountain(candidate);
            if(!animated){error="Park Designer fountain state invalid";return false;}
            const auto& r=animated->source;
            draw(plan.draws,p::kRetailFountainSheet.for_season(season),animated->x,animated->y,
                 p::Rect{r.left,r.top,r.right,r.bottom});
        } else if(o.record_index>=200) {
            auto sprite=p::retail_park_sprite(o.record_index>=300?p::EditorMode::Decorate:p::EditorMode::Bandstand,
                season,o.bound_category,o.bound_variant);
            if(!sprite){error="Park Designer unsupported original sprite cell";return false;}
            draw(plan.draws,sprite->bitmap,o.left,o.top,sprite->source);
        } else {
            if(o.object_code>=static_cast<int>(p::kRetailPrimaryObjects.size())) {
                error="Park Designer unsupported primary sprite code";return false;
            }
            draw(plan.draws,p::kRetailPrimaryObjects[static_cast<std::size_t>(o.object_code)].for_season(season),o.left,o.top);
        }
    }
    draw(plan.draws,p::kRetailParkOverlay,0,0);
    save=std::move(candidate);out=std::move(plan);error.clear();return true;
}
} // namespace btb::full_game
