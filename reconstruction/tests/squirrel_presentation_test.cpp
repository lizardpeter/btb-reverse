#include "btb/squirrel_presentation.hpp"

#include <cassert>

using namespace btb::squirrel;

int main() {
    static_assert(std::size(kSurfaceBindings) == 16);

    constexpr auto* background =
        surface_binding(SceneSurface::LevelBackground);
    static_assert(background);
    static_assert(background->global_address == 0x0051509C);
    static_assert(!background->color_keyed);

    constexpr auto* all =
        surface_binding(SceneSurface::AssembledRunSheet);
    static_assert(all);
    static_assert(all->global_address == 0x005150A0);
    static_assert(all->filename ==
        "Data\\SubGameSquirrel\\all.bmp");
    static_assert(all->color_keyed);

    constexpr auto* conveyor_choices =
        surface_binding(SceneSurface::ConveyorChoiceSheet);
    static_assert(conveyor_choices);
    static_assert(conveyor_choices->global_address == 0x005150A4);
    static_assert(conveyor_choices->filename ==
        "Data\\SubGameSquirrel\\allconveyor.bmp");

    constexpr auto* hook =
        surface_binding(SceneSurface::Hook);
    static_assert(hook && hook->global_address == 0x005150A8);

    constexpr auto* eyes =
        surface_binding(SceneSurface::Eyes);
    static_assert(eyes && eyes->global_address == 0x005150AC);

    constexpr auto* mouth =
        surface_binding(SceneSurface::Mouth);
    static_assert(mouth && mouth->global_address == 0x005150B0);

    constexpr auto* body =
        surface_binding(SceneSurface::LoftyBody);
    static_assert(body && body->global_address == 0x005150B4);

    constexpr auto* arm =
        surface_binding(SceneSurface::LoftyArmSwing);
    static_assert(arm && arm->global_address == 0x005150B8);

    constexpr auto* squirrel =
        surface_binding(SceneSurface::SquirrelSheet);
    static_assert(squirrel && squirrel->global_address == 0x005150BC);

    constexpr auto* conveyor =
        surface_binding(SceneSurface::ConveyorStrip);
    static_assert(conveyor);
    static_assert(conveyor->global_address == 0x005150C0);
    static_assert(conveyor->filename ==
        "Data\\SubGameSquirrel\\conveyor.bmp");

    constexpr auto* moves =
        surface_binding(SceneSurface::LoftyMoves);
    static_assert(moves);
    static_assert(moves->global_address == 0x005150C4);
    static_assert(moves->filename ==
        "Data\\SubGameSquirrel\\LOFTYmoves8bit.bmp");

    constexpr auto* nut =
        surface_binding(SceneSurface::NutPile);
    static_assert(nut && nut->global_address == 0x005150C8);

    static_assert(kConveyorFrameWidth == 640);
    static_assert(kConveyorFrameHeight == 44);
    static_assert(kConveyorDestX == 0);
    static_assert(kConveyorDestY == 417);

    constexpr auto conveyor0 = conveyor_source_rect(0);
    static_assert(conveyor0 == SourceRect{0,0,640,44});
    constexpr auto conveyor3 = conveyor_source_rect(3);
    static_assert(conveyor3 == SourceRect{1920,0,2560,44});

    static_assert(kLoftyMovesFrameWidth == 463);
    static_assert(kLoftyMovesBandHeight == 321);
    static_assert(kLoftyMovesDestX == 110);
    static_assert(kLoftyMovesDestY == 0);

    constexpr auto lofty0 = lofty_moves_source_rect(0);
    static_assert(lofty0);
    static_assert(*lofty0 == SourceRect{0,0,463,321});

    constexpr auto lofty20 = lofty_moves_source_rect(20);
    static_assert(lofty20);
    static_assert(*lofty20 ==
        SourceRect{20*463,0,21*463,321});

    constexpr auto lofty21 = lofty_moves_source_rect(21);
    static_assert(lofty21);
    static_assert(*lofty21 == SourceRect{0,321,463,642});

    constexpr auto lofty40 = lofty_moves_source_rect(40);
    static_assert(lofty40);
    static_assert(*lofty40 ==
        SourceRect{19*463,321,20*463,642});

    constexpr auto lofty41 = lofty_moves_source_rect(41);
    static_assert(lofty41);
    static_assert(*lofty41 == SourceRect{0,642,463,963});

    constexpr auto lofty59 = lofty_moves_source_rect(59);
    static_assert(lofty59);
    static_assert(*lofty59 ==
        SourceRect{18*463,642,19*463,963});

    static_assert(!lofty_moves_source_rect(-1));
    static_assert(!lofty_moves_source_rect(60));

    static_assert(kLoftyBodyX == 231);
    static_assert(kLoftyBodyY == 160);
    static_assert(kLoftyEyesX == 402);
    static_assert(kLoftyEyesY == 200);
    static_assert(kLoftyMouthX == 405);
    static_assert(kLoftyMouthY == 232);
    static_assert(kLoftyArmSwingX == 196);
    static_assert(kLoftyArmSwingY == 0);

    static_assert(eyes_source_rect(2) ==
        SourceRect{256,0,384,35});
    static_assert(mouth_source_rect(2) ==
        SourceRect{240,0,360,47});
    static_assert(arm_swing_source_rect(3) ==
        SourceRect{657,0,876,192});

    static_assert(run_piece_source_rect(0) ==
        SourceRect{0,0,90,134});
    static_assert(run_piece_source_rect(35) ==
        SourceRect{3150,0,3240,134});

    static_assert(kFinalBorderDraws[0].surface ==
        SceneSurface::BorderTop);
    static_assert(kFinalBorderDraws[0].x == 0);
    static_assert(kFinalBorderDraws[0].y == 0);
    static_assert(kFinalBorderDraws[1].surface ==
        SceneSurface::BorderLeft);
    static_assert(kFinalBorderDraws[1].x == 0);
    static_assert(kFinalBorderDraws[1].y == 20);
    static_assert(kFinalBorderDraws[2].surface ==
        SceneSurface::BorderRight);
    static_assert(kFinalBorderDraws[2].x == 620);
    static_assert(kFinalBorderDraws[2].y == 20);
    static_assert(kFinalBorderDraws[3].surface ==
        SceneSurface::BorderBottom);
    static_assert(kFinalBorderDraws[3].x == 20);
    static_assert(kFinalBorderDraws[3].y == 400);

    // Normal scene: background + static Lofty composite + run + hook +
    // conveyor choices + frame borders.
    const auto normal = scene_layer_order({
        false, // transition inactive
        false, // no large Lofty move
        true,  // target level -> nutpile
    });

    const std::vector<SceneLayer> expected_normal{
        SceneLayer::LevelBackground,
        SceneLayer::ConveyorStrip,
        SceneLayer::LoftyBody,
        SceneLayer::LoftyEyes,
        SceneLayer::LoftyMouth,
        SceneLayer::LoftyArmSwing,
        SceneLayer::ExistingRunPieces,
        SceneLayer::ConnectorGeometry,
        SceneLayer::NutPile,
        SceneLayer::LiveRunAssemblyAndSquirrel,
        SceneLayer::Hook,
        SceneLayer::ConveyorChoices,
        SceneLayer::BorderTop,
        SceneLayer::BorderLeft,
        SceneLayer::BorderRight,
        SceneLayer::BorderBottom,
    };
    assert(normal == expected_normal);

    // Inter-level transition with a live large Lofty animation suppresses the
    // ordinary background, static Lofty pieces, and hook.
    const auto moving = scene_layer_order({
        true,
        true,
        false,
    });

    const std::vector<SceneLayer> expected_moving{
        SceneLayer::ConveyorStrip,
        SceneLayer::LoftyLargeMove,
        SceneLayer::ExistingRunPieces,
        SceneLayer::ConnectorGeometry,
        SceneLayer::TransitionConnectorGeometry,
        SceneLayer::LiveRunAssemblyAndSquirrel,
        SceneLayer::ConveyorChoices,
        SceneLayer::BorderTop,
        SceneLayer::BorderLeft,
        SceneLayer::BorderRight,
        SceneLayer::BorderBottom,
    };
    assert(moving == expected_moving);

    // Transition active but large animation state idle: retail returns to the
    // layered body/eyes/mouth/arms and draws the hook again.
    const auto transition_idle = scene_layer_order({
        true,
        false,
        true,
    });

    assert(transition_idle.front() == SceneLayer::ConveyorStrip);
    assert(transition_idle[1] == SceneLayer::LoftyBody);
    assert(transition_idle[2] == SceneLayer::LoftyEyes);
    assert(transition_idle[3] == SceneLayer::LoftyMouth);
    assert(transition_idle[4] == SceneLayer::LoftyArmSwing);
    assert(transition_idle[7] == SceneLayer::NutPile);
    assert(transition_idle[8] ==
        SceneLayer::TransitionConnectorGeometry);
    assert(transition_idle[10] == SceneLayer::Hook);
    assert(transition_idle[11] == SceneLayer::ConveyorChoices);
    assert(transition_idle.back() == SceneLayer::BorderBottom);
}
