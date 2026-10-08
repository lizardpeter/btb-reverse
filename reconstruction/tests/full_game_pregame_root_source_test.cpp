#include "btb/full_game_generic_ui.hpp"
#include "btb/full_game_pregame.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace {
using namespace btb;
using namespace btb::full_game;

GenericUiCatalog source_shaped_catalog() {
    GenericUiCatalog catalog;
    for (std::size_t screen=0; screen<front_end::kScreenCount; ++screen) {
        const int count = front_end::kRetailHotAreaCounts[screen];
        catalog.counts.push_back(count);
        front_end::HotAreaScreen areas;
        front_end::ReplacementScreen rows;
        for (int i=0; i<count; ++i) {
            const int x=10+i*70;
            areas.areas.push_back({
                {{{x,50},{x+40,50},{x+40,90},{x,90}}},{}
            });
            front_end::ReplacementRecord row{};
            row.target_state_or_action = -6;
            row.click_sound_id = -1;
            row.hover_sound_ids = {-1,-1,-1};
            rows.records.push_back(std::move(row));
        }
        // The original instruction table records, in their actual row
        // order: Back, Easy, Medium, Hard, Start, Help.
        if (screen == 2) {
            for (int i=0; i<count; ++i) {
                rows.records[static_cast<std::size_t>(i)]
                    .target_state_or_action =
                    std::array<int,6>{-1,-2,-3,-4,-5,-6}
                        [static_cast<std::size_t>(i)];
            }
        }
        // Fireworks and no-difficulty instruction tables both contain
        // three areas: Back, Start(-2), Help(-6). These are original
        // source values, not a fabricated universal -5 Start shortcut.
        if (screen == 7 || screen == 8) {
            rows.records[0].target_state_or_action = -1;
            rows.records[1].target_state_or_action = -2;
            rows.records[2].target_state_or_action = -6;
        }
        catalog.hot_areas.push_back(std::move(areas));
        catalog.replacements.push_back(std::move(rows));
    }
    return catalog;
}

btb::full_game::GameFrame click_area(
    btb::full_game::GameRoot& game,int index) {
    btb::full_game::ActivityFrameInput input{};
    input.pointer_x = 20 + index*70;
    input.pointer_y = 65;
    input.click_pulse = true;
    auto pending=game.advance(input);
    assert(pending.kind ==
           btb::full_game::FrameKind::FrontEndUpdated);
    assert(!pending.effects.next_outer_state);
    input.click_pulse = false;
    return game.advance(input);
}
}

int main() {
    using namespace btb::full_game;
    using btb::game_flow::State;
    auto catalog=source_shaped_catalog();
    std::string error;
    assert(catalog.validate(error));

    for (const auto& pregame : kRetailPregamePairs) {
        GameRoot root;
        assert(root.select_profile(0));
        assert(!root.install_pregame_front_end_pair(
            pregame.update,std::make_unique<GenericUiScreenDriver>(
                catalog,pregame.screen,
                std::vector<int>(front_end::kRetailHotAreaCounts[
                    static_cast<std::size_t>(pregame.screen)],0))));
        assert(root.install_pregame_front_end_pair(
            pregame.setup,std::make_unique<GenericUiScreenDriver>(
                catalog,pregame.screen,
                std::vector<int>(front_end::kRetailHotAreaCounts[
                    static_cast<std::size_t>(pregame.screen)],0))));

        root.set_outer_state(pregame.setup);
        const auto initialized=root.advance({});
        assert(initialized.kind == FrameKind::FrontEndInitialized);
        assert(initialized.requires_original_walkthrough_host);
        assert(root.globals().dispatcher.current_state ==
               static_cast<int>(pregame.update));

        const int easy_index = pregame.selected_difficulty ? 1 : 1;
        auto easy=click_area(root,easy_index);
        assert(easy.kind == FrameKind::FrontEndUpdated);
        assert(easy.requires_original_walkthrough_host);
        assert(easy.pregame_action && easy.pregame_action->recognized);
        assert(easy.effects.negative_ui_action == -2);
        assert(root.globals().menu.source_variant == 0);

        if (pregame.update == State::SpudSkatePregameUpdate) {
            assert(easy.pregame_action->skate_immediate_start);
            assert(root.globals().spud_skate_start_latch);
            assert(root.globals().dispatcher.current_state ==
                   static_cast<int>(State::SpudSkateInit));
        } else {
            assert(root.globals().dispatcher.current_state ==
                   static_cast<int>(pregame.update));
        }

        if (pregame.selected_difficulty) {
            // The retail Start button -5 is a different source-data area
            // from Easy/Medium/Hard -2/-3/-4.
            auto start=click_area(root,4);
            assert(start.pregame_action);
            assert(start.pregame_action->start_activity);
            assert(start.pregame_action->next_state ==
                   pregame.activity_init);
            assert(root.globals().dispatcher.current_state ==
                   static_cast<int>(pregame.activity_init));
            assert(root.advance({}).kind ==
                   FrameKind::ActivityRequiresAdapter);
        }

        // Returning to the original pregame setup resets its active
        // hover/click state. Back must return to its actual parent chooser.
        root.set_outer_state(pregame.setup);
        assert(root.advance({}).kind == FrameKind::FrontEndInitialized);
        auto back=click_area(root,0);
        assert(back.pregame_action);
        assert(back.pregame_action->leaves_to_parent);
        assert(back.pregame_action->next_state == pregame.back);
        assert(root.globals().dispatcher.current_state ==
               static_cast<int>(pregame.back));
    }

    // Herding retains 0x51C340 separately from the current source
    // variant when the user changes Easy/Medium/Hard.
    GameRoot herding;
    assert(herding.select_profile(0));
    assert(herding.install_pregame_front_end_pair(
        State::HerdingPregameSetup,
        std::make_unique<GenericUiScreenDriver>(
            catalog,front_end::Screen::InstructionWithDifficulty,
            std::vector<int>(6,0))));
    herding.set_outer_state(State::HerdingPregameSetup);
    assert(herding.advance({}).kind == FrameKind::FrontEndInitialized);
    const auto hard=click_area(herding,3);
    assert(hard.pregame_action->difficulty == 2);
    assert(herding.globals().retained_herding_difficulty == 2);
    assert(herding.globals().menu.source_variant == 2);
    herding.set_outer_state(State::HerdingPregameSetup);
    assert(herding.advance({}).kind == FrameKind::FrontEndInitialized);
    assert(herding.globals().menu.source_variant == 2);
}
