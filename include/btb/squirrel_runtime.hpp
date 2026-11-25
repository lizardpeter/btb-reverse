#pragma once

#include "btb/squirrel_data.hpp"
#include "btb/retail_crt_random.hpp"

#include <array>
#include <cstdint>
#include <optional>

namespace btb::squirrel {

// Global 0x5150E4. Names intentionally describe the observed transition role;
// the exact story label of the two mirrored paths is not assumed.
inline constexpr std::size_t kMotionKeyCount = 14;
inline constexpr std::size_t kFirstMotionPhaseSubsteps = 3;
inline constexpr std::size_t kSecondMotionPhaseFirstSubstep = 3;

// Stack table reconstructed from 0x00425F40. The four motion-key globals
// selected by 0x004259D0 are mapped through this table before indexing the
// loaded 8x7 vertical-motion table.
inline constexpr std::array<std::int32_t,kMotionKeyCount>
kVerticalProfileByMotionKey{{
    0, 0, 3, 3, 5, 5, 4, 4, 2, 2, 0, 0, 1, 1,
}};

[[nodiscard]] constexpr std::optional<VerticalMotionProfile>
vertical_profile_for_motion_key(std::int32_t key) noexcept {
    if (key < 0 ||
        key >= static_cast<std::int32_t>(kMotionKeyCount)) {
        return std::nullopt;
    }
    return static_cast<VerticalMotionProfile>(
        kVerticalProfileByMotionKey[static_cast<std::size_t>(key)]);
}

[[nodiscard]] constexpr bool loaded_profile_is_live(
    VerticalMotionProfile profile) noexcept {
    const auto value = static_cast<std::size_t>(profile);
    for (const auto selected : kVerticalProfileByMotionKey) {
        if (static_cast<std::size_t>(selected) == value) {
            return true;
        }
    }
    return false;
}

enum class PlacementState : std::int32_t {
    IdleSelect = 0,
    MoveToSelectedCorrect = 1,
    MoveToSelectedDecoy = 2,
    CommitCorrectPlacement = 3,
    CommitDecoyPlacement = 4,
    ReturnHome = 5,
    ReturnAfterDecoy = 6,
    MoveToDecoyReturnPoint = 7,
    ReturnDecoyToConveyor = 8,
};

enum class Difficulty : std::int32_t {
    Easy = 0,
    Medium = 1,
    Hard = 2,
};

struct ConveyorMix {
    std::int32_t correct_items{};
    std::int32_t decoy_items{};

