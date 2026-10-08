#pragma once

#include "btb/game_flow.hpp"
#include "btb/front_end_ui.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::full_game {

// All ten native pregame setup/run pairs. The generic screen is proven by
// writes to 0x51C27C immediately before LoadGenericUIScreenResources.
// The walkthrough Bink runtime is separate and still needs a real host.
struct RetailPregamePair {
    game_flow::State setup{};
    game_flow::State update{};
    game_flow::State activity_init{};
    game_flow::State back{};
    front_end::Screen screen{};
    bool selected_difficulty{};
    bool starts_global_movie_mode14{};
};

inline constexpr std::array<RetailPregamePair,10> kRetailPregamePairs{{
    {game_flow::State::HerdingPregameSetup,
     game_flow::State::HerdingPregameUpdate,
     game_flow::State::HerdingInit,
     game_flow::State::ActivitySelectSetup,
     front_end::Screen::InstructionWithDifficulty,true,true},
    {game_flow::State::DinoPregameSetup,
     game_flow::State::DinoPregameUpdate,
     game_flow::State::DinoInit,
     game_flow::State::DinoChooserSetup,
     front_end::Screen::InstructionWithDifficulty,true,false},
    {game_flow::State::SpudSkatePregameSetup,
     game_flow::State::SpudSkatePregameUpdate,
     game_flow::State::SpudSkateInit,
     game_flow::State::SpudChooserSetup,
     front_end::Screen::InstructionNoDifficulty,false,false},
    {game_flow::State::MazePregameSetup,
     game_flow::State::MazePregameUpdate,
     game_flow::State::MazeInit,
     game_flow::State::AdventureChooserSetup,
     front_end::Screen::InstructionWithDifficulty,true,false},
    {game_flow::State::FireworksPregameSetup,
     game_flow::State::FireworksPregameUpdate,
     game_flow::State::FireworksInit,
     game_flow::State::ActivitySelectSetup,
     front_end::Screen::FireworksInstruction,false,true},
    {game_flow::State::SquirrelPregameSetup,
     game_flow::State::SquirrelPregameUpdate,
     game_flow::State::SquirrelInit,
     game_flow::State::ActivitySelectSetup,
     front_end::Screen::InstructionWithDifficulty,true,true},
    {game_flow::State::BobsBandPregameSetup,
     game_flow::State::BobsBandPregameUpdate,
     game_flow::State::BobsBandInit,
     game_flow::State::MusicChooserSetup,
     front_end::Screen::InstructionNoDifficulty,false,false},
    {game_flow::State::ParkDesignerPregameSetup,
     game_flow::State::ParkDesignerPregameUpdate,
     game_flow::State::ParkDesignerInit,
     game_flow::State::ActivitySelectSetup,
     front_end::Screen::InstructionNoDifficulty,false,true},
    {game_flow::State::GolfPregameSetup,
     game_flow::State::GolfPregameUpdate,
     game_flow::State::GolfInit,
     game_flow::State::AdventureChooserSetup,
     front_end::Screen::InstructionWithDifficulty,true,false},
    {game_flow::State::SpudMazePregameSetup,
     game_flow::State::SpudMazePregameUpdate,
     game_flow::State::SpudMazeInit,
     game_flow::State::SpudChooserSetup,
     front_end::Screen::InstructionWithDifficulty,true,false},
}};

[[nodiscard]] constexpr const RetailPregamePair*
retail_pregame_for_state(game_flow::State state) noexcept {
    for (const auto& entry : kRetailPregamePairs) {
        if (state == entry.setup || state == entry.update) {
            return &entry;
        }
    }
    return nullptr;
}

// Argument passed to the independent 0x4281D0 OpenWalkthroughMovie.
// The Spud and Adventure groups calculate these from original global
// 0x51C2E4, not from the outer state number or difficulty. No synthetic
// Bink filenames are constructed here; the retail binkwalk table owns them.
[[nodiscard]] constexpr std::optional<int> retail_walkthrough_index(
    game_flow::State setup,
    int selected_subgame) noexcept {
    using game_flow::State;
    switch (setup) {
    case State::HerdingPregameSetup: return 0;
    case State::DinoPregameSetup: return 1;
    case State::SpudSkatePregameSetup:
    case State::SpudMazePregameSetup:
        return selected_subgame >= 0 && selected_subgame <= 1
            ? std::optional<int>{2 + selected_subgame} : std::nullopt;
    case State::MazePregameSetup:
    case State::GolfPregameSetup:
        return selected_subgame >= 0 && selected_subgame <= 1
            ? std::optional<int>{4 + selected_subgame} : std::nullopt;
    case State::FireworksPregameSetup: return 6;
    case State::SquirrelPregameSetup: return 7;
    case State::BobsBandPregameSetup: return 8;
    case State::ParkDesignerPregameSetup: return 9;
    default: return std::nullopt;
    }
}

