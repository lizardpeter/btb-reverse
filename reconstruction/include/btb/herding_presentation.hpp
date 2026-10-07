#pragma once

#include "btb/herding_runtime.hpp"

#include <array>
#include <cstdint>
#include <optional>

namespace btb::herding {

struct RenderRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
    friend bool operator==(const RenderRect&, const RenderRect&) = default;
};

inline constexpr std::int32_t kWorldViewportWidth = 600;
inline constexpr std::int32_t kWorldViewportHeight = 380;
inline constexpr std::int32_t kWorldViewportDestX = 20;
inline constexpr std::int32_t kWorldViewportDestY = 20;

inline constexpr std::int32_t kCameraFollowX = 300;
inline constexpr std::int32_t kCameraFollowY = 110;
inline constexpr std::int32_t kCameraMaximumX = 672;
inline constexpr std::int32_t kCameraMaximumY = 502;

struct CameraPlan {
    std::int32_t camera_x{};
    std::int32_t camera_y{};
    RenderRect background_source{};
    std::int32_t background_dest_x{kWorldViewportDestX};
    std::int32_t background_dest_y{kWorldViewportDestY};
};

// Exact opening camera math from 0x00416370 DrawHerdingActivity.
// Pickles X tracks around 300 and clamps source X to 0..672.
// Pickles Y tracks around 110 and clamps source Y to 0..502.
[[nodiscard]] constexpr CameraPlan herding_camera_plan(
    std::int32_t pickles_x,
    std::int32_t pickles_y) noexcept {

    std::int32_t camera_x{};
    if (pickles_x < 300) {
        camera_x = 0;
    } else if (pickles_x > 972) {
        camera_x = kCameraMaximumX;
    } else {
        camera_x = pickles_x - kCameraFollowX;
    }

    std::int32_t camera_y{};
    const auto y_plus_80 = pickles_y + 80;
    if (y_plus_80 < 190) {
        camera_y = 0;
    } else if (y_plus_80 > 692) {
        camera_y = kCameraMaximumY;
    } else {
        camera_y = pickles_y - kCameraFollowY;
    }

    return {
        camera_x,
        camera_y,
        {
            camera_x,
            camera_y,
            camera_x + kWorldViewportWidth,
            camera_y + kWorldViewportHeight,
        },
        kWorldViewportDestX,
        kWorldViewportDestY,
    };
}

[[nodiscard]] constexpr std::int32_t entity_sprite_height(
    const RetailEntityRecord32& entity) noexcept {
    return entity.source_bottom - entity.source_top;
}

[[nodiscard]] constexpr std::int32_t entity_depth_y(
    const RetailEntityRecord32& entity) noexcept {
    return entity.y + entity_sprite_height(entity);
}

[[nodiscard]] constexpr bool is_gate(EntityType type) noexcept {
    return type == EntityType::GateLeft ||
           type == EntityType::GateRight;
}

// Exact qsort callback at 0x00416310.
// Gates always sort before ordinary entities. Ordinary entities sort by
// sprite-bottom world Y. Retail returns -1 on equal bottoms rather than 0.
[[nodiscard]] constexpr std::int32_t compare_entities_by_depth(
    const RetailEntityRecord32& a,
    const RetailEntityRecord32& b) noexcept {

    const auto a_type = static_cast<EntityType>(a.type);
    const auto b_type = static_cast<EntityType>(b.type);

    if (is_gate(a_type)) {
        return -1;
    }
    if (is_gate(b_type)) {
        return 1;
    }

    return entity_depth_y(a) > entity_depth_y(b)
        ? 1
        : -1;
}

struct SpriteSheetGeometry {
    std::int32_t frame_width{};
    std::int32_t frame_height{};
    std::int32_t frames_per_direction{};
};

inline constexpr std::int32_t kPicklesCellSize = 128;
inline constexpr std::int32_t kPicklesOddDirectionFrameBankOffset = 39;

[[nodiscard]] constexpr std::optional<RenderRect>
pickles_source_rect(
    std::int32_t direction,
    std::int32_t frame) noexcept {

    if (direction < 0 || direction >= 8 || frame < 0) {
        return std::nullopt;
    }

    const auto horizontal_bank =
        (direction & 1) != 0
            ? kPicklesOddDirectionFrameBankOffset
            : 0;
    const auto vertical_bank = direction / 2;

    const auto left =
        (frame + horizontal_bank) * kPicklesCellSize;
    const auto top =
        vertical_bank * kPicklesCellSize;

    return RenderRect{
        left,
        top,
        left + kPicklesCellSize,
        top + kPicklesCellSize,
    };
}

