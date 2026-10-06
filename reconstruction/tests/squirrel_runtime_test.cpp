#include "btb/squirrel_runtime.hpp"

#include <cassert>

using namespace btb::squirrel;

int main() {
    static_assert(kMotionKeyCount == 14);
    static_assert(kFirstMotionPhaseSubsteps == 3);
    static_assert(kSecondMotionPhaseFirstSubstep == 3);
    static_assert(kVerticalProfileByMotionKey[0] == 0);
    static_assert(kVerticalProfileByMotionKey[2] == 3);
    static_assert(kVerticalProfileByMotionKey[4] == 5);
    static_assert(kVerticalProfileByMotionKey[6] == 4);
    static_assert(kVerticalProfileByMotionKey[8] == 2);
    static_assert(kVerticalProfileByMotionKey[12] == 1);

    static_assert(
        vertical_profile_for_motion_key(0) ==
        VerticalMotionProfile::NeutralArc);
    static_assert(
        vertical_profile_for_motion_key(2) ==
        VerticalMotionProfile::Up32Arc);
    static_assert(
        vertical_profile_for_motion_key(4) ==
        VerticalMotionProfile::Down32Arc);
    static_assert(
        vertical_profile_for_motion_key(6) ==
        VerticalMotionProfile::Down16Arc);
    static_assert(
        vertical_profile_for_motion_key(8) ==
        VerticalMotionProfile::Up16Arc);
    static_assert(
        vertical_profile_for_motion_key(12) ==
        VerticalMotionProfile::NeutralFlat);
    static_assert(!vertical_profile_for_motion_key(-1).has_value());
    static_assert(!vertical_profile_for_motion_key(14).has_value());

    static_assert(
        loaded_profile_is_live(VerticalMotionProfile::NeutralArc));
    static_assert(
        loaded_profile_is_live(VerticalMotionProfile::NeutralFlat));
    static_assert(
        loaded_profile_is_live(VerticalMotionProfile::Up16Arc));
    static_assert(
        loaded_profile_is_live(VerticalMotionProfile::Up32Arc));
    static_assert(
        loaded_profile_is_live(VerticalMotionProfile::Down16Arc));
    static_assert(
        loaded_profile_is_live(VerticalMotionProfile::Down32Arc));
    static_assert(
        !loaded_profile_is_live(VerticalMotionProfile::Up32Direct));
    static_assert(
        !loaded_profile_is_live(VerticalMotionProfile::Down32Direct));

    static_assert(kConnectorMotionLookupIndices[0] == 0);
    static_assert(kConnectorMotionLookupIndices[1] == 16);
    static_assert(kConnectorMotionLookupIndices[2] == 33);

    static_assert(transpose_piece_id_for_motion_lookup(0) == 0);
    static_assert(transpose_piece_id_for_motion_lookup(1) == 4);
    static_assert(transpose_piece_id_for_motion_lookup(8) == 32);
    static_assert(transpose_piece_id_for_motion_lookup(9) == 1);
    static_assert(transpose_piece_id_for_motion_lookup(34) == 31);
    static_assert(transpose_piece_id_for_motion_lookup(35) == 35);
    static_assert(transpose_piece_id_for_motion_lookup(-1) == -1);
    static_assert(transpose_piece_id_for_motion_lookup(36) == -1);

    static_assert(motion_piece_slot_for_run_section(0) == -1);
    static_assert(motion_piece_slot_for_run_section(1) == 0);
    static_assert(motion_piece_slot_for_run_section(2) == 1);
    static_assert(motion_piece_slot_for_run_section(3) == 1);
    static_assert(motion_piece_slot_for_run_section(4) == 1);
    static_assert(motion_piece_slot_for_run_section(5) == 2);
    static_assert(motion_piece_slot_for_run_section(6) == 2);
    static_assert(motion_piece_slot_for_run_section(7) == 3);

    constexpr RunPlan selector_connectors{{
        {{0,1,2,0}},
        {{2,1,0,2}},
        {{1,2,0,1}},
    }};
    constexpr PlacedRunPieces selector_pieces{{
        34,1,15,20,
        0,9,18,27,
        5,14,23,32,
    }};

    constexpr auto selector0 =
        select_run_assembly_motion_keys(
            0, 0, selector_connectors, selector_pieces);
    static_assert(selector0.has_value());
    static_assert(selector0->a == 0);
    static_assert(selector0->b == 1);
    static_assert(selector0->c == 0);
    static_assert(selector0->d == 1);
    static_assert(selector0->first_lookup_index == 0);
    static_assert(selector0->other_lookup_index == 0);
    static_assert(selector0->placed_piece_slot == -1);

    constexpr auto selector1 =
        select_run_assembly_motion_keys(
            1, 0, selector_connectors, selector_pieces);
    static_assert(selector1.has_value());
    static_assert(selector1->a == 12);
    static_assert(selector1->b == 1);
    static_assert(selector1->c == 0);
    static_assert(selector1->d == 1);
    static_assert(selector1->first_lookup_index == 31);
    static_assert(selector1->other_lookup_index == 16);
    static_assert(selector1->placed_piece_slot == 0);

    constexpr auto selector2 =
        select_run_assembly_motion_keys(
            2, 0, selector_connectors, selector_pieces);
    static_assert(selector2.has_value());
    static_assert(selector2->a == 0);
    static_assert(selector2->b == 13);
    static_assert(selector2->c == 12);
    static_assert(selector2->d == 13);
    static_assert(selector2->first_lookup_index == 0);
    static_assert(selector2->other_lookup_index == 4);
    static_assert(selector2->placed_piece_slot == 1);

    constexpr auto selector3 =
        select_run_assembly_motion_keys(
            3, 0, selector_connectors, selector_pieces);
    static_assert(selector3.has_value());
    static_assert(selector3->a == 0);
    static_assert(selector3->b == 13);
    static_assert(selector3->c == 12);
    static_assert(selector3->d == 13);
    static_assert(selector3->first_lookup_index == 33);
    static_assert(selector3->other_lookup_index == 4);

    constexpr auto selector4 =
        select_run_assembly_motion_keys(
            4, 0, selector_connectors, selector_pieces);
    static_assert(selector4.has_value());
    static_assert(selector4->a == 12);
    static_assert(selector4->b == 1);
    static_assert(selector4->c == 0);
    static_assert(selector4->d == 1);
    static_assert(selector4->first_lookup_index == 4);
    static_assert(selector4->other_lookup_index == 33);

    constexpr auto selector5 =
        select_run_assembly_motion_keys(
            5, 0, selector_connectors, selector_pieces);
    static_assert(selector5.has_value());
    static_assert(selector5->a == 0);
    static_assert(selector5->b == 4);
    static_assert(selector5->c == 5);
    static_assert(selector5->d == 4);

    constexpr auto selector6 =
        select_run_assembly_motion_keys(
            6, 0, selector_connectors, selector_pieces);
    static_assert(selector6.has_value());
    static_assert(selector6->a == 0);
    static_assert(selector6->b == 4);
    static_assert(selector6->c == 5);
    static_assert(selector6->d == 4);

    // Retained default branch uses raw piece ID rather than the transposed
    // connector-major index.
    constexpr auto selector7 =
        select_run_assembly_motion_keys(
            7, 0, selector_connectors, selector_pieces);
    static_assert(selector7.has_value());
    static_assert(selector7->first_lookup_index == 20);
    static_assert(selector7->other_lookup_index == 20);
    static_assert(selector7->a == 12);
    static_assert(selector7->b == 13);
    static_assert(selector7->c == 12);
    static_assert(selector7->d == 13);

    constexpr PlacedRunPieces invalid_pieces{{
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
    }};
    static_assert(!select_run_assembly_motion_keys(
        1, 0, selector_connectors, invalid_pieces).has_value());

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
    static_assert(decoded.from_connector == 2);
    static_assert(decoded.to_connector == 1);

    constexpr RunPlan raw_plan{{
        {{0,1,2,0}},
        {{2,2,1,1}},
        {{0,1,0,2}},
    }};
    constexpr auto plan = chain_run_plan(raw_plan);
    static_assert(plan[0][3] == 0);
    static_assert(plan[1][0] == 0);
    static_assert(plan[1][3] == 1);
    static_assert(plan[2][0] == 1);
    static_assert((required_connectors(plan, 0, 0) == ConnectorPair{0,1}));
    static_assert((required_connectors(plan, 0, 1) == ConnectorPair{1,2}));
    static_assert((required_connectors(plan, 0, 2) == ConnectorPair{2,0}));
    static_assert(is_correct_connector_pair({1,2}, {1,2}));
    static_assert(!is_correct_connector_pair({1,0}, {1,2}));
    static_assert(is_retail_decoy_pair({2,0}, {1,2}));
    static_assert(!is_retail_decoy_pair({1,0}, {1,2}));
    static_assert(!is_retail_decoy_pair({2,2}, {1,2}));

    static_assert(level_count(Difficulty::Easy) == 1);
    static_assert(level_count(Difficulty::Medium) == 2);
    static_assert(level_count(Difficulty::Hard) == 3);
    static_assert(kPiecesPerLevel == 3);
    static_assert(required_correct_placements(Difficulty::Easy) == 3);
    static_assert(required_correct_placements(Difficulty::Medium) == 6);
    static_assert(required_correct_placements(Difficulty::Hard) == 9);
    static_assert((kConveyorItemOrigins[0] == Vec2i{100,300}));
    static_assert((kConveyorItemOrigins[3] == Vec2i{459,300}));
    static_assert(kConveyorHitWidth == 89);
    static_assert(kConveyorHitHeight == 133);
    static_assert((kRunPlacementTargets[0] == Vec2i{116,327}));
    static_assert((kRunPlacementTargets[2] == Vec2i{436,327}));
    static_assert((kLoftyHomeTarget == Vec2i{93,185}));
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

    static_assert(correct_placement_feedback_sound_id(0) == 633);
    static_assert(correct_placement_feedback_sound_id(3) == 636);
    static_assert(correct_placement_feedback_sound_id(4) == 656);
    static_assert(correct_placement_feedback_sound_id(6) == 658);

    static_assert(decoy_lofty_feedback_sound_id(0) == 637);
    static_assert(decoy_lofty_feedback_sound_id(1) == 638);

    static_assert(decoy_direction_feedback_sound_id(1,2,0) == 650);
    static_assert(decoy_direction_feedback_sound_id(1,2,2) == 652);
    static_assert(decoy_direction_feedback_sound_id(2,1,0) == 645);
    static_assert(decoy_direction_feedback_sound_id(2,1,1) == 646);
    static_assert(decoy_direction_feedback_sound_id(2,1,2) == 648);
    static_assert(decoy_direction_feedback_sound_id(2,1,3) == 649);
    static_assert(decoy_direction_feedback_sound_id(1,1,0) == 637);
    static_assert(decoy_direction_feedback_sound_id(1,1,1) == 638);
}
