#pragma once
// Original-executable-driven Park Designer scene (PE32 SHA256:
// c6e35972af53e972382f095ac3eaf13e00efbe1573a0fbfeccd39c2eff388b05).
// Function bodies: 0040D1D0, 0040D2E0, 0040D470, 0040D900,
// 0040DA60, 0040DB50, 0040DFF0, 0040E2B0.
// Types reuse the exact 76-byte object and 400-record save representation.
// Source-level behavior; host asset/DirectDraw provider and retail ABI remain
// separate, with missing evidence never silently replaced by fake assets.
#include "btb/park_designer_data.hpp"
#include "btb/park_designer_runtime.hpp"
#include "btb/retail_source_batch_20261011.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace btb::park_designer {
using Rect = btb::retail_source_batch::Rect32;
using I32 = std::int32_t;
using U32 = std::uint32_t;

struct CompactionResult {
    std::size_t moved_primary{};
    std::size_t moved_decoration{};
};
// 0040E2B0: compact ONLY primary pond slots [0,100) and decorations
// [300,400). Preserve the 76-byte record and update internal record_index.
// Original selected/next-record global updates are mirrored in SaveData.
[[nodiscard]] CompactionResult compact_record_families(SaveData& data) noexcept;

struct PickHit {
    I32 original_sorted_position{-1};
    I32 record_slot{-1};
    friend bool operator==(const PickHit&,const PickHit&)=default;
};
// 0040D2E0: input list is the original 400-entry qsort result, not a list
// already sorted by this function. Polygon status 0 means INSIDE (retail).
// Nullopt indicates no hit. Invalid polygon-index records are ignored rather
// than reading outside the verified 60-polygon native array.
[[nodiscard]] std::optional<PickHit> find_topmost_object(
    const SaveData& data,const BoundAreas& bounds,
    std::span<const std::size_t> sorted_slots,
    I32 mouse_x,I32 mouse_y,I32 picking_mode,
    I32 selected_bandstand,I32 pointer_pressed,I32 cursor_enabled);

struct PlacementResult {
    bool permitted{true};
    bool invalid_geometry{};
    std::vector<I32> invalidated_large_object_slots{};
};
// 0040DFF0: preserves the unusual side effect where a colliding
// >399-code record is invalidated, then other exclusion checks continue.
// "flag" is the native parameter_5, and excluded_slot is parameter_6.
// Offsets are original 0x00441B18..24 16-byte rectangle table entries.
// Record field +0x10 is a 32-bit index or -1 sentinel in this path.
[[nodiscard]] PlacementResult evaluate_placement(
    SaveData& data, I32 x,I32 y,I32 width,I32 height,
    I32 flag,I32 excluded_slot,
    std::span<const Rect> source_geometry);

struct ToolbarHover {
    I32 next_hover_index{-1};
    std::optional<I32> new_voice_id{};
    std::optional<std::size_t> highlighted_region{};
};
// 0040D470. Regions start from source index 0, terminate when left==-1.
// Managed hover sound is emitted only when the hover index changes.
[[nodiscard]] ToolbarHover update_toolbar_hover(
    std::span<const Rect> regions,I32 x,I32 y,
    I32 previous_hover,EditorMode editor_mode) noexcept;

struct SeasonResources {
    std::array<std::array<U32,15>,2> seasonal_triplets{};
    std::array<U32,15> current_triplets{};
    std::array<std::array<U32,5>,2> seasonal_palette{};
    std::array<U32,5> current_palette{};
    std::array<U32,2> background_surfaces{};
    U32 current_background{};
    std::array<U32,2> seasonal_scene_background{};
    U32 current_scene_background{};
    struct Placement {
        I32 shape{};
        I32 variant{};
        I32 object_code{}; // -1 = update all 5 sub-sprites.
    };
    std::array<Placement,9> placements{};
    std::array<std::array<U32,2>,9> seasonal_objects{};
    std::array<U32,9> current_object_surfaces{};
    std::array<std::array<U32,5>,20> sub_sprite_surfaces{};
};
// 0040D1D0. Caller owns the 9 records/20 sprite families recovered from
// original globals. No fabricated season-bitmaps. Unsupported indices fail.
[[nodiscard]] bool apply_season(
    SeasonResources& assets,Season season) noexcept;

struct FountainRender {
    I32 slot{-1};
    I32 x{},y{};
    FountainSourceRect source{};
    FountainAnimationStep animation{};
};
// 0040D900: update fountain and linked pond frames, produce source rectangle.
// No blit happens until the actual DirectDraw host consumes the plan.
[[nodiscard]] std::optional<FountainRender> advance_fountain(
    SaveData& data) noexcept;

enum class DrawKind : std::uint8_t {
    Background, Object, PondSurface, SegmentedPondSurface,
    Fountain, Overlay,
};
struct DrawEntry {
    DrawKind kind{DrawKind::Object};
    I32 slot{-1};
    I32 sorted_position{-1};
    I32 sprite_index{-1};
    I32 x{},y{};
    U32 handle32{};
};
// 0040DA60: selects original rendering family in provided qsort order.
// 0040DB50: backgrounds before objects, toolbar overlay after objects.
// Does not substitute missing original asset handles with synthetic sprites.
[[nodiscard]] std::vector<DrawEntry> plan_sorted_object_draws(
    const SaveData& data,std::span<const std::size_t> sorted_slots);
[[nodiscard]] std::vector<DrawEntry> plan_activity_draw(
    const SaveData& data,std::span<const std::size_t> sorted_slots,
    U32 background_surface,U32 overlay_surface);

} // namespace btb::park_designer
