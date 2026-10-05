#include "btb/squirrel_runtime.hpp"

#include <cassert>

using namespace btb::squirrel;

int main() {
    static_assert(static_cast<int>(PlacementState::IdleSelect) == 0);
    static_assert(static_cast<int>(PlacementState::ReturnDecoyToConveyor) == 8);

    static_assert(conveyor_mix(Difficulty::Easy).correct_items == 3);
    static_assert(conveyor_mix(Difficulty::Easy).decoy_items == 0);
    static_assert(conveyor_mix(Difficulty::Medium).correct_items == 3);
    static_assert(conveyor_mix(Difficulty::Medium).decoy_items == 1);
    static_assert(conveyor_mix(Difficulty::Hard).correct_items == 2);
    static_assert(conveyor_mix(Difficulty::Hard).decoy_items == 2);
    static_assert(conveyor_mix(Difficulty::Easy).total_items() == 3);
    static_assert(conveyor_mix(Difficulty::Medium).total_items() == 4);
    static_assert(conveyor_mix(Difficulty::Hard).total_items() == 4);

    static_assert(kOuterVariantPermutations[0][0] == 0);
    static_assert(kOuterVariantPermutations[0][3] == 3);
    static_assert(kOuterVariantPermutations[6][0] == 3);
    static_assert(kOuterVariantPermutations[6][3] == 0);

    constexpr PieceIdParts parts{3,2,1};
    static_assert(encode_piece_id(parts) == 34);
    constexpr auto decoded = decode_piece_id(34);
    static_assert(decoded.outer_variant == 3);
    static_assert(decoded.inner_a == 2);
    static_assert(decoded.inner_b == 1);

    static_assert(level_count(Difficulty::Easy) == 1);
    static_assert(level_count(Difficulty::Medium) == 2);
    static_assert(level_count(Difficulty::Hard) == 3);
    static_assert(!run_reached_level_end(6));
    static_assert(run_reached_level_end(7));
    static_assert(should_finish_activity(Difficulty::Easy, 0, 7));
    static_assert(!should_finish_activity(Difficulty::Hard, 1, 7));
    static_assert(should_advance_level(Difficulty::Hard, 1, 7));
    static_assert(level_finished_feedback_sound_id(0) == 664);
    static_assert(level_finished_feedback_sound_id(3) == 667);

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
