#pragma once
// Retail Park Designer tables and decoration placement branch, recovered
// directly from the original PE32 (SHA256 c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05).
// Coordinates supplied to the commit operation must already include the
// retail cursor hotspot adjustment. No synthetic bitmap assets are required.
#include "btb/park_designer_scene.hpp"
#include "btb/retail_crt_random.hpp"

#include <algorithm>
#include <array>
#include <limits>
#include <optional>
#include <string_view>

namespace btb::park_designer {

inline constexpr std::array<Rect,13> kRetailToolbarRegions{{
    {246,414,280,457},{280,414,314,457},{314,414,348,457},
    {348,414,382,457},{382,414,416,457},{425,402,465,427},
    {425,428,465,453},{68,419,119,471},{123,419,174,471},
    {178,419,229,471},{478,416,532,471},{535,426,571,462},
    {-1,-1,-1,-1}
}}; // 00441CD8
inline constexpr std::array<Rect,4> kRetailViewRegions{{
    {270,416,324,470},{329,416,383,470},{388,416,442,470},
    {535,426,571,462}
}}; // 00441DA8 / 00441DB8; extents in 0040E440.

[[nodiscard]] constexpr I32 retail_toolbar_hit(I32 x,I32 y) noexcept {
    for(std::size_t i=0;i<kRetailToolbarRegions.size();++i) {
        const auto& r=kRetailToolbarRegions[i];
        if(r.left==-1)break;
        if(r.left<x&&x<r.right&&r.top<y&&y<r.bottom)
            return static_cast<I32>(i);
    }
    return -1;
}

struct SeasonalBitmap {
    std::string_view summer;
    std::string_view winter;
    [[nodiscard]] constexpr std::string_view for_season(Season s) const noexcept {
        return s==Season::Summer?summer:s==Season::Winter?winter:std::string_view{};
    }
};
inline constexpr SeasonalBitmap kRetailParkBackground{
    "Data/SubGameDYP/largegras.bmp","Data/SubGameDYP/SNOW/largegras.bmp"};
inline constexpr std::string_view kRetailParkOverlay="Data/SubGameDYP/background.bmp";
inline constexpr std::array<SeasonalBitmap,4> kRetailDecorationSheets{{
    {"Data/SubGameDYP/bushes.bmp","Data/SubGameDYP/SNOW/bushes.bmp"},
    {"Data/SubGameDYP/trees.bmp","Data/SubGameDYP/SNOW/trees.bmp"},
    {"Data/SubGameDYP/benchs.bmp","Data/SubGameDYP/SNOW/benchs.bmp"},
    {"Data/SubGameDYP/flowers.bmp","Data/SubGameDYP/SNOW/flowers.bmp"}
}};
inline constexpr std::array<SeasonalBitmap,3> kRetailBandstandSheets{{
    {"Data/SubGameDYP/bases_01.bmp","Data/SubGameDYP/SNOW/bases_01.bmp"},
    {"Data/SubGameDYP/pillars.bmp","Data/SubGameDYP/SNOW/pillars.bmp"},
    {"Data/SubGameDYP/roofs.bmp","Data/SubGameDYP/SNOW/roofs.bmp"}
}};
inline constexpr std::array<Vec2i,4> kRetailDecorationDimensions{{
    {124,65},{104,199},{93,79},{114,84}
}}; // immediate initializers in 0040B7E0
inline constexpr std::array<Vec2i,3> kRetailBandstandDimensions{{
    {127,72},{17,64},{127,81}
}};
inline constexpr std::array<std::string_view,3> kRetailPaletteSheets{{
    "Data/SubGameDYP/pondopjects.bmp","Data/SubGameDYP/bandstandobjects.bmp",
    "Data/SubGameDYP/decorateobjects.bmp"
}};
inline constexpr std::array<std::string_view,3> kRetailPaletteTrays{{
    "Data/SubGameDYP/pondmodetray.bmp","Data/SubGameDYP/bandmodetray.bmp",
    "Data/SubGameDYP/designmodetray.bmp"
}};
struct ToolbarBitmapPair {std::string_view hover,pressed;};
inline constexpr std::array<ToolbarBitmapPair,5> kRetailModeToolbarSheets{{
    {"Data/SubGameDYP/pondred.bmp","Data/SubGameDYP/ponddep.bmp"},
    {"Data/SubGameDYP/bandred.bmp","Data/SubGameDYP/banddep.bmp"},
    {"Data/SubGameDYP/decoratered.bmp","Data/SubGameDYP/decoratedep.bmp"},
    {"Data/SubGameDYP/vuiewred.bmp","Data/SubGameDYP/viewdep.bmp"},
    {"Data/SubGameDYP/deletered.bmp","Data/SubGameDYP/deletedep.bmp"}
}};
struct ViewToolbarBitmap {std::string_view normal,hover,pressed;};
inline constexpr std::array<ViewToolbarBitmap,4> kRetailViewToolbarSheets{{
    {"Data/SubGameDYP/summer.bmp","Data/SubGameDYP/summerred.bmp","Data/SubGameDYP/summerdep.bmp"},
    {"Data/SubGameDYP/winter.bmp","Data/SubGameDYP/winterred.bmp","Data/SubGameDYP/winterdep.bmp"},
    {"Data/SubGameDYP/print.bmp","Data/SubGameDYP/printred.bmp","Data/SubGameDYP/printdep.bmp"},
    {"Data/SubGameDYP/deleteall.bmp","Data/SubGameDYP/deleteallred.bmp","Data/SubGameDYP/deletealldep.bmp"}
}};

// Exact retail 00441844/58/94, including the source's hard 2053
// pixel crop cap. Segmented water draws each layer at the SAME position.
inline constexpr std::array<I32,5> kRetailPondSegmentCounts{{4,2,2,3,3}};
inline constexpr std::array<I32,5> kRetailPondSegmentWidths{{198,164,221,257,173}};
inline constexpr std::array<I32,5> kRetailPondSegmentHeights{{109,103,130,138,117}};
inline constexpr std::array<SeasonalBitmap,5> kRetailPondSheets{{
    {"Data/SubGameDYP/roundsquqre_01.bmp","Data/SubGameDYP/SNOW/roundsquqre_01.bmp"},
    {"Data/SubGameDYP/round_pond.bmp","Data/SubGameDYP/SNOW/round_pond.bmp"},
    {"Data/SubGameDYP/square.bmp","Data/SubGameDYP/SNOW/square.bmp"},
    {"Data/SubGameDYP/2xsquare.bmp","Data/SubGameDYP/SNOW/2xsquare.bmp"},
    {"Data/SubGameDYP/roundround.bmp","Data/SubGameDYP/SNOW/roundround.bmp"}
}};
inline constexpr SeasonalBitmap kRetailFountainSheet{
    "Data/SubGameDYP/fountains_1-5_square.bmp","Data/SubGameDYP/SNOW/fountains_1-5_square.bmp"};
inline constexpr std::array<SeasonalBitmap,8> kRetailPrimaryObjects{{
    kRetailParkBackground,
    {"Data/SubGameDYP/lilly.bmp","Data/SubGameDYP/SNOW/lilly.bmp"},
    {"Data/SubGameDYP/reeds_01.bmp","Data/SubGameDYP/SNOW/reeds_01.bmp"},
    {"Data/SubGameDYP/reed_02.bmp","Data/SubGameDYP/SNOW/reed_02.bmp"},
    {"Data/SubGameDYP/rock_02.bmp","Data/SubGameDYP/SNOW/rock_02.bmp"},
    {"Data/SubGameDYP/rock_01.bmp","Data/SubGameDYP/SNOW/rock_01.bmp"},
    {"Data/SubGameDYP/lilly.bmp","Data/SubGameDYP/lilly.bmp"},
    kRetailFountainSheet
}};
[[nodiscard]] constexpr std::optional<Rect> retail_pond_segment_rect(
    I32 variant,I32 segment) noexcept {
    if(variant<0||variant>=5||segment<0||segment>20)return std::nullopt;
    const auto i=static_cast<std::size_t>(variant);
    const I32 left=segment*kRetailPondSegmentWidths[i];
    const I32 right=std::min((segment+1)*kRetailPondSegmentWidths[i],0x805);
    if(left>=right)return std::nullopt;
    return Rect{left,0,right,kRetailPondSegmentHeights[i]};
}
[[nodiscard]] constexpr std::optional<Rect> retail_primary_pond_rect(
    I32 variant,I32 frame) noexcept {
    if(variant<0||variant>=5||frame<0||frame>12)return std::nullopt;
    return retail_pond_segment_rect(variant,frame>0?frame+kRetailPondSegmentCounts[static_cast<std::size_t>(variant)]:frame);
}

struct RetailSprite {std::string_view bitmap;Rect source;};
[[nodiscard]] constexpr std::optional<RetailSprite> retail_park_sprite(
    EditorMode mode,Season season,I32 category,I32 variant) noexcept {
    if(category<0||variant<0||variant>=5)return std::nullopt;
    const auto i=static_cast<std::size_t>(category);
    Vec2i dim{};
    std::string_view asset;
    if(mode==EditorMode::Decorate&&i<kRetailDecorationSheets.size()){
        dim=kRetailDecorationDimensions[i];
        asset=kRetailDecorationSheets[i].for_season(season);
    }else if(mode==EditorMode::Bandstand&&i<kRetailBandstandSheets.size()){
        dim=kRetailBandstandDimensions[i];
        asset=kRetailBandstandSheets[i].for_season(season);
    }else return std::nullopt;
    if(asset.empty())return std::nullopt;
    return RetailSprite{asset,{variant*dim.x,0,(variant+1)*dim.x,dim.y}};
}
[[nodiscard]] constexpr std::optional<Rect> retail_palette_thumbnail(I32 category,I32 variant) noexcept {
    if(category<0||category>=4||variant<0||variant>=5)return std::nullopt;
    return Rect{variant*34,category*61,(variant+1)*34,(category+1)*61};
}

// Separate tables: the candidate footprint at 00441DD8 is NOT the
// stored object's collision rectangle indexed by +0x10 at 00441B18.
inline constexpr std::array<Rect,20> kRetailDecorationCandidateFootprints{{
    {31,9,105,47},{33,9,92,58},{0,0,124,65},{27,8,80,51},{39,19,86,46},
    {30,150,67,172},{41,150,64,164},{51,165,59,171},{43,159,58,165},{44,182,57,189},
    {6,0,74,76},{3,4,92,70},{5,1,73,76},{28,12,65,56},{23,14,67,78},
    {25,13,113,72},{40,27,73,54},{0,11,108,67},{45,31,59,41},{8,24,88,74}
}};
inline constexpr std::array<Rect,28> kRetailStoredCollisionFootprints{{
    {40,16,92,43},{46,20,90,48},{18,16,353,46},{40,22,74,43},{47,27,82,39},
    {30,150,67,172},{41,150,64,164},{51,165,59,171},{43,159,58,165},{44,182,57,189},
    {10,49,68,68},{7,49,83,69},{5,49,66,71},{34,43,58,58},{38,63,66,78},
    {26,21,109,22},{47,39,60,54},{6,22,100,23},{45,33,58,42},{10,31,80,32},
    {-1,-1,-1,-1},{8,6,30,20},{10,35,32,38},{17,53,36,56},
    {8,16,35,22},{6,18,35,26},{-1,-1,-1,-1},{25,58,58,71}
}};

enum class DecorationCommitStatus {InvalidInput,Blocked,Full,Placed};
struct DecorationCommitResult {
    DecorationCommitStatus status{DecorationCommitStatus::InvalidInput};
    I32 slot{-1};
    Vec2i restored_pointer{};
    std::optional<I32> managed_voice;
    I32 voice_context{}; // 99: original input-interruptible voice; 1: full warning.
};

// 004104F1..00410858, after source drag/pointer readiness. The collision
// check always passes flag=0 and excluded_slot=-1, including moves.
// The source preserves alternate-origin words +14/+18 and increments
// the allocation counter only for a new object.
[[nodiscard]] inline DecorationCommitResult commit_retail_decoration(
    SaveData& data,I32 x,I32 y,I32 selected_variant,
    std::optional<I32> moving_slot,retail::OriginalRetailRandom& random) {
    if(data.state(TrailingStateIndex::ActiveEditorMode)!=2)return {};
    const I32 page=data.state(TrailingStateIndex::DecoratePalettePage);
    if(page<0||page>=4)return {};
    I32 category=page,variant=selected_variant;
    if(moving_slot){
        if(*moving_slot<300||*moving_slot>=400)return {};
        const auto& old=data.objects[static_cast<std::size_t>(*moving_slot)];
        if(old.object_code==-1)return {};
        category=old.bound_category;variant=old.bound_variant;
    }
    if(category<0||category>=4||variant<0||variant>=5)return {};
    const auto index=static_cast<std::size_t>(category*5+variant);
    const auto& dim=kRetailDecorationDimensions[static_cast<std::size_t>(category)];
    const auto& footprint=kRetailDecorationCandidateFootprints[index];
    // All offsets and rectangle extents are nonnegative. Fail closed
    // on malformed external coordinates instead of signed overflow.
    if(x>std::numeric_limits<I32>::max()-dim.x||
       y>std::numeric_limits<I32>::max()-dim.y)return {};
    const auto check=evaluate_placement(data,x+footprint.left,y+footprint.top,
        footprint.right-footprint.left,footprint.bottom-footprint.top,0,-1,
        kRetailStoredCollisionFootprints);
    if(check.invalid_geometry)return {};
    if(!check.permitted)return {DecorationCommitStatus::Blocked};
    I32 slot{};
    if(moving_slot)slot=*moving_slot;
    else{
        auto& next=data.state(TrailingStateIndex::NextDecorateRecord);
        if(next<300||next>=400)return {};
        if(next>=398)return {DecorationCommitStatus::Full,-1,{},241,1};
        slot=next;
        if(data.objects[static_cast<std::size_t>(slot)].object_code!=-1)return {};
        ++next;
    }
    auto& o=data.objects[static_cast<std::size_t>(slot)];
    o.left=x;o.top=y;o.right=x+dim.x;o.bottom=y+dim.y;
    o.surface_ptr32=static_cast<U32>(index);
    o.object_code=400;o.object_subcode=0;o.enabled_or_active=1;
    o.sprite_width=dim.x;o.sprite_height=dim.y;o.sort_layer=201;o.record_index=slot;
    o.bound_category=category;o.bound_variant=variant;o.visual_frame_or_segment=0;
    o.fountain_phase=0;o.fountain_frame_countdown=9;
    DecorationCommitResult result{DecorationCommitStatus::Placed,slot,
        {x+dim.x/2,y+dim.y/2}};
    if(random.next_rand()%3==0){
        const I32 base=page==0?219:page==1?216:page==2?(variant<=2?225:229):222;
        result.managed_voice=base+random.next_rand()%2;
        result.voice_context=99;
    }
    return result;
}
} // namespace btb::park_designer
