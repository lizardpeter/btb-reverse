#include "btb/full_game_menu_actions.hpp"

#include <cassert>

using namespace btb::full_game;
using btb::game_flow::State;

int main() {
    RetailMenuState initial{};

    // Four original chooser jump-table lookups at
    // 0x42CE94, 0x42CEC4, 0x42CF0C, 0x42CF84.
    const auto back = route_retail_menu_action(
        State::MusicChooserUpdate,-1,initial,0);
    assert(back.recognized);
    assert(back.next_state == State::ActivitySelectSetup);
    assert(!back.selected_subgame);
    assert(!back.source_variant);

    const auto main_back = route_retail_menu_action(
        State::ActivitySelectUpdate,-1,initial,0);
    assert(main_back.next_state == State::PlayerProfileAndNameEntry);

    for (const auto source : {
             State::DinoChooserUpdate,State::MusicChooserUpdate,
             State::SpudChooserUpdate,State::AdventureChooserUpdate}) {
        const auto help = route_retail_menu_action(
            source,-6,initial,0);
        assert(help.recognized);
        assert(help.request_contextual_help);
        assert(!help.next_state);
        assert(!help.selected_subgame);
    }

    for (int selected=0; selected<3; ++selected) {
        auto dino = initial;
        dino.dino_source_variants[static_cast<std::size_t>(selected)] =
            100 + selected;
        auto choice = route_retail_menu_action(
            State::DinoChooserUpdate,-2-selected,dino,0);
        assert(choice.recognized);
        assert(choice.next_state == State::DinoPregameSetup);
        assert(choice.selected_subgame == selected);
        assert(choice.source_variant == 100+selected);
        assert(!choice.requires_source_variant);
        apply_retail_menu_action(dino,State::DinoChooserUpdate,choice);
        assert(dino.selected_subgame == selected);
        assert(dino.source_variant == 100+selected);
        assert(dino.variant_selection_origin == State::DinoChooserUpdate);

        auto band = initial;
        auto music = route_retail_menu_action(
            State::MusicChooserUpdate,-2-selected,band,0);
        assert(music.next_state == State::BobsBandPregameSetup);
        assert(music.selected_subgame == selected);
        assert(!music.source_variant);
        apply_retail_menu_action(band,State::MusicChooserUpdate,music);
        assert(band.selected_subgame == selected);
        assert(band.subgame_selection_origin == State::MusicChooserUpdate);
    }
    auto missing_native_variant = initial;
    missing_native_variant.dino_source_variants[0].reset();
    assert(route_retail_menu_action(
        State::DinoChooserUpdate,-2,missing_native_variant,0)
        .requires_source_variant);

    assert(route_retail_menu_action(
        State::SpudChooserUpdate,-2,initial,0).next_state ==
        State::SpudMazePregameSetup);
    assert(route_retail_menu_action(
        State::SpudChooserUpdate,-3,initial,0).next_state ==
        State::SpudSkatePregameSetup);
    assert(route_retail_menu_action(
        State::AdventureChooserUpdate,-2,initial,0).next_state ==
        State::MazePregameSetup);
    assert(route_retail_menu_action(
        State::AdventureChooserUpdate,-3,initial,0).next_state ==
        State::GolfPregameSetup);

    // Actual -20 Play Again Yes branches depend on original 0x51C2FC.
    auto standard = initial;
    standard.replay_class = 1;
    assert(route_retail_menu_action(
        State::PlayAgainYesNoUpdate,-20,standard,0x36).next_state ==
        State::PlayAgainDifficultySetup);
    standard.replay_class = 2;
    assert(route_retail_menu_action(
        State::PlayAgainYesNoUpdate,-20,standard,0x24).next_state ==
        State::FireworksReplayChoiceSetup);
    standard.replay_class = 0;
    const auto resume = route_retail_menu_action(
        State::PlayAgainYesNoUpdate,-20,standard,0x32);
    assert(resume.next_state == State::ParkDesignerInit);
    assert(resume.clear_replay_active_latch);

    const auto no = route_retail_menu_action(
        State::PlayAgainYesNoUpdate,-21,standard,0x36);
    assert(no.next_state == State::SharedMovieTransition);
    assert(no.clear_replay_active_latch);

    for (int difficulty=0; difficulty<3; ++difficulty) {
        auto replay = route_retail_menu_action(
            State::PlayAgainDifficultyUpdate,
            -30-difficulty,standard,0x36);
        assert(replay.recognized);
        assert(replay.stop_activity_music);
        assert(replay.source_variant == difficulty);
        assert(replay.replay_class == 0);
        assert(replay.clear_replay_active_latch);
        assert(replay.next_state == State::GolfInit);
        auto mutable_state = standard;
        apply_retail_menu_action(
            mutable_state,State::PlayAgainDifficultyUpdate,replay);
        assert(mutable_state.source_variant == difficulty);
        assert(mutable_state.variant_selection_origin ==
               State::PlayAgainDifficultyUpdate);
        assert(mutable_state.replay_class == 0);
    }

    const auto firework_edit = route_retail_menu_action(
        State::FireworksReplayChoiceUpdate,-20,initial,0);
    assert(firework_edit.next_state == State::FireworksRun);
    assert(firework_edit.fireworks_view_mode == 0);
    assert(firework_edit.restart_fireworks_editor);
    assert(!firework_edit.restart_fireworks_view);

    const auto firework_view = route_retail_menu_action(
        State::FireworksReplayChoiceUpdate,-21,initial,0);
    assert(firework_view.next_state == State::FireworksRun);
    assert(firework_view.fireworks_view_mode == 8);
    assert(!firework_view.restart_fireworks_editor);
    assert(firework_view.restart_fireworks_view);

    // Unknown actions never turn into arbitrary valid states.
    for (const auto state : {
        State::SpudChooserUpdate,State::AdventureChooserUpdate,
        State::MusicChooserUpdate,State::DinoChooserUpdate,
        State::PlayAgainYesNoUpdate,State::PlayAgainDifficultyUpdate,
        State::FireworksReplayChoiceUpdate}) {
        const auto invalid=route_retail_menu_action(
            state,-123,initial,0x36);
        assert(!invalid.recognized);
        assert(!invalid.next_state);
    }
}
