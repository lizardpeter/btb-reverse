#include "btb/squirrel_runtime.hpp"

#include <cassert>

using namespace btb::squirrel;

int main() {
    static_assert(kMotionKeyCount == 14);
    static_assert(kAssemblySpriteBases[0]==0 && kAssemblySpriteBases[11]==48);
    static_assert(kAssemblySpriteBases[12]==63 && kAssemblySpriteBases[13]==66);
    for(int key=0;key<14;++key)for(int phase=0;phase<4;++phase)for(int sub=0;sub<3;++sub) {
        RunAssemblyAnimationRuntime s{};
        s.motion_keys.a=s.motion_keys.b=s.motion_keys.c=s.motion_keys.d=key;
        s.sprite_mode=phase;s.substep=sub;
        assert(active_assembly_sprite_frame(s)==kAssemblySpriteBases[key]+sub);
    }
    for(int seed=0;seed<256;++seed) {
        btb::retail::OriginalRetailRandom rng(seed),expected(seed);
        RunAssemblyAnimationRuntime s{};
        s.sprite_frame=62;s.delay_remaining=1;
        const int first=(expected.next_rand()&3)==0?31:48;
        assert(tick_assembly_idle_sprite(s,7,[&]{return rng.next_rand();})==first);
        assert(rng.state()==expected.state());
        s.motion_keys.d=13;s.sprite_frame=85;s.delay_remaining=1;
        assert(tick_assembly_idle_sprite(s,7,[&]{return rng.next_rand();})==86);
        s.delay_remaining=1;
        assert(tick_assembly_idle_sprite(s,7,[&]{return rng.next_rand();})==72);
        assert(rng.state()==expected.state());
    }
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
    static_assert(selector1->a == 0);
    static_assert(selector1->b == 13);
    static_assert(selector1->c == 12);
    static_assert(selector1->d == 13);
    static_assert(selector1->first_lookup_index == 0);
    static_assert(selector1->other_lookup_index == 31);
    static_assert(selector1->placed_piece_slot == 0);

    constexpr auto selector2 =
        select_run_assembly_motion_keys(
            2, 0, selector_connectors, selector_pieces);
    static_assert(selector2.has_value());
    static_assert(selector2->a == 12);
    static_assert(selector2->b == 1);
    static_assert(selector2->c == 0);
    static_assert(selector2->d == 1);
    static_assert(selector2->first_lookup_index == 31);
    static_assert(selector2->other_lookup_index == 16);
    static_assert(selector2->placed_piece_slot == 0);

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
    static_assert(selector6->a == 5);
    static_assert(selector6->b == 1);
    static_assert(selector6->c == 0);
    static_assert(selector6->d == 1);

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

    // Exhaustive independent branch-index reference: 69,984 combinations.
    constexpr std::array<int,3> conn_idx{{0,16,33}};
    for(int level=0;level<3;++level)for(int tuple=0;tuple<81;++tuple)
    for(int piece=0;piece<36;++piece) {
        RunPlan plan{};PlacedRunPieces pieces{};
        int value=tuple;
        for(auto& connector:plan[level]){connector=value%3;value/=3;}
        for(int i=0;i<4;++i)pieces[level*4+i]=(piece+i*7)%36;
        const auto c0=conn_idx[plan[level][0]],c1=conn_idx[plan[level][1]],
                   c2=conn_idx[plan[level][2]],c3=conn_idx[plan[level][3]];
        const auto p0=transpose_piece_id_for_motion_lookup(pieces[level*4]),
                   p1=transpose_piece_id_for_motion_lookup(pieces[level*4+1]),
                   p2=transpose_piece_id_for_motion_lookup(pieces[level*4+2]);
        const int raw=pieces[level*4+3];
        const std::array<int,8> a{{c0,c0,p0,c2,p1,c2,p2,raw}};
        const std::array<int,8> b{{c0,p0,c1,p1,c2,p2,c3,raw}};
        const std::array<int,8> slots{{-1,0,0,1,1,2,2,3}};
        for(int section=0;section<8;++section) {
            const auto keys=select_run_assembly_motion_keys(section,level,plan,pieces);
            assert(keys && keys->first_lookup_index==a[section] &&
                keys->other_lookup_index==b[section] && keys->placed_piece_slot==slots[section]);
            assert(keys->a==kMotionKeyLookupA[a[section]] &&
                keys->b==kMotionKeyLookupB[b[section]] &&
                keys->c==kMotionKeyLookupC[b[section]] &&
                keys->d==kMotionKeyLookupD[b[section]]);
        }
    }
    // Section 2 must work before the second piece has been placed.
    PlacedRunPieces one_piece{};one_piece.fill(-1);one_piece[0]=34;
    assert(select_run_assembly_motion_keys(2,0,selector_connectors,one_piece));
    assert(!select_run_assembly_motion_keys(3,0,selector_connectors,one_piece));

    Data animation_data;
    animation_data.horizontal_motion_deltas =
        {0,3,10,27,8,-4,-4};
    animation_data.vertical_motion_deltas = {{
        {{0,0,1,-1,0,0,0}},
        {{0,0,0,0,0,0,0}},
        {{0,0,1,-17,0,0,0}},
        {{0,0,1,-33,0,0,0}},
        {{0,0,-1,17,0,0,0}},
        {{0,0,19,13,0,0,0}},
        {{0,0,0,-32,0,0,0}},
        {{0,0,0,32,0,0,0}},
    }};
    animation_data.animation_step_delay = 7;

    RunAssemblyAnimationRuntime assembly;
    assembly.run_section = 1;
    begin_run_assembly_animation(
        assembly, animation_data, *selector0);

    assert(
        assembly.state ==
        RunAssemblyAnimationState::FirstThreeSubsteps);
    assert(assembly.delay_remaining == 7);
    assert(assembly.substep == 0);
    assert(assembly.sprite_mode == 0);
    assert(assembly.sprite_frame == 0);

    auto motion_tick =
        tick_run_assembly_animation(assembly, animation_data);
    assert(motion_tick.applied_motion);
    assert(motion_tick.absolute_motion_substep == 0);
    assert(
        motion_tick.profile ==
        VerticalMotionProfile::NeutralArc);
    assert((motion_tick.delta == Vec2i{0,0}));
    assert(assembly.delay_remaining == 6);

    // Finish the first two A substeps. A1 begins on update 8 and A2 on 15.
    for (int i = 0; i < 19; ++i) {
        motion_tick =
            tick_run_assembly_animation(assembly, animation_data);
    }

    // Update 21 ends A2 and immediately falls through transient state 2 into
    // state 3, applying B's absolute substep 3 on that same update.
    motion_tick =
        tick_run_assembly_animation(assembly, animation_data);
    assert(motion_tick.applied_motion);
    assert(motion_tick.entered_transient_state_2);
    assert(motion_tick.absolute_motion_substep == 3);
    assert(
        assembly.state ==
        RunAssemblyAnimationState::LastFourSubsteps);
    assert(assembly.sprite_mode == 1);
    assert(assembly.sprite_frame == 3);
    assert((Vec2i{assembly.x,assembly.y} == Vec2i{40,0}));

    // Restart from a clean state and run the exact full retail cycle.
    assembly = {};
    assembly.run_section = 1;
    begin_run_assembly_animation(
        assembly, animation_data, *selector0);

    int updates = 0;
    do {
        motion_tick =
            tick_run_assembly_animation(assembly, animation_data);
        ++updates;
        assert(updates <= 100);
    } while (!motion_tick.cycle_completed);

    assert(updates == 95);
    assert(
        assembly.state ==
        RunAssemblyAnimationState::Idle);
    assert(assembly.run_section == 2);
    assert(assembly.substep == 3);
    assert(assembly.delay_remaining == 7);
    assert(assembly.sprite_mode == 3);
    assert((Vec2i{assembly.x,assembly.y} == Vec2i{80,0}));

    // The retained state-5 bridge normalizes directly into state 6 and begins
    // the D second-half profile in the same update.
    RunAssemblyAnimationRuntime retained;
    retained.state =
        RunAssemblyAnimationState::RetainedTransition;
    retained.delay_remaining = 0;
    retained.motion_keys = *selector0;
    motion_tick =
        tick_run_assembly_animation(retained, animation_data);
    assert(
        retained.state ==
        RunAssemblyAnimationState::SecondLastFourSubsteps);
    assert(retained.sprite_mode == 3);
    assert(retained.sprite_frame == 3);
    assert(motion_tick.applied_motion);
    assert(motion_tick.absolute_motion_substep == 3);
    assert((Vec2i{retained.x,retained.y} == Vec2i{27,-1}));

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

    static_assert(step_toward_target({0,0},{5,-5}).reached);
    static_assert(!step_toward_target({0,0},{6,-6}).reached);

    constexpr auto already = step_toward_target({20,30}, {20,30});
    static_assert(already.reached);

    for(bool correct:{false,true})for(int slot=0;slot<3;++slot)for(int choice=0;choice<4;++choice) {
        PlacementMotion actor;
        assert(select_placement(actor,choice,correct));
        assert(!select_placement(actor,choice,correct));
        int pickup=0,correct_drop=0,wrong_drop=0,retrieve=0,returned=0,home=0;
        for(int tick=0;tick<2500 && actor.state!=PlacementState::IdleSelect;++tick) {
            switch(advance_placement_motion(actor,slot)) {
            case PlacementEvent::PickedUp:++pickup;assert(actor.carrying);break;
            case PlacementEvent::CorrectDropped:++correct_drop;assert(!actor.carrying);break;
            case PlacementEvent::DecoyDropped:++wrong_drop;assert(!actor.carrying);break;
            case PlacementEvent::DecoyRetrieved:++retrieve;assert(actor.carrying);break;
            case PlacementEvent::DecoyReturned:++returned;assert(!actor.carrying);break;
            case PlacementEvent::Home:++home;break;
            default:break;
            }
        }
        assert(actor.state==PlacementState::IdleSelect && actor.position==kLoftyHomeTarget);
        assert(actor.selected==-1 && !actor.carrying && pickup==1 && home==1);
        assert(correct_drop==int(correct) && wrong_drop==int(!correct));
        assert(retrieve==int(!correct) && returned==int(!correct));
    }

    // 004267B0 mouth state must use the same CRT draws in the same
    // order, including the one last active tick on physical sound EOS.
    for(std::uint32_t seed=0;seed<1024;++seed) {
        btb::retail::OriginalRetailRandom actual(seed),expected(seed);
        LoftySpeechAnimation mouth;
        mouth.begin(656,actual); // Wendy's line consumes nothing.
        assert(mouth.sound_id==-1 && actual.state()==expected.state());
        mouth.begin(633,actual);
        const int frame=expected.next_rand()%3,delay=expected.next_rand()%10+10;
        assert(mouth.sound_id==633 && mouth.mouth_frame==frame &&
            mouth.countdown==delay && actual.state()==expected.state());
        for(int i=0;i<delay-1;++i)mouth.advance(true,actual);
        assert(mouth.countdown==1 && actual.state()==expected.state());
        const int next_delay=expected.next_rand()%10+10;
        int next_frame;
        do{next_frame=expected.next_rand()%3;}while(next_frame==frame);
        mouth.advance(false,actual);
        assert(mouth.sound_id==-1 && mouth.countdown==next_delay &&
            mouth.mouth_frame==next_frame && actual.state()==expected.state());
        mouth.advance(true,actual);
        assert(actual.state()==expected.state() && mouth.countdown==next_delay);
    }

    // Every legal connector pair, difficulty, and seed. Hard mode's two
    // decoys use distinct wrong first connectors; the missing-to branch
    // leaves the original landing connector unchanged.
    for(int diff=0;diff<3;++diff)for(int from=0;from<3;++from)
    for(int to=-1;to<3;++to)for(std::uint32_t seed=0;seed<256;++seed) {
        const auto difficulty=static_cast<Difficulty>(diff);
        btb::retail::OriginalRetailRandom rng(seed*0x9e3779b9u+0x1234u);
        auto generated=generate_conveyor_choices(difficulty,{from,to},rng);
        assert(generated && generated->count==conveyor_mix(difficulty).total_items());
        assert(generated->landing_connector.value_or(-1)==to);
        int correct=0,decoy=0,previous_wrong=-1;
        for(int i=0;i<generated->count;++i) {
            const auto& choice=generated->choices[i];
            const auto decoded=decode_piece_id(choice.piece_id);
            assert(choice.active && decoded.outer_variant==
                kOuterVariantPermutations[generated->variant_index][i]);
            assert(choice.feedback_to==decoded.to_connector);
            if(choice.correct) {
                ++correct;
                assert(decoded.from_connector==from && (to==-1||decoded.to_connector==to));
                assert(choice.feedback_comparison==from);
            } else {
                ++decoy;
                assert(decoded.from_connector!=from && decoded.from_connector!=previous_wrong);
                assert(to==-1||decoded.to_connector!=to);
                assert(choice.feedback_comparison==decoded.from_connector*3+decoded.to_connector);
                previous_wrong=decoded.from_connector;
            }
        }
        assert(correct==conveyor_mix(difficulty).correct_items);
        assert(decoy==conveyor_mix(difficulty).decoy_items);
        for(int i=generated->count;i<4;++i)assert(!generated->choices[i].active);
    }
    {
        btb::retail::OriginalRetailRandom rng(42);
        assert(!generate_conveyor_choices(Difficulty::Hard,{-1,0},rng));
        assert(rng.state()==42);
    }

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
