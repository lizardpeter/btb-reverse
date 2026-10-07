#include "btb/herding_presentation.hpp"

#include <cassert>

using namespace btb::herding;

int main() {
    static_assert(kWorldViewportWidth == 600);
    static_assert(kWorldViewportHeight == 380);
    static_assert(kWorldViewportDestX == 20);
    static_assert(kWorldViewportDestY == 20);

    constexpr auto camera_low = herding_camera_plan(200,100);
    static_assert(camera_low.camera_x == 0);
    static_assert(camera_low.camera_y == 0);
    static_assert(
        camera_low.background_source ==
        RenderRect{0,0,600,380});

    constexpr auto camera_mid = herding_camera_plan(478,471);
    static_assert(camera_mid.camera_x == 178);
    static_assert(camera_mid.camera_y == 361);
    static_assert(
        camera_mid.background_source ==
        RenderRect{178,361,778,741});

    constexpr auto camera_high = herding_camera_plan(1200,800);
    static_assert(camera_high.camera_x == 672);
    static_assert(camera_high.camera_y == 502);
    static_assert(
        camera_high.background_source ==
        RenderRect{672,502,1272,882});

    RetailEntityRecord32 lower{};
    lower.type = static_cast<std::int32_t>(EntityType::Sheep);
    lower.y = 100;
    lower.source_top = 0;
    lower.source_bottom = 103;

    RetailEntityRecord32 higher{};
    higher.type = static_cast<std::int32_t>(EntityType::Rabbit);
    higher.y = 150;
    higher.source_top = 0;
    higher.source_bottom = 69;

    static_assert(entity_sprite_height(RetailEntityRecord32{}) == 0);
    assert(entity_depth_y(lower) == 203);
    assert(entity_depth_y(higher) == 219);
    assert(compare_entities_by_depth(lower,higher) == -1);
    assert(compare_entities_by_depth(higher,lower) == 1);

    // Retail comparator never returns zero for ordinary equal-depth entities.
    higher.y = 134;
    higher.source_bottom = 69;
    assert(entity_depth_y(higher) == 203);
    assert(compare_entities_by_depth(lower,higher) == -1);
    assert(compare_entities_by_depth(higher,lower) == -1);

    RetailEntityRecord32 left_gate{};
    left_gate.type = static_cast<std::int32_t>(EntityType::GateLeft);
    RetailEntityRecord32 right_gate{};
    right_gate.type = static_cast<std::int32_t>(EntityType::GateRight);

    assert(compare_entities_by_depth(left_gate, lower) == -1);
    assert(compare_entities_by_depth(right_gate, higher) == -1);
    assert(compare_entities_by_depth(lower, left_gate) == 1);
    assert(compare_entities_by_depth(higher, right_gate) == 1);

    static_assert(kSheepGeometry.frame_width == 98);
    static_assert(kSheepGeometry.frame_height == 103);
    static_assert(kSheepGeometry.frames_per_direction == 10);
    static_assert(kRabbitGeometry.frame_width == 73);
    static_assert(kRabbitGeometry.frame_height == 69);
    static_assert(kRabbitGeometry.frames_per_direction == 10);
    static_assert(kDuckGeometry.frame_width == 53);
    static_assert(kDuckGeometry.frame_height == 50);
    static_assert(kDuckGeometry.frames_per_direction == 6);
    static_assert(kScrufftyGeometry.frame_width == 86);
    static_assert(kScrufftyGeometry.frame_height == 87);
    static_assert(kScrufftyGeometry.frames_per_direction == 7);

    constexpr auto sheep =
        directional_source_rect(EntityType::Sheep, 2, 6);
    static_assert(sheep);
    static_assert(*sheep == RenderRect{2548,0,2646,103});

    constexpr auto rabbit =
        directional_source_rect(EntityType::Rabbit, 3, 4);
    static_assert(rabbit);
    static_assert(*rabbit == RenderRect{2482,0,2555,69});

    constexpr auto duck =
        directional_source_rect(EntityType::Duck, 1, 5);
    static_assert(duck);
    static_assert(*duck == RenderRect{583,0,636,50});

    constexpr auto scruffty =
        directional_source_rect(EntityType::Scruffty, 7, 3);
    static_assert(scruffty);
    static_assert(*scruffty == RenderRect{4472,0,4558,87});

    static_assert(!directional_source_rect(
        EntityType::FarmerPickles,0,0));
    static_assert(!directional_source_rect(
        EntityType::Sheep,-1,0));

    static_assert(kTrailer1Source == RenderRect{0,0,151,240});
    static_assert(kTrailer2Source == RenderRect{0,0,43,76});

    constexpr auto cab3 = travis_cab_source_rect(3);
    static_assert(cab3 == RenderRect{690,0,920,240});

    static_assert(kGateFrameWidth == 120);
    static_assert(kGateFrameHeight == 70);
    static_assert(kLeftGateWorldPosition == Vec2i{717,162});
    static_assert(kRightGateWorldPosition == Vec2i{859,101});

    constexpr auto gate2 = gate_source_rect(2);
    static_assert(gate2 == RenderRect{240,0,360,70});

    constexpr auto trailer1 = static_entity_render_plan(
        EntityType::Trailer1,0,{123,456});
    static_assert(trailer1);
    static_assert(trailer1->world_position == Vec2i{123,456});
    static_assert(!trailer1->force_world_position);
    static_assert(trailer1->source_rect == kTrailer1Source);

    constexpr auto trailer2 = static_entity_render_plan(
        EntityType::Trailer2,0,{0,0});
    static_assert(trailer2);
    static_assert(trailer2->world_position == Vec2i{380,364});
    static_assert(trailer2->force_world_position);
    static_assert(trailer2->source_rect == kTrailer2Source);

    constexpr auto gate_left = static_entity_render_plan(
        EntityType::GateLeft,2,{0,0});
    static_assert(gate_left);
    static_assert(gate_left->world_position == kLeftGateWorldPosition);
    static_assert(gate_left->source_rect == RenderRect{240,0,360,70});

    constexpr auto gate_right = static_entity_render_plan(
        EntityType::GateRight,1,{0,0});
    static_assert(gate_right);
    static_assert(gate_right->world_position == kRightGateWorldPosition);
    static_assert(gate_right->source_rect == RenderRect{120,0,240,70});

    static_assert(kHerdingRenderStages[0] ==
        HerdingRenderStage::BackgroundViewport);
    static_assert(kHerdingRenderStages[1] ==
        HerdingRenderStage::DepthSortedEntities);
    static_assert(kHerdingRenderStages[2] ==
        HerdingRenderStage::ThreeConditionalWorldOverlays);
    static_assert(kHerdingRenderStages[3] ==
        HerdingRenderStage::UiSurround);
    static_assert(kHerdingRenderStages[4] ==
        HerdingRenderStage::SelectedFoodBag);

    static_assert(kSelectedFoodBagScreenPosition == Vec2i{284,417});
}
