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
    return transition_active ? -(transition_counter / 5) : 0;
}

// 00425780 level-transition globals. Retail initialization selects the
// direct branch (00446A44=1), but the alternate scroll path is retained.
struct LevelTransitionState {
    int phase{1};
    bool direct{true};
    int level{};
    int available_section{};
    int assembly_phase{};
    int squirrel_x{};
    int run_section{};
    int conveyor_phase{};
    int scroll_aux{};
    int scroll_active{};
    int scroll_distance{};
    int scroll_start{};
    int scroll_ticks{};
    int scroll_speed{-1};
    int background_left{};
    int background_right{kBackgroundViewportWidth};
};
struct LevelTransitionFrame {
    bool level_advanced{};
    bool draw_borders{};
    std::optional<SourceRect> background{};
};
[[nodiscard]] constexpr LevelTransitionFrame advance_level_transition(
    LevelTransitionState& s,int inactive_remaining_hint=0) noexcept {
    LevelTransitionFrame result;
    auto advance=[&]() constexpr {
        ++s.level;
        s.scroll_aux=s.scroll_active=s.phase=0;
        s.available_section=s.assembly_phase=0;
        s.run_section=s.conveyor_phase=1;
        result.level_advanced=true;
    };
    if(s.phase==1 && s.direct) {
        advance();s.squirrel_x=46;
        return result; // no background or borders in direct branch
    }
    if(s.phase==1) {
        s.phase=2;s.scroll_distance=0;s.scroll_active=1;
        s.scroll_start=s.background_left;
    }
    int remaining=inactive_remaining_hint;
    if(s.scroll_active>0) {
        ++s.scroll_ticks;
        const int step=s.scroll_speed/5;
        s.background_left+=step;s.background_right+=step;
        remaining=kBackgroundPanelStride+s.scroll_start-s.background_left;
        if(remaining<=0) {
            s.squirrel_x+=scene_scroll_offset(true,s.scroll_distance);
            advance();
        }
        // The completion frame still submits the scrolled backdrop.
        result.background=SourceRect{s.background_left,0,s.background_right,
                                     kBackgroundViewportHeight};
    }
    if(s.scroll_speed<10)++s.scroll_speed;
    if(remaining<s.scroll_speed)s.scroll_speed=remaining;
    if(s.scroll_speed<10)s.scroll_speed=10;
    s.scroll_distance+=s.scroll_speed;
    result.draw_borders=true;
    return result;
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

// Local stack table in 0x004267B0 turns connector values 0..2 into
// all.bmp sprite IDs. Nut pile positioning uses a SEPARATE stack table.
inline constexpr std::array<std::int32_t,3> kConnectorPieceIds{{
    18,4,17,
}};

[[nodiscard]] constexpr std::optional<Vec2i> nut_pile_destination(
    std::int32_t connector) noexcept {
    if (connector < 0 || connector >= 3) {
        return std::nullopt;
    }
    constexpr std::array<int,3> y_offsets{80,50,20};
    return Vec2i{
        kConnectorDestinations.back().x,
        kConnectorDestinations.back().y +
            y_offsets[static_cast<std::size_t>(connector)],
    };
}

// Retail squriel_1_86.bmp is 8772x92: exactly 86 horizontal
// cells of width 102 and height 92. The source assembly draw in
// 0x00425F40 obtains these integers at PE VAs 0x0044677C/0x00446780.
inline constexpr std::int32_t kSourceSquirrelFrameWidth=102;
inline constexpr std::int32_t kSourceSquirrelFrameHeight=92;
inline constexpr std::int32_t kSourceSquirrelFrameCount=86;

// 0x00425160 computes initial DAT_00514F98 from the first source
// connector's 0x00446988 table and 0x004467C0=194. The X baseline
// uses 0x004467BC=36 minus loaded 0x00515084 (retail sqdata=70).
// This helper returns the unadjusted origin; the native driver applies it.
inline constexpr std::array<std::int32_t,3>
kSourceSquirrelStartYOffsets{{40,11,-22}};
inline constexpr std::int32_t kSourceSquirrelStartX=36;
inline constexpr std::int32_t kSourceSquirrelStartY=194;

[[nodiscard]] constexpr std::optional<SourceRect>
source_squirrel_frame_rect(std::int32_t frame) noexcept {
    if(frame<0||frame>=kSourceSquirrelFrameCount)return std::nullopt;
    return SourceRect{
        frame*kSourceSquirrelFrameWidth,0,
        (frame+1)*kSourceSquirrelFrameWidth,
        kSourceSquirrelFrameHeight
    };
}

[[nodiscard]] constexpr std::optional<Vec2i>
source_squirrel_start_position(std::int32_t connector) noexcept {
    if(connector<0||connector>=3)return std::nullopt;
    return Vec2i{
        kSourceSquirrelStartX,
        kSourceSquirrelStartY +
        kSourceSquirrelStartYOffsets[static_cast<std::size_t>(connector)]
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

// 00426A62 divides actor X by 30 for eye frames, 00426B8F
// divides (X+28) by 21 for arm frames. The hook uses the original
// 48x446 BMP with a variable top crop and a fixed exclusive bottom.
[[nodiscard]] constexpr SourceRect lofty_hook_source_rect(int actor_y) noexcept {
    return {0,436-actor_y,47,445};
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