    [[nodiscard]] constexpr std::int32_t total_items() const noexcept {
        return correct_items + decoy_items;
    }
};

[[nodiscard]] constexpr ConveyorMix conveyor_mix(
    Difficulty difficulty) noexcept {
    switch (difficulty) {
        case Difficulty::Easy: return {3, 0};
        case Difficulty::Medium: return {3, 1};
        case Difficulty::Hard: return {2, 2};
    }
    return {0, 0};
}

inline constexpr std::array<std::array<std::int32_t,4>,7>
    kOuterVariantPermutations{{
        {{0,1,2,3}},
        {{0,3,2,1}},
        {{1,3,2,0}},
        {{1,0,2,3}},
        {{3,2,0,1}},
        {{2,1,3,0}},
        {{3,1,2,0}},
    }};

struct PieceIdParts {
    std::int32_t outer_variant{};
    std::int32_t from_connector{};
    std::int32_t to_connector{};
};

[[nodiscard]] constexpr std::int32_t encode_piece_id(
    PieceIdParts parts) noexcept {
    return parts.outer_variant * 9
         + parts.from_connector * 3
         + parts.to_connector;
}

[[nodiscard]] constexpr PieceIdParts decode_piece_id(
    std::int32_t piece_id) noexcept {
    return {
        piece_id / 9,
        (piece_id % 9) / 3,
        piece_id % 3,
    };
}

inline constexpr std::size_t kMaximumLevelCount = 3;
inline constexpr std::size_t kConnectorsPerLevel = 4;
inline constexpr std::size_t kPiecesPerLevel = 3;

using RunPlan = std::array<
    std::array<std::int32_t, kConnectorsPerLevel>,
    kMaximumLevelCount>;

inline constexpr std::array<std::int32_t,3>
kConnectorMotionLookupIndices{{0,16,33}};

inline constexpr std::array<std::int32_t,36>
kMotionKeyLookupA{{
    0,0,0,0,12,9,3,3,12,3,3,3,
    0,12,7,0,0,0,12,0,12,3,3,3,
    12,5,5,5,0,0,0,12,12,0,0,0,
}};

inline constexpr std::array<std::int32_t,36>
kMotionKeyLookupB{{
    1,1,1,13,13,8,1,1,13,2,2,2,
    4,13,6,4,1,1,13,1,13,1,1,1,
    13,4,4,4,4,4,4,13,13,1,1,1,
}};

inline constexpr std::array<std::int32_t,36>
kMotionKeyLookupC{{
    0,0,0,12,12,9,0,0,12,3,3,3,
    5,12,7,5,0,0,12,0,12,0,0,0,
    12,5,5,5,5,5,5,12,12,0,0,0,
}};

inline constexpr std::array<std::int32_t,36>
kMotionKeyLookupD{{
    1,1,1,13,13,8,2,2,13,2,2,2,
    1,13,6,1,1,1,13,1,13,2,2,2,
    13,4,4,4,1,1,1,13,13,1,1,1,
}};

[[nodiscard]] constexpr std::int32_t transpose_piece_id_for_motion_lookup(
    std::int32_t piece_id) noexcept {
    if (piece_id < 0 || piece_id >= 36) {
        return -1;
    }
    return piece_id / 9 + 4 * (piece_id % 9);
}

// This is the peculiar decrement sequence at 0x00425DD0. For the normal
// retail run sections 1..6 it yields placed-piece slots 0,1,1,1,2,2.
// Section 0 produces -1, but its selector branch never uses the read value.
[[nodiscard]] constexpr std::int32_t motion_piece_slot_for_run_section(
    std::int32_t run_section) noexcept {
    auto value = run_section;
    if (value >= 0) --value;
    if (value >= 2) --value;
    if (value >= 2) --value;
    if (value >= 3) --value;
    return value;
}

struct RunAssemblyMotionKeys {
    std::int32_t a{};
    std::int32_t b{};
    std::int32_t c{};
    std::int32_t d{};
    std::int32_t first_lookup_index{};
    std::int32_t other_lookup_index{};
    std::int32_t placed_piece_slot{-1};
};

using PlacedRunPieces = std::array<std::int32_t,12>;

[[nodiscard]] constexpr std::optional<RunAssemblyMotionKeys>
select_run_assembly_motion_keys(
    std::int32_t run_section,
    std::size_t level,
    const RunPlan& connectors,
    const PlacedRunPieces& placed_piece_ids) noexcept {

    if (level >= kMaximumLevelCount || run_section < 0) {
        return std::nullopt;
    }

    auto connector_lookup = [&](std::size_t connector_index)
        constexpr -> std::int32_t {
        const auto connector = connectors[level][connector_index];
        if (connector < 0 || connector >= 3) {
            return -1;
        }
        return kConnectorMotionLookupIndices[
            static_cast<std::size_t>(connector)];
    };

    std::int32_t first_index{};
    std::int32_t other_index{};
    std::int32_t placed_slot = -1;

    if (run_section == 0) {
        const auto index = connector_lookup(0);
        if (index < 0) {
            return std::nullopt;
        }
        first_index = index;
        other_index = index;
    } else {
        // Section 2 explicitly rereads slot zero; the preliminary
        // generic slot calculation in 004259D0 is not its live operand.
        placed_slot = run_section==2?0:
            motion_piece_slot_for_run_section(run_section);
        if (placed_slot < 0 || placed_slot >= 4) {
            return std::nullopt;
        }

        const auto storage_index =
            level * 4 + static_cast<std::size_t>(placed_slot);
        const auto piece_id = placed_piece_ids[storage_index];
        const auto transposed =
            transpose_piece_id_for_motion_lookup(piece_id);
        if (transposed < 0) {
            return std::nullopt;
        }

        switch (run_section) {
        case 1:
            first_index = connector_lookup(0);
            other_index = transposed;
            break;
        case 2:
            first_index = transposed;
            other_index = connector_lookup(1);
            break;
        case 3:
        case 5:
            first_index = connector_lookup(2);
            other_index = transposed;
            break;
        case 4:
            first_index = transposed;
            other_index = connector_lookup(2);
            break;
        case 6:
            first_index = transposed;
            other_index = connector_lookup(3);
            break;
        default:
            // The retained default branch indexes all four lookup tables by
            // the raw piece ID rather than the transposed piece index.
            first_index = piece_id;
            other_index = piece_id;
            break;
        }

        if (first_index < 0 || other_index < 0 ||
            first_index >= 36 || other_index >= 36) {
            return std::nullopt;
        }
    }

    return RunAssemblyMotionKeys{
        kMotionKeyLookupA[static_cast<std::size_t>(first_index)],
        kMotionKeyLookupB[static_cast<std::size_t>(other_index)],
        kMotionKeyLookupC[static_cast<std::size_t>(other_index)],
        kMotionKeyLookupD[static_cast<std::size_t>(other_index)],
        first_index,
        other_index,
        placed_slot,
    };
}

enum class RunAssemblyAnimationState : std::int32_t {
    Idle = 0,
    FirstThreeSubsteps = 1,
    TransitionToSecondPhase = 2,
    LastFourSubsteps = 3,
    SecondFirstThreeSubsteps = 4,
    RetainedTransition = 5,
    SecondLastFourSubsteps = 6,
};

struct RunAssemblyAnimationRuntime {
    RunAssemblyAnimationState state{RunAssemblyAnimationState::Idle};
    std::int32_t run_section{1};
    std::int32_t delay_remaining{};
    std::int32_t substep{};
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t sprite_mode{};
    std::int32_t sprite_frame{};
    RunAssemblyMotionKeys motion_keys{};
};

// Initialized PE table at 004467E8: fourteen pairs of atlas indices.
// 004266C2 loads the FIRST word of each pair and adds the substep.
inline constexpr std::array<int,14> kAssemblySpriteBases{
    0,3,7,10,13,16,19,22,25,28,31,48,63,66
};
[[nodiscard]] constexpr int active_assembly_sprite_frame(
    const RunAssemblyAnimationRuntime& s) noexcept {
    const std::array<int,4> keys{s.motion_keys.a,s.motion_keys.b,
                                s.motion_keys.c,s.motion_keys.d};
    if(s.sprite_mode<0 || s.sprite_mode>=4)return -1;
    const int key=keys[static_cast<std::size_t>(s.sprite_mode)];
    if(key<0||key>=14||s.substep<0||s.substep>3)return -1;
    return kAssemblySpriteBases[static_cast<std::size_t>(key)]+s.substep;
}
[[nodiscard]] constexpr bool special_assembly_idle_bank(int key) noexcept {
    return key==3||key==4||key==13;
}
template<class NextRand>
constexpr int tick_assembly_idle_sprite(RunAssemblyAnimationRuntime& s,
    int delay,NextRand&& next_rand) noexcept {
    if(--s.delay_remaining<1) {
        s.delay_remaining=delay;
        ++s.sprite_frame;
        s.substep=0;
        if(special_assembly_idle_bank(s.motion_keys.d)) {
            // Inclusive 86 is original; no corresponding BMP cell exists.
            if(s.sprite_frame<72||s.sprite_frame>86)s.sprite_frame=72;
        } else {
            if(s.sprite_frame<31)s.sprite_frame=31;
            if(s.sprite_frame>62)s.sprite_frame=(next_rand()&3)==0?31:48;
        }
    }
    return s.sprite_frame+s.substep;
}
[[nodiscard]] constexpr bool assembly_sprite_visible(
    const RunAssemblyAnimationRuntime& s,int frame,
    std::size_t level,const PlacedRunPieces& pieces) noexcept {
    if(frame<0 || frame>=86)return false;
    // 004266EA..00426735: odd sections suppress the sprite for raw
    // piece ID 1. Keep the original storage-index arithmetic bounded.
    const int section=s.run_section;
    if(section!=0&&section!=2&&section!=4&&section!=6) {
        int slot=section;
        if(slot>=1)--slot;
        if(slot>=3)--slot;
        if(slot>=0&&level<kMaximumLevelCount) {
            const auto index=level*4+static_cast<std::size_t>(slot);
            if(index<pieces.size()&&pieces[index]==1)return false;
        }
    }
    const int phase=static_cast<int>(s.state);
    return !(phase>=4&&phase<=7&&frame>=63);
}

struct RunAssemblyAnimationTick {
    bool applied_motion{};
    Vec2i delta{};
    std::optional<VerticalMotionProfile> profile{};
    std::int32_t absolute_motion_substep{-1};
    bool entered_transient_state_2{};
    bool cycle_completed{};
};

constexpr void begin_run_assembly_animation(
    RunAssemblyAnimationRuntime& runtime,
    const Data& data,
    RunAssemblyMotionKeys keys) noexcept {

    runtime.state = RunAssemblyAnimationState::FirstThreeSubsteps;
    runtime.delay_remaining = data.animation_step_delay;
    runtime.substep = 0;
    runtime.sprite_mode = 0;
    runtime.sprite_frame = 0;
    runtime.motion_keys = keys;
}

// Exact positional state machine inside 0x00425F40. Motion is applied once,
// when delay_remaining equals the loaded delay. Each substep is then held while
// the delay counts down. Phase boundaries deliberately fall through and apply
// the next phase's first motion delta on the same update that ends the previous
// phase.
[[nodiscard]] constexpr RunAssemblyAnimationTick
tick_run_assembly_animation(
    RunAssemblyAnimationRuntime& runtime,
    const Data& data) noexcept {

    RunAssemblyAnimationTick result;

    auto apply = [&](std::int32_t key, std::int32_t absolute_substep)
        constexpr {
        const auto profile = vertical_profile_for_motion_key(key);
        if (!profile ||
            absolute_substep < 0 ||
            absolute_substep >=
                static_cast<std::int32_t>(kMotionSubstepCount)) {
            return;
        }

        const auto delta = motion_delta(
            data,
            *profile,
            static_cast<std::size_t>(absolute_substep));
        runtime.x += delta.x;
        runtime.y += delta.y;

        result.applied_motion = true;
        result.delta = delta;
        result.profile = profile;
        result.absolute_motion_substep = absolute_substep;
    };

    // Retail state 2 exists only as an in-function transition. If entered
    // externally, it normalizes to state 3 and immediately starts substep 3.
    if (runtime.state ==
        RunAssemblyAnimationState::TransitionToSecondPhase) {
        result.entered_transient_state_2 = true;
        runtime.state = RunAssemblyAnimationState::LastFourSubsteps;
        runtime.delay_remaining = data.animation_step_delay;
        runtime.substep = 0;
        runtime.sprite_frame = 3;
    }

    // State 5 is similarly a retained bridge into state 6.
    if (runtime.state ==
        RunAssemblyAnimationState::RetainedTransition) {
        runtime.state =
            RunAssemblyAnimationState::SecondLastFourSubsteps;
        runtime.delay_remaining = data.animation_step_delay;
        runtime.substep = 0;
        runtime.sprite_frame = 3;
    }

    while (true) {
        std::int32_t key = -1;
        std::int32_t absolute_substep = -1;
        std::int32_t phase_length = 0;

        switch (runtime.state) {
        case RunAssemblyAnimationState::FirstThreeSubsteps:
            key = runtime.motion_keys.a;
            absolute_substep = runtime.substep;
            phase_length = 3;
            break;

        case RunAssemblyAnimationState::LastFourSubsteps:
            key = runtime.motion_keys.b;
            absolute_substep = runtime.substep + 3;
            phase_length = 4;
            runtime.sprite_mode = 1;
            break;

        case RunAssemblyAnimationState::SecondFirstThreeSubsteps:
            key = runtime.motion_keys.c;
            absolute_substep = runtime.substep;
            phase_length = 3;
            break;

        case RunAssemblyAnimationState::SecondLastFourSubsteps:
            key = runtime.motion_keys.d;
            absolute_substep = runtime.substep + 3;
            phase_length = 4;
            runtime.sprite_mode = 3;
            break;

        case RunAssemblyAnimationState::Idle:
        case RunAssemblyAnimationState::TransitionToSecondPhase:
        case RunAssemblyAnimationState::RetainedTransition:
            return result;
        }

        if (runtime.delay_remaining == data.animation_step_delay) {
            apply(key, absolute_substep);
        }

        --runtime.delay_remaining;
        if (runtime.delay_remaining > 0) {
            return result;
        }

        runtime.delay_remaining = data.animation_step_delay;
        ++runtime.substep;

        if (runtime.substep < phase_length) {
            return result;
        }

        switch (runtime.state) {
        case RunAssemblyAnimationState::FirstThreeSubsteps:
            // Retail briefly writes state 2, immediately calls its transition
            // helper, then continues as state 3 in this same update.
            result.entered_transient_state_2 = true;
            runtime.state =
                RunAssemblyAnimationState::LastFourSubsteps;
            runtime.substep = 0;
            runtime.sprite_frame = 3;
            continue;

        case RunAssemblyAnimationState::LastFourSubsteps:
            runtime.state =
                RunAssemblyAnimationState::SecondFirstThreeSubsteps;
            runtime.substep = 0;
            runtime.sprite_mode = 2;
            runtime.sprite_frame = 0;
            continue;

        case RunAssemblyAnimationState::SecondFirstThreeSubsteps:
            runtime.state =
                RunAssemblyAnimationState::SecondLastFourSubsteps;
            runtime.substep = 0;
            runtime.sprite_frame = 3;
            continue;

        case RunAssemblyAnimationState::SecondLastFourSubsteps:
            // The binary increments to substep 4, then decrements it back to 3
            // while returning to state 0 and advancing the run section.
            runtime.substep = 3;
            runtime.state = RunAssemblyAnimationState::Idle;
            ++runtime.run_section;
            result.cycle_completed = true;
            return result;

        case RunAssemblyAnimationState::Idle:
        case RunAssemblyAnimationState::TransitionToSecondPhase:
        case RunAssemblyAnimationState::RetainedTransition:
            return result;
        }
    }
}

[[nodiscard]] constexpr RunPlan chain_run_plan(
    RunPlan random_draws) noexcept {
    // Retail generates all 12 rand()%3 values first, then overwrites the
    // first connector of levels 1 and 2 with the preceding level's end.
    random_draws[1][0] = random_draws[0][3];
    random_draws[2][0] = random_draws[1][3];
    return random_draws;
}

struct ConnectorPair {
    std::int32_t from{};
    std::int32_t to{};
    friend bool operator==(const ConnectorPair&, const ConnectorPair&) = default;
};

[[nodiscard]] constexpr ConnectorPair required_connectors(
    const RunPlan& plan,
    std::size_t level,
    std::size_t piece_index) noexcept {
    return {
        plan[level][piece_index],
        plan[level][piece_index + 1],
    };
}

struct ConveyorChoice {
    int piece_id{-1};
    bool correct{};
    bool active{};
    int feedback_comparison{};
    int feedback_to{};
};
struct ConveyorChoicesResult {
    std::array<ConveyorChoice,4> choices{};
    int count{};
    int variant_index{};
    std::optional<int> landing_connector;
};
// Original 00426F30: all random draws are made on the shared CRT stream.
// A missing destination (-1) uses the original random correct-to branch.
[[nodiscard]] constexpr std::optional<ConveyorChoicesResult> generate_conveyor_choices(
    Difficulty difficulty,ConnectorPair required,retail::OriginalRetailRandom& rng) noexcept {
    if(required.from<0||required.from>2||required.to< -1||required.to>2)
        return std::nullopt;
    const auto mix=conveyor_mix(difficulty);
    if(mix.total_items()==0)return std::nullopt;
    ConveyorChoicesResult out;
    out.count=mix.total_items();
    if(required.to>=0)out.landing_connector=required.to;
    std::array<ConnectorPair,4> corrects{},decoys{};
    for(int i=0;i<mix.correct_items;++i)
        corrects[i]={required.from,required.to<0?rng.next_rand()%3:required.to};
    for(int i=0;i<mix.decoy_items;++i) {
        auto& p=decoys[i];
        do {p.from=rng.next_rand()%3;}
        while(p.from==required.from||(i>0&&p.from==decoys[0].from));
        do {p.to=rng.next_rand()%3;}while(p.to==required.to);
    }
    out.variant_index=rng.next_rand()%7;
    const auto& variant=kOuterVariantPermutations[out.variant_index];
    int correct_idx=0,decoy_idx=0;
    for(int i=0;i<out.count;++i) {
        bool correct=false,selected=false;
        for(int attempt=0;attempt<4096;++attempt) {
            correct=(rng.next_rand()&1)==0;
            if(correct?correct_idx<mix.correct_items:decoy_idx<mix.decoy_items) {
                selected=true;break;
            }
        }
        if(!selected)return std::nullopt;
        const auto& p=correct?corrects[correct_idx++]:decoys[decoy_idx++];
        out.choices[i]={encode_piece_id({variant[i],p.from,p.to}),correct,true,
            correct?p.from:p.from*3+p.to,p.to};
    }
    return out;
}

[[nodiscard]] constexpr bool is_correct_connector_pair(
    ConnectorPair offered,
    ConnectorPair required) noexcept {
    return offered == required;
}

[[nodiscard]] constexpr bool is_retail_decoy_pair(
    ConnectorPair offered,
    ConnectorPair required) noexcept {
    // GenerateSquirrelConveyorChoices explicitly retries each random connector
    // until both dimensions differ from the required pair.
    return offered.from != required.from &&
           offered.to != required.to;
}

[[nodiscard]] constexpr std::int32_t level_count(
    Difficulty difficulty) noexcept {
    return static_cast<std::int32_t>(difficulty) + 1;
}

[[nodiscard]] constexpr std::int32_t required_correct_placements(
    Difficulty difficulty) noexcept {
    return level_count(difficulty) *
           static_cast<std::int32_t>(kPiecesPerLevel);
}

inline constexpr std::array<Vec2i,4> kConveyorItemOrigins{{
    {100,300},
    {189,300},
    {370,300},
    {459,300},
}};

inline constexpr std::int32_t kConveyorHitWidth = 89;
inline constexpr std::int32_t kConveyorHitHeight = 133;

inline constexpr std::array<Vec2i,kPiecesPerLevel> kRunPlacementTargets{{
    {116,327},
    {276,327},
    {436,327},
}};

inline constexpr Vec2i kLoftyHomeTarget{93,185};

[[nodiscard]] constexpr bool run_reached_level_end(
    std::int32_t run_section) noexcept {
    return run_section > 6;
}

[[nodiscard]] constexpr bool should_finish_activity(
    Difficulty difficulty,
    std::int32_t level_index,
    std::int32_t run_section) noexcept {
    return run_reached_level_end(run_section) &&
           level_index >= static_cast<std::int32_t>(difficulty);
}

[[nodiscard]] constexpr bool should_advance_level(
    Difficulty difficulty,
    std::int32_t level_index,
    std::int32_t run_section) noexcept {
    return run_reached_level_end(run_section) &&
           level_index < static_cast<std::int32_t>(difficulty);
}

[[nodiscard]] constexpr std::int32_t level_finished_feedback_sound_id(
    std::int32_t random_mod_4) noexcept {
    return random_mod_4 >= 0 && random_mod_4 < 4
        ? 664 + random_mod_4 // SR_WEN_22..25
        : -1;
}

struct MotionStep {
    Vec2i position{};
    bool reached{};
};

// Exact 0x00427270: move each axis by 2 first, then snap when the
// post-step absolute delta is less than 4 (initial delta <=5).
[[nodiscard]] constexpr MotionStep step_toward_target(
    Vec2i current,
    Vec2i target) noexcept {

    auto step_axis = [](std::int32_t value, std::int32_t goal) constexpr {
        if(value<goal)value+=2;
        else if(value>goal)value-=2;
        const auto delta=goal-value;
        return (delta>=-3 && delta<=3)?goal:value;
    };

    current.x = step_axis(current.x, target.x);
    current.y = step_axis(current.y, target.y);

    return {
        current,
        current.x == target.x && current.y == target.y,
    };
}

// 004273F0: actor motion and carrying are independent of the squirrel's
// run animation. Selection, drop and return are explicit frame boundaries.
enum class PlacementEvent { None, PickedUp, CorrectDropped, DecoyDropped,
                            DecoyRetrieved, DecoyReturned, Home };
struct PlacementMotion {
    PlacementState state{PlacementState::IdleSelect};
    Vec2i position{kLoftyHomeTarget};
    int selected{-1};
    bool carrying{};
};
[[nodiscard]] constexpr bool select_placement(
    PlacementMotion& actor,int index,bool correct) noexcept {
    if(actor.state!=PlacementState::IdleSelect || index<0 || index>=4)return false;
    actor.selected=index;
    actor.carrying=false;
    actor.state=correct?PlacementState::MoveToSelectedCorrect:PlacementState::MoveToSelectedDecoy;
    return true;
}
[[nodiscard]] constexpr PlacementEvent advance_placement_motion(
    PlacementMotion& actor,int piece_slot) noexcept {
    if(actor.state==PlacementState::IdleSelect)return PlacementEvent::None;
    if(actor.selected<0 || actor.selected>=4 || piece_slot<0 || piece_slot>=3)
        return PlacementEvent::None;
    const auto origin=kConveyorItemOrigins[static_cast<std::size_t>(actor.selected)];
    const Vec2i pick{origin.x,origin.y+kConveyorHitHeight};
    const auto drop=kRunPlacementTargets[static_cast<std::size_t>(piece_slot)];
    Vec2i destination=kLoftyHomeTarget;
    switch(actor.state) {
    case PlacementState::MoveToSelectedCorrect:
    case PlacementState::MoveToSelectedDecoy:
    case PlacementState::ReturnDecoyToConveyor:destination=pick;break;
    case PlacementState::CommitCorrectPlacement:
    case PlacementState::CommitDecoyPlacement:
    case PlacementState::MoveToDecoyReturnPoint:destination=drop;break;
    default:break;
    }
    const auto step=step_toward_target(actor.position,destination);
    actor.position=step.position;
    if(!step.reached)return PlacementEvent::None;
    switch(actor.state) {
    case PlacementState::MoveToSelectedCorrect:
        actor.state=PlacementState::CommitCorrectPlacement;actor.carrying=true;
        return PlacementEvent::PickedUp;
    case PlacementState::MoveToSelectedDecoy:
        actor.state=PlacementState::CommitDecoyPlacement;actor.carrying=true;
        return PlacementEvent::PickedUp;
    case PlacementState::CommitCorrectPlacement:
        actor.state=PlacementState::ReturnHome;actor.carrying=false;
        return PlacementEvent::CorrectDropped;
    case PlacementState::CommitDecoyPlacement:
        actor.state=PlacementState::ReturnAfterDecoy;actor.carrying=false;
        return PlacementEvent::DecoyDropped;
    case PlacementState::ReturnAfterDecoy:
        actor.state=PlacementState::MoveToDecoyReturnPoint;
        return PlacementEvent::None;
    case PlacementState::MoveToDecoyReturnPoint:
        actor.state=PlacementState::ReturnDecoyToConveyor;actor.carrying=true;
        return PlacementEvent::DecoyRetrieved;
    case PlacementState::ReturnDecoyToConveyor:
        actor.state=PlacementState::ReturnHome;actor.carrying=false;
        return PlacementEvent::DecoyReturned;
    case PlacementState::ReturnHome:
        actor.state=PlacementState::IdleSelect;actor.selected=-1;
        return PlacementEvent::Home;
    default:return PlacementEvent::None;
    }
}

// 004273F0 starts the mouth animation only on Lofty's own feedback
// IDs (633..638), with two original CRT draws. 004267B0 observes the
// specific active sound, not the aggregate managed sound bank.
struct LoftySpeechAnimation {
    int sound_id{-1};
    int mouth_frame{-1};
    int countdown{};
    constexpr void begin(int id,retail::OriginalRetailRandom& random) noexcept {
        if(id<633 || id>638)return;
        mouth_frame=random.next_rand()%3;
        countdown=random.next_rand()%10+10;
        sound_id=id;
    }
    constexpr void advance(bool specific_sound_playing,
                           retail::OriginalRetailRandom& random) noexcept {
        if(sound_id==-1)return;
        if(!specific_sound_playing)sound_id=-1;
        // Retail still advances the countdown on the frame that clears
        // the sound ID; a replacement frame can be selected on that tick.
        if(--countdown<1) {
            countdown=random.next_rand()%10+10;
            const int prior=mouth_frame;
            do{mouth_frame=random.next_rand()%3;}while(mouth_frame==prior);
        }
    }
};

enum class CompletionAction : std::int32_t {
    None,
    PlayFinalWendyLine,
    ExitToPlayAgain,
};

struct CompletionStep {
    std::int32_t stage{};
    CompletionAction action{CompletionAction::None};
    std::int32_t sound_id{-1};
};

// Global 0x515058 in the complete path of UpdateSquirrelActivity:
// stage 0 stops managed sound and plays SR_WEN_23..25,
// stage 1 waits for managed audio to finish,
// stage 2 exits through the common completion/Play Again path.
[[nodiscard]] constexpr CompletionStep completion_step(
    std::int32_t stage,
    bool any_managed_sound_playing,
    std::int32_t random_mod_3) noexcept {

    if (stage == 0) {
        if (random_mod_3 < 0 || random_mod_3 > 2) {
            return {0, CompletionAction::None, -1};
        }
        return {
            1,
            CompletionAction::PlayFinalWendyLine,
            665 + random_mod_3,
        };
    }

    if (stage == 1) {
        if (any_managed_sound_playing) {
            return {1, CompletionAction::None, -1};
        }
        return {2, CompletionAction::None, -1};
    }

    if (stage >= 2) {
        return {stage, CompletionAction::ExitToPlayAgain, -1};
    }

    return {stage, CompletionAction::None, -1};
}

inline constexpr std::int32_t kStartupWendySoundId = 643; // SR_WEN_01
inline constexpr std::int32_t kStartupLoftySoundId = 632; // SR_LOF_01
inline constexpr std::int32_t kStartupWendyFollowupSoundId = 644; // SR_WEN_02

inline constexpr std::array<std::int32_t,4> kPrimaryPlacementLoftyFeedback{
    633, 634, 635, 636 // SR_LOF_02..05
};

inline constexpr std::array<std::int32_t,3> kPrimaryPlacementWendyFeedback{
    656, 657, 658 // SR_WEN_14..16
};

inline constexpr std::array<std::int32_t,2> kAlternatePlacementLoftyFeedback{
    637, 638 // SR_LOF_06..07
};

inline constexpr std::array<std::int32_t,7> kAlternatePlacementWendyFeedback{
    645, 646, 648, 649, 650, 651, 652
    // SR_WEN_03,04,06,07,08,09,10
};

inline constexpr std::array<std::int32_t,3> kFinalWendyFeedback{
    665, 666, 667 // SR_WEN_23..25
};

[[nodiscard]] constexpr std::int32_t correct_placement_feedback_sound_id(
    std::int32_t random_mod_7) noexcept {
    if (random_mod_7 < 0 || random_mod_7 > 6) return -1;
    return random_mod_7 < 4
        ? 633 + random_mod_7
        : 652 + random_mod_7;
}

[[nodiscard]] constexpr std::int32_t decoy_lofty_feedback_sound_id(
    std::int32_t random_bit) noexcept {
    return random_bit >= 0 && random_bit < 2
        ? 637 + random_bit
        : -1;
}

// This models the non-1-in-5 branch of the retail decoy feedback selector.
// Generated decoys normally have offered_from != required_from. The equality
// path is retained because the executable still contains it.
[[nodiscard]] constexpr std::int32_t decoy_direction_feedback_sound_id(
    std::int32_t required_from,
    std::int32_t offered_from,
    std::int32_t random_index) noexcept {

    if (required_from < offered_from) {
        return random_index >= 0 && random_index < 3
            ? 650 + random_index // SR_WEN_08..10
            : -1;
    }

    if (required_from > offered_from) {
        constexpr std::array<std::int32_t,4> ids{
            645, 646, 648, 649 // SR_WEN_03,04,06,07
        };
        return random_index >= 0 && random_index < 4
            ? ids[static_cast<std::size_t>(random_index)]
            : -1;
    }

    return decoy_lofty_feedback_sound_id(random_index);
}

} // namespace btb::squirrel
