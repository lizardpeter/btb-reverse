#pragma once

#include "btb/game_flow.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::full_game {

// Retail source: RunMainGameFlow 0x0042A2C0 state handlers.
// These are actual globals in the x86 executable, not newly invented menu
// destinations. Their uses are cross-referenced in the disassembly:
// 0x0044DE14 outer game state, 0x0051C2E4 choice, 0x0051C284 variant,
// 0x0051C2FC replay class, 0x0051B418 saved game state, 0x0050A5BC
// Fireworks edit/view runtime mode.
struct RetailMenuState {
    std::int32_t selected_subgame{};  // original 0x0051C2E4
    std::optional<game_flow::State> subgame_selection_origin{};
    std::int32_t source_variant{};    // original 0x0051C284
    std::optional<game_flow::State> variant_selection_origin{};
    std::int32_t replay_class{};      // original 0x0051C2FC (0/1/2)
    bool replay_active_latch{};     // original 0x0051C300
    std::int32_t fireworks_view_mode{}; // original 0x0050A5BC

    // Three values initialized separately by retail and copied into the
    // selected source_variant when Dino's three species are chosen.
    // Do not replace these with species indices: the native instruction
    // sequence copies 0x51C344 / 0x51C348 / 0x51C34C respectively.
    // These addresses live in the original uninitialized image region and
    // are zero-filled by the PE loader until the Dino pregame updates them.
    std::array<std::optional<std::int32_t>,3> dino_source_variants{{0,0,0}};
};

struct RetailMenuAction {
    bool recognized{};
    std::optional<game_flow::State> next_state{};
    std::optional<std::int32_t> selected_subgame{};
    std::optional<std::int32_t> source_variant{};
    std::optional<std::int32_t> replay_class{};
    std::optional<std::int32_t> fireworks_view_mode{};
    bool clear_replay_active_latch{};
    bool request_contextual_help{};
    bool stop_activity_music{};
    bool restart_fireworks_editor{};
    bool restart_fireworks_view{};
    bool requires_source_variant{};
};

// Full and exact routable action branch table for the four *chooser* update
// states and three *replay* update states. This is only the native state
// mutation and effect policy, not generic UI surface teardown/DirectSound
// completion (owned by the menu host).
//
// Negative action -6 invokes contextual Help in the original generic UI
// runtime and keeps the chooser update state; no invented destination.
// Ordinary Activity Select Back (-1) is separate: it targets the five-sign
// player-profile state 0x01, not state 0x04.
[[nodiscard]] constexpr RetailMenuAction route_retail_menu_action(
    game_flow::State update,
    std::int32_t action,
    const RetailMenuState& current,
    std::int32_t saved_state) noexcept {

    using game_flow::State;
    RetailMenuAction out{};
    if (action == -6) {
        switch (update) {
        case State::DinoChooserUpdate:
        case State::SpudChooserUpdate:
        case State::AdventureChooserUpdate:
        case State::MusicChooserUpdate:
            out.recognized = true;
            out.request_contextual_help = true;
            return out;
        default:
            break;
        }
    }

    if (action == -1) {
        switch (update) {
        case State::DinoChooserUpdate:
        case State::SpudChooserUpdate:
        case State::AdventureChooserUpdate:
        case State::MusicChooserUpdate:
            out.recognized = true;
            out.next_state = State::ActivitySelectSetup;
            return out;
        case State::ActivitySelectUpdate:
            out.recognized = true;
            out.next_state = State::PlayerProfileAndNameEntry;
            return out;
        default:
            break;
        }
    }

    if (update == State::ActivitySelectUpdate && action == -6) {
        out.recognized = true;
        out.request_contextual_help = true;
        return out;
    }

    switch (update) {
    case State::DinoChooserUpdate:
        if (action <= -2 && action >= -4) {
            const auto i = static_cast<std::size_t>(-2-action);
            out.recognized = true;
            out.next_state = State::DinoPregameSetup;
            out.selected_subgame = static_cast<std::int32_t>(i);
            out.source_variant = current.dino_source_variants[i];
            out.requires_source_variant = !out.source_variant.has_value();
        }
        return out;
    case State::SpudChooserUpdate:
        if (action == -2 || action == -3) {
            out.recognized = true;
            out.next_state = action == -2
                ? State::SpudMazePregameSetup : State::SpudSkatePregameSetup;
            out.selected_subgame = action == -2 ? 0 : 1;
        }
        return out;
    case State::AdventureChooserUpdate:
        if (action == -2 || action == -3) {
            out.recognized = true;
            out.next_state = action == -2
                ? State::MazePregameSetup : State::GolfPregameSetup;
            out.selected_subgame = action == -2 ? 0 : 1;
        }
        return out;
    case State::MusicChooserUpdate:
        if (action <= -2 && action >= -4) {
            out.recognized = true;
            out.next_state = State::BobsBandPregameSetup;
            out.selected_subgame = -2-action;
        }
        return out;

    case State::PlayAgainYesNoUpdate:
        if (action == -20) {
            out.recognized = true;
            if (current.replay_class == 1) {
                out.next_state = State::PlayAgainDifficultySetup;
            } else if (current.replay_class == 2) {
                out.next_state = State::FireworksReplayChoiceSetup;
            } else if (game_flow::valid_state_value(saved_state)) {
                out.next_state = static_cast<State>(saved_state);
                out.clear_replay_active_latch = true;
            }
        } else if (action == -21) {
            out.recognized = true;
            out.next_state = State::SharedMovieTransition;
            out.clear_replay_active_latch = true;
        }
        return out;

    case State::PlayAgainDifficultyUpdate:
        if (action <= -30 && action >= -32) {
            out.recognized = true;
            out.stop_activity_music = true;
            out.source_variant = -30-action;
            out.replay_class = 0;
            out.clear_replay_active_latch = true;
            if (game_flow::valid_state_value(saved_state)) {
                out.next_state = static_cast<State>(saved_state);
            }
        }
        return out;

    case State::FireworksReplayChoiceUpdate:
        if (action == -20 || action == -21) {
            out.recognized = true;
            out.next_state = State::FireworksRun;
            out.clear_replay_active_latch = true;
            out.fireworks_view_mode = action == -20 ? 0 : 8;
            out.restart_fireworks_editor = action == -20;
            out.restart_fireworks_view = action == -21;
        }
        return out;

    default:
        return out;
    }
}

constexpr void apply_retail_menu_action(
    RetailMenuState& state,
    game_flow::State update,
    const RetailMenuAction& action) noexcept {

    if (action.selected_subgame) {
        state.selected_subgame = *action.selected_subgame;
        state.subgame_selection_origin = update;
    }
    if (action.source_variant) {
        state.source_variant = *action.source_variant;
        state.variant_selection_origin = update;
    }
    if (action.replay_class) {
        state.replay_class = *action.replay_class;
    }
    if (action.fireworks_view_mode) {
        state.fireworks_view_mode = *action.fireworks_view_mode;
    }
}

} // namespace btb::full_game
