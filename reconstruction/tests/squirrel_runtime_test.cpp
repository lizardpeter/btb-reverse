#include "btb/squirrel_runtime.hpp"

#include <cassert>

using namespace btb::squirrel;

int main() {
    static_assert(static_cast<int>(PlacementState::IdleSelect) == 0);
    static_assert(static_cast<int>(PlacementState::ReturnPieceToConveyor) == 8);

    constexpr auto step1 = step_toward_target({0,0}, {10,-10});
    static_assert((step1.position == Vec2i{2,-2}));
    static_assert(!step1.reached);

    constexpr auto snap = step_toward_target({8,-8}, {10,-10});
    static_assert((snap.position == Vec2i{10,-10}));
    static_assert(snap.reached);

    constexpr auto mixed = step_toward_target({9,0}, {10,20});
    static_assert((mixed.position == Vec2i{10,2}));
    static_assert(!mixed.reached);

    constexpr auto already = step_toward_target({20,30}, {20,30});
    static_assert(already.reached);

    constexpr auto final0 = completion_step(0, false, 0);
    static_assert(final0.stage == 1);
    static_assert(final0.action == CompletionAction::PlayFinalWendyLine);
    static_assert(final0.sound_id == 665);

    constexpr auto final2 = completion_step(0, false, 2);
    static_assert(final2.sound_id == 667);

    constexpr auto waiting = completion_step(1, true, 0);
    static_assert(waiting.stage == 1);
    static_assert(waiting.action == CompletionAction::None);

    constexpr auto advance = completion_step(1, false, 0);
    static_assert(advance.stage == 2);
    static_assert(advance.action == CompletionAction::None);

    constexpr auto exit = completion_step(2, false, 0);
    static_assert(exit.action == CompletionAction::ExitToPlayAgain);

    static_assert(kStartupWendySoundId == 643);
    static_assert(kStartupLoftySoundId == 632);
    static_assert(kStartupWendyFollowupSoundId == 644);
    static_assert(kPrimaryPlacementLoftyFeedback[0] == 633);
    static_assert(kPrimaryPlacementLoftyFeedback[3] == 636);
    static_assert(kPrimaryPlacementWendyFeedback[2] == 658);
    static_assert(kAlternatePlacementLoftyFeedback[1] == 638);
    static_assert(kAlternatePlacementWendyFeedback[0] == 645);
    static_assert(kAlternatePlacementWendyFeedback[6] == 652);
    static_assert(kFinalWendyFeedback[2] == 667);
}
