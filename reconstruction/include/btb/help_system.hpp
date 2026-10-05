#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace btb::help {

enum class Context : std::int32_t {
    EnterNameScreen = 0,
    EnterNamePopup = 1,
    ActivitySelect = 2,
    SpudChooser = 3,
    DinoChooser = 4,
    AdventureChooser = 5,
    MusicChooser = 6,
    PregameNoDifficulty = 7,
    PregameWithDifficulty = 8,
    ParkDesigner = 9,
    SpudMaze = 10,
    SpudSkate = 11,
    Dino = 12,
    Squirrel = 13,
    Maze = 14,
    Golf = 15,
    BobsBand = 16,
    PetsCorner = 17,
    Fireworks = 18,
    ProgressScreen = 19,
};

inline constexpr std::int32_t kNoHelpContext = -1;
inline constexpr std::int32_t kDynamicProfileContext = -99;

// Exact table constructed on the stack by 0x004011B0.
// Index is the 68-state outer game-flow value at 0x0044DE14.
inline constexpr std::array<std::int32_t, 68> kGameFlowHelpContext{{
    -1, -99, -1, -1, -1,  2, -1, -1,
    -1,  -1, -1, -1, -1,  8, -1, 17,
    -1,   4, -1,  8, -1, 12, -1,  3,
    -1,   7, -1, 11, -1,  5, -1,  8,
    -1,  14, -1,  7, -1, 18, -1,  8,
    -1,  13, -1,  6, -1,  7, -1, 16,
    -1,   7, -1,  9, -1,  8, -1, 15,
    -1,   8, -1, 10, -1, -1, -1, -1,
    -1,  -1, -1,  0,
}};

struct EntryDecision {
    bool active{};
    std::int32_t context{kNoHelpContext};
};

[[nodiscard]] constexpr EntryDecision enter_decision(
    std::int32_t outer_game_flow_state,
    bool profile_popup_active,
    bool progress_screen_active) noexcept {

    if (outer_game_flow_state < 0 ||
        outer_game_flow_state >=
            static_cast<std::int32_t>(kGameFlowHelpContext.size())) {
        return {false, kNoHelpContext};
    }

    auto context =
        kGameFlowHelpContext[static_cast<std::size_t>(outer_game_flow_state)];

    bool active = true;

    if (context == kDynamicProfileContext) {
        context = profile_popup_active
            ? static_cast<std::int32_t>(Context::EnterNamePopup)
            : static_cast<std::int32_t>(Context::EnterNameScreen);
    } else if (context == kNoHelpContext) {
        active = false;
    }

    // This override occurs after the retail unsupported-context check.
    // It changes the selected context but does not re-enable a help mode that
    // was already cancelled by a -1 outer-state entry.
    if (progress_screen_active) {
        context = static_cast<std::int32_t>(Context::ProgressScreen);
    }

    return {active, context};
}

struct Region {
    std::int32_t left{};
    std::int32_t top{};
    std::int32_t right{};
    std::int32_t bottom{};
    std::int32_t sound_id{};
};

[[nodiscard]] constexpr bool cursor_inside_region_strict(
    const Region& region,
    std::int32_t x,
    std::int32_t y) noexcept {
    return x > region.left && x < region.right &&
           y > region.top && y < region.bottom;
}

enum class ActivationPhase : std::int32_t {
    Idle = 0,
    Pressed = 1,
    ReadyToEnter = 2,
};

inline constexpr std::int32_t kHelpIntroSoundId = 476; // GENH_WEN_01.wav
inline constexpr std::int32_t kHelpExitCursorDelay = 50;

} // namespace btb::help