inline constexpr SpriteSheetGeometry kSheepGeometry{98,103,10};
inline constexpr SpriteSheetGeometry kRabbitGeometry{73,69,10};
inline constexpr SpriteSheetGeometry kDuckGeometry{53,50,6};
inline constexpr SpriteSheetGeometry kScrufftyGeometry{86,87,7};

[[nodiscard]] constexpr std::optional<SpriteSheetGeometry>
directional_geometry(EntityType type) noexcept {
    switch (type) {
    case EntityType::Sheep: return kSheepGeometry;
    case EntityType::Rabbit: return kRabbitGeometry;
    case EntityType::Duck: return kDuckGeometry;
    case EntityType::Scruffty: return kScrufftyGeometry;
    default: return std::nullopt;
    }
}

[[nodiscard]] constexpr std::optional<RenderRect>
directional_source_rect(
    EntityType type,
    std::int32_t direction,
    std::int32_t frame) noexcept {

    const auto geometry = directional_geometry(type);
    if (!geometry || direction < 0 || frame < 0) {
        return std::nullopt;
    }

    const auto index =
        direction * geometry->frames_per_direction + frame;
    const auto left = index * geometry->frame_width;
    return RenderRect{
        left,
        0,
        left + geometry->frame_width,
        geometry->frame_height,
    };
}

inline constexpr RenderRect kTrailer1Source{0,0,151,240};
inline constexpr RenderRect kTrailer2Source{0,0,43,76};

inline constexpr std::int32_t kTravisCabFrameWidth = 230;
inline constexpr std::int32_t kTravisCabFrameHeight = 240;
[[nodiscard]] constexpr RenderRect travis_cab_source_rect(
    std::int32_t frame) noexcept {
    return {
        frame * kTravisCabFrameWidth,
        0,
        (frame + 1) * kTravisCabFrameWidth,
        kTravisCabFrameHeight,
    };
}

inline constexpr std::int32_t kGateFrameWidth = 120;
inline constexpr std::int32_t kGateFrameHeight = 70;
inline constexpr Vec2i kLeftGateWorldPosition{717,162};
inline constexpr Vec2i kRightGateWorldPosition{859,101};

[[nodiscard]] constexpr RenderRect gate_source_rect(
    std::int32_t frame) noexcept {
    return {
        frame * kGateFrameWidth,
        0,
        (frame + 1) * kGateFrameWidth,
        kGateFrameHeight,
    };
}

struct StaticEntityRenderPlan {
    Vec2i world_position{};
    RenderRect source_rect{};
    bool force_world_position{};
};

[[nodiscard]] constexpr std::optional<StaticEntityRenderPlan>
static_entity_render_plan(
    EntityType type,
    std::int32_t frame,
    Vec2i current_position) noexcept {

    switch (type) {
    case EntityType::Trailer1:
        return StaticEntityRenderPlan{
            current_position, kTrailer1Source, false};
    case EntityType::TravisCab:
        return StaticEntityRenderPlan{
            current_position, travis_cab_source_rect(frame), false};
    case EntityType::Trailer2:
        return StaticEntityRenderPlan{
            {380,364}, kTrailer2Source, true};
    case EntityType::GateLeft:
        return StaticEntityRenderPlan{
            kLeftGateWorldPosition, gate_source_rect(frame), true};
    case EntityType::GateRight:
        return StaticEntityRenderPlan{
            kRightGateWorldPosition, gate_source_rect(frame), true};
    default:
        return std::nullopt;
    }
}

enum class HerdingRenderStage {
    BackgroundViewport,
    DepthSortedEntities,
    ThreeConditionalWorldOverlays,
    UiSurround,
    SelectedFoodBag,
};

inline constexpr std::array<HerdingRenderStage,5>
kHerdingRenderStages{{
    HerdingRenderStage::BackgroundViewport,
    HerdingRenderStage::DepthSortedEntities,
    HerdingRenderStage::ThreeConditionalWorldOverlays,
    HerdingRenderStage::UiSurround,
    HerdingRenderStage::SelectedFoodBag,
}};

inline constexpr std::array<Vec2i,3> kWorldFoodBagPositions{{
    {272,332},
    {365,354},
    {269,394},
}};

struct PostEntityOverlayPlan {
    bool draw_world_food_bags{true};
    bool draw_ui_surround{true};
    std::optional<std::int32_t> selected_food_toolbar_index{};
    Vec2i selected_food_toolbar_position{284,417};
};

[[nodiscard]] constexpr PostEntityOverlayPlan post_entity_overlay_plan(
    FoodType selected_food) noexcept {
    PostEntityOverlayPlan result;
    if (selected_food != FoodType::None) {
        result.selected_food_toolbar_index =
            static_cast<std::int32_t>(selected_food);
    }
    return result;
}

inline constexpr Vec2i kSelectedFoodBagScreenPosition{284,417};

} // namespace btb::herding
