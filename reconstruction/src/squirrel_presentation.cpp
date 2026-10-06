#include "btb/squirrel_presentation.hpp"

namespace btb::squirrel {

std::vector<SceneLayer> scene_layer_order(
    const SceneShellInput& input) {

    std::vector<SceneLayer> layers;
    layers.reserve(18);

    // 0x004267B0 skips the normal level-background BltFast while the
    // inter-level transition/scroll state at 0x005150D8 is nonzero.
    if (!input.level_transition_active) {
        layers.push_back(SceneLayer::LevelBackground);
    }

    // conveyor.bmp is always drawn immediately after the optional background.
    layers.push_back(SceneLayer::ConveyorStrip);

    // During the transition, a nonzero large-Lofty animation state draws the
    // single LOFTYmoves8bit.bmp sprite. Otherwise retail composes Lofty from
    // body + eyes + mouth + arm-swing surfaces.
    if (input.level_transition_active &&
        input.lofty_large_move_active) {
        layers.push_back(SceneLayer::LoftyLargeMove);
    } else {
        layers.push_back(SceneLayer::LoftyBody);
        layers.push_back(SceneLayer::LoftyEyes);
        layers.push_back(SceneLayer::LoftyMouth);
        layers.push_back(SceneLayer::LoftyArmSwing);
    }

    layers.push_back(SceneLayer::ExistingRunPieces);
    layers.push_back(SceneLayer::ConnectorGeometry);

    if (input.draw_nut_pile) {
        layers.push_back(SceneLayer::NutPile);
    }

    if (input.level_transition_active) {
        layers.push_back(SceneLayer::TransitionConnectorGeometry);
    }

    // The live run/squirrel animator is called after all retained run geometry.
    layers.push_back(SceneLayer::LiveRunAssemblyAndSquirrel);

    // The hook is suppressed only while both the level transition is active
    // and the large-Lofty animation state remains nonzero.
    if (!input.level_transition_active ||
        !input.lofty_large_move_active) {
        layers.push_back(SceneLayer::Hook);
    }

    layers.push_back(SceneLayer::ConveyorChoices);

    // These four keyed border surfaces are the final four blits in the scene.
    layers.push_back(SceneLayer::BorderTop);
    layers.push_back(SceneLayer::BorderLeft);
    layers.push_back(SceneLayer::BorderRight);
    layers.push_back(SceneLayer::BorderBottom);

    return layers;
}

} // namespace btb::squirrel
