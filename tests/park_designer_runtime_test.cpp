#include "btb/park_designer_runtime.hpp"

#include <cassert>

using namespace btb::park_designer;

int main() {
    static_assert(
        view_toolbar_hover_sound_id(ViewToolbarItem::Summer) == 997);
    static_assert(
        view_toolbar_hover_sound_id(ViewToolbarItem::Winter) == 998);
    static_assert(
        view_toolbar_hover_sound_id(ViewToolbarItem::Print) == 293);
    static_assert(
        view_toolbar_hover_sound_id(ViewToolbarItem::InfoHoverOnly) == 299);

    constexpr auto summer_click =
        view_toolbar_click(ViewToolbarItem::Summer, 0);
    static_assert(summer_click.action == ViewToolbarAction::ApplySummer);
    static_assert(summer_click.season == Season::Summer);
    static_assert(summer_click.sound_id && *summer_click.sound_id == 294);

    constexpr auto summer_click_alt =
        view_toolbar_click(ViewToolbarItem::Summer, 1);
    static_assert(
        summer_click_alt.sound_id && *summer_click_alt.sound_id == 295);

    constexpr auto winter_click =
        view_toolbar_click(ViewToolbarItem::Winter, 1);
    static_assert(winter_click.action == ViewToolbarAction::ApplyWinter);
    static_assert(winter_click.season == Season::Winter);
    static_assert(winter_click.sound_id && *winter_click.sound_id == 298);

    constexpr auto print_click =
        view_toolbar_click(ViewToolbarItem::Print, 0);
    static_assert(
        print_click.action == ViewToolbarAction::PrintCurrentFrame);
    static_assert(!print_click.season);
    static_assert(!print_click.sound_id);

    constexpr auto info_click =
        view_toolbar_click(ViewToolbarItem::InfoHoverOnly, 0);
    static_assert(info_click.action == ViewToolbarAction::None);
    static_assert(!info_click.season);
    static_assert(!info_click.sound_id);

    static_assert(kFountainFrameWidth == 152);
    static_assert(kFountainFramesPerVariant == 13);
    static_assert(kFountainLoopFirstFrame == 8);
    static_assert(kFountainLoopEndExclusive == 13);
    static_assert(kFountainCountdownReset == 9);

    RetailObjectRecord32 fountain{};
    fountain.object_code = 7;
    fountain.bound_variant = 2;
    fountain.visual_frame_or_segment = 4;
    fountain.fountain_phase = 0;
    fountain.fountain_frame_countdown = 1;
    std::int32_t linked_primary_frame = 0;

    auto fountain_step =
        update_fountain_animation(fountain, linked_primary_frame);
    assert(fountain_step.frame_advanced);
    assert(!fountain_step.entered_loop_phase);
    assert(fountain.visual_frame_or_segment == 5);
    assert(fountain.fountain_frame_countdown == 9);
    assert(linked_primary_frame == 1);

    fountain.visual_frame_or_segment = 7;
    fountain.fountain_phase = 0;
    fountain.fountain_frame_countdown = 1;
    linked_primary_frame = 2;
    fountain_step =
        update_fountain_animation(fountain, linked_primary_frame);
    assert(fountain_step.frame_advanced);
    assert(fountain_step.entered_loop_phase);
    assert(fountain.fountain_phase == 1);
    assert(fountain.visual_frame_or_segment == 8);
    // Phase-0 frame >=5 advances once, then the retail phase-transition branch
    // raises a linked primary value below 4 once more.
    assert(linked_primary_frame == 4);

    fountain.visual_frame_or_segment = 12;
    fountain.fountain_phase = 1;
    fountain.fountain_frame_countdown = 1;
    fountain_step =
        update_fountain_animation(fountain, linked_primary_frame);
    assert(fountain_step.frame_advanced);
    assert(fountain.visual_frame_or_segment == 8);
    assert(fountain.fountain_phase == 1);
    assert(fountain.fountain_frame_countdown == 9);

    fountain.bound_variant = 2;
    fountain.visual_frame_or_segment = 8;
    constexpr auto expected_left =
        (2 * kFountainFramesPerVariant + 8) * kFountainFrameWidth;
    const auto source = fountain_source_rect(fountain);
    assert(source.left == expected_left);
    assert(source.top == 0);
    assert(source.right == expected_left + 152);
    assert(source.bottom == 152);

    auto step = completion_step(0, false, 0);
    assert(step.stage == 1);
    assert(step.action == CompletionAction::PlayClosingLine);
    assert(step.sound_id == 246);

    step = completion_step(0, false, 2);
    assert(step.sound_id == 248);

    step = completion_step(1, true, 0);
    assert(step.stage == 1);
    assert(step.action == CompletionAction::None);

    step = completion_step(1, false, 0);
    assert(step.stage == 2);
    assert(step.action == CompletionAction::None);

    step = completion_step(2, false, 0);
    assert(step.action == CompletionAction::SaveUnloadAndExit);

    SaveData data{};
    for (auto& object : data.objects) {
        object.object_code = -1;
    }
    assert(!has_any_placed_object(data));
    data.objects[399].object_code = 7;
    assert(has_any_placed_object(data));
}
