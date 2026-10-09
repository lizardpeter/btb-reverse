#pragma once

#include "btb/herding_source_temporary_target.hpp"
#include "btb/herding_source_follower_approach.hpp"
#include "btb/herding_source_tracked_targets.hpp"
#include "btb/retail_crt_random.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>

namespace btb::herding {

// SOURCE-SCOPED coordinator for verified subsequences of
// UpdateHerdingAnimal. It is not a substitute for the unrecovered
// free-roam/ordinary follower movement paths. Never treat Uncovered
// as successful simulation.
//
// Original dispatch order for the covered state-zero path:
// 0x416F49: behavior state must still be zero
// 0x416F57: positive temporary-target timer takes precedence
// 0x41721E: tracked-target table 0x50AF78 checked FIRST
// 0x417239: otherwise consult ordinary follower table 0x50AF14
// 0x417251: ordinary follower may enter tracked table at <150
// 0x4172B0: existing tracked animal steers/arrives at <30
//
// The externally provided species point is the ORIGINAL per-type
// integer point at 0x50B2F4/0x50B2F8. No fabricated destination is
// selected here.
enum class CoveredAnimalBranch {
    InactiveBehaviorState,
    TemporaryTargetMoving,
    TemporaryTargetCleared,
    RegisteredTrackedTarget,
    TrackedTargetApproaching,
    TrackedTargetArrived,
    UncoveredOriginalBranch,
    InvalidSourceState
};

struct CoveredAnimalStep {
    CoveredAnimalBranch branch{CoveredAnimalBranch::UncoveredOriginalBranch};
    RetailEntityRecord32 entity{};
    std::uint32_t rand_state_before{};
    std::uint32_t rand_state_after{};
    int rand_calls{};
    int remaining_to_register{};
    bool last_target_registered{};
    bool covered() const noexcept {
        return branch!=CoveredAnimalBranch::UncoveredOriginalBranch &&
               branch!=CoveredAnimalBranch::InvalidSourceState;
    }
};

// Original tracked-target array, original follower list and global
// remaining-to-register counter stay distinct. The caller owns one
// process-global PRNG shared with ALL activities.
class CoveredAnimalMotionDispatcher {
public:
    explicit CoveredAnimalMotionDispatcher(
        int original_remaining_to_register)
        : remaining_to_register_(original_remaining_to_register) {}

    [[nodiscard]] const RetailTrackedTargetSlots& tracked() const noexcept {
        return tracked_;
    }
    [[nodiscard]] int remaining_to_register() const noexcept {
        return remaining_to_register_;
    }

    [[nodiscard]] CoveredAnimalStep advance(
        std::int32_t original_entity_index,
        const RetailEntityRecord32& entity,
        const FollowerList& ordinary_followers,
        Vec2i source_species_point,
        retail::OriginalRetailRandom& shared_random) noexcept {

        CoveredAnimalStep result{};
        result.entity=entity;
        result.rand_state_before=shared_random.state();
        result.rand_state_after=result.rand_state_before;
        result.remaining_to_register=remaining_to_register_;

        if (original_entity_index<0 ||
            entity.entity_id!=original_entity_index ||
            !is_herd_animal(entity.entity_type())) {
            result.branch=CoveredAnimalBranch::InvalidSourceState;
            return result;
        }

        if (entity.behavior_state!=0) {
            // State 1 and both home-route state groups are handled
            // by the independent original state jump table.
            result.branch=CoveredAnimalBranch::InactiveBehaviorState;
            return result;
        }

        if (entity.temporary_target_timer>0) {
            const auto temporary=
                original_herding_temporary_target_update(entity);
            if (!temporary || !temporary->branch_taken) {
                result.branch=CoveredAnimalBranch::InvalidSourceState;
                return result;
            }
            result.entity=temporary->moved;
            result.branch=temporary->early_return
                ? CoveredAnimalBranch::TemporaryTargetCleared
                : CoveredAnimalBranch::TemporaryTargetMoving;
            // Both outcomes terminate original animal update.
            return result;
        }

        const auto membership=tracked_.route_for(
            original_entity_index,ordinary_followers);
        if (membership==RetailTrackedRoute::TrackedTargetApproach) {
            const auto approached=original_herding_follower_approach(
                entity,source_species_point,shared_random);
            if (!approached) {
                result.branch=CoveredAnimalBranch::InvalidSourceState;
                return result;
            }
            result.entity=approached->updated;
            result.rand_calls=approached->source_rand_calls;
            result.rand_state_after=shared_random.state();
            if (approached->remove_from_follower_table) {
                if (!tracked_.remove(original_entity_index)) {
                    result.branch=CoveredAnimalBranch::InvalidSourceState;
                    return result;
                }
                // Retail writes entity behavior_state=1 at 0x417314,
                // while ordinary follower list membership is unchanged.
                result.branch=CoveredAnimalBranch::TrackedTargetArrived;
            } else {
                result.branch=CoveredAnimalBranch::TrackedTargetApproaching;
            }
            return result;
        }

        if (membership==RetailTrackedRoute::OrdinaryFollower) {
            // The native ordinary-follower branch below the threshold
            // is not reconstructed here. We only execute the original
            // <150 registration path if its real point and follower
            // list make that branch source-reachable.
            auto admission=tracked_.admit_from_follower(
                original_entity_index,
                {entity.x,entity.y},source_species_point,
                ordinary_followers,remaining_to_register_);
            if (admission.status==
                RetailTrackedAdmissionStatus::Inserted) {
                result.branch=CoveredAnimalBranch::RegisteredTrackedTarget;
                result.remaining_to_register=remaining_to_register_;
                result.last_target_registered=
                    admission.reached_registration_target;
            } else if (admission.status==
                    RetailTrackedAdmissionStatus::InvalidInput) {
                result.branch=CoveredAnimalBranch::InvalidSourceState;
            }
            return result;
        }

        // Truly untracked animals need the still-incomplete source
        // random-target/free-roam and collision dispatch.
        return result;
    }

private:
    RetailTrackedTargetSlots tracked_{};
    int remaining_to_register_{};
};

} // namespace btb::herding