// Original pregame bitmap name table at 0x490770 uses 256-byte
// records. The native setup passes 0x490770 + slot*0x100 to
// LoadBitmapToDirectDrawSurface (0x4038D0), so the resulting slot indexes
// loaddata/uiBitmapName.txt exactly. Spud/Adventure/Dino options add
// 0x100 * original 0x51C2E4 selected_subgame.
[[nodiscard]] constexpr std::optional<std::size_t>
retail_pregame_backdrop_index(
    game_flow::State setup,
    int selected_subgame) noexcept {
    using game_flow::State;
    switch (setup) {
    case State::HerdingPregameSetup: return 4;
    case State::DinoPregameSetup:
        return selected_subgame >= 0 && selected_subgame <= 2
            ? std::optional<std::size_t>{static_cast<std::size_t>(
                  5 + selected_subgame)} : std::nullopt;
    case State::SpudSkatePregameSetup:
    case State::SpudMazePregameSetup:
        return selected_subgame >= 0 && selected_subgame <= 1
            ? std::optional<std::size_t>{static_cast<std::size_t>(
                  10 + selected_subgame)} : std::nullopt;
    case State::MazePregameSetup:
    case State::GolfPregameSetup:
        return selected_subgame >= 0 && selected_subgame <= 1
            ? std::optional<std::size_t>{static_cast<std::size_t>(
                  13 + selected_subgame)} : std::nullopt;
    case State::FireworksPregameSetup: return 15;
    case State::SquirrelPregameSetup: return 16;
    case State::BobsBandPregameSetup: return 17;
    case State::ParkDesignerPregameSetup: return 18;
    default: return std::nullopt;
    }
}

// State effects directly recovered from all ten 6-entry action+6 jump tables:
// -6 Help/no-transition, -5 Start, -4 Hard, -3 Medium, -2 Easy or
// screen-specific Start, -1 Back. Values outside that interval never jump.
struct RetailPregameAction {
    bool recognized{};
    std::optional<game_flow::State> next_state{};
    std::optional<std::int32_t> difficulty{};
    bool persistent_herding_difficulty{};
    bool skate_immediate_start{};
    bool help_retains_screen{};
    bool start_activity{};
    bool leaves_to_parent{};
};

[[nodiscard]] constexpr RetailPregameAction route_retail_pregame_action(
    game_flow::State update,
    std::int32_t action) noexcept {

    const auto* pair = retail_pregame_for_state(update);
    if (!pair || pair->update != update || action > -1 || action < -6) {
        return {};
    }
    RetailPregameAction result{};
    result.recognized = true;

    switch (action) {
    case -1:
        result.next_state = pair->back;
        result.leaves_to_parent = true;
        return result;
    case -6:
        result.help_retains_screen = true;
        return result;
    case -5:
        result.next_state = pair->activity_init;
        result.start_activity = true;
        return result;
    case -2:
        // Retail 0x42B6AD: Skate's no-difficulty Start button jumps
        // directly to 0x1A while setting its per-activity start latch.
        if (update == game_flow::State::SpudSkatePregameUpdate) {
            result.next_state = pair->activity_init;
            result.difficulty = 0;
            result.skate_immediate_start = true;
            result.start_activity = true;
            return result;
        }
        [[fallthrough]];
    case -3:
    case -4:
        result.difficulty = -2 - action;
        result.persistent_herding_difficulty =
            update == game_flow::State::HerdingPregameUpdate;
        // Other no-difficulty menus (Band/Park/Fireworks) STILL write
        // a 0/1/2 difficulty value and stay in the instruction update
        // state for -2; do not make up a direct launch that retail
        // does not perform.
        return result;
    }
    return {};
}

} // namespace btb::full_game
