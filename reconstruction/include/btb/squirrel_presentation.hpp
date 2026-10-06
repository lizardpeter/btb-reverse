#pragma once

#include "btb/squirrel_data.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

namespace btb::squirrel {

struct SourceRect {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
    friend bool operator==(const SourceRect&, const SourceRect&) = default;
};

enum class SceneSurface {
    LevelBackground,
    AssembledRunSheet,
    ConveyorChoiceSheet,
    Hook,
    Eyes,
    Mouth,
    LoftyBody,
    LoftyArmSwing,
    SquirrelSheet,
    BorderTop,
    BorderLeft,
    BorderRight,
    BorderBottom,
    ConveyorStrip,
    LoftyMoves,
    NutPile,
};

struct RetailSurfaceBinding {
    SceneSurface surface{};
    std::uint32_t global_address{};
    std::string_view filename{};
    bool color_keyed{};
};

inline constexpr RetailSurfaceBinding kSurfaceBindings[] = {
    {SceneSurface::LevelBackground, 0x0051509C,
     "Data\\SubGameSquirrel\\level{1|2|3}.bmp", false},
    {SceneSurface::AssembledRunSheet, 0x005150A0,
     "Data\\SubGameSquirrel\\all.bmp", true},
    {SceneSurface::ConveyorChoiceSheet, 0x005150A4,
     "Data\\SubGameSquirrel\\allconveyor.bmp", true},
    {SceneSurface::Hook, 0x005150A8,
     "Data\\SubGameSquirrel\\hook.bmp", true},
    {SceneSurface::Eyes, 0x005150AC,
     "Data\\SubGameSquirrel\\eyes.bmp", true},
    {SceneSurface::Mouth, 0x005150B0,
     "Data\\SubGameSquirrel\\mouth.bmp", true},
    {SceneSurface::LoftyBody, 0x005150B4,
     "Data\\SubGameSquirrel\\body_01.bmp", true},
    {SceneSurface::LoftyArmSwing, 0x005150B8,
     "Data\\SubGameSquirrel\\loftyarmswing_8bit.bmp", true},
    {SceneSurface::SquirrelSheet, 0x005150BC,
     "Data\\SubGameSquirrel\\squriel_1_86.bmp", true},
    {SceneSurface::BorderTop, 0x00515068,
     "Data\\SubGameSquirrel\\top.bmp", true},
    {SceneSurface::BorderLeft, 0x0051506C,
     "Data\\SubGameSquirrel\\left.bmp", true},
    {SceneSurface::BorderRight, 0x00515070,
     "Data\\SubGameSquirrel\\right.bmp", true},
    {SceneSurface::BorderBottom, 0x00515074,
     "Data\\SubGameSquirrel\\bottom.bmp", true},
    {SceneSurface::ConveyorStrip, 0x005150C0,
     "Data\\SubGameSquirrel\\conveyor.bmp", true},
    {SceneSurface::LoftyMoves, 0x005150C4,
     "Data\\SubGameSquirrel\\LOFTYmoves8bit.bmp", true},
    {SceneSurface::NutPile, 0x005150C8,
     "Data\\SubGameSquirrel\\nutpile.bmp", true},
};

[[nodiscard]] constexpr const RetailSurfaceBinding*
surface_binding(SceneSurface surface) noexcept {
    for (const auto& binding : kSurfaceBindings) {
        if (binding.surface == surface) {
            return &binding;
        }
    }
    return nullptr;
}

inline constexpr std::int32_t kBackgroundPanelStride = 480;
inline constexpr std::int32_t kBackgroundViewportWidth = 620;
inline constexpr std::int32_t kBackgroundViewportHeight = 480;

[[nodiscard]] constexpr SourceRect level_background_source_rect(
    std::int32_t level_index) noexcept {
    const auto left = level_index * kBackgroundPanelStride;
    return {
        left,
        0,
        left + kBackgroundViewportWidth,
        kBackgroundViewportHeight,
    };
}

// When level-transition state is nonzero, 0x004267B0 computes this with the
// signed divide-by-5 magic-multiply sequence. C++ integer division has the same
// truncate-toward-zero semantics for int32_t.
[[nodiscard]] constexpr std::int32_t scene_scroll_offset(
    bool transition_active,
    std::int32_t transition_counter) noexcept {
    return transition_active ? transition_counter / 5 : 0;
}

inline constexpr std::array<Vec2i,3> kCompletedRunPieceDestinations{{
    {116,194},
    {276,194},
    {436,194},
}};

inline constexpr std::array<Vec2i,4> kConnectorDestinations{{
    {36,194},
    {196,194},
    {356,194},
    {516,194},
}};

// Local stack table in 0x004267B0 used to turn connector values 0..2 into
// all.bmp sprite IDs. The final-target nut-pile Y offset reuses the same three
// integer values directly.
inline constexpr std::array<std::int32_t,3> kConnectorPieceIds{{
    18,4,17,
}};

[[nodiscard]] constexpr std::optional<Vec2i> nut_pile_destination(
    std::int32_t connector) noexcept {
    if (connector < 0 || connector >= 3) {
        return std::nullopt;
    }
    return Vec2i{
        kConnectorDestinations.back().x,
        kConnectorDestinations.back().y +
            kConnectorPieceIds[static_cast<std::size_t>(connector)],
    };
}

inline constexpr std::int32_t kConveyorFrameWidth = 640;
inline constexpr std::int32_t kConveyorFrameHeight = 44;
inline constexpr std::int32_t kConveyorDestX = 0;
inline constexpr std::int32_t kConveyorDestY = 417;

[[nodiscard]] constexpr SourceRect conveyor_source_rect(
    std::int32_t frame) noexcept {
    return {
        frame * kConveyorFrameWidth,
        0,
        (frame + 1) * kConveyorFrameWidth,
        kConveyorFrameHeight,
    };
}

inline constexpr std::int32_t kLoftyMovesFrameWidth = 463;
inline constexpr std::int32_t kLoftyMovesBandHeight = 321;
inline constexpr std::int32_t kLoftyMovesDestX = 110;
inline constexpr std::int32_t kLoftyMovesDestY = 0;

// LOFTYmoves8bit.bmp is three vertical 321-pixel bands. Retail animation
// frames 0..20 use band 0, 21..40 use band 1, and 41..59 use band 2.
[[nodiscard]] constexpr std::optional<SourceRect> lofty_moves_source_rect(
    std::int32_t frame) noexcept {

    if (frame < 0 || frame >= 60) {
        return std::nullopt;
    }

    std::int32_t local_frame{};
    std::int32_t band{};

    if (frame < 21) {
        local_frame = frame;
        band = 0;
    } else if (frame < 41) {
        local_frame = frame - 21;
        band = 1;
    } else {
        local_frame = frame - 41;
        band = 2;
    }

    return SourceRect{
        local_frame * kLoftyMovesFrameWidth,
        band * kLoftyMovesBandHeight,
        (local_frame + 1) * kLoftyMovesFrameWidth,
        (band + 1) * kLoftyMovesBandHeight,
    };
}

inline constexpr std::int32_t kLoftyBodyX = 231;
inline constexpr std::int32_t kLoftyBodyY = 160;
inline constexpr std::int32_t kLoftyEyesX = 402;
inline constexpr std::int32_t kLoftyEyesY = 200;
inline constexpr std::int32_t kLoftyMouthX = 405;
inline constexpr std::int32_t kLoftyMouthY = 232;
inline constexpr std::int32_t kLoftyArmSwingX = 196;
inline constexpr std::int32_t kLoftyArmSwingY = 0;

inline constexpr std::int32_t kEyesFrameWidth = 128;
inline constexpr std::int32_t kEyesFrameHeight = 35;
inline constexpr std::int32_t kMouthFrameWidth = 120;
inline constexpr std::int32_t kMouthFrameHeight = 47;
inline constexpr std::int32_t kArmSwingFrameWidth = 219;
inline constexpr std::int32_t kArmSwingFrameHeight = 192;

[[nodiscard]] constexpr SourceRect eyes_source_rect(
    std::int32_t frame) noexcept {
    return {
        frame * kEyesFrameWidth,
        0,
        (frame + 1) * kEyesFrameWidth,
        kEyesFrameHeight,
    };
}

[[nodiscard]] constexpr SourceRect mouth_source_rect(
    std::int32_t frame) noexcept {
    return {
        frame * kMouthFrameWidth,
        0,
        (frame + 1) * kMouthFrameWidth,
        kMouthFrameHeight,
    };
}

[[nodiscard]] constexpr SourceRect arm_swing_source_rect(
    std::int32_t frame) noexcept {
    return {
        frame * kArmSwingFrameWidth,
        0,
        (frame + 1) * kArmSwingFrameWidth,
        kArmSwingFrameHeight,
    };
}

inline constexpr std::int32_t kRunPieceFrameWidth = 89;
inline constexpr std::int32_t kRunPieceFrameHeight = 133;

// Exact 0x00425980 source-cell layout. Both all.bmp and allconveyor.bmp store
// the 36 pieces connector-pair-major, so retail first transposes the normal
// outer-major piece ID and then uses 89-pixel horizontal cells.
[[nodiscard]] constexpr std::int32_t run_piece_sheet_index(
    std::int32_t piece_id) noexcept {
    if (piece_id < 0 || piece_id >= 36) {
        return -1;
    }
    return piece_id / 9 + 4 * (piece_id % 9);
}

[[nodiscard]] constexpr std::optional<SourceRect> run_piece_source_rect(
    std::int32_t piece_id) noexcept {
    const auto index = run_piece_sheet_index(piece_id);
    if (index < 0) {
        return std::nullopt;
    }
    const auto left = index * kRunPieceFrameWidth;
    return SourceRect{
        left,
        0,
        left + kRunPieceFrameWidth,
        kRunPieceFrameHeight,
    };
}

enum class SceneLayer {
    LevelBackground,
    ConveyorStrip,
    LoftyLargeMove,
    LoftyBody,
    LoftyEyes,
    LoftyMouth,
    LoftyArmSwing,
    ExistingRunPieces,
    ConnectorGeometry,
    NutPile,
    TransitionConnectorGeometry,
    LiveRunAssemblyAndSquirrel,
    Hook,
    ConveyorChoices,
    BorderTop,
    BorderLeft,
    BorderRight,
    BorderBottom,
};

struct SceneShellInput {
    bool level_transition_active{};
    bool lofty_large_move_active{};
    bool draw_nut_pile{};
};

[[nodiscard]] std::vector<SceneLayer> scene_layer_order(
    const SceneShellInput& input);

struct BorderDraw {
    SceneSurface surface{};
    std::int32_t x{};
    std::int32_t y{};
};

inline constexpr BorderDraw kFinalBorderDraws[] = {
    {SceneSurface::BorderTop, 0, 0},
    {SceneSurface::BorderLeft, 0, 20},
    {SceneSurface::BorderRight, 620, 20},
    {SceneSurface::BorderBottom, 20, 400},
};

} // namespace btb::squirrel
