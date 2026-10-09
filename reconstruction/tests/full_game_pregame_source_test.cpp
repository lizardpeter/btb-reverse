#include "btb/full_game_pregame.hpp"

#include <cassert>

using namespace btb::full_game;
using btb::game_flow::State;
using btb::front_end::Screen;

int main() {
    assert(kRetailPregamePairs.size() == 10);

    for (const auto& pair : kRetailPregamePairs) {
        assert(retail_pregame_for_state(pair.setup) == &pair);
        assert(retail_pregame_for_state(pair.update) == &pair);
        assert(pair.activity_init == static_cast<State>(
            static_cast<int>(pair.update)+1));

        const auto back = route_retail_pregame_action(pair.update,-1);
        assert(back.recognized && back.leaves_to_parent);
        assert(back.next_state == pair.back);
        assert(!back.start_activity);

        const auto help = route_retail_pregame_action(pair.update,-6);
        assert(help.recognized && help.help_retains_screen);
        assert(!help.next_state);
        assert(!help.difficulty);

        const auto start = route_retail_pregame_action(pair.update,-5);
        assert(start.recognized && start.start_activity);
        assert(start.next_state == pair.activity_init);

        for (int level=0; level<3; ++level) {
            const auto choice = route_retail_pregame_action(
                pair.update,-2-level);
            assert(choice.recognized);
            assert(choice.difficulty == level);
            assert(choice.persistent_herding_difficulty ==
                (pair.update == State::HerdingPregameUpdate));
            if (pair.update == State::SpudSkatePregameUpdate && level == 0) {
                // Unlike the other nine instruction handlers, source
                // 0x42B6AD jumps straight to game init 0x1A.
                assert(choice.skate_immediate_start);
                assert(choice.start_activity);
                assert(choice.next_state == State::SpudSkateInit);
            } else {
                assert(!choice.next_state);
                assert(!choice.start_activity);
            }
        }
        for (int invalid : {-99,-30,-21,0,1}) {
            assert(!route_retail_pregame_action(
                pair.update,invalid).recognized);
        }
        assert(!route_retail_pregame_action(pair.setup,-5).recognized);
    }

    // Source command 0x4281D0 and the 256-byte bitmap-path slots must
    // follow original selected-subgame globals, not be hard-coded from
    // the generic instruction UI screen ID.
    assert(retail_walkthrough_index(State::HerdingPregameSetup,0) == 0);
    assert(retail_walkthrough_index(State::DinoPregameSetup,2) == 1);
    assert(retail_walkthrough_index(State::SpudMazePregameSetup,0) == 2);
    assert(retail_walkthrough_index(State::SpudSkatePregameSetup,1) == 3);
    assert(retail_walkthrough_index(State::MazePregameSetup,0) == 4);
    assert(retail_walkthrough_index(State::GolfPregameSetup,1) == 5);
    assert(retail_walkthrough_index(State::FireworksPregameSetup,0) == 6);
    assert(retail_walkthrough_index(State::SquirrelPregameSetup,0) == 7);
    assert(retail_walkthrough_index(State::BobsBandPregameSetup,2) == 8);
    assert(retail_walkthrough_index(State::ParkDesignerPregameSetup,0) == 9);
    assert(!retail_walkthrough_index(State::GolfPregameSetup,2));
    assert(!retail_walkthrough_index(State::SpudMazePregameSetup,-1));

    assert(retail_pregame_backdrop_index(State::HerdingPregameSetup,0) == 4);
    assert(retail_pregame_backdrop_index(State::DinoPregameSetup,0) == 5);
    assert(retail_pregame_backdrop_index(State::DinoPregameSetup,1) == 6);
    assert(retail_pregame_backdrop_index(State::DinoPregameSetup,2) == 7);
    assert(retail_pregame_backdrop_index(State::SpudMazePregameSetup,0) == 10);
    assert(retail_pregame_backdrop_index(State::SpudSkatePregameSetup,1) == 11);
    assert(retail_pregame_backdrop_index(State::MazePregameSetup,0) == 13);
    assert(retail_pregame_backdrop_index(State::GolfPregameSetup,1) == 14);
    assert(retail_pregame_backdrop_index(State::FireworksPregameSetup,0) == 15);
    assert(retail_pregame_backdrop_index(State::SquirrelPregameSetup,0) == 16);
    assert(retail_pregame_backdrop_index(State::BobsBandPregameSetup,2) == 17);
    assert(retail_pregame_backdrop_index(State::ParkDesignerPregameSetup,0) == 18);
    assert(!retail_pregame_backdrop_index(State::DinoPregameSetup,3));
    assert(!retail_pregame_backdrop_index(State::GolfPregameSetup,2));

    assert(retail_pregame_for_state(State::ActivitySelectUpdate) ==
           nullptr);
    assert(retail_pregame_for_state(State::PlayAgainYesNoUpdate) ==
           nullptr);

    // The loaded source screen is independent of the desired difficulty
    // and must reflect the original setup's generic UI slot.
    assert(retail_pregame_for_state(State::HerdingPregameSetup)->screen ==
           Screen::InstructionWithDifficulty);
    assert(retail_pregame_for_state(State::DinoPregameSetup)->screen ==
           Screen::InstructionWithDifficulty);
    assert(retail_pregame_for_state(State::SpudSkatePregameSetup)->screen ==
           Screen::InstructionNoDifficulty);
    assert(retail_pregame_for_state(State::MazePregameSetup)->screen ==
           Screen::InstructionWithDifficulty);
    assert(retail_pregame_for_state(State::FireworksPregameSetup)->screen ==
           Screen::FireworksInstruction);
    assert(retail_pregame_for_state(State::SquirrelPregameSetup)->screen ==
           Screen::InstructionWithDifficulty);
    assert(retail_pregame_for_state(State::BobsBandPregameSetup)->screen ==
           Screen::InstructionNoDifficulty);
    assert(retail_pregame_for_state(State::ParkDesignerPregameSetup)->screen ==
           Screen::InstructionNoDifficulty);
    assert(retail_pregame_for_state(State::GolfPregameSetup)->screen ==
           Screen::InstructionWithDifficulty);
    assert(retail_pregame_for_state(State::SpudMazePregameSetup)->screen ==
           Screen::InstructionWithDifficulty);
}
