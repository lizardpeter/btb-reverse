#pragma once
// Recovered branches of 0040E440: pond, fountain, accessories, bandstand.
// All input coordinates are retail cursor-origin coordinates, not screen
// cursor centers. State writes use the exact 76-byte retail save fields.
#include "btb/park_designer_retail_catalog.hpp"

namespace btb::park_designer {
enum class PlacementStatus {Invalid,Blocked,Full,Placed};
struct ParkPlacement {
    PlacementStatus status{PlacementStatus::Invalid};
    I32 slot{-1};
    I32 voice{-1};
    I32 voice_context{};
    I32 scroll_animation{};
};
inline constexpr std::array<Vec2i,5> kRetailPondCandidateDimensions{{
    {197,110},{163,104},{214,129},{250,110},{173,117}
}};
inline constexpr std::array<Vec2i,5> kRetailFountainAnchors{{
    {122,51},{83,59},{114,77},{145,68},{98,77}
}};
inline constexpr std::array<Vec2i,5> kRetailFountainHitDimensions{{
    {200,110},{160,104},{212,128},{230,130},{166,115}
}};
inline constexpr std::array<Vec2i,8> kRetailPrimarySpriteDimensions{{
    {640,480},{40,28},{43,61},{47,79},{47,33},{40,38},{40,28},{40,28}
}};
inline constexpr std::array<Rect,8> kRetailAccessoryCornerOffsets{{
    {-1,-1,-1,-1},{8,6,30,20},{10,35,32,38},{17,53,36,56},
    {8,16,35,22},{6,18,35,26},{-1,-1,-1,-1},{25,58,58,71}
}};
inline constexpr std::array<std::array<Vec2i,10>,5> kRetailPondInterior{{
    {{{124,10},{174,49},{110,77},{101,67},{80,95},{30,95},{9,71},{35,50},{81,51},{62,40}}},
    {{{82,14},{133,30},{143,54},{123,79},{86,89},{32,74},{17,42},{47,20}}},
    {{{127,23},{190,76},{100,114},{38,61}}},
    {{{157,15},{221,64},{140,101},{132,98},{75,123},{23,82},{72,58},{77,49}}},
    {{{55,13},{94,20},{108,44},{151,67},{135,94},{61,90},{49,64},{22,36}}}
}};
inline constexpr std::array<std::size_t,5> kRetailPondInteriorCounts{{10,8,4,8,8}};
inline constexpr std::array<Vec2i,4> kRetailBandstandPillarAnchors{{
    {15,32},{49,10},{94,26},{61,50}
}};
[[nodiscard]] ParkPlacement commit_retail_pond(SaveData&,I32 x,I32 y,I32 variant,
    std::optional<I32> moving_slot,retail::OriginalRetailRandom&);
[[nodiscard]] ParkPlacement commit_retail_bandstand(SaveData&,I32 x,I32 y,I32 variant,
    std::optional<I32> moving_slot,retail::OriginalRetailRandom&);

// 0040E440 pickup and 0040E3E0 finish: pillar pickup removes its roof
// and unlinks its old anchor before the carried object can be placed.
[[nodiscard]] bool begin_retail_park_drag(SaveData&,I32 slot) noexcept;
void finish_retail_park_drag(SaveData&,I32 slot) noexcept;

// Deletion uses source mode 1 picking, which excludes auxiliary pond
// layers (101..199); the selected primary and fountain have distinct
// family cleanup effects. The operation returns the original voice.
[[nodiscard]] ParkPlacement delete_retail_park_object(SaveData&,I32 slot,
    retail::OriginalRetailRandom&);
}
